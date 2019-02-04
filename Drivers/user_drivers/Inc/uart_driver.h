/**
  ******************************************************************************
  * @file    uart_driver.h
  * @author  
  * @version 
  * @date    
  * @brief   UART Driver Module Header File
  *******************************************************************************/
#ifndef __UART_DRIVER_H
#define __UART_DRIVER_H
#include "stm32f0xx_hal.h"



void UART_Init(void);
void UART_send_byte_array(char* buffer, int size);
int UART_read_byte(void);
void UART_send_byte(char data);
int UART_bytes_to_read(void);

#endif
