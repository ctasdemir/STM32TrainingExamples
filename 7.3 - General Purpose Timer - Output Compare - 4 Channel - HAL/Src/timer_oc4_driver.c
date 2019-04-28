#include "timer_oc4_driver.h"

__IO uint32_t uhCCR1_Val = 100; // Kanal-1 periyot değeri
__IO uint32_t uhCCR2_Val = 200; // Kanal-2 periyot değeri
__IO uint32_t uhCCR3_Val = 500; // Kanal-3 periyot değeri
__IO uint32_t uhCCR4_Val = 1000;// Kanal-4 periyot değeri

/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

static void OC_Error_Handler(void);


void timer_oc4_init(void)
{
   GPIO_InitTypeDef   GPIO_InitStruct;
	
  /* -1- Clock 'lari aciyoruz */  
  __HAL_RCC_TIM1_CLK_ENABLE();
  
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*
   * PA.08 (TIM1_Channel1)
     PA.09 (TIM1_Channel2)
     PA.10 (TIM1_Channel3)
     PA.11 (TIM1_Channel4)
     output, push-pull, alternate function mode
  */
  
 /* Timer kanali olarak kullanilacak GPIO Ayarlari */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  	/*##-1- Configure the TIM peripheral */
    /* Initialize TIMx peripheral as follows:
       + Prescaler = (48 Mhz / 1 Khz) - 1 = 47999
       + ClockDivision = 0
       + Counter direction = Up
  */
	
  TimHandle.Instance = TIM1;
  TimHandle.Init.Prescaler         = 47999;
  TimHandle.Init.Period            = 65535;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
  if (HAL_TIM_OC_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    OC_Error_Handler();
  }

  /*##-2- OC Mode Kanal Ayarlari */
  /* T�m kanallar icin ortak ayarlar */
  sConfig.OCMode       = TIM_OCMODE_TOGGLE;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;  

  /* Kanal-1 Pulse (periyot) Degeri */
  sConfig.Pulse = uhCCR1_Val;
  if (HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Error */
    OC_Error_Handler();
  }
	
  /* Kanal-2 Pulse (periyot) Degeri */
  sConfig.Pulse = uhCCR2_Val;
  if (HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Error */
    OC_Error_Handler();
  }
	
  /* Kanal-3 Pulse (periyot) Degeri */
  sConfig.Pulse = uhCCR3_Val;
  if (HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Error */
    OC_Error_Handler();
  }
	
  /* Kanal-4 Pulse (periyot) Degeri */
  sConfig.Pulse = uhCCR4_Val;
  if (HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Error */
    OC_Error_Handler();
  }
	
	  /*##-3- Enable the TIM2 global Interrupt & set priority ####################*/
  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);

 
 /*##-3- Start signals generation #######################################*/ 
  /* Start channel 1 in Output compare mode */
  if(HAL_TIM_OC_Start_IT(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    OC_Error_Handler();
  }
  /* Start channel 2 in Output compare mode */
  if(HAL_TIM_OC_Start_IT(&TimHandle, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Starting Error */
    OC_Error_Handler();
  }
  /* Start channel 3 in Output compare mode */
  if(HAL_TIM_OC_Start_IT(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Starting Error */
    OC_Error_Handler();
  }
  /* Start channel 4 in Output compare mode */
  if(HAL_TIM_OC_Start_IT(&TimHandle, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Starting Error */
    OC_Error_Handler();
  }
}

/**
  * @brief  Output Compare callback in non blocking mode 
  * @param  htim : TIM OC handle
  * @retval None
  */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  uint32_t uhCapture = 0;

  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    uhCapture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    /* Set the Capture Compare Register value */
    __HAL_TIM_SET_COMPARE(&TimHandle, TIM_CHANNEL_1, (uhCapture + uhCCR1_Val));
  }
  

  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    uhCapture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    /* Set the Capture Compare Register value */
    __HAL_TIM_SET_COMPARE(&TimHandle, TIM_CHANNEL_2, (uhCapture + uhCCR2_Val));
  }
  

  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
  {
    uhCapture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);

    /* Set the Capture Compare Register value */
    __HAL_TIM_SET_COMPARE(&TimHandle, TIM_CHANNEL_3, (uhCapture + uhCCR3_Val));
  }
  

  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
    uhCapture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);

    /* Set the Capture Compare Register value */
    __HAL_TIM_SET_COMPARE(&TimHandle, TIM_CHANNEL_4, (uhCapture + uhCCR4_Val));
  }
}

void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TimHandle);
}


/**
  * @brief  Yuzde cinsinden verilen(0-100) duty cycle degerini kanallara atar
  * @param  duty (yuzde degeri)
  * @retval channel (duty cycle degeri ayarlanacak kanal numarasi (1-4)
  */
void OC_set_period_cycle(uint32_t period, uint32_t channel)
{
    switch (channel) {
        case 1:
            uhCCR1_Val = period;
            break;
        case 2:
            uhCCR2_Val = period;
            break;
        case 3:
            uhCCR3_Val = period;
            break;
        case 4:
            uhCCR4_Val = period;
            break;
        default:
            break;
    }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void OC_Error_Handler(void)
{

  while (1)
  {
  }
}
