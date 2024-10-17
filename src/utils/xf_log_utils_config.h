/**
 * @file xf_log_utils_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-14
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LOG_UTILS_CONFIG_H__
#define __XF_LOG_UTILS_CONFIG_H__

/* ==================== [Includes] ========================================== */

#include "../xf_log_config_internel.h"
#include "xf_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

// 定义打印等级，如果未定义则使用 XF_LOG_LVL_INFO
#ifndef XF_LOG_LEVEL
#   define XF_LOG_LEVEL XF_LOG_LVL_INFO
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_LOG_UTILS_CONFIG_H__
