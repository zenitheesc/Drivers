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

// Registradores de configuração









// Registradores de leitura dos dados
// Acelerômetro

// Temperatura

// Giroscópio

// Magnetômetro

// Euler degree

// Quaternions degree

// LIA(linear acceleration)

// GV(Gravity vector)

typedef struct {
    //General

    //Filter

    //Ranges

    //Modes
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