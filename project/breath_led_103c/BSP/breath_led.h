#ifndef __BREATH_LED_H
#define __BREATH_LED_H

// 必须包含STM32 HAL总头文件，才能识别TIM函数、宏
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim2;
extern uint16_t pwm_val;
extern int16_t dir;

void Breath_Led_Running(void);

#endif
