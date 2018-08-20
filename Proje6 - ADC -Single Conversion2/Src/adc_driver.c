
#include "stm32f0xx_hal.h"
#include "adc_driver.h"



void adc_driver_init(void)
{
	
	//ADC INPUT PIN A0 ==>  GPIOA - 0. pin
	
	
	
	// Enable Clock
	__HAL_RCC_GPIOA_CLK_ENABLE();	
	
	// Select Mode: Input
	GPIOA->MODER   |= (3UL<<2 );  // set 11 ==> Analog mode	
	

	
		// enable ADC1 clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 
	
	// enable ADC 14 Mhz Clock
	RCC->CR2 |= RCC_CR2_HSI14ON; 
	
	// wait for HSI 14 MHZ Clock ready
	while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) 
	{
	
	}
	// Enable ADC
//	ADC1->CR |= ADC_CR_ADEN;
	
	ADC1->CFGR2 &= (~ADC_CFGR2_CKMODE); 
  
	// Channel Selection
	ADC1->CHSELR = ADC_CHSELR_CHSEL1 | ADC_CHSELR_CHSEL17; 
	ADC1->SMPR |= 7; // 111  	
	
	// Enable VREF
	ADC->CCR |= ADC_CCR_VREFEN; 
}

int32_t adc_start_conversion(void)
{
	int32_t result = 0;

	/* Performs the AD conversion */
	ADC1->CR |= ADC_CR_ADSTART; /* Start the ADC conversion */
		
	while ((ADC1->ISR & ADC_ISR_EOC) == 0) /* Wait end of conversion */	
	{
		/* For robust implementation, add here time-out management */
	}
			
	result = ADC1->DR; /* Store the ADC conversion result */	
		
	return result;
}

int32_t adc_return_conversion_result(void)
{
	return 0;
}

void adc_start_calibration(void)
{
	if ((ADC1->CR & ADC_CR_ADEN) != 0) 
	{
		ADC1->CR &= (uint32_t)(~ADC_CR_ADEN); 
	}
	
	ADC1->CR &= (uint32_t)(~ADC_CR_ADEN);

	ADC1->CR |= ADC_CR_ADCAL; 
	
	while ((ADC1->CR & ADC_CR_ADCAL) != 0) 
	{
	
	}
	
	// Enable ADC
	ADC1->CR |= ADC_CR_ADEN;
}
