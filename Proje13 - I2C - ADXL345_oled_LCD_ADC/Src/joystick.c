
#include "joystick.h"
#include <stdint.h>
#include "stm32f0xx_hal.h"

int32_t joystick_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
	 
	
	/*
    PA1     ------> UP
    PA4     ------> RIGHT
	  PB0			------> LEFT
	  PC1     ------> ENTER
	  PC0     ------> DOWN
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		
		GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	return 0;
}

JOYSTICK_BUTTON joystick_get_state(void)
{
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1) == GPIO_PIN_RESET)
	{
		return JOYSTICK_UP;
	}
	else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_RESET)
	{
		return JOYSTICK_RIGHT;
	}
	else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == GPIO_PIN_RESET)
	{
		return JOYSTICK_LEFT;
	}
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1) == GPIO_PIN_RESET)
	{
		return JOYSTICK_ENTER;
	}
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0) == GPIO_PIN_RESET)
	{
		return JOYSTICK_DOWN;
	}
	else
	{
		return JOYSTICK_NONE;
	}	
}



