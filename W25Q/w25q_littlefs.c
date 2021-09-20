/*
 * w25q_littlefs.h
 *
 *  Created on: Sep 19, 2021
 *      Author: leocelente
 */
#include "Storage/w25q_littlefs.h"

int block_device_read(const struct lfs_config *c, lfs_block_t block,
		lfs_off_t off, void *buffer, lfs_size_t size) {

	uint32_t pageInBlock = off / W25Q_PAGE_SIZE;
	uint32_t offsetInPage = off % W25Q_PAGE_SIZE;
	uint32_t pageAddress = block * W25Q_SECTOR_SIZE
			+ (pageInBlock * W25Q_PAGE_SIZE);

	w25q_address_t address = { .page = pageAddress, .offset = offsetInPage };
	buffer_view_t view = { .data = buffer, .size = size };
	w25q_page_read(_littlefs_flash, view, address);
	return 0;
}

int block_device_prog(const struct lfs_config *c, lfs_block_t block,
		lfs_off_t off, const void *buffer, lfs_size_t size) {

	uint32_t pageInBlock = off / W25Q_PAGE_SIZE;
	uint32_t offsetInPage = off % W25Q_PAGE_SIZE;
	uint32_t pageAddress = block * W25Q_SECTOR_SIZE
			+ (pageInBlock * W25Q_PAGE_SIZE);

	w25q_address_t address = { .page = pageAddress, .offset = offsetInPage };
	buffer_view_t view = { .data = (uint8_t* const ) buffer, .size = size };
	w25q_page_write(_littlefs_flash, view, address);
	return 0;
}

int block_device_erase(const struct lfs_config *c, lfs_block_t block) {
	w25q_sector_erase(_littlefs_flash, block);
	return 0;
}

int block_device_sync(const struct lfs_config *c) {
	return 0;
}

struct lfs_config get_littlefs_config(w25q_t flash) {
	const uint32_t block_count = (flash.page_count * W25Q_PAGE_SIZE)
			/ W25Q_SECTOR_SIZE;
	struct lfs_config cfg = { //
			.read = block_device_read, 	//
					.prog = block_device_prog, 	//
					.erase = block_device_erase, 	//
					.sync = block_device_sync, 	//
					.read_size = W25Q_PAGE_SIZE, 			//
					.prog_size = W25Q_PAGE_SIZE, 			//
					.block_size = W25Q_SECTOR_SIZE, 		//
					.block_count = block_count, 		//
					.lookahead_size = W25Q_PAGE_SIZE / 2,   	//
					.cache_size = W25Q_PAGE_SIZE,     		//
					.block_cycles = 1000,		//
			};

	return cfg;
}

error_t littlefs_init(w25q_t flash) {
	_littlefs_flash = flash;
	cfg = get_littlefs_config(flash);
	int err = lfs_mount(&file_system, &cfg);
	if (err) {
		w25q_chip_erase(flash);
		lfs_format(&file_system, &cfg);
		lfs_mount(&file_system, &cfg);
	}
	return 0;
}

