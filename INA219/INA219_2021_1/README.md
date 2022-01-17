# INA219

## Propósito
Sensor capaz de medir a corrente e tensão passando por Vin- e Vin+

## Exemplo
Setup
```c
ina219_config_t config = { 
        .mode = INA_Cont_Shunt_Bus_voltage, 
        .shuntADCResolution = INA_SMode_12bit, 	
        .busADCResolution = INA_BMode_12bit, 	
        .shuntVoltageRange = INA_Range_40, 		
        .busVoltageRange = 0x0000 
};
i2c_device_t dev = { 
        .i2c = &hi2c1, 
        .address = 0x40 
};
ina219_t ina = { .config = config, .device = dev };
ina219_reset(ina);
ina219_config(ina);
ina219_calibrate(&ina, INA219_FULL_16V_40);
ina219_values_t valor;
```

Medida:
```c
ina219_measure(ina, &valor);
printf("Voltage: %f\r\n", valor.Bus_Voltage);
printf("Current: %f\r\n", valor.Current);
printf("Power: %f\r\n", valor.Power);
printf("Shunt: %f\r\n", valor.Shunt_Voltage);
```
## Documentação
 Como estão organizadas as funções?

## Notas
**_Ainda não está funcionando a medição da Potencia ou Corrente. _**
