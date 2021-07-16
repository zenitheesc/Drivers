# HDC1080

## Propósito
Sensor capaz de medir a umidade do ar e temperatura ambiente.

## Exemplo
Setup
```c
i2c_device_t sen = { .address = HDC1080_ADDR, .i2c = &hi2c1 };
hdc1080_init(sen);
```

Medida:
```c
hdc1080_mesurement_t th = hdc1080_mesure(sen);
printf("temp: %f\r\n", th.temperature);
printf("humd: %f\r\n", th.humidity);
```

## Documentação
Quando é requisitado uma medição, a temperatura e umidade são medidas sequencialmente 
 o sensor leva em torno de 7ms para medir cada uma então, no total , 14ms.

Ordem de operações:
 - configura somente temperatura
 - le temperatura
 - configura somente umidade
 - le umidade 

## Notas
Os módulos mais comuns não tem pinos para alterar o endereço I2C (0x40) que é
o mesmo que o padrão do INA219

Em teoria é possivel medir ambas ao mesmo tempo e reduzir o intervalo, mas 
não obtive bons resultados.  A umidade sempre corrompia. Então optei por 
fazer as medições em separado.

