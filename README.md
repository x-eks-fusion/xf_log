# xf_log

轻量级格式化的日志库.

# 特性
1. 最低支持c99，无任何仓库依赖
2. 支持不同等级的颜色输出
3. 支持多后端对接，可以同时串口输出和保存成文件
4. 每个后端支持独立的过滤器，可以通过tag，func，level进行过滤
5. 优化字符串格式化，尽量低的缓存和低的IO操作
6. 可以自行配置 xf_log_config.h 减少仓库的占用
7. 支持宏级别的等级屏蔽
8. 自定义输出文件信息(文件名, 行号, 函数名)的level

# 开源地址

[github](https://github.com/x-eks-fusion/xf_log)

[gitee](https://gitee.com/x-eks-fusion/xf_log)

# 快速入门

1. 安装 xmake.

    [安装 - xmake](https://xmake.io/#/zh-cn/guide/installation)

2. 运行 linux 示例.

    ```bash
    cd examples/
    xmake b xf_log; 
    xmake r xf_log
    ```

# 快速移植指南

1. 将 xf_log 所需文件加入编译:

    ```shell
    # 源文件路径列表
    "src/*.c"

    # 头文件路径列表
    "src"
    "src/utils"
    ```

2. 请定义xf_log_config.h用于配置内容, 详情参考**src/xf_log_config_internel.h**
3. 对接后端

    ```c
    int log_uart_id = xf_log_register_obj(uart_write, NULL);        // 对接串口打印
    int log_file_id = xf_log_register_obj(file_write, "./log.log"); // 对接文件保存
    ```

4. 对接时间戳（可选）

    ```c
    xf_log_set_time_func(get_current_time_ms);
    ```

5. 调用API开始使用

    ```c
    XF_LOGU(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    XF_LOGE(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    XF_LOGW(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    XF_LOGI(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    XF_LOGD(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    XF_LOGV(TAG, "Hello, %s, date: %d, pi: %f!\n", name, date, pi);
    ```
