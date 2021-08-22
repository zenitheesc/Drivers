/*
 * guvcs10gd.c
 *
 * Created on: Aug 18, 2021
 *     Author: Carlos Craveiro
 */

#include "guvcs10gd.h"
#include <math.h>

error_t guvcs10gd_init(guvcs10gd_t guvc) {
	adc_init(guvc.adc);
	//WHAT ADC_INIT DOES IF AN ERROR OCCURS?
	return 0;
}


result16_t guvcs10gd_get_raw(guvcs10gd_t guvc) {
	return adc_read(guvc.adc);
}


uint16_t guvcs10gd_conv_tension_to_current(guvcs10gd_t guvc, result16_t raw_data) {
	uint16_t output_tension = GUVCS10GD_ADC_CONVERTER(raw_data.value);

	uint16_t maximum_tension_value = guvc.converter.tension.offset + guvc.converter.tension.amplitude;
	uint16_t minimum_tension_value = guvc.converter.tension.offset - guvc.converter.tension.amplitude;
	

	if(output_tension > maximum_tension_value) {
		output_tension =  maximum_tension_value;
	}
	if(output_tension < minimum_value) {
		output_tension =  minimum_tension_value;
	}
	
	uint16_t converter_constant =  guvc.converter.current.amplitude / guvc.converter.tension.amplitude;
	int phase_param = (guvc.converter.phase_difference < 90 || guvc.converter.phase_difference >= 270)? 1 : -1;
	uint16_t output_current = converter_constant * (output_tension - guvc.converter.tension.offset) * phase_param + guvc.converter.current.offset;

	return output_current;
}


float guvcs10gd_get_uvcpower(guvcs10gd_t guvc) {
	result16_t raw = guvcs10gd_get_raw(guvc);

	if(raw.hasError) {
		return (float) GUVCS10GD_READ_ERROR;
	}

	uint16_t sensors_current = guvcs10gd_conv_tension_to_current(guvc, raw);
	float uvc_radiation = (sensors_current / 1000.f) * GUVCS10GD_CONV_A;

	return uvc_radiation;

}


double guvcs10gd_accurate_conv_degrees_to_radians(uint16_t deg) {
	uint64_t pi = 3141592654;

	return ((double)(2 * pi * deg ) / (360 * 1000000000));
}


double guvcs10gd_accurate_conv_tension_to_current(guvcs10gd_t guvc, result16_t raw_data) {
	uint16_t output_tension = GUVCS10GD_ADC_CONVERTER(raw_data.value);

	uint16_t maximum_tension_value = guvc.converter.tension.offset + guvc.converter.tension.amplitude;
	uint16_t minimum_tension_value = guvc.converter.tension.offset - guvc.converter.tension.amplitude;
	

	if(output_tension > maximum_tension_value) {
		output_tension =  maximum_tension_value;
	}
	if(output_tension < minimum_value) {
		output_tension =  minimum_tension_value;
	}

	double sine_x = (double)(output_tension - guvc.converter.tension.offset) / guvc.converter.tension.amplitude;
	double cosine_x = sqrt(1 - (double)((output_tension - guvc.converter.tension.offset) / guvc.converter.tension.amplitude) * ((output_tension - guvc.converter.tension.offset) / guvc.converter.tension.amplitude));

	double output_current = guvc.converter.tension.amplitude * (sine_x * cos(guvc.converter.phase_difference) + cossine_x * sin(guvc.converter.phase_difference)) + guvc.converter.current.offset;
	return output_current;
}


double guvcs10gd_accurate_get_uvcpower(guvcs10gd_t guvc) {
	result16_t raw = guvcs10gd_get_raw(guvc);

	if(raw.hasError) {
		return (float) GUVCS10GD_READ_ERROR;
	}

	double sensors_current = guvcs10gd_accurate_conv_tension_to_current(guvc, raw);
	double uvc_radiation = (sensors_current / 1000) * GUVCS10GD_CONV_A;

	return uvc_radiation;

}
