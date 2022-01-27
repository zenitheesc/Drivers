# MS5706 Barometric Pressure Sensor

## Propósito:
Sensor capaz de mensurar a pressão do ambiente

## Exemplo:

###### Setup:

```c
//Build device struct
prom_t prom = {0};
Dconst_t Dconst = {0};

i2c_device_t ms5607_dvc = {
.i2c = &hi2c1,
.address = I2C_ADR
};

ms5607_t ms5607 = {
        .device = ms5607_dvc,
        .prom = prom,
        .Dconst = Dconst,
        .OSR_mode = 0
};

//Initialize
ms5607_init(ms5607, mode_2);
```

###### Medida:
```c
//Declare and get the pressure
int32_t pressure = ms5607_getPressure(ms5607);
printf("Pressão: %d mbar", pressure);
```

## Documentação:

###### Funcionamento Geral:
A função prototipada em formato I2C, quando requisitada, inicializa duas *structs* (*PROM* e *Dconst*) para o ajustar uma variável de temperatura(*TEMP*), que posteriormente será referência para calibrar o cálculo da pressão. Para isso é necessário definir a quantidade de amostras (*OSR_mode*) que serão utilizadas para a conversão ADC  do sensor de temperatura e pressão.

###### OSR_mode:
Representa a quantidade de amostras (iguais para D1 e D2) que serão convertidas em sinais digitais (ADC), logo seguem o padrão:

- ```mode_1``` = 256 Samples
- ```mode_2``` = 512 Samples
- ```mode_3``` = 1024 Samples
- ```mode_4``` = 2048 Samples
- ```mode_5``` = 4096 Samples

## Referências:
- MS5607 Datasheet: <br>
https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS5607-02BA03%7FB2%7Fpdf%7FEnglish%7FENG_DS_MS5607-02BA03_B2.pdf%7FCAT-BLPS0035