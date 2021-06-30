/*
 * neo6m.c
 *
 *  Created on: Jun 20, 2021
 *      Author: leocelente
 */

#include "Application/ublox_neo8.h"

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
	uint8_t message[] = { 0xB5, 0x62, // sync
			0x06, 0x2, // class and id
			0x01, 0x00, // size
						// payload
			0x00, 0x00  // checksum
			};

	ubx_checksum(sizeof(message), message);
	buffer_view_t bv = { .data = message, .size = sizeof(message) };
	return uart_writeN(gps.conn, bv);
}

/***
 * Seleciona Modelo de calculo para High Altitude 1G
 * ideal para sondas
 */
static error_t set_model_mode(ublox_gps_t gps) {
	uint8_t message[] = { 0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06,
			0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00,
			0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x16, 0xDC };
	buffer_view_t bv = { .data = message, .size = sizeof(message) };
	return uart_writeN(gps.conn, bv);
}

static void disableNmea(ublox_gps_t gps) {
	// Array of two bytes for CFG-MSG packets payload.
	const uint8_t messages[][2] = { { 0xF0, 0x0A }, { 0xF0, 0x09 },
			{ 0xF0, 0x00 }, { 0xF0, 0x01 }, { 0xF0, 0x0D }, { 0xF0, 0x06 }, {
					0xF0, 0x02 }, { 0xF0, 0x07 }, { 0xF0, 0x03 },
			{ 0xF0, 0x04 }, { 0xF0, 0x0E }, { 0xF0, 0x0F }, { 0xF0, 0x05 }, {
					0xF0, 0x08 }, { 0xF1, 0x00 }, { 0xF1, 0x01 },
			{ 0xF1, 0x03 }, { 0xF1, 0x04 }, { 0xF1, 0x05 }, { 0xF1, 0x06 }, };

	// CFG-MSG packet buffer.
	uint8_t packet[] = { 0xB5, 0x62, // sync
			0x06, 0x01, // class and id
			0x03, 0x00, // length
			0x00, 0x00, 0x00, // payload (not changed in the case)
			0x00, 0x00, // checksum
			};
	const int packetSize = sizeof(packet);
	const int messageSize = sizeof(messages[0]);
	const int messageCount = sizeof(messages) / messageSize;
	const int payloadOffset = 6;

	// Iterate over the messages array.
	for (int i = 0; i < messageCount; i++) {
		for (int j = 0; j < messageSize; j++) {
			packet[payloadOffset + j] = messages[i][j];
		}
		ubx_checksum(sizeof(packet), packet);
		buffer_view_t bv = { .data = packet, .size = packetSize };
		uart_writeN(gps.conn, bv);
		delay_ms(1);
	}
}
/***
 * Requisita um pacote PVT
 */
static error_t trigger(ublox_gps_t gps) {
	uint8_t message[] = { 0xB5, 0x62, // sync
			0x01, 0x07, // class and id
			0x00, 0x00, // size little endian
						// payload
			0x00, 0x00  // checksum (to be defined)
			};
	ubx_checksum(sizeof(message), message);
	buffer_view_t bv = { .data = message, .size = sizeof(message) };
	return uart_writeN(gps.conn, bv);
}

/***
 * Desativa individualmente cada tipo de mensagem NMEA
 */
static void silence(ublox_gps_t gps){
		buffer_view_t c0 = { .data = "$PUBX,40,GLL,0,0,0,0*5C\r\n", .size = 26 };
		uart_writeN(gps.conn, c0);
		buffer_view_t c1 = { .data = "$PUBX,40,GGA,0,0,0,0*5A\r\n", .size = 26 };
		uart_writeN(gps.conn, c1);
		buffer_view_t c2 = { .data = "$PUBX,40,GSA,0,0,0,0*4E\r\n", .size = 26 };
		uart_writeN(gps.conn, c2);
		buffer_view_t c3 = { .data = "$PUBX,40,RMC,0,0,0,0*47\r\n", .size = 26 };
		uart_writeN(gps.conn, c3);
		buffer_view_t c4 = { .data = "$PUBX,40,GSV,0,0,0,0*59\r\n", .size = 26 };
		uart_writeN(gps.conn, c4);
		buffer_view_t c5 = { .data = "$PUBX,40,VTG,0,0,0,0*5E\r\n", .size = 26 };
		uart_writeN(gps.conn, c5);
}

/***
 * Pede pacote e decodifica
 */
error_t ublox_get(ublox_gps_t gps, ublox_pvt_t *pvt) {
	trigger(gps);
	uint8_t b[6 + 92 + 2 + 1] = { 0 };
	buffer_view_t bv = { .data = b, .size = sizeof(b) };
	uart_readN(gps.conn, bv);
	if (bv.data[0] != 0xB5 && bv.data[1] != 0x62) {
		return 1;
	}

	pvt->time = (bv.data[6 + 0 + 3] << 24 | bv.data[6 + 0 + 2] << 16
			| bv.data[6 + 0 + 1] << 8 | bv.data[6 + 0 + 0] << 0);
	pvt->year = (bv.data[6 + 4 + 1] << 8 | bv.data[6 + 4 + 0] << 0);
	pvt->month = bv.data[6 + 6 + 0];
	pvt->day = bv.data[6 + 7 + 0];
	pvt->hour = bv.data[6 + 8 + 0];
	pvt->minute = bv.data[6 + 9 + 0];
	pvt->second = bv.data[6 + 10 + 0];
	pvt->valid = bv.data[6 + 11 + 0];
	pvt->tAcc = (bv.data[6 + 12 + 3] << 24 | bv.data[6 + 12 + 2] << 16
			| bv.data[6 + 12 + 1] << 8 | bv.data[6 + 12 + 0] << 0);
	pvt->nano = (bv.data[6 + 16 + 3] << 24 | bv.data[6 + 16 + 2] << 16
			| bv.data[6 + 16 + 1] << 8 | bv.data[6 + 16 + 0] << 0);

	pvt->lat = (bv.data[6 + 24 + 3] << 24 | bv.data[6 + 24 + 2] << 16
			| bv.data[6 + 24 + 1] << 8 | bv.data[6 + 24 + 0] << 0);
	pvt->lng = (bv.data[6 + 28 + 3] << 24 | bv.data[6 + 28 + 2] << 16
			| bv.data[6 + 28 + 1] << 8 | bv.data[6 + 28 + 0] << 0);
	return 0;
}

/***
 * Inicializa GPS ublox com o protocolo binario UBX
 */
error_t ublox_init(ublox_gps_t gps) {
	silence(gps);
	disableNmea(gps); // TODO: conferir se é necessário
	set_protocol(gps);
	set_model_mode(gps);
	return 0; // TODO: checar erros
}

