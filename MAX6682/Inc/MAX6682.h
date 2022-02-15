/*
 * MAX6682.h
 *
 *  Created on: 15 de out de 2021
 *      Author: gabrielkenji18950@gmail.com
 */


#ifndef INC_MAX6682_H_
#define INC_MAX6682_H_

#define		Rext1	0.12345
#define		Rext2	0.6789
#define		Rext3	0.8765


float MAX6682_Getvalue(int bits,int Rext_select);
float MAX6682_Temp_Conv( uint16_t SPI_data[], int Rext_select);  /*Converts SPI data in temperature values(ºC)*/

#endif /* INC_MAX6682_H_ */
