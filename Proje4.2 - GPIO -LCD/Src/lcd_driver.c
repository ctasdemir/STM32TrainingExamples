#include "lcd_driver.h"
#include "gpio_driver.h"

/* Private functions */
static void lcd_init_pins(void);
static void lcd_send_command(uint8_t cmd);
static void lcd_send_command_4bit(uint8_t cmd);
static void lcd_send_data(uint8_t data);
static void lcd_set_cursor(uint8_t col, uint8_t row);



/* Pin definitions */
#define HD44780_RS_CMD             HAL_GPIO_WritePin(HD44780_RS_PORT,HD44780_RS_PIN,GPIO_PIN_RESET)
#define HD44780_RS_DATA            HAL_GPIO_WritePin(HD44780_RS_PORT,HD44780_RS_PIN,GPIO_PIN_SET)
#define HD44780_E_LOW              HAL_GPIO_WritePin(HD44780_E_PORT,HD44780_E_PIN,GPIO_PIN_RESET) 
#define HD44780_E_HIGH             HAL_GPIO_WritePin(HD44780_E_PORT,HD44780_E_PIN,GPIO_PIN_SET) 

#define HD44780_E_TOGGLE            HD44780_E_HIGH; lcd_delay(1); HD44780_E_LOW; lcd_delay(1)
#define lcd_delay(x)            		HAL_Delay(x)

/* Commands*/
#define HD44780_CLEARDISPLAY        0x01
#define HD44780_RETURNHOME          0x02
#define HD44780_ENTRYMODESET        0x04
#define HD44780_DISPLAYCONTROL      0x08
#define HD44780_CURSORSHIFT         0x10
#define HD44780_FUNCTIONSET         0x20
#define HD44780_SETCGRAMADDR        0x40
#define HD44780_SETDDRAMADDR        0x80

/* Flags for display entry mode */
#define HD44780_ENTRYRIGHT          0x00
#define HD44780_ENTRYLEFT           0x02
#define HD44780_ENTRYSHIFTINCREMENT 0x01
#define HD44780_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define HD44780_DISPLAYON           0x04
#define HD44780_CURSORON            0x02
#define HD44780_BLINKON             0x01

/* Flags for display/cursor shift */
#define HD44780_DISPLAYMOVE         0x08
#define HD44780_CURSORMOVE          0x00
#define HD44780_MOVERIGHT           0x04
#define HD44780_MOVELEFT            0x00

/* Flags for function set */
#define HD44780_8BITMODE            0x10
#define HD44780_4BITMODE            0x00
#define HD44780_2LINE               0x08
#define HD44780_1LINE               0x00
#define HD44780_5x10DOTS            0x04
#define HD44780_5x8DOTS             0x00

void lcd_init(void) {

	
	//Initialize the outputs.
	HAL_Delay(50);

	/* Init pins */
	lcd_init_pins();
	
	/* At least 40ms */
	lcd_delay(45);
	
		/* Set  4bit mode */
	lcd_send_command_4bit(0x3);
	lcd_delay(45);
	
	/* Second try */
	lcd_send_command_4bit(0x3);
	lcd_delay(45);
	
	/* Third goo! */
	lcd_send_command_4bit(0x3);
	lcd_delay(45);	
	
	/* Set 4-bit interface */
	lcd_send_command_4bit(0x2);
	lcd_delay(100);
	
	/* Set # lines, font size, etc. */
	lcd_send_command(HD44780_FUNCTIONSET|HD44780_2LINE|HD44780_5x8DOTS);

	/* Turn the display on with no cursor or blinking default */
	
	lcd_display_on();
	lcd_delay(45);
	/* Clear lcd */
	lcd_clear();

	/* Default font directions */
	
	lcd_send_command(HD44780_ENTRYMODESET |HD44780_ENTRYLEFT );


	
	
} 

void lcd_clear(void) {
	lcd_send_command(HD44780_CLEARDISPLAY);
	lcd_delay(3);
}

void lcd_put_string(uint8_t x, uint8_t y, char* str) {
	lcd_set_cursor(x, y);
	while (*str) {
		lcd_send_data(*str);
		str++;
	}
/*		
	while (*str) {

		if (HD44780_Opts.currentX >= HD44780_Opts.Cols) {
			HD44780_Opts.currentX = 0;
			HD44780_Opts.currentY++;
			TM_HD44780_CursorSet(HD44780_Opts.currentX, HD44780_Opts.currentY);
		}
		if (*str == '\n') {
			HD44780_Opts.currentY++;
			TM_HD44780_CursorSet(HD44780_Opts.currentX, HD44780_Opts.currentY);
		} else if (*str == '\r') {
			TM_HD44780_CursorSet(0, HD44780_Opts.currentY);
		} else {
			lcd_send_data(*str);
			HD44780_Opts.currentX++;
		}
		str++;
	}
		*/
}

void lcd_display_on(void) {
	
	lcd_send_command(HD44780_DISPLAYCONTROL|HD44780_DISPLAYON);
}

void lcd_display_off(void) {
	
	lcd_send_command(HD44780_DISPLAYCONTROL);
}

void lcd_blink_on(void) {
	
	lcd_send_command(HD44780_DISPLAYCONTROL);
}

void lcd_blink_off(void) {
	
	lcd_send_command(HD44780_DISPLAYCONTROL);
}

void lcd_blink_cursor_on(void) {
	
	lcd_send_command(HD44780_DISPLAYCONTROL);
}

void lcd_blink_cursor_off(void) {
	
	lcd_send_command(HD44780_DISPLAYCONTROL);
}

void lcd_scroll_left(void) {
	lcd_send_command(HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVELEFT);
}

void lcd_scroll_right(void) {
	lcd_send_command(HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVERIGHT);
}

void lcd_create_char(uint8_t location, uint8_t *data) {
	uint8_t i;
	/* We have 8 locations available for custom characters */
	location &= 0x07;
	lcd_send_command(HD44780_SETCGRAMADDR | (location << 3));
	
	for (i = 0; i < 8; i++) {
		lcd_send_data(data[i]);
	}
}

void lcd_put_custom_char(uint8_t x, uint8_t y, uint8_t location) {
	lcd_set_cursor(x, y);
	lcd_send_data(location);
}

/* Private functions */
static void lcd_send_command(uint8_t cmd) {
	/* Command mode */
	HD44780_RS_CMD;
	
	/* High nibble */
	lcd_send_command_4bit(cmd >> 4);
	/* Low nibble */
	lcd_send_command_4bit(cmd & 0x0F);
}

static void lcd_send_data(uint8_t data) {
	
	/* Data mode */
	HD44780_RS_DATA;
	
	/* High nibble */
	lcd_send_command_4bit(data >> 4);
	
	/* Low nibble */
	lcd_send_command_4bit(data & 0x0F);
}

static void lcd_send_command_4bit(uint8_t cmd) {
	/* Set output port */
	/*
	GPIO_output_set_pin_value(HD44780_D7_PORT,HD44780_D7_PIN,(cmd&0x08));
	GPIO_output_set_pin_value(HD44780_D6_PORT,HD44780_D6_PIN,(cmd&0x04));
	GPIO_output_set_pin_value(HD44780_D5_PORT,HD44780_D5_PIN,(cmd&0x02));	
	GPIO_output_set_pin_value(HD44780_D4_PORT,HD44780_D4_PIN,(cmd&0x01));
*/
	
	HAL_GPIO_WritePin(HD44780_D7_PORT,HD44780_D7_PIN,(cmd&0x08));
	HAL_GPIO_WritePin(HD44780_D6_PORT,HD44780_D6_PIN,(cmd&0x04));
	HAL_GPIO_WritePin(HD44780_D5_PORT,HD44780_D5_PIN,(cmd&0x02));
	HAL_GPIO_WritePin(HD44780_D4_PORT,HD44780_D4_PIN,(cmd&0x01));
	
	
	HD44780_E_TOGGLE;
}

static void lcd_set_cursor(uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	
	/* Set location address */
	lcd_send_command(HD44780_SETDDRAMADDR | (col + row_offsets[row]));
}

static void lcd_init_pins(void) {
	
	// Enable Clock
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();	
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	__HAL_RCC_GPIOC_CLK_ENABLE();		
	/*
  GPIO_set_pin_output(HD44780_RS_PORT,HD44780_RS_PIN);
	GPIO_set_pin_output(HD44780_E_PORT,HD44780_E_PIN);
	GPIO_set_pin_output(HD44780_D7_PORT,HD44780_D7_PIN);
	GPIO_set_pin_output(HD44780_D6_PORT,HD44780_D6_PIN);
	GPIO_set_pin_output(HD44780_D5_PORT,HD44780_D5_PIN);
	GPIO_set_pin_output(HD44780_D4_PORT,HD44780_D4_PIN);
	*/
	
	/* -2- Configure IOs in output push-pull mode to drive external LEDs */
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  GPIO_InitStruct.Pin = HD44780_D7_PIN;
  HAL_GPIO_Init(HD44780_D7_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = HD44780_D6_PIN;
	HAL_GPIO_Init(HD44780_D6_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = HD44780_D5_PIN;
	HAL_GPIO_Init(HD44780_D5_PORT, &GPIO_InitStruct);
  
	GPIO_InitStruct.Pin = HD44780_D4_PIN;
	HAL_GPIO_Init(HD44780_D4_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = HD44780_RS_PIN;
	HAL_GPIO_Init(HD44780_RS_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = HD44780_E_PIN;
	HAL_GPIO_Init(HD44780_E_PORT, &GPIO_InitStruct);
	
	
	
}



	
