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
error_t BNO055_reset (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t sys_raw = read(bno, BNO_SYS_TRIGGER);
        sys_reg = sys_raw.value;

        //Adição da configuração selecionada
        sys_reg |= BNO_SYS_RST;


        return write(bno, BNO_SYS_TRIGGER, sys_reg);
}

// Função de seleção de páginas
//pagina 0
error_t BNO055_page0 (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t page_raw = read(bno, BNO_PAGE_ID);
        page_reg = page_raw.value;

        //Adição da configuração selecionada
        page_reg |= BNO_PAGE_0;


        return write(bno, BNO_PAGE_ID, page_reg);
}

//pagina 1
error_t BNO055_page1 (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t page_raw = read(bno, BNO_PAGE_ID);
        page_reg = page_raw.value;

        //Adição da configuração selecionada
        page_reg |= BNO_PAGE_1;


        return write(bno, BNO_PAGE_ID, page_reg);
}

// Função de self-test
error_t BNO055_test (BNO055_t bno) {
        //Leitura dos valores default do registrador
        result_uint8_t page_raw = read(bno, BNO_SYS_TRIGGER);
        sys_reg = sys_raw.value;

        //Adição da configuração selecionada
        sys_reg |= BNO_SYS_ST;


        return write(bno, BNO_SYS_TRIGGER, sys_reg);
}

// Função para realizar todas as configurações
error_t BNO055_config (BNO055_t bno) {

        // Coloca o chip em modo de configuração
        BNO055_config_mode(bno);

        // Seta a página das configurações iniciais
        BNO055_page0(bno);
        
        // Seta o sistema de unidades
        BNO055_unit(bno);

        // Seleciona o modo de operação
        BNO055_opmode(bno);

        // Configura os sensores caso não seja um modo fusion
        if (bno.config.OpMode < BNO_MODE_IMU) {
                // Realiza a configuração dos sensores
                // Muda de página
                BNO055_page1(bno);

                // ACC
                BNO055_acc_filter(bno);

                //GYR
                BNO055_gyr_filter(bno);

                //MAG
                BNO055_mag_filter(bno);
        }
}

// Função para inicializar o sistema
error_t BNO055_init (BNO055_t bno) {

        // Aplica um reset no sistema para garantir default
        BNO055_reset(bno);

        // Aplica as configurações
        BNO055_config(bno);

        // Coloca o sistema em em modo de leitura dos dados
        BNO055_page0(bno);
}

error_t BNO055_measures(BNO055_t bno, BNO055_values_t *medida) {

        // Realiza a leitura dos regustradores de resultados
        // Separa os resultados em cada uma das leituras
        // Converte se necessario as medidas
        // Armazena na struct de resultados para leitura

        // Cria um buffer para leitura burst-read
        uint8_t buffer[44] = {0xFF};
        buffer_view_t buffer_view = {.data = buffer, .size = sizeof(buffer)};

        // Leitura de todos os data registers
        uint8_t addr = BNO_MEASURES; //endereço do início das medidas
        buffer_view_t addr_v = {.data = &addr, .size = sizeof(addr)};
        i2c_transmit(bno.device, addr_v);
        error_t error = i2c_receive(bno.device, buffer_view);

        // Tratamento de erro da leitura da HAL
        if (error) {
                return error;
        }

        // Separação das componentes da leitura

        // Acelerômetro
        int16_t acc_x = ((buffer[0] << 8) | buffer[1]);
        int16_t acc_y = ((buffer[2] << 8) | buffer[3]);
        int16_t acc_z = ((buffer[4] << 8) | buffer[5]);

        // Magnetometro
        int16_t mag_x = ((buffer[6] << 8) | buffer[7]);
        int16_t mag_y = ((buffer[8] << 8) | buffer[9]);
        int16_t mag_z = ((buffer[10] << 8) | buffer[11]);

        // Gyroscopio
        int16_t gyr_x = ((buffer[12] << 8) | buffer[13]);
        int16_t gyr_y = ((buffer[14] << 8) | buffer[15]);
        int16_t gyr_z = ((buffer[16] << 8) | buffer[17]);

        // Euler
        int16_t eul_x = ((buffer[18] << 8) | buffer[19]);
        int16_t eul_y = ((buffer[20] << 8) | buffer[21]);
        int16_t eul_z = ((buffer[22] << 8) | buffer[23]);

        // Quaternion
        int16_t qua_x = ((buffer[24] << 8) | buffer[25]);
        int16_t qua_z = ((buffer[26] << 8) | buffer[27]);
        int16_t qua_y = ((buffer[28] << 8) | buffer[29]);
        int16_t qua_w = ((buffer[30] << 8) | buffer[31]);

        // LIA
        // GRV

        // Temperatura
        int16_t temp = (buffer[44]);

        // Alocação na struct de resultados
        // Acelerômetro
        int const scaleAcc = 1; // verificar se precisa escalar
        medida -> AccelX = ((float)acc_x) / scaleAcc;
        medida -> AccelY = ((float)acc_y) / scaleAcc;
        medida -> AccelZ = ((float)acc_z) / scaleAcc;

        // Giroscopio
        int const scaleGyr = 1; // verificar se precisa escalar
        medida -> GyroX = ((float)gyr_x) / scaleGyr;
        medida -> GyroY = ((float)gyr_y) / scaleGyr;
        medida -> GyroZ = ((float)gyr_z) / scaleGyr;

        // Magnetometro
        int const scaleMag = 1; // verificar se precisa escalar
        medida -> MagX = ((float)mag_x) / scaleMag;
        medida -> MagY = ((float)mag_y) / scaleMag;
        medida -> MagZ = ((float)mag_z) / scaleMag;

        // Euler
        int const scaleEul = 1; // verificar se precisa escalar
        medida -> EulerX = ((float)eul_x) / scaleEul;
        medida -> EulerY = ((float)eul_y) / scaleEul;
        medida -> EulerZ = ((float)eul_z) / scaleEul;

        // Quaternion
        int const scaleQua = 1; // verificar se precisa escalar
        medida -> QuateX = ((float)qua_x) / scaleQua;
        medida -> QuateY = ((float)qua_y) / scaleQua;
        medida -> QuateZ = ((float)qua_z) / scaleQua;
        medida -> QuateW = ((float)qua_w) / scaleQua;

        // Temp
        int const scaleTemp = 1; // verificar se precisa escalar
        medida -> Temp = ((float)temp) / scaleTemp;
}