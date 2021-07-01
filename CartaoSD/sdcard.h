/*
 * sdcard.h
 *
 *  Created on: 1 de jul de 2021
 *      Author: leocelente
 */

#ifndef INC_STORAGE_SDCARD_H_
#define INC_STORAGE_SDCARD_H_

#include "fatfs.h"
#include "bsp.h"

typedef struct {
	FIL* file; // ponteiro para file
	char const * filename;
} sdcard_file_t;

/***
 * Mounts SDCard and creates file if it doesn't exist
 */
error_t sdcard_init(sdcard_file_t file);
/***
 * Escreve dados do Buffer View no final do arquivo e retorna se
 * ocorreram erros e a quantidade de bytes escritos
 */
result16_t sdcard_append(sdcard_file_t file, buffer_view_t view);


void sdcard_destroy();
/***
 * Variaveis Globais definidas pelo fatfs.h
 */
extern FATFS SDFatFS; /* File system object for SD logical drive */
extern char SDPath[4]; /* SD logical drive path */


#endif /* INC_STORAGE_SDCARD_H_ */
