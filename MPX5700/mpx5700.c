/*
 * mpx5700.c
 *
 *  Created on: Jul 21, 2021
 *      Author: leocelente
 */
#include "mpx5700.h"

error_t mpx5700_init(mpx5700_t mpx) { return adc_init(mpx.adc); }

result_uint16_t mpx5700_get_raw(mpx5700_t mpx) { return adc_read(mpx.adc); }

// Calulo do ganho de um divisor de tensão
static float volt_div(float r1, float r2) { return (r2) / (r1 + r2); }

/***
 * Reverte a conversão do circuito entre o ADC e o Sensor
 * Nesse caso, um divisor de tensão
 * Entrada: 0 ... 3.3f (volts)
 * Saida:   0 ... 5.0f (volts)
 */
static float inv_adjustment_circuit(float adc_in) {
  float R1 = 750.f;
  float R2 = 1.3e3f;
  float inv_volt_div = 1.f / (volt_div(R1, R2));
  return inv_volt_div * adc_in;
}

/***
 * Calcula a pressão baseado na tensão saindo do sensor
 * Supondo Vs = 5v
 * Entrada: 0 ...   5.0f 	(volts)
 * Saida: 	0 ... 700.0f	(kPa)
 */
static float conversion_function(float sensor_out) {
  float Vs = 5.f;
  float pressure = ((sensor_out / Vs) - MPX_CONV_A) / MPX_CONV_B;
  return pressure;
}

/***
 * Lê o ADC e cacula a pressão
 */
float mpx5700_get_pressure(mpx5700_t mpx) {
  result_uint16_t raw = mpx5700_get_raw(mpx);
  if (raw.hasError) {
    return -99.f;
  }

  // Tensão que chega no ADC
  const float read_voltage = adc_raw_to_voltage(raw.value);

  // Tensão saindo do Sensor
  const float sensor_out = inv_adjustment_circuit(read_voltage);

  return conversion_function(sensor_out);
}
