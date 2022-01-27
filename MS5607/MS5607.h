//
// Created by gusta on 14/11/2021.
//
// * Em I2C *

#ifndef MS5607_H
#define MS5607_H

#include "bsp.h"
#include <stdint.h>

//    REGISTER_NAME       ADDRESS(HEX)
#define I2C_ADR        ((uint8_t) (111011Cx))

//    REGISTER_NAME       COMMAND(BIN)
#define PROM_MASK      ((uint8_t) (10100000))   // os bits 1, 2 e 3 representam o coef.
                                                // que o PROM retornará ex. 1010 001 0 retornará o C1

#define D1_MASK        ((uint8_t) (01000000))   //os bits 1, 2 e 3 representam o OSR_mode
#define D2_MASK        ((uint8_t) (01010000))

#define RESET_CMD      ((uint8_t) (00011110))

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

} Dconst_t;

typedef struct {
    i2c_device_t device;
    prom_t prom;
    Dconst_t Dconst;
    uint8_t OSR_mode; // Quantity of samples for conversion (ADC)
    
} ms5607_t;




#endif //MS5607_H
