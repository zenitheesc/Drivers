# W25Q FLASH Driver
## Propósito

Driver da familia memórias flash W25Qxxx, compatível com os modelos:
 - W25Q16 
 - W25Q32
 - W25Q64
 - W25Q128

O driver fornece apenas a interface mínima para usar o sistema de arquivos
[LittleFS](https://github.com/littlefs-project/littlefs). Então permite apenas
a `read/write` de páginas de 256 bytes e `erase` de setores de 4kB.

## LittleFS

Para usar o LittleFS inclua o `w25q_littlefs.h`. Segue o exemplo de um 
contador de boots:

```c
	puts("Start-----\r");
	gpio_pin_t flash_cs = { .pin = FLASH_CS_Pin, .port = FLASH_CS_GPIO_Port };
	spi_device_t spi_dev_flash = { .spi = &hspi1, .pin = flash_cs };
	w25q_t flash = { .page_count = page_count_from_model(W25Q16), .dev =
			spi_dev_flash };

	w25q_init(&flash, true);

	littlefs_init(flash);

	struct lfs_file file;

	uint32_t boot_count = 0;
	int e = lfs_file_open(&file_system, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
	printf("Open Boot Count (%d)\r\n", e);
	e = lfs_file_read(&file_system, &file, &boot_count, sizeof(boot_count));
	printf("Read Boot Count (%d), Value: %lu\r\n", e, boot_count);
	boot_count += 1;
	e = lfs_file_rewind(&file_system, &file);
	printf("Rewind (%d)\r\n", e);
	e = lfs_file_write(&file_system, &file, &boot_count, sizeof(boot_count));
	printf("Write Boot Count (%d), Value: %lu\r\n", e, boot_count);

	e = lfs_file_close(&file_system, &file);
	printf("Close... %d\r\n", e);

	e = lfs_unmount(&file_system);
	printf("Unmount (%d)\r\n", e);
	printf("boot_count: %lu\r\n", boot_count);
```

