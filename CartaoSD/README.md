# Cartao SD
## Requisitos
 - Configurar o SDMMc
 - Configurar o FatFS

## Como usar:
Setup, mount
```c
FIL logfile;
sdcard_file_t log = { .file = &logfile, .filename = "MISSAO.TXT" };
if (sdcard_init(log)) {
	puts("Failed to INIT\r");
}
```

Adicionar uma linha ao log, append

```c
uint8_t txt[] = "Status da Missao\r\n";
buffer_view_t line = { .data = txt, .size = sizeof(txt) };
result16_t x = sdcard_append(log, line);
if (x.hasError) {
    puts("Failed to APPEND\r");
}
	
```

Limpeza, unmout

```c
sdcard_destroy();
```

