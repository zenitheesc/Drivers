#ifndef STM_HAL_H
#define STM_HAL_H

#if defined STM32F103xx
#include "stm32f1xx_hal.h"

#elif defined STM32F030xx
#include "stm32f0xx_hal.h"

#elif defined STM32F407xx || defined STM32F411xx
#include "stm32f4xx_hal.h"

#elif defined STM32L476xx
#include "stm32l4xx_hal.h"

#elif defined STM32G031xx
#include "stm32g0xx_hal.h"

#endif
#endif
