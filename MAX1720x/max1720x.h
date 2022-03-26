/*
 * max17205.h
 *
 *  Created on: Mar 20, 2022
 *      Author: leocelente
 */

#ifndef INC_MAX1720X_H_
#define INC_MAX1720X_H_

#include "platform/platform.h"

typedef struct {
  i2c_device_t dev;
} max1720x_t;

typedef struct {
  float state_of_charge;
  float capacity;
  float time_to_empty;
} max1720x_values_t;

MAKE_RESULT(max1720x_values_t);

/**
 *
 */
error_t max1720x_init(max1720x_t *max);

/**
 *
 */
result_max1720x_values_t max1720x_measure(max1720x_t *max);

#endif /* INC_MAX1720X_H_ */
