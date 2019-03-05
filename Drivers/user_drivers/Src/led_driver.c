#include "stm32f070xb.h"
#include "stm32f0xx_hal.h"


void user_led_init()
{
	//LED2 ==>  GPIOA - 5. pin
	GPIO_InitTypeDef gpio_init;
		
	// Enable Clock
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	
	gpio_init.Pin = GPIO_PIN_8;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_LOW;
	
	HAL_GPIO_Init(GPIOC,&gpio_init);
}


void user_led_toggle()
{
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_8);
}


void user_led_set()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
	
}

void user_led_reset()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
}


