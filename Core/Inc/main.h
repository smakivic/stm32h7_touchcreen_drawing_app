/**
  ******************************************************************************
  * @file    BSP/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module for Cortex-M7.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "stm32h7xx_hal.h"
#include "stm32h750b_discovery.h"
#include "stm32h750b_discovery_lcd.h"
#include "stm32h750b_discovery_ts.h"
#include "stm32_lcd.h"

/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LED_GREEN      LED1
#define LED_RED        LED2
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Touchscreen_demo(void);
void Error_Handler(void);
void QSPI_demo (void);
void MMC_demo (void);
void DMA2_Stream0_IRQHandler(void);
void DMA2_Stream5_IRQHandler(void);
uint8_t CheckForUserInput(void);
void Toggle_Leds(void);
void Touchscreen_DrawBackground_Circles(uint8_t state);
uint8_t TouchScreen_GetTouchPosition(void);

#endif /* __MAIN_H */

