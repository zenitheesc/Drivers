# CANZenTool

## Objective
This library was designed to help Zenith's Members to use/deal with `stm32f1xx_hal_can.h`, the HAL's `Hardware Abstraction Layer` "sublibrary" to deal with CAN `Controlled Area Network` Protocol.

## Resume

### Important - 1

This library is not over yet!! It is an ongoing project that will evolve as long we (Zenith's Members) continue to work with CAN. So the idea is to look for our bare necessities, which means, when new opportunities to avoid code duplication and reduce painful and unnecessary journeys to H.A.L and BluePill documentation pop up, then we will come up with new features. So forget about your worries and your strife with code, when new bare necessities arise, the new features will come to you. They'll come to you!!

### Important - 2

Maybe it's obvious, but this library depends on the libraries `"stm32f1xx_hal_can.h"` and `"stm32f1xx_hal_def.h"`. So you will need them to use this library.

## Function `CANZenTool_setFilter()`

This function sets the configuration parameters passed in the function call and other standard settings in a `CAN_FilterTypeDef` named `canFilterConfigs`. The "other standard settings" refers to no required parameters in the function call, but that does not vary between Zenith's projects, so they are defaulted set inside the function.


### Sample

Setting just one Filter Bank
```c

/* General Configs */

CAN_HandleTypeDef hcan;

bool wholeFilterIsActive = true;

/* Filter Configs */

CAN_FilterTypeDef canFilterConfig;

uint32_t filterBank = 10;

uint32_t filterId = 0x103;

uint32_t filterMaskId = 0x101;


/* The Function */

CANZenTool_setFilter(&hcan, &canFilterConfig , wholeFilterIsActive, filterBank, filterId, filterMaskId);

```

Setting more than one Filter Bank
```c
/* General Configs */

CAN_HandleTypeDef hcan;

bool wholeFilterIsActive = true;


/*  */

CAN_FilterTypeDef canFilterConfig1;

uint32_t filterBank1 = 10;

uint32_t filterId1 = 0x102;

uint32_t filterMaskId1 = 0x101;

/*  */
CANZenTool_setFilter(&hcan, &canFilterConfig1 , wholeFilterIsActive, filterBank1, filterId1, filterMaskId1);


/*  */


/**/

CANZenTool_setFilter(&hcan, &canFilterConfig2, );

```
## Function `CANZenTool_writeStdCanFrame()`

### Sample

## Function `CANZenTool_sendFrameMsg()`

### Sample

