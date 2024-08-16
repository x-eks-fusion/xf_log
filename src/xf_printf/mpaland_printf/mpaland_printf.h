/**
 * @file mpaland_printf.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief Marco Paland 的 printf 实现。
 * @version 1.0
 * @date 2024-08-08
 * 
 * 2014-2019, PALANDesign Hannover, Germany
 * 
 */

///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2014-2019, PALANDesign Hannover, Germany
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources.
//        Use this instead of bloated standard/newlib printf.
//        These routines are thread safe and reentrant.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _MPALAND_PRINTF_H_
#define _MPALAND_PRINTF_H_

#include <stdarg.h>
#include <stddef.h>

#include "mpaland_printf_config.h"

#ifdef __cplusplus
extern "C" {
#endif


#define PRINTF_INCLUDE_CONFIG_H 1



/**
 * @ingroup group_xf_log_port
 * @{
 */


/**
 * @brief 输出一个字符函数的类型。
 */
typedef int (*putchar_t)(int chr);


/**
 * @brief 自定义输出函数类型。
 *
 * @param chr 当前输出的字符。
 * @param user_args 用户参数。
 */
typedef void (*out_fct_t)(char chr, void *user_args);


/**
 * @brief 设置 printf 的字符输出函数。
 *
 * @param putchar_func 类似于 putchar 的函数，用作 printf 的输出。
 */
void printf_set_putchar_(putchar_t putchar_func);


/**
 * @brief 获取 printf 的字符输出函数。
 *
 * @return putchar_t 当前默认的字符输出函数。
 */
putchar_t printf_get_putchar_(void);


/**
 * End of group_xf_log_port
 * @}
 */


/**
 * @ingroup group_xf_log_internal
 * @{
 */


/**
 * Output a character to a custom device like UART, used by the printf() function
 * This function is declared here only. You have to write your custom implementation somewhere
 * \param character Character to output
 */
int putchar_(int character);


/**
 * Tiny printf implementation
 * You have to implement _putchar if you use printf()
 * To avoid conflicts with the regular printf() API it is overridden by macro defines
 * and internal underscore-appended functions like printf_() are used
 * \param format A string that specifies the format of the output
 * \return The number of characters that are written into the array, not counting the terminating null character
 */
int printf_(const char *format, ...);


/**
 * Tiny sprintf implementation
 * Due to security reasons (buffer overflow) YOU SHOULD CONSIDER USING (V)SNPRINTF INSTEAD!
 * \param buffer A pointer to the buffer where to store the formatted string. MUST be big enough to store the output!
 * \param format A string that specifies the format of the output
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 */
int sprintf_(char *buffer, const char *format, ...);


/**
 * Tiny snprintf implementation
 * \param buffer A pointer to the buffer where to store the formatted string
 * \param count The maximum number of characters to store in the buffer, including a terminating null character
 * \param format A string that specifies the format of the output
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 *         If the formatted string is truncated the buffer size (count) is returned
 */
int  snprintf_(char *buffer, size_t count, const char *format, ...);


/**
 * Tiny vsnprintf implementation
 * \param buffer A pointer to the buffer where to store the formatted string
 * \param count The maximum number of characters to store in the buffer, including a terminating null character
 * \param format A string that specifies the format of the output
 * \param va A value identifying a variable arguments list
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 *         If the formatted string is truncated the buffer size (count) is returned
 */
int vsnprintf_(char *buffer, size_t count, const char *format, va_list va);


/**
 * Tiny vprintf implementation
 * \param format A string that specifies the format of the output
 * \param va A value identifying a variable arguments list
 * \return The number of characters that are WRITTEN into the buffer, not counting the terminating null character
 */
int vprintf_(const char *format, va_list va);


/**
 * printf with output function
 * You may use this as dynamic alternative to printf() with its fixed _putchar() output
 * \param out_fct An output function which takes one character and an argument pointer
 * \param arg An argument pointer for user data passed to output function
 * \param format A string that specifies the format of the output
 * \return The number of characters that are sent to the output function, not counting the terminating null character
 */
int fctprintf(out_fct_t out_fct, void *arg, const char *format, ...);


/**
 * printf with output function
 * You may use this as dynamic alternative to printf() with its fixed _putchar() output
 * \param out_fct An output function which takes one character and an argument pointer
 * \param arg An argument pointer for user data passed to output function
 * \param format A string that specifies the format of the output
 * \param va A value identifying a variable arguments list
 * \return The number of characters that are sent to the output function, not counting the terminating null character
 */
int vfctprintf(out_fct_t out_fct, void *arg, const char *format, va_list va);

/**
 * End of group_xf_log_internal
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif  // _MPALAND_PRINTF_H_
