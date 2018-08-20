#ifndef __ADXL345_H
#define __ADXL345_H
#include <stdint.h>



int32_t adxl345_init(void);
int32_t adxl345_get_axis(float* x, float* y, float* z);



#endif