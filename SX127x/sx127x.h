/*
 * sx127x.h
 *
 *  Created on: 30 de out de 2021
 *      Author: leocelente
 */

#ifndef INC_SX127X_H_
#define INC_SX127X_H_

#include "platform/platform.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define _frequency(f) ((uint64_t)f << 19)/ 32000000

typedef enum {
	MOD_FSK, MOD_LORA
} modulation_t;

typedef enum {
	ZENITH = 'Z', OBSAT = 'O'
} network_t;

typedef enum {
	F915MHz = _frequency(915E6),
	F868MHz = _frequency(868E6),
	F434MHz = _frequency(434E6),
	F433MHz = _frequency(433E6)
} frequency_t;

typedef enum {
	OP_TRANSMIT = 0x40, OP_RECEIVE = 0x00
} opmode_t;

typedef struct {
	buffer_view_t data;
	network_t network;
	modulation_t modulation;
} packet_t;

typedef struct {
	volatile bool RxDone;
	volatile bool TxDone;
	int index;
} radio_interrupts_t;

typedef struct {
	modulation_t modulation;
	network_t network;
	frequency_t frequency;
	spi_device_t dev;
	gpio_pin_t dio0;
	gpio_pin_t reset;
	opmode_t mode;
	radio_interrupts_t flags;
} radio_t;


typedef enum {
	OK, FAIL_UNKNOWN, FAIL_READ, FAIL_WRITE, NO_CHIP
} radio_error_t;

error_t init(radio_t* radio);
error_t transmit(radio_t* radio, packet_t packet);
void handle_interrupt(gpio_pin_t gpio);
void onError(radio_error_t error);
void onTxDone(void);

#define MODEM_FSK 0x00
#define MODEM_LORA 0x80
#define WRITE_BIT 0x80
#define CHIP_ID 0x42
#define FIFO 0x00
#define OPMODE 0x01
#define MODE_SLEEP (0b000 << 0)
#define MODE_STDBY (0b001 << 0)
#define MODE_TX (0b011 << 0)
#define MODE_RX (0b101 << 0)
#define MODE_LF (0b1 << 3)
#define FREQ_HSB 0x06
#define FREQ_MSB 0x07
#define FREQ_LSB 0x08
#define FREQ_915M_HSB 0xE4
#define FREQ_915M_MSB 0xC0
#define FREQ_915M_LSB 0x00
#define PACKET_CFG 0x30
#define PACKET_CFG_FORMAT_FIXED (0b1 << 7)
#define PACKET_CFG_FORMAT_VAR (0b0 << 7)
#define PACKET_CFG_CODING_NONE (0b00 << 5)
#define PACKET_CFG_NO_CRC (0b0 << 4)
#define PACKET_CFG_NO_CRC_AUTO_DIS (0b1 << 3)
#define PACKET_CFG_CODING
#define PACKET_CFG_NO_ADDR_FILT (0b00 << 1)
#define PACKET_CFG_CRC_WHITE
#define PACKET_CFG_2 0x31
#define PACKET_LEN 0x32
#define FIFO_FLAGS 0x3F
#define PA_CONFIG 0x09
#define PA_SELECT_BOOST 0x80
#define PA_MAX_POWER (0b111 <<4)
#define PA_DAC 0x4D
#define BOOST_ON 0x87
#define BOOST_OFF 0x84
#define OCP 0x0B
#define MODEM_CONFIG_1 0x1D
#define MODEM_CONFIG_2 0x1E
#define MODEM_CONFIG_3 0x26


#define BW_31_25K 	0x40
#define BW_41_7K 	0x50
#define BW_62_5K 	0x60
#define BW_125K 	0x70
#define BW_250K 	0x80
#define BW_500K 	0x90

#define DIO_MAPPING_1 0x40
#define SYNC_CONFIG 0x27
#define LORA_IRQ_FLAGS 0x12

#define FSK_REG_RX_BW						 0x12	// Rx Bandwidth Configuration - Check page 88

#define FSK_BW_2_6_KHZ						0b00010111
#define FSK_BW_3_1_KHZ						0b00001111
#define FSK_BW_3_9_KHZ						0b00000111
#define FSK_BW_5_2_KHZ						0b00010110
#define FSK_BW_6_3_KHZ						0b00001110
#define FSK_BW_7_8_KHZ						0b00000110
#define FSK_BW_10_4_KHZ						0b00010101
#define FSK_BW_12_5_KHZ						0b00001101
#define FSK_BW_15_6_KHZ						0b00000101
#define FSK_BW_20_8_KHZ						0b00010100
#define FSK_BW_25_0_KHZ						0b00001100
#define FSK_BW_31_3_KHZ						0b00000100
#define FSK_BW_41_7_KHZ						0b00010011
#define FSK_BW_50_0_KHZ						0b00001011
#define FSK_BW_62_5_KHZ						0b00000011
#define FSK_BW_83_3_KHZ						0b00010010
#define FSK_BW_100_0_KHZ					0b00001010
#define FSK_BW_125_0_KHZ					0b00000010
#define FSK_BW_166_7_KHZ					0b00010001
#define FSK_BW_200_0_KHZ					0b00001001
#define FSK_BW_150_0_KHZ					0b00000001

#define FSK_REG_BITRATE_MSB		             0x02U
#define FSK_REG_BITRATE_LSB     	         0x03U

#define FSK_BITRATE_1_2_KBPS 			(uint16_t)0x682B
#define FSK_BITRATE_2_4_KBPS			(uint16_t)0x3415
#define FSK_BITRATE_4_8_KBPS			(uint16_t)0x1A0B
#define FSK_BITRATE_9_6_KBPS			(uint16_t)0x0D05
#define FSK_BITRATE_12_5_KBPS			(uint16_t)0x0A00
#define FSK_BITRATE_19_2_KBPS			(uint16_t)0x0683
#define FSK_BITRATE_25_KBPS 			(uint16_t)0x0500
#define FSK_BITRATE_38_4_KBPS 			(uint16_t)0x0341
#define FSK_BITRATE_50_KBPS 			(uint16_t)0x8000
#define FSK_BITRATE_57_6_KBPS 			(uint16_t)0x020C
#define FSK_BITRATE_76_8_KBPS  			(uint16_t)0x01A1
#define FSK_BITRATE_100_KBPS 			(uint16_t)0x0140
#define FSK_BITRATE_150_KBPS 			(uint16_t)0x00D5
#define FSK_BITRATE_153_6_KBPS 			(uint16_t)0x00D0
#define FSK_BITRATE_200_KBPS 			(uint16_t)0x00A0
#define FSK_BITRATE_250_KBPS 			(uint16_t)0x0080
#define FSK_BITRATE_300_KBPS 			(uint16_t)0x006B

#define FSK_REG_FDEV_MSB					 0x04
#define FSK_REG_FDEV_LSB					 0x05

#define FSK_REG_LNA	 					     0x0C	// Low-Noise Amplifier Configuration


#define FSK_REG_SEQ_CONFIG_1                 0x36U
#define FSK_REG_SEQ_CONFIG_2                 0x37U
#define FSK_REG_TIMER_RESOL                  0x38U
#define FSK_REG_TIMER_1_COEF                 0x39U
#define FSK_REG_TIMER_2_COEF                 0x3AU


#define FSK_REG_IRQ_FLAGS1                   0x3EU
#define FSK_REG_IRQ_FLAGS2                   0x3FU
#define FSK_REG_FIFO_THRESH                  0x35U


#define FSK_REG_PREAMBLE_LEN_MSB			 0x25	// Preamble Length Configuration MSB
#define FSK_REG_PREAMBLE_LEN_LSB			 0x26 	// Preamble Length Configuration LSB

#endif /* INC_SX127X_H_ */
