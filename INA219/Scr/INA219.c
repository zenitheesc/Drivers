#include <INA219.h>

static const uint8_t CONF_INA = 0x00;
static const uint8_t SHUNT_VOLT = 0x01;
static const uint8_t BUS_VOLT = 0x02;
static const uint8_t CURRENT = 0x04;
static const uint8_t CALIBRATION_INA = 0x05;

//===========================================================================
//							Sensor Reset
//===========================================================================

HAL_StatusTypeDef INA_Reset (I2C_HandleTypeDef * hi2c, uint8_t REG_INA){

	uint16_t rst= Reset;
	uint8_t buf[3];
	HAL_StatusTypeDef ret;

	 buf[0] = CONF_INA;
	 buf[1] = (rst & 0xFF00) >> 8; // bitmask
	 buf[2]  = (rst & 0x00FF);

	 ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 3, 10);
	 	if(ret != HAL_OK){
	 		return ret;
	 	}else{
	 		return ret;
	 	}
}

//===========================================================================
//							Sensor Configuration
//===========================================================================

HAL_StatusTypeDef INA_Configuration (I2C_HandleTypeDef * hi2c, uint8_t REG_INA, uint16_t BUS_VOLT, uint16_t SHUT_VOLT, uint16_t BUS_ADC, uint16_t SHUT_ADC, uint16_t MODE){

	uint16_t Geral;
	uint8_t buf[3];
	HAL_StatusTypeDef ret;

	Geral = BUS_VOLT + SHUT_VOLT + BUS_ADC + SHUT_ADC + MODE;

	buf[0] = CONF_INA;
	buf[1] = (Geral & 0xFF00) >> 8; // bitmask
	buf[2]  = (Geral & 0x00FF);

	ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 3, 10);
		if(ret != HAL_OK){
		 		return ret;
		}

	return HAL_OK;
}

//===========================================================================
//							Sensor Calibration
//===========================================================================

HAL_StatusTypeDef INA_Calibration(I2C_HandleTypeDef * hi2c, uint8_t REG_INA, float Max_Current, float RSHUT){

	uint8_t buf[2];
	uint16_t hbuf;
	HAL_StatusTypeDef ret;
	float Current_LSB;
	float Power_LSB;

	Current_LSB = Max_Current/32768;
	Power_LSB = 20 * Current_LSB;
	hbuf = 0.04096/(Current_LSB*RSHUT);

	buf[0] = CALIBRATION_INA;
	buf[1] = (hbuf & 0xFF00) >> 8; // bitmask
	buf[2]  = (hbuf & 0x00FF);

	ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 3, 10);
	if(ret != HAL_OK){
		return ret;
	}
	return HAL_OK;
}

//===========================================================================
//								Measure values
//===========================================================================

HAL_StatusTypeDef INA_Values (I2C_HandleTypeDef * hi2c, uint8_t REG_INA, values* valores){

	uint8_t buf[2];
	HAL_StatusTypeDef ret;
	uint16_t hbuf[1];

	//Shunt Voltage values
	buf [0] = SHUNT_VOLT;
	ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 1, 10);
		if(ret != HAL_OK){
			return ret;
		}else{
			ret = HAL_I2C_Master_Receive(hi2c, REG_INA, buf, 2, 10);
			if(ret != HAL_OK){
				return HAL_ERROR;
			}
	hbuf[0] = buf[0] << 8;
	hbuf[0] = hbuf[0] + buf[1];
	valores->Shunt_Voltage = hbuf[0]/100;
		}
	//Buss Voltage

	buf[0] = CONF_INA;

	ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 1, 10);
	if(ret != HAL_OK){
		return ret;
	}else{
		ret = HAL_I2C_Master_Receive(hi2c, REG_INA, buf, 1, 10);
		if(ret != HAL_OK){
			return ret;
		}else{
			if(buf[0]|0b00000000){ //Bus configuration 16 volts

				buf[0] = BUS_VOLT;

				ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 1, 10);
				if(ret != HAL_OK){
				return ret;
				}else{

					ret = HAL_I2C_Master_Receive(hi2c, REG_INA, buf, 2, 10);
					hbuf[0] = buf[0] << 8;
					hbuf[0] = hbuf[0] + buf[1];

					valores->Bus_Voltage = hbuf[0] * 0.004;
				}

			}

			if(buf[0]|0b00100000){ //Bus configuration 32 volts
				buf[0] = BUS_VOLT;

				ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 1, 10);
				if(ret != HAL_OK){
				return ret;
				}else{

					ret = HAL_I2C_Master_Receive(hi2c, REG_INA, buf, 2, 10);
					hbuf[0] = buf[0] << 8;
					hbuf[0] = hbuf[0] + buf[1];

					valores->Bus_Voltage = hbuf[0] * 0.004;
				}
			}

		}
	}

	//Current Register

	buf[0] = CALIBRATION_INA;

	ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 1, 10);
	if(ret != HAL_OK){
		return ret;
	}else{
		ret = HAL_I2C_Master_Receive(hi2c, REG_INA, buf, 2, 10);
		if(ret != HAL_OK){
			return ret;
		}else{
			hbuf[0] = buf[0] << 8;
			hbuf[0] = hbuf[0] + buf[1];

			buf[0] = SHUNT_VOLT;
			ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 1, 10);
			if(ret != HAL_OK){
			return ret;
			}else{
				ret = HAL_I2C_Master_Receive(hi2c, REG_INA, buf, 2, 10);
				if(ret != HAL_OK){
				return ret;
				}else{
					hbuf[1] = buf[0] << 8;
					hbuf[1] = hbuf[1] + buf[1];

					valores->Current = (hbuf[0]*hbuf[1])/4096;
				}
			}
		}
	}

	//Power Register

	buf[0] = CURRENT;

	ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 1, 10);
	if(ret != HAL_OK){
		return ret;
	}else{
		ret = HAL_I2C_Master_Receive(hi2c, REG_INA, buf, 2, 10);
		if(ret != HAL_OK){
			return ret;
		}else{
			hbuf[0] = buf[0] << 8;
			hbuf[0] = hbuf[0] + buf[1];

			buf[0] = BUS_VOLT;
			ret = HAL_I2C_Master_Transmit(hi2c, REG_INA, buf, 1, 10);
			if(ret != HAL_OK){
			return ret;
			}else{
				ret = HAL_I2C_Master_Receive(hi2c, REG_INA, buf, 2, 10);
				if(ret != HAL_OK){
				return ret;
				}else{
					hbuf[1] = buf[0] << 8;
					hbuf[1] = hbuf[1] + buf[1];

					valores->Power = (hbuf[0]*hbuf[1])/5000;
				}
			}
		}
	}
	return HAL_OK;
}




