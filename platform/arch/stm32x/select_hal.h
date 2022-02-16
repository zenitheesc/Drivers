/**
 * @file select_hal.h
 * @author Leonardo Celente (@leocelente)
 *
 * @version 0.1
 * @date 2022-02-16
 *
 * @copyright Copyright (c) 2022
 *
 * Esse arquivo, seleciona entre os arquivos gerados da HAL
 * quando a CubeIDE gera código, ela utiliza um arquivo associado a
 * familia do STM32. Além disso todos os arquivos compilados tem uma macro
 * definida indicando essa familia.
 */
#ifndef STM_HAL_H
#define STM_HAL_H

#if defined STM32F103xB
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
