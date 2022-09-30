# BMP280

## Propósito
Sensor de pressão e temperatura

## Exemplo
Setup
```c'
bmp280_t bmp = { .dev = { .address = 0x76, .i2c = &hi2c1 } };
bmp280_init(&bmp);

```

Medida:
```c
bmp280_measurement_t measure = bmp280_measure(&bmp);
printf("Presure: %.1f Temperature: %.2f", measure.pressure, measure.temperature);
```

## Documentação

## Notas

