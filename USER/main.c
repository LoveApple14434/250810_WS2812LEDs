#include "delay.h"
#include "key.h"
#include "led.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
#include "ws2812.h"
#include <stdint.h>
#include <stdio.h>

/************************************************
 ALIENTEK 战舰STM32F103开发板 实验9
 PWM输出实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/

int main(void) {
  uint16_t i;

  HAL_Init();                     // 初始化HAL库
  Stm32_Clock_Init(RCC_PLL_MUL9); // 设置时钟,72M
  delay_init(72);                 // 初始化延时函数
  uart_init(115200);              // 初始化串口
  LED_Init();                     // 初始化LED
  KEY_Init();                     // 初始化按键
  TIM3_PWM_Init(89, 0);           // 72MHz / (89 + 1) = 800kHz, period = 1.25us

  for (i = 0; i < LED_NUM; i++) {
    ws2812_set_color(i, 0, 0, 0);
  }

  uint16_t len = 0;
  uint8_t r_datas[6] = {0};
  while (1) {
    if (USART_RX_STA & 0x8000) {
      len = USART_RX_STA & 0x3fff; // 得到此次接收到的数据长度
      printf("\r\n您发送的消息为:\r\n");
      HAL_UART_Transmit(&UART1_Handler, (uint8_t *)USART_RX_BUF, len,
                        1000); // 发送接收到的数据
      while (__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET)
        ;                 // 等待发送结束
      printf("\r\n\r\n"); // 插入换行
      for (int i = 0; i < 6; ++i) {
        r_datas[i] = USART_RX_BUF[i];
      }
      USART_RX_STA = 0;
      if (len == 6) {
        if (r_datas[0] == 0x34) {
          ws2812_set_color(r_datas[1] << 8 | r_datas[2], r_datas[3], r_datas[4],
                           r_datas[5]);
          printf("data written!\r\n");
          ws2812_show();
        } else printf("data head error!\r\n");
      } else printf("data too long!\r\n");
    }
  }
}
