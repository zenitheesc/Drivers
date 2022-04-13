/*
 * MMC5983MA.h
 *
 * Author: Thiago Henrique Vicentini (Anakin)
 */

#ifndef MMC5983MA_H_
#define MMC5983MA_H_

#include "bsp.h"

//#########################################################################################################################
//Register map for MMC5983MA

#define MMC5983MA_ADDRESS       0x30

#define MMC5983MA_XOUT_0        0x00
#define MMC5983MA_XOUT_1        0x01
#define MMC5983MA_YOUT_0        0x02
#define MMC5983MA_YOUT_1        0x03
#define MMC5983MA_ZOUT_0        0x04
#define MMC5983MA_ZOUT_1        0x05
#define MMC5983MA_XYZOUT_2      0x06
#define MMC5983MA_TOUT          0x07
#define MMC5983MA_STATUS        0x08
#define MMC5983MA_CONTROL_0     0x09
#define MMC5983MA_CONTROL_1     0x0A
#define MMC5983MA_CONTROL_2     0x0B
#define MMC5983MA_CONTROL_3     0x0C

#define MMC5983MA_INT_meas_done_en    0x04
#define MMC5983MA_Auto_SR_en          0x20

// Sample rates
#define MODR_ONESHOT   0x00
#define MODR_1Hz       0x01
#define MODR_10Hz      0x02
#define MODR_20Hz      0x03
#define MODR_50Hz      0x04
#define MODR_100Hz     0x05
#define MODR_200Hz     0x06 // BW = 0x01 only
#define MODR_1000Hz    0x07 // BW = 0x11 only

//Bandwidths
#define MBW_100Hz 0x00  // 8 ms measurement time
#define MBW_200Hz 0x01  // 4 ms
#define MBW_400Hz 0x02  // 2 ms
#define MBW_800Hz 0x03  // 0.5 ms


// Set/Reset as a function of measurements
#define MSET_1     0x00 // Set/Reset each data measurement
#define MSET_25    0x01 // each 25 data measurements
#define MSET_75    0x02
#define MSET_100   0x03
#define MSET_250   0x04
#define MSET_500   0x05
#define MSET_1000  0x06
#define MSET_2000  0x07

//#########################################################################################################################

error_t mmc5983ma_init(i2c_device_t device, uint8_t measurementFrequency, uint8_t bandwidth);
void mmc5983ma_readData(mmc5983ma_t *mmc, uint32_t * destination);
void SET(i2c_device_t device);
void RESET(i2c_device_t device);
void mmc5983ma_selfTest();

error_t readTemperature(i2c_device_t device, uint8_t temp);
error_t readStatus(i2c_device_t device, uint8_t _status);
void mmc5983ma_clearInt();
void powerDown(i2c_device_t device);
void powerUp(i2c_device_t device, uint8_t MODR);
void mmc5983ma_offsetBias(mmc5983ma_t *mmc, float * dest1, float * dest2);

#endif /* MMC5983MA_H_ */