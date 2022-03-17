/*
 * thermistor.h
 *
 *  Created on: Mar 17, 2022
 *      Author: leocelente
 */

#ifndef INC_MAX6682_H_
#define INC_MAX6682_H_
#include "platform/platform.h"


typedef struct {
	spi_device_t dev;
	// Linearizing external resistance
	float Rext;
} max6682_t;

/**
 * Validates Rext and device connection
 */
error_t max6682_init(max6682_t * max);

/**
 * Reads SPI output and returns the thermistor resistance
 */
result_float max6682_measure(max6682_t const* max);


#endif /* INC_MAX6682_H_ */
