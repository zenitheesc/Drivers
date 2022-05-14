#ifndef PLATFORM_H
#define PLATFORM_H
#include <stddef.h>
#include <stdint.h>

/* COMMON INTERFACE */

/**
 * Define nossa enumeração de erros
 * Isso define um padrão: 0 quando não há erros,
 * por isso na struct "result_XX_t" veja o uso comum na struct
 * "result_XX_t"
 */
typedef uint_fast8_t error_t;

/**
 * Agrupa em uma struct o ponteiro e tamanho de um buffer já existente
 * então o buffer deve ser criado antes e depois a buffer_view deve ser
 * construida:
 * 		uint8_t buffer[16] = { 0 };
 * 		buffer_view_t b_view = {
 * 			.data = &buffer,
 * 			.size = 16 // melhor ainda, usar sizeof(buffer)
 * 		}
 */
typedef struct {
  uint8_t *data;
  size_t size;
} buffer_view_t;

/**
 * Agrupa o valor de uma operação e se teve algum erro
 * a semantica é de:
 * 	"Resultado tem erros implica result.hasError != 0"
 * então na hora de usar fica:
 *
 *	 	resultado = operação();
 * 		if(resultado.hasError){
 * 			cuida dos erros
 * 		}
 */
#define MAKE_RESULT(type)                                                      \
  typedef struct {                                                             \
    error_t hasError;                                                          \
    type value;                                                                \
  } result_##type

MAKE_RESULT(uint8_t);
MAKE_RESULT(uint16_t);
MAKE_RESULT(float);
MAKE_RESULT(int);

/* SELECT IMPLEMENTATION */

#if ESP_PLATFORM // Workaround to name conflict with ESP-IDF HAL
#define i2c_t zen_i2c_t
#define spi_t zen_spi_t
#define uart_t zen_uart_t
#define adc_t zen_adc_t
#endif

#if ARDUINO
#include "arch/platform_arduino.h"
#elif ESP_PLATFORM
#include "arch/platform_esp32.h"
#elif __x86_64
#include "arch/platform_debug.h"
#else
#include "arch/stm32x/platform_stm32.h"
#endif

// #include "arch/platform_debug.h"

#endif
