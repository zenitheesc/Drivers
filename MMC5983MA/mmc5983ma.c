/*
 * MMC5983MA.c
 *
 * Author: Thiago Henrique Vicentini (Anakin)
 */

#include "mmc5983ma.h"

error_t mmc5983ma_init(i2c_device_t device, uint8_t measurementFrequency, uint8_t bandwidth){
    device.addr = MMC5983MA_ADDRESS;

    result8_t result;
    error_t error;

    // Start configuration registers
    //--------------------------------------------------------------------------------

    result = i2c_read8(device, MMC5983MA_CONTROL_0);
    if(result.hasError)
        return result.hasError;

    // -> enable the interrupt for completed measurements (bit 2 == 1)
    // -> enable auto set/reset (bit 5 == 1)
    result.value |= (MMC5983MA_Auto_SR_en | MMC5983MA_INT_meas_done_en);
    if(error = i2c_write8(device, MMC5983MA_CONTROL_0, result.value))
        return error;

    //--------------------------------------------------------------------------------

    result = i2c_read8(device, MMC5983MA_CONTROL_1);
    if(result.hasError)
        return result.hasError;

    // -> set magnetometer bandwidth
    result.value |= bandwidth;
    if(error = i2c_write8(device, MMC5983MA_CONTROL_1, result.value));
        return error;
    
    //--------------------------------------------------------------------------------

    result = i2c_read8(device, MMC5983MA_CONTROL_2);
    if(result.hasError)
        return result.hasError;

    // -> enable continuous measurement mode (bit 3 == 1)
    // -> enable the feature of periodic set (bit 7 == 1)
    result.value |= (0x08 | measurementFrequency); // 0x80 | (setFrequency << 4) | 0x08 | measurementFrequency
    if(error = i2c_write8(device, MMC5983MA_CONTROL_2, result.value));
        return error;

    //--------------------------------------------------------------------------------
    // End configuration registers

    return 0;
}

error_t readData(i2c_device_t device, uint32_t* destination){
    uint8_t rawData[7] = { 0 }; // x/y/z mag register data stored here
	buffer_view_t view = { .data = rawData, .size = sizeof(rawData) };
	error_t error = i2c_readN(device, MMC5983MA_XOUT_0, view);
    if(error) return error;

    destination[0] = (uint32_t)(rawData[0] << 10 | rawData[1] << 2 | (rawData[6] & 0xC0) >> 6); // Turn the 18 bits into a unsigned 32-bit value
    destination[1] = (uint32_t)(rawData[2] << 10 | rawData[3] << 2 | (rawData[6] & 0x30) >> 4); // Turn the 18 bits into a unsigned 32-bit value
    destination[2] = (uint32_t)(rawData[4] << 10 | rawData[5] << 2 | (rawData[6] & 0x0C) >> 2); // Turn the 18 bits into a unsigned 32-bit value

	return 0;
}

void SET(i2c_device_t device){
    result8_t result = i2c_read8(device, MMC5983MA_CONTROL_0);
    if(result.hasError)
        return result.hasError;

    result.value |= 0x08;
    i2c_write8(device, MMC5983MA_CONTROL_0, result.value);  
    delay_ms(1); // self clearing after 500 us
}

void RESET(i2c_device_t device){
    result8_t result = i2c_read8(device, MMC5983MA_CONTROL_0);
    if(result.hasError)
        return result.hasError;
    
    result.value |= 0x10;
    i2c_write8(device, MMC5983MA_CONTROL_0, result.value);
    delay_ms(1);    // self clearing after 500 us
}

void selfTest(i2c_device_t device){
    uint8_t rawData[6] = {0};  // x/y/z mag register data stored here
    uint16_t data_set[3] = {0}, data_reset[3] = {0};
    uint32_t delta_data[3] = {0};
    
    // Clear control registers
    i2c_write8(device, MMC5983MA_CONTROL_0, 0x00);  
    i2c_write8(device, MMC5983MA_CONTROL_1, 0x00);  
    i2c_write8(device, MMC5983MA_CONTROL_2, 0x00);

    SET(); // Enable set current
    i2c_write8(device, MMC5983MA_CONTROL_0, 0x01);  // Enable one-time mag measurement
    delay_ms(10);
    
    i2c_read8(device, MMC5983MA_XOUT_0, 6, &rawData[0]);  // Read the 6 raw data registers into data array
    data_set[0] = (uint16_t) (((uint16_t) rawData[0] << 8) | rawData[1]); // x-axis
    data_set[1] = (uint16_t) (((uint16_t) rawData[2] << 8) | rawData[3]); // y-axis
    data_set[2] = (uint16_t) (((uint16_t) rawData[4] << 8) | rawData[5]); // z-axis

    RESET(); // Enable reset current
    i2c_write8(device, MMC5983MA_CONTROL_0, 0x01);  // Enable one-time mag measurement
    delay_ms(10);
    
    i2c_read8(device, MMC5983MA_XOUT_0, 6, &rawData[0]);  // Read the 6 raw data registers into data array
    data_reset[0] = (uint16_t) (((uint16_t) rawData[0] << 8) | rawData[1]); // x-axis
    data_reset[1] = (uint16_t) (((uint16_t) rawData[2] << 8) | rawData[3]); // y-axis
    data_reset[2] = (uint16_t) (((uint16_t) rawData[4] << 8) | rawData[5]); // z-axis
 
    for(uint8_t i = 0; i < 3; i++){
        if(data_set[i] > data_reset[i])
            delta_data[i] = data_set[i] - data_reset[i];
        else
            delta_data[i] = data_reset[i] - data_set[i];
    }
}

//--------------------------------------------------------------------------------

error_t readTemperature(i2c_device_t device, uint8_t temp){
    result8_t result = i2c_read8(device, MMC5983MA_TOUT); // Read temperature register
    if(result.hasError)
        return 1;

    (temp*) = result.value;

    return 0;
}

error_t readStatus(i2c_device_t device, uint8_t _status){
    result8_t result = i2c_read8(device, MMC5983MA_STATUS); // Read status register
    if(result.hasError)
        return 1;

    (_status*) = result.value;

    return 0;
}

void clearInt(i2c_device_t device){
    uint8_t temp = i2c_read8(device, MMC5983MA_STATUS);
    temp &= 0x01; // Clear data ready interrupts
    i2c_write8(device, MMC5983MA_STATUS, temp);
}

void powerDown(i2c_device_t device){
    uint8_t temp = i2c_read8(device, MMC5983MA_CONTROL_2);
    temp &= ~(0x07); // Clear lowest four bits
    i2c_write8(device, MMC5983MA_CONTROL_2, temp);
    delay_ms(20);   // Make sure to finish the lest measurement
}

void powerUp(i2c_device_t device, uint8_t MODR){
    uint8_t temp = i2c_read8(device, MMC5983MA_CONTROL_2);
    i2c_write8(device, MMC5983MA_CONTROL_2, temp | MODR);  // Start continuous mode
}

void offsetBias(i2c_device_t device, float* dest1, float* dest2){
    int32_t mag_bias[3] = {0, 0, 0}, mag_scale[3] = {0, 0, 0};
    int32_t mag_max[3] = {-262143, -262143, -262143}, mag_min[3] = {262143, 262143, 262143};
    uint32_t mag_temp[3] = {0, 0, 0}, magOffset = 131072;
    float _mRes = 1.0f/16384.0f; // mag sensitivity if using 18 bit data
    delay_ms(4000);

    for (int i=0; i<4000; i++){
        readData(device, mag_temp);

        for (int j=0; j<3; j++){
            if((int32_t)(mag_temp[j] - magOffset) > mag_max[j]) 
                mag_max[j] = (int32_t)(mag_temp[j] - magOffset);

            if((int32_t)(mag_temp[j] - magOffset) < mag_min[j]) 
                mag_min[j] = (int32_t)(mag_temp[j] - magOffset);
        }

        delay_ms(12);
    }

    // Get hard iron correction
    mag_bias[0]  = (mag_max[0] + mag_min[0])/2; // get average x mag bias in counts
    mag_bias[1]  = (mag_max[1] + mag_min[1])/2; // get average y mag bias in counts
    mag_bias[2]  = (mag_max[2] + mag_min[2])/2; // get average z mag bias in counts
    
    dest1[0] = (float) (mag_bias[0]) * _mRes; // Save mag biases in G for main program
    dest1[1] = (float) (mag_bias[1]) * _mRes;   
    dest1[2] = (float) (mag_bias[2]) * _mRes;  
    
    // Get soft iron correction estimate
    mag_scale[0]  = (mag_max[0] - mag_min[0])/2; // get average x axis max chord length in counts
    mag_scale[1]  = (mag_max[1] - mag_min[1])/2; // get average y axis max chord length in counts
    mag_scale[2]  = (mag_max[2] - mag_min[2])/2; // get average z axis max chord length in counts

    float avg_rad = (mag_scale[0] + mag_scale[1] + mag_scale[2])/3.0f;

    dest2[0] = avg_rad/((float)mag_scale[0]);
    dest2[1] = avg_rad/((float)mag_scale[1]);
    dest2[2] = avg_rad/((float)mag_scale[2]);
}