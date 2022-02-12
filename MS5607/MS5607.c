//
// Created by gusta on 14/11/2021.
//
#include "MS5607.h"

static error_t ms5607_reset(ms5607_t* ms5607){
    //Create the reset command buffer
    uint_t buffer_Rcmd[1] = {RESET_CMD};

    buffer_view_t buffer_resetCmd = {
        .data = buffer_Rcmd,
        .size = sizeof(buffer_Rcmd)
    };

    return i2c_transmit(ms560->device, buffer_resetCmd);
}

static uint8_t get_PROM(ms5607_t* ms5607){

    uint16_t* prom_pt[] = {&ms5607->prom.C1, &ms5607->prom.C2, &ms5607->prom.C3,
                           &ms5607->prom.C4, &ms560->prom.C5,  &ms5607->prom.C6};

    //Needs to start on 1 (Add 0 reserved for manufacturer)
    for (int i = 1; i <= QNT_PROM_CONST; i++) {

        int const_ADR = i;

        //Found specific address to prom
        uint8_t CMD_PROM = PROM_MASK | (const_ADR<<1);

        //Create the array (command/response)
        uint_t prom_command[1] = {CMD_PROM};
        uint_t prom_response[2] = {0};

        //Create the buffer
        buffer_view_t buffer_promCommand = {
            .data = prom_commmand,
            .size = sizeof(prom_command)
        };
        buffer_view_t buffer_promResponse = {
            .data = prom_response,
            .size = sizeof(prom_response)
        };

        //Transmit / Receive system
        i2c_transmit(ms5607->device, buffer_promCommand);
        i2c_receive(ms5607->device, buffer_promResponse);

        //Concatenate response ".data[1]+.data[0]"
        uint16_t response = (buffer_promResponse.data[1]<<8);
        response = response | buffer_promReponse.data[0];

        //Write in "prom_t" struct
        *(prom_pt[i]) = response;
    }

    return 0;
}

static uint8_t get_Dconst(ms5607_t* ms5607){

    uint8_t mode = ms5607.OSR_mode;

    uint32_t* pt_Dconst[] = {&ms5607->Dconst.D1, &ms5607->Dconst.D2};
    uint8_t vec_mask [] = {D1_MASK, D2_MASK};

    for (int i = 0; i < QNT_DCONST; i++) {

        //Create the command according to OSR_mode
        int Command_D = i;
        Command_D = (Command_D<<1) | vec_mask[i];

        //Create the buffer cmd/resp
        uint_t buffer_Dcmd[1] = {Command_D};
        uint_t buffer_Dresp[3] = {0};


        buffer_view_t buffer_Dcommand = {
                .data = buffer_Dcmd,
                .size = sizeof(buffer_Dcmd)
        };
        buffer_view_t buffer_Dresponse = {
                .data = buffer_Dresp,
                .size = sizeof(buffer_Dresp)
        };

        //Transmit / receive system
        i2c_transmit(ms5607->device, buffer_Dcommand);
        i2c_receive(ms5607->device, buffer_Dresponse);

        //Concatenate response values ".data[2]+.data[1]+.data[0]"
        uint32_t response = 0;
        for (int j=2; j >= 0; j--){
            response = response | (buffer_Dresponse.data[j]<<(8*j));
        }

        //Attach values to Dconst struct
        *(pt_Dconst[i]) = response;
    }

    return 0;
}

uint8_t ms5607_init(ms5607_t* ms5607, enum OSR_samples mode){

    //Reset
    error_t resetDevice = ms5607_reset(ms5607);

    if (resetDevice.hasError) {
        return resetDevice.hasError;
    }

    //Write OSR_mode
    ms5607->OSR_mode = mode;

    //read PROM (att PROM struct)
    get_PROM(ms5607);

    delay_ms(10);

    return 0;
    }

int32_t ms5607_getPressure(ms5607_t* ms5607){
    int32_t T2, dT, TEMP  ;
    int64_t OFF, SENS, P, OFF2, SENS2, dT;

    T2 = OFF2 = SENS2 = 0;

    //Attach PROM values
    uint16_t C1 = ms5607->prom.C1;
    uint16_t C2 = ms5607->prom.C2;
    uint16_t C3 = ms5607->prom.C3;
    uint16_t C4 = ms5607->prom.C4;
    uint16_t C5 = ms5607->prom.C5;
    uint16_t C6 = ms5607->prom.C6;


    //Read D1 and D2 (att Dconst struct)
    get_Dconst(ms5607);

    //Attach D values
    uint32_t D1 = ms5607->Dconst.D1;
    uint32_t D2 = ms5607->Dconst.D2;

    //Calculate Temperature
    dT = D2 - (C5<<8));
    TEMP = 2000 + dT * (C6>>23));

    OFF = (C2<<17) + ((C4 * dT)>>6);
    SENS = (C1<<16) + ((C3 * dT)>>7));

    //Second Order Temperature Compensation
    if (TEMP < 20) {

        T2 = (dT*dT)>>31;
        OFF2 = 61 * ((TEMP-2000)*(TEMP-2000))>>4;
        SENS2 = 2*((TEMP - 2000)*(TEMP - 2000));
    }

    if (TEMP < 15) {
        OFF2 = OFF2 + (15 * (TEMP + 1500)* (TEMP + 1500));
        SENS2 = SENS2 + (8 * (TEMP + 1500) * (TEMP + 1500));
    }

    //Calculate compensated P and T
    TEMP = TEMP - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    P = ((D1 * (SENS>>21)) - OFF)>>15);

    return P;
}