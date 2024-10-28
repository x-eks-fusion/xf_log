/**
 * @file xf_log.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-09
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_log.h"
#include <stdarg.h>

/* ==================== [Defines] =========================================== */

#define PL_CSI_START                "\033["
#define PL_CSI_END                  "\033[0m"

#if XF_LOG_CTYPE_IS_ENABLE
    #include <ctype.h>
#else
    #define isdigit(c) ((c) >= '0' && (c) <= '9')
#endif

#if XF_LOG_STRLEN_IS_ENABLE
    #include <string.h>
    #define xf_log_strlen(s) strlen(s)
#endif

#if XF_LOG_VSNPRINTF_IS_ENABLE
    #include <stdio.h>
    #define xf_log_vsprintf(buffer, maxlen, fmt, args) vsnprintf(buffer, maxlen, fmt, args)
#endif

/* ==================== [Typedefs] ========================================== */

typedef enum _xf_log_color_t {
    XF_LOG_COLOR_NULL       = 0x00,
    XF_LOG_COLOR_BLACK      = '0',
    XF_LOG_COLOR_RED,
    XF_LOG_COLOR_GREEN,
    XF_LOG_COLOR_YELLOW,
    XF_LOG_COLOR_BLUE,
    XF_LOG_COLOR_MAGENTA,
    XF_LOG_COLOR_CYAN,
    XF_LOG_COLOR_WHITE,

    XF_LOG_COLOR_MAX,
} xf_log_color_t;

#if XF_LOG_FILTER_IS_ENABLE

typedef struct _xf_log_filter_t {
    uint8_t enable;
    uint8_t b_or_w; // 黑白名单，0表示黑名单表示加入后不会被输出，1表示白名单表示加入后会被输出
    uint8_t level;
    uint8_t is_colorful;
    const char *tag;
    const char *file;
} xf_log_filter_t;

#endif

typedef struct _xf_log_obj_t {
    uint8_t info_level;
    xf_log_out_t out_func;
    void *user_args;

#if XF_LOG_FILTER_IS_ENABLE

    xf_log_filter_t filter;

#endif

} xf_log_obj_t;

/* ==================== [Static Prototypes] ================================= */

static size_t xf_log_vprintf(xf_log_out_t log_out, void *arg, const char *format, va_list va);
static size_t xf_log_color_format(int log_obj_id, uint8_t level, const char *tag, const char *file, uint32_t line,
                                  const char *func, const char *fmt, va_list va);

/* ==================== [Static Variables] ================================== */

static const char s_lvl_to_prompt[] = {
    '\0',
    'U',
    'E',
    'W',
    'I',
    'D',
    'V',
};

#if XF_LOG_COLORS_IS_ENABLE
static const uint8_t s_lvl_to_color[] = {
    XF_LOG_COLOR_NULL,
    XF_LOG_COLOR_BLUE,
    XF_LOG_COLOR_RED,
    XF_LOG_COLOR_YELLOW,
    XF_LOG_COLOR_GREEN,
    XF_LOG_COLOR_CYAN,
    XF_LOG_COLOR_NULL,
};
#endif

static xf_log_obj_t s_log_obj[XF_LOG_OBJ_NUM] = {0};

static xf_log_time_func_t s_log_time_func = NULL;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

int xf_log_register_obj(xf_log_out_t out_func, void *user_args)
{
    for (size_t i = 0; i < XF_LOG_OBJ_NUM; i++) {
        if (s_log_obj[i].out_func != NULL) {
            continue;
        }
        s_log_obj[i].out_func = out_func;
        s_log_obj[i].user_args = user_args;
        s_log_obj[i].info_level = XF_LOG_LVL_ERROR;     // 当log等级大于等于XF_LOG_LVL_ERROR时才会输出相关信息

#if XF_LOG_FILTER_IS_ENABLE

        s_log_obj[i].filter.enable = 0;                 // 不开启过滤器
        s_log_obj[i].filter.b_or_w = 0;                 // 默认黑名单
        s_log_obj[i].filter.level = XF_LOG_LVL_VERBOSE; // 不对等级做任何屏蔽
        s_log_obj[i].filter.is_colorful = 1;            // 不对颜色做任何屏蔽
        s_log_obj[i].filter.tag = NULL;                 // 不对 tag 进行任何屏蔽
        s_log_obj[i].filter.file = NULL;                // 不对 file 进行任何屏蔽

#endif
        return i;
    }

    return -1;
}

#if XF_LOG_FILTER_IS_ENABLE

void xf_log_set_filter_enable(int log_obj_id)
{
    s_log_obj[log_obj_id].filter.enable = 1;
}

void xf_log_set_filter_disable(int log_obj_id)
{
    s_log_obj[log_obj_id].filter.enable = 0;
}

void xf_log_set_filter_colorful_enable(int log_obj_id)
{
    s_log_obj[log_obj_id].filter.is_colorful = 1;
}

void xf_log_set_filter_colorful_disable(int log_obj_id)
{
    s_log_obj[log_obj_id].filter.is_colorful = 0;
}

void xf_log_set_filter_is_blacklist(int log_obj_id)
{
    s_log_obj[log_obj_id].filter.b_or_w = 0;
}

void xf_log_set_filter_is_whitelist(int log_obj_id)
{
    s_log_obj[log_obj_id].filter.b_or_w = 1;
}

void xf_log_set_filter_tag(int log_obj_id, const char *tag)
{
    s_log_obj[log_obj_id].filter.tag = tag;
}

void xf_log_set_filter_level(int log_obj_id, uint8_t level)
{
    s_log_obj[log_obj_id].filter.level = level;
}

void xf_log_set_filter_file(int log_obj_id, const char *file)
{
    s_log_obj[log_obj_id].filter.file = file;
}

#endif

void xf_log_set_info_level(int log_obj_id, uint8_t level)
{
    s_log_obj[log_obj_id].info_level = level;
}

void xf_log_set_time_func(xf_log_time_func_t log_time_func)
{
    s_log_time_func = log_time_func;
}

size_t xf_log(uint8_t level, const char *tag, const char *file, uint32_t line, const char *func, const char *fmt, ...)
{
    size_t len = 0;
    // 根据不同的订阅进行不同的输出
    va_list args;
    va_start(args, fmt);
    for (size_t i = 0; i < XF_LOG_OBJ_NUM; i++) {
        if (s_log_obj[i].out_func == NULL) {
            continue;
        }
#if XF_LOG_FILTER_IS_ENABLE
        // 根据屏蔽等级判断后续是否执行
        xf_log_filter_t filter = s_log_obj[i].filter;
        if (filter.enable) {
            if (filter.b_or_w == 0) {
                if (filter.level < level) {
                    continue;
                } else if (filter.tag != NULL && filter.tag == tag) {
                    continue;
                } else if (filter.file != NULL && filter.file == file) {
                    continue;
                }

            } else if (filter.b_or_w == 1) {
                if (filter.level < level) {
                    continue;
                } else if (filter.tag != NULL && filter.tag != tag) {
                    continue;
                } else if (filter.file != NULL && filter.file != file) {
                    continue;
                }
            }
        }
#endif
        len = xf_log_color_format(i, level, tag, file, line, func, fmt, args);
    }
    va_end(args);

    return len;
}

size_t xf_log_printf(const char *format, ...)
{
    size_t len = 0;
    va_list args;
    va_start(args, format);
    for (size_t i = 0; i < XF_LOG_OBJ_NUM; i++) {
        if (s_log_obj[i].out_func == NULL) {
            continue;
        }
        len = xf_log_vprintf(s_log_obj[i].out_func, s_log_obj[i].user_args, format, args);
    }
    va_end(args);

    return len;
}

/* ==================== [Static Functions] ================================== */

static void find_args_from_index(va_list *va, size_t index)
{
    for (int i = 0; i < index; i++) {
        va_arg(*va, void *);
    }
}

static size_t xf_log_vprintf(xf_log_out_t log_out, void *arg, const char *format, va_list va)
{
    const char *p = format;
    char format_flag[XF_FORMAT_FLAG_SIZE];         // 用于格式化部分的缓冲区
    char format_buffer[XF_FORMAT_BUFFER_SIZE];      // 用于格式化结果的缓冲区
    size_t total_length = 0;
    size_t index = 0, used_index = 0;
    va_list args_copy;

    while (*p != '\0') {
        // 普通字符处理，直接输出到串口
        if (*p != '%') {
            const char *start = p;
            while (*p != '%' && *p != '\0') {
                p++;  // 找到下一个 '%' 或者字符串末尾
            }
            total_length += p - start;
            log_out(start, p - start, arg);  // 输出普通文本部分
        } else {
            p++;  // 移动到 % 后面的字符
            char *format_start = format_flag;  // 记录 % 开始的地方
            *format_start = '%';
            format_start++;
            used_index = 0;

            // 优化一些特殊情况
            if (*p == '%') {
                p++;
                total_length++;
                log_out("%", 1, arg);
                continue;
            } else if (*p == 's') {
                p++;
                va_copy(args_copy, va);
                find_args_from_index(&args_copy, index);
                const char *start = va_arg(args_copy, const char *);
                index++;
                va_end(args_copy);
                total_length += xf_log_strlen(start);
                log_out(start, xf_log_strlen(start), arg);
                continue;
            }

            // 收录格式控制符
            switch (*p) {
            case '-':
            case '+':
            case ' ':
            case '#':
            case '0':
                if ((format_start - format_flag) < XF_FORMAT_FLAG_SIZE - 1) {
                    *format_start++ = *p++;
                }
                break;

            default:
                break;
            }
            // 收录宽度信息
            if (*p == '*') {
                used_index++;
                if ((format_start - format_flag) < XF_FORMAT_FLAG_SIZE - 1) {
                    *format_start++ = *p++;
                }
            } else {
                while (isdigit(*p)) {
                    if ((format_start - format_flag) < XF_FORMAT_FLAG_SIZE - 1) {
                        *format_start++ = *p++;
                    }
                }
            }

            // 收录位置信息
            if (*p == '.') {
                if ((format_start - format_flag) < XF_FORMAT_FLAG_SIZE - 1) {
                    *format_start++ = *p++;
                }
                if (*p == '*') {
                    used_index++;
                    if ((format_start - format_flag) < XF_FORMAT_FLAG_SIZE - 1) {
                        *format_start++ = *p++;
                    }
                } else {
                    while (isdigit(*p)) {
                        if ((format_start - format_flag) < XF_FORMAT_FLAG_SIZE - 1) {
                            *format_start++ = *p++;
                        }
                    }
                }
            }
            // 收录长度
            switch (*p) {
            case 'h':
            case 'l':
            case 'j':
            case 'z':
            case 't': {
                if ((format_start - format_flag) < XF_FORMAT_FLAG_SIZE - 1) {
                    *format_start++ = *p++;
                }
                switch (*p) {
                case 'h':
                case 'l':
                    if ((format_start - format_flag) < XF_FORMAT_FLAG_SIZE - 1) {
                        *format_start++ = *p++;
                    }
                    break;

                default:
                    break;
                }
            }
            break;

            default:
                break;
            }
            // 收录类型转换符
            switch (*p) {
            case 'E':
            case 'F':
            case 'G':
            case 'X':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'i':
            case 'o':
            case 'p':
            case 's':
            case 't':
            case 'u':
            case 'x':
                used_index++;
                if ((format_start - format_flag) < XF_FORMAT_FLAG_SIZE - 1) {
                    *format_start++ = *p++;
                }
                break;

            default:
                break;
            }

            *format_start = '\0';  // 结束符
            // 格式化字符串并输出;
            va_copy(args_copy, va);
            find_args_from_index(&args_copy, index);
            // 获取完整格式化的长度
            int formatted_len = xf_log_vsprintf(format_buffer, XF_FORMAT_BUFFER_SIZE, format_flag, args_copy);
            va_end(args_copy);
            if (formatted_len < XF_FORMAT_BUFFER_SIZE) {
                log_out(format_buffer, formatted_len, arg);
                total_length += formatted_len;
            } else {
                int total_written = 0;
                while (total_written < formatted_len) {
                    int remaining = formatted_len - total_written;
                    int chunk_size = remaining < XF_FORMAT_BUFFER_SIZE ? remaining : XF_FORMAT_BUFFER_SIZE - 1;
                    va_copy(args_copy, va);
                    find_args_from_index(&args_copy, index);
                    xf_log_vsprintf(format_buffer, chunk_size + 1, format_flag, args_copy);  // 输出分块格式化内容
                    va_end(args_copy);
                    log_out(format_buffer, chunk_size, arg);
                    total_written += chunk_size;
                }
                total_length += total_written;
            }
            index += used_index;
        }
    }

    return total_length;
}

static size_t xf_log_printf_out(xf_log_out_t log_out, void *arg, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    size_t len = xf_log_vprintf(log_out, arg, format, va);
    va_end(va);

    return len;
}

static size_t xf_log_color_format(int log_obj_id, uint8_t level, const char *tag, const char *file, uint32_t line,
                                  const char *func, const char *fmt, va_list va)
{
    size_t len = 0;
    xf_log_out_t out_func = s_log_obj[log_obj_id].out_func;
    void *user_args = s_log_obj[log_obj_id].user_args;

#if XF_LOG_COLORS_IS_ENABLE
#if XF_LOG_FILTER_IS_ENABLE
    if (!s_log_obj[log_obj_id].filter.enable || (s_log_obj[log_obj_id].filter.enable
            && s_log_obj[log_obj_id].filter.is_colorful)) {
#endif
        if (s_lvl_to_color[level] != XF_LOG_COLOR_NULL) {
            /* \033[0;3%cm: 重置样式并设置前景色 */
            len += xf_log_printf_out(out_func, user_args, PL_CSI_START "0;3" "%cm", s_lvl_to_color[level]);
        }
#if XF_LOG_FILTER_IS_ENABLE
    }
#endif
#endif

    // 添加时间戳打印
    if (s_log_time_func) {
        len += xf_log_printf_out(out_func, user_args, "%c (%lu)-%s", s_lvl_to_prompt[level], s_log_time_func(), tag);
    } else {
        len += xf_log_printf_out(out_func, user_args, "%c %s", s_lvl_to_prompt[level], tag);
    }

    // 打印信息
    if (level <= s_log_obj[log_obj_id].info_level) {
        len += xf_log_printf_out(out_func, user_args, "[%s:%lu(%s)]", file, line, func);
    }

    // 用户日志打印
    len += xf_log_printf_out(out_func, user_args, ": ");
    len += xf_log_vprintf(out_func, user_args, fmt, va);

#if XF_LOG_COLORS_IS_ENABLE
#if XF_LOG_FILTER_IS_ENABLE
    if (!s_log_obj[log_obj_id].filter.enable || (s_log_obj[log_obj_id].filter.enable
            && s_log_obj[log_obj_id].filter.is_colorful)) {
#endif
        /* 清除 CSI 格式 */
        if (s_lvl_to_color[level] != XF_LOG_COLOR_NULL) {
            len += xf_log_printf_out(out_func, user_args, PL_CSI_END);
        }
#if XF_LOG_FILTER_IS_ENABLE
    }
#endif
#endif
    return len;
}

