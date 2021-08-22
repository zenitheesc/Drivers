# GUVC-S10GD

## Propósito
Sensor de Radiação UV-C , que medirá a densidade de radiação do tipo UV-C (`mW/cm²`) que incidir-se-á nas sondas que o contiverem.

## Exemplo
Setup
```c
guvcs10gd_t guvc = {
	.adc = &hadc1,
	.converter.current.amplitude = 200000, //unit => pico-amperes
	.converter.current.offset = 200000,    //unit => pico-amperes
	.converter.tension.amplitude = 1500,   //unit => mili-volts
	.converter.tension.offset = 1500,      //unit => mili-volts
	.converter.phase_difference = 0        //unit => degrees
};
guvcs10gd_init(guvc);
```

Medida Simples
```c
float uvc_radiation = guvcs10gd_get_uvcpower(guvc);
printf("radiation in  (mW/cm^2): %f\r\n", uvc_radiation);
```

Media Acurata
```c
double uvc_radiation = guvcs10gd_accurate_get_uvcpower(guvc);
printf("radiation in  (mW/cm^2): %lf\r\n", uvc_radiation);
```
## Documentação
Equação de conversão entre corrente e densidade de radiação apresentada no [datasheet](http://www.geni-uv.com/download/products/GUVC-S10GD.pdf):
<br>
<br>
<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\bg_white&space;.\;&space;\rho_{uvc}&space;\;(mW/cm^{2})&space;=&space;i_{out}\;(nA)&space;\cdot&space;(1/78)&space;\;." title="\bg_white .\; \rho_{uvc} \;(mW/cm^{2}) = i_{out}\;(nA) \cdot (1/78) \;." />

Equação de conversão genérica entre tensão de saída e corrente emitida pelo sensor:
<br>
<br>
<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\bg_white&space;.\;&space;i_{out}&space;=&space;A_{i}&space;\cdot&space;(\frac{U_{out}&space;-&space;b_{u}}{A_{u}}&space;\cdot&space;\cos(\phi)&space;&plus;&space;\sqrt{\frac{{A_{u}}^{2}&space;-&space;{(U_{out}&space;-&space;b_{u})}^{2}}{{A_{u}}^{2}}}&space;\cdot&space;\sin(\phi))&space;&plus;&space;b_{i}&space;\;." title="\bg_white .\; i_{out} = A_{i} \cdot (\frac{U_{out} - b_{u}}{A_{u}} \cdot \cos(\phi) + \sqrt{\frac{{A_{u}}^{2} - {(U_{out} - b_{u})}^{2}}{{A_{u}}^{2}}} \cdot \sin(\phi)) + b_{i} \;." />

## Configuração do guvcs10gd.h
Devido à limitações de corrente emitidas pelo sensor GUVC-S10GD, faz-se necessário o uso de amplificadores/conversores para tornar o sinal, da ordem de nano-amperes, emitido por ele em um sinal capaz de ser lido.

Portanto, visto a variedade de circuitos que podem ser empregados para essa finalidade, foi estabelecido que para cada projeto que pretenda usar o GUVC-S10GD, será necessário configurar a sessão **"CONVERSION VALUES THAT VARY BETWEEN PROJECTS - AMPLIFIER"** do **guvcs10gd.h** com informações da curva de calibração do amplificador/conversor para o referido sensor.

Uma curva de calibração genérica está apresentada abaixo, mais detalhes sobre a relação da sessão do **"CONVERSION VALUES THAT VARY BETWEEN PROJECTS - AMPLIFIER"** do **guvcs10gd.h** com a curva de calibração podem ser acessadas no pdf que descreve a [equação de conversão](https://github.com/CarlosCraveiro/Drivers/blob/GUVC-S10GD/GUVC-S10GD/extra_doc/Dedution_of_Conversion_Equation.pdf).
![curva de calibração](https://github.com/CarlosCraveiro/Drivers/blob/GUVC-S10GD/GUVC-S10GD/extra_doc/tension_current_relation_1.png)

## Notas
- Faz se necessário pré-configurar o **guvcs10gd.h** para cada projeto

