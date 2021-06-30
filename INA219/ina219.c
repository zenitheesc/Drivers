/*
 * ina219.c
 *
 *  Created on: Jun 20, 2021
 *      Author: leocelente
 */

#include "Application/ina219.h"

error_t ina219_reset(ina219_t ina) {

	uint16_t config = 0x0000;
	config |= INA_Reset;

	return i2c_write16(ina.device, INA_CONF_INA, config);
}

error_t ina219_config(ina219_t ina) {

	uint16_t config_register = 0x0000;

	config_register |= ina.config.mode;
	config_register |= ina.config.busVoltageRange;
	config_register |= ina.config.shuntADCResolution;
	config_register |= ina.config.busADCResolution;
	config_register |= ina.config.shuntVoltageRange;

	return i2c_write16(ina.device, INA_CONF_INA, config_register);
}

error_t ina219_calibrate(ina219_t* ina, ina219_modes_t mode){
	switch (mode) {
	case INA219_FULL_16V_40:
		ina->config.calValue = 8192;
		ina->config.currentDivider_mA = 20;
		ina->config.powerMultiplier_mW = 1;
		break;

	default:
	case INA219_FULL_32V_320:
		ina->config.calValue = 4096;
		ina->config.currentDivider_mA = 10;
		ina->config.powerMultiplier_mW = 2;
		break;
	}


	return i2c_write16(ina->device, INA_CALIBRATION, ina->config.calValue);
}

error_t ina219_measure(ina219_t ina, ina219_values_t *valores) {

	//Shunt Voltage values
	result16_t raw = i2c_read16(ina.device, INA_SHUNT_VOLT);
	if (raw.hasError) {
		return 1;
	}

	valores->Shunt_Voltage = ((float) raw.value) * 0.01;

	//Bus Voltage
	raw = i2c_read16(ina.device, INA_BUS_VOLT);
	if (raw.hasError) {
		return 1;
	}

	uint16_t val = raw.value >> 3;
	valores->Bus_Voltage = ((float) val) * 0.004;
	/***
	 * Não funciona, ver como configurar a resistencia do shunt para que ele calcule a
	 * corrente e a potencia
	 */
	raw = i2c_read16(ina.device, INA_CURRENT);
	if (raw.hasError) {
		return 1;
	}

	valores->Current = ((float) raw.value) / (float)ina.config.currentDivider_mA;

	raw = i2c_read16(ina.device, INA_POWER_REG);
	if (raw.hasError) {
		return 1;
	}

	valores->Power = ((float) raw.value) * (float)ina.config.powerMultiplier_mW;

	return 0;
}
