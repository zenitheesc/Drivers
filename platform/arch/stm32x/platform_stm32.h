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
 *
 *  	Cada arquivo platform_xxxx.h estaria então relacionado com uma
 * plataforma
 *
 *  	Essa é do STM32 com HAL*, poderia ter uma para Arduino
 * (platform_arduino.h) que usaria as Wire e SPI. Poderia ter uma linux
 * embarcado (platform_linux.h) que envolveria os /dev/i2c0 /dev/spi0 etc e a
 * biblioteca não iria precisar de mudanças para ser utilizada em qualquer uma
 * dessas plataformas
 *
 *  	* somente em blocking mode, ainda tenho que pensar em abstrações
 * eficientes para interrupts e DMA
 */

#ifndef INC_PLATFORM_STM32_H_
#define INC_PLATFORM_STM32_H_
#include <stdint.h>
#define EXPORT static inline

#include "select_hal.h"
#define TIMEOUT                                                                \
  HAL_MAX_DELAY // Tempo limite de transações da HAL em milisegundos

EXPORT void delay_ms(uint32_t time) { HAL_Delay(time); }

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

EXPORT error_t spi_transceive(spi_device_t device, buffer_view_t rx_buffer,
                              buffer_view_t tx_buffer) {

  if (tx_buffer.size != rx_buffer.size) {
    return 1;
  }
  error_t error = HAL_SPI_TransmitReceive(
      device.spi, tx_buffer.data, rx_buffer.data, tx_buffer.size, TIMEOUT);
  return error;
}

EXPORT error_t spi_receive(spi_device_t device, buffer_view_t buffer_view) {
  error_t error =
      HAL_SPI_Receive(device.spi, buffer_view.data, buffer_view.size, TIMEOUT);
  return error;
}

EXPORT error_t spi_transmit(spi_device_t device, buffer_view_t buffer_view) {
  error_t error =
      HAL_SPI_Transmit(device.spi, buffer_view.data, buffer_view.size, TIMEOUT);
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
typedef ADC_HandleTypeDef adc_handle_t;
typedef struct {
  adc_handle_t *handle;
  uint8_t bits;
  float voltage_reference;
} adc_t;

EXPORT error_t adc_init(adc_t *adc) {
  HAL_ADC_Start(adc->handle);
  // Na CubeIDE está com um falso-positivo de erro sobre o ErrorCode, mas
  // compila sem errros
  return adc->handle->ErrorCode;
}

EXPORT result_uint16_t adc_read(adc_t *adc) {
  result_uint16_t out = {.hasError = 1, .value = 0xFF};
  out.value = HAL_ADC_GetValue(adc->handle);
  out.hasError = adc->handle->ErrorCode;
  return out;
}

EXPORT float adc_raw_to_voltage(adc_t adc, uint16_t value) {
  const float volts_per_step =
      (adc.voltage_reference - 0.f) / ((1 << adc.bits) - 1);
  return volts_per_step * value;
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

#endif /* INC_PLATFORM_STM32_H_ */
