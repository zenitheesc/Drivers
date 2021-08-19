/*
 * guvcs10gd.h
 *
 * Created on: Aug 18, 2021
 *     Author: Carlos Craveiro
 */

#ifndef __GUVCS10GD_H__
#define __GUVCS10GD_H__

#include "bsp.h"

typedef struct {
	adc_t *adc;
} guvcs10gd_t;

error_t guvcs10gd_init(guvcs10gd_t guvc);
result16_t guvcs10gd_get_raw(guvcs10gd_t guvc);

/* 
 * More details about the equation used to base this function should be refered
 * in the README.md of this Directory
 */
double guvcs10gd_conv_tension_to_current(result16_t raw_data);

float guvcs10gd_get_uvcpower(guvcs10gd_t guvc);


/*CONVERSION VALUES GOT FROM DATASHEET*/

// CONV_A = (1 / 78)
#define GUVC_CONV_A (float)(0.01282051282)


/*CONVERSION VALUES THAT VARY BETWEEN PROJECTS - AMPLIFIER*/

//AMPLITUDE VALUES IN UNITS OF THE Y-AXIS OF THE BASED GRAPHICS
#define GUVCS10GD_CURRENT_AMPLITUDE 200
#define GUVCS10GD_TENSION_AMPLITUDE 1.5

// OFFSET VALUES IN UNITS OF THE Y-AXIS OF THE BASED GRAPHICS
#define GUVCS10GD_CURRENT_OFFSET 200
#define GUVCS10GD_TENSION_OFFSET 1.5

// PHASE_DIFFERENCE AS 0 OR PI (in radians)
#define GUVCS10GD_PHASE_DIFFERENCE 0


/*ERRORS CODE-SHEET*/

//#define GUVCS10GD_INIT_ERROR
#define GUVCS10GD_READ_ERROR ((float) -101);


#endif /* __INC_APPLICATION_GUVCS10GD_H__*/

