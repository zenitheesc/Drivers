/*
 * termistor.h
 *
 *  Created on: 25 de mai de 2021
 *      Author: gabrielkenji18950@gmail.com
 */

#ifndef INC_thermistor_H_
#define INC_thermistor_H_


#include <inttypes.h>
#include <stm32l4xx_hal.h>

	#define	Ro 10000		//resistencia do termistor a 25ºC
	#define To 298.15		//25ºC em kelvin
	#define B 3100
	#define Rd 10000		//resitence of the voltage divider
	#define a 4095			//resolution of the adc reading




uint16_t *ADC_init(void);

float thermistor_TempConv( double raw);  /*Converts ADC readings in temperature values(ºC)*/
float thermistor_GetValue(ADC_HandleTypeDef *hadc);		/*Reads ADC and convert the data in temperature(ºC)*/



#endif /* INC_thermistor_H_ */
