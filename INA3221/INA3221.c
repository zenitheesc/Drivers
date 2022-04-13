//
// Created by gusta on 05/09/2021.
//

#include "INA3221.h"

static result_uint16_t read(ina3221_t ina, uint8_t addr) {
  uint8_t tx = addr;
  buffer_view_t tx_v = {.size = sizeof(tx), .data = &tx};
  error_t e = i2c_transmit(ina.device, tx);

  uint8_t rx[2] = {0};
  buffer_view_t rx_v = {.size = sizeof(rx), .data = rx};
  e |= i2c_receive(ina.device, rx);

  uint16_t value = (rx[1] << 8) | (rx[0]);
  result_uint16_t res = {.hasError = e, .value = value};
  return res;
}

static error_t write(ina3221_t ina, uint8_t addr, uint16_t value) {
  uint8_t tx[] = {addr, value << 8, value};
  buffer_view_t view = {.size = sizeof(tx), .data = tx};
  return i2c_transmit(ina.device, tx);
}

static int16_t to_value(uint16_t raw_value) {

	raw_value >>= 3;
	if (raw_value & (1 << (15 - 3))) {
		raw_value |= 0xE000;
	}
	return (int16_t) raw_value;
}

error_t ina3221_init(ina3221_t ina) {
    if(!ina3221_alive(ina)){
    	return 1;
    }

	uint8_t address_config = CONFIG_ADR;
	uint16_t bit_config = 0x7127 | RESET_CONFIG;

    result16_t id = i2c_read16(ina.device, CONFIG_ADR);

    delay_ms(1);

    if (id.hasError) {
        return id.hasError;
    }

    delay_ms(10);

	return 0;
    return i2c_write16(ina.device, address_config, bit_config);
}

error_t ina3221_config(ina3221_t ina) {

    uint16_t config_register = 0;
    uint8_t address_config = CONFIG_ADR;

    config_register |= (ina.config.ch1 << BIT_CH1);
    config_register |= (ina.config.ch2 << BIT_CH2);
    config_register |= (ina.config.ch3 << BIT_CH3);
    config_register |= (ina.config.avg_mode << BIT_AVG);
    config_register |= (ina.config.v_bus_ct << BIT_BUS_CT);
    config_register |= (ina.config.v_sh_ct << BIT_SH_CT);
    config_register |= (ina.config.op_mode << BIT_MODE);

    return 0;
    return i2c_write16(ina.device, address_config, config_register);;
}

error_t ina3221_mensurement(ina3221_t ina, ina3221_values_t *values){
    float final_value;

    // Shunt Voltage values

    result16_t raw = i2c_read16(ina.device, CH1_SHUNT_V);
    if (raw.hasError) {
        return raw.hasError;
    }

	final_value = T_SH_CONV * to_value(raw.value);

    values->ch1_sh_v = final_value;


    raw = i2c_read16(ina.device, CH2_SHUNT_V);
    if (raw.hasError) {
        return raw.hasError;
    }

	final_value = T_SH_CONV * to_value(raw.value);

    values->ch2_sh_v = final_value;


    raw = i2c_read16(ina.device, CH3_SHUNT_V);
    if (raw.hasError) {
        return raw.hasError;
    }

	final_value = T_SH_CONV * to_value(raw.value);

    values->ch3_sh_v = final_value;


    // Bus Voltage Values

    raw = i2c_read16(ina.device, CH1_BUS_V);
    if (raw.hasError) {
        return raw.hasError;
    }
	final_value = T_BUS_CONV * to_value(raw.value);

    values->ch1_bus_v = final_value;


    raw = i2c_read16(ina.device, CH2_BUS_V);
    if (raw.hasError) {
        return raw.hasError;
    }
	final_value = T_BUS_CONV * to_value(raw.value);

    values->ch2_bus_v = final_value;


    raw = i2c_read16(ina.device, CH3_BUS_V);
    if (raw.hasError) {
        return raw.hasError;
    }
	final_value = T_BUS_CONV * to_value(raw.value);

    values->ch3_bus_v = final_value;

    // Converted Current values   U(shunt) = R * i

	float CH1_current = (values->ch1_sh_v) / (ina.config.ch1_resistor);
	float CH2_current = (values->ch2_sh_v) / (ina.config.ch2_resistor);
	float CH3_current = (values->ch3_sh_v) / (ina.config.ch3_resistor);

    values->ch1_current = CH1_current;
    values->ch2_current = CH2_current;
    values->ch3_current = CH3_current;

	// Power values   (potência do bus - potência do shunt)

	float CH1_pot = ((values->ch1_bus_v) * (values->ch1_current))
			- ((values->ch1_sh_v) * (values->ch1_current));
	float CH2_pot = ((values->ch2_bus_v) * (values->ch2_current))
			- ((values->ch2_sh_v) * (values->ch2_current));
	float CH3_pot = ((values->ch3_bus_v) * (values->ch3_current))
			- ((values->ch3_sh_v) * (values->ch3_current));

    values->ch1_pot = CH1_pot;
    values->ch2_pot = CH2_pot;
    values->ch3_pot = CH3_pot;

	return 0;
}

bool ina3221_alive(ina3221_t ina) {
    result16_t code = i2c_read16(ina.device, DIE_ADR);
    if (code.value != DIE_CODE){
        return false;
    }
    return true;
}
