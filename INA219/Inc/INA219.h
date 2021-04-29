#ifndef LIB_INA219_H
#define LIB_INA219_H
#ifdef _cplusplus
extern "C"	{
#endif

#include "stm32f4xx_hal.h"

// Sensor reset [15]
#define Normal_mode 			(uint16_t)(0x0000) 			// 0b000000000000000
#define Reset 					(uint16_t)(0x0001 << 15)	// 0b100000000000000

// Bus voltage Range [13]
#define Bus_16v 				(uint16_t)(0x0000) 			// 0b000000000000000
#define Bus_32v				 	(uint16_t)(0x0001 << 13) 	// 0b001000000000000

//Shunt voltage Range [12:11]
#define Range_40 				(uint16_t)(0x0000) 			// 0b000000000000000
#define Range_80 				(uint16_t)(0x0001 << 11) 	// 0b000010000000000
#define Range_160 				(uint16_t)(0x0002 << 11) 	// 0b000100000000000
#define Range_320 				(uint16_t)(0x0003 << 11) 	// 0b000110000000000

//Bus ADC Resolution/Averaging [10:7]
#define BMode_9bit				(uint16_t)(0x0000)			// 0b000000000000000
#define BMode_10bit				(uint16_t)(0x0001 << 7)		// 0b000000010000000
#define BMode_11bit				(uint16_t)(0x0002 << 7)		// 0b000000100000000
#define BMode_12bit				(uint16_t)(0x0003 << 7)		// 0b000000110000000
#define BMode2_12bit			(uint16_t)(0x0008 << 7)		// 0b000010000000000
#define BMode_2					(uint16_t)(0x0009 << 7)		// 0b000010010000000
#define BMode_4					(uint16_t)(0x000A << 7)		// 0b000010100000000
#define BMode_8					(uint16_t)(0x000B << 7)		// 0b000010110000000
#define BMode_16				(uint16_t)(0x000C << 7)		// 0b000011000000000
#define BMode_32				(uint16_t)(0x000D << 7)		// 0b000011010000000
#define BMode_64				(uint16_t)(0x000E << 7)		// 0b000011100000000
#define BMode_128				(uint16_t)(0x000F << 7)		// 0b000011110000000

//Shunt ADC Resolution/Averaging [6:3]
#define SMode_9bit				(uint16_t)(0x0000) 			// 0b000000000000000
#define SMode_10bit				(uint16_t)(0x0001 << 3) 		// 0b000000000001000
#define SMode_11bit				(uint16_t)(0x0002 << 3) 		// 0b000000000010000
#define SMode_12bit				(uint16_t)(0x0003 << 3) 		// 0b000000000011000
#define SMode2_12bit			(uint16_t)(0x0008 << 3) 		// 0b000000001000000
#define SMode_2					(uint16_t)(0x0009 << 3) 		// 0b000000001001000
#define SMode_4					(uint16_t)(0x000A << 3) 		// 0b000000001010000
#define SMode_8					(uint16_t)(0x000B << 3) 		// 0b000000001011000
#define SMode_1					(uint16_t)(0x000C << 3) 		// 0b000000001100000
#define SMode_32				(uint16_t)(0x000D << 3) 		// 0b000000001101000
#define SMode_64				(uint16_t)(0x000E << 3) 		// 0b000000001110000
#define SMode_128				(uint16_t)(0x000F << 3) 		// 0b000000001111000

// Mode [3:0]
#define Power_down				(uint16_t)(0x0000)	 		// 0b000000000000000
#define Trig_Shunt_voltage		(uint16_t)(0x0001)	 		// 0b000000000000001
#define Trig_Bus_voltage		(uint16_t)(0x0002)	 		// 0b000000000000010
#define Trig_Shunt_Bus_voltage	(uint16_t)(0x0003)	 		// 0b000000000000011
#define ADC_off					(uint16_t)(0x0004)	 		// 0b000000000000100
#define Cont_Shunt_voltage		(uint16_t)(0x0005)	 		// 0b000000000000101
#define Cont_Bus_voltage		(uint16_t)(0x0006)	 		// 0b000000000000110
#define Cont_Shunt_Bus_voltage	(uint16_t)(0x0007)	 		// 0b000000000000111

// Address Pins and Slave Addresses
//	A1_A0

#define GND_GND				(uint8_t)(0x40) 		// 0b1000000
#define GND_VS				(uint8_t)(0x41) 		// 0b1000001
#define GND_SDA				(uint8_t)(0x42) 		// 0b1000010
#define GND_SCL				(uint8_t)(0x43) 		// 0b1000011
#define VS_GND				(uint8_t)(0x44) 		// 0b1000100
#define VS_VS				(uint8_t)(0x45) 		// 0b1000101
#define VS_SDA				(uint8_t)(0x46) 		// 0b1000110
#define VS_SCL				(uint8_t)(0x47) 		// 0b1000111
#define SDA_GND				(uint8_t)(0x48) 		// 0b1001000
#define SDA_VS				(uint8_t)(0x49) 		// 0b1001001
#define SDA_SDA				(uint8_t)(0x4A) 		// 0b1001010
#define SDA_SCL				(uint8_t)(0x4B) 		// 0b1001011
#define SCL_GND				(uint8_t)(0x4C) 		// 0b1001100
#define SCL_VS				(uint8_t)(0x4D) 		// 0b1001101
#define SCL_SDA				(uint8_t)(0x4E) 		// 0b1001110
#define SCL_SCL				(uint8_t)(0x4F) 		// 0b1001111

typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t address;
} ina219_t;

typedef struct {
	uint16_t mode;
	uint16_t busVoltageRange;
	uint16_t shuntVoltageRange;
	uint16_t busADCResolution;
	uint16_t shuntADCResolution;
} ina219_config_t;

typedef struct {
	float Shunt_Voltage;
	float Bus_Voltage;
	float Current;
	float Power;
} ina219_values_t;

//External Function
HAL_StatusTypeDef INA_Calibration(ina219_t ina, float Max_Current, float RSHUT);
HAL_StatusTypeDef INA_Values(ina219_t ina, ina219_values_t *valores);
HAL_StatusTypeDef INA_Reset(ina219_t ina);
HAL_StatusTypeDef INA_Configuration(ina219_t ina, ina219_config_t config);

//Internal Function
float Converter(uint16_t);

#ifdef _cplusplus
}
#endif
#endif// LIB_INA_219_H
