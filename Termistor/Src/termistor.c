 /*
 * termitor.c
 *
 *  Created on: 25 de mai de 2021
 *      Author: gabrielkenji18950@gmail.com
 */

#include <termistor.h>

#include "stm32f4xx_hal.h"
#include "math.h"


float * Temp_GetValue( uint16_t vector[], int Length){


// 	Utilizes the Beta Factor Equation:
//    	Temperature in Kelvin = 1 / {1/To + 1/B[ln(R/Ro)]}
//  	Where To, B, and Ro are given by the manufacturer




	#define	Ro 10000		//resistencia do termistor a 25ºC
	#define To 298.15		//25ºC em kelvin
	#define B 3100
	#define Rd 10000

	float a=4095;
	double raw[Length];
	static float temp[3];
	int	 i;


		for(i=0; i<Length; i++)
		{
			  raw[i]= (a/vector[i]);				//(Vadc/Vcc)
			  raw[i] = (raw[i]-1)*Rd;			//calculo da resistencia (10000 é a resistencia do divisor)
			  raw[i] /= Ro;							//calculo da temperatura
			  raw[i] = log(raw[i]);
			  raw[i] = raw[i]/B;
			  raw[i] = (1/To)+raw[i];
			  raw[i] = (1/raw[i]);
			  temp[i] = raw[i] - 273.15;
		}
		return temp;
}
uint32_t ADC_DMA_Init(uint32_t size, uint32_t vetor[])
{
}
