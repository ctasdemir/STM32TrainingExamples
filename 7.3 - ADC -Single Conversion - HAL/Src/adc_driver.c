
#include "stm32f0xx_hal.h"
#include "adc_driver.h"

ADC_HandleTypeDef       AdcHandle;
ADC_ChannelConfTypeDef  sConfig;

void adc_driver_init(void)
{
	GPIO_InitTypeDef gpio_init;
	
	//ADC INPUT PIN A0 ==>  GPIOA - 0. pin
	
	// Enable GPIOA Clock

	__HAL_RCC_GPIOA_CLK_ENABLE();	
	
	// enable ADC1 clock

	__HAL_RCC_ADC1_CLK_ENABLE();
	
	// Enable HSI14 Mhz ADC Clock
	
 __HAL_RCC_HSI14ADC_ENABLE();

	// GPIOA - PA0: Select Mode: Analog mode 
	gpio_init.Pin = GPIO_PIN_0;
	gpio_init.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA,&gpio_init);
	
	
	AdcHandle.Instance          = ADC1;
  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
  {
    /* ADC de-initialization Error */
   // Error_Handler();
  }

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;      		/* Synchronous clock mode, input ADC clock with prscaler 2 */
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;            /* 12-bit resolution for converted data */
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;    /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;                       /* Auto-delayed conversion feature disabled */
  AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode enabled (automatic conversion restart after each conversion) */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
  AdcHandle.Init.DMAContinuousRequests = DISABLE;                        /* ADC DMA continuous request to match with DMA circular mode */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.SamplingTimeCommon    = ADC_SAMPLETIME_71CYCLES_5;

  /* 1-  Init ADC */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    //Error_Handler();
  }
  
  
  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
  {
    //Error_Handler();
  }
  
  /* ### - 3 - Channel configuration ######################################## */
  sConfig.Channel      = ADC_CHANNEL_0;               /* Channel to be converted */
  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
 }

int32_t adc_get_result(void)
{
	int32_t result = 0;
	HAL_StatusTypeDef retVal;
	HAL_ADC_Start(&AdcHandle);
	retVal  = HAL_ADC_PollForConversion(&AdcHandle,100);
	if(retVal == HAL_OK)
	{
		result = HAL_ADC_GetValue(&AdcHandle);
	
	}
	else
	{
		return -1; // error
	}
	
	return result;				
}
