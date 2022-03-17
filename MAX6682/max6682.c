/*
 * thermistor.c
 *
 *  Created on: Mar 17, 2022
 *      Author: leocelente
 */

#include "max6682.h"
#include <math.h>

// Vr+, voltage the sensor sets at pin R+, aka Rref
#define V_Rp (1.220f)
// Conversion constants: datasheet Page 4, Temperature Conversion
#define C_1 (0.010404f)
#define C_2 (0.174387f)

/**
 * Read from MAX6682 via SPI
 */
static result_uint16_t read(max6682_t const *max) {
	uint8_t raw[2] = { 0 };
	buffer_view_t view_raw = { .data = raw, .size = sizeof(raw) };
	gpio_low(max->dev.pin);
	error_t e = spi_receive(max->dev, view_raw);
	gpio_high(max->dev.pin);
	uint16_t value = (raw[0]) | (raw[1] << 8);
	result_uint16_t out = { .hasError = e, .value = value };
	return out;
}

error_t max6682_init(max6682_t *max) {
	if (max->Rext <= 0.f) {
		return ERROR;
	}
//	...
	return SUCCESS;
}

// Convert raw data from SPI to resistance in Ohms
static result_float convert(max6682_t const *max, uint16_t Dout) {
	// voltage drop on Rext
	float Vext = V_Rp * ((Dout * C_1)/8 + C_2);
	// current passing through both resistors
	float Iload = Vext / max->Rext;
	// voltage drop in thermistor
	float Vth = V_Rp - Vext;
	// thermistor resistance
	float R = Vth / Iload;

	result_float result = { .value = R, .hasError = 0 };
	if (R <= 0.f || isnan(R) || !isfinite(R)) {
		result.hasError = ERROR;
	}
	return result;
}

result_float max6682_measure(max6682_t const *max) {
	// output is 11 bits long
	result_uint16_t result = read(max);
	if (result.hasError) {
		result_float failed = { .value = 0.f, .hasError = result.hasError };
		return failed;
	}
	return convert(max, result.value);
}

