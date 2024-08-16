/**
 * @file xf_printf.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief 对 printf 相关函数的封装。
 * @version 1.0
 * @date 2024-06-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __XF_PRINTF_H__
#define __XF_PRINTF_H__

/* ==================== [Includes] ========================================== */

#include "../xf_log_config_internal.h"
#include "mpaland_printf/mpaland_printf.h"

/**
 * @ingroup group_xf_log_user
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#define xf_printf_set_putchar(func)         printf_set_putchar_(func)
#define xf_printf_get_putchar()             printf_get_putchar_()

#define xf_putchar(chr)                     putchar_(chr)
#define xf_puts(str)                        printf_("%s", (str))

#define xf_printf(fmt, ...)                 printf_((fmt), ##__VA_ARGS__)
#define xf_vprintf(fmt, va)                 vprintf_((fmt), (va))

#define xf_sprintf(buff, fmt, ...)          sprintf_((buff), (fmt), ##__VA_ARGS__)
#define xf_snprintf(buff, size, fmt, ...)   snprintf_((buff), (size), (fmt), ##__VA_ARGS__)
#define xf_vsnprintf(buff, size, fmt, va)   vsnprintf_((buff), (size), (fmt), (va))

/**
 * @brief printf 输出到指定函数中。
 *
 * @param func 被调用的输出函数。
 * @param user_args 输出函数的用户参数。
 * @param fmt 格式化输出字符串。
 * @param ... 可变参数。
 * @return int 输出的字符数。
 */
#define xf_func_printf(func, user_args, fmt, ...)   fctprintf((func), (user_args), (fmt), ##__VA_ARGS__)

/**
 * @brief vprintf 输出到指定函数中。
 *
 * @param func 被调用的输出函数。
 * @param user_args 输出函数的用户参数。
 * @param fmt 格式化输出字符串。
 * @param va 参数列表。
 * @return int 输出的字符数。
 */
#define xf_func_vprintf(func, user_args, fmt, va)   vfctprintf((func), (user_args), (fmt), (va))

#if XF_PRINTF_OVERRIDE_STD_IS_ENABLE
#define putchar(c)                      putchar_(c)
#define printf(fmt, ...)                printf_(fmt, ##__VA_ARGS__)
#define vprintf(fmt, va)                vprintf_(fmt, va)
#define sprintf(buf, fmt, ...)          sprintf_(buf, fmt, ##__VA_ARGS__)
#define snprintf(buf, cnt, fmt, ...)    snprintf_(buf, cnt, fmt, ##__VA_ARGS__)
#define vsnprintf(buf, cnt, fmt, va)    vsnprintf_(buf, cnt, fmt, va)
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

/**
 * End of group_xf_log_user
 * @}
 */

#endif /* __XF_PRINTF_H__ */
