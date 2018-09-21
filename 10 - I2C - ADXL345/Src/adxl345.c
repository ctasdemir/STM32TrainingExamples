#include "stm32f0xx_hal.h"
#include "adxl345.h"




I2C_HandleTypeDef I2cHandle;

uint8_t i2c_buffer[12];



int32_t adxl345_test_connection()
{
	HAL_I2C_Mem_Read(&I2cHandle,ADXL345_ADDRESS,ADXL345_DEVID,1,i2c_buffer,1,2000);
	
	if(i2c_buffer[0] == ADXL345_ID)
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
	HAL_StatusTypeDef ret = HAL_ERROR;

  /*##-1- Configure the I2C peripheral ######################################*/
  I2cHandle.Instance             = I2C1;
  I2cHandle.Init.Timing          = I2C_TIMING;  
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;  
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  
  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    //Error_Handler();
  }

  /* Enable the Analog I2C Filter */
  HAL_I2CEx_ConfigAnalogFilter(&I2cHandle,I2C_ANALOGFILTER_ENABLE);

	
  ret = HAL_I2C_Mem_Read(&I2cHandle,ADXL345_ADDRESS,0,1,i2c_buffer,1,2000);
	
	if(ret == HAL_ERROR)
	{
		return -1;
	}
	
	if(i2c_buffer[0] == ADXL345_ID)
	{
		i2c_buffer[0] = 0x0B;
		HAL_I2C_Mem_Write(&I2cHandle,ADXL345_ADDRESS,ADXL345_DATA_FORMAT,1,i2c_buffer,1,2000);
		i2c_buffer[0] = 0x08;
		HAL_I2C_Mem_Write(&I2cHandle,ADXL345_ADDRESS,ADXL345_POWER_CTL,1,i2c_buffer,1,2000);	
	}
	
	return 0;
}

int32_t adxl345_get_g_values(float *x, float *y, float *z)
{
	int16_t acc_x, acc_y, acc_z;	 
  HAL_StatusTypeDef ret = HAL_ERROR;	
	
	ret = HAL_I2C_Mem_Read(&I2cHandle,ADXL345_ADDRESS,ADXL345_DATAX0,1,i2c_buffer,6,2000);
	if(ret == HAL_OK){
  
	acc_x = ((int16_t)((uint16_t)(i2c_buffer[1]<<8) | (uint16_t)i2c_buffer[0]));
	acc_y = ((int16_t)((uint16_t)(i2c_buffer[3]<<8) | (uint16_t)i2c_buffer[2]));
	acc_z = ((int16_t)((uint16_t)(i2c_buffer[5]<<8) | (uint16_t)i2c_buffer[4]));
		
	*x = acc_x*ADXL345_SCALE_FACTOR; 
	*y = acc_y*ADXL345_SCALE_FACTOR;
  *z = acc_z*ADXL345_SCALE_FACTOR;
	 
		return 0;
	}
	else
	{
		return -1;
	}
}
	
int32_t adxl345_get_raw_values(int16_t *x, int16_t *y, int16_t *z)
{
	HAL_I2C_Mem_Read(&I2cHandle,ADXL345_ADDRESS,ADXL345_DATAX0,1,i2c_buffer,6,2000);
  
	*x = ((int16_t)((uint16_t)(i2c_buffer[1]<<8) | (uint16_t)i2c_buffer[0]));
	*y = ((int16_t)((uint16_t)(i2c_buffer[3]<<8) | (uint16_t)i2c_buffer[2]));
	*z = ((int16_t)((uint16_t)(i2c_buffer[5]<<8) | (uint16_t)i2c_buffer[4]));
	
	return 0;
}


