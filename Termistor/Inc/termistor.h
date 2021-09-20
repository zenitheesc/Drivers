/*
 * termistor.h
 *
 *  Created on: 25 de mai de 2021
 *      Author: gabrielkenji18950@gmail.com
 */

#ifndef INC_TERMISTOR_H_
#define INC_TERMISTOR_H_


#include <inttypes.h>





uint16_t *ADC_init(void);

float* Temp_GetValue( uint16_t vector[], int Length);  /*Converts ADC readings in temperature values(ºC)*/

uint32_t ADC_DMA_Init(uint32_t size, uint32_t vetor[]); /*Iniciates ADC in DMA mode and saves its readings*/
#endif /* INC_TERMISTOR_H_ */
