/*
 * ublox_neo8.c
 *
 *  Created on: Jun 20, 2021
 *      Author: leocelente
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "ublox_neo8.h"
#include <math.h>

// Calculate checksum over the packet buffer excluding sync (first two) and checksum chars (last two).
static void ubx_checksum(int len, uint8_t packet[len]) {
	packet[len - 2] = 0x00;
	packet[len - 1] = 0x00;

	for (uint8_t j = 0; j < len - 4; j++) {
		packet[len - 2] += packet[2 + j];
		packet[len - 1] += packet[len - 2];
	}
}

/***
 * Seleciona UBX como protocolo principal
 */
static error_t set_protocol(ublox_gps_t gps) {
	static uint8_t message[] = { 0xB5, 0x62, // sync
			0x06, 0x2, // class and id
			0x01, 0x00, // size
						// payload
			0x00, 0x00  // checksum
			};

	ubx_checksum(sizeof(message), message);
	buffer_view_t bv = { .data = message, .size = sizeof(message) };


	return gps_transmit(gps.conn, bv);
}

/***
 * Seleciona Modelo de calculo para High Altitude 1G
 * ideal para sondas
 */
static error_t set_model_mode(ublox_gps_t gps) {
	static const  uint8_t message[] = { 0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06,
			0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00,
			0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x16, 0xDC };
	buffer_view_t bv = { .data = message, .size = sizeof(message) };
	return gps_transmit(gps.conn, bv);
}

static void disableNmea(ublox_gps_t gps) {
	// Array of two bytes for CFG-MSG packets payload.
	static const uint8_t messages[][2] = { { 0xF0, 0x0A }, { 0xF0, 0x09 },
			{ 0xF0, 0x00 }, { 0xF0, 0x01 }, { 0xF0, 0x0D }, { 0xF0, 0x06 }, {
					0xF0, 0x02 }, { 0xF0, 0x07 }, { 0xF0, 0x03 },
			{ 0xF0, 0x04 }, { 0xF0, 0x0E }, { 0xF0, 0x0F }, { 0xF0, 0x05 }, {
					0xF0, 0x08 }, { 0xF1, 0x00 }, { 0xF1, 0x01 },
			{ 0xF1, 0x03 }, { 0xF1, 0x04 }, { 0xF1, 0x05 }, { 0xF1, 0x06 }, };

	// CFG-MSG packet buffer.
	static  uint8_t packet[] = { 0xB5, 0x62, // sync
			0x06, 0x01, // class and id
			0x03, 0x00, // length
			0x00, 0x00, 0x00, // payload (not changed in the case)
			0x00, 0x00, // checksum
			};
	int packetSize = sizeof(packet);
	int messageSize = sizeof(messages[0]);
	int messageCount = sizeof(messages) / messageSize;
	int payloadOffset = 6;

	// Iterate over the messages array.
	for (int i = 0; i < messageCount; i++) {
		for (int j = 0; j < messageSize; j++) {
			packet[payloadOffset + j] = messages[i][j];
		}
		ubx_checksum(sizeof(packet), packet);
		buffer_view_t bv = { .data = packet, .size = packetSize };


		gps_transmit(gps.conn, bv);


		delay_ms(1);
	}
}

/***
 * Requisita um pacote PVT
 */
static error_t trigger(ublox_gps_t gps) {
	static uint8_t message[] = { 0xB5, 0x62, // sync
			0x01, 0x07, // class and id
			0x00, 0x00, // size little endian
						// payload
			0x00, 0x00  // checksum (to be defined)
			};
	ubx_checksum(sizeof(message), message);
	buffer_view_t bv = { .data = message, .size = sizeof(message) };
	return gps_transmit(gps.conn, bv);
}


static uint8_t get(ublox_gps_t gps){
	uint8_t x = 0;
	buffer_view_t data_v = {.data=&x, sizeof(x)};
	gps_receive(gps.conn, data_v);
	return x;
}

static uint8_t wait_for(ublox_gps_t gps, uint8_t data, uint32_t attempts){
	while (attempts--) {
		uint8_t x = get(gps);
		if(x == data) return x;
//		delay_ms(1);
	}
	return attempts;
}

/***
 * Pede pacote e decodifica
 */
error_t ublox_get(ublox_gps_t gps, ublox_pvt_t *pvt) {
	trigger(gps);
#if defined UBLOX_I2C
	uint8_t stream = UBLOX_STREAM_REG;
	error_t e = i2c_transmit(gps.conn, (buffer_view_t){.data=&stream, .size=1});
	if(e){
		return e;
	}

#endif
	wait_for(gps, 0xB5, 150);
	wait_for(gps, 0x62, 1);
	wait_for(gps, 0x01, 1);
	wait_for(gps, 0x07, 1);
	wait_for(gps, 0x5C, 1);
	wait_for(gps, 0x00, 1);

	static uint8_t message[92+2] = {0};
	for(int i = 0; i < sizeof(message); ++i){
		uint8_t received = get(gps);
		message[i] = received;
	}

	static ubx_pvt_parser_t data = {0};
	memcpy(data.raw, message, sizeof(data.raw));
	*pvt = data.values;

	if((pvt->valid & 0x07) != 0x7){
		return 2;
	}

	if(fabs(pvt->lat) < 1e-9 || fabs(pvt->lng) < 1e-9){
		return 2;
	}
	return 0;
}

/***
 * Inicializa GPS ublox com o protocolo binario UBX
 */
error_t ublox_init(ublox_gps_t gps) {
	disableNmea(gps);
	set_protocol(gps);
	set_model_mode(gps);
	return 0;
}

