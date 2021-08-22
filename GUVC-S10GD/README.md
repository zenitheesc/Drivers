# GUVC-S10GD

## Propósito
Sensor de Radiação UV-C , que medirá a densidade de energia da radiação do tipo UV-C (`mW/cm²`) que incidir-se-á nas sondas que o contiverem.

## Ilustração
Projeto onde o GUVC-S10GD foi utilizado (módulo de UV mais à direita)
<img src="https://zenith.eesc.usp.br/images/Projetos/Hardware/VarellaAcquisitionModule.webp" title="guvc-aplication" width="500px"/>

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

Medida Acurata
```c
double uvc_radiation = guvcs10gd_accurate_get_uvcpower(guvc);
printf("radiation in  (mW/cm^2): %lf\r\n", uvc_radiation);
```
## Documentação
### Equação de conversão entre corrente e densidade de radiação apresentada no [datasheet](http://www.geni-uv.com/download/products/GUVC-S10GD.pdf):

<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\bg_white&space;.\;&space;\rho_{uvc}&space;\;(mW/cm^{2})&space;=&space;i_{out}\;(nA)&space;\cdot&space;(1/78)&space;\;." title="\bg_white .\; \rho_{uvc} \;(mW/cm^{2}) = i_{out}\;(nA) \cdot (1/78) \;." />

### Equação de conversão genérica entre tensão de saída e corrente emitida pelo sensor:

<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\bg_white&space;.\;&space;i_{out}&space;=&space;A_{i}&space;\cdot&space;(\frac{U_{out}&space;-&space;b_{u}}{A_{u}}&space;\cdot&space;\cos(\phi)&space;&plus;&space;\sqrt{\frac{{A_{u}}^{2}&space;-&space;{(U_{out}&space;-&space;b_{u})}^{2}}{{A_{u}}^{2}}}&space;\cdot&space;\sin(\phi))&space;&plus;&space;b_{i}&space;\;." title="\bg_white .\; i_{out} = A_{i} \cdot (\frac{U_{out} - b_{u}}{A_{u}} \cdot \cos(\phi) + \sqrt{\frac{{A_{u}}^{2} - {(U_{out} - b_{u})}^{2}}{{A_{u}}^{2}}} \cdot \sin(\phi)) + b_{i} \;." />

Essa equação, deduzida e explicada com maior riqueza de detalhes em um [documento]() a parte, é utilizada para converter a tensão de saída no ADC da Bluepill (STM32) na corrente de saída do `GUVC-S10GD`. Para esssa conversão, faz-se necessário inserir as informações da curva de calibração do `GUVC-S10GD` na configuração da struct do tipo `guvcs10gd_t` exemplificada na sessão `Setup` do tópico acima.

### Equação de conversão específica entre tensão de saída e corrente emitida pelo sensor:

<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\bg_white&space;.\;&space;i_{out}&space;=&space;(\frac{A_{i}}{A_{u}})&space;\cdot&space;(U_{out}&space;-&space;b_{u})&space;\cdot&space;(\lambda_{phase})&space;&plus;&space;b_{i}&space;\;." title="\bg_white .\; i_{out} = (\frac{A_{i}}{A_{u}}) \cdot (U_{out} - b_{u}) \cdot (\lambda_{phase}) + b_{i} \;." />

Essa equação pode ser utilizada para substituir a equação genérica em algumas situações. Como já foi abordado, a Bluepill (ou STM32) não foi projetada para trabalhar com números de "ponto flutuante". Assim, a fim de exigir menos poder de processamento da Bluepill em relação à expressão genérica, essa equação, implementada na função `guvcs10gd_get_uvcpower` desta biblioteca, apesar de retornar um `float`, abre mão de raizes quadras e funções seno ou cosseno, exigindo bem menos poder de processamento da Bluepill. Entretanto, para conseguir se livrar dessas funções da `math.h`, a equação só leva em consideração os casos onde os gráficos de tensão e corrente estão em perfeita oposição (`lambda = -1`) ou conjunção de fases (`lambda = 1`). Contudo, vale o adendo de que é possível utilizar essa função em situações "não ideais", porém os resultados não serão os mais precisos possíveis.

## Configuração do guvcs10gd.h
Devido à limitações de corrente emitidas pelo sensor GUVC-S10GD, faz-se necessário o uso de amplificadores/conversores para tornar o sinal, da ordem de nano-amperes, emitido por ele em um sinal capaz de ser lido.

Portanto, visto a variedade de circuitos que podem ser empregados para essa finalidade, foi estabelecido que para cada projeto que pretenda usar o GUVC-S10GD, será necessário configurar a sessão **"CONVERSION VALUES THAT VARY BETWEEN PROJECTS - AMPLIFIER"** do **guvcs10gd.h** com informações da curva de calibração do amplificador/conversor para o referido sensor.

Uma curva de calibração genérica está apresentada abaixo, mais detalhes sobre a relação da sessão do **"CONVERSION VALUES THAT VARY BETWEEN PROJECTS - AMPLIFIER"** do **guvcs10gd.h** com a curva de calibração podem ser acessadas no pdf que descreve a [equação de conversão](https://github.com/CarlosCraveiro/Drivers/blob/GUVC-S10GD/GUVC-S10GD/extra_doc/Dedution_of_Conversion_Equation.pdf).
<img src="https://github.com/CarlosCraveiro/Drivers/blob/GUVC-S10GD/GUVC-S10GD/extra_doc/tension_current_relation_1.png" title="curva de calibração" height="500px"/>

## Notas
- Para resultados aproximados da radiação UVC, utilize a função `guvcs10gd_get_uvcpower`.
- Para resultados mais precisos da radiação UVC, utilize a função `guvcs10gd_accurate_get_uvcpower`, mas saiba que ela exigirá contas utilizandon `double`.
- Para mais informações sobre as equações de conversão de tensão em corrente [clique aqui]().
