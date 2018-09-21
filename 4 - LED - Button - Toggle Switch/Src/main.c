/**
  ******************************************************************************
  * @file    main.c 
  * @author  C.T
  * @version V1.0.0
  * @date    04-April-2017
  * @brief   Main program body
  ******************************************************************************
  */

#include "main.h"
#include "led_driver.h"
#include "button_driver.h"

static void SystemClock_Config(void);
static void Error_Handler(void);



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	int32_t led_state = 0;
	/* STM32F0xx HAL library initialization:
			 - Configure the Flash prefetch
			 - Systick timer is configured by default as source of time base, but user 
				 can eventually implement his proper time base source (a general purpose 
				 timer for example or other time source), keeping in mind that Time base 
				 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
				 handled in milliseconds basis.
			 - Low Level Initialization
		 */
	HAL_Init();

	/* Configure the system clock to have a system clock = 48 Mhz */
	SystemClock_Config();

	user_led_init();

	button_init();

	/* Infinite loop */
	while (1)
	{
		if(button_get_state() == 0){		
			
			//while(button_get_state() == 0);
			
			if(led_state == 0){
			user_led_set();
			led_state = 1;				
			}				
			else{
			user_led_reset();		
			led_state = 0;						
			}
		}
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            PREDIV                         = 1
  *            PLLMUL                         = 6
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* No HSE Oscillator on Nucleo, Activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState          = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue  = RCC_HSICALIBRATION_DEFAULT;  
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
  }
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
