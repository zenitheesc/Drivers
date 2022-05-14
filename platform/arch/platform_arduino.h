/*
 *      Created on: april, 15, 2022
 *      Author: murilotrevisan
 *
 *  Description:
 *  	O intuito desse arquivo é separar as funções da biblioteca de abstração
 * do arduino da lógica da biblioteca, então abstrai transações I2C e SPI.
 *
 *  	Se a biblioteca usa diretamente a biblioteca de abstração do arduino, o
 * código vai precisar se mudado para usar em outro microcontrolador, sistema
 * (eg BeagleBone) ou mesmo uma alternativa a biblioteca de abstração do
 * arduino.
 *
 *  	Cada arquivo platform_xxxx.h estaria então relacionado com uma
 * plataforma
 *
 *  	Essa é do Arduino com biblioteca de abstração do arduino. Poderia ter
 * uma linux embarcado (platform_linux.h) que envolveria os /dev/i2c0 /dev/spi0
 * etc e a biblioteca não iria precisar de mudanças para ser utilizada em
 * qualquer uma dessas plataformas
 *
 *  	* somente em blocking mode, ainda tenho que pensar em abstrações
 * eficientes para interrupts e DMA
 */

#ifndef INC_PLATFORM_ARDUINO_H_
#define INC_PLATFORM_ARDUINO_H_
#include <Arduino.h>
#include <stdint.h>
#define EXPORT static inline

#define ARDUINO_MAX_DELAY                                                      \
  1000000 // Delay padrão em us para comunicação em arduino
#define TIMEOUT                                                                \
  ARDUINO_MAX_DELAY // Tempo limite de transações da arduino em microsegundos

EXPORT void delay_ms(uint32_t time) { delay(time); }

/**
 * Essa macro só é definida se o I2C for ser utilizado
 *
 *
 *
 *
 *
 *
 *
 *
 */
#ifdef TwoWire_h // Macro para detectar uso da wire

/**
 *
 * Define o tipo da interface i2c
 * Utilizado para compatibilidade com outras platform
 *
 */
typedef int i2c_t;
static i2c_t fake_i2c = 1;

/**
 * Agrupa a interface i2c e o endereço do escravo
 * address: Endereço NÃO shiftado do escravo
 * i2c_device_t = {
 * 		.address = 0x40
 * }
 */
typedef struct {
  i2c_t *i2c;
  uint8_t address;
} i2c_device_t;
//#include <Wire.h>

/**
 * Inicializa a biblioteca wire e acessa o bus 12c como controlador
 * Configura o timeout das funções da wire
 * Em caso de timeout, efetua reset do hardware do i2c
 * O timeout deve ser configurado testando se há possibilidade de configurá-lo
 * na board a ser utilizada, A macro WIRE_HAS_TIMEOUT testa se a versao do wire
 * possui configuração de timeout
 */
// Wire.begin();
//#if defined(WIRE_HAS_TIMEOUT)
//     Wire.setWireTimeout(TIMEOUT, true);
//#endif

/***
 * Acesso direto, transmite no barramento do I2C, sem enviar
 * endereço de registrador
 * DAR SHIFT NO ENDEREÇO!!!!
 */
EXPORT error_t i2c_transmit(i2c_device_t device, buffer_view_t buffer) {
  uint8_t transmit = 0;
  Wire.beginTransmission(device.address << 1);
  transmit = Wire.write(buffer.data, buffer.size); // ver quantos bytes ele
                                                   // envia
  Wire.endTransmission(); // VER ESSE PROBLEMA DE USAR MAIS BYTES
  return transmit;
}
/***
 * Acesso direto, lê o que estiver no barramento do I2C, sem enviar
 * endereço de registrador
 */
EXPORT error_t i2c_receive(i2c_device_t device, buffer_view_t buffer) {
  uint8_t i = 0;
  Wire.requestFrom(device.address, buffer.size);
  for (int i = 0; Wire.available() && i < buffer.size; ++i) {
    buffer.data[i] = Wire.read();
  }
  return (i == buffer.size);
}

#endif

/**
 * Essa macro só é definida se o GPIO for ser utilizado
 *
 *
 *
 *
 *
 *
 *
 *
 */
#ifdef Arduino_h

/**
 *
 * Define o tipo da interface GPIO
 * Utilizado para compatibilidade com outras platform
 *
 */
typedef int GPIO_TypeDef;
static GPIO_TypeDef fake_GPIO = 1;

/**
 * Agrupa a interface gpio e o pino
 * Usado para portabilidade com outras platform
 *
 */
typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
} gpio_pin_t;

EXPORT void gpio_low(gpio_pin_t pin) {
  pinMode(pin.pin, OUTPUT);
  digitalWrite(pin.pin, LOW);
}

EXPORT void gpio_high(gpio_pin_t pin) {
  pinMode(pin.pin, OUTPUT);
  digitalWrite(pin.pin, HIGH);
}

EXPORT void gpio_toggle(gpio_pin_t pin) {
  pinMode(pin.pin, OUTPUT);
  digitalWrite(pin.pin, !digitalRead(pin.pin));
}
#endif

/**
 * Essa macro só é definida se o SPI for ser utilizado
 *
 *
 *
 *
 *
 *
 *
 */
#ifdef _SPI_H_INCLUDED

/**
 *
 * Define o tipo da interface spi
 * Utilizado para compatibilidade com outras platform
 *
 */
typedef int spi_t;
static spi_t fake_spi = 1;

typedef struct {
  spi_t *spi;
  gpio_pin_t pin;
} spi_device_t;
//#include <SPI.h>

/**
 * Inicializa a biblioteca SPI
 * Configura os pinos de SCK, MOSI e SS como outputs
 * Pulling SCK e MOSI como low e SS como high
 *
 */
// SPI.begin();

EXPORT error_t spi_transceive(spi_device_t device, buffer_view_t rx_buffer,
                              buffer_view_t tx_buffer) {

  if (tx_buffer.size != rx_buffer.size) {
    return 1;
  }
  rx_buffer.data = SPI.transfer(tx_buffer.data); // receive rx and transmit tx
  error_t error = rx_buffer;
  return error;
}

EXPORT error_t spi_receive(spi_device_t device, buffer_view_t buffer_view) {
  buffer_view.data = SPI.transfer(0x00); // envia um 0 para receber os dados
  error_t error = buffer_view;
  return error;
}

EXPORT error_t spi_transmit(spi_device_t device, buffer_view_t buffer_view) {
  error_t error = SPI.transfer(buffer_view.data);
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

#ifdef SoftwareSerial_h

/**
 *
 * Define o tipo da interface uart
 * Utilizado para compatibilidade com outras platform
 *
 */
typedef int uart_t;
static uart_t fake_UART = 1;

/**
 * Inicializa a biblioteca serial e acessa como controlador
 * Configura o timeout das funções da serial
 * Em caso de timeout, efetua reset do hardware da serial
 *
 */
// serial.begin();
// serial.setTimeout(TIMEOUT);
typedef struct {
  uart_t *uart;
} uart_connection_t;

EXPORT error_t uart_writeN(uart_connection_t conn, buffer_view_t buffer) {
  return serial.write(buffer.data, buffer.size);
}

EXPORT error_t uart_readN(uart_connection_t conn, buffer_view_t buffer) {
  return serial.readBytes(buffer.data, buffer.size);
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
#ifdef Arduino_h

typedef int adc_handle_t;
static adc_handle_t fake_ADC = 1;

typedef struct {
  adc_handle_t *handle;
  uint8_t bits; // o número de bits é fixo no arduino, 10 para a maioria, 12 em
                // alguns
  float voltage_reference; // a referência de tensão no arduino depende do
                           // modelo utilizado
} adc_t;

EXPORT result_uint16_t adc_read(adc_t *adc) {
  result_uint16_t out = {.hasError = 1, .value = 0xFF};
  out.value = analogRead(*adc->handle);
  out.hasError = 0;
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

#endif /* INC_PLATFORM_ARDUINO_H_ */
