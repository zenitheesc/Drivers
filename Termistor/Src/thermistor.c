/*
 * termitor.c
 *
 *  Created on: 25 de mai de 2021
 *      Author: gabrielkenji18950@gmail.com
 */
#ifndef SRC_thermistor_C_
#define SRC_thermistor_C_

#include <thermistor.h>

#include "stm32l4xx_hal.h"
#include "math.h"


float thermistor_TempConv( double raw){


// 		Utilizes the Beta Factor Equation:
//    Temperature in Kelvin = 1 / {1/To + 1/B[ln(R/Ro)]}
//    Where To, B, and Ro are given by the manufacturer



		float temp;

		  raw /= (a);						//resistence
		  raw = (1 - raw)*Rd;
		  raw /= Ro;							//temperature
		  temp = log(raw);
		  temp = temp/B;
		  temp = (1/To)+temp;
		  temp = (1/temp);
		  temp = temp - 273.15;

		return temp;
}


float thermistor_GetValue(ADC_HandleTypeDef *hadc){

// Utilizes the Beta Factor Equation:
//    Temperature in Kelvin = 1 / {1/To + 1/B[ln(R/Ro)]}
//    Where To, B, and Ro are given by the manufacturer

	float temp;
	double 	raw;
	int 	sample_n = 5;
	int		sample[sample_n], i;


	  for (i=0; i< sample_n; i++) {			//adc sample collection routine
			  HAL_ADC_Start(hadc);
			  HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
			  sample[i] = HAL_ADC_GetValue(hadc);
			  HAL_Delay(10);
		  }

		  raw = 0;								//average five samples
		  for (i=0; i< sample_n; i++) {
		     raw += sample[i];
		  }
		  raw /= sample_n;




		  raw /= (4095);						//resistence
		  raw = (1 - raw)*10000;
		  raw /= Ro;							//temperature
		  temp = log(raw);
		  temp = temp/B;
		  temp = (1/To)+temp;
		  temp = (1/temp);
		  temp = temp - 273.15;

		  return temp;

}

#endif /* SRC_thermistor_C_ */

