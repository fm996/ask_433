/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include "stdint.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define LED_DCLK_Pin GPIO_PIN_4
#define LED_DCLK_GPIO_Port GPIOA
#define LED_HOLD_Pin GPIO_PIN_5
#define LED_HOLD_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_6
#define SW1_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_7
#define SW2_GPIO_Port GPIOA
#define LED_LEFT_Pin GPIO_PIN_0
#define LED_LEFT_GPIO_Port GPIOB
#define LED_DOWN_Pin GPIO_PIN_1
#define LED_DOWN_GPIO_Port GPIOB
#define LED_UP_Pin GPIO_PIN_10
#define LED_UP_GPIO_Port GPIOB
#define LED_RIGHT_Pin GPIO_PIN_11
#define LED_RIGHT_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_12
#define LED1_GPIO_Port GPIOB
#define INT_Pin GPIO_PIN_5
#define INT_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define DEBUG_ENABLE                1                   //debug输出开启
#define SYS_FREQUENCY       		32000000    		//32MHz主频
#define LOG_ENABLE					0					//使能输出记录


#if DEBUG_ENABLE
#define LOG_DEBUG(format, args...)    do { \
                                        printf(format, ##args);\
                                      } while(0)
#else
#define LOG_DEBUG(format, args...)    do {\
                                      } while(0)
#endif  
									  
/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
