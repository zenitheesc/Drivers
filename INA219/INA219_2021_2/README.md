# Driver_INA219_MVD
Repositório para confecção do driver mínimo para funcionamento do sensor INA219 a ser utilizada nos projetos de Baixo Nível do zenith

## Função
Sensor com função de medição da tensão e corrente em seus terminais.
## Exemplo

SETUP:
~~~C
//configurações a serem utilizadas no sensor
INA219_config_t config = {
    .BusVoltageRange = INA_BRANGE_16,
    .ShuntVoltageRange = INA_SRANGE_40,
    .BusADCResolution = INA_BADC1_12bit,
    .ShuntADCResolution = INA_SADC1_12bit,
    .OperationMode = INA_CONTINUOUS
}

//configuração do i2c
//configurar o endereço de acordo com o utilizado no hardware
i2c_device_t dev = {
    .i2c = &hi2c1,
    .adress = 0x40
}

INA219_t ina = {
    .config = config,
    .device = dev
}

INA219_reset(ina);
INA219_config(ina);

INA219_values_t medida;

~~~

MEDIDA:

~~~C

INA219_measure(ina, &medida);
printf("Tensão: %f\r\n", medida.Bus_Voltage);
printf("Corrente: %f\r\n", medida.Shunt_Current)

~~~

## Funcionamento
Este driver foi feito utilizando somente as funções mínimas necessárias para ser testado em tempo hábil, por este motivo não utiliza todas as funções disponíveis do sensor configuradas, utilizando assim as configurações default onde possível; para as configurações estão configuradas as configurações de faixa de valores de tensão no bus, para 16 V, e faixa de valores para o shunt de 40 mV, além disso a resolução do shunt e do bus como 12 bits, com tempo de 532 us.

## Funções
As funções estão organizadas como mostradas no exemplo, com a função de reset, para reiniciar o sensor e suas configurações, config, para realizar as configurações estabelecidas na struct de configurações, no .h do driver há outras opções de valores que podem ser utilizados nas configurações, caso haja a necessidade de modificar algum modo de operação, como o modo de resolução dos ADCs, e as faixas de valores de operação, mais detalhes no .h do driver, as conversões das leituras feitas para o valor em tensão em corrente seguem como mencionados no [datasheet](https://www.ti.com/lit/ds/symlink/ina219.pdf), na seção 8.6.3.

### Operações
Toda vez que utilizada a função `INA219_measure` são feitas as leituras nos bancos de registradores de cada seção (Shunt e Bus) e convertidos para valores de tensão e corrente, cada leitura ocorre, segundo o datasheet, a cada 532 us, como são feitas duas leituras (Shunt e Bus), é necessário o dobro do tempo para a atualização dos dados de tensão e corrente medidas.

## Notas
Segundo o datasheet, para evitar conflitos com outros componentes i2c, há a possibilidade de mudar o endereço i2c do INA219, como segue na seção 8.5.5.1 tabela 1, com 16 endereços possíveis com a combinação dos pinos A0 e A1.