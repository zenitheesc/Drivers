# GUVC-S10GD

## Propósito
Sensor de Radiação UV-C , que medirá a densidade de radiação do tipo UV-C (![unidades UVC](https://github.com/CarlosCraveiro/Drivers/blob/GUVC-S10GD/GUVC-S10GD/extra_doc/uvc_units.png)) que incidir-se-á nas sondas que o contiverem.

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
Equação de conversão entre corrente e densidade de radiação apresentada no [datasheet](http://www.geni-uv.com/download/products/GUVC-S10GD.pdf):
![equação do datasheet](https://github.com/CarlosCraveiro/Drivers/blob/GUVC-S10GD/GUVC-S10GD/extra_doc/datasheet_equation.png)

Equação de conversão genérica entre tensão de saída e corrente emitida pelo sensor:
![equação de conversão](https://github.com/CarlosCraveiro/Drivers/blob/GUVC-S10GD/GUVC-S10GD/extra_doc/conversion_equation.png)

## Configuração do guvcs10gd.h
Devido à limitações de corrente emitidas pelo sensor GUVC-S10GD, faz-se necessário o uso de amplificadores/conversores para tornar o sinal, da ordem de nano-amperes, emitido por ele em um sinal capaz de ser lido.

Portanto, visto a variedade de circuitos que podem ser empregados para essa finalidade, foi estabelecido que para cada projeto que pretenda usar o GUVC-S10GD, será necessário configurar a sessão **"CONVERSION VALUES THAT VARY BETWEEN PROJECTS - AMPLIFIER"** do **guvcs10gd.h** com informações da curva de calibração do amplificador/conversor para o referido sensor.

Uma curva de calibração genérica está apresentada abaixo, mais detalhes sobre a relação da sessão do **"CONVERSION VALUES THAT VARY BETWEEN PROJECTS - AMPLIFIER"** do **guvcs10gd.h** com a curva de calibração podem ser acessadas no pdf que descreve a [equação de conversão](https://github.com/CarlosCraveiro/Drivers/blob/GUVC-S10GD/GUVC-S10GD/extra_doc/Dedution_of_Conversion_Equation.pdf).
![curva de calibração](https://github.com/CarlosCraveiro/Drivers/blob/GUVC-S10GD/GUVC-S10GD/extra_doc/tension_current_relation_2.png)

## Notas
-Faz se necessário pré-configurar o **guvcs10gd.h** para cada projeto-

