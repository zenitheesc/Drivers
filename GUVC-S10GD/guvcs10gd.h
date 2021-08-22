/*
 * guvcs10gd.h
 *
 * Created on: Aug 18, 2021
 *     Author: Carlos Craveiro
 */

#ifndef __GUVCS10GD_H__
#define __GUVCS10GD_H__

#include "bsp.h"



/* TYPEDEFS */

typedef struct {

	float offset;
	float amplitude;

} guvcs10gd_conv_param_t


typedef struct {
	adc_t *adc;
	
	struct {
		
		guvcs10gd_conv_param_t tension; //unit => mili-volts
		guvcs10gd_conv_param_t current; //unit => pico-amperes
		uint16_t phase_difference;      //unit => degrees

	} converter;

} guvcs10gd_t;



/* FUNCTIONS */

error_t guvcs10gd_init(guvcs10gd_t guvc);

result16_t guvcs10gd_get_raw(guvcs10gd_t raw);

result16_t guvcs10gd_conv_tension_to_current(guvcs10gd_t guvc, result16_t raw_data);

float guvcs10gd_get_uvcpower(guvcs10gd_t guvc);


// OPTIONAL FUNCTIONS THAT REQUIRES MORE PROCESSING POWER FROM THE BLUEPILL(STM32)
double guvcs10gd_accurate_conv_degrees_to_radians(uint16_t deg);

double guvcs10gd_accurate_conv_tension_to_current(guvcs10gd_t guvc, result16_t raw_data);

double guvcs10gd_accurate_get_uvcpower(guvcs10gd_t guvc);



/*CONVERSION VALUES GOT FROM DATASHEET*/

// CONV_A = (1 / 78)
#define GUVC_CONV_A (float)(0.01282051282)



/*CONVERSION BLUEPILL_ADC UNIT TO MILI-VOLTS*/

#define GUVCS10GD_ADC_CONVERTER(bpill_output) ((bpill_output * 3300) / 4096)



/*ERRORS CODE-SHEET*/

//#define GUVCS10GD_INIT_ERROR
#define GUVCS10GD_READ_ERROR ((float) -101);



#endif /*__INC_APPLICATION_GUVCS10GD_H__*/

