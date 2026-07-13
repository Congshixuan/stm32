#include "stm32f1xx_hal.h"
#include "breath_led.h"

void Breath_Led_Running(void)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm_val);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pwm_val);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, pwm_val);

    pwm_val += dir;

    if(pwm_val >= 1000)
    {
        dir = -20;
    }
    if(pwm_val <= 0)
    {
        dir = 20;
    }

    HAL_Delay(5);
}
