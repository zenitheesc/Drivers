#include "Application/bmp280.h"
#include <stdint.h>

static result_uint8_t read(bmp280_t *bmp, uint8_t addr) {
  error_t e = i2c_transmit(bmp->dev, (buffer_view_t){.data = &addr, .size = 1});
  uint8_t value = 0xFF;
  buffer_view_t value_v = {.data = &value, .size = sizeof(value)};
  e |= i2c_receive(bmp->dev, value_v);
  return (result_uint8_t){.hasError = e, .value = value};
}

static error_t write(bmp280_t *bmp, uint8_t addr, uint8_t value) {
  uint8_t write[] = {addr, value};
  buffer_view_t write_v = {.data = write, .size = sizeof(write)};
  return i2c_transmit(bmp->dev, write_v);
}

static result_int read_temp(bmp280_t *bmp) {
  error_t e = 0x00;
  uint8_t start_addr = BMP_TEMP_MSB;
  e |= i2c_transmit(bmp->dev, (buffer_view_t){.data = &start_addr, .size = 1});

  uint8_t temp_buf[3] = {0};
  buffer_view_t temp_buf_v = {.data = temp_buf, .size = sizeof(temp_buf)};
  e |= i2c_receive(bmp->dev, temp_buf_v);
  int32_t temp = temp_buf[0] << 16 | temp_buf[1] << 8 | temp_buf[0];
  temp >>= 4;
  return (result_int){.value = temp, .hasError = e};
}

static result_int read_pres(bmp280_t *bmp) {
  error_t e = 0x00;
  uint8_t start_addr = BMP_PRESS_MSB;
  e |= i2c_transmit(bmp->dev, (buffer_view_t){.data = &start_addr, .size = 1});

  uint8_t pres_buf[3] = {0};
  buffer_view_t temp_buf_v = {.data = pres_buf, .size = sizeof(pres_buf)};
  e |= i2c_receive(bmp->dev, temp_buf_v);
  int32_t pres = pres_buf[0] << 16 | pres_buf[1] << 8 | pres_buf[0];
  pres >>= 4;
  return (result_int){.value = pres, .hasError = e};
}

static void reset(bmp280_t *bmp) { write(bmp, BMP_RESET, 0xB6); }

static error_t read_temp_calibration(bmp280_t *bmp) {
  error_t e = 0;
  // We skip index 0 to keep datasheet indexing (start from 1)
  for (int i = 0; i < 3 * 2; i += 2) {
    result_uint8_t lsb = read(bmp, BMP_CALIB_T1_MSB + i);
    result_uint8_t msb = read(bmp, BMP_CALIB_T1_MSB + i + 1);
    bmp->temp_calib[(i / 2) + 1] = (int16_t)(msb.value << 8 | lsb.value);
    e |= msb.hasError | lsb.hasError;
  }
  bmp->temp_calib[1] &= ~0xFFFF0000;
  return e;
}

static error_t read_pres_calibration(bmp280_t *bmp) {
  error_t e = 0;
  // We skip index 0 to keep datasheet indexing (start from 1)
  for (int i = 0; i < 9 * 2; i += 2) {
    result_uint8_t lsb = read(bmp, BMP_CALIB_P1_MSB + i);
    result_uint8_t msb = read(bmp, BMP_CALIB_P1_MSB + i + 1);
    bmp->pres_calib[(i / 2) + 1] = (int16_t)(msb.value << 8 | lsb.value);
    e |= msb.hasError | lsb.hasError;
  }
  bmp->pres_calib[1] &= ~0xFFFF0000;
  return e;
}

static error_t config(bmp280_t *bmp) {
  uint8_t ctrl_meas = 0x00;
  ctrl_meas |= (0b010 << 5); // osrs_t = x2 oversampling
  ctrl_meas |= (0b001 << 2); // osrs_p = x1 oversampling
  ctrl_meas |= (0b00 << 0);	  // mode   = sleep
  error_t e = write(bmp, BMP_CTRL_MEAS, ctrl_meas);

  uint8_t config = 0x00;
  config |= (0b000 << 5); // t_sb     =
  config |= (0b000 << 2); // filter   =
  config |= (0b0 << 0);   // spi3w_en =
  e |= write(bmp, BMP_CONFIG, config);
  return e;
}

static error_t force_measure(bmp280_t * bmp){
	result_uint8_t ctrl_meas = read(bmp, BMP_CTRL_MEAS);
	ctrl_meas.value |= (0b01 << 0);  // mode   = forced
	error_t e = write(bmp, BMP_CTRL_MEAS, ctrl_meas.value);
  return e;

}


error_t bmp280_init(bmp280_t *bmp) {
  result_uint8_t id = read(bmp, BMP_ID);
  error_t e = id.hasError;
  if (id.value != BMP_EXPECTED_ID) {
    // return e;
  }
  e |= read_pres_calibration(bmp);
  e |= read_temp_calibration(bmp);
  e |= config(bmp);
  return e;
}

bmp280_measurement_t bmp280_measure(bmp280_t *bmp) {
  bmp280_measurement_t out = {.temperature = 0.f, .pressure = 0.f, .error = 0};
  force_measure(bmp);
  result_int temp = read_temp(bmp);

  int64_t var1 = (temp.value >> 3) - (bmp->temp_calib[1] << 1);
  var1 *= bmp->temp_calib[2];
  var1 >>= 11;

  int64_t var2 = (temp.value >> 4) - bmp->temp_calib[1];
  var2 = var2 * var2;
  var2 >>= 12;
  var2 *= bmp->temp_calib[3];
  var2 >>= 14;

  int64_t t_fine = var1 + var2;

  float T = (t_fine * 5 + 128) >> 8;
  T /= 100.f;

  result_int pres = read_pres(bmp);

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)bmp->pres_calib[6];
  var2 = var2 + ((var1 * (int64_t)bmp->pres_calib[5]) << 17);
  var2 = var2 + (((int64_t)bmp->pres_calib[4]) << 35);
  var1 = ((var1 * var1 * (int64_t)bmp->pres_calib[3]) >> 8) +
         ((var1 * (int64_t)bmp->pres_calib[2]) << 12);
  var1 = (((((int64_t) 1) << 47) + var1))
									* ((int64_t) bmp->pres_calib[1]) >> 33;

							if (var1 == 0) {
								out.error = 1;
								return out; // avoid exception caused by division by zero
							}

							int64_t p = 1048576 - pres.value;
							p = (((p << 31) - var2) * 3125) / var1;
							var1 = (((int64_t) bmp->pres_calib[9]) * (p >> 13)
									* (p >> 13)) >> 25;
							var2 = (((int64_t) bmp->pres_calib[8]) * p) >> 19;

							p = ((p + var1 + var2) >> 8)
									+ (((int64_t) bmp->pres_calib[7]) << 4);
							float P = p / 256.f;

							out.pressure = P;
							out.temperature = T;
							out.error = 0;
							return out;
						}
