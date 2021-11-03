# CANZenTool

## Objective
This library was designed to help Zenith's Members to use/deal with `stm32f1xx_hal_can.h`, the HAL's `Hardware Abstraction Layer` "sub library" to deal with CAN `Controlled Area Network` Protocol.

## Resume

### Important - 1

This library is not over yet!! It is an ongoing project that will evolve as long we (Zenith's Members) continue to work with CAN. So the idea is to look for our bare necessities, which means, when new opportunities to avoid code duplication and reduce painful and unnecessary journeys to H.A.L and BluePill documentation pop up, then we will come up with new features. So forget about your worries and your strife with code, when new bare necessities arise, the new features will come to you. They'll come to you!!

### Important - 2

Maybe it's obvious, but this library depends on the libraries `"stm32f1xx_hal_can.h"` and `"stm32f1xx_hal_def.h"`. So you will need them to use this library.

## Function `CANZenTool_setFilter`

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
## Function `CANZenTool_writeStdCanFrame`

### Sample

## Function `CANZenTool_sendFrameMsg`

### Sample

## Struct `CANZenTool_IdNdMaskBuilder_t`

This structure and the functions below from `CANZenTool_addId` to `CANZenTool_newFilterBuilder`, together try to follow the **[Builder Design Pattern]**.

The idea is providing for the user a tool to create the **FilterId** and the **FiltertMask** (required by `CANZenTool_setFilter`) based on the Standard CAN Ids that the user wants to allow pass the filter.

## Function `CANZenTool_newIdNdMaskBuilder`

### Sample
```c
CANZenTool_IdMask_t builder = newIdNdMaskBuilder();

```


## Function `CANZenTool_addId`

This function "adds" a **standard CAN Id** to the "builder" referred as "this". The addition "re-calculates" the new Id and Mask inside the "builder" structure based in the income Id. 

Note: this function can be both used as a "method" of the "builder" structure or as a stand-alone function.


### Sample - Stand-alone
```c
CANZenTool_IdNdMaskBuilder_t builder = newIdNdMaskBuilder();

CANZenTool_addId(&builder, 0x0D0);

CANZenTool_addId(&builder, 0x0D1);

CANZenTool_addId(&builder, 0x0D2);

CANZenTool_addId(&builder, 0x0D3);

CANZenTool_addId(&builder, 0x0D4);
```


### Sample - Builder-method

```c
CANZenTool_IdNdMaskBuilder_t builder = newIdNdMaskBuilder();

builder.addId(&builder, 0x0D0);

builder.addId(&builder, 0x0D1);

builder.addId(&builder, 0x0D2);

builder.addId(&builder, 0x0D3);

builder.addId(&builder, 0x0D4);
```



## Function `CANZenTool_addIdList`

This function "adds" a list of **standard CAN Ids** to the "builder" referred as "this". The addition "re-calculates" the new Id and Mask inside the "builder" structure based in the income Ids. 

Note: this function can be both used as a "method" of the "builder" structure or as a stand-alone function.


### Sample - Stand-alone
```c
CANZenTool_IdNdMaskBuilder_t builder = newIdNdMaskBuilder();


uint32_t list[] =   {0x0C0, 0x0C1, 0x0C2, 0x0C3, 0x0C4, 0x0C5, 0x0C6, 0x0C7
                    ,0x0D0, 0x0D1, 0x0D2, 0x0D3, 0x0D4
                    ,0x0F0, 0x0F1, 0x0F2, 0x0F3, 0x0F4, 0x0F5, 0x0F6, 0x0F7};
    
CANZenTool_addIdList(&builder, 21, list);

```


### Sample - Builder-method

```c
CANZenTool_IdNdMaskBuilder_t builder = newIdNdMaskBuilder();


uint32_t list[] =   {0x0C0, 0x0C1, 0x0C2, 0x0C3, 0x0C4, 0x0C5, 0x0C6, 0x0C7 
                    ,0x0D0, 0x0D1, 0x0D2, 0x0D3, 0x0D4
                    ,0x0F0, 0x0F1, 0x0F2, 0x0F3, 0x0F4, 0x0F5, 0x0F6, 0x0F7};
    
builder.addIdList(&builder, 21, list);

```


## Function `CANZenTool_getResultMask`

### Sample - Stand-alone
```c
CANZenTool_IdNdMaskBuilder_t builder = newIdNdMaskBuilder();

CANZenTool_addId(&builder, 0x0D0);

CANZenTool_addId(&builder, 0x0D1);

uint32_t filterMask = CANZenTool_getResultMask();


```


### Sample - Builder-method

```c
CANZenTool_IdNdMaskBuilder_t builder = newIdNdMaskBuilder();

builder.addId(&builder, 0x0D0);

builder.addId(&builder, 0x0D1);

uint32_t filterMask = builder.getResultMask();
```


## Function `CANZenTool_getResultId`

### Sample - Stand-alone
```c
CANZenTool_IdNdMaskBuilder_t builder = newIdNdMaskBuilder();

CANZenTool_addId(&builder, 0x0D0);

CANZenTool_addId(&builder, 0x0D1);

uint32_t filterId = CANZenTool_getResultId();
```


### Sample - Builder-method

```c
CANZenTool_IdNdMaskBuilder_t builder = newIdNdMaskBuilder();

builder.addId(&builder, 0x0D0);

builder.addId(&builder, 0x0D1);

uint32_t filterId = builder.getResultId();
```

<!-- links -->

[Builder Design Pattern]: https://refactoring.guru/design-patterns/builder "Refactoring Guru - Builder Design Pattern"
