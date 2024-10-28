/**
 * @file xf_log.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-10-09
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_LOG_H__
#define __XF_LOG_H__

/* ==================== [Includes] ========================================== */

#include "xf_log_config_internel.h"

#if XF_LOG_STDDEF_IS_ENABLE
    #include <stddef.h>
#else
    typedef long unsigned int size_t;
#endif

#if XF_LOG_STDINT_IS_ENABLE
    #include <stdint.h>
#else
    typedef unsigned int uint32_t;
    typedef unsigned char uint8_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

#define XF_LOG_LVL_NONE     (0)
#define XF_LOG_LVL_USER     (1)
#define XF_LOG_LVL_ERROR    (2)
#define XF_LOG_LVL_WARN     (3)
#define XF_LOG_LVL_INFO     (4)
#define XF_LOG_LVL_DEBUG    (5)
#define XF_LOG_LVL_VERBOSE  (6)

/* ==================== [Typedefs] ========================================== */

typedef void(*xf_log_out_t)(const char *str, size_t len, void *arg);
typedef uint32_t (*xf_log_time_func_t)(void);

/* ==================== [Global Prototypes] ================================= */

/**
 * @brief 注册log后端是输出到哪里，其最大值受到 XF_LOG_OBJ_MAX 的限制
 *
 * @param out_func 后端输出函数， 如果减少IO操作，可以考虑使用异步缓冲
 * @param user_args 传入的参数，会在 out_func 中被调用
 * @return int  -1:失败, >=0:注册成功后返回的id
 */
int xf_log_register_obj(xf_log_out_t out_func, void *user_args);

#if XF_LOG_FILTER_IS_ENABLE

/**
 * @brief 启用过滤器
 *
 * @param log_obj_id 指定log对象id
 */
void xf_log_set_filter_enable(int log_obj_id);

/**
 * @brief 禁用过滤器
 *
 * @param log_obj_id 指定log对象id
 */
void xf_log_set_filter_disable(int log_obj_id);

/**
 * @brief 开启彩色显示
 *
 * @param log_obj_id 指定log对象id
 */
void xf_log_set_filter_colorful_enable(int log_obj_id);

/**
 * @brief 关闭彩色显示
 *
 * @param log_obj_id 指定log对象id
 */
void xf_log_set_filter_colorful_disable(int log_obj_id);

/**
 * @brief 设置过滤器为黑名单（其内容将被过滤）
 *
 * @param log_obj_id 指定log对象id
 */
void xf_log_set_filter_is_blacklist(int log_obj_id);

/**
 * @brief 设置过滤器为白名单（仅其内容将被暴露）
 *
 * @param log_obj_id 指定log对象id
 */
void xf_log_set_filter_is_whitelist(int log_obj_id);

/**
 * @brief 设置过滤器的标签过滤
 *
 * @param log_obj_id 指定log对象id
 * @param tag 指定过滤的标签，如果为NULL则表示不过滤
 */
void xf_log_set_filter_tag(int log_obj_id, const char *tag);

/**
 * @brief 设置过滤器的等级过滤
 *
 * @param log_obj_id 指定log对象id
 * @param level 指定过滤的等级，小于等于level将不会被过滤
 */
void xf_log_set_filter_level(int log_obj_id, uint8_t level);

/**
 * @brief 设置过滤器的文件过滤
 *
 * @param log_obj_id 指定log对象id
 * @param file 指定过滤的文件，如果为NULL则表示不过滤
 */
void xf_log_set_filter_file(int log_obj_id, const char *file);

#endif

/**
 * @brief 显示文件函数等信息的最小等级
 *
 * @param log_obj_id 指定log对象id
 * @param level 指定显示文件函数等信息的最小等级，小于等于level将不会被过滤
 */
void xf_log_set_info_level(int log_obj_id, uint8_t level);

/**
 * @brief 设置log的时间戳打印函数
 *
 * @param log_time_func log的时间戳打印函数
 */
void xf_log_set_time_func(xf_log_time_func_t log_time_func);

/**
 * @brief log打印函数
 *
 * @param level log打印等级
 * @param tag 打印标签
 * @param file 当前文件
 * @param line 当前行数
 * @param func 当前函数
 * @param fmt 格式化日志
 * @param ...
 * @return size_t 格式化输出的长度
 */
size_t xf_log(uint8_t level, const char *tag, const char *file, uint32_t line, const char *func, const char *fmt, ...);

/**
 * @brief 朴实无华的打印函数
 * 
 * @param format 需要格式化打印的字符串
 * @param ... 需要格式化的参数 
 * @return size_t 格式化打印的长度
 */
size_t xf_log_printf(const char *format, ...);

/* ==================== [Macros] ============================================ */

#define xf_log_level(level, tag, fmt, ...)  xf_log(level, tag, __FILE__, __LINE__, __func__, fmt XF_LOG_NEWLINE, ##__VA_ARGS__)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_LOG_H__
