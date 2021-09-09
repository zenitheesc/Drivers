//
// Created by gustavo on 05/09/2021.
//


#ifndef OBJ_C_INA3221_H
#define OBJ_C_INA3221_H

#include "bsp.h"
#include <stdint.h>

//      REGISTER_NAME     ADDRESS(HEX)
#define CONFIG_ADR            0x00
#define CH1_SHUNT_V           0x01
#define CH1_BUS_V             0x02
#define CH2_SHUNT_V           0x03
#define CH2_BUS_V             0x04
#define CH3_SHUNT_V           0x05
#define CH3_BUS_V             0x06
#define DIE_ADR                0xFF


// CONST

#define BIT_CH1              14
#define BIT_CH2              13
#define BIT_CH3              12
#define BIT_AVG              11
#define BIT_BUS_CT           8
#define BIT_SH_CT            5
#define OP_MODE              2
#define DIE_CODE             0x3220
#define RESET_CONFIG        (1<<15)

#define T_SH_CONV             40e-6f
#define T_BUS_CONV            8e-3f

typedef struct {

    bool ch1;
    bool ch2;
    bool ch3;
    uint8_t avg_mode;
    uint8_t v_bus_ct;
    uint8_t v_sh_ct;
    uint8_t op_mode;
    uint8_t manuf_id;

} ina3221_config_t;


typedef struct {

    float ch1_sh_v; // sh_v = R * (i)
    float ch1_bus_v;
    float ch2_sh_v;
    float ch2_bus_v;
    float ch3_sh_v;
    float ch3_bus_v;

    float ch1_current;
    float ch2_current;
    float ch3_current;

    uint16_t ch1_resistor;
    uint16_t ch2_resistor;
    uint16_t ch3_resistor;

    float ch1_pot;
    float ch2_pot;
    float ch3_pot;

} ina3221_values_t;


typedef struct {

    i2c_device_t device;
    ina3221_config_t config;

} ina3221_t;

error_t ina3221_init(ina3221_t ina);
error_t ina3221_config(ina3221_t ina);
error_t ina3221_mensurement(ina3221_t ina, ina3221_values_t *values);
error_t ina3221_alive(ina3221_t ina);



#endif //OBJ_C_INA3221_H
