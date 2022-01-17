/*
 * ina219.h
 *
 *  Created on: Jun 20, 2021
 *      Author: leocelente
 */

#ifndef INC_APPLICATION_INA219_H_
#define INC_APPLICATION_INA219_H_

#include "bsp.h"


typedef enum ina219_modes_t {
	INA219_FULL_32V_320, INA219_FULL_16V_40
} ina219_modes_t;

typedef struct {
	uint16_t mode;
	uint16_t busVoltageRange;
	uint16_t shuntVoltageRange;
	uint16_t busADCResolution;
	uint16_t shuntADCResolution;

	uint16_t calValue;
	uint16_t currentDivider_mA;
	uint16_t powerMultiplier_mW;

} ina219_config_t;

typedef struct {
	float Shunt_Voltage;
	float Bus_Voltage;
	float Current;
	float Power;
} ina219_values_t;

typedef struct  {
	i2c_device_t device;
	ina219_config_t config;
} ina219_t;

error_t ina219_reset(ina219_t ina);
error_t ina219_config(ina219_t ina);
error_t ina219_calibrate(ina219_t* ina, ina219_modes_t mode);
error_t ina219_measure(ina219_t ina, ina219_values_t *values);


// CONSTANT
#define CURRENT_DIV ()
#define POWER_MULT ()

// Sensor reset [15]
#define INA_Normal_mode 			(uint16_t)(0x0000)
#define INA_Reset 					(uint16_t)(0x0001 << 15)

// Bus voltage Range [13]
#define INA_Bus_16v 				(uint16_t)(0x0000)
#define INA_Bus_32v				 	(uint16_t)(0x0001 << 13)

//Shunt voltage Range [12:11]
#define INA_Range_40 				(uint16_t)(0x0000)
#define INA_Range_80 				(uint16_t)(0x0001 << 11)
#define INA_Range_160 				(uint16_t)(0x0002 << 11)
#define INA_Range_320 				(uint16_t)(0x0003 << 11)

//Bus ADC Resolution/Averaging [10:7]
#define INA_BMode_9bit				(uint16_t)(0x0000)
#define INA_BMode_10bit				(uint16_t)(0x0001 << 7)
#define INA_BMode_11bit				(uint16_t)(0x0002 << 7)
#define INA_BMode_12bit				(uint16_t)(0x0003 << 7)
#define INA_BMode2_12bit			(uint16_t)(0x0008 << 7)
#define INA_BMode_2					(uint16_t)(0x0009 << 7)
#define INA_BMode_4					(uint16_t)(0x000A << 7)
#define INA_BMode_8					(uint16_t)(0x000B << 7)
#define INA_BMode_16				(uint16_t)(0x000C << 7)
#define INA_BMode_32				(uint16_t)(0x000D << 7)
#define INA_BMode_64				(uint16_t)(0x000E << 7)
#define INA_BMode_128				(uint16_t)(0x000F << 7)

//Shunt ADC Resolution/Averaging [6:3]
#define INA_SMode_9bit				(uint16_t)(0x0000)
#define INA_SMode_10bit				(uint16_t)(0x0001 << 3)
#define INA_SMode_11bit				(uint16_t)(0x0002 << 3)
#define INA_SMode_12bit				(uint16_t)(0x0003 << 3)
#define INA_SMode2_12bit			(uint16_t)(0x0008 << 3)
#define INA_SMode_2					(uint16_t)(0x0009 << 3)
#define INA_SMode_4					(uint16_t)(0x000A << 3)
#define INA_SMode_8					(uint16_t)(0x000B << 3)
#define INA_SMode_1					(uint16_t)(0x000C << 3)
#define INA_SMode_32				(uint16_t)(0x000D << 3)
#define INA_SMode_64				(uint16_t)(0x000E << 3)
#define INA_SMode_128				(uint16_t)(0x000F << 3)

// Mode [3:0]
#define INA_Power_down				(uint16_t)(0x0000)
#define INA_Trig_Shunt_voltage		(uint16_t)(0x0001)
#define INA_Trig_Bus_voltage		(uint16_t)(0x0002)
#define INA_Trig_Shunt_Bus_voltage	(uint16_t)(0x0003)
#define INA_ADC_off					(uint16_t)(0x0004)
#define INA_Cont_Shunt_voltage		(uint16_t)(0x0005)
#define INA_Cont_Bus_voltage		(uint16_t)(0x0006)
#define INA_Cont_Shunt_Bus_voltage	(uint16_t)(0x0007)

// Address Pins and Slave Addresses
//	A1_A0

#define INA_GND_GND				(uint8_t)(0x40)
#define INA_GND_VS				(uint8_t)(0x41)
#define INA_GND_SDA				(uint8_t)(0x42)
#define INA_GND_SCL				(uint8_t)(0x43)
#define INA_VS_GND				(uint8_t)(0x44)
#define INA_VS_VS				(uint8_t)(0x45)
#define INA_VS_SDA				(uint8_t)(0x46)
#define INA_VS_SCL				(uint8_t)(0x47)
#define INA_SDA_GND				(uint8_t)(0x48)
#define INA_SDA_VS				(uint8_t)(0x49)
#define INA_SDA_SDA				(uint8_t)(0x4A)
#define INA_SDA_SCL				(uint8_t)(0x4B)
#define INA_SCL_GND				(uint8_t)(0x4C)
#define INA_SCL_VS				(uint8_t)(0x4D)
#define INA_SCL_SDA				(uint8_t)(0x4E)
#define INA_SCL_SCL				(uint8_t)(0x4F)

// REGISTRADORES
#define INA_CONF_INA  		(uint8_t)(0x00)
#define INA_SHUNT_VOLT  	(uint8_t)(0x01)
#define INA_BUS_VOLT  		(uint8_t)(0x02)
#define INA_POWER_REG  		(uint8_t)(0x03)
#define INA_CURRENT  		(uint8_t)(0x04)
#define INA_CALIBRATION  	(uint8_t)(0x05)




#endif /* INC_APPLICATION_INA219_H_ */
