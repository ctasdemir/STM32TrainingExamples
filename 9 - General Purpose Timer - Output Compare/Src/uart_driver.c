#include "button_driver.h"
#include "stm32f0xx_hal.h"
#include "uart_buffer.h"


UART_HandleTypeDef UartHandle;

static void UART_Error_Handler(void);


/*  
  UART_Init()
  UART'i proje specifikasyonunda belirtilen hiz ve
  ayarlarla baslatir. 
  RX kesmelerini aktif hale getirir.

*/
void UART_Init()
{
	
GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */  
	__HAL_RCC_GPIOA_CLK_ENABLE();

  /* Enable USARTx clock */
  __HAL_RCC_USART2_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
	
	
  GPIO_InitStruct.Pin       = GPIO_PIN_2;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
    
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    

/*##-3- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USART2;
	
  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; 
	
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    //ERROR
		UART_Error_Handler();
  }  
	
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
		//ERROR
    UART_Error_Handler();
  }
	
/* 4- Enable UART Receive Data Register Not Empty */
   SET_BIT(USART2->CR1, USART_CR1_RXNEIE);

	
	/* 5 - Enable UART Interrupt in NVIC */
	
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}




/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void UART_Error_Handler(void)
{
     while(1)
    {      
  
    }
}


void send_time_string()
{

	uint32_t n = 0;
	static uint32_t zaman;
	zaman++;
	
	n = UART_bytes_to_read();
	printf("zaman:%d gelen_veri:%d Buton Durum:%d\n\r",zaman,n,button_get_state());
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  UART_send_char(ch);

  return ch;
}


/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA 
  *         used for USART data transmission     
  */
void USART2_IRQHandler(void)
{  
  uint32_t isrflags = USART2->ISR;
  uint32_t control_reg1 = USART2->CR1;
  uint32_t rx_data = 0;
	
	
    /* UART in mode Receiver */
    if(((isrflags & USART_ISR_RXNE) != RESET) && ((control_reg1 & USART_CR1_RXNEIE) != RESET))
    {
			rx_data = (uint16_t) USART2->RDR;
			
			/* Read one byte from the receive data register */ 
			
			UART_BufferRX.buffer[UART_BufferRX.head_pointer] = rx_data;
			
			UART_BufferRX.head_pointer = UART_BufferRX.head_pointer + 1;
			
			if(UART_BufferRX.head_pointer == BUFFER_SIZE)
			{
				UART_BufferRX.head_pointer = 0;
			}
			
      return;
    }  


  /* UART in mode Transmitter */
  if(((isrflags & USART_ISR_TXE) != RESET) && ((control_reg1 & USART_CR1_TXEIE) != RESET))
  {    
		if(UART_BufferTX.head_pointer != UART_BufferTX.tail_pointer)
		{
			// Send one byte from Transmit buffer
			USART2->TDR = UART_BufferTX.buffer[UART_BufferTX.tail_pointer++];
			
			if(UART_BufferTX.tail_pointer == BUFFER_SIZE)
			{
				UART_BufferTX.tail_pointer = 0;
			}			
		}
		else
		{
		/* Disable the UART Transmit Data Register Empty Interrupt */
      CLEAR_BIT(USART2->CR1, USART_CR1_TXEIE);
		}			
		
    return;
  }
	
}


