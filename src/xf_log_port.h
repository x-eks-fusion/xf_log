/**
 * @file xf_log_port.h
 * @author catcatBlue (catcatblue@qq.com)
 * @brief xf_log 对接接口。
 * @version 1.0
 * @date 2024-06-20
 *
 * Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LOG_PORT_H__
#define __XF_LOG_PORT_H__

/* ==================== [Includes] ========================================== */

#include "xf_log_config_internal.h"
#include "xf_log_types.h"

/**
 * @ingroup group_xf_log_port
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 设置 ms 级时间戳来源。
 *
 * @param get_ms 提供给 xf_log 的用于获取时间戳的函数。 @ref xf_log_time_func_t.
 * @return xf_log_err_t
 *      - XF_LOG_OK                     成功
 *      - XF_LOG_ERR_BUSY               存储时间戳函数的变量正在被使用
 *      - XF_LOG_ERR_INVALID_ARG        无效参数
 */
xf_log_err_t xf_log_set_time_src(xf_log_time_func_t get_ms);

/**
 * @brief 获取 log 的时间戳获取函数。
 *
 * @return xf_log_time_func_t 当前提供给 xf_log 的用于获取时间戳的函数。
 */
xf_log_time_func_t xf_log_get_time_src(void);

/**
 * @brief log 默认后端，输出到 xf_putchar。
 *
 * 通常用于在使用 xf_log_set_backend 设置自定义后端后，还原默认后端使用。
 * 见 @ref xf_log_backend_t.
 *
 * @param p_buf 待输出的字符串缓存。
 * @param buf_size 字符串缓存大小。
 * @param p_args 后端控制参数。此处未使用。
 * @return size_t 输出字节数。
 *
 * @note 通常不要调用该函数。
 */
size_t xf_log_default_backend(
    char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args);

/**
 * @brief 设置输出后端。
 *
 * @param backend 输出后端函数。传入 NULL 时自动设为 xf_log_default_backend.
 * @return xf_log_err_t
 *      - XF_LOG_OK                     成功
 *      - XF_LOG_ERR_BUSY               存储时间戳函数的变量正在被使用
 *      - XF_LOG_ERR_INVALID_ARG        无效参数
 *
 * @note 默认后端实现示例见 @ref xf_log_default_backend.
 */
xf_log_err_t xf_log_set_backend(xf_log_backend_t backend);

/**
 * @brief 获取当前输出后端函数。
 *
 * @return xf_log_backend_t 输出后端函数。
 */
xf_log_backend_t xf_log_get_backend(void);

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /*extern "C"*/
#endif

/**
 * End of group_xf_log_port
 * @}
 */

#endif /* __XF_LOG_PORT_H__ */
