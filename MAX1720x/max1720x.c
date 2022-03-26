/*
 * max17205.c
 *
 *  Created on: Mar 20, 2022
 *      Author: leocelente
 */

#include <max1720x.h>

/**
 * Read register
 */
static result_uint16_t read(max1720x_t const *max, uint8_t addr) {
	result_uint16_t result = { 0 };
	buffer_view_t addr_tr = { .data = &addr, .size = sizeof(addr) };
	result.hasError |= i2c_transmit(max->dev, addr_tr);
	uint8_t rx[2] = { 0 };
	buffer_view_t read_tr = { .data = rx, .size = sizeof(rx) };
	result.hasError |= i2c_receive(max->dev, read_tr);
	result.value = (rx[0] << 8) | (rx[1]);
	return result;
}

/**
 * Write register
 */
static error_t write(max1720x_t const *max, uint8_t addr, uint16_t value) {
	uint8_t msb = (value & 0xFF00) >> 8;
	uint8_t lsb = value;
	uint8_t data[] = { addr, lsb, msb };
	buffer_view_t transmission = { .data = data, .size = sizeof(data) };
	return i2c_transmit(max->dev, transmission);;
}

/**
 * Full device reset
 */
static void reset(max1720x_t const *max) {
	write(max, 0x60, 0x00F);
	delay_ms(50);
	write(max, 0xBB, 0x001);
}

error_t max1720x_init(max1720x_t *max) {
	result_uint16_t ok = read(max, 0x21);
	if(ok.hasError){
		return ERROR;
	}

	if(ok.value != 0x01 || ok.value != 0x05){
		return ERROR;
	}

	reset(max);
	return SUCCESS;
}

result_max1720x_values_t max1720x_measure(max1720x_t *max) {
	result_max1720x_values_t out = { .value = { 0 }, .hasError = 0 };

	result_uint16_t result_soc = read(max, 0x06);
	out.hasError |= result_soc.hasError;
	out.value.state_of_charge = ((float) result_soc.value) / 256.f;

	result_uint16_t result_cap = read(max, 0x05);
	out.hasError |= result_cap.hasError;
	out.value.capacity = ((float) result_cap.value) * 0.005f / 0.01f;

	result_uint16_t result_tte = read(max, 0x11);
	out.hasError |= result_tte.hasError;
	out.value.time_to_empty = ((float) result_tte.value) * 5.625f;
	return out;
}
