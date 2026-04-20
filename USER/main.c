#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "ws2812.h"
/************************************************
 ALIENTEK 战舰STM32F103开发板 实验9
 PWM输出实验-HAL库函数版 
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
  uint16_t i;

    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	uart_init(115200);					//初始化串口
	LED_Init();							//初始化LED	
	KEY_Init();							//初始化按键
    TIM3_PWM_Init(89, 0);          //72MHz / (89 + 1) = 800kHz, period = 1.25us

  for(i = 0; i < LED_NUM; i++)
  {
    ws2812_set_color(i, i*4, 255-i*4, i<LED_NUM/2?i*4:255-i*4);
  }

    // ws2812_set_color(0, 255, 0, 0);
    // ws2812_set_color(1, 0, 255, 0);
    ws2812_show();

    while(1)
    {
		delay_ms(1000);
    }
}
