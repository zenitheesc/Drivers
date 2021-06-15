/*
 * termistor.h
 *
 *  Created on: 25 de mai de 2021
 *      Author: gushi
 */

#ifndef INC_TERMISTOR_H_
#define INC_TERMISTOR_H_
/******************* Define Thermistors Pins below******************/


/*
		#define TH1_Pin GPIO_PIN_2
		#define TH1_GPIO_Port GPIOA
		#define TH2_Pin GPIO_PIN_3
		#define TH2_GPIO_Port GPIOA

		CHECAR QUAIS PINOS SÃO ADC
*/



double Temp_GetValue(void);  // put cursor at the entered position row (0 or 1), col (0-15);
double TEMP_GETVALUE(void);
#endif /* INC_TERMISTOR_H_ */
