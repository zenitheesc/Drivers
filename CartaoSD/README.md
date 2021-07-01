# Cartao SD
## Requisitos
 - Configurar o SDMMc
 - Configurar o FatFS

## Como usar:
A sequencia de funções é:
 - f_mount
 - f_open
 - f_write
 - f_close

 ```c
 	FIL logfile;

	sdcard_file_t log = { .file = &logfile, .filename = "MISSAO.TXT" };

	if (sdcard_init(log)) {
		puts("Failed to INIT\r");
	}
	uint8_t txt[] = "Status da Missao\r\n";
	buffer_view_t line = { .data = txt, .size = sizeof(txt) };
	result16_t x = sdcard_append(log, line);
	if (x.hasError) {
		puts("Failed to APPEND\r");
	}
	x = sdcard_append(log, line);
	if (x.hasError) {
		puts("Failed to APPEND\r");
	}
	sdcard_destroy();
	puts("done -- \r");
 ```

