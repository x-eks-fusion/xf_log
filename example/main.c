#include <stdio.h>
#include "xf_log.h"
#include "xf_log_uitls.h"
#include <time.h>

#define TAG "main"

uint32_t get_current_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
}

static void uart_write(const char *str, size_t len, void *arg)
{
    // 逐字节发送字符串到“串口”
    for (size_t i = 0; i < len; ++i) {
        // 假设这是发送到串口的部分，我们使用putchar模拟
        putchar(str[i]);
    }
}

static void file_write(const char *str, size_t len, void *arg)
{
    const char *file = (const char *)arg;
    FILE *fp = fopen(file, "a+");
    fwrite(str, 1, len, fp);
    fclose(fp);
}

int main(void)
{
    const char *name = "skldfjaslkdfjaslkdfj;asldfja;sldfjasljdflksjdfl;kaj;dlfja;lskdjf;alskjdfaljlasdflj;";
    int date = 20241014;
    float pi = 3.141592;
    int log_uart_id = 0;
    int log_file_id = 0;

    xf_log_set_time_func(get_current_time_ms);

    log_uart_id = xf_log_register_obj(uart_write, NULL);
    xf_log_set_info_level(log_uart_id, XF_LOG_LVL_ERROR);

    log_file_id = xf_log_register_obj(file_write, "./log.log");
    xf_log_set_info_level(log_file_id, XF_LOG_LVL_VERBOSE); // 所有等级打印都带有全部信息
    xf_log_set_filter_colorful_disable(log_file_id);        // 不用彩色打印
    xf_log_set_filter_file(log_file_id, __FILE__);          // 过滤文件名为__FILE__的打印
    xf_log_set_filter_enable(log_file_id);                  // 打开过滤器
    xf_log_set_filter_is_blacklist(log_file_id);            // 设置过滤器为黑名单

    xf_log(XF_LOG_LVL_USER, TAG, __FILE__, __LINE__, __func__, "Hello, %.5s, date: %d, pi: %f!\n", name, date, pi);
    xf_log(XF_LOG_LVL_ERROR, TAG, "file1.c", __LINE__, __func__, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log(XF_LOG_LVL_WARN, TAG, __FILE__, __LINE__, __func__, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log(XF_LOG_LVL_INFO, TAG, "file2.c", __LINE__, __func__, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log(XF_LOG_LVL_DEBUG, TAG, __FILE__, __LINE__, __func__, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log(XF_LOG_LVL_VERBOSE, TAG, "file3.c", __LINE__, __func__, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);

    xf_log_level(XF_LOG_LVL_USER, TAG, "Hello, %.5s, date: %d, pi: %f!\n", name, date, pi);
    xf_log_level(XF_LOG_LVL_ERROR, TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log_level(XF_LOG_LVL_WARN, TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log_level(XF_LOG_LVL_INFO, TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log_level(XF_LOG_LVL_DEBUG, TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    xf_log_level(XF_LOG_LVL_VERBOSE, TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);

    XF_LOGU(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    XF_LOGE(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    XF_LOGW(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    XF_LOGI(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    XF_LOGD(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    XF_LOGV(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);

    return 0;
}
