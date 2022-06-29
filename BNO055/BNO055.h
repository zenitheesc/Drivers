/*
 * BNO055.h
 *
 *  Created on: Jun 10, 2022
 *      Author: Murilo Henrique Pasini Trevisan
*/

#ifndef INC_BNO055_H
#define INC_BNO055_H

#include "platform.h"

/* REGISTRADORES */

// Name                //Address(hex)

// Registradores de leitura dos dados
#define BNO_MEASURES        (uint8_t)(0x08)

// Acelerômetro
#define BNO_ACC_DATA_X_LSB  (uint8_t)(0x08)
#define BNO_ACC_DATA_X_MSB  (uint8_t)(0x09)
#define BNO_ACC_DATA_Y_LSB  (uint8_t)(0x0A)
#define BNO_ACC_DATA_Y_MSB  (uint8_t)(0x0B)
#define BNO_ACC_DATA_Z_LSB  (uint8_t)(0x0C)
#define BNO_ACC_DATA_Z_MSB  (uint8_t)(0x0D)

// Magnetômetro
#define BNO_MAG_DATA_X_LSB  (uint8_t)(0x0E)
#define BNO_MAG_DATA_X_MSB  (uint8_t)(0x0F)
#define BNO_MAG_DATA_Y_LSB  (uint8_t)(0x10)
#define BNO_MAG_DATA_Y_MSB  (uint8_t)(0x11)
#define BNO_MAG_DATA_Z_LSB  (uint8_t)(0x12)
#define BNO_MAG_DATA_Z_MSB  (uint8_t)(0x13)

// Giroscópio
#define BNO_GYR_DATA_X_LSB  (uint8_t)(0x14)
#define BNO_GYR_DATA_X_MSB  (uint8_t)(0x15)
#define BNO_GYR_DATA_Y_LSB  (uint8_t)(0x16)
#define BNO_GYR_DATA_Y_MSB  (uint8_t)(0x17)
#define BNO_GYR_DATA_Z_LSB  (uint8_t)(0x18)
#define BNO_GYR_DATA_Z_MSB  (uint8_t)(0x19)

// Euler degree
#define BNO_EUL_DATA_X_LSB  (uint8_t)(0x1A)
#define BNO_EUL_DATA_X_MSB  (uint8_t)(0x1B)
#define BNO_EUL_DATA_Y_LSB  (uint8_t)(0x1C)
#define BNO_EUL_DATA_Y_MSB  (uint8_t)(0x1D)
#define BNO_EUL_DATA_Z_LSB  (uint8_t)(0x1E)
#define BNO_EUL_DATA_Z_MSB  (uint8_t)(0x1F)

// Quaternions degree
#define BNO_QUA_DATA_X_LSB  (uint8_t)(0x20)
#define BNO_QUA_DATA_X_MSB  (uint8_t)(0x21)
#define BNO_QUA_DATA_Y_LSB  (uint8_t)(0x22)
#define BNO_QUA_DATA_Y_MSB  (uint8_t)(0x23)
#define BNO_QUA_DATA_Z_LSB  (uint8_t)(0x24)
#define BNO_QUA_DATA_Z_MSB  (uint8_t)(0x25)
#define BNO_QUA_DATA_W_LSB  (uint8_t)(0x26)
#define BNO_QUA_DATA_W_MSB  (uint8_t)(0x27)

// LIA(linear acceleration)
#define BNO_LIA_DATA_X_LSB  (uint8_t)(0x28)
#define BNO_LIA_DATA_X_MSB  (uint8_t)(0x29)
#define BNO_LIA_DATA_Y_LSB  (uint8_t)(0x2A)
#define BNO_LIA_DATA_Y_MSB  (uint8_t)(0x2B)
#define BNO_LIA_DATA_Z_LSB  (uint8_t)(0x2C)
#define BNO_LIA_DATA_Z_MSB  (uint8_t)(0x2D)

// GV(Gravity vector)
#define BNO_GRV_DATA_X_LSB  (uint8_t)(0x2E)
#define BNO_GRV_DATA_X_MSB  (uint8_t)(0x2F)
#define BNO_GRV_DATA_Y_LSB  (uint8_t)(0x30)
#define BNO_GRV_DATA_Y_MSB  (uint8_t)(0x31)
#define BNO_GRV_DATA_Z_LSB  (uint8_t)(0x32)
#define BNO_GRV_DATA_Z_MSB  (uint8_t)(0x33)

// Temperature
#define BNO_TEMP_DATA       (uint8_t)(0x34)

// Registradores de configuração

// Mudança de página
#define Page_ID             (uint8_t)(0x07)

// Page 0 //

// Seleeção da unidade de medida
#define BNO_UNIT_SEL        (uint8_t)(0x3B)

// Modo de operação(!!!)
#define BNO_OPR_MODE        (uint8_t)(0x3D)

// Triggers do sistema
#define BNO_SYS_TRIGGER     (uint8_t)(0x3F)

// Page 1 //

// Configuração do acelerometro
#define BNO_ACC_CONFIG      (uint8_t)(0x08)

// Configuração do magnetometro
#define BNO_MAG_CONFIG      (uint8_t)(0x09)






/* CONFIGURAÇÕES */

//UNIT_SEL
//Por padrão espera-se que os defaults sejam zerados
//Como o datasheet não comenta o default
//Altera-se para todos zeros, para unidades convenientes
#define BNO_UNIT_DEFAULT    (uint8_t)(0x00)
#define BNO_UNIT_ANDROID    (uint8_t)(0x80)
#define BNO_UNIT_RADIAN     (uint8_t)(0x0C)

//OPR_MODE
//Há muitas formas dele operar
//Cada uma com um conjunto de sensores
//Deve-se colocar no modo de configuração para escrever registradores
//Deve-se sair do modo configuração para receber dados
//Os modos fusion não aceitam muitas configurações
//Somente o OPR_MODE é writable se não estiver neste modo
#define BNO_MODE_CONFIG     (uint8_t)(0x00)
//Non-fusion modes
#define BNO_MODE_ACCONLY    (uint8_t)(0x01)
#define BNO_MODE_MAGONLY    (uint8_t)(0x02)
#define BNO_MODE_GYRONLY    (uint8_t)(0x03)
#define BNO_MODE_ACCGYRO    (uint8_t)(0x05)
//Fusion modes
#define BNO_MODE_IMU        (uint8_t)(0x08)
#define BNO_MODE_M4G        (uint8_t)(0x0A)
#define BNO_MODE_NDOFOFF    (uint8_t)(0x0B)
#define BNO_MODE_NDO        (uint8_t)(0x0C)

//SYS_TRIGGER
//Além das funções de interrupções não utilizadas
//Há dois bits para reset do sistema
//Importantes para reiniciar o sistema e garantir tudo em default
//Reset do sistema
#define BNO_SYS_RST         (uint8_t)(0x20)
//Self test
#define BNO_SYS_ST          (uint8_t)(0x01)

// Page ID
// Mudar a página do bloco de registradores
// Registradores separados já por comentários em cada página
// Registradores de leitura na primeira página
#define BNO_PAGE_0          (uint8_t)(0x00)
#define BNO_PAGE_1          (uint8_t)(0X01)
// Mesma página das configurações 1 mas com nome diferebnte 
// para facilitar a leitura do driver
#define BNO_PAGE_MEASURES   (uint8_t)(0x00)

// ACC_CONFIG
// As configurações do acelerometro somente podem ser alteradas
// no modo sensor e não no modo fusion, ainda assim, para teste
// pode ser util testar o modo sensor
// Caso seja escrito algo e colocado em modo fusion
// o proprio BNO reescreve os registradores sem problemas
// Considerando modo de operação normal e 4G de range
//                                   (0b000xxx01)
#define BNO_ACC_07HZ        (uint8_t)(0x01)
#define BNO_ACC_16HZ        (uint8_t)(0x05)
#define BNO_ACC_31HZ        (uint8_t)(0x09)
#define BNO_ACC_62HZ        (uint8_t)(0x0D)
#define BNO_ACC_125HZ       (uint8_t)(0x11)
#define BNO_ACC_250HZ       (uint8_t)(0x15)
#define BNO_ACC_500HZ       (uint8_t)(0x19)
#define BNO_ACC_10000HZ     (uint8_t)(0x1D)

// MAG_CONFIG
// As configurações do magnetometro seguem a mesma regra do acc
// Considerando modo de energia normal e operação regular
// mudando a taxa de saida de dados
//
#define BNO_MAG_2HZ         (uint8_t)(0x08)
#define BNO_MAG_6HZ         (uint8_t)(0x09)
#define BNO_MAG_8HZ         (uint8_t)(0x10)
#define BNO_MAG_10HZ        (uint8_t)(0x0B)
#define BNO_MAG_15HZ        (uint8_t)(0x0C)
#define BNO_MAG_20HZ        (uint8_t)(0x0D)
#define BNO_MAG_25HZ        (uint8_t)(0x0E)
#define BNO_MAG_30HZ        (uint8_t)(0x0F)








typedef struct {
    //Unit
    uint8_t UnitSelect;
    //Filter
    uint8_t LowPassFilter;
    //Ranges
    uint8_t AccelRange;
    uint8_t GyroRange;
    uint8_t MagRange;
    //Modes
    uint8_t OpMode;
}BNO055_config_t;

typedef struct {
    i2c_device_t dev;
    BNO055_config_t config;
}BNO055_t;

typedef struct {
    float AccelX;
    float AccelY;
    float AccelZ;
    float Temp;
    float GyroX;
    float GyroY;
    float GyroZ;
    float EulerX;
    float EulerY;
    float EulerZ;
    float QuateX;
    float QuateY;
    float QuateZ;
    float QuateW;
}

