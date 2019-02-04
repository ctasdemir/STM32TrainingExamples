/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "UART_driver.h"
#include "led_driver.h"
#include "button_driver.h"
#include "adc_driver.h"



extern ADC_HandleTypeDef  AdcHandle;
volatile int adc_value;
volatile int adc_ready;

static void SystemClock_Config(void);
static void Error_Handler(void);
void send_adc_string(void);



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  HAL_Init();

  /* system clock = 48 Mhz */
  SystemClock_Config();

	UART_Init();	
	user_led_init();
  button_init();
	adc_driver_init();
	HAL_ADC_Start_IT(&AdcHandle);

	/* Infinite loop */
  while (1)
  {
			user_led_toggle();
		//	send_adc_string();
		
		if(adc_ready == 1)
		{
			adc_ready = 0;
			printf("ADC val == %d\r\n", adc_value);
		}
			HAL_Delay(100);			
  }
}

void send_adc_string(void)
{
	uint32_t adc = 0;
	float voltage = 0.0;
	static uint32_t time;
	time++;
	adc = adc_get_result();
	voltage = (adc / 4095.0) * 3.3;
	
	printf("zaman:%d ADC Verisi:%d %2.2f V Buton Durum:%s\n\r",time,adc,voltage,button_get_state()?"OFF":"ON");
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1)
{

 adc_value = HAL_ADC_GetValue(hadc1);
 adc_ready = 1;

 HAL_ADC_Start_IT(hadc1); // Re-Start ADC1 under Interrupt
                         // this is necessary because we don'use
                         // the Continuos Conversion Mode
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
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}
