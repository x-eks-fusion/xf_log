/**
 * @file main.c
 * @author catcatBlue (catcatblue@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-06-20
 *
 * @copyright Copyright (c) 2024
 *
 */

/* ==================== [Includes] ========================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>

#include "xf_log.h"
#include "xf_log_port.h"

/* ==================== [Defines] =========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/**
 * @brief xf_log 输出后端函数。
 *
 * @param p_buf xf_log 传入的想要输出的缓冲区指针。
 * @param buf_size 缓冲区大小。
 * @param p_args xf_log 信息结构体。
 * @return size_t
 *
 * @note 见 @ref xf_log_backend_t.
 */
static size_t _log_backend(
    char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args);

static uint32_t _log_time(void);

static void test_xf_test_assert(void);
static void test_log_hello(void);
static void test_log_level(void);
static void test_log_change_output(void);

/* ==================== [Static Variables] ================================== */

static const char *TAG = "main";

static char s_log_buf[512] = {0};
static size_t s_log_buf_idx = 0;

static struct timespec start = {0};

/* ==================== [Macros] ============================================ */

#define XF_TEST_ASSERT(EXPR) \
    if (!(EXPR)) { \
        XF_LOG_WITH_EXTRA(XF_LOG_LVL_ERROR, "assert", "\"%s\" has assert failed.", #EXPR); \
        /* while (1); */ \
    }

/* ==================== [Global Functions] ================================== */

int main(void)
{
    clock_gettime(CLOCK_MONOTONIC, &start);
    xf_log_set_backend(_log_backend);
    xf_log_set_time_src(_log_time);
    xf_printf_set_putchar(putchar);
    test_xf_test_assert();
    usleep(1000 * 10); /*!< Linux 上运行太快可能会看不到时间戳变化, 手动延迟  */
    test_log_hello();
    usleep(1000 * 10);
    test_log_level();
    usleep(1000 * 10);
    test_log_change_output();

    return 0;
}

/* ==================== [Static Functions] ================================== */

static size_t _log_backend(
    char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args)
{
    UNUSED(p_args);
    if ((NULL == p_buf) || (0 == buf_size)) {
        return 0;
    }
    return printf("%.*s", (int)buf_size, p_buf);
}

static size_t _log_backend_to_buf(
    char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args)
{
    UNUSED(p_args);
    if ((NULL == p_buf) || (0 == buf_size)) {
        return 0;
    }
    int ssize = (int)buf_size;
    if (s_log_buf_idx + buf_size > sizeof(s_log_buf)) {
        ssize = sizeof(s_log_buf) - s_log_buf_idx;
    }
    if (0 >= ssize) {
        return 0;
    }
    memcpy(s_log_buf + s_log_buf_idx, p_buf, ssize);
    s_log_buf_idx += ssize;
    return ssize;
}

static uint32_t _log_time(void)
{
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    uint32_t ms = 0;
    ms = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / (1000 * 1000);
    return ms;
}

static void test_xf_test_assert(void)
{
    XF_TEST_ASSERT(1 == 1);
    XF_TEST_ASSERT(1 == 2);
}

static void test_log_hello(void)
{
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_USER, TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_ERROR, TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_WARN, TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_DEBUG, TAG, "hello: 测试格式化输出: int: %d, str: %s, float: %f", 123, "abc", -123.456f);

    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, TAG, "hello: 二进制输出: bin: 0b%08b", 0x2a);

    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, TAG, "一共有错误 Error、警告 Warn、信息 Info、调试 Debug、无 None 五个主要等级");
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_USER, TAG, "额外有一个用户 User 等级");
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, TAG, "默认的等级是 Info");
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, TAG, "用户 User 等级不可通过 XF_LOG_MINIMUM_LEVEL 被关闭");
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, TAG, "用户 User 和信息 Info 等级最大的区别是：");
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_USER, TAG,
            "用户 User 和错误 Error 等级在输出固定格式时也会自带文件名、行号、函数名信息");

    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, TAG, "测试关闭输出时间戳");
    xf_log_set_time_src(NULL);
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, TAG, "hello");
    xf_log_set_time_src(_log_time);
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, TAG, "重新打开输出时间戳");
}

static void test_log_level(void)
{
    xf_printf("测试输出等级" XF_LOG_NEWLINE);
    int lvl_backup = xf_log_get_global_level();
    for (int lvl = XF_LOG_LVL_NONE; lvl <= XF_LOG_LVL_USER; lvl++) {
        xf_log_set_global_level(lvl);
        xf_printf("--- 输出开始 --- lvl: %u ---" XF_LOG_NEWLINE, lvl);
        XF_LOG_WITH_EXTRA(XF_LOG_LVL_USER, TAG, "hello");
        XF_LOG_WITH_EXTRA(XF_LOG_LVL_ERROR, TAG, "hello");
        XF_LOG_WITH_EXTRA(XF_LOG_LVL_WARN, TAG, "hello");
        XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, TAG, "hello");
        XF_LOG_WITH_EXTRA(XF_LOG_LVL_DEBUG, TAG, "hello");
        xf_printf("--- 输出结束 ---------------" XF_LOG_NEWLINE XF_LOG_NEWLINE);
    }
    xf_log_set_global_level(lvl_backup);
}

static void test_log_change_output(void)
{
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, "change_out", "测试改变输出接口");
    xf_log_set_time_src(NULL);
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, "change_out", "关闭时间戳方便比对字符串，以下是用于对比的字符串");
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, "change_out", "hello: int: %d, str: %s", 123, "abc");

    xf_log_set_backend(_log_backend_to_buf);
    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, "change_out", "hello: int: %d, str: %s", 123, "abc");
    xf_log_set_backend(NULL); /*!< 改回默认的输出 */

    xf_log_set_time_src(_log_time);

    xf_printf("检查改变结果：" XF_LOG_NEWLINE);
    xf_printf("------------------------------------------------------------" XF_LOG_NEWLINE);
    xf_printf("%s", s_log_buf);
    xf_printf("------------------------------------------------------------" XF_LOG_NEWLINE);

    XF_LOG_WITH_EXTRA(XF_LOG_LVL_INFO, "change_out", "hello: int: %d, str: %s", 123, "abc");
    xf_printf("如果上一行还有重复的 I (none)-change_out 输出，说明还原成功" XF_LOG_NEWLINE);
}

