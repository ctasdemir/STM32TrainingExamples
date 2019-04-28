

#include "timer_oc_driver.h"


static void OC_Error_Handler(void);

#define  PERIOD_VALUE       (uint32_t)(20000)  /* Period Value  */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE/2)        /* Capture Compare 1 Value  */
#define  PULSE2_VALUE       (uint32_t)(PERIOD_VALUE*37.5/100) /* Capture Compare 2 Value  */
#define  PULSE3_VALUE       (uint32_t)(PERIOD_VALUE/4)        /* Capture Compare 3 Value  */
#define  PULSE4_VALUE       (uint32_t)(PERIOD_VALUE*12.5/100) /* Capture Compare 4 Value  */

/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;



void timer_oc_init(void)
{
	 GPIO_InitTypeDef   GPIO_InitStruct;
	
  /* -1- Clock 'lari aciyoruz */  
  __HAL_RCC_TIM3_CLK_ENABLE();

  
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* PB.04 (TIM1_Channel1)
     output, push-pull, alternate function mode
  */
  
	/* Timer kanali olarak kullanilacak GPIO Ayarlari */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  	/*##-1- Configure the TIM peripheral */
  /* -----------------------------------------------------------------------
  TIM1 Configuration: generate 4 PWM signals with 4 different duty cycles.

    In this example TIM1 input clock (TIM1CLK) is set to APB1 clock (PCLK1),
    since APB1 prescaler is equal to 1.
      TIM1CLK = PCLK1
      PCLK1 = HCLK
      => TIM1CLK = HCLK = SystemCoreClock (48 Mhz)

    TIM1 Counter Clock ==> 24 Mhz 
       Prescaler = (TIM1CLK / TIM1 counter clock) - 1
       Prescaler = ((SystemCoreClock) /24 MHz) - 1
			 Prescaler = 1

       
		TIM1 output clock ==> 24 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM1 counter clock / TIM1 output clock) - 1
           = (24 Mhz / 24 Khz ) -1 
					 = 999

    TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR + 1)* 100 = 50%
    TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR + 1)* 100 = 37.5%
    TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR + 1)* 100 = 25%
    TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR + 1)* 100 = 12.5%

   
  ----------------------------------------------------------------------- */

  /* Initialize TIMx peripheral as follows:
       + Prescaler = (48 Mhz / 1 Mhz) - 1 = 47 
       + Period = (20000) = 20 ms
       + ClockDivision = 0
       + Counter direction = Up
  */
	
  TimHandle.Instance = TIM3;

  TimHandle.Init.Prescaler         = 47999;
  TimHandle.Init.Period            = 500;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
  if (HAL_TIM_OC_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    OC_Error_Handler();
  }

  /*##-2- PWM Kanal Ayarlari */
  /* Tüm kanallar icin ortak ayarlar */
  sConfig.OCMode       = TIM_OCMODE_TOGGLE;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;  

  /* Kanal-1 Duty Cycle Degeri */
  sConfig.Pulse = 500;
  if (HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Error */
    OC_Error_Handler();
  }

 
  /* -3- OC sinyali uretmeye basliyoruz */
  
	/* Kanal 1'i baslat */
  if (HAL_TIM_OC_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Error */
    OC_Error_Handler();
  }
}




/**
  * @brief  Yuzde cinsinden verilen(0-100) duty cycle degerini kanallara atar
  * @param  duty (yuzde degeri)
  * @retval channel (duty cycle degeri ayarlanacak kanal numarasi (1-4)
  */
void OC_set_period_cycle(uint32_t period)
{
	

	

__HAL_TIM_SET_COMPARE(&TimHandle, TIM_CHANNEL_1, period);
		
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
