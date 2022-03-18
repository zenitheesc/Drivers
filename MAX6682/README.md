# MAX6682

## Propósito
Interface externa de termistores. O MAX6682 permite o controle da região de
linearidade da relação entre resistência e temperatura de um termistor.


## Exemplo
```c
spi_device_t spi = { .spi = &hspi1, 			// SPI Handle
    .pin = { .port = MAX6682_CS_GPIO_Port, 	// CS GPIO Port
        .pin = MAX6682_CS_Pin },		// CS GPIO Pin
    };

max6682_t max = { .dev = spi,		//
    .Rext = 7680 // External Resistance (Rext)
    };

max6682_init(&max);
result_float result = max6682_measure(&max);
if(result.hasError){
  printf("Invalid Resistance\r\n");
}
float R = result.value;
printf("Thermistor Resistance: %.2f\r\n", R);
```

## Documentação

Para determinar a resistência externa Rext utilize a formula do datasheet

![relação datasheet](https://i.imgur.com/9JAlXYt.png)

Por último o parãmetro da resistência de calibração é a resistência 
anunciada pelo termistor, isto é, a sua resistência esperada para temperatura
de 25°C.

## Notas
Não esqueça de ativar printf com floats antes de rodar o exemplo.
