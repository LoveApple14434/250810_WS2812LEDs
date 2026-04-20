cmake_minimum_required(VERSION 4.0)

# 包含工具链配置
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake)

project(PWM LANGUAGES C ASM)

# 添加可执行文件
add_executable(${PROJECT_NAME}
    ./main.c
    ./stm32f1xx_it.c
    ./system_stm32f1xx.c
    ./stm32f1xx_hal_msp.c
    ../CORE/startup_stm32f103xe.s
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_usart.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
    ../HALLIB/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
    ../SYSTEM/delay/delay.c
    ../SYSTEM/sys/sys.c
    ../SYSTEM/usart/usart.c
    ../HARDWARE/LED/led.c
    ../HARDWARE/KEY/key.c
    ../HARDWARE/TIMER/timer.c
    ../HARDWARE/WS2812/ws2812.c
)

# 添加自定义目标用于生成二进制文件 (使用 POST_BUILD)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_FROMELF} --i32combined --output="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}${PROJECT_NAME}.hex" "$<TARGET_FILE:${PROJECT_NAME}>"
    COMMENT "Generating HEX file"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_FROMELF} --bin --output="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}${PROJECT_NAME}.bin" "$<TARGET_FILE:${PROJECT_NAME}>"
    COMMENT "Generating BIN file"
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ../CORE
    ../USER
    ../OBJ
    ../SYSTEM/delay
    ../SYSTEM/sys
    ../SYSTEM/usart
    ../HALLIB/STM32F1xx_HAL_Driver/Inc
    ../HARDWARE/LED
    ../HARDWARE/KEY
    ../HARDWARE/TIMER
    ../HARDWARE/WS2812
)

# 预处理器定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    USE_HAL_DRIVER
    STM32F103xE
)

# 链接器脚本设置
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/Template.sct")

# 链接选项
target_link_options(${PROJECT_NAME} PRIVATE
    ${LINKER_FLAGS}
    "--scatter=${LINKER_SCRIPT}"
    
)
