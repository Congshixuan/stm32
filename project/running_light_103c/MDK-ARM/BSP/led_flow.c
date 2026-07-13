#include "stm32f1xx_hal.h"
#include "main.h"    // ÐÂÔö£¬¼ÓÔØLEDÒý½Åºê
#include "led_flow.h"

void LED_Flow_Running(void)
{
    // ºìµÆÁÁ£¬ÂÌÀ¶Ãð£¬ÑÓÊ±500ms
    HAL_GPIO_WritePin(GPIOA, LED_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, LED_G_Pin | LED_B_Pin, GPIO_PIN_SET);
    HAL_Delay(500);

    // ÂÌµÆÁÁ£¬ºìÀ¶Ãð£¬ÑÓÊ±500ms
    HAL_GPIO_WritePin(GPIOA, LED_G_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, LED_R_Pin | LED_B_Pin, GPIO_PIN_SET);
    HAL_Delay(500);

    // À¶µÆÁÁ£¬ºìÂÌÃð£¬ÑÓÊ±500ms
    HAL_GPIO_WritePin(GPIOA, LED_B_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, LED_R_Pin | LED_G_Pin, GPIO_PIN_SET);
    HAL_Delay(500);
}