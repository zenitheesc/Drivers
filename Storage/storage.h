//
// Created by gusta on 27/01/2022.
//

#ifndef MS5607_C_STORAGE_H
#define MS5607_C_STORAGE_H

//#include "CDH.h"
#include "platform/platform.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
    char* format;
    char* filename;
    int file_id;
} storage_file_t;


//-

//RAS
typedef struct {
	float preassure_primary;
	float temp_1, temp_2, temp_3;
	float v_1, v_2, v_3;
	float i_1, i_2, i_3;
	float humidity, temp_amb;
	float batt_charge;
} ras_blob_values_t;

typedef union {
	ras_blob_values_t values;
	uint8_t raw[sizeof(ras_blob_values_t)];
} ras_blob_t;

//OBC
typedef uint8_t obc_blob_t[30];

//GPS
typedef struct {
	uint8_t day, month, year;
	uint8_t second, minute, hour;
	float latitude, longitude, altitude;

} gps_event_t;

typedef union {
	gps_event_t values;
	uint8_t raw[sizeof(gps_event_t)];
} gps_blob_t;

//-



typedef struct {
// flash device
// SDCard device
} storage_t;

error_t storage_init(storage_t * storage);
error_t storage_file_init(storage_file_t *file);

error_t storage_log_ras(storage_file_t const *file_, ras_blob_t const* data);
error_t storage_log_obc(storage_file_t const *file, obc_blob_t const* data);
error_t storage_log_gps(storage_file_t const *file, gps_blob_t const* data);

#endif //MS5607_C_STORAGE_H
