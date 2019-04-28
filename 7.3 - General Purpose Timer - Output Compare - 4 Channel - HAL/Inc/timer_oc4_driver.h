/**
  ******************************************************************************
  * @file    timer_oc_driver.h
  * @author  C.T
  * @version 
  * @date    
  * @brief   Timer Output Compare Driver Module Header File
  *******************************************************************************/
#ifndef __OC_DRIVER_H
#define __OC_DRIVER_H

#include "stm32f0xx_hal.h"




void timer_oc4_init(void);
void OC_set_period_cycle(uint32_t period, uint32_t channel);



#endif
