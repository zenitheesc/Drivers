/*
 * INA219_MVD.c
 *
 *  Created on: Sep 8, 2021
 *      Author: Murilo Henrique Pasini Trevisan
 */

#include "ina219.h"

static result_uint16_t read(INA219_t ina, uint8_t address) {
	buffer_view_t addr = {.data=&address, .size=1};
	error_t e = i2c_transmit(ina.device, addr);
	uint8_t value_buf[2] = {0};
	buffer_view_t response = {.data=value_buf, .size=sizeof(value_buf)};
	e |= i2c_receive(ina.device, response);
	uint16_t value = response.data[1] << 8 | response.data[0];
	result_uint16_t out = {.value=value, .hasError=e};
	return out;
}

static error_t write(INA219_t ina, uint8_t address, uint16_t value) {
	uint8_t transaction_buf[3] = {address, value >> 8, value}; 
	buffer_view_t transaction = {.data=transaction_buf, .size=sizeof(transaction_buf)};
	return i2c_transmit(ina.device, transaction);
}

error_t INA219_reset(INA219_t ina) {

		uint16_t config_rst = 0x0000;
		config_rst |= INA_RST;

		return write(ina, INA_CONFIG_ADR, config_rst);
}

error_t INA219_config(INA219_t ina) {

		// result_uint16_t config_raw = i2c_read16(ina.device, INA_CONFIG_ADR);
		// config_register = config_raw

		uint16_t config_register = 0x0000;

		config_register |= ina.config.BusVoltageRange;
		config_register |= ina.config.ShuntVoltageRange;
		config_register |= ina.config.BusADCResolution;
		config_register |= ina.config.ShuntADCResolution;
		config_register |= ina.config.OperationMode;

		return write(ina, INA_CONFIG_ADR, config_register);
}

error_t INA219_measure(INA219_t ina, INA219_values_t *medida) {

		//Shunt Voltage
		//Leitura do valor i2c no registrador do shunt
		result_uint16_t raw_s = read(ina, INA_SHUNT_VOLT);

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
		result_uint16_t raw_b = read(ina, INA_BUS_VOLT);

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

