# GUVC-S10GD

## Propósito
Sensor de Radiação UV-C , que medirá a densidade de radiação do tipo UV-C (![unidades UVC](https://latex.codecogs.com/png.image?\dpi{110}%20\frac{mW}{cm^2})) que incidir-se-á nas sondas que o contiverem.

## Exemplo
Setup
```c
guvcs10gd_t guvc = {.adc = &hadc1};
guvcs10gd_init(guvc);
```

Medida
```c
float uvc_radiation = guvcs10gd_get_uvcpower(guvc);
printf("radiation in  (mW/cm^2): %f\r\n", radiation);
```
## Documentação
![equação do datasheet]()
![equação de conversão]()
Para ma
## Configuração do guvcs10gd.h
Devido à limitações de corrente emitidas pelo sensor GUVC-S10GD, faz-se necessário o uso de amplificadores/conversores para tornar o sinal, da ordem de nano-amperes, emitido por ele em um sinal capaz de ser lido.

Portanto, visto a variedade de circuitos que podem ser empregados para essa finalidade, foi estabelecido que para cada projeto que pretenda usar o GUVC-S10GD, será necessário configurar a sessão **"CONVERSION VALUES THAT VARY BETWEEN PROJECTS - AMPLIFIER"** do **guvcs10gd.h** com informações da curva de calibração do amplificador/conversor para o referido sensor.

Uma curva de calibração genérica está apresentada abaixo, mais detalhes sobre a relação da sessão do "" do guvcs10gd.h com a curva de calibração podem ser acessadas no pdf que descreve a [equação de conversão]().
![curva de calibração]()

## Notas
