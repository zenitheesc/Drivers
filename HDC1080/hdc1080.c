/*
 * hdc1080.c
 *
 *  Created on: Jun 20, 2021
 *      Author: leocelente
 */

#include "Application/hdc1080.h"

static const float div = 65536.0f;

error_t hdc1080_init(i2c_device_t device) {
	delay_ms(1);
	result16_t id = i2c_read16(device, HDC1080_DEVID);
	if (id.hasError) {
		return id.hasError;
	}
	if (id.value != 0x1050) {
		return 1; // sensor not correct
	}
	delay_ms(14); // valor maximo necessario

	uint16_t config = 0x0000;
	config |= HDC1080_MESURE_BOTH;
	config |= HDC1080_TEMP_14;
	config |= HDC1080_HUMD_14;

	return i2c_write16(device, HDC1080_CONFIG, config);
}

float hdc_1080_convert_temperature(uint16_t raw) {
	return raw / div * 165.0f ;//- 40.0f; // não funciona
}

float hdc_1080_convert_humidity(uint16_t raw) {
	return  raw / div * 100.0f;
}

/***
 * Temperatura não funciona. Não achei o porque
 */
void hdc_1080_get_both_raw(i2c_device_t device, uint16_t *h_raw_out,
		uint16_t *t_raw_out) {

	// trigger mesurement
	uint8_t useless = 0x00;
	buffer_view_t view = { .data = &useless, .size = 0 };
	if (i2c_writeN(device, HDC1080_MEASURE, view)) {
		*h_raw_out = 0;
		*t_raw_out = 0;
		return;
	}
	delay_ms(15);

	//TODO: criar acesso a essa função na BSP
	uint8_t data[4] = { 0 };
	if (HAL_I2C_Master_Receive(device.i2c, HDC1080_ADDR << 1, data, 4,
			HAL_MAX_DELAY) != HAL_OK) {
		*h_raw_out = 0;
		*t_raw_out = 0;
	}
	*t_raw_out = data[0] << 8 | data[1];
	*h_raw_out = data[2] << 8 | data[3];
}

hdc1080_mesurement_t hdc_1080_mesure(i2c_device_t device) {
	uint16_t t = 0;
	uint16_t h = 0;
	hdc_1080_get_both_raw(device, &h, &t);
	float humidity = hdc_1080_convert_humidity(h);
	float temperature = hdc_1080_convert_temperature(t);
	hdc1080_mesurement_t out = { .humidity = humidity, .temperature =
			temperature };
	return out;
}
