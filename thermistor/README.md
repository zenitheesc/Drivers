# Thermistor

Biblioteca para conversão de resistência para temperatura usando termistores.

## Exemplo
```c
b_param_t params = {.beta = 3900, .cal_resistance = 10e3 };
b_param_init(&params);

float R = 8000; // thermistor resistance
result_float result = b_param_convert(&params, R);
if(result.hasError){
  printf("Invalid Temperature detected\r\n");
}

float temperature = result.value;
printf("T: %.2f C\r\n", temperature);
```

## Documentação

A intenção da biblioteca é conter multiplos métodos de conversão. 
Atualmente foi implementado o _beta-parameter_ ou _b-value_. Ele é uma simplificação
da equação de Steinhart–Hart, que é uma aproximação logarítimica de 3ª ordem da 
relação real. 

A principal simplificação além do método escolhido, é o uso de um _beta_ constante.
Alguns datasheets de termistores disponibilizam _betas_ que variam com a faixa
de temperatura escolhida. 

Geralmente o valor da resistência em 25°C é a principal especificação do termistor
o _beta_ pode ser mais dificil de encontrar.

## Referências

[Documento Interno] Controle Térmico das Baterias - Gabriel Kenji (@18950)
