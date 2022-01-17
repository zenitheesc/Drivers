/*
 * INA219_MVD.h
 *
 *  Created on: Sep 8, 2021
 *      Author: Murilo Henrique Pasini Trevisan
 */

#ifndef INC_INA219_MVD_H_
#define INC_INA219_MVD_H_

#include "bsp.h"

/* REGISTRADORES */
// Nome						Adrres(hex)

#define INA_CONFIG_ADR		(uint16_t)(0x00)
#define INA_SHUNT_VOLT		(uint16_t)(0x01)
#define INA_BUS_VOLT		(uint16_t)(0x02)
//#define INA_POWER			(uint16_t)(0x03) //Sem uso no MVD
//#define INA_CURRENT		(uint16_t)(0x04) //Depende da calibração
//#define INA_CALIB			(uint16_t)(0x05) //Sem uso no MVD

/* CONFIGURAÇÕES */

// Reset sensor
// Bit RST(15)
#define INA_RST				(uint16_t)(0x0001 << 15)

// Bus voltage range
// Bit BRNG(13)
#define INA_BRANGE_16		(uint16_t)(0x0000 << 13)
//#define INA_BUS_32		(uint16_t)(0x0001 << 13)// Sem uso no MVD

// Shunt voltage range
// Bit PG(11-12)
#define INA_SRANGE_40		(uint16_t)(0x0000 << 11)
//#define INA_SRANGE_80		(uint16_t)(0x0001 << 11)
//#define INA_SRANGE_160	(uint16_t)(0x0002 << 11)
//#define INA_SRANGE_320	(uint16_t)(0x0003 << 11)

// Bus ADC resolution
// Bit BADC(10-7)
#define INA_BADC1_12bit		(uint16_t)(0x0003 << 7) //DEFAULT 532us
#define INA_BADC2_12bit		(uint16_t)(0x0008 << 7) //CONFIG  532us

//SHUNT ADC reslution
// Bit SADC(6-3)
#define INA_SADC1_12bit		(uint16_t)(0x0003 << 7)//DEFAULT 532us
#define INA_SADC2_12bit		(uint16_t)(0x0008 << 7)//CONFIG  532 us

// Operation mode
// Bit MODE (2-0)
#define INA_CONTINUOUS		(uint16_t)(0x0007 << 0)//DEFAULT



/* SENSOR STRUCTS */

typedef struct{
		uint16_t BusVoltageRange;		// BRNG
		uint16_t ShuntVoltageRange;		// PG
		uint16_t BusADCResolution;		// BADC
		uint16_t ShuntADCResolution;	// SADC
		uint16_t OperationMode;
} INA219_config_t ;

typedef struct{
		float Shunt_Voltage;
		float Bus_Voltage;
		float Shunt_Current;
} INA219_values_t ;

typedef struct{
		i2c_device_t device;
		INA219_config_t config;
} INA219_t ;

error_t INA219_reset(INA219_t ina);
error_t INA219_config(INA219_t ina);
error_t INA219_measure(INA219_t ina, INA219_values_t *values);

//confirmar o valor do resistor shunt, depende da calibração.
#define INA_RESISTOR		(float)(0.100)
#define INA_BUS_MULTIPLY	(float)(0.004)
#define INA_SHUNT_MULTIPLY	(float)(0.01)




#endif /* INC_INA219_MVD_H_ */
