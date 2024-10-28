/**
 * @file xf_log_config_internel.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-10
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LOG_CONFIG_INTERNEL_H__
#define __XF_LOG_CONFIG_INTERNEL_H__

/* ==================== [Includes] ========================================== */

#include "xf_log_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

// 打开彩色打印，xf_log_config.h 中如果定义 XF_LOG_COLORS_ENABLE 为 0 则关闭
#if !defined(XF_LOG_COLORS_ENABLE) || XF_LOG_COLORS_ENABLE
#define XF_LOG_COLORS_IS_ENABLE (1)
#else
#define XF_LOG_COLORS_IS_ENABLE (0)
#endif

// 打开过滤器，xf_log_config.h 中如果定义 XF_LOG_FILTER_ENABLE 为 0 则关闭
#if !defined(XF_LOG_FILTER_ENABLE) || XF_LOG_FILTER_ENABLE
#define XF_LOG_FILTER_IS_ENABLE (1)
#else
#define XF_LOG_FILTER_IS_ENABLE (0)
#endif

// ctype.h头文件的支持，如果关闭则启用内部宏实现 isdigit 函数
#if !defined(XF_LOG_CTYPE_ENABLE) || XF_LOG_CTYPE_ENABLE
#define XF_LOG_CTYPE_IS_ENABLE (1)
#else
#define XF_LOG_CTYPE_IS_ENABLE (0)
#endif

// stddef.h头文件的支持，如果关闭则启用内部实现 size_t 类型
#if !defined(XF_LOG_STDDEF_ENABLE) || XF_LOG_STDDEF_ENABLE
#define XF_LOG_STDDEF_IS_ENABLE (1)
#else
#define XF_LOG_STDDEF_IS_ENABLE (0)
#endif

// stdint.h头文件的支持，如果关闭则启用内部实现 uint32_t uint8_t 类型
#if !defined(XF_LOG_STDINT_ENABLE) || XF_LOG_STDINT_ENABLE
#define XF_LOG_STDINT_IS_ENABLE (1)
#else
#define XF_LOG_STDINT_IS_ENABLE (0)
#endif

// string.h头文件的支持，如果关闭则需要手动实现 xf_log_strlen(s) 函数
#if !defined(XF_LOG_STRLEN_ENABLE) || XF_LOG_STRLEN_ENABLE
#define XF_LOG_STRLEN_IS_ENABLE (1)
#else
#define XF_LOG_STRLEN_IS_ENABLE (0)
#endif

#if !XF_LOG_STRLEN_IS_ENABLE && !defined(xf_log_strlen)
#error "xf_log_strlen(s) must be defined when XF_LOG_STRLEN_IS_ENABLE is 0"
#endif

// stdio.h头文件的支持，如果关闭则需要手动实现 xf_log_vsprintf(buffer, maxlen, fmt, args) 函数
#if !defined(XF_LOG_VSNPRINTF_ENABLE) || XF_LOG_VSNPRINTF_ENABLE
#define XF_LOG_VSNPRINTF_IS_ENABLE (1)
#else
#define XF_LOG_VSNPRINTF_IS_ENABLE (0)
#endif

#if !XF_LOG_VSNPRINTF_IS_ENABLE && !defined(xf_log_vsprintf)
#error "xf_log_vsprintf(buffer, maxlen, fmt, args) must be defined when XF_LOG_VSNPRINTF_IS_ENABLE is 0"
#endif

// 后端对接的输出对象数目，默认为一个对象
#ifndef XF_LOG_OBJ_NUM
#define XF_LOG_OBJ_NUM (1)
#endif

#ifndef XF_LOG_NEWLINE
#define XF_LOG_NEWLINE "\n"
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_LOG_CONFIG_INTERNEL_H__
