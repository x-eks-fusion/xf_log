/**
 * @file xf_log_config_internal.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief xf_log 模块内部配置.
 * @version 1.0
 * @date 2024-06-20
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LOG_CONFIG_INTERNAL_H__
#define __XF_LOG_CONFIG_INTERNAL_H__

/* ==================== [Includes] ========================================== */

#include "xf_log_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief 获取文件名。
 * __FILENAME__ 定义于 xf_def.h，需要 gcc 编译器支持。
 * 如果不支持，__FILENAME__ == __FILE__。
 */
#if !defined(XF_LOG_FILE_NAME)
#   define XF_LOG_FILE_NAME             (__FILENAME__)
#endif

/**
 * @brief 新行符。从XF_LOG_NEWLINE_LF、XF_LOG_NEWLINE_CRLF、XF_LOG_NEWLINE_NONE选一个定义
 * 或者通过XF_LOG_NEWLINE自定义
 */
#if !defined(XF_LOG_NEWLINE)
#   if defined(XF_LOG_NEWLINE_LF)
#       define XF_LOG_NEWLINE "\n"
#   elif defined(XF_LOG_NEWLINE_CRLF)
#       define XF_LOG_NEWLINE "\r\n"
#   elif defined(XF_LOG_NEWLINE_NONE)
#       define XF_LOG_NEWLINE ""
#   else
#       define XF_LOG_NEWLINE "\n"
#   endif
#endif

/**
 * @brief 日志动态栈数组缓存大小。
 */
#if !defined(XF_LOG_BUF_SIZE)
#   define XF_LOG_BUF_SIZE              (64U)
#endif

/**
 * @brief 默认日志等级。
 */
#if !defined(XF_LOG_DEFAULT_LEVEL)
#   define XF_LOG_DEFAULT_LEVEL         (XF_LOG_LVL_INFO)
#endif

/**
 * @brief 默认打印最小等级。
 */
#if !defined(XF_LOG_MINIMUM_LEVEL)
#   define XF_LOG_MINIMUM_LEVEL         (XF_LOG_DEFAULT_LEVEL)
#endif

/**
 * @brief 是否使用彩色 log。
 */
#if !defined(XF_LOG_COLORS_ENABLE) || (XF_LOG_COLORS_ENABLE)
#   define XF_LOG_COLORS_IS_ENABLE             (1)
#else
#   define XF_LOG_COLORS_IS_ENABLE             (0)
#endif

/**
 * @brief xf_printf.
 */

#if !defined(XF_PRINTF_OVERRIDE_STD_ENABLE) || (XF_PRINTF_OVERRIDE_STD_ENABLE)
#   define XF_PRINTF_OVERRIDE_STD_IS_ENABLE    (1)
#else
#   define XF_PRINTF_OVERRIDE_STD_IS_ENABLE    (0)
#endif

/**
 * @brief mpaland_printf.
 */

// 'ntoa' conversion buffer size, this must be big enough to hold one converted
// numeric number including padded zeros (dynamically created on stack)
// default: 32 byte
#if !defined(XF_PRINTF_NTOA_BUFFER_SIZE)
#   define PRINTF_NTOA_BUFFER_SIZE              32U
#else
#   define PRINTF_NTOA_BUFFER_SIZE              XF_PRINTF_NTOA_BUFFER_SIZE
#endif

// 'ftoa' conversion buffer size, this must be big enough to hold one converted
// float number including padded zeros (dynamically created on stack)
// default: 32 byte
#if !defined(XF_PRINTF_FTOA_BUFFER_SIZE)
#   define PRINTF_FTOA_BUFFER_SIZE              32U
#else
#   define PRINTF_FTOA_BUFFER_SIZE              XF_PRINTF_FTOA_BUFFER_SIZE
#endif

// support for the floating point type (%f)
// default: activated
#if !defined(XF_PRINTF_SUPPORT_FLOAT_DISABLE) && !defined(XF_PRINTF_SUPPORT_FLOAT)
#   define PRINTF_DISABLE_SUPPORT_FLOAT         1
#else
#   define PRINTF_SUPPORT_FLOAT                 1
#endif

// support for exponential floating point notation (%e/%g)
// default: activated
#if !defined(XF_PRINTF_SUPPORT_EXPONENTIAL_DISABLE) && !defined(XF_PRINTF_SUPPORT_EXPONENTIAL)
#   define PRINTF_DISABLE_SUPPORT_EXPONENTIAL   1
#else
#   define PRINTF_SUPPORT_EXPONENTIAL           1
#endif

// define the default floating point precision
// default: 6 digits
#if !defined(XF_PRINTF_DEFAULT_FLOAT_PRECISION)
#   define PRINTF_DEFAULT_FLOAT_PRECISION       6U
#else
#   define PRINTF_DEFAULT_FLOAT_PRECISION       XF_PRINTF_DEFAULT_FLOAT_PRECISION
#endif

// define the largest float suitable to print with %f
// default: 1e9
#if !defined(XF_PRINTF_MAX_FLOAT)
#   define PRINTF_MAX_FLOAT                     1e9
#else
#   define PRINTF_MAX_FLOAT                     XF_PRINTF_MAX_FLOAT
#endif

// support for the long long types (%llu or %p)
// default: activated
#if !defined(XF_PRINTF_DISABLE_SUPPORT_LONG_LONG) && !defined(XF_PRINTF_SUPPORT_LONG_LONG)
#   define PRINTF_DISABLE_SUPPORT_LONG_LONG     1
#else
#   define PRINTF_SUPPORT_LONG_LONG             1
#endif

// support for the ptrdiff_t type (%t)
// ptrdiff_t is normally defined in <stddef.h> as long or long long type
// default: activated
#if !defined(XF_PRINTF_SUPPORT_PTRDIFF_T_DISABLE) && !defined(XF_PRINTF_SUPPORT_PTRDIFF_T)
#   define PRINTF_DISABLE_SUPPORT_PTRDIFF_T     1
#else
#   define PRINTF_SUPPORT_PTRDIFF_T             1
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __XF_LOG_CONFIG_INTERNAL_H__ */
