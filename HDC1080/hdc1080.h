/*
 * hdc1080.h
 *
 *  Created on: Jun 20, 2021
 *      Author: leocelente
 */

#ifndef INC_APPLICATION_HDC1080_H_
#define INC_APPLICATION_HDC1080_H_

#include "bsp.h"

typedef struct {
	float temperature;
	float humidity;
} hdc1080_mesurement_t;

error_t hdc1080_init(i2c_device_t device);

result16_t hdc_1080_get_humidity_raw(i2c_device_t device);
result16_t hdc_1080_get_temperature_raw(i2c_device_t device);
void hdc_1080_get_both_raw(i2c_device_t device, uint16_t *h_raw_out,
		uint16_t *t_raw_out);

hdc1080_mesurement_t hdc_1080_mesure(i2c_device_t device);

float hdc_1080_convert_temperature(uint16_t raw);
float hdc_1080_convert_humidity(uint16_t raw);

#define HDC1080_ADDR 		((uint8_t)(0x40))
#define HDC1080_CONFIG 		((uint8_t)(0x02))
#define HDC1080_DEVID 		((uint8_t)(0xFF))
#define HDC1080_MEASURE 	((uint8_t)(0x00))
#define HDC1080_HUMIDITY 	((uint8_t)(0x01))

#define HDC1080_HEAT	((uint16_t)(1 << 13))
#define HDC1080_MESURE_BOTH	((uint16_t)(1 << 12))
#define HDC1080_TEMP_14 ((uint16_t)(0 << 10))
#define HDC1080_HUMD_14 ((uint16_t)((0 << 9) & (0 << 8)))
#endif /* INC_APPLICATION_HDC1080_H_ */
