#ifndef __ADXL345_H
#define __ADXL345_H
#include <stdint.h>



int32_t adxl345_init(void);
int32_t adxl345_get_axis(int16_t x, int16_t y, int16_t z);



#endif