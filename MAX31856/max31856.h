/*
 * MAX31856.h
 *
 * Author: Thiago Henrique Vicentini (Anakin)
 */

#ifndef INC_MAX31856_H_
#define INC_MAX31856_H_

#include "..\bsp.h"

//#########################################################################################################################
#define MAX31856_CR0_READ                0x00
#define MAX31856_CR0_WRITE               0x80
#define MAX31856_CR1_READ                0x01
#define MAX31856_CR1_WRITE               0x81
#define MAX31856_MASK_READ               0x02
#define MAX31856_MASK_WRITE              0x82
#define MAX31856_CJHF_READ               0x03
#define MAX31856_CJHF_WRITE              0x83
#define MAX31856_CJLF_READ               0x04
#define MAX31856_CJLF_WRITE              0x84
#define MAX31856_LTHFTH_READ             0x05
#define MAX31856_LTHFTH_WRITE            0x85
#define MAX31856_LTHFTL_READ             0x06
#define MAX31856_LTHFTL_WRITE            0x86
#define MAX31856_LTLFTH_READ             0x07
#define MAX31856_LTLFTH_WRITE            0x87
#define MAX31856_LTLFTL_READ             0x08
#define MAX31856_LTLFTL_WRITE            0x88
#define MAX31856_CJTO_READ               0x09
#define MAX31856_CJTO_WRITE              0x89
#define MAX31856_CJTH_READ               0x0A
#define MAX31856_CJTH_WRITE              0x8A
#define MAX31856_CJTL_READ               0x0B
#define MAX31856_CJTL_WRITE              0x8B

#define MAX31856_LTCBH_READ              0x0C
#define MAX31856_LTCBM_READ              0x0D
#define MAX31856_LTCBL_READ              0x0E

#define MAX31856_CONFIG_CMODE            0x80

#define MAX31856_Temperature_Resolution  7.8125e-3
//#########################################################################################################################

typedef struct {
    spi_device_t* device;
} max31856_t;

typedef enum {
    MAX31856_1_SAMPLE   = 0x00,
    MAX31856_2_SAMPLES  = 0x01,
    MAX31856_4_SAMPLES  = 0x02,
    MAX31856_8_SAMPLES  = 0x03,
    MAX31856_16_SAMPLES = 0x04,
} max31856_samples_t;

typedef enum {
    MAX31856_TCTYPE_B  = 0x00,
    MAX31856_TCTYPE_E  = 0x01,
    MAX31856_TCTYPE_J  = 0x02,
    MAX31856_TCTYPE_K  = 0x03,
    MAX31856_TCTYPE_N  = 0x04,
    MAX31856_TCTYPE_R  = 0x05,
    MAX31856_TCTYPE_S  = 0x06,
    MAX31856_TCTYPE_T  = 0x07,
    MAX31856_VMODE_G8  = 0x08,
    MAX31856_VMODE_G32 = 0x0C,
} max31856_thermocoupletype_t;

error_t max31856_setConversionMode(max31856_t *max, uint8_t enable);
error_t max31856_setAveragingMode(max31856_t *max, max31856_samples_t samples);
error_t max31856_setThermocoupleType(max31856_t *max, max31856_thermocoupletype_t tc_type);

error_t max31856_init(max31856_t *max, SPI_HandleTypeDef *spi, gpio_pin_t gpio);
error_t max31856_readThermocoupleTemp(max31856_t *max, float *temp);

#endif /* INC_MAX31856_H_ */