# xf_log

轻量级格式化的日志库.

## 特性

本仓库基于 [mpaland/printf](https://github.com/mpaland/printf)(MIT) 制作的一个轻量级log库。有以下特点：
1. 内置 mpaland/printf 的实现。提供轻量级基础的格式化字符串内容
2. 支持不同等级不同颜色输出
3. 可以通过 xf_log_set_backend 修改输出后端
4. 提供函数级 log 等级过滤
5. 支持日志逐字节输出或用数组缓存后输出
6. 错误等级日志可输出文件信息(文件名, 行号, 函数名).

# 开源地址

[github](https://github.com/x-eks-fusion/xf_log)

[gitee](https://gitee.com/x-eks-fusion/xf_log)

# 快速入门

1. 安装 xmake.

   [安装 - xmake](https://xmake.io/#/zh-cn/guide/installation)

1. 运行 linux 示例.

   ```bash
   cd examples/
   clear; xmake clean linux; xmake build linux; xmake run linux
   ```

# 快速移植指南

1. 将 xf_log 所需文件加入编译:

   ```
   # 源文件路径列表
   "*.c",
   "xf_printf/*.c",
   "xf_printf/mpaland_printf/*.c",

   # 头文件路径列表
   ".",
   ```

1. 请定义xf_log_config.h用于配置内容, 详情参考**config_template/xf_log_config_template.h**

2. 对接 xf_log 后端的输出和时间戳函数:

   ```c
   #include <stdio.h>
   #include <time.h>
   #include <unistd.h>

   #include "xf_log_port.h"  /*!< 对接后端 */

   static struct timespec start = {0};

   // log后端，可以重定向到文件内。方便保存log
   // 该函数可以省略，内部有默认对接调用底层的putchar
   static size_t _log_backend(
      char *p_buf, size_t buf_size, xf_log_backend_args_t *p_args)
   {
      (void)(p_args);
      if ((NULL == p_buf) || (0 == buf_size)) { return 0; }
      return printf("%.*s", (int)buf_size, p_buf);
   }

   // 对接log打印时间戳，可以通过XF_LOG_TIMESTAMP_TIME_MS为0，省略这个部分的配置
   static uint32_t _log_time(void)
   {
      struct timespec end;
      clock_gettime(CLOCK_MONOTONIC, &end);
      uint32_t ms = 0;
      ms = 1000 * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / (1000 * 1000);
      return ms;
   }


   ```
3. 调用API开始使用
   ```c
   #include "xf_log.h"
   #include "log_port.h" // 引入上面对接的内容

   static const char* tag = "XF_LOGI";

   int main(void)
   {
      clock_gettime(CLOCK_MONOTONIC, &start);
      xf_log_set_backend(_log_backend); /*!< 设置 log 输出后端，提高IO性能，可以不对接 */
      xf_log_set_time_src(_log_time); /*!< 设置时间戳来源 */
      xf_printf_set_putchar(putchar); /*!< 设置输出来源，串口只需对接它 */


      xf_printf("xf_printf: hello!\r\n");
      XF_LOGI(tag, "hello!");

      return 0;
   }
   ```
