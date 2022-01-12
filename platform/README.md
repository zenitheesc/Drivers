#  Platform.h
## Objective

Platform headers is an abstraction layer to help application drivers to be
platform agnostic. That means that including the header will provide a
common interface between our embedded platforms such as STM32 (HAL) or 
Arduino. 

## Scope

Each platform implementation provides access to common (internal) peripherals,
such as I2C, SPI, UART and ADCs. As it's expected to be implemented in various 
circumstances, the scope is limited to transmitting and receiving raw bytes.

## Specification

### Common

#### Types
 - `error_t`
 ```c
  typedef int error_t;
 ```
 This is a common error code type, it is recommended to be an underlying `int` but can also be a platform specific. The requirement is that 
 evaluation of no error is false. So essentially, that the following pattern
 still applies:
 ```c
  if(error){
    // error handling...
  }
  // ... code assuming no errors
 ``` 
This pattern was chosen to prevent [Indent Hadouken](https://i.pinimg.com/originals/98/bd/38/98bd38321fad0e8b14ebf3a5a9ec70c9.jpg)
 
 - `buffer_view_t`
```c
typedef struct {
	uint8_t *data;
	int size;
} buffer_view_t;
```

It's used to pass pointers to `data` buffers of `size`. It is used throughout 
this header to pass a bunch of bytes. This was chosen to keep things simple 
and prevent mixing up sizes and arrays.
 
 - `result{8,16,float}_t`
```c
typedef struct {
	error_t hasError;
	uint8_t value;
} result8_t;

```

The `results` are a complement to the error handling strategy. Because 
there are no parameters in C, the platform header only creates result types
for common types: `uint8_t`, `uint16_t` and `float`. Other result types
should be declared on a driver-by-driver basis.


#### Functions
 - `delay_ms`
 ```c
  void delay_ms(uint32_t time);
 ```
 A blocking millisecond delay. It is recommended that RTOS use be 
 detected via preprocessor macros, so that drivers can take advantage.

### GPIO

#### Types

- `i2c_t`
 ```c
  typedef int i2c_dev_t;
 ```
 Erro

- `i2c_dev_t`
 ```c
  typedef int i2c_dev_t;
 ```
 Erro
 
 #### Functions

```c
```


### I2C

#### Types

- `i2c_t`
 ```c
  typedef int i2c_dev_t;
 ```
 Erro

- `i2c_dev_t`
 ```c
  typedef int i2c_dev_t;
 ```
 Erro
 
 #### Functions

```c
```
### SPI

#### Types
- `spi_t`
 ```c
  typedef int spi_t;
 ```
 Erro

- `spi_dev_t`
 ```c
  typedef int spi_dev_t;
 ```
 Erro
 
#### Functions

```c
```
### UART

#### Types

- `uart_t`
 ```c
  typedef int uart_t;
 ```
 Erro
 #### Functions

```c
```
### ADC

#### Types

```c
```
#### Functions

```c
```
