//
// Created by gusta on 27/01/2022.
//
#include <stdio.h>
#include "storage.h"



error_t storage_log_ras(storage_file_t const *file, ras_blob_t const* data){

	//Escrevendo no Cartao SD(csv)

	//Concatena o nome do arquivo com o .txt
	char* sd_filename;
	sprintf(sd_filename, "%s.txt", file->filename);

	FILE* sdfile = fopen(sd_filename, file->format);

	char sd_buffer[256] = {0};

	sprintf(sd_buffer, "%.02f, %.02f, %.02f, %.02f, %.02f, %.02f, %.02f, %.02f, %.02f, %.02f, %.02f, %.02f, %.02f, ",
				   data->values.preassure_primary,
				   data->values.temp_1, data->values.temp_2, data->values.temp_3,
				   data->values.v_1, data->values.v_2, data->values.v_3,
				   data->values.i_1, data->values.i_2, data->values.i_3,
				   data->values.humidity, data->values.temp_amb,
				   data->values.batt_charge);

	fwrite(sd_buffer, sizeof(sd_buffer), 1, sdfile);
	fclose(sdfile);

	//Escrevendo na memória FLASH

	char* flash_filename;
	sprintf(flash_filename, "%s.bin", file->filename);

	FILE* flashfile = fopen(flash_filename, file->format);

	uint8_t *pt;


	for(int i=0; i < sizeof(ras_blob_values_t); i++){

	    pt = &data->raw[i];

	    fwrite(pt, sizeof(uint8_t), 1, flashfile);
	}

	fclose(flashfile);
	return 0;
}

error_t storage_log_obc(storage_file_t const *file, obc_blob_t const* data){

	//Escrevendo no Cartao SD(csv)
	char* sd_filename;
	sprintf(sd_filename, "%s.txt", file->filename);

	FILE* sdfile = fopen(sd_filename, file->format);


	uint8_t buffer_data[256] = {0};
	uint8_t *pt_sd;

	for(int i=0; i < sizeof(obc_blob_t); i++){

		pt_sd = &data[i];

		fwrite(pt_sd, sizeof(uint8_t), 1, sdfile);
	}

	fclose(sdfile);


	//Escrevendo na memória FLASH
	char* flash_filename;
	sprintf(flash_filename, "%s.bin", file->filename);

	FILE* flashfile = fopen(flash_filename, file->format);

	uint8_t *pt_flash;

	for(int i=0; i < sizeof(obc_blob_t); i++){

	    pt_flash = &data[i];

	    fwrite(pt_flash, sizeof(uint8_t), 1, flashfile);
	}

	fclose(flashfile);
	return 0;
}

error_t storage_log_gps(storage_file_t const *file, gps_blob_t const* data){


	//Escrevendo no Cartao SD(csv)
	char* sd_filename;
	sprintf(sd_filename, "%s.txt", file->filename);

	FILE* sdfile = fopen(sd_filename, file->format);


	//Dia/horario
	char data_uint8_buffer[256] = {0};

	sprintf(data_uint8_buffer, "%d, %d, %d, %d, %d, %d, ", data->values.day, data->values.month, data->values.year, data->values.second, data->values.minute, data->values.hour);

	//Localidade
	float data_float_buffer[3] = {0};

	sprintf(data_float_buffer, "%.02f, %.02f, %.02f, ", data->values.latitude, data->values.longitude, data->values.altitude);

	fwrite(data_uint8_buffer, sizeof(data_uint8_buffer), 1, sdfile);
	fwrite(data_float_buffer, sizeof(data_float_buffer), 1, sdfile);

	fclose(sdfile);

	//Escrevendo na memória FLAS
	char* flash_filename;
	sprintf(flash_filename, "%s.bin", file->filename);

	FILE* flashfile = fopen(flash_filename, file->format);

	uint8_t *pt_flash;

	for(int i=0; i < sizeof(gps_event_t); i++){

	    pt_flash = &data->raw[i];

	    fwrite(pt_flash, sizeof(uint8_t), 1, flashfile);
	}

	fclose(flashfile);
	return 0;
}



