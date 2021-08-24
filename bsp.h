/*
 * bsp_stm32.h
 *
 *  Created on: May 7, 2021
 *      Author: leocelente
 *
 *  Description:
 *  	O intuito desse arquivo é separar as funções da HAL
 *  	da lógica da biblioteca, então abstrai transações
 *  	I2C e SPI.
 *
 *  	Se a biblioteca usa diretamente a HAL, o código
 *  	vai precisar se mudado para usar em outro microcontrolador,
 *  	sistema (eg BeagleBone) ou mesmo uma alternativa a HAL (eg: LL).
 *  	E a HAL tem muito código repetitivo para funções simples,
 *  	usar essas funções deixa o código mais limpo e evita erros bobos.
 *
 *  	Cada arquivo bsp_xxxx.h estaria então relacionado com uma plataforma
 *  	BSP é sigla para Board Support Package ("Pacode de Suporte para Placa")
 *
 *  	Essa é do STM32 com HAL*, poderia ter uma para Arduino (bsp_arduino.h) que usaria
 *  	as Wire e SPI. Poderia ter uma linux embarcado (bsp_linux.h) que envolveria
 *  	os /dev/i2c0 /dev/spi0 etc e a biblioteca não iria precisar de mudanças para
 *  	ser utilizada em qualquer uma dessas plataformas
 *
 *  	* somente em blocking mode, ainda tenho que pensar em abstrações eficientes
 * 		para interrupts e dma
 */

#ifndef INC_BSP_H_
#define INC_BSP_H_
#include <stdint.h>
#define EXPORT static inline

#include "stm32f1xx_hal.h"
#define TIMEOUT HAL_MAX_DELAY // Tempo limite de transações da HAL em milisegundos

/**
 * Define nossa enumeração de erros como a mesma da HAL
 * Isso define um padrão: 0 quando não há erros,
 * por isso na struct "resultXX_t" veja o uso comum na struct
 * "resultXX_t"
 */
typedef HAL_StatusTypeDef error_t;

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
	int size;
} buffer_view_t;

/**
 * Agrupa o valor de uma operação de 16 bits e se teve algum erro
 * a semantica é de:
 * 	"Resultado tem erros implica result.hasError != 0"
 * então na hora de usar fica:
 *
 *	 	resultado = operação();
 * 		if(resultado.hasError){
 * 			cuida dos erros
 * 		}
 */
typedef struct {
	error_t hasError;
	uint16_t value;
} result16_t;

/**
 * Agrupa o valor de uma operação de 16 bits e se teve algum erro
 * mais detalhes na result16_t
 */
typedef struct {
	error_t hasError;
	uint8_t value;
} result8_t;

EXPORT void delay_ms(uint32_t time) {
	HAL_Delay(time);
}

/**
 * Essa macro só é definida se o I2C for configurado na CubeIDE
 *
 *
 *
 *
 *
 *
 *
 *
 */
#ifdef HAL_I2C_MODULE_ENABLED

/**
 * Define o tipo da interface i2c como o da HAL
 */
typedef I2C_HandleTypeDef i2c_t;

/**
 * Agrupa a interface i2c e o endereço do escravo
 * i2c: é um PONTEIRO para o da HAL
 * address: Endereço NÃO shiftado do escravo
 * i2c_device_t = {
 * 		.i2c = &hi2c1,
 * 		.address = 0x40
 * }
 */
typedef struct {
	i2c_t *i2c;
	uint8_t address;
} i2c_device_t;

/***
 * Acesso direto, transmite no barramento do I2C, sem enviar
 * endereço de registrador
 */
EXPORT error_t i2c_transmit(i2c_device_t device, buffer_view_t buffer) {
	return HAL_I2C_Master_Transmit(device.i2c, (device.address << 1), buffer.data,
			buffer.size, TIMEOUT);
}
/***
 * Acesso direto, lê o que estiver no barramento do I2C, sem enviar
 * endereço de registrador
 */
EXPORT error_t i2c_receive(i2c_device_t device, buffer_view_t buffer) {
	return HAL_I2C_Master_Receive(device.i2c, (device.address << 1), buffer.data,
			buffer.size, TIMEOUT);
}


/**
 * Le N bytes do escravo i2c, onde N é o size do buffer_view que é passado.
 * Os valores lidos serão escritos no buffer apontado pelo buffer_view
 */
EXPORT error_t i2c_readN(i2c_device_t device, uint8_t register_address,
		buffer_view_t buffer_view) {
	return HAL_I2C_Mem_Read(device.i2c, (device.address << 1), register_address,
	I2C_MEMADD_SIZE_8BIT, buffer_view.data, buffer_view.size, TIMEOUT);
}
/**
 * Le um byte do escravo i2c
 */
EXPORT result8_t i2c_read8(i2c_device_t device, uint8_t register_address) {

	uint8_t buffer[1] = { 0 };
	buffer_view_t view = { .data = buffer, .size = 1 };
	error_t error = i2c_readN(device, register_address, view);
	uint8_t value = buffer[0];
	result8_t result = { .hasError = error, .value = value };
	return result;
}
/**
 * Le dois bytes do escravo i2c, retorna como u16
 */
EXPORT result16_t i2c_read16(i2c_device_t device, uint8_t register_address) {

	uint8_t buffer[2] = { 0 };
	buffer_view_t view = { .data = buffer, .size = sizeof(buffer) };
	error_t error = i2c_readN(device, register_address, view);
	uint16_t value = (buffer[0] << 8) | buffer[1];
	result16_t result = { .hasError = error, .value = value };
	return result;
}

EXPORT error_t i2c_writeN(i2c_device_t device, uint8_t register_address,
		buffer_view_t buffer_view) {

	return HAL_I2C_Mem_Write(device.i2c, (device.address << 1),
			register_address, I2C_MEMADD_SIZE_8BIT, buffer_view.data,
			buffer_view.size,
			TIMEOUT);
}

EXPORT error_t i2c_write8(i2c_device_t device, uint8_t register_address,
		uint8_t value) {
	uint8_t buffer[1] = { value };
	buffer_view_t view = { .data = buffer, .size = 1 };
	return i2c_writeN(device, register_address, view);
}

EXPORT error_t i2c_write16(i2c_device_t device, uint8_t register_address,
		uint16_t value) {

	uint8_t buffer[2] = { 0 };
	buffer[0] = (uint8_t) (value & 0xFF00) >> 8;
	buffer[1] = (uint8_t) value;
	buffer_view_t view = { .data = buffer, .size = sizeof(buffer) };
	return i2c_writeN(device, register_address, view);
}

#endif

#ifdef HAL_GPIO_MODULE_ENABLED

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} gpio_pin_t;

EXPORT void gpio_low(gpio_pin_t pin) {
	HAL_GPIO_WritePin(pin.port, pin.pin, GPIO_PIN_RESET);
}

EXPORT void gpio_high(gpio_pin_t pin) {
	HAL_GPIO_WritePin(pin.port, pin.pin, GPIO_PIN_SET);
}

EXPORT void gpio_toggle(gpio_pin_t pin) {
	HAL_GPIO_TogglePin(pin.port, pin.pin);
}
#endif

/**
 * Essa macro só é definida se o SPI for configurado na CubeIDE
 *
 *
 *
 *
 *
 *
 *
 */
#ifdef HAL_SPI_MODULE_ENABLED
typedef SPI_HandleTypeDef spi_t;

typedef struct {
	spi_t *spi;
	gpio_pin_t pin;
} spi_device_t;

EXPORT error_t spi_readN(spi_device_t device, uint8_t register_address,
		buffer_view_t buffer_view) {

	gpio_low(device.pin);
	error_t error = HAL_SPI_Receive(device.spi, buffer_view.data,
			buffer_view.size, TIMEOUT);
	gpio_high(device.pin);
	return error;
}

EXPORT result8_t spi_read8(spi_device_t device, uint8_t register_address) {

	uint8_t buffer[1] = { 0 };
	buffer_view_t view = { .data=buffer, .size=1};
	error_t error = spi_readN(device, register_address, view);
	uint8_t value = buffer[0];
	result8_t result = { .hasError = error, .value = value };
	return result;
}

EXPORT result16_t spi_read16(spi_device_t device, uint8_t register_address) {

	uint8_t buffer[2] = { 0 };
	buffer_view_t view = { .data=buffer, .size=sizeof(buffer)};
	error_t error = spi_readN(device, register_address, view);
	uint16_t value = (buffer[0] << 8) | buffer[1];
	result16_t result = { .hasError = error, .value = value };
	return result;
}

EXPORT error_t spi_writeN(spi_device_t device, uint8_t register_address,
		buffer_view_t buffer_view) {
	gpio_low(device.pin);
	error_t error = HAL_SPI_Receive(device.spi, buffer_view.data,
			buffer_view.size,
			TIMEOUT);
	gpio_high(device.pin);
	return error;
}

EXPORT error_t spi_write8(spi_device_t device, uint8_t register_address,
		uint8_t value) {
	uint8_t buffer[1] = { value };
	buffer_view_t view = {.data=buffer, .size = 1};
	return spi_writeN(device, register_address, view);
}
EXPORT error_t spi_write16(spi_device_t device, uint8_t register_address,
		uint16_t value) {
	uint8_t buffer[2] = { 0 };
	buffer[0] = (value & 0xFF00) >> 8;
	buffer[1] = (uint8_t) value;
	buffer_view_t view = {.data=buffer, .size = sizeof(buffer)};
	return spi_writeN(device, register_address, view);
}


// TODO: Implement transceive
EXPORT error_t spi_transceive(spi_device_t device, uint8_t register_address,
		buffer_view_t rx_buffer, buffer_view_t tx_buffer) {

	if (tx_buffer.size != rx_buffer.size) {
		return 1;
	}
	gpio_low(device.pin);
	error_t error = HAL_SPI_TransmitReceive(device.spi, tx_buffer.data,
			rx_buffer.data, tx_buffer.size, TIMEOUT);
	gpio_high(device.pin);
	return error;
}
#endif

/***
 * MODULO UART
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#ifdef HAL_UART_MODULE_ENABLED

typedef UART_HandleTypeDef uart_t;

typedef struct {
	uart_t *uart;
} uart_connection_t;

EXPORT error_t uart_writeN(uart_connection_t conn, buffer_view_t buffer) {
	return HAL_UART_Transmit(conn.uart, buffer.data, buffer.size, HAL_MAX_DELAY);
}

EXPORT error_t uart_readN(uart_connection_t conn, buffer_view_t buffer) {
	return HAL_UART_Receive(conn.uart, buffer.data, buffer.size, 1000);
}

EXPORT result8_t uart_read8(uart_connection_t conn) {
	uint8_t buffer[1] = { 0 };
	buffer_view_t view = { .data = buffer, .size = 1 };
	result8_t result = { .hasError = 1, .value = 0 };
	result.hasError = uart_readN(conn, view);
	result.value = view.data[0];
	return result;
}

#endif

/***
 * MODULO ADC
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
#ifdef HAL_ADC_MODULE_ENABLED
typedef ADC_HandleTypeDef adc_t;
EXPORT error_t adc_init(adc_t* adc){
	HAL_ADC_Start(adc);
	// Na CubeIDE está com um falso-positivo de erro sobre o ErrorCode, mas compila sem errros
	return adc->ErrorCode;
}

EXPORT result16_t adc_read(adc_t* adc){
	result16_t out = {.hasError = 1, .value = 0xFF};
	out.value = HAL_ADC_GetValue(adc);
	out.hasError = adc->ErrorCode; // ignore: erro
	return out;
}

#endif

/***
 * MODULO PWM
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#endif /* INC_BSP_H_ */
