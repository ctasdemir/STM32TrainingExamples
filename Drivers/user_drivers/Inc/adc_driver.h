/**
  ******************************************************************************
  * @file    adc_driver.h
  * @author  
  * @version 
  * @date    
  * @brief   ADC Driver Module Header File
  *******************************************************************************/
#ifndef __ADC_DRIVER_H
#define __ADC_DRIVER_H

#include <stdint.h>


void adc_driver_init(void);
int32_t adc_get_result(void);


#endif