/*
 * mpx5700.c
 *
 *  Created on: Jul 21, 2021
 *      Author: leocelente
 */
#include "Application/mpx5700.h"

error_t mpx5700_init(mpx5700_t mpx) {
	adc_init(mpx.adc);
	return 0;
}

result16_t mpx5700_get_raw(mpx5700_t mpx) {
	return adc_read(mpx.adc);
}

float mpx5700_get_pressure(mpx5700_t mpx) {
	result16_t raw = mpx5700_get_raw(mpx);
	if (raw.hasError) {
		return -99.f;
	}
	// TODO: Ver com hardware se vale deixar como parametro
	const int Vs = 5; // Supondo Vs = 5v
	float pressure = ((raw.value / Vs) - MPX_CONV_A) / MPX_CONV_B;
	return pressure;
}
