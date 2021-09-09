/*
 * MAX31856.c
 *
 * Author: Thiago Henrique Vicentini (Anakin)
 */

#include "MAX31856.h"

error_t max31856_setConversionMode(max31856_t *max, uint8_t enable){
    result8_t t = spi_read8(max->device, MAX31856_CR0_READ);
    if(t.hasError){
        return t.hasError;
    }

    if(enable)
        t.value |= MAX31856_CONFIG_MODEAUTO;
    else
        t.value &= ~MAX31856_CONFIG_MODEAUTO;
    
    error_t error = spi_write8(max->device, MAX31856_CR0_READ, t.value);
    return error;
}

// The Thermocouple Voltage Conversion Averaging Mode settings should not be changed while conversions are taking place.
error_t max31856_setAveragingMode(max31856_t *max, max31856_samples_t samples){
    result8_t t = spi_read8(max->device, MAX31856_CR1_READ);
    if(t.hasError){
        return t.hasError;
    }

    t.value |= (samples << 4); // bit 6:4
    
    error_t error = spi_write8(max->device, MAX31856_CR1_READ, t.value);
    return error;
}

error_t max31856_setThermocoupleType(max31856_t *max, max31856_thermocoupletype_t tc_type){
    result8_t t = spi_read8(max->device, MAX31856_CR1_READ);
    if(t.hasError){
        return t.hasError;
    }

    t.value |= tc_type// bit 3:0;
    
    error_t error = spi_write8(max->device, MAX31856_CR1_READ, t.value);
    return error;
}

error_t max31856_init(max31856_t *max, SPI_HandleTypeDef *spi, gpio_pin_t gpio){
    max->device->spi = spi;
    max->device->pin = gpio;
    gpio_high(device->pin);
    
    error_t error;
    error = max31856_setConversionMode(max, 1); // NOMINAL
    if(error)
        return error;

    error = max31856_setAveragingMode(max, MAX31856_4_SAMPLES);
    if(error)
        return error;

    error = max31856_setThermocoupleType(max, MAX31856_TCTYPE_K);
    if(error)
        return error;

    result8_t fault_register = spi_read8(max->device, MAX31856_CR1_READ);
    if(fault_register.hasError){
        return fault_register.hasError;
    }
    if(fault_register.value != 0x00){
        if( (fault_register.value)&(0x80) == 0x80){
            //The Cold-Junction temperature is outside of the normal operating range.
        }if( (fault_register.value)&(0x40) == 0x40){
            //The Thermocouple Hot Junction temperature is outside of the normal operating range.
        }if( (fault_register.value)&(0x20) == 0x20){
            //The Cold-Junction temperature is higher than the cold-junction temperature high threshold. The FAULT output is asserted unless masked.
        }if( (fault_register.value)&(0x10) == 0x10){
            //The Cold-Junction temperature is lower than the cold-junction temperature low threshold. The FAULT output is asserted unless masked.
        }if( (fault_register.value)&(0x08) == 0x08){
            //The Thermocouple Temperature is higher than the thermocouple temperature high threshold. The FAULT output is asserted unless masked.
        }if( (fault_register.value)&(0x04) == 0x04{
            //Thermocouple temperature is lower than the thermocouple temperature low threshold. The FAULT output is asserted unless masked.
        }if( (fault_register.value)&(0x02) == 0x02){
            //The input voltage is negative or greater than VDD. The FAULT output is asserted unless masked.
        }if( (fault_register.value)&(0x01) == 0x01){
            //An open circuit such as broken thermocouple wires has been detected. The FAULT output is asserted unless masked.
        }
        return 1;
    }
    
    return 0;
}

error_t max31856_readThermocoupleTemp(max31856_t *max, float *temp){
    
    uint8_t buffer[3] = { 0 };
	buffer_view_t view = { .data=buffer, .size=sizeof(buffer) };
	error_t error = spi_readN(max->device, MAX31856_LTCBH_READ, view);
    if(error){
        return error;
    }

	uint32_t temp24 = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
    
    if(temp24 & 0x800000) {
        temp24 |= 0xFF000000; // fix sign
    }

    (*temp) = (temp24 >> 5)*MAX31856_Temperature_Resolution; 

    return 0;
}