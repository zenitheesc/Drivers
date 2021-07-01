/*
 * sdcard.c
 *
 *  Created on: 1 de jul de 2021
 *      Author: leocelente
 */

#include "Storage/sdcard.h"

error_t sdcard_init(sdcard_file_t file) {
	int err = FR_OK;
	err = f_mount(&SDFatFS, "", 1);
	if (err) {
		return err;
	}
	// FA_OPEN_ALWAYS: Opens the file if it is existing. If not, a new file will be created.
	err = f_open(file.file, file.filename, FA_OPEN_ALWAYS);
	if (err) {
		return err;
	}
	f_close(file.file);
	return (error_t) err;
}

result16_t sdcard_append(sdcard_file_t file, buffer_view_t view) {
	int err = f_open(file.file, file.filename, FA_OPEN_APPEND | FA_WRITE);
	result16_t out = { .hasError = err, .value = 0 };
	if (err) {
		return out;
	}

	unsigned int written_bytes = 0; // bytes escritos
	err = f_write(file.file, view.data, view.size, &written_bytes);

	f_close(file.file);
	out.hasError = err;
	// TODO: possivel truncamento da quantidade de bytes escritos.
	out.value = (uint16_t) written_bytes;
	return out;
}


void sdcard_destroy(){
	f_mount(0, "", 0);
}



