/*
 * mpx5700.h
 *
 *  Created on: Jul 21, 2021
 *      Author: leocelente
 */

#ifndef INC_MPX5700_H_
#define INC_MPX5700_H_
#include "platform/platform.h"

typedef struct {
  adc_t *adc;
} mpx5700_t;

error_t mpx5700_init(mpx5700_t mpx);
result_uint16_t mpx5700_get_raw(mpx5700_t mpx);
float mpx5700_get_pressure(mpx5700_t mpx);

// Vout = A*pressao + B
#define MPX_CONV_A (0.04f)
#define MPX_CONV_B (0.0012858f)

#endif /* INC_MPX5700_H_ */
