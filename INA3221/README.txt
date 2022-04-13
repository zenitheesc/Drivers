Inicialização / Reset:

ina3221_init(ina);

A função "init" inicializa o dispositivo resetando o bit de configuração. Assim,
o ina é configurado como:
- 3 channels
- avg mode: 1 sample
- shunt/bus conversion time: 1.1ms (cada)
- operating mode: shunt/bus continuous

OBS: É necessário criar a struct "ina3221_config_t" para utilizar a função init.


ina3221_alive(ina);

Retorna falso se posssui um erro (Die ID)

OBS: a função "alive" é utilizada dentro da "init".


Setup:

ina3221_config(ina);

Configura o dispositivo de acordo com a struct referenciada.

OBS: avg mode: a quantidade de samples é dada no tipo 4^(n).
Onde "n" é o parâmetro em binário (3 algarismos).

Ex: 010, 16 samples.


Medida:

ina3221_mensurement(ina, *values);

Retorna os valores da medida parra a struct referenciada na função.

Ex: printf("corrente ch1: %d", values.ch1_current);
    printf("potência ch1: %d", values.ch1.pot);