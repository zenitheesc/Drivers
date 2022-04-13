/*
 * sx127x.c
 *
 *  Created on: 30 de out de 2021
 *      Author: leocelente
 */

#include "sx127x.h"
#define ENABLE_ASSERT_ON_WRITE 1
#define ENABLE_PRINTF_ERROR 1

#define cap_max(x, L) x = ((x) > (L)) ? (L) : (x)
#define cap_min(x, L) x = ((x) < (L)) ? (L) : (x)

volatile uint8_t dio0_flag = 0;

static void maximize_tx(radio_t *radio);
static void set_power(radio_t radio, uint8_t);


/**
 * HAL
 */

/**
 * Read register
 */
static uint8_t reg_read(radio_t radio, uint8_t reg) {

	buffer_view_t addr_v = {.size=1, .data=&reg};
	uint8_t r = 0;
	buffer_view_t response_v = {.size=1, .data=&r};

	gpio_low(radio.dev.pin);
	error_t e = spi_transmit(radio.dev, addr_v);
	e |= spi_receive(radio.dev, response_v);
	gpio_high(radio.dev.pin);

	if (e) {
		onError(FAIL_READ);
	}

	uint8_t value = response_v.data[0];
	return value;
}

/**
 * Write to register, except FIFO
 */
static void reg_write(radio_t radio, uint8_t reg, uint8_t value) {
	if(reg == FIFO){
		// use dedicated function to write to fifo
		onError(FAIL_WRITE);
	}

	printf("Write register: 0x%.2X to: 0x%.2X...", reg, value);
	uint8_t cmd[] = { reg | WRITE_BIT, value };
	buffer_view_t cmd_v = {
			.size = 2,
			.data = cmd };

	gpio_low(radio.dev.pin);
	error_t error = spi_transmit(radio.dev, cmd_v);
	gpio_high(radio.dev.pin);

	if (error) {
		onError(FAIL_WRITE);
	}

#if ENABLE_ASSERT_ON_WRITE // Optional assertion that value was written
	delay_ms(1);
	uint8_t read =  reg_read(radio, reg);
	// test write, with the exception of irq flags
	if (read != value && (reg != LORA_IRQ_FLAGS)) {
		onError(12);
	}
#endif

	printf("OK\r\n");
}
/**
 * Write buffer to FIFO
 */
static void fifo_write(radio_t radio, buffer_view_t packet) {
	printf("Writing to FIFO:\r\n[ ");
	for (int i = 0; i < packet.size; ++i) {
		printf(" 0x%.2X", packet.data[i]);
		if (i % 15 == 0) {
			printf("\r\n");
		}
	}
	printf(" ]\r\n");

	uint8_t addr = FIFO | WRITE_BIT;
	buffer_view_t addr_v = { .size = 1, .data = &addr };

	gpio_low(radio.dev.pin);
	error_t e = spi_transmit(radio.dev, addr_v);
	e |= spi_transmit(radio.dev, packet);
	gpio_high(radio.dev.pin);

	if (e) {
		onError(FAIL_WRITE);
	}
}

/**
 * Set frequency to possible values
 * [F915MHz,F868MHz,F434MHz,F433MHz]
 */
static void set_frequency(radio_t radio) {
	uint8_t const high = radio.frequency >> 16;
	uint8_t const mid = radio.frequency >> 8;
	uint8_t const low = (uint8_t) radio.frequency;

	switch (radio.frequency) {
	case F433MHz:
	case F434MHz:
	case F868MHz:
	default:
	case F915MHz:
		reg_write(radio, FREQ_HSB, high);
		reg_write(radio, FREQ_MSB, mid);
		reg_write(radio, FREQ_LSB, low);
		break;
	}
}
/**
 * Reset radio
 */
static error_t reset(radio_t *radio) {
	gpio_low(radio->reset);
	delay_ms(30);
	gpio_high(radio->reset);
	delay_ms(30);
	return OK;
}

/**
 * Set OpMode register,
 * takes in consideration current radio parameters
 */
static void set_opmode(radio_t radio, mode_t mode) {
	uint8_t op = reg_read(radio, OPMODE);
	op &= ~0x0F;
	op |= (radio.modulation == MOD_LORA) ? MODEM_LORA : MODEM_FSK;
	op |= mode;
	reg_write(radio, OPMODE, op);
}


/**
 * Set radio parameters to maximize TX range
 * LoRa: SF12, BW125kHz
 * FSK:
 */
static void maximize_tx(radio_t *radio) {
	set_power(*radio, 17); // for experimentation

	uint8_t ocp_trim = (140 /*mA*/+ 30) / 10;
	uint8_t val = (0x20 | (0x1F & ocp_trim));
	reg_write(*radio, OCP, val);

	if (radio->modulation == MOD_LORA) {
		// Spreading Factor
		uint8_t sf = 11 << 4;
		reg_write(*radio, MODEM_CONFIG_2, sf);

		// Bandwidth
		uint8_t bw = BW_125K;
		uint8_t modem_config_1 = reg_read(*radio, MODEM_CONFIG_1);
		modem_config_1 &= 0x0F;
		modem_config_1 |= bw;
		reg_write(*radio, MODEM_CONFIG_1, modem_config_1);
	} else {
		// Bandwidth
		uint8_t bw = reg_read(*radio, FSK_REG_RX_BW);
		reg_write(*radio, FSK_REG_RX_BW, bw | FSK_BW_200_0_KHZ);

		// AFC (Automatic Frequency Correction ?)
		uint8_t afc_bw = reg_read(*radio, FSK_REG_RX_BW);
		reg_write(*radio, FSK_REG_RX_BW, afc_bw | FSK_BW_83_3_KHZ);


		// Bitrate
		reg_write(*radio, FSK_REG_BITRATE_MSB, FSK_BITRATE_1_2_KBPS >> 8);
		reg_write(*radio, FSK_REG_BITRATE_LSB, (uint8_t) FSK_BITRATE_1_2_KBPS);

		// Frequency Deviation
		uint16_t dev = 25000;
		uint16_t frequencyDev = (uint16_t) (dev / 61);
		reg_write(*radio, FSK_REG_FDEV_MSB, frequencyDev >> 8);
		reg_write(*radio, FSK_REG_FDEV_LSB, frequencyDev);

		// Packet Config
		uint8_t packet_config = 0x00;
		packet_config |= PACKET_CFG_FORMAT_VAR; // variable len
		packet_config |= (0b00 << 5);           // no encoding
		packet_config |= (0b1 << 4);            // crc
		packet_config |= (0b0 << 3);            // auto crc discard
		packet_config |= (0b00 << 1);           // no addr filtering
		reg_write(*radio, PACKET_CFG, packet_config);

		uint8_t packet_config_2 = reg_read(*radio, PACKET_CFG_2);
		packet_config_2 |= 0x40; // makes sure it is in packet mode
		reg_write(*radio, PACKET_CFG_2, packet_config_2);

	}
}

/**
 * Set Module Output Power
 * @arg power: Power in dBm
 */
static void set_power(radio_t radio, uint8_t power) {
	/**
	 * Hardcoded Values, based on other libraries
	 * Datasheet PowerOutput function DOESN'T work
	 * Pout = 17 - (15 - x)
	 */
	reg_write(radio, PA_DAC, 0x84);
	reg_write(radio, PA_CONFIG, 0x91);
	reg_write(radio, OCP, 0x2B);
}

/**
 * Configure IO pins (DIO) as interrupts sources
 * DIO0, as TxDone (aka PacketSent)
 */
static void set_diomap(radio_t radio) {
	uint8_t map = 0;
	if (radio.mode == OP_TRANSMIT && radio.modulation == MOD_LORA) {
		map = 0x40;
	} else if (radio.mode == OP_RECEIVE && radio.modulation == MOD_LORA) {
		map = 0x00;
	} else if (radio.mode == OP_TRANSMIT && radio.modulation == MOD_FSK) {
		map = 0x00;
	} else if (radio.mode == OP_RECEIVE && radio.modulation == MOD_FSK) {
		map = 0x40;
	}
	reg_write(radio, DIO_MAPPING_1, map);
}

static void set_rx(radio_t *radio) {
	// Enable LNA
	uint8_t lna = reg_read(*radio, FSK_REG_LNA);
	reg_write(*radio, FSK_REG_LNA, lna | 0b00000011);

	radio->mode = OP_RECEIVE;
	set_opmode(*radio, MODE_RX);
	set_diomap(*radio);
}

/**
 * Set syncword to network identification
 */
static void set_syncword(radio_t radio, uint8_t len, uint8_t value) {
	cap_max(len, 8);
	if (value == 0x00) {
		onError(-10);
	}
	if (radio.modulation == MOD_LORA) {
		reg_write(radio, 0x39, value);
	} else {
//		uint8_t sync_config = reg_read(radio, SYNC_CONFIG);
//		sync_config &= 0b11110100;	//
		uint8_t sync_config = 0x00;
		sync_config |= 0b10 << 6; 		// auto restart after received packet
		sync_config |= 0b0 << 5; 		// preamble polarity default
		sync_config |= 0b1 << 4; 		// sync word on
		sync_config |= 0b0 << 3; 		// Fifo Fill Condition (?) keep default
		sync_config |= (len - 1); 		// 1 + N sync word size

		reg_write(radio, SYNC_CONFIG, sync_config);
		while (len--) {
			reg_write(radio, 0x2F - len, value);
		}
	}
}

/**
 * Transmit packet
 */
error_t transmit(radio_t *radio, packet_t packet) {
	radio->modulation = packet.modulation;
	radio->network = packet.network;

	init(radio);// re-initializes radio using packet params



	fifo_write(*radio, packet.data);

	set_opmode(*radio, MODE_TX);

	// Pool Interrupt Flag
	// TODO: timeout
	while (!dio0_flag) {
		// we expect packets to take between 500ms and 1000ms
		delay_ms(10);
	}
	dio0_flag = 0;

	set_opmode(*radio, MODE_STDBY);
	if (radio->modulation == MOD_LORA) {
		reg_write(*radio, LORA_IRQ_FLAGS, 0xFF);
	}

	delay_ms(1);

	reg_read(*radio, FIFO_FLAGS);

	// half duplex behavior
	set_rx(radio);
	return OK;
}

/**
 * Initialize radio with parameters,
 */
error_t init(radio_t *radio) {

	gpio_high(radio->dev.pin);
	gpio_high(radio->reset);

	reset(radio);

	// check if module is connected
	uint8_t const id = reg_read(*radio, CHIP_ID);
	if (id != 0x12) {
		onError(NO_CHIP);
	}

	set_opmode(*radio, MODE_SLEEP);
	delay_ms(10);

	set_frequency(*radio);


	if(radio->modulation == MOD_LORA)
		set_syncword(*radio, 1, radio->network);
	else
		set_syncword(*radio, 8, radio->network);


	maximize_tx(radio);

	set_diomap(*radio);

	set_opmode(*radio, MODE_STDBY);

	delay_ms(10);

	set_rx(radio);

	return OK;
}


/**
 * User Hooks
 */

/**
 * Called on
 */
void onRxDone(buffer_view_t view) {
	printf("Received Packet (%d) \r\n", view.size);
}

/**
 * Called on PacketSent Interrupt
 */
void onTxDone(void) {
	printf("OnDone\r\n");
}

/**
 * Called on internal errors
 */
void onError(radio_error_t error) {
	char *e_name = "";
	switch (error) {
	case NO_CHIP:
		e_name = "NO_CHIP";
		break;
	case FAIL_UNKNOWN:
		e_name = "UNKNOWN";
		break;
	default:
		sprintf(e_name, "E%d", error);
		break;
	}
#if ENABLE_PRINTF_ERROR
	printf("RADIO ERROR: %s\r\n", e_name);
#endif
	while (1)
		;
}
