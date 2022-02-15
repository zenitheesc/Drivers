/*
 * MAX6682.c
 *
 *  Created on: 15 de out de 2021
 *      Author: gabrielkenji18950@gmail.com
 */

#ifndef SRC_MAX6682_C_
#define SRC_MAX6682_C_


#include "stm32l4xx_hal.h"
#include <MAX6682.h>




float MAX6682_GetValue(SPI_HandleTypeDef *hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, int Rext_select){

		typedef SPI_HandleTypeDef spi_t;
		typedef GPIO_TypeDef gpio_pin_t;

		typedef struct
		{
			spi_t *spi;
			gpio_pin_t pin;
		} spi_device_t;

		spi_device_t MAX6682;

		MAX6682.spi = hspi;
		MAX6682.pin = *GPIOx;

		static uint16_t SPI_data[11];


		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
		HAL_SPI_Receive(&MAX6682.spi, (uint8_t *)SPI_data, 11, 100);
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

		float LSB,temp=0;
			int i, Length=11;


			switch(Rext_select){	//LSB value depends on the external resistence, read datasheet for more details


			case 1:
				LSB = Rext1;
				break;
			case 2:
				LSB = Rext2;
				break;
			case 3:
				LSB = Rext3;
				break;
			}

			if (SPI_data[0]==1){				//temp neg
				int k = 0;
				for (i = 1; i < Length; i++){		//Conv array to int
				    k = 10 * k + SPI_data[i];
				}
				k= ~k;									//negative

				 int num, decimal_num = 0, base = 1, rem;  //conv bin to dec
				num=k*(-1);
				  while ( num > 0)
				    {
				        rem = num % 10; /* divide the binary number by 10 and store the remainder in rem variable. */
				        decimal_num = decimal_num + rem * base;
				        num = num / 10; // divide the number with quotient
				        base = base * 2;
				    }
				  temp = decimal_num*LSB*(-1);

			}else{								//temp posi
				for (i=Length-1; i>0; i--){
					temp=temp+(LSB*SPI_data[i]);
					LSB = LSB*2;
				}
			}

				return temp;
	}




  float MAX6682_Temp_Conv( uint16_t SPI_data[], int Rext_select){	/*Converts SPI data in temperature values(ºC)*/


	float LSB,temp=0;
	int i, Length=11;


	switch(Rext_select){	//LSB value depends on the external resistence, read datasheet for more details

		case 1:
			LSB = Rext1;
			break;
		case 2:
			LSB = Rext2;
			break;
		case 3:
			LSB = Rext3;
			break;
		}

	if (SPI_data[0]==1){				//temp neg
		int k = 0;
		for (i = 1; i < Length; i++){		//Conv array to int
		    k = 10 * k + SPI_data[i];
		}
		k= ~k;									//mostra q é negativo

		 int num, decimal_num = 0, base = 1, rem;  //conv bin to dec
		num=k*(-1);
		  while ( num > 0)
		    {
		        rem = num % 10; /* divide the binary number by 10 and store the remainder in rem variable. */
		        decimal_num = decimal_num + rem * base;
		        num = num / 10; // divide the number with quotient
		        base = base * 2;
		    }
		  temp = decimal_num*LSB*(-1);

	}else{								//temp posi
		for (i=Length-1; i>0; i--){
			temp=temp+(LSB*SPI_data[i]);
			LSB = LSB*2;
		}
	}

		return temp;

}
#endif /* SRC_MAX6682_C_ */
