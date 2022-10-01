/*
 * MPU6050_MVD.c
 *
 *  Created on: Oct 19, 2021
 *      Author: Murilo Henrique Pasini Trevisan
 */


#include "MPU6050_MVD.h"

static result_uint8_t read(MPU6050_t mpu, uint8_t addr) {
	buffer_view_t tx_v = {.size=sizeof(addr), .data=&addr};
	error_t e = i2c_transmit(mpu.device, tx_v);
	uint8_t rx = 0;
	buffer_view_t rx_v = {.size=sizeof(rx), .data=&rx};
	e |= i2c_receive(mpu.device, rx_v);
	result_uint8_t res = {.hasError = e, .value = rx};
	return res;
}

static error_t write(MPU6050_t mpu, uint8_t addr, uint8_t value){
	uint8_t tx[] = {addr, value};
	buffer_view_t tx_v = {.size=sizeof(tx), .data=tx};
	return i2c_transmit(mpu.device, tx_v);
}

/* Configurações do sensor */

// Função para enviar via i2c o valor do samplerate
error_t MPU6050_smprt(MPU6050_t mpu) {

	//Caso o valor de reset não seja o mesmo do datasheet usar esta parte comentada
	// result8_t config_raw = read(mpu, MPU_SMPRT_DIV);
	// config_reg = config_raw.value

	uint8_t smprt_reg = 0x00;

	smprt_reg |= mpu.config.Sample;

	return write(mpu, MPU_SMPRT_DIV, smprt_reg);
}


// Função para enviar via i2c o valor do registrador de configuração geral
error_t MPU6050_config(MPU6050_t mpu) {

	//Caso o valor de reset não seja o mesmo do datasheet usar esta parte comentada
	// result8_t config_raw = read(mpu, MPU_CONFIG);
	// config_reg = config_raw.value

	uint8_t config_reg = 0x00;

	config_reg |= mpu.config.FSyncEnable;
	config_reg |= mpu.config.LowPassFilter;

	return write(mpu, MPU_CONFIG, config_reg);
}

// Função para enviar via i2c o valor do registrador de configuração do giroscópio
error_t MPU6050_gyro_config(MPU6050_t mpu) {

	//Caso o valor de reset não seja o mesmo do datasheet usar esta parte comentada
	//result8_t config_raw = read(mpu, MPU_GYRO_CONFIG);
	//gyro_config_reg = config_raw.value

	uint8_t gyro_config_reg = 0x00;

	gyro_config_reg |= mpu.config.GyroRange;

	return write(mpu, MPU_GYRO_CONFIG, gyro_config_reg);
}

// Função para enviar via i2c o valor do registrador de configuração do acelerômetro
error_t MPU6050_accel_config(MPU6050_t mpu) {

	//Caso o valor de reset não seja o mesmo do datasheet usar esta parte comentada
	//result8_t config_raw = read(mpu, MPU_ACCEL_CONFIG);
	//accel_config_reg = config_raw.value

	uint8_t accel_config_reg = 0x00;

	accel_config_reg |= mpu.config.AccelRange;

	return write(mpu, MPU_ACCEL_CONFIG, accel_config_reg);
}

error_t MPU6050_init(MPU6050_t mpu) {

	//Realiza todas as configurações em uma única função
	//para facilitar a execução do programa na main

	write(mpu, 0x6B, 0);
	MPU6050_smprt(mpu);

	MPU6050_config(mpu);
	MPU6050_accel_config(mpu);
	MPU6050_gyro_config(mpu);

	return 0;
}

/* Leituras do sensor */

error_t MPU6050_measure(MPU6050_t mpu, MPU6050_values_t *medida) {

	//Realiza a leitura dos registradores dos resultados
	//Separa os resultados em cada uma das leituras
	//Converte para as medidas necessárias
	//Armazena na struct de resultados

	//Criação do buffer para leitura burst-read
	uint8_t buffer[14] = { 0xFF, [10] = 0xFF };
	buffer_view_t buffer_view =  { .data = buffer, .size = sizeof(buffer) };

	//Leitura de todos os registradores de resultados e save no buffer
	uint8_t addr = MPU_MEASURES; // envia endereço do registrador primeiro
	buffer_view_t addr_v = {.size=sizeof(addr), .data=&addr};
	i2c_transmit(mpu.device, addr_v);
	error_t error = i2c_receive(mpu.device, buffer_view);

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

	//Alocação na struct de resultados
	//Acelerômetro
	int const scaleAcc = 1 << ((3 - (mpu.config.AccelRange / (1 << 3))) + 11);
	medida -> AccelX = ((float)accel_x) / scaleAcc;
	medida -> AccelY = ((float)accel_y) / scaleAcc;
	medida -> AccelZ = ((float)accel_z) / scaleAcc;
	//Temperatura
	medida -> Temp   = (((float)temp) / 340.0f) + 36.53f;
	//Giroscópio
	int const scaleGyr = (1 << ( 17 - (mpu.config.GyroRange >> 3))) / 1000.f;

	medida -> GyroX  = (float)gyro_x / scaleGyr;
	medida -> GyroY  = (float)gyro_y / scaleGyr;
	medida -> GyroZ  = (float)gyro_z / scaleGyr;

	return 0;
}

