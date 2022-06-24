/*
 * BNO055.c
 *
 *  Created on: Jun 10, 2022
 *      Author: Murilo Henrique Pasini Trevisan
*/

#include "BNO055.h"

/* Read and Write register functions*/

//Read Registers
static result_uint8_t read(BNO055_t const *bno, uint8_t addr){
        //value to return
        result_uint8_t result = { 0 };
        //addres send
        buffer_view_t addr_tr = { .data = &addr, .size = sizeof(addr)};
        result.HasError |= i2c_transmit(bno->dev, addr_tr);
        //value send
        uint8_t rx = 0;
        buffer_view_t read_tr = {.data = &rx, .size = sizeof(rx)};
        result.HasError |= i2c_receive(bno->dev,read_tr);
        result.value = rx;
        return result;
}

//Write registers
static error_t write(BNO055_t const *bno, uint8_t addr, uint16_t value){
        //shifting value to 8 bit i2c
        uint8_t data[] = {addr, value};
        buffer_view_t tx = {.data = data, .size = sizeof(data)};
        return i2c_transmit(bno->dev, tx);
}


