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
static void set_power(radio_t *radio, uint8_t);
static void radio_register(radio_t *radio);
/**
 * HAL
 */

/**
 * Read register
 */
static uint8_t reg_read(radio_t *radio, uint8_t reg) {

	buffer_view_t addr_v = { .size = 1, .data = &reg };
	uint8_t r = 0;
	buffer_view_t response_v = { .size = 1, .data = &r };

	gpio_low(radio->dev.pin);
	error_t e = spi_transmit(radio->dev, addr_v);
	e |= spi_receive(radio->dev, response_v);
	gpio_high(radio->dev.pin);

	if (e) {
		onError(FAIL_READ);
	}

	uint8_t value = response_v.data[0];
	return value;
}

/**
 * Write to register, except FIFO
 */
static void reg_write(radio_t *radio, uint8_t reg, uint8_t value) {
	if (reg == FIFO) {
		// use dedicated function to write to fifo
		onError(FAIL_WRITE);
	}

	printf("Write register: 0x%.2X to: 0x%.2X...", reg, value);
	uint8_t cmd[] = { reg | WRITE_BIT, value };
	buffer_view_t cmd_v = { .size = 2, .data = cmd };

	gpio_low(radio->dev.pin);
	error_t error = spi_transmit(radio->dev, cmd_v);
	gpio_high(radio->dev.pin);

	if (error) {
		onError(FAIL_WRITE);
	}

#if ENABLE_ASSERT_ON_WRITE // Optional assertion that value was written
	delay_ms(1);
	uint8_t read = reg_read(radio, reg);
	// test write, with the exception of irq flags and sequencer
	if (read != value  //
	&& (reg != LORA_IRQ_FLAGS) && (reg != FSK_REG_SEQ_CONFIG_1)) {
		printf("\r\n \tFailed: expected: 0x%.2X got: 0x%.2X\r\n", value, read);
		onError(12);
	} else
#endif
		printf("OK\r\n");
}
/**
 * Write buffer to FIFO
 */
static void fifo_write(radio_t *radio, buffer_view_t packet) {
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

	gpio_low(radio->dev.pin);
	error_t e = spi_transmit(radio->dev, addr_v);
	e |= spi_transmit(radio->dev, packet);
	gpio_high(radio->dev.pin);

	if (e) {
		onError(FAIL_WRITE);
	}
}

/**
 * Set frequency to possible values
 * [F915MHz,F868MHz,F434MHz,F433MHz]
 */
static void set_frequency(radio_t *radio) {
	reg_write(radio, FREQ_HSB, radio->frequency >> 16);
	reg_write(radio, FREQ_MSB, radio->frequency >> 8);
	reg_write(radio, FREQ_LSB, radio->frequency);
}

/**
 * Reset radio
 */
static void reset(radio_t *radio) {
	gpio_low(radio->reset);
	delay_ms(30);
	gpio_high(radio->reset);
	delay_ms(30);
}

/**
 * Set OpMode register,
 * takes in consideration current radio parameters
 */
static void set_opmode(radio_t *radio, mode_t mode) {
	uint8_t op = reg_read(radio, OPMODE);
	op &= ~0x0F;
	op |= (radio->modulation == MOD_LORA) ? MODEM_LORA : MODEM_FSK;
	op |= mode;
	reg_write(radio, OPMODE, op);
	delay_ms(5);
}

/**
 * Set radio parameters to maximize TX range
 * LoRa: SF12, BW125kHz
 * FSK:
 */
static void maximize_tx(radio_t *radio) {
	set_power(radio, 18); // for experimentation

	// Over Current Protection
	uint8_t ocp_trim = (140 /*mA*/+ 30) / 10;
	uint8_t val = (0x20 | (0x1F & ocp_trim));
	reg_write(radio, OCP, val);

	if (radio->modulation == MOD_LORA) {
		// Spreading Factor
		uint8_t sf = 11 << 4;
		reg_write(radio, MODEM_CONFIG_2, sf);

		// Bandwidth
		uint8_t bw = BW_31_25K;
		uint8_t modem_config_1 = 0x00; // reg_read(*radio, MODEM_CONFIG_1);
		modem_config_1 |= bw;
		// Coding Rate
		modem_config_1 |= 0b0010;
		reg_write(radio, MODEM_CONFIG_1, modem_config_1);
		// Low Data Rate Optimization
		reg_write(radio, 0x26, 0b00001000);

	} else {
		// Bandwidth
		uint8_t bw = reg_read(radio, FSK_REG_RX_BW);
		reg_write(radio, FSK_REG_RX_BW, bw | FSK_BW_200_0_KHZ);

		// AFC (Automatic Frequency Correction ?)
		uint8_t afc_bw = reg_read(radio, FSK_REG_RX_BW);
		reg_write(radio, FSK_REG_RX_BW, afc_bw | FSK_BW_83_3_KHZ);

		// Bitrate
		reg_write(radio, FSK_REG_BITRATE_MSB, FSK_BITRATE_1_2_KBPS >> 8);
		reg_write(radio, FSK_REG_BITRATE_LSB, (uint8_t) FSK_BITRATE_1_2_KBPS);

		// Frequency Deviation
		uint16_t dev = 10000;
		uint16_t frequencyDev = (uint16_t) (dev / 61);
		reg_write(radio, FSK_REG_FDEV_MSB, frequencyDev >> 8);
		reg_write(radio, FSK_REG_FDEV_LSB, frequencyDev);

		// Packet Config
		uint8_t packet_config = 0x00;
		packet_config |= PACKET_CFG_FORMAT_VAR; // variable len
		packet_config |= (0b00 << 5);           // no encoding
		packet_config |= (0b1 << 4);            // crc
		packet_config |= (0b0 << 3);            // auto crc discard
		packet_config |= (0b00 << 1);           // no addr filtering
		reg_write(radio, PACKET_CFG, packet_config);

		uint8_t packet_config_2 = reg_read(radio, PACKET_CFG_2);
		packet_config_2 |= (0b1 << 6); // makes sure it is in packet mode
		reg_write(radio, PACKET_CFG_2, packet_config_2);

	}
}

/**
 * Set Module Output Power
 * @arg power: Power in dBm
 */
static void set_power(radio_t *radio, uint8_t power) {
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
static void set_diomap(radio_t *radio) {
	uint8_t map = 0;
	if (radio->mode == OP_TRANSMIT && radio->modulation == MOD_LORA) {
		map = 0x40;
	} else if (radio->mode == OP_RECEIVE && radio->modulation == MOD_LORA) {
		map = 0x00;
	} else if (radio->mode == OP_TRANSMIT && radio->modulation == MOD_FSK) {
		map = 0x00;
		map |= 0b00110000; // remove o som do buzzer no DIO1
	} else if (radio->mode == OP_RECEIVE && radio->modulation == MOD_FSK) {
		map = 0x40;
	}
	reg_write(radio, DIO_MAPPING_1, map);
}

static void set_rx(radio_t *radio) {
	uint8_t lna = reg_read(radio, FSK_REG_LNA);
	reg_write(radio, FSK_REG_LNA, lna | 0b00000011);
	radio->mode = OP_RECEIVE;

	set_opmode(radio, MODE_RX);
	set_diomap(radio);
}

/**
 * Set syncword to network identification
 */
static void set_syncword(radio_t *radio, uint8_t len, uint8_t value) {
	cap_max(len, 8);
	if (value == 0x00) {
		onError(-10);
	}

	if (radio->modulation == MOD_LORA) {
		reg_write(radio, 0x39, value);
	} else {
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
	// Set radio parameters according to this packet's parameters
	radio->modulation = packet.modulation;
	radio->network = packet.network;
	radio->mode = OP_TRANSMIT;
	init(radio); 		// re-initializes radio using packet params

	set_opmode(radio, MODE_STDBY);
	set_syncword(radio, 8, packet.network);

	uint16_t preamble_len = 16;
	if (radio->modulation == MOD_FSK) {
		//
		// PacketLength can be more than 64 bytes
		reg_write(radio, PACKET_LEN, packet.data.size);
		// Set preamble
		reg_write(radio, FSK_REG_PREAMBLE_LEN_MSB, preamble_len >> 8);
		reg_write(radio, FSK_REG_PREAMBLE_LEN_LSB, preamble_len);
		// FIFO Threshold 1 less than packet size
		//	(read the register description for explanation)
		reg_write(radio, FSK_REG_FIFO_THRESH, packet.data.size - 1);
	} else {
		reg_write(radio, 0x20, preamble_len >> 8);
		reg_write(radio, 0x21, preamble_len);
		// PacketLen (max)
		reg_write(radio, 0x22, packet.data.size);
		uint8_t fifo_tx_addr = reg_read(radio, 0x0E);
		reg_write(radio, 0x0D, fifo_tx_addr);
	}

	int len = packet.data.size % 65;
	buffer_view_t data_window_v = { .size = len, .data = packet.data.data };
	// send those first bytes still in standby mode
	fifo_write(radio, data_window_v);
	// enter transmit mode
	set_opmode(radio, MODE_TX);

	uint8_t bytes_sent = data_window_v.size;
	uint8_t rest = packet.data.size - bytes_sent;

	while (rest > 0) {
		// While NOT FifoEmpty Flag (wait for fifo to clear)
		// TODO: add timeout to avoid whole program freeze
		while (!((reg_read(radio, FIFO_FLAGS) >> 6) & 1)) {
			delay_ms(10);
		}
		// re-construct buffer view pointing to remaining bytes
		data_window_v.data = &packet.data.data[bytes_sent];
		data_window_v.size = rest % 65; // what is left, up to 64
		fifo_write(radio, data_window_v);
		bytes_sent += data_window_v.size;
		rest = packet.data.size - bytes_sent;
	}

	// Pool Interrupt Flag
	// TODO: add timeout to avoid whole program freeze
	printf("Waiting for DIO0 Interrupt\r\n");
	while (radio->flags.TxDone) {
		// we expect packets to take between 500ms and 1000ms
		delay_ms(10);
	}
	dio0_flag = 0;

	// Get out of TX mode
	set_opmode(radio, MODE_STDBY);

	// Clear appropriate flag
	if (radio->modulation == MOD_LORA) {
		reg_write(radio, LORA_IRQ_FLAGS, 0xFF);
	} else {
		reg_read(radio, FIFO_FLAGS);
	}

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
	radio_register(radio);

	// check if module is connected
	uint8_t id = reg_read(radio, CHIP_ID);
	if (id != 0x12) {
		onError(NO_CHIP);
	}

	set_opmode(radio, MODE_SLEEP);

	set_frequency(radio);

	set_opmode(radio, MODE_STDBY);

	if (radio->modulation == MOD_LORA)
		set_syncword(radio, 1, radio->network);
	else
		set_syncword(radio, 8, radio->network);

	set_diomap(radio);
	maximize_tx(radio);

	// in case we are configuring radio for transmission
	if (radio->mode != OP_TRANSMIT) {
		set_rx(radio);
	}
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

void getRunningRadios() {

}

/**
 * Called on internal errors
 */
void onError(radio_error_t error) {
	char *e_name = "   ";
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

//	while (1)
//		;
}

#define MAX_RADIOS  4
radio_t *radios[MAX_RADIOS];

static void radio_register(radio_t *radio) {
	static int counter = 0;
	int index = radio->flags.index;
	if (radios[index] == radio) {
		return;
	}
	radios[counter] = radio;
	radio->flags.index = counter;
	counter++;
}

static void get_packet(radio_t *radio) {
	#error "Not fully implemented"
	// read fifo
	// clear flags
	// write to buffer
}

void handle_interrupt(gpio_pin_t gpio) {
	for (int i = 0; i < MAX_RADIOS; ++i) {
		radio_t *radio = radios[i];
		if (memcmp(&gpio, &radio->dio0, sizeof(gpio)) == 0) {
			if (radio->mode == OP_TRANSMIT) {
				radio->flags.TxDone = true;
			} else if (radio->mode == OP_RECEIVE) {
				radio->flags.RxDone = true;
				get_packet(radio);
			} else {
			}
			return;
		}
	}
}

