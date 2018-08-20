#ifndef __TIMER6_DRIVER_H
#define __TIMER7_DRIVER_H

#include <stdint.h>


void timer6_init(void);
uint16_t timer6_get_counter_value(void);
void timer6_enable(void);
void timer6_disable(void);

void timer6_printf_cnt_value(void);

#endif
