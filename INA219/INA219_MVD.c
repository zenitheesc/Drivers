/*
 * INA219_MVD.c
 *
 *  Created on: Sep 8, 2021
 *      Author: Murilo Henrique Pasini Trevisan
 */

#include "INA219_MVD.h"


error_t INA219_reset(INA219_t ina) {

		uint16_t config_rst = 0x0000;
		config_rst |= INA_RST;

		return i2c_write16(ina.device, INA_CONFIG_ADR, config_rst);
}

error_t INA219_config(INA219_t ina) {

		// result16_t config_raw = i2c_read16(ina.device, INA_CONFIG_ADR);
		// config_register = config_raw

		uint16_t config_register = 0x0000;

		config_register |= ina.config.BusVoltageRange;
		config_register |= ina.config.ShuntVoltageRange;
		config_register |= ina.config.BusADCResolution;
		config_register |= ina.config.ShuntADCResolution;
		config_register |= ina.config.OperationMode;

		return i2c_write16(ina.device, INA_CONFIG_ADR, config_register);
}

error_t INA219_measure(INA219_t ina, INA219_values_t *medida) {

		//Shunt Voltage
		//Leitura do valor i2c no registrador do shunt
		result16_t raw_s = i2c_read16(ina.device, INA_SHUNT_VOLT);

		if (raw_s.hasError){
			return raw_s.hasError;
		}

		//Complemento de 2 do valor lido do shunt
		int16_t val_s = raw_s.value;

		//Conversão para tensão
		float volt_s = ((float) val_s) * INA_SHUNT_MULTIPLY;


		//
		medida -> Shunt_Voltage = volt_s;


		//Bus Voltage
		//Leitura do valor i2c do registrador Bus
		result16_t raw_b = i2c_read16(ina.device, INA_BUS_VOLT);

		if (raw_b.hasError) {
			return raw_b.hasError;
		}

		//complemento de 2 do valor lido do Bus
		int16_t val_b = raw_b.value;

		//shift dos bits em 3 bits
		int16_t val_shift = val_b >> 3;

		//Conversão para tensão
		float volt_b = ((float) val_shift) * INA_BUS_MULTIPLY;

		//
		medida -> Bus_Voltage = volt_b;


		//Current
		//Calculo a partir da tensão no Shunt e do valor do resistor do datasheet
		float current_s = (volt_s)/(INA_RESISTOR);

		//
		medida -> Shunt_Current = ( current_s );

}

