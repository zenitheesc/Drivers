/*
 * b_parameter.c
 *
 *  Created on: Mar 17, 2022
 *      Author: leocelente
 */

#include <math.h>
#include <thermistor.h>

static float to_celcius(float kelvin) {
	return kelvin - 273.15f;
}

error_t b_param_init(b_param_t *params) {
	if (params->beta <= 0.f) {
		return ERROR;
	}

	if (params->cal_resistance <= 0.f) {
		return ERROR;
	}

	// TODO: do actual checks
	return SUCCESS;
}

result_float b_param_convert(b_param_t const *params, float resistance) {
	float beta = params->beta;
	float R0 = params->cal_resistance;
	float K = beta/(log( resistance / R0) + (beta / T0));

	result_float result = { .value = to_celcius(K), .hasError = 0 };

	if (K < 0 || isnan(K) || !isfinite(K)) {
		result.hasError = ERROR;
	}

	return result;

}
