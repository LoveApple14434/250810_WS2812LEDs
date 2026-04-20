#include "ws2812.h"
#include "timer.h"
#include "delay.h"

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define WS2812_DISABLE_IRQ() __disable_irq()
#define WS2812_ENABLE_IRQ()  __enable_irq()
#else
#define WS2812_DISABLE_IRQ() __asm volatile ("cpsid i" : : : "memory")
#define WS2812_ENABLE_IRQ()  __asm volatile ("cpsie i" : : : "memory")
#endif

uint16_t ws2812_dma_buf[LED_NUM * 24] = {0};

void ws2812_set_color(uint16_t idx, uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t grb;
    uint16_t base;
    uint16_t i;

    if (idx >= LED_NUM)
    {
        return;
    }

    grb = ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;
    base = idx * 24;

    for (i = 0; i < 24; i++)
    {
        ws2812_dma_buf[base + i] = (grb & (1UL << (23 - i))) ? CODE_1 : CODE_0;
    }
}

void ws2812_show(void)
{
    GPIO_InitTypeDef gpio_init;
    DMA_HandleTypeDef *dma_handle;
    uint32_t timeout = 0x1FFFFU;

    WS2812_DISABLE_IRQ();

    HAL_TIM_PWM_Stop_DMA(&TIM3_Handler, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&TIM3_Handler, TIM_CHANNEL_3, 0);

    HAL_TIM_PWM_Start_DMA(&TIM3_Handler, TIM_CHANNEL_3, (uint32_t *)ws2812_dma_buf, LED_NUM * 24);

    dma_handle = TIM3_Handler.hdma[TIM_DMA_ID_CC3];
    while ((dma_handle != NULL) && (HAL_DMA_GetState(dma_handle) != HAL_DMA_STATE_READY) && (timeout > 0U))
    {
        timeout--;
    }

    HAL_TIM_PWM_Stop_DMA(&TIM3_Handler, TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&TIM3_Handler, TIM_CHANNEL_3, 0);

    gpio_init.Pin = GPIO_PIN_0;
    gpio_init.Mode = GPIO_MODE_OUTPUT_OD;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    delay_us(60);

    gpio_init.Mode = GPIO_MODE_AF_PP;
    gpio_init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &gpio_init);

    WS2812_ENABLE_IRQ();
}
