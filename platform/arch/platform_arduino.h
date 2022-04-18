/*
 *      Created on: april, 15, 2022
 *      Author: murilotrevisan
 *
 *  Description:
 *  	O intuito desse arquivo é separar as funções da biblioteca de abstração do arduino
 *  	da lógica da biblioteca, então abstrai transações
 *  	I2C e SPI.
 *
 *  	Se a biblioteca usa diretamente a biblioteca de abstração do arduino, o código
 *  	vai precisar se mudado para usar em outro microcontrolador,
 *  	sistema (eg BeagleBone) ou mesmo uma alternativa a biblioteca de abstração do arduino.
 *
 *  	Cada arquivo platform_xxxx.h estaria então relacionado com uma
 * plataforma
 *
 *  	Essa é do Arduino com biblioteca de abstração do arduino. Poderia ter uma linux
 * embarcado (platform_linux.h) que envolveria os /dev/i2c0 /dev/spi0 etc e a
 * biblioteca não iria precisar de mudanças para ser utilizada em qualquer uma
 * dessas plataformas
 *
 *  	* somente em blocking mode, ainda tenho que pensar em abstrações
 * eficientes para interrupts e DMA
 */

#ifndef INC_PLATFORM_ARDUINO_H_
#define INC_PLATFORM_ARDUINO_H_
#include <stdint.h>
#include <arduino.h>
#define EXPORT static inline



#define ARDUINO_MAX_DELAY 1000000 //Delay padrão em us para comunicação em arduino
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
//#ifdef HAL_I2C_MODULE_ENABLED //Macro para detectar uso da wire

/**
 * Agrupa a interface i2c e o endereço do escravo
 * address: Endereço NÃO shiftado do escravo
 * i2c_device_t = {
 * 		.address = 0x40
 * }
 */
typedef struct {
  uint8_t address;
} i2c_device_t;
//#include <Wire.h>

/**
 * Inicializa a biblioteca wire e acessa o bus 12c como controlador
 * Configura o timeout das funções da wire
 * Em caso de timeout, efetua reset do hardware do i2c
 * O timeout deve ser configurado testando se há possibilidade de configurá-lo na board a ser utilizada,
 * A macro WIRE_HAS_TIMEOUT testa se a versao do wire possui configuração de timeout
 */
Wire.begin();
#if defined(WIRE_HAS_TIMEOUT)
    Wire.setWireTimeout(TIMEOUT, true);
#endif

/***
 * Acesso direto, transmite no barramento do I2C, sem enviar
 * endereço de registrador
 */
EXPORT error_t i2c_transmit(i2c_device_t device, buffer_view_t buffer) {
  Wire.beginTransmission(device.address);
  transmit = Wire.write(buffer.data, buffer.size);
  Wire.endTransmission();
  return Transmit;
}
/***
 * Acesso direto, lê o que estiver no barramento do I2C, sem enviar
 * endereço de registrador
 */
EXPORT error_t i2c_receive(i2c_device_t device, buffer_view_t buffer) {
  Wire.requestFrom(device.address, buffer.size);
  buffer_position = 0;
  while(Wire.available() && n < buffer.size){
      buffer.data[buffer_position] = Wire.read();
      buffer_position++;
  }
  return buffer;
}

//#endif

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
//#ifdef HAL_GPIO_MODULE_ENABLED //macro para detectar uso da gpio

/**
 * Agrupa a interface gpio e o pino
 * Usado para portabilidade com outras platform
 * 
 */
typedef struct {
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
//#endif

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
//#if defined(HAL_SPI_MODULE_ENABLED)


typedef struct {
  gpio_pin_t pin;
} spi_device_t;
//#include <SPI.h>

/**
 * Inicializa a biblioteca SPI 
 * Configura os pinos de SCK, MOSI e SS como outputs
 * Pulling SCK e MOSI como low e SS como high
 * 
 */
SPI.begin();
#if defined(WIRE_HAS_TIMEOUT)
    Wire.setWireTimeout(TIMEOUT, true);
#endif

EXPORT error_t spi_transceive(spi_device_t device, buffer_view_t rx_buffer,
                              buffer_view_t tx_buffer) {

  if (tx_buffer.size != rx_buffer.size) {
    return 1;
  }
  rx_buffer.data = SPI.transfer(tx_buffer.data); //receive rx and transmit tx
  error_t error = rx_buffer;
  return error;
}

EXPORT error_t spi_receive(spi_device_t device, buffer_view_t buffer_view) {
  buffer_view.data = SPI.transfer(0x00); //envia um 0 para receber os dados
  error_t error = buffer_view;
  return error;
}

EXPORT error_t spi_transmit(spi_device_t device, buffer_view_t buffer_view) {
  error_t error = SPI.transfer(buffer_view.data);
  return error;
}

//#endif