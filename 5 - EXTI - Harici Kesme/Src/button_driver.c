#include "button_driver.h"
#include "stm32f070xb.h"
#include "stm32f0xx_hal.h"


void button_init()
{
	//BUTTON ==>  GPIOC - 13. pin
	
	// Enable GPIOC Clock
	__HAL_RCC_GPIOC_CLK_ENABLE();	
	
	// Enable SYSCFG Clock (for EXTI)
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	
	// Select Mode: Input
	GPIOC->MODER   &= ~((3UL << 26));  // set 00 ==> Input
	
	// Select Output Type	
  GPIOC->OTYPER  &= ~((1UL <<13));   // 0 ==> Push pull
	
	// Select Speed	
  GPIOC->OSPEEDR &= ~((3UL << 26));  // 00 ==> Low speed
	
	// Pull-up pull-down	
  GPIOC->PUPDR   &= ~((3UL << 26));  // 00 ==> // No pull-up, no pull-down
	
	// Select EXTI_13 ==> GPIOC
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	
	// EXTI13 Interrupt unmask
	EXTI->IMR |= EXTI_IMR_IM13;
	
	// EXTI13 Interrupt falling edge
	EXTI->FTSR |= EXTI_FTSR_TR13;
	
	// set EXTI4_14 Priority on NVIC
	NVIC_SetPriority(EXTI4_15_IRQn,1);

	// Enable EXTI4_15 Interrupt on NVIC
	NVIC_EnableIRQ(EXTI4_15_IRQn);	
}

int32_t button_get_state(void)
{
	//BUTTON ==>  GPIOC - 13. pin
	if( (GPIOC->IDR & GPIO_PIN_13) )
	{
		return 1;
	}
	else
	{
		return 0;
	}	
}


void EXTI4_15_IRQHandler(void)
{
	if((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13)
	{
		EXTI->PR |= EXTI_PR_PR13;
		GPIOA->ODR ^= GPIO_PIN_5;
	}
}
