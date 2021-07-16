# NEO-8M

## Propósito
GPS capaz de funcionar na altitude das sondas

## Exemplo
Setup
```c
uart_connection_t conn = { .uart = &huart3 };
ublox_gps_t gps = { .conn = conn };
ublox_pvt_t pvt;
ublox_init(gps);
```
Leitura de PVT (_Position Velocity Time_)
```c
if (ublox_get(gps, &pvt)) {
    // caso erro, pula pacote
    continue;
}

printf("t: %u, y: %u, m: %u, d: %u, lat: %f, lng: %f\r\n",
        pvt.time / 1000, pvt.year, pvt.month, pvt.day,
        (float) pvt.lat / 1e7, (float) pvt.lng / 1e7);

```

Lembrando que o projeto deve ser configurado para mandar `float` com printf
e as syscalls também devem estar configuradas

## Documentação
### Apresentações e Justificativas
Por padrão o GPS, assim que energizado manda mensagens NMEA via UART
essas mensagens contem as informações que queremos, mas vem na forma de 
uma string de tamanho variado, o que implica uma carga e complexidade 
desnecessárias no microcontrolador. Para isso a u-blox (fabricante) disponibiliza
o protocolo alternativo (e proprietário) UBX. Pelo UBX as mensagens vem como 
sequencia de bytes contendo os dados em sequência então é mais eficiente que o NMEA.

Outra configuração que é opcional mas importante é a forma que fazemos uma medida
ao invés de deixar o GPS decidir quando pegar um dado (por padrão 1Hz), configuramos
para que ele nos de os dados em resposta a uma mensagem.

Por ultimo, mas talvez mais interessante, temos a configuração do modelo 
físico. É uma configuração que otimiza o GPS para diversos casos de uso. Um
desses casos é referenciado como High Altitude < 1G. Que significa um "veiculo"
navegando em alta altitude sem acelerações bruscas, somente com a atuação da
gravidade. O que é perfeito para sondas atmosféricas.

### Funcionamento da Biblioteca

Toda biblioteca funciona ao redor das mensagens do protocolo UBX
as funções são _wrappers_ para mensagens de interesse.

Se começarmos pela `init` vemos a sequencia de configuração
```c
silence(gps);
disableNmea(gps); 
set_protocol(gps);
set_model_mode(gps);
```
 - Paramos o envio padrão de 1Hz
 - Desabilitamos o protocolo NMEA
 - Ativamos o protocolo UBX
 - Configuramos o Modelo de Sonda

 Cada uma dessas funções representa o envio de uma ou mais mensagens 
 via UART conforme o padrão de mensagem do UBX
 
```c
{   0xB5, 0x62, // sync                 (sempre presente)
    0x01, 0x07, // class and id         (identifica mensagem)
    0x00, 0x00, // size little endian   (tamanho do payload)
    0xDE, 0xAD  // payload              (payload)
    0x00, 0x00  // checksum             (validador)
};
```

Para requisitar uma nova medição, é só usar a função `ublox_get` como no exemplo

## Notas
Tive alguns problemas de sincronia com os dados chegando. Na maioria das vezes
foi sem problemas, mas algumas vezes o inicio do pacote desalinha da struct
e nesses casos acabei desconsiderando os pacotes. Seria bom no futuro usar um
jeito mais robusto. 
