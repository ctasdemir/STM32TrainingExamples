#ifndef __ADC_DRIVER_H
#define __ADC_DRIVER_H

#include <stdint.h>


void adc_driver_init(void);
int32_t adc_start_conversion(void);
void adc_print_results(void);
#endif
