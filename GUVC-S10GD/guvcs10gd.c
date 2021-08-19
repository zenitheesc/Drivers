/*
 * guvcs10gd.c
 *
 * Created on: Aug 18, 2021
 *     Author: Carlos Craveiro
 */

#include "guvcs10gd.h" //Application?
#include <math.h>

error_t guvcs10gd_init(guvcs10gd_t guvc) {
	adc_init(guvc.adc);
	//WHAT ADC_INIT DOES IF AN ERROR OCCURS?
	return 0;
}

result16_t guvcs10gd_get_raw(guvcs10gd_t guvc) {
	return adc_read(guvc.adc);
}

double guvcs10gd_conv_tension_to_current(result16_t raw_data) {
	double maximum_value = GUVS10GD_TENSION_OFFSET + GUVCS10GD_TENSION_AMPLITUDE;
	double minimum_value = GUVS10GD_TENSION_OFFSET - GUVCS10GD_TENSION_AMPLITUDE;
	if(raw_data.value > maximum_value) {
		raw_data.value = (uint16_t) maximum_value;
	}
	if(raw_data.value < (uint16_t) minimum_value) {
		raw_data.value = (uint16_t) minimum_value;
	}
	double repeated_term = raw_data.value - GUVCS10GD_TENSION_OFFSET;
	double first_term = (repeated_term) / (GUVCS10GD_TENSION_AMPLITUDE);
	double second_term = sqrt(1 -  (pow(repeated_term, (double) 2) / pow(GUVCS10GD_TENSION_AMPLITUDE, (double) 2)));
	double output_current = GUVCS10GD_CURRENT_AMPLITUDE * (first_term * sen(GUVCS10GD_PHASE_DIFFERENCE) + second_term * cos(GUVCS10GD_PHASE_DIFFERENCE)) + GUVCS10GD_CURRENT_OFFSET;
	return output_current;
}

float guvcs10gd_get_uvcpower(guvcs10gd_t guvc) {
	result16_t raw = guvcs10gd_get_raw(guvc);
	if(raw.hasError) {
		return (float) GUVCS10GD_READ_ERROR;
	}
	double sensors_current = guvcs10gd_conv_tension_to_current(raw);
	float uvc_radiation = (float) sensors_current * GUVCS10GD_CONV_A;
	return uvc_radiation;

}

