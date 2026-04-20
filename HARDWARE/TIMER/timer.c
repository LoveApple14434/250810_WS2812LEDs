#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//定时器中断驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/5/25
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
TIM_HandleTypeDef 	TIM3_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef 	TIM3_CH3Handler;		//定时器3通道3句柄
DMA_HandleTypeDef  hdma_tim3_ch3;      //定时器3通道3 DMA句柄

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!



void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}

//TIM3 PWM部分初始化 
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;          	//定时器3
    TIM3_Handler.Init.Prescaler=psc;       //定时器分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM3_Handler.Init.Period=arr;          //自动重装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
    
	TIM3_CH3Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
	TIM3_CH3Handler.Pulse=0;                //默认拉低，发送时由DMA更新CCR
	TIM3_CH3Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为高
	HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH3Handler,TIM_CHANNEL_3);//配置TIM3通道3
	
	HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_3);//开启PWM通道3
	 	   
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
        __HAL_RCC_DMA1_CLK_ENABLE();           //使能DMA1时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
		
        GPIO_Initure.Pin=GPIO_PIN_0;           	//PB0
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
        HAL_GPIO_Init(GPIOB,&GPIO_Initure);

        hdma_tim3_ch3.Instance = DMA1_Channel2;
        hdma_tim3_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_tim3_ch3.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_tim3_ch3.Init.MemInc = DMA_MINC_ENABLE;
        hdma_tim3_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim3_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_tim3_ch3.Init.Mode = DMA_NORMAL;
        hdma_tim3_ch3.Init.Priority = DMA_PRIORITY_HIGH;
        HAL_DMA_Init(&hdma_tim3_ch3);
        __HAL_LINKDMA(&TIM3_Handler,hdma[TIM_DMA_ID_CC3],hdma_tim3_ch3);
	}
}

//设置TIM通道3的占空比
//compare:比较值
void TIM_SetTIM3Compare3(u32 compare)
{
    TIM3->CCR3=compare; 
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    (void)htim;
}
