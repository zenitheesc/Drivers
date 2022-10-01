/*
 * w25q.c
 *
 *  Created on: Sep 18, 2021
 *      Author: leocelente
 */

#ifndef INC_STORAGE_W25Q_C_
#define INC_STORAGE_W25Q_C_
#include "platform/platform.h"
#include <stdbool.h>
typedef struct {
	/**
	 * Page Count on Chip
	 */
	uint32_t page_count;

	/**
	 * SPI Connection
	 */
	spi_device_t dev;

} w25q_t;

/**
 * Identifies a single byte, inside a page
 */
typedef struct {
	// page address
	uint32_t sector_index;
	uint32_t page_index;
	uint32_t offset_bytes;
} w25q_address_t;

/**
 * Initialization, checks connection and chip model
 * capacity. If `check_capacity` then fails if capacity
 * read from chip is different from expected.
 */
error_t w25q_init(w25q_t *flash, bool check_capacity);

/**
 * Writes up to a page (256 bytes) at some `PageAddress`
 */
error_t w25q_page_write(w25q_t flash, buffer_view_t tx_data,
		w25q_address_t address);

/**
 * Reads up to a page (256 bytes) at some `PageAddress`
 */
error_t w25q_page_read(w25q_t flash, buffer_view_t rx_data,
		w25q_address_t address);

/**
 * Erases all SECTOR_SIZE bytes of some sector
 */
error_t w25q_sector_erase(w25q_t flash, size_t sector_address);

/**
 * Erase all data in the Chip
 */
error_t w25q_chip_erase(w25q_t flash);

/**
 * Models by user-friendly "name"
 */
typedef enum {
	W25Q16, W25Q32, W25Q64, W25Q128
} w25q_model_e;

/**
 * Gets page count based on model
 */
uint32_t page_count_from_model(w25q_model_e model);

/**
 * W25Qxxx fixed sizes
 */
#define W25Q_SECTOR_SIZE 	4096
#define W25Q_PAGE_SIZE 		256

/**
 * Instructions
 */
static const uint8_t W25Q_READ = 0x0B;
static const uint8_t W25Q_WRITE = 0x02;
static const uint8_t W25Q_ERASE = 0x20;
static const uint8_t W25Q_WR_EN = 0x06;
static const uint8_t W25Q_ID = 0x9F;
static const uint8_t W25Q_BUSY_MSK = 0x01;
static const uint8_t W25Q_RD_STATUS1 = 0x05;
static const uint8_t W25Q_WR_STATUS1 = 0x01;
static const uint8_t W25Q_CHIP_ERASE = 0xC7;

#endif /* INC_STORAGE_W25Q_C_ */
