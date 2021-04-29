#include <INA219.h>

// REGISTRADORES
static const uint8_t CONF_INA = 0x00;
static const uint8_t SHUNT_VOLT = 0x01;
static const uint8_t BUS_VOLT = 0x02;
//static const uint8_t CURRENT = 0x04;
//static const uint8_t CALIBRATION_INA = 0x05;

//===========================================================================
//							Sensor Reset
//===========================================================================

HAL_StatusTypeDef INA_Reset(ina219_t ina) {

	uint8_t buf[2] = { 0 };

	buf[0] = (Reset & 0xFF00) >> 8;
	buf[1] = (Reset & 0x00FF);

	return HAL_I2C_Mem_Write(ina.hi2c, ina.address, CONF_INA,
	I2C_MEMADD_SIZE_8BIT, buf, sizeof(buf),
	HAL_MAX_DELAY);
}

//===========================================================================
//							Sensor Configuration
//===========================================================================

HAL_StatusTypeDef INA_Configuration(ina219_t ina, ina219_config_t config) {

	uint16_t config_register = 0x0000;

	config_register |= config.mode;
	config_register |= config.busVoltageRange;
	config_register |= config.shuntADCResolution;
	config_register |= config.busADCResolution;
	config_register |= config.shuntVoltageRange;

	uint8_t buf[2];
	buf[0] = (config_register & 0xFF00) >> 8;
	buf[1] = (config_register & 0x00FF);

	return HAL_I2C_Mem_Write(ina.hi2c, ina.address, CONF_INA,
	I2C_MEMADD_SIZE_8BIT, buf, 2,
	HAL_MAX_DELAY);
}

//===========================================================================
//							Sensor Calibration
//===========================================================================
__attribute__((deprecated))
 HAL_StatusTypeDef INA_Calibration(ina219_t ina, float Max_Current,
		float Shunt_Resistance) {

	/* Não vale a pena checar os calculos agr, é melhor usar a configuração
		padrão de Continuous 12bit 320mV Bus and Shunt
	*/
	/*
	uint8_t buf[2];
	uint16_t Cal;
	float Current_LSB;

	Current_LSB = Max_Current / (1 << 15);

	Cal = 0.04096 / (Current_LSB * Shunt_Resistance);

	buf[0] = (Cal & 0xFF00) >> 8;
	buf[1] = (Cal & 0x00FF);

	return HAL_I2C_Mem_Write(ina.hi2c, ina.address, CALIBRATION_INA,
	I2C_MEMADD_SIZE_8BIT, buf, 2,
	HAL_MAX_DELAY);
*/ return HAL_OK;
}

//===========================================================================
//								Measure values
//===========================================================================

HAL_StatusTypeDef INA_Values(ina219_t ina, ina219_values_t *valores) {

	HAL_StatusTypeDef ret;
	uint8_t buf[2];
	int16_t hbuf;

	//Shunt Voltage values
	buf[0] = SHUNT_VOLT;
	ret = HAL_I2C_Mem_Read(ina.hi2c, ina.address, SHUNT_VOLT,
	I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	hbuf = (buf[0] << 8) | buf[1];
	valores->Shunt_Voltage = ((float)hbuf) * 0.01;

	//Bus Voltage
	ret = HAL_I2C_Mem_Read(ina.hi2c, ina.address, BUS_VOLT ,
	I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	hbuf = (buf[0] << 8) | buf[1];
	uint16_t val = hbuf >> 3;

	valores->Bus_Voltage = ((float)val) * 0.004;

 /*
	uint8_t b32_msb = Bus_32v >> 8;
	buf[0] = BUS_VOLT;
	if (buf[0] & b32_msb) { // is 32v
		ret = HAL_I2C_Mem_Read(ina.hi2c, ina.address, BUS_VOLT,
		I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY);

		hbuf = (buf[0] << 8) | buf[1];

		valores->Bus_Voltage = hbuf * 0.004;
	} else { // 16v
		ret = HAL_I2C_Mem_Read(ina.hi2c, ina.address, BUS_VOLT,
		I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY);

		hbuf = (buf[0] << 8) | buf[1];

		valores->Bus_Voltage = hbuf * 0.004;
	}

	//Current Register
	buf[0] = 0xFF;
	ret = HAL_I2C_Mem_Read(ina.hi2c, ina.address, CALIBRATION_INA,
	I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	hbuf = (buf[0] << 8) | buf[1];

	buf[0] = 0xEF;
	ret = HAL_I2C_Mem_Read(ina.hi2c, ina.address, SHUNT_VOLT,
	I2C_MEMADD_SIZE_8BIT, buf, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	hbuf = (buf[0] << 8) | buf[1];

	valores->Current = (hbuf * hbuf) / 4096;

	//Power Register
	buf[0] = 0xDF;
	ret = HAL_I2C_Mem_Read(ina.hi2c, ina.address, CURRENT, I2C_MEMADD_SIZE_8BIT,
			buf, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	hbuf = (buf[0] << 8) | buf[1];

	buf[0] = BUS_VOLT;

	ret = HAL_I2C_Master_Receive(ina.hi2c, ina.address, buf, 2,
	HAL_MAX_DELAY);

	if (ret != HAL_OK) {
		return ret;
	}

	hbuf = (buf[0] << 8) | buf[1];

	valores->Power = (hbuf[0] * hbuf[1]) / 5000;
*/
	return HAL_OK;
}
