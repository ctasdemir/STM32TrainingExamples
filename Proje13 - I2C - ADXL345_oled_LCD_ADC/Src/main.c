#include "main.h"
#include "adxl345.h"
#include "ssd1306.h"
#include "joystick.h"
#include "adc_driver.h"



void SystemClock_Config(void);

static void Error_Handler(void);

void write_acc_values(void);

float x,y,z;
char str[50];

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
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

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

  /* Configure LED2 */
  BSP_LED_Init(LED2);
	
	adxl345_init();
	joystick_init();
	adc_driver_init();
  adc_start_calibration();
	
	ssd1306_Init();
  HAL_Delay(1000);
  ssd1306_Fill(White);
  ssd1306_UpdateScreen();
	
  /* Infinite loop */
  while (1)
  {
		write_acc_values();
		HAL_Delay(10);

  }
}




void write_acc_values()
{
	JOYSTICK_BUTTON btn;
	uint32_t n = 0;
	
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString(" ivmeolcer",Font_11x18,Black);
	adxl345_get_axis(&x,&y,&z);
	
	ssd1306_SetCursor(0,20);
	sprintf(str,"x:%f",x);
	ssd1306_WriteString(str,Font_7x10,Black);
	
	ssd1306_SetCursor(0,30);
	sprintf(str,"y:%f",y);
	ssd1306_WriteString(str,Font_7x10,Black);
	
	ssd1306_SetCursor(0,40);
	sprintf(str,"z:%f",z);
	ssd1306_WriteString(str,Font_7x10,Black);
	
	
	btn = joystick_get_state();
	ssd1306_SetCursor(0,50);
	
	switch(btn){
	case JOYSTICK_DOWN:
	ssd1306_WriteString("BUTTON DOWN",Font_7x10,Black);
	break;
	case JOYSTICK_UP:
	ssd1306_WriteString("BUTTON UP",Font_7x10,Black);
	break;
	case JOYSTICK_LEFT:
	ssd1306_WriteString("BUTTON LEFT",Font_7x10,Black);
	break;
	case JOYSTICK_RIGHT:
	ssd1306_WriteString("BUTTON RIGHT",Font_7x10,Black);
	break;
	case JOYSTICK_ENTER:
	ssd1306_WriteString("BUTTON ENTER",Font_7x10,Black);
	break;
	case JOYSTICK_NONE:
	//ssd1306_WriteString("            ",Font_7x10,Black);
	break;
	}
	
	n = adc_start_conversion();
	ssd1306_SetCursor(0,50);
	
	
	sprintf(str,"adc:%04d",n);
	ssd1306_WriteString(str,Font_7x10,Black);
	ssd1306_UpdateScreen();
	
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
  *            PREDIV                         = 2
  *            PLLMUL                         = 12
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* No HSE Oscillator on Nucleo, Activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}
/**
  * @brief  I2C error callbacks.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /** Error_Handler() function is called when error occurs.
    * 1- When Slave don't acknowledge it's address, Master restarts communication.
    * 2- When Master don't acknowledge the last data transferred, Slave don't care in this example.
    */
  if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
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
  /* Error if LED2 is slowly blinking (1 sec. period) */
  while(1)
  {    
    BSP_LED_Toggle(LED2); 
    HAL_Delay(1000);
  } 
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
