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
#define EXPORT static inline
#include <arduino.h>


#define ARDUINO_MAX_DELAY 1000 //Delay padrão para comunicação em arduino
#define TIMEOUT                                                                \
  ARDUINO_MAX_DELAY // Tempo limite de transações da AVR em milisegundos

EXPORT void delay_ms(uint32_t time) { delay(time); }