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

/* Configurações do sensor */

//Seleção da unidade de medida
error_t BNO055_unit (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t unit_raw = read(bno, BNO_UNIT_SEL);
        unit_reg = unit_raw.value;

        //Adição da configuração selecionada
        unit_reg |= bno.config.UnitSelect;

        return write(bno, BNO_UNIT_SEL, unit_reg);
}

//Seleção dos filtros de leitura dos dados
//Os filtros somente são utilizados se não estiver no modo fusion

//Acelerometro
error_t BNO055_acc_filter (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t acc_filter_raw = read(bno, BNO_ACC_CONFIG);
        acc_filter_reg = acc_filter_raw.value;

        //Adição da configuração selecionada
        acc_filter_reg |= bno.config.AccelFilter;

        return write(bno, BNO_ACC_CONFIG, acc_filter_reg);
}

//Gyroscope
error_t BNO055_gyr_filter (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t gyr_filter_raw = read(bno, BNO_GYR_CONFIG);
        gyr_filter_reg = gyr_filter_raw.value;

        //Adição da configuração selecionada
        gyr_filter_reg |= bno.config.GyroFilter;

        return write(bno, BNO_GYR_CONFIG, gyr_filter_reg);
}

//Magnetometer
error_t BNO055_mag_filter (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t mag_filter_raw = read(bno, BNO_MAG_CONFIG);
        mag_filter_reg = mag_filter_raw.value;

        //Adição da configuração selecionada
        mag_filter_reg |= bno.config.MagFilter;


        return write(bno, BNO_MAG_CONFIG, mag_filter_reg);
}

//Coloca o chip em modo de configuração
error_t BNO055_config_mode (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t config_mode_raw = read(bno, BNO_OPR_MODE);
        config_mode_reg = config_mode_raw.value;

        //Adição da configuração selecionada
        config_mode_reg |= BNO_MODE_CONFIG;


        return write(bno, BNO_OPR_MODE, config_mode_reg);
}

// Função de seleção de modo de operação
error_t BNO055_opmode (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t opmode_raw = read(bno, BNO_OPR_MODE);
        opmode_reg = opmode_raw.value;

        //Adição da configuração selecionada
        opmode_reg |= bno.config.OpMode;


        return write(bno, BNO_OPR_MODE, opmode_reg);
}

// Função de reset do chip
error_t BNO_reset (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t sys_raw = read(bno, BNO_SYS_TRIGGER);
        sys_reg = sys_raw.value;

        //Adição da configuração selecionada
        sys_reg |= BNO_SYS_RST;


        return write(bno, BNO_SYS_TRIGGER, sys_reg);
}

// Função de seleção de páginas
//pagina 0
error_t BNO_page0 (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t page_raw = read(bno, BNO_PAGE_ID);
        page_reg = page_raw.value;

        //Adição da configuração selecionada
        page_reg |= BNO_PAGE_0;


        return write(bno, BNO_PAGE_ID, page_reg);
}

//pagina 1
error_t BNO_page1 (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t page_raw = read(bno, BNO_PAGE_ID);
        page_reg = page_raw.value;

        //Adição da configuração selecionada
        page_reg |= BNO_PAGE_1;


        return write(bno, BNO_PAGE_ID, page_reg);
}

error_t BNO_test (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t page_raw = read(bno, BNO_SYS_TRIGGER);
        sys_reg = sys_raw.value;

        //Adição da configuração selecionada
        sys_reg |= BNO_SYS_ST;


        return write(bno, BNO_SYS_TRIGGER, sys_reg);
}