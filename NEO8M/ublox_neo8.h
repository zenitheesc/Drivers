/*
 * neo6m.h
 *
 *  Created on: Jun 20, 2021
 *      Author: leocelente
 */

#ifndef INC_APPLICATION_UBLOX_GPS_H_
#define INC_APPLICATION_UBLOX_GPS_H_

#include "bsp.h"

typedef struct {
	uart_connection_t conn;
} ublox_gps_t;

typedef struct {
	uint32_t time;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t valid;
	uint32_t tAcc;
	int32_t nano;
	int32_t lat;
	int32_t lng;

} ublox_pvt_t;

error_t ublox_get(ublox_gps_t gps, ublox_pvt_t *pvt);
error_t ublox_init(ublox_gps_t gps);
#endif /* INC_APPLICATION_UBLOX_GPS_H_ */
