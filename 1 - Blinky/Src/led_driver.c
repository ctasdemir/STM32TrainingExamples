#include "led_driver.h"
#include "stm32f0xx_nucleo.h"


int32_t led_status = 0;

void led_init(void)
{
	BSP_LED_Init(LED2);
}


void led_on(void)
{
	BSP_LED_On(LED2);
	led_status = 1;
}
	
void led_off(void)
{
		BSP_LED_Off(LED2);
	led_status= 0;
}
	
uint32_t led_get_status(void)
{
   return led_status;
}
