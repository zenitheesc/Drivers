//
// Created by gusta on 05/09/2021.
//

#include "INA3221.h"


error_t ina3221_init(ina3221_t ina) {
    if(!ina3221_alive(ina)){
    	return 1;
    }

    uint8_t address_config = CONFIG_ADR;
    uint16_t bit_config = RESET_CONFIG;

    result16_t id = i2c_read16(ina.device, CONFIG_ADR);

    delay_ms(1);

    if (id.hasError) {
        return id.hasError;
    }

    delay_ms(10);

    return i2c_write16(ina.device, address_config, bit_config);
}

error_t ina3221_config(ina3221_t ina) {

    uint16_t config_register = 0;
    uint8_t address_config = CONFIG_ADR;

    config_register |= (ina.config.ch1        <<  BIT_CH1);
    config_register |= (ina.config.ch2        <<  BIT_CH2);
    config_register |= (ina.config.ch3        <<  BIT_CH3);
    config_register |= (ina.config.avg_mode   <<  BIT_AVG);
    config_register |= (ina.config.v_bus_ct   <<  BIT_BUS_CT);
    config_register |= (ina.config.v_sh_ct    <<  BIT_SH_CT);
    config_register |= (ina.config.op_mode    <<  OP_MODE);

    return i2c_write16(ina.device, address_config, config_register);
}

error_t ina3221_mensurement(ina3221_t ina, ina3221_values_t *values){
    float final_value;

    // Shunt Voltage values

    result16_t raw = i2c_read16(ina.device, CH1_SHUNT_V);
    if (raw.hasError) {
        return raw.hasError;
    }

    final_value = T_SH_CONV * ((int16_t) raw.value);

    values->ch1_sh_v = final_value;


    raw = i2c_read16(ina.device, CH2_SHUNT_V);
    if (raw.hasError) {
        return raw.hasError;
    }

    final_value = T_SH_CONV * ((int16_t) raw.value);

    values->ch2_sh_v = final_value;


    raw = i2c_read16(ina.device, CH3_SHUNT_V);
    if (raw.hasError) {
        return raw.hasError;
    }

    final_value = T_SH_CONV * ((int16_t) raw.value);

    values->ch3_sh_v = final_value;


    // Bus Voltage Values

    raw = i2c_read16(ina.device, CH1_BUS_V);
    if (raw.hasError) {
        return raw.hasError;
    }
    final_value = T_BUS_CONV * ((int16_t) raw.value);

    values->ch1_bus_v = final_value;


    raw = i2c_read16(ina.device, CH2_BUS_V);
    if (raw.hasError) {
        return raw.hasError;
    }
    final_value = T_BUS_CONV * ((int16_t) raw.value);

    values->ch2_bus_v = final_value;


    raw = i2c_read16(ina.device, CH3_BUS_V);
    if (raw.hasError) {
        return raw.hasError;
    }
    final_value = T_BUS_CONV * ((int16_t) raw.value);

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
