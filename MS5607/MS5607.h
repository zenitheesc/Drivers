//
// Created by gusta on 14/11/2021.
//
// * I2C *

#ifndef MS5607_H
#define MS5607_H

#include "platform/platform.h"
#include <stdint.h>

enum OSR_samples {mode_1, mode_2, mode_3, mode_4, mode_5};

// OSR_Mode: (Same for D1 and D2)
//            mode_1 = 256 Samples
//            mode_2 = 512 Samples
//            mode_3 = 1024 Samples
//            mode_4 = 2048 Samples
//            mode_5 = 4096 Samples


typedef struct {
    uint16_t C1; // Pressure sensitivity
    uint16_t C2; // Pressure offset
    uint16_t C3; // Temperature coef. of pressure sens.
    uint16_t C4; // Temperature coef. of pressure offset
    uint16_t C5; // Reference temperature
    uint16_t C6; // Temperature coef. of the temperature

} prom_t;

typedef struct {
    uint32_t D1; // Digital pressure value
    uint32_t D2; // Digital temperature value

} DValue_t;

typedef struct {
    i2c_device_t device;
    prom_t prom;
    DValue_t DValue;
    uint8_t OSR_mode; // Quantity of samples for conversion (ADC)
    
} ms5607_t;


error_t ms5607_init(ms5607_t* ms5607, enum OSR_samples mode);
int32_t ms5607_getPressure(ms5607_t* ms5607);
int32_t ms5607_getTemperature(ms5607_t* ms5607);


#define QNT_PROM_CONST ((uint8_t) (6))
#define QNT_DVALUE     ((uint8_t) (2))

//    REGISTER_NAME       ADDRESS(BIN)
#define I2C_ADR        ((uint8_t) (0b11101100))
#define I2S_ADR_ALT	   ((uint8_t) (0b11101111))


//    REGISTER_NAME       COMMAND(BIN)
#define PROM_MASK      ((uint8_t) (0b10100000))   // Bits 1, 2 e 3 represents the coef.

#define D1_MASK        ((uint8_t) (0b01000000))   // Bits 1, 2 e 3 represents the OSR_mode
#define D2_MASK        ((uint8_t) (0b01010000))

#define RESET_CMD      ((uint8_t) (0b00011110))


#endif //MS5607_H
