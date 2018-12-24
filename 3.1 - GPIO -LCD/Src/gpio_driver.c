#include "gpio_driver.h"




void GPIO_init_pin(GPIO_TypeDef* port, uint32_t pin, uint16_t speed,uint32_t output_type, uint16_t pull_up_pull_down)
{

}

void GPIO_set_pin_output(GPIO_TypeDef* port, uint32_t pin)
{
  // Select Mode
	port->MODER   &= ~((3UL << pin*2)); // zero bits
  port->MODER   |=  ((1UL << pin*2)); // set 01 (output)
	
	// Select Output Type	
  port->OTYPER  &= ~((1UL <<pin)); // 0 - Push - pull output
	

}
	
void GPIO_set_pin_input(GPIO_TypeDef* port, uint32_t pin)
{
	
}

void GPIO_set_pin_speed(GPIO_TypeDef* port, uint32_t pin, uint16_t speed)
{
		// Select Speed	
  port->OSPEEDR &= ~((3UL << pin*2)); // Zero bits
  port->OSPEEDR |=  ((1UL << pin*2)); // 01 Medium Speed	
}


void GPIO_set_pin_as_alternate(GPIO_TypeDef* port, uint32_t pin)
{
	
}
	
void GPIO_set_pin_pull_up_pull_down(GPIO_TypeDef* port,uint32_t pin, uint16_t pull_up_pull_down)
{
		
	// Pull-up pull-down	
  port->PUPDR   &= ~((3UL << pin*2)); // No pull-up, no pull-down
}	

void GPIO_output_set_pin_value(GPIO_TypeDef* port,uint32_t pin, uint32_t output_value)
{
	if(output_value == 1){
		GPIOA->BSRR = (1<<pin);
	}
	else{
		GPIOA->BSRR = (1<<(pin+16));
	}
}
