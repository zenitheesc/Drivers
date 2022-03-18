/*
 * b_paramter.h
 *
 *  Created on: Mar 17, 2022
 *      Author: leocelente
 */

#ifndef INC_THERMISTOR_H_
#define INC_THERMISTOR_H_

#include "platform/platform.h"

#define T0 (25.0f + 273.15f)


// Conversion Parameters for Beta-Parameter method
typedef struct {
	// B-parameter of thermistor
	float beta;
	// Resistance expected at 25C
	float cal_resistance;
} b_param_t;

/**
 * Checks calibration data is valid for the method
 */
error_t b_param_init(b_param_t *params);

/**
 * Converts measured resistance in Ohms to temperature in degree Celcius
 * Using the Beta-Parameter method
 */
result_float b_param_convert(b_param_t const * params, float resistance);


#endif /* INC_THERMISTOR_H_ */
