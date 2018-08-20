

#include "stm32f0xx_hal.h"


//#define MASTER_BOARD
#define I2C_ADDRESS       ( 0x53<<1)

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 48 MHz */
/* This example use TIMING to 0x00A51314 to reach 1 MHz speed (Rise time = 100 ns, Fall time = 100 ns) */
#define I2C_TIMING      0x00A51314

#define I2Cx                            I2C1
#define RCC_PERIPHCLK_I2Cx              RCC_PERIPHCLK_I2C1
#define RCC_I2CxCLKSOURCE_SYSCLK        RCC_I2C1CLKSOURCE_SYSCLK
#define I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SCL_SDA_AF                 GPIO_AF1_I2C1

I2C_HandleTypeDef I2cHandle;

uint8_t i2c_buffer[12];



int32_t adxl345_test_connection()
{
	HAL_I2C_Mem_Read(&I2cHandle,I2C_ADDRESS,0,1,i2c_buffer,1,2000);
	
	if(i2c_buffer[0] == 0xE5)
	{
	return 0;
	}
	else
	{
		return -1;
	}
	
}

int32_t adxl345_init()
{
	

  /*##-1- Configure the I2C peripheral ######################################*/
  I2cHandle.Instance             = I2Cx;
  I2cHandle.Init.Timing          = I2C_TIMING;
  I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress2     = 0xFF;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  
  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    //Error_Handler();
  }

  /* Enable the Analog I2C Filter */
  HAL_I2CEx_ConfigAnalogFilter(&I2cHandle,I2C_ANALOGFILTER_ENABLE);

	
  HAL_I2C_Mem_Read(&I2cHandle,I2C_ADDRESS,0,1,i2c_buffer,1,2000);
	
	if(i2c_buffer[0] == 0xE5)
	{
	i2c_buffer[0] = 0x0B;
	HAL_I2C_Mem_Write(&I2cHandle,I2C_ADDRESS,0x31,1,i2c_buffer,1,2000);
	i2c_buffer[0] = 0x08;
	HAL_I2C_Mem_Write(&I2cHandle,I2C_ADDRESS,0x2D,1,i2c_buffer,1,2000);
	i2c_buffer[0] = 0x80;
	HAL_I2C_Mem_Write(&I2cHandle,I2C_ADDRESS,0x2E,1,i2c_buffer,1,2000);		
	}
}

int32_t adxl345_get_axis(float* x, float* y, float* z)
{
			
	HAL_I2C_Mem_Read(&I2cHandle,I2C_ADDRESS,0x32,1,i2c_buffer,6,2000);
  
	*x = ((int16_t)((uint16_t)(i2c_buffer[1]<<8) | (uint16_t)i2c_buffer[0]));
	*y = ((int16_t)((uint16_t)(i2c_buffer[3]<<8) | (uint16_t)i2c_buffer[2]));
	*z = ((int16_t)((uint16_t)(i2c_buffer[5]<<8) | (uint16_t)i2c_buffer[4]));
		
	*x *= 0.035;
	*y *= 0.035;
  *z *= 0.035;
	
	return 0;
}

