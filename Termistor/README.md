# INA219

## Propósito
 Pra que serve o sensor?

## Exemplo
```c
ina219_t ina = {
        .hi2c = &hi2c1,
        .address = (0x40 << 1)
};

INA_Reset(ina);

ina219_config_t config = {
        .mode = Cont_Shunt_Bus_voltage,
        .shuntADCResolution = SMode_12bit,
        .busADCResolution = BMode_12bit,
        .shuntVoltageRange = Range_320,
        .busVoltageRange = Bus_32v
};

INA_Configuration(ina, config);
INA_Calibration(ina, 1.0, 0.1);

ina219_values_t valor;
INA_Values(ina, &valor);

printf("Valor: %f %f\r\n", valor.Bus_Voltage, valor.Shunt_Voltage);

```
## Documentação
 Como estão organizadas as funções?

## Notas
 Tem alguma coisa que é bom saber antes de usar a biblioteca?
