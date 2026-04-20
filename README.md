# STM32F103 WS2812 示例

基于 STM32F103（HAL 库）的 WS2812 灯带驱动示例工程。

- 通过 TIM3 PWM + DMA 输出 WS2812 时序
- 默认控制 64 颗 LED（可在 `HARDWARE/WS2812/ws2812.h` 修改）
- 提供 CMake 构建与 Keil 工程文件

## 目录说明

- `USER/`：主程序、启动与中断、CMake 工程
- `HARDWARE/WS2812/`：WS2812 驱动实现
- `SYSTEM/`：延时、串口、系统初始化
- `HALLIB/`：STM32 HAL 库源码

## 快速构建（CMake）

1. 安装 CMake 与 Keil ARM 编译工具链。
2. 根据本机环境修改 `USER/toolchain.cmake` 中工具链路径。
3. 在项目根目录执行：

```powershell
cmake -S USER -B build
cmake --build build
```

构建完成后可在 `build/` 下看到可执行文件及转换得到的 `.hex` / `.bin` 文件。

## 运行说明

- 主程序在 `USER/main.c`。
- 示例会为 64 颗灯写入渐变颜色并调用 `ws2812_show()` 刷新。
- 将生成的固件下载到开发板后即可观察灯效。
