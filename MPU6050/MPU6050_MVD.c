/*
 * MPU6050_MVD.c
 *
 *  Created on: Oct 19, 2021
 *      Author: Murilo Henrique Pasini Trevisan
 */


#include "MPU6050_MVD.h"

/* Configurações do sensor */

// Função para enviar via i2c o valor do samplerate
error_t MPU6050_smprt(MPU6050_t mpu) {

	//Caso o valor de reset não seja o mesmo do datasheet usar esta parte comentada
	// result8_t config_raw = i2c_read8(mpu.device, MPU_SMPRT_DIV);
	// config_reg = config_raw.value

	uint8_t smprt_reg = 0x00;

	smprt_reg |= mpu.config.Sample;

	return i2c_write8(mpu.device, MPU_SMPRT_DIV, smprt_reg);
}


// Função para enviar via i2c o valor do registrador de configuração geral
error_t MPU6050_config(MPU6050_t mpu) {

	//Caso o valor de reset não seja o mesmo do datasheet usar esta parte comentada
	// result8_t config_raw = i2c_read8(mpu.device, MPU_CONFIG);
	// config_reg = config_raw.value

	uint8_t config_reg = 0x00;

	config_reg |= mpu.config.FSyncEnable;
	config_reg |= mpu.config.LowPassFilter;

	return i2c_write8(mpu.device, MPU_CONFIG, config_reg);
}

// Função para enviar via i2c o valor do registrador de configuração do giroscópio
error_t MPU6050_gyro_config(MPU6050_t mpu) {

	//Caso o valor de reset não seja o mesmo do datasheet usar esta parte comentada
	//result8_t config_raw = i2c_read8(mpu.device, MPU_GYRO_CONFIG);
	//gyro_config_reg = config_raw.value

	uint8_t gyro_config_reg = 0x00;

	gyro_config_reg |= mpu.config.GyroRange;

	return i2c_write8(mpu.device, MPU_GYRO_CONFIG, gyro_config_reg);
}

// Função para enviar via i2c o valor do registrador de configuração do acelerômetro
error_t MPU6050_accel_config(MPU6050_t mpu) {

	//Caso o valor de reset não seja o mesmo do datasheet usar esta parte comentada
	//result8_t config_raw = i2c_read8(mpu.device, MPU_ACCEL_CONFIG);
	//accel_config_reg = config_raw.value

	uint8_t accel_config_reg = 0x00;

	accel_config_reg |= mpu.config.AccelRange;

	return i2c_write8(mpu.device, MPU_ACCEL_CONFIG, accel_config_reg);
}

error_t MPU6050_power(MPU6050 mpu) {

	//Caso o valor de reset não seja o mesmo do datasheet usar esta parte comentada
	//result8_t config_raw = i2c_read8(mpu.device, MPU_ACCEL_CONFIG);
	//power_config_reg = config_raw.value

	uint8_t power_config_reg = 0x00;

	return i2c_write8(mpu.device, MPU_PWR_MGMT1, power_config_reg);
}

error_t MPU6050_init(MPU6050_t mpu) {

	//Realiza todas as configurações em uma única função
	//para facilitar a execução do programa na main

	MPU6050_power(mpu);			//Desabilita o reset e o sleep
	MPU6050_smprt(mpu);			//Configura taxa de amostragem
	MPU6050_config(mpu);		//Configurações gerais
	MPU6050_accel_config(mpu);	//Configurações do acelerometro
	MPU6050_gyro_config(mpu);	//Configurações do giroscopio

	return 0;
}


/* Leituras do sensor */

error_t MPU6050_measure(MPU6050_t mpu, MPU6050_values_t *medida) {

	//Realiza a leitura dos registradores dos resultados
	//Separa os resultados em cada uma das leituras
	//Converte para as medidas necessárias
	//Armazena na struct de resultados

	//Criação do buffer para leitura burst-read
	uint8_t buffer[14] = { 0 };
	buffer_view_t buffer_view =  { .data = buffer, .size = sizeof(buffer) };

	//Leitura de todos os registradores de resultados e save no buffer
	error_t error = i2c_readN(mpu.device, MPU_MEASURES, buffer_view);

	//Tratamento de erro da HAL
	if (error) {
		return error;
	}

	//Separação das componentes da leitura
	//Acelerômetro
	int16_t accel_x = (buffer[0] << 8) | buffer[1];
	int16_t accel_y = (buffer[2] << 8) | buffer[3];
	int16_t accel_z = (buffer[4] << 8) | buffer[5];
	//Temperatura
	int16_t temp    = (buffer[6] << 8) | buffer[7];
	//Giroscópio
	int16_t gyro_x  = (buffer[8] << 8) | buffer[9];
	int16_t gyro_y  = (buffer[10] << 8) | buffer[11];
	int16_t gyro_z  = (buffer[12] << 8) | buffer[13];

	//Calculo das escalas de medição
	//Acelerometro
	int const fullScaleA = (mpu.config.AccelRange >> SCALE_SHIFT);	//Obtem o valor de fundo de escala selecionado (1-3)
	int const invScaleA  = SCALE_INV_A - fullScaleA;				//Coloca em ordem decrescente o valor de escala(3-1)
	int const bitScaleA  = (invScaleA + ACCEL_BIT);					//obtem o tamanho em bits do fundo de escala (11 - 14)
	int const scaleAcc   = (BIT_DIV << bitScaleA);					//multiplica o valor e coloca em escala(2048 - 16384)
	//Giroscopio
	int const fullScaleG = (mpu.config.GyroRange >> SCALE_SHIFT);	//Obtem o valor de fundo de escala selecionado (1-3)
	int const invScaleG  = SCALE_INV_G - fullScaleG;				//Coloca em ordem decrescente o valor de escala(6-4)
	int const bitScaleG  = (invScaleG + GYRO_BIT);					//obtem o tamanho em bits do fundo de escala (11 - 14)
	int const rangeGyr   = (BIT_DIV << bitScaleG);					//multiplica o valor e coloca em escala(2048 - 16384)
	int const scaleGyr   = (rangeGyr / UNIT_DIV);					//ajusta a unidade de acelereção


	//Alocação na struct de resultados
	//Acelerômetro
	medida -> AccelX = ((float)accel_x / scaleAcc);
	medida -> AccelY = ((float)accel_y / scaleAcc);
	medida -> AccelZ = ((float)accel_z / scaleAcc);
	//Temperatura
	medida -> Temp   = ((float)temp / TMP_DIV_CONST) + TMP_SUM_CONST;
	//Giroscópio
	medida -> GyroX  = ((float)gyro_x / scaleGyr);
	medida -> GyroY  = ((float)gyro_y / scaleGyr);
	medida -> GyroZ  = ((float)gyro_z / scaleGyr);

	return 0;
}

