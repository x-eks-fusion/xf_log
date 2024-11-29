/**
 * @file xf_log_uitls.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-14
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LOG_UITLS_H__
#define __XF_LOG_UITLS_H__

/* ==================== [Includes] ========================================== */

#include "xf_log_utils_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#if XF_LOG_LEVEL >= XF_LOG_USER
#   define XF_LOGU(tag, format, ...)  xf_log_level(XF_LOG_LVL_USER, tag, format, ##__VA_ARGS__)
#else
#   define XF_LOGU(tag, format, ...)  (void)(tag)
#endif

#if XF_LOG_LEVEL >= XF_LOG_ERROR
#   define XF_LOGE(tag, format, ...)  xf_log_level(XF_LOG_LVL_ERROR, tag, format, ##__VA_ARGS__)
#else
#   define XF_LOGE(tag, format, ...)  (void)(tag)
#endif

#if XF_LOG_LEVEL >= XF_LOG_WARN
#   define XF_LOGW(tag, format, ...)  xf_log_level(XF_LOG_LVL_WARN, tag, format, ##__VA_ARGS__)
#else
#   define XF_LOGW(tag, format, ...)  (void)(tag)
#endif

#if XF_LOG_LEVEL >= XF_LOG_INFO
#   define XF_LOGI(tag, format, ...)  xf_log_level(XF_LOG_LVL_INFO, tag, format, ##__VA_ARGS__)
#else
#   define XF_LOGI(tag, format, ...)  (void)(tag)
#endif

#if XF_LOG_LEVEL >= XF_LOG_DEBUG
#   define XF_LOGD(tag, format, ...)  xf_log_level(XF_LOG_LVL_DEBUG, tag, format, ##__VA_ARGS__)
#else
#   define XF_LOGD(tag, format, ...)  (void)(tag)
#endif

#if XF_LOG_LEVEL >= XF_LOG_VERBOSE
#   define XF_LOGV(tag, format, ...)  xf_log_level(XF_LOG_LVL_VERBOSE, tag, format, ##__VA_ARGS__)
#else
#   define XF_LOGV(tag, format, ...)  (void)(tag)
#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_LOG_UITLS_H__
