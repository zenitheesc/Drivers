# MS5706 Barometric Pressure Sensor

## Propósito:
Sensor capaz de mensurar a pressão do ambiente

## Exemplo:

###### Setup:

```c
//Build device struct
i2c_device_t ms5607 = {
.i2c = &hi2c1,
.address = I2C_ADR
};

//Initialize
ms5607_init(ms5607);
```

###### Medida:
```c
// OSR_Mode: (Same for D1 and D2)
//            0 = 256 Samples
//            1 = 512 Samples
//            2 = 1024 Samples
//            3 = 2048 Samples
//            4 = 4096 Samples

//Configure OSR_mode
uint8_t OSR_mode = ... ;

//Declare and get the pressure
int32_t pressure = ms5607_getPressure(ms5607, OSR_mode);
printf("Pressão: %d mbar", pressure);
```

## Documentação:
A função prototipada em formato I2C, quando requisitada, inicializa duas *structs* (*PROM* e *Dconst*) para o ajustar uma variável de temperatura(*TEMP*), que posteriormente será referência para calibrar o cálculo da pressão. Para isso é necessário definir a quantidade de amostras (*OSR_mode*) que serão utilizadas para a conversão ADC  do sensor de temperatura e pressão.

## Referências:
- MS5607 Datasheet: <br>
https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS5607-02BA03%7FB2%7Fpdf%7FEnglish%7FENG_DS_MS5607-02BA03_B2.pdf%7FCAT-BLPS0035