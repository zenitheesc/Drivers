# CANZenTool

## Objective
This library was designed to help Zenith's Members to use/deal with `stm32f1xx_hal_can.h`, the HAL's `Hardware Abstraction Layer` "sublibrary" to deal with CAN `Controlled Area Network` Protocol.

## Resume

### Important

This library is not over yet!! It is an ongoing project that will evolve as long we (Zenith's Members) continue to work with CAN. So the idea is to look for our bare necessities, which means, when new opportunities to avoid code duplication and reduce painful and unnecessary journeys to H.A.L and BluePill documentation pop up, then we will come up with new features. So forget about your worries and your strife with code, when new bare necessities arise, the new features will come to you. They'll come to you!!

## Function CANZenTool_setFilter()

### Sample
Setup
```c
guvcs10gd_t guvc = {
	.adc = &hadc1,
	.converter.current.amplitude = 200000, //unit => pico-amperes
	.converter.current.offset = 200000,    //unit => pico-amperes
	.converter.tension.amplitude = 1500,   //unit => mili-volts
	.converter.tension.offset = 1500,      //unit => mili-volts
	.converter.phase_difference = 0        //unit => degrees
};
guvcs10gd_init(guvc);
```

Medida Simples
```c
float uvc_radiation = guvcs10gd_get_uvcpower(guvc);
printf("radiation in  (mW/cm^2): %f\r\n", uvc_radiation);
```

Medida Acurata
```c
double uvc_radiation = guvcs10gd_accurate_get_uvcpower(guvc);
printf("radiation in  (mW/cm^2): %lf\r\n", uvc_radiation);
```
## Function CANZenTool_writeCanFrame()

### Sample

