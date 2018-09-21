/**
  ******************************************************************************
  * @file    led_driver.h
  * @author  C.TASDEMIR
  * @version 
  * @date    
  * @brief   LED driver routines
  *******************************************************************************/
#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H
#include <stdint.h>



void led_init(void);
void led_on(void);
void led_off(void);
uint32_t led_get_status(void);

#endif
