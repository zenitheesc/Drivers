/**
 * @file platform_debug.h
 * @author Leonardo Celente (@leocelente)
 *
 * @version 0.1
 * @date 2022-01-18
 *
 * @copyright Copyright (c) 2022
 *
 * Implementação de plataforma de testes. Inclui toda interface
 * especificada, mas não se comunica com hardware, apenas mostra
 * uma mensagem expondo qual seria o efeito da comunicação.
 * O principal objetivo é permitir que desenvolvedores dos
 * drivers possam compilar os projetos (geralmente no inicio)
 * como qualquer projeto C, sem necessitar de IDEs.
 *
 */

#ifndef PLATFORM_DEBUG_H
#define PLATFORM_DEBUG_H
#include "../platform.h"

#include <stdint.h>
#define EXPORT static inline

#include <stdarg.h>
#include <stdio.h>

#define CURRENT_MODULE "DELAY"

#define LOG(fmt, ...)                                                          \
  printf("[%s] - " fmt "\r\n", CURRENT_MODULE, ##__VA_ARGS__)

#define BUFFER(name, bv)                                                       \
  printf("Buffer: " name);                                                     \
  for (int i = 0; i < bv.size; ++i) {                                          \
    if (i % 7 == 0)                                                            \
      printf("\r\n");                                                          \
    printf("  %.2X", bv.data[i]);                                              \
  }                                                                            \
  printf("  --\r\n")

EXPORT void delay_ms(uint32_t time) {
  LOG("Waiting for %d milliseconds", time);
}

#undef CURRENT_MODULE
#define CURRENT_MODULE "I2C"

/**
 * Define o tipo da interface i2c como o da HAL
 */
typedef int i2c_t;
/* Substitui a interface de hardware */
static i2c_t fake_i2c = 1;

/**
 * Agrupa a interface i2c e o endereço do escravo
 * i2c: é um PONTEIRO para interface
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
  LOG("Transmitting via I2C%d to Device: %.2X", *device.i2c, device.address);
  BUFFER("Bytes", buffer);
  return 0;
}
/*
 * Acesso direto, lê o que estiver no barramento do I2C, sem enviar
 * endereço de registrador
 */
EXPORT error_t i2c_receive(i2c_device_t device, buffer_view_t buffer) {
  LOG("Receiving (blocking) via I2C%d to Device: %.2X", *device.i2c,
      device.address);
  BUFFER("Bytes", buffer);
  return 0;
}

#undef CURRENT_MODULE
#define CURRENT_MODULE "GPIO"

typedef int gpio_port_t;
typedef struct {
  gpio_port_t *port;
  uint16_t pin;
} gpio_pin_t;

EXPORT void gpio_low(gpio_pin_t pin) {
  LOG("Putting Pin: %d on Port: %d to LOW", pin.pin, *pin.port);
}

EXPORT void gpio_high(gpio_pin_t pin) {
  LOG("Putting Pin: %d on Port: %d to HIGH", pin.pin, *pin.port);
}

EXPORT void gpio_toggle(gpio_pin_t pin) {
  LOG("Toggling Pin: %d on Port: %d", pin.pin, *pin.port);
}

#undef CURRENT_MODULE
#define CURRENT_MODULE "SPI"

typedef int spi_t;
static spi_t fake_spi = 1;

typedef struct {
  spi_t *spi;
  gpio_pin_t pin;
} spi_device_t;

EXPORT error_t spi_transceive(spi_device_t device, buffer_view_t rx_buffer,
                              buffer_view_t tx_buffer) {

  if (tx_buffer.size != rx_buffer.size) {
    return 1;
  }
  gpio_low(device.pin);
  LOG("Transmiting and Receiving");
  BUFFER("RX", rx_buffer);
  BUFFER("TX", tx_buffer);
  gpio_high(device.pin);
  return 0;
}

#undef CURRENT_MODULE
#define CURRENT_MODULE "UART"

typedef int uart_t;
static uart_t fake_uart = 1;

typedef struct {
  uart_t *uart;
} uart_connection_t;

EXPORT error_t uart_transmit(uart_connection_t conn, buffer_view_t buffer) {
  LOG("Transmiting via UART%d: ", *conn.uart);
  BUFFER("Bytes", buffer);
  return 0;
}

EXPORT error_t uart_receive(uart_connection_t conn, buffer_view_t buffer) {
  LOG("Receiving (blocking) via UART%d: ", *conn.uart);
  BUFFER("Bytes", buffer);
  return 0;
}

#undef CURRENT_MODULE
#define CURRENT_MODULE "ADC"

typedef int adc_handle_t;
typedef struct {
  adc_handle_t *handle;
  uint8_t bits;
  float voltage_reference;
} adc_t;

static int fake_adc_handle = 1;
static adc_t fake_adc = {
    .handle = &fake_adc_handle, .bits = 12, .voltage_reference = 3.3};
static uint16_t _adc_value = 1023;

EXPORT error_t adc_init(adc_t *adc) {
  LOG("Starting the ADC%d", *adc);
  return 0;
}

EXPORT result_uint16_t adc_read(adc_t *adc) {
  LOG("Converting with ADC%d to Value: %d", *adc, _adc_value);
  result_uint16_t out = {.hasError = 0, .value = _adc_value};
  return out;
}

EXPORT float adc_raw_to_voltage(adc_t adc, uint16_t value) {
  const float volts_per_step =
      (adc.voltage_reference - 0.f) / ((1 << adc.bits) - 1);
  return volts_per_step * value;
}

/* INC_BSP_H_ */

#undef CURRENT_MODULE
#undef LOG
#endif
