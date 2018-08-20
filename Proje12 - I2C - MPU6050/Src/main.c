/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @version V1.7.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "UART_driver.h"
#include "led_driver.h"
#include "button_driver.h"
#include "adc_driver.h"
#include "timer_driver.h"
#include "sd_hal_mpu6050.h"


static void SystemClock_Config(void);
static void Error_Handler(void);
static void MX_I2C1_Init(void);
void send_mpu6050_Values();

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

SD_MPU6050 mpu1;
uint32_t result = 0;
void send_adc_string();



/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

	
  HAL_Init();

  /* Configure the system clock to have a system clock = 48 Mhz */
  SystemClock_Config();

	UART_Init();	
	user_led_init();
  button_init();
	timer6_init();
	adc_driver_init();	
	adc_start_calibration();
	//timer6_enable();
	MX_I2C1_Init();
  
	result = SD_MPU6050_Init(&hi2c1,&mpu1,SD_MPU6050_Device_0,SD_MPU6050_Accelerometer_2G,SD_MPU6050_Gyroscope_250s );
	  HAL_Delay(500);
  /* USER CODE END WHILE */
	  if(result == SD_MPU6050_Result_Ok)
	  {
		  printf("MPU6050  OK\n");
	  }
	  else
	  {
		  printf("MPU6050 ERROR!\n");
			//SD_UART_Send(&huart1,mpu_not,(uint16_t)17);
	  }
		HAL_Delay(500);
	
	/* Infinite loop */
  while (1)
  {
			 //	user_led_toggle();
			//timer6_printf_cnt_value();
			send_mpu6050_Values();
			HAL_Delay(100);			
  }
}

void send_mpu6050_Values()
{

	SD_MPU6050_Interrupt interrupt;
	
	
	SD_MPU6050_ReadAll(&hi2c1,&mpu1);
//	SD_MPU6050_ReadInterrupts(&hi2c1,&mpu6050,&interrupt);
	SD_MPU6050_ReadTemperature(&hi2c1,&mpu1);
	
	printf("ACC x:%d y:%d z:%d Gyro x:%d y:%d z:%d\n",mpu1.Accelerometer_X,
																										mpu1.Accelerometer_Y,
																										mpu1.Accelerometer_Z,
																										mpu1.Gyroscope_X,
																										mpu1.Gyroscope_Y,
																										mpu1.Gyroscope_Z);
	
	//printf("%d",interrupt.Status);
	printf("Temp:%f\n",mpu1.Temperature);
}


void send_adc_string()
{

	uint32_t n = 0;
	static uint32_t zaman;
	zaman++;
	n = adc_start_conversion();
	
	//printf("zaman:%d gelen_veri:%d Buton Durum:%d\n\r",zaman,n,button_get_state());
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


/* I2C1 init function */
void MX_I2C1_Init(void)
{ 
	GPIO_InitTypeDef gpio;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
	
	gpio.Pin = GPIO_PIN_8;
	gpio.Mode = GPIO_MODE_AF_OD;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio.Alternate = GPIO_AF1_I2C1;
	
	HAL_GPIO_Init(GPIOB,&gpio);
	gpio.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOB,&gpio);
	
  
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
  HAL_I2C_Init(&hi2c1);

    /**Configure Analogue filter 
    */
  HAL_I2CEx_AnalogFilter_Config(&hi2c1, I2C_ANALOGFILTER_ENABLED);

}