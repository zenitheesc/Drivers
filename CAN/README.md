# CANZenTool

## Objective
This library was designed to help Zenith's Members to use/deal with `stm32f1xx_hal_can.h`, the HAL's `Hardware Abstraction Layer` "sub library" to deal with CAN `Controlled Area Network` Protocol.

## Resume

This library provides functions to help the user to:

<ol>
    <li> Find the best Id and Mask for the Filter (CANZenTool_IdNdMaskBuilder_t)</li>
    <li> Configure the filter (CANZenTool_setFilter) </li>
    <li> Write a standard can frame (CANZenTool_writeStdCanFrame) </li>
    <li> Send a standard can frame (CANZenTool_sendCanFrameMsg) </li>
</ol>


### Important - 1

This library is not over yet!! It is an ongoing project that will evolve as long we (Zenith's Members) continue to work with CAN. So the idea is to look for our bare necessities, which means, when new opportunities to avoid code duplication and reduce painful and unnecessary journeys to H.A.L and BluePill documentation pop up, then we will come up with new features. So forget about your worries and your strife with code, when new bare necessities arise, the new features will come to you. They'll come to you!!

### Important - 2

Maybe it's obvious, but this library depends on the libraries `"stm32f1xx_hal_can.h"` and `"stm32f1xx_hal_def.h"`. So you will need them to use this library.

## Function `CANZenTool_setFilter`

This function sets the configuration parameters passed in the function call and other standard settings in a `CAN_FilterTypeDef` named `canFilterConfigs`. The "other standard settings" refers to no required parameters in the function call, but that does not vary between Zenith's projects, so they are defaulted set inside the function.


### Sample

```c
/* Filter Configs */

bool wholeFilterIsActive = true;

CAN_FilterTypeDef canFilterConfig;

uint32_t filterBank = 0;

CANZenTool_IdNdMaskBuilder_t builder = CANZenTool_newIdNdMaskBuilder();

uint32_t ids[] = { 0xF2, 0x55 };
builder.addIdList(&builder, 2, ids);
uint32_t filterId = builder.getResultId(&builder);

uint32_t filterMaskId = builder.getResultId(&builder);

/* The Function */
CANZenTool_setFilter(&hcan, &canFilterConfig, wholeFilterIsActive, filterBank, filterId, filterMaskId);

/* An Important Interrupt */
HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING); //Remember to activate this Interrupt!!
```
## Function `CANZenTool_writeStdCanFrame`

This function pre-configures a standard CAN Frame with the specifications inserted in the function call, such as the length of the CAN Frame (DLC - the first parameter), the ID of the frame (second parameter) and if it is a data frame or not (last parameter).

### Sample
```c
uint32_t mailbox;

/*...  some code ...*/

CAN_TxHeaderTypeDef header = CANZenTool_writeStdCanFrame(8, 0xE1, true);
uint8_t buffer[8] = { 0, [3]=0xFA };

/*... some code ...*/

/*... CAN Start ...*/ //CAN also can be started before, but not after any CANZenTool_sendFrameMsg() call

CANZenTool_sendCanFrameMsg(&hcan, &header, buffer, &mailbox);
```

## Function `CANZenTool_sendFrameMsg`

This function in few words helps the user to send a Standard CAN Frame, already calling the ``Error_Handler`` in failure case.

### Sample
```c
uint32_t mailbox;

/*...  some code ...*/

CAN_TxHeaderTypeDef header = CANZenTool_writeStdCanFrame(8, 0xE1, true);
uint8_t buffer[8] = { 0, [3]=0xFA };

/*... some code ...*/

/*... CAN Start ...*/ //CAN also can be started before, but not after any CANZenTool_sendFrameMsg() call

CANZenTool_sendCanFrameMsg(&hcan, &header, buffer, &mailbox);
```

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

This function is a "getter" for the result ``filterMask``.

Note: this function can be both used as a "method" of the "builder" structure or as a stand-alone function.

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

This function is a "getter" for the result ``filterId``.

Note: this function can be both used as a "method" of the "builder" structure or as a stand-alone function.

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

## Full Sample

There is one tested-working sample (created with the **"STM-CUBEIDE"**) ``main.c`` in a folder named as "CUBEIDE-sample", if you are having problems with some of the provided tools, you should check the code and see if it clarifies something.

<!-- links -->

[Builder Design Pattern]: https://refactoring.guru/design-patterns/builder "Refactoring Guru - Builder Design Pattern"
