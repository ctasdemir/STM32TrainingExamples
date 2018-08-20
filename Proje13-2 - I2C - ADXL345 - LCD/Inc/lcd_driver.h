
#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H 

#include "stm32f070xb.h"
#include "stm32f0xx_hal.h"


#define HD44780_RS_PORT				GPIOA
#define HD44780_RS_PIN				GPIO_PIN_9


/* E - Enable pin */

#define HD44780_E_PORT				GPIOC
#define HD44780_E_PIN				GPIO_PIN_7

/* Data pins */
/* D4 - Data 4 pin */

#define HD44780_D4_PORT				GPIOB
#define HD44780_D4_PIN				GPIO_PIN_5

/* D5 - Data 5 pin */

#define HD44780_D5_PORT				GPIOB
#define HD44780_D5_PIN				GPIO_PIN_4

/* D6 - Data 6 pin */

#define HD44780_D6_PORT				GPIOB
#define HD44780_D6_PIN				GPIO_PIN_10

/* D7 - Data 7 pin */

#define HD44780_D7_PORT				GPIOA
#define HD44780_D7_PIN				GPIO_PIN_8




void lcd_init(void);
void lcd_display_on(void);
void lcd_display_off(void);
void lcd_clear(void);
void lcd_put_string(uint8_t x, uint8_t y, char* str);
void lcd_blink_on(void);
void lcd_blink_off(void);
void lcd_cursor_on(void);
void lcd_cursor_off(void);
void lcd_scroll_left(void);
void lcd_scroll_right(void);
void lcd_create_char(uint8_t location, uint8_t* data);
void lcd_put_custom_char(uint8_t x, uint8_t y, uint8_t location);

#endif

