/**
 * @file xf_log.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief xf_log 日志模块。
 * @version 1.0
 * @date
 * - 2023-07-26 初版。
 * - 2024-01-14 替换 printf 实现、加变量锁、增加日志输出后端、增加 VERBOSE 等级。
 *              由于通过日志输出后端可以实现异步输出，因此 log 不设总锁。
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/**
 * @defgroup group_xf_log xf_log
 * @brief xf_log 日志实现。
 *
 * 通常不直接使用 xf_log, 而是使用 xf_utils_log 内定义的宏.
 *
 */

/**
 * @ingroup group_xf_log
 * @defgroup group_xf_log_user 用户接口
 * @brief 如 xf_log, xf_log_set_global_level, xf_log_get_ms 等。
 */

/**
 * @ingroup group_xf_log
 * @defgroup group_xf_log_port 移植接口
 * @brief 注册日志输出后端或日志时间戳来源。
 *
 * 对接 xf_log 时只需 `#include "xf_log_port.h"` 即可。
 *
 */

/**
 * @ingroup group_xf_log
 * @defgroup group_xf_log_internal 内部接口
 * @brief 用户最好不要直接使用。
 */

#ifndef __XF_LOG_H__
#define __XF_LOG_H__

/* ==================== [Includes] ========================================== */

#include "xf_log_config_internal.h"
#include "xf_log_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @ingroup group_xf_log_user
 * @{
 */

/**
 * @brief 输出日志到后端。
 *
 * @param level 日志等级，见 XF_LOG_LVL_*.
 * @param tag 日志标签。
 * @param file 文件名，从 __FILE__ 中获得。
 * @param line 行号。
 * @param func 函数名。
 * @param fmt 格式化字符串，用户输入。
 * @param ... 可变参数。
 * @return size_t 本次日志字节数。
 *
 * @note 不要直接使用这个函数，请使用 `xf_utils_log.h` 内的 XF_LOG* 宏。
 *       如 @ref XF_LOGI.
 * @note 暂未启用格式化检查(`__format_attribute(6, 7)`)。
 */
size_t xf_log(
    uint8_t level, const char *tag,
    const char *file, uint32_t line, const char *func,
    const char *fmt, ...);

/**
 * @brief 设置全局日志等级，如果日志等级低于该等级则不会输出。
 *
 * @param level 日志等级。
 * @return xf_log_err_t
 *      - XF_LOG_OK                     成功
 *      - XF_LOG_ERR_INVALID_ARG        无效参数
 */
xf_log_err_t xf_log_set_global_level(int level);

/**
 * @brief 获取全局日志等级。
 *
 * @return int 全局日志等级。
 */
int xf_log_get_global_level(void);

/**
 * @brief 获取日志时间。
 *
 * @return uint32_t ms 级时间戳。
 */
uint32_t xf_log_get_ms(void);

/**
 * @brief 获取时间字符串。
 *
 * @param[out] p_buf 指向存储字符串的缓冲区。
 * @param buf_size 缓冲区最大大小。用于截断。
 * @return xf_log_err_t
 *      - XF_LOG_OK         成功
 */
xf_log_err_t xf_log_get_timestamp(char *p_buf, uint16_t buf_size);

/**
 * End of group_xf_log_user
 * @}
 */

/**
 * @ingroup group_xf_log_internal
 * @{
 */

/**
 * @brief 日志格式化输出实现。
 *
 * @param p_cb mpaland_printf 内 fctprintf 的回调函数。
 * @param p_info 日志等级、行号、文件名等信息。
 * @param p_details 用户格式化字符串及参数细节。
 * @return size_t 本次日志字节数。
 *
 * @attention 如无必要 **请勿** 使用该函数。
 * @note 具体用法参见 `xf_log.c`.
 */
size_t xf_log_vfunc(
    xf_log_cb_t *p_cb, xf_log_info_t *p_info, xf_log_details_t *p_details);

/**
 * End of group_xf_log_internal
 * @}
 */

/* ==================== [Macros] ============================================ */

/**
 * @ingroup group_xf_log_user
 * @{
 */

/**
 * @brief 输出日志宏（错误等级以上带有文件名行号等信息）。
 *
 * @param level 日志等级，见 XF_LOG_LVL_*.
 * @param tag 日志标签。
 * @param fmt 格式化字符串，用户输入。
 * @param ... 可变参数。
 * @return size_t 本次日志字节数。
 */
#define XF_LOG_WITH_EXTRA(level, tag, fmt, ...) \
    xf_log((level), (tag), \
        (XF_LOG_FILE_NAME), (__LINE__), (XF_LOG_FUNCTION_NAME), \
        (fmt), ##__VA_ARGS__)

/**
 * @brief 输出日志宏（不带有文件名行号等信息）。
 *
 * @param level 日志等级，见 XF_LOG_LVL_*.
 * @param tag 日志标签。
 * @param fmt 格式化字符串，用户输入。
 * @param ... 可变参数。
 * @return size_t 本次日志字节数。
 */
#define XF_LOG_WITHOUT_EXTRA(level, tag, fmt, ...) \
    xf_log((level), (tag), \
        (NULL), (0), (NULL), \
        (fmt), ##__VA_ARGS__)

/**
 * End of group_xf_log_user
 * @}
 */

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __XF_LOG_H__ */
