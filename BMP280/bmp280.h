#include "platform/platform.h"

typedef int32_t bmp280_temp_calib_t[4];
typedef int32_t bmp280_pres_calib_t[10];

typedef struct {
  i2c_device_t dev;
  bmp280_temp_calib_t temp_calib;
  bmp280_pres_calib_t pres_calib;
} bmp280_t;

typedef struct {
  float temperature;
  float pressure;
  error_t error;
} bmp280_measurement_t;

error_t bmp280_init(bmp280_t *bmp);

bmp280_measurement_t bmp280_measure(bmp280_t *bmp);

#define BMP_TEMP_XLSB 0xFC
#define BMP_TEMP_LSB 0xFB
#define BMP_TEMP_MSB 0xFA
#define BMP_PRESS_XLSB 0xF9
#define BMP_PRESS_LSB 0xF8
#define BMP_PRESS_MSB 0xF7
#define BMP_CONFIG 0xF5
#define BMP_CTRL_MEAS 0xF4
#define BMP_STATUS 0xF3
#define BMP_RESET 0xE0
#define BMP_ID 0xD0
#define BMP_EXPECTED_ID 0x58
#define BMP_CALIB25 0xA1
#define BMP_CALIB_T1_MSB 0x88
#define BMP_CALIB_T1_LSB 0x89

#define BMP_CALIB_P1_MSB 0x8E

#define BMP_CALIB_T1_MSB 0x88
