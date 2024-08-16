/**
 * @file xf_log_types.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief xf_log 通用类型与宏定义。
 * @version 1.0
 * @date 2024-06-20
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LOG_TYPES_H__
#define __XF_LOG_TYPES_H__

/* ==================== [Includes] ========================================== */

#include "xf_log_config_internal.h"
#include "xf_printf/xf_printf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @ingroup group_xf_log_internal
 * @{
 */

/**
 * @name log 等级
 * @{
 */
#define XF_LOG_LVL_NONE     (0)
#define XF_LOG_LVL_USER     (1)
#define XF_LOG_LVL_ERROR    (2)
#define XF_LOG_LVL_WARN     (3)
#define XF_LOG_LVL_INFO     (4)
#define XF_LOG_LVL_DEBUG    (5)
#define XF_LOG_LVL_VERBOSE  (6)
/**
 * End of `log 等级`
 * @}
 */

/* log buf 类型标识 */
#define XF_LOG_MAGIC_NUM    (0xDFDF)

#if !defined(__FILENAME__)
#if defined(__GNUC__)
/**
 * @brief 获取不含路径的文件名。
 */
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') \
                        ? (__builtin_strrchr(__FILE__, '/') + 1) \
                        : (__FILE__))
#else
#define __FILENAME__ __FILE__
#endif /* defined(__GNUC__) */
#endif /* !defined(__FILENAME__) */

#define XF_LOG_FUNCTION_NAME        (__FUNCTION__)

/* ==================== [Typedefs] ========================================== */

#include <string.h>     /*!< 使用了 memset */
#include <stdint.h>     /*!< uint32_t 等 */
#include <stdarg.h>     /*!< 用于可变参数 */

typedef int xf_log_err_t;
#define XF_LOG_OK                   (0)
#define XF_LOG_FAIL                 (-1)
#define XF_LOG_ERR_INVALID_ARG      (0x101)
#define XF_LOG_ERR_BUSY             (0x107)

#ifndef UNUSED
/**
 * @brief 未使用的变量通过 UNUSED 防止编译器警告。
 */
#define UNUSED(x)                   do { (void)(x); } while(0)
#endif

/**
 * @brief 颜色列表。
 */
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

/**
 * @brief xf_log 控制命令枚举。
 */
typedef enum _xf_log_ctrl_t {
    XF_LOG_CTRL_NONE        = 0x00, /*!< 无特殊控制，自动 */
    XF_LOG_CTRL_STORE,              /*!< 需要存储到缓存中 */
    XF_LOG_CTRL_OUT,                /*!< log 准备完毕可以输出 */

    XF_LOG_CTRL_MAX,
} xf_log_ctrl_t;

/**
 * @brief log 输出回调函数结构体，基于 Marco Paland 的 printf。
 */
typedef struct _xf_log_cb_t {
    out_fct_t out_func;             /*!< log 输出回调函数 */
    void *user_args;                /*!< log 输出回调函数参数 */
} xf_log_cb_t;

/**
 * @brief log 信息结构体。
 */
typedef struct _xf_log_info_t {
    uint8_t level;                  /*!< log 等级 */
    const char *tag;                /*!< log 标签 */
    const char *file;               /*!< log 所在的文件名 */
    uint32_t line;                  /*!< log 所在的行数 */
    const char *func;               /*!< log 所在的函数 */
} xf_log_info_t;

/**
 * @brief log 详情结构体。
 */
typedef struct _xf_log_details_t {
    const char *fmt;                /*!< 用户格式化字符串 */
    va_list va;                     /*!< 用户参数列表 */
} xf_log_details_t;

/**
 * @brief log 信息结构体。
 */
typedef struct _xf_log_handler_args_t {
    uint16_t magic;                 /*!< 标识数字，默认为 XF_LOG_MAGIC_NUM */
    uint8_t ctrl;                   /*!< 控制信息，见 @ref xf_log_ctrl_t */
    uint8_t reserve;                /*!< 保留 */
    char *p_buf;                    /*!< 缓冲区指针 */
    size_t buf_size;                /*!< 缓冲区大小 */
    size_t index;                   /*!< 缓冲区索引 */
} xf_log_handler_args_t;

/**
 * End of group_xf_log_internal
 * @}
 */

/**
 * @ingroup group_xf_log_port
 * @{
 */

/**
 * @brief 日志获取 ms 戳函数类型。
 */
typedef uint32_t (*xf_log_time_func_t)(void);

/**
 * @brief log 信息结构体。
 */
typedef struct _xf_log_backend_args_t {
    void *log_id;   /*!< 日志标识信息。通常不使用。
                     *   异步输出时可以用随机数标识当前 log。
                     *   如果 p_buf 来自栈空间，也可以用来标识当前 log。
                     *   目前使用栈数组标识。
                     *   为 NULL 时表示数据是逐字节输出的。
                     */
    uint32_t ctrl;  /*!< 后端输出控制，见 @ref xf_log_ctrl_t.
                     *   xf_log 的栈数组会暂存 printf 的逐字节输出。
                     *   数组塞满时，会调用后端输出函数转移信息。因此
                     *   不是每次调用后端函数时都可以直接输出。
                     *   异步输出时，当后端函数接收到 XF_LOG_CTRL_OUT
                     *   信号后，说明当前 log 输出完成，可以输出。
                     */
} xf_log_backend_args_t;

/**
 * @brief 日志输出后端函数类型。
 *
 * @param p_buf 交给后端输出的字符串缓存。
 * @param buf_size 字符串缓存大小。0 字节说明本次调用只用于传输控制信号。
 * @param p_args 后端控制参数。见 @ref xf_log_backend_args_t.
 * @return size_t 本次日志字节数。
 */
typedef size_t (*xf_log_backend_t)(
    char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args);

/**
 * End of group_xf_log_port
 * @}
 */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __XF_LOG_TYPES_H__ */
