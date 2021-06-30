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
	config |= HDC1080_MESURE_SINGLE;
	config |= HDC1080_TEMP_14;
	config |= HDC1080_HUMD_14;

	return i2c_write16(device, HDC1080_CONFIG, config);
}

float hdc_1080_convert_temperature(uint16_t raw) {
	return raw / div * 165.0f - 40.0f;
}

float hdc_1080_convert_humidity(uint16_t raw) {
	return  raw / div * 100.0f;
}

/***
 * Depreciado, resultados inconsistentes na humidade
 */
static void hdc_1080_get_both_raw(i2c_device_t device, uint16_t *h_raw_out,
		uint16_t *t_raw_out) {

	// trigger mesurement
	uint8_t useless = 0x00;
	buffer_view_t view = { .data = &useless, .size = 1 };
	i2c_transmit(device, view);

	delay_ms(15);

	uint8_t data[4] = { 0 };
	buffer_view_t raw_view = {.data = data, .size = sizeof(data)};
	i2c_receive(device, raw_view);

	*t_raw_out = data[0] << 8 | data[1];
	*h_raw_out = data[2] << 8 | data[3];

}

result16_t hdc1080_get_temperature_raw(i2c_device_t device){
	result16_t out = {.value = 0, .hasError=1};
	uint8_t address = 0x00;
	buffer_view_t view = { .data = &address, .size = 1 };
	i2c_transmit(device, view);

	delay_ms(7);

	uint8_t data[4] = { 0 };
	buffer_view_t raw_view = {.data = data, .size = sizeof(data)};
	out.hasError = i2c_receive(device, raw_view);
	out.value = data[0] << 8 | data[1];
	return out;
}

result16_t hdc1080_get_humidity_raw(i2c_device_t device){
	result16_t out = {.value = 0, .hasError=1};
	uint8_t address = 0x01;
	buffer_view_t view = { .data = &address, .size = 1 };
	i2c_transmit(device, view);

	delay_ms(7);

	uint8_t data[4] = { 0 };
	buffer_view_t raw_view = {.data = data, .size = sizeof(data)};
	out.hasError = i2c_receive(device, raw_view);
	out.value = data[0] << 8 | data[1];
	return out;
}

hdc1080_mesurement_t hdc1080_mesure(i2c_device_t device) {
	result16_t t = hdc1080_get_temperature_raw(device);
	result16_t h = hdc1080_get_humidity_raw(device);

	float humidity = hdc_1080_convert_humidity(h.value);
	float temperature = hdc_1080_convert_temperature(t.value);
	hdc1080_mesurement_t out = { .humidity = humidity, .temperature =
			temperature };
	return out;
}
