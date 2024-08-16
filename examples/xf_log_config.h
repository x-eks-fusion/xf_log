/**
 * @file xf_log_config.h
 *
 * @brief xf_log 配置文件.
 */

/**
 * @brief 使用说明.
 *
 * 1. 复制该文件到工程目录, 并重命名为 `xf_log_config.h`.
 * 2. 打开本文件(`#if 0` 改为 `#if 1`, 或删除 `#if 0` 和对应的 `#endif`).
 */

#ifndef __XF_LOG_CONFIG_H__
#define __XF_LOG_CONFIG_H__

/* ==================== [Includes] ========================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */

/**
 * @brief 获取文件名。
 * __FILENAME__ 定义于 xf_def.h，需要 gcc 编译器支持。
 * 如果不支持，__FILENAME__ == __FILE__。
 */
#define XF_LOG_FILE_NAME                    (__FILENAME__)
//                                          (__FILE__)

/**
 * @brief 新行符。
 */
#define XF_LOG_NEWLINE                      "\n"
//                                          "\r\n"
//                                          ""

/**
 * @brief 日志动态栈数组缓存大小。
 */
#define XF_LOG_BUF_SIZE                     (64U)

/**
 * @brief 时间戳样式。二选一.
 */
#define XF_LOG_TIMESTAMP_TIME_MS_ENABLE            (1)

/**
 * @brief 默认日志等级。 见 @ref xf_log_types.h
 */
#define XF_LOG_DEFAULT_LEVEL                (XF_LOG_LVL_INFO)

/**
 * @brief 是否使用彩色 log。
 */
#define XF_LOG_COLORS_ENABLE                    (1)
//                                          (0)

/**
 * @brief 是否使用 xf_printf() 替代 printf()
 */
#define XF_XF_PRINTF_OVERRIDE_STD_ENABLE           (0)
//                                          (1)

/**
 * @brief 是否使用 xf_printf.
 */
#define XF_XF_PRINTF_CUSTOM_XF_PRINTF_ENABLE          (1)
//                                          (0)

/**
 * @brief 'ntoa' 转换缓冲区大小，必须足够大才能容纳一个转换后的缓冲区
 * 包含填充零的数字（在堆栈上动态创建）
 * 默认值：32 字节
 */
#define XF_PRINTF_NTOA_BUFFER_SIZE             32U

/**
 * @brief 'ftoa' 转换缓冲区大小，必须足够大才能容纳转换后的数据
 * 包含填充零的浮点数（在堆栈上动态创建）
 * 默认值：32 字节
 */
#define XF_PRINTF_FTOA_BUFFER_SIZE             32U

/**
 * @brief 支持浮点类型（%f）
 */
// #define XF_PRINTF_DISABLE_SUPPORT_FLOAT        1
#define XF_PRINTF_SUPPORT_FLOAT                1

/**
 * @brief 支持指数浮点表示法 (%e/%g)
 */
// #define XF_PRINTF_DISABLE_SUPPORT_EXPONENTIAL   1
#define XF_PRINTF_SUPPORT_EXPONENTIAL           1

/**
 * @brief 定义默认浮点精度
 */
#define XF_PRINTF_DEFAULT_FLOAT_PRECISION       6U

/**
 * @brief 定义适合用 %f 打印的最大浮点数
 */
#define XF_PRINTF_MAX_FLOAT                     1e9

/**
 * @brief 支持 long long 类型（%llu 或 %p）
 */
// #define XF_PRINTF_DISABLE_SUPPORT_LONG_LONG     1
#define XF_PRINTF_SUPPORT_LONG_LONG             1

/**
 * @brief 支持 ptrdiff_t 类型 (%t)
 * ptrdiff_t 通常在 <stddef.h> 中定义为 long 或 long long 类型
 */
// #define XF_PRINTF_DISABLE_SUPPORT_PTRDIFF_T     1
#define XF_PRINTF_SUPPORT_PTRDIFF_T             1

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __XF_LOG_CONFIG_H__ */
