/*
 * ublox_neo8.h
 *
 *  Created on: Jun 20, 2021
 *      Author: leocelente
 */

#ifndef UBLOX_GPS_H_
#define UBLOX_GPS_H_

#include "platform/platform.h"

#define UBLOX_I2C
#if defined UBLOX_I2C
typedef i2c_device_t connection_t;
#define uart_transmit i2c_transmit
#define uart_receive i2c_receive

#define UBLOX_I2C_ADDR 0x42
#define UBLOX_STREAM_REG 0xFF
#define UBLOX_LEN_LSB_REG 0xFE
#define UBLOX_LEN_HSB_REG 0xFD

#else
typedef uart_connection_t connection_t;
#endif

typedef struct {
	connection_t conn;
} ublox_gps_t;

typedef struct {
	uint32_t time;		// GPS time of week of the navigation epoch.
	uint16_t year;		// Year
	uint8_t month;		// Month, range
	uint8_t day;		// Day of month
	uint8_t hour;		// Hour of day
	uint8_t minute;		// Minute of hour
	uint8_t second;		// Seconds of minute
	uint8_t valid;		// Validity flags
	uint32_t tAcc;		// Time accuracy estimate
	int32_t nano;		// Fraction of second
	uint8_t fixType;	// GNSSfix Type
	uint8_t flags;		// Fix status flags
	uint8_t flags2;		// Additional flags
	uint8_t sats;		// Number of satellites used in Nav Solution
	int32_t lng;		// Longitude
	int32_t lat;		// Latitude
	int32_t height;		// Height above ellipsoid
	int32_t hMSL;		// Height above mean sea level
	uint32_t hAcc;		// Horizontal accuracy estimate
	uint32_t vAcc;		// Vertical accuracy estimate
	int32_t velN;		// NED north velocity
	int32_t velE;		// NED east velocity
	int32_t velD;		// NED down velocity
	int32_t gSpeed;		// Ground Speed (2-D)
	int32_t headMot;	// Heading of motion (2-D)
	uint32_t sAcc; 		// Speed accuracy estimate
	uint32_t headAcc;	// Heading accuracy estimate
	uint16_t pDOP;		// Position DOP
	uint16_t flags3;	// Additional flags
	uint8_t _reserved_;
	int32_t headVeh; 	// Heading of vehicle (2-D), this is only valid when headVehValid is set, otherwise the output is set to the heading of motion
	int16_t magDev; 	// Magnetic declination. Only supported in ADR 4.10 and later.
	uint16_t magAcc; 	// Magnetic declination accuracy. Only supported in ADR 4.10 and later.
} ublox_pvt_t;

typedef union {
	ublox_pvt_t values;
	uint8_t raw[sizeof(ublox_pvt_t)];
} ubx_pvt_parser_t;

error_t ublox_get(ublox_gps_t gps, ublox_pvt_t *pvt);
error_t ublox_init(ublox_gps_t gps);
#endif /* UBLOX_GPS_H_ */
