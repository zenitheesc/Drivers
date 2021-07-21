/*
 * mpx5700.h
 *
 *  Created on: Jul 21, 2021
 *      Author: leocelente
 */

#ifndef INC_APPLICATION_MPX5700_H_
#define INC_APPLICATION_MPX5700_H_
#include "bsp.h"

typedef struct {
	adc_t* adc;
} mpx5700_t;

error_t mpx5700_init(mpx5700_t mpx);
result16_t mpx5700_get_raw(mpx5700_t mpx);
float mpx5700_get_pressure(mpx5700_t mpx);

// Vout = A*pressao + B
#define MPX_CONV_A (float)(0.04)
#define MPX_CONV_B (float)(0.0012858)

#endif /* INC_APPLICATION_MPX5700_H_ */
