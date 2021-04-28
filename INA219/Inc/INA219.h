#ifndef LIB_INA219_H
#define LIB_INA219_H
#ifdef _cplusplus
extern "C"	{
#endif

#include "stm32f4xx_hal.h"

// Sensor reset
#define Normal_mode 			0b000000000000000
#define Reset 					0b100000000000000

// Bus voltage Range
#define Bus_16v 				0b000000000000000
#define Bus_32v				 	0b001000000000000

//Shunt voltage Range
#define Range_40 				0b000000000000000
#define Range_80 				0b000010000000000
#define Range_160 				0b000100000000000
#define Range_320 				0b000110000000000

//Bus ADC Resolution/Averaging
#define BMode_9bit				0b000000000000000
#define BMode_10bit				0b000000010000000
#define BMode_11bit				0b000000100000000
#define BMode_12bit				0b000000110000000
#define BMode2_12bit			0b000010000000000
#define BMode_2					0b000010010000000
#define BMode_4					0b000010100000000
#define BMode_8					0b000010110000000
#define BMode_16				0b000011000000000
#define BMode_32				0b000011010000000
#define BMode_64				0b000011100000000
#define BMode_128				0b000011110000000

//Shunt ADC Resolution/Averaging
#define SMode_9bit				0b000000000000000
#define SMode_10bit				0b000000000001000
#define SMode_11bit				0b000000000010000
#define SMode_12bit				0b000000000011000
#define SMode2_12bit			0b000000001000000
#define SMode_2					0b000000001001000
#define SMode_4					0b000000001010000
#define SMode_8					0b000000001011000
#define SMode_1					0b000000001100000
#define SMode_32				0b000000001101000
#define SMode_64				0b000000001110000
#define SMode_128				0b000000001111000

// Mode
#define Power_down				0b000000000000000
#define Trig_Shunt_voltage		0b000000000000001
#define Trig_Bus_voltage		0b000000000000010
#define Trig_Shunt_Bus_voltage	0b000000000000011
#define ADC_off					0b000000000000100
#define Cont_Shunt_voltage		0b000000000000101
#define Cont_Bus_voltage		0b000000000000110
#define Cont_Shunt_Bus_voltage	0b000000000000111

// Address Pins and Slave Addresses
//	A1_A0

#define GND_GND				0b1000000
#define GND_VS				0b1000001
#define GND_SDA				0b1000010
#define GND_SCL				0b1000011
#define VS_GND				0b1000100
#define VS_VS				0b1000101
#define VS_SDA				0b1000110
#define VS_SCL				0b1000111
#define SDA_GND				0b1001000
#define SDA_VS				0b1001001
#define SDA_SDA				0b1001010
#define SDA_SCL				0b1001011
#define SCL_GND				0b1001100
#define SCL_VS				0b1001101
#define SCL_SDA				0b1001110
#define SCL_SCL				0b1001111

typedef struct{
  float Shunt_Voltage;
  float Bus_Voltage;
  float Current;
  float Power;
 }values;


//External Function
HAL_StatusTypeDef INA_Calibration (I2C_HandleTypeDef * hi2c, uint8_t REG_INA, float Max_Current, float RSHUT);
HAL_StatusTypeDef INA_Values (I2C_HandleTypeDef * hi2c, uint8_t REG_INA, values* valores);
HAL_StatusTypeDef INA_Reset (I2C_HandleTypeDef * hi2c, uint8_t REG_INA);
HAL_StatusTypeDef INA_Configuration (I2C_HandleTypeDef * hi2c, uint8_t REG_INA, uint16_t BUS_VOLT, uint16_t SHUT_VOLT, uint16_t BUS_ADC, uint16_t SHUT_ADC, uint16_t MODE);


//Internal Function
float Converter(uint16_t UVA_RES);

#ifdef _cplusplus
}
#endif
#endif// LIB_INA_219_H
