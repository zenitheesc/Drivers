 /*
 * termistor.c
 *
 *  Created on: 25 de mai de 2021
 *      Author: gushi
 */

#include <termistor.h>

#include "stm32f1xx_hal.h"
#include "math.h"

/******************* Define Thermistors Pins below******************/


/*
		#define TH1_Pin GPIO_PIN_2
		#define TH1_GPIO_Port GPIOA
		#define TH2_Pin GPIO_PIN_3
		#define TH2_GPIO_Port GPIOA

		CHECAR QUAIS PINOS SÃO ADC
*/

static void MX_ADC1_Init(void);
ADC_HandleTypeDef hadc1;

double Temp_GetValue(void){

/******************************************************************/
/* Utilizes the Beta Factor Equation:				*/
/*    Temperature in Kelvin = 1 / {1/To + 1/B[ln(R/Ro)]}		*/
/*    Where To, B, and Ro are given by the manufacturer
/******************************************************************/

	double 	raw, temp;
	float	Ro = 10000;
	float 	To = 298.15;
	int 	NUMAMOSTRAS = 5;
	int		B = 3984;
	int		amostra[5], i;


	  for (i=0; i< NUMAMOSTRAS; i++) {
			  HAL_ADC_Start(&hadc1);
			  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			  amostra[i] = HAL_ADC_GetValue(&hadc1);
			  HAL_Delay(10);
		  }

		  raw = 0;
		  for (i=0; i< NUMAMOSTRAS; i++) {
		     raw += amostra[i];
		  }
		  raw /= NUMAMOSTRAS;
		  raw /= (4095);
		  raw = (1 - raw)*10000;
		  raw /= Ro;
		  temp = log(raw);
		  temp = temp/B;
		  temp = (1/To)+temp;
		  temp = (1/temp);
		  temp = temp - 273.15;

		  return temp;

}


double TEMP_GETVALUE(void){

/******************************************************************/
	/* Utilizes the Beta Factor Equation:
	/*    Temperature in Kelvin = 1 / {A + B[ln(R)] +C[ln(R)]^3}
	/*    Where A, B and C can be calculated using values or
	 *		temperature and resistence inside the temperature range
	 * 		that we want to mesure.
	 * 		https://pt.wikipedia.org/wiki/Term%C3%ADstor
/******************************************************************/

	double 	raw, temp;
	double	R;
	double	R1 = 96761.1;
	float 	T1 = 253.15;	//-20°C
	double	R2 = 10000;
	float 	T2 = 298.15;	//25°C
	double	R3 = 1747.7;
	float 	T3 = 343.15;	//70°C
	float L1, L2, L3;
	float A, B, C;
	float y1, y2, y3;
	float Y1, Y2, Y3;
	int 	NUMAMOSTRAS = 5;
//	B = 3984;
	int		lnR;
	int		amostra[5], i;

	L1 = log(R1);
	L2 = log(R2);
	L3 = log(R3);

	Y1 = 1/T1;
	Y2 = 1/T2;
	Y3 = 1/T3;

	y2 = (Y2-Y1)/(L2-L1);
	y3 = (Y3-Y1)/(L3-L1);
	y1 = (y3-y2)/(L3-L2);

	C = 1/(L1+L2+L3);
	C = C*y1;
	B = L1*L1;
	B = B+L2*L2;
	B = B+L1*L2;
	B = C*B;
	B = y2 - C;
	A = C*L1*L1;
	A = (A+B)*L1;
	A = Y1-A;


	for (i=0; i< NUMAMOSTRAS; i++) {
			  HAL_ADC_Start(&hadc1);
			  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			  amostra[i] = HAL_ADC_GetValue(&hadc1);
			  HAL_Delay(10);
		  }

		  raw = 0;
		  for (i=0; i< NUMAMOSTRAS; i++) {
		     raw += amostra[i];
		  }
		  raw /= NUMAMOSTRAS;
		  raw /= (4095);
		  R = (1 - raw)*10000;

/*		  lnR = log(R);
		  temp *= B;


		  temp += A;
		  lnR = lnR*inR*lnR;
		  temp += C*lnR;
		  temp = (1/temp);
		  temp -= 273.15;
*/
		  return Y1;
}

