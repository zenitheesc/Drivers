# MPX5700

## Propósito
Sensor de pressão para o balão

## Exemplo
Setup
```c
mpx5700_t mpx = {.adc = &hadc1};
mpx5700_init(mpx);
```

Medida:
```c
float pressao = mpx5700_get_pressure(mpx);
printf("pressao: %f\r\n", pressao);
```

## Documentação
Direta, usa a equação na Figure 2 do [datasheet](https://www.nxp.com/docs/en/data-sheet/MPX5700.pdf), invertida para 
nos dar `P`

```
 Vout = VS*(0.0012858*P+0.04) ± Error
```
### Referencia:
[https://www.fernandok.com/2018/11/conheca-agora-um-sensor-importante.html](https://www.fernandok.com/2018/11/conheca-agora-um-sensor-importante.html)


## Notas
 - **Assume ADC em Modo Continuo no STM32**
 - Biblioteca não testada (ainda)
 - Assume Vs (alimentação), de 5V
