/**
 * @file xf_log.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief xf_log 日志模块实现。
 * @version 1.0
 * @date 2023-07-26     初版。
 *       2024-01-14     替换 printf 实现、加变量锁、增加日志输出后端、
 *                      增加 VERBOSE 等级。
 *                      由于通过日志输出后端可以实现异步输出，因此 log 不设总锁。
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_log.h"
#include "xf_log_port.h"

/* ==================== [Defines] =========================================== */

/**
 * CSI(Control Sequence Introducer/Initiator) sign
 * more information on https://en.wikipedia.org/wiki/ANSI_escape_code
 */
#define PL_CSI_START                "\033["
#define PL_CSI_END                  "\033[0m"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/**
 * @brief 字符处理函数。
 *
 * @param chr 新日志字符。
 * @param user_args 用户测试。此处用于存储 xf_log_handler_args_t。
 */
static void _xf_log_char_handler(char chr, void *user_args);

/**
 * @brief 调用后端输出函数。
 */
static inline size_t _xf_log_call_backend(
    char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args);

static void *_memset(void *s, int c, size_t n);

/* ==================== [Static Variables] ================================== */

static const char sc_lvl_to_prompt[] = {
    [XF_LOG_LVL_NONE]       = '\0',
    [XF_LOG_LVL_USER]       = 'U',
    [XF_LOG_LVL_ERROR]      = 'E',
    [XF_LOG_LVL_WARN]       = 'W',
    [XF_LOG_LVL_INFO]       = 'I',
    [XF_LOG_LVL_DEBUG]      = 'D',
    [XF_LOG_LVL_VERBOSE]    = 'V',
};

static const uint8_t sc_lvl_to_color[] = {
    [XF_LOG_LVL_NONE]       = XF_LOG_COLOR_NULL,
    [XF_LOG_LVL_USER]       = XF_LOG_COLOR_BLUE,
    [XF_LOG_LVL_ERROR]      = XF_LOG_COLOR_RED,
    [XF_LOG_LVL_WARN]       = XF_LOG_COLOR_YELLOW,
    [XF_LOG_LVL_INFO]       = XF_LOG_COLOR_GREEN,
    [XF_LOG_LVL_DEBUG]      = XF_LOG_COLOR_CYAN,
    [XF_LOG_LVL_VERBOSE]    = XF_LOG_COLOR_NULL,
};

/**
 * @brief log 默认输出等级。
 */
static int s_global_level = (int)XF_LOG_LVL_VERBOSE;

/**
 * @brief 获取以 ms 为单位的时间戳的函数。
 */
static xf_log_time_func_t s_get_ms = NULL;

/**
 * @brief log 后端。
 */
static xf_log_backend_t s_backend = xf_log_default_backend;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

size_t xf_log(
    uint8_t level, const char *tag,
    const char *file, uint32_t line, const char *func,
    const char *fmt, ...)
{
    size_t len = 0;

#if (XF_LOG_BUF_SIZE == 0)
    xf_log_cb_t log_cb = {
        .out_func = _xf_log_char_handler,
        .user_args = NULL,
    };
#else /* !(XF_LOG_BUF_SIZE == 0) */
    char log_buf[XF_LOG_BUF_SIZE] = {0};
    xf_log_handler_args_t handler_args = {
        .magic      = XF_LOG_MAGIC_NUM,
        .ctrl       = XF_LOG_CTRL_NONE,
        .p_buf      = log_buf,
        .buf_size   = XF_LOG_BUF_SIZE,
        .index      = 0,
    };
    xf_log_cb_t log_cb = {
        .out_func   = _xf_log_char_handler,
        .user_args  = &handler_args,
    };
#endif /* (XF_LOG_BUF_SIZE == 0) */
    xf_log_info_t log_info = {
        .level      = level,
        .tag        = tag,
        .file       = file,
        .line       = line,
        .func       = func,
    };
    xf_log_details_t log_details = {
        .fmt        = fmt,
    };

    va_start(log_details.va, fmt);
    len = xf_log_vfunc(&log_cb, &log_info, &log_details);
    va_end(log_details.va);

#if (XF_LOG_BUF_SIZE != 0)
    /* 如果还有数据没有输出到后端 */
    if (handler_args.index != 0) {
        handler_args.ctrl = XF_LOG_CTRL_OUT;
        _xf_log_char_handler('\0', &handler_args);
    }

    /* 向后端发送 log 完毕信号 */
    xf_log_backend_args_t backend_args = {0};
    backend_args.log_id     = log_buf;
    backend_args.ctrl       = XF_LOG_CTRL_OUT;
    /* 0 字节说明本次调用只用于发 log 完毕信号 */
    _xf_log_call_backend(log_buf, 0, &backend_args);
#endif /* (XF_LOG_BUF_SIZE != 0) */

    return len;
}

size_t xf_log_vfunc(
    xf_log_cb_t *p_cb, xf_log_info_t *p_info, xf_log_details_t *p_details)
{
    if ((NULL == p_cb) || (NULL == p_info) || (NULL == p_details)) {
        return 0;
    }
    if ((NULL == p_cb->out_func) || (NULL == p_details->fmt)) {
        return 0;
    }
    if ((p_info->level > s_global_level) || (XF_LOG_LVL_NONE == s_global_level)) {
        return 0;
    }

    size_t len = 0;

#if XF_LOG_COLORS_IS_ENABLE
    if (sc_lvl_to_color[p_info->level] != XF_LOG_COLOR_NULL) {
        /* \033[0;3%cm: 重置样式并设置前景色 */
        len += xf_func_printf(
                   p_cb->out_func, p_cb->user_args,
                   PL_CSI_START "0;3" "%cm",
                   sc_lvl_to_color[p_info->level]);
    }
#endif

    /* 存放时间信息的字符串 */
    char time_buf[16] = {0};
    /* 获取时间戳 */
    xf_log_get_timestamp(time_buf, sizeof(time_buf));

    /* I (123)-main */
    len += xf_func_printf(
               p_cb->out_func, p_cb->user_args,
               "%c (%s)-%s",
               sc_lvl_to_prompt[p_info->level],
               (!!time_buf[0]) ? (time_buf) : ("none"),
               p_info->tag);

    /* [main.c:102(test_log_hello)] */
    if (p_info->level <= XF_LOG_LVL_ERROR) {
        if (NULL != p_info->func) {
            len += xf_func_printf(
                       p_cb->out_func, p_cb->user_args,
                       "[%s:%lu(%s)]",
                       p_info->file, p_info->line, p_info->func);
        } else {
            len += xf_func_printf(
                       p_cb->out_func, p_cb->user_args,
                       "[%s:%lu]",
                       p_info->file, p_info->line);
        }
    }

    /* `I (123)-main: ` 或 `I (123)-main[main.c:102(test_log_hello)]: ` */
    len += xf_func_printf(
               p_cb->out_func, p_cb->user_args, ": ");

    /* 用户日志详情 */
    len += xf_func_vprintf(
               p_cb->out_func, p_cb->user_args,
               p_details->fmt, p_details->va);

#if XF_LOG_COLORS_IS_ENABLE
    /* 清除 CSI 格式 */
    if (sc_lvl_to_color[p_info->level] != XF_LOG_COLOR_NULL) {
        len += xf_func_printf(p_cb->out_func, p_cb->user_args, PL_CSI_END);
    }
#endif

    /* 新行 */
    len += xf_func_printf(p_cb->out_func, p_cb->user_args, XF_LOG_NEWLINE);

    return len;
}

xf_log_err_t xf_log_set_global_level(int level)
{
    if ((level < 0) || (level > XF_LOG_LVL_VERBOSE)) {
        return XF_LOG_ERR_INVALID_ARG;
    }
    s_global_level = level;
    return XF_LOG_OK;
}

int xf_log_get_global_level(void)
{
    return s_global_level;
}

xf_log_err_t xf_log_set_time_src(xf_log_time_func_t get_ms)
{
    s_get_ms = get_ms;
    return XF_LOG_OK;
}

xf_log_time_func_t xf_log_get_time_src(void)
{
    return s_get_ms;
}

uint32_t xf_log_get_ms(void)
{
    if (NULL == s_get_ms) {
        return 0;
    }
    uint32_t ms;
    ms = s_get_ms();
    return ms;
}

xf_log_err_t xf_log_get_timestamp(char *p_buf, uint16_t buf_size)
{
    if ((NULL == p_buf) || (0 == buf_size)) {
        return XF_LOG_FAIL;
    }
    _memset(p_buf, 0, buf_size);
    if (NULL == s_get_ms) {
        xf_snprintf(p_buf, buf_size, "none");
    } else {
        xf_snprintf(p_buf, buf_size, "%lu", xf_log_get_ms());
    }
    return XF_LOG_OK;
}

size_t xf_log_default_backend(
    char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args)
{
    UNUSED(p_args);
    if ((NULL == p_buf) || (0 == buf_size)) {
        return XF_LOG_ERR_INVALID_ARG;
    }
    size_t idx = 0;
    while ((*(p_buf + idx) != '\0') && (idx < buf_size)) {
        xf_putchar(*(p_buf + idx));
        ++idx;
    }
    return idx;
}

xf_log_err_t xf_log_set_backend(xf_log_backend_t backend)
{
    if (NULL == backend) {
        backend = xf_log_default_backend;
    }
    s_backend = backend;
    return XF_LOG_OK;
}

xf_log_backend_t xf_log_get_backend(void)
{
    return s_backend;
}

/* ==================== [Static Functions] ================================== */

static inline size_t _xf_log_call_backend(
    char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args)
{
    size_t len;
    len = s_backend(p_buf, buf_size, p_args);
    return len;
}

static void _xf_log_char_handler(char chr, void *user_args)
{
    xf_log_backend_args_t backend_args = {0};
#if (XF_LOG_BUF_SIZE == 0)
    UNUSED(user_args);
    backend_args.log_id = NULL;
    backend_args.ctrl = XF_LOG_CTRL_OUT;
    _xf_log_call_backend(&chr, 1, &backend_args);
    return;
#else /* !(XF_LOG_BUF_SIZE == 0) */
    xf_log_handler_args_t *const p_hargs = (xf_log_handler_args_t *)user_args;
    uint8_t exit_flag = 0;
    uint8_t ctrl = p_hargs->ctrl;
    if ((NULL == p_hargs) || (XF_LOG_MAGIC_NUM != p_hargs->magic)) {
        backend_args.log_id = NULL;
        backend_args.ctrl = XF_LOG_CTRL_STORE;
        _xf_log_call_backend(&chr, 1, &backend_args);
        return;
    }
    /* 自动或手动选择存储或输出的状态机 */
    do {
        switch (ctrl) {
        case XF_LOG_CTRL_NONE:
        case XF_LOG_CTRL_STORE: {
            if (p_hargs->index < p_hargs->buf_size) {
                p_hargs->p_buf[p_hargs->index++] = chr;
                exit_flag = 1;
                break;
            }
            /* 转换到输出状态 */
            ctrl = XF_LOG_CTRL_OUT;
            exit_flag = 0;
        } break;
        case XF_LOG_CTRL_OUT: {
            backend_args.log_id = p_hargs->p_buf;
            backend_args.ctrl = XF_LOG_CTRL_STORE;
            _xf_log_call_backend(
                p_hargs->p_buf, p_hargs->index, &backend_args);
            _memset(p_hargs->p_buf, 0, p_hargs->buf_size);
            p_hargs->index = 0;
            ctrl = XF_LOG_CTRL_STORE; /*!< 回到存储状态 */
            if (chr != '\0') {
                exit_flag = 0; /*!< 开启新一轮的存储 */
            } else {
                exit_flag = 1;
            }
        } break;
        default:
            exit_flag = 1;
            break;
        }
    } while (1 != exit_flag);
    return;
#endif /* (XF_LOG_BUF_SIZE == 0) */
}

static void *_memset(void *s, int c, size_t n)
{
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}
