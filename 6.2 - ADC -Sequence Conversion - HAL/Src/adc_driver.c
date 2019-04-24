
#include "stm32f0xx_hal.h"
#include "adc_driver.h"

volatile int32_t adc_result[4] = {0};
int32_t adc_index =0 ;

ADC_HandleTypeDef       AdcHandle;
ADC_ChannelConfTypeDef  sConfig;
GPIO_InitTypeDef gpio_init;
void adc_handler(ADC_HandleTypeDef* hadc);
void adc_driver_init(void)
{	
	// Enable Clock
	__HAL_RCC_GPIOA_CLK_ENABLE();	
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	// Enable HSI14 Mhz ADC Clock
	__HAL_RCC_HSI14ADC_ENABLE();

	//ADC INPUT PIN ADC_IN0 ==>  GPIOA - 0. pin
	//ADC INPUT PIN ADC_IN1 ==>  GPIOA - 1. pin
	//ADC INPUT PIN ADC_IN4 ==>  GPIOA - 4. pin
	//ADC INPUT PIN ADC_IN8 ==>  GPIOB - 0. pin
	
	gpio_init.Pin = GPIO_PIN_0;
	gpio_init.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.Pin = GPIO_PIN_1;	
	HAL_GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOA,&gpio_init);
	
	gpio_init.Pin = GPIO_PIN_0;	
	HAL_GPIO_Init(GPIOB,&gpio_init);
	
	
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
  AdcHandle.Init.DMAContinuousRequests = DISABLE;                       /* ADC DMA continuous request to match with DMA circular mode */
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
	
	/* ### - 3 - Channel configuration ######################################## */
  sConfig.Channel      = ADC_CHANNEL_1;               /* Channel to be converted */
  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
	
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
	
	/* ### - 3 - Channel configuration ######################################## */
  sConfig.Channel      = ADC_CHANNEL_4;               /* Channel to be converted */
  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
	
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
	
	/* ### - 3 - Channel configuration ######################################## */
  sConfig.Channel      = ADC_CHANNEL_8;               /* Channel to be converted */
  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
	
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }
	
		/* ### - 3 - Channel configuration ######################################## */
  sConfig.Channel      = ADC_CHANNEL_17;               /* Channel to be converted */
  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
	
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    //Error_Handler();
  }

	NVIC_EnableIRQ(ADC1_IRQn);
	NVIC_SetPriority(ADC1_IRQn,0);
	
	HAL_ADC_Start_IT(&AdcHandle);
}


void adc_print_results()
{	
	printf("ADC1=%d ADC2=%d ADC3=%d ADC4=%d\n\r",adc_result[0],adc_result[1],adc_result[2],adc_result[3]);
}







void ADC1_IRQHandler()
{
	if( ((ADC1->ISR & ADC_ISR_EOC) !=0) && (ADC1->IER & ADC_IER_EOCIE) != 0 )
	{
		adc_result[adc_index++] = ADC1->DR;
		
		if( (ADC1->ISR & ADC_ISR_EOS) != 0 )
		{
			adc_index = 0;	
		}	
		
		SET_BIT(ADC1->ISR,ADC_ISR_EOC); // EOC Flag cleared by writing 1 	
	}
}



/*
void ADC1_IRQHandler()
{
	HAL_ADC_IRQHandler(&AdcHandle);
}
*/

/*
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if( __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC))	
	{
		adc_result[adc_index++] = HAL_ADC_GetValue(hadc);
			
		if( __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS))	
		{
	 		adc_index = 0;	
		}		
			
	 }	
}
*/
