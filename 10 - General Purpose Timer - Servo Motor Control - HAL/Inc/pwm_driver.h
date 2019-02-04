#ifndef __PWM_DRIVER_H
#define __PWM_DRIVER_H
#include "stm32f0xx_hal.h"
#include <stdint.h>


extern TIM_HandleTypeDef    TimHandle;

void pwm_init(void);
void pwm_enable(void);
void pwm_disable(void);
void pwm_set_duty_cycle(uint32_t duty, uint32_t channel);
void pwm_test(void);

#endif
