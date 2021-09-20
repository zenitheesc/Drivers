/*
 * w25q_littlefs.h
 *
 *  Created on: Sep 19, 2021
 *      Author: leocelente
 */

#ifndef INC_STORAGE_W25Q_LITTLEFS_H_
#define INC_STORAGE_W25Q_LITTLEFS_H_

#include "Storage/w25q.h"
#include "Storage/littlefs/lfs.h"

/**
 * W25Q Flash chip in use by littlefs
 * gets set in `littlefs_init`
 */
w25q_t _littlefs_flash;

/**
 * Global littlefs state
 */
lfs_t file_system;

/**
 * Configuration
 * Note: References to it must be valid even after `lfs_mount`
 * is done, thats why it is a global in this case.
 */
struct lfs_config cfg;
/**
 * Initialize and configure littlefs for use with
 * some FLASH
 */
error_t littlefs_init(w25q_t flash);

/**
 * Wrapper that converts littlefs "blocks" to page
 * addresses before calling driver's `page_read`
 */
int block_device_read(const struct lfs_config *c, lfs_block_t block,
		lfs_off_t off, void *buffer, lfs_size_t size);
/**
 * Wrapper that converts littlefs "blocks" to page
 * addresses before calling driver's `page_write`
 */

int block_device_prog(const struct lfs_config *c, lfs_block_t block,
		lfs_off_t off, const void *buffer, lfs_size_t size);
/**
 * Wrapper that converts littlefs "blocks" to sector
 * addresses before calling driver's `sector_erase`
 */
int block_device_erase(const struct lfs_config *c, lfs_block_t block);
/**
 * Does nothing, littlefs uses it check for device errors
 */
int block_device_sync(const struct lfs_config *c);

/**
 * Returns a W25Qxxx compatible configuration
 */
struct lfs_config get_littlefs_config(w25q_t flash) ;

#endif /* INC_STORAGE_W25Q_LITTLEFS_H_ */
