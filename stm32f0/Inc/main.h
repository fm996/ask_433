/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
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
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define THERM_Pin           GPIO_PIN_1
#define THERM_GPIO_Port     GPIOA
#define PROJ_ON_Pin         GPIO_PIN_10
#define PROJ_ON_GPIO_Port   GPIOA
#define IR_IN_Pin           GPIO_PIN_1
#define IR_IN_GPIO_Port     GPIOB
#define LED_Pin             GPIO_PIN_2
#define LED_GPIO_Port       GPIOB

#define USART1_GPIO_Port    GPIOA
#define USART1_TX_Pin       GPIO_PIN_9
#define USART1_RX_Pin       GPIO_PIN_10

#define I2C2_GPIO_Port      GPIOB
#define I2C2_SCL_Pin        GPIO_PIN_8
#define I2C2_SDA_Pin        GPIO_PIN_9
#define I2C1_SCL_Pin        GPIO_PIN_8
#define I2C1_SDA_Pin        GPIO_PIN_9

#define INT_Pin             GPIO_PIN_5
#define INT_GPIO_Port       GPIOB

/*
#define GYRO_INT2_Pin GPIO_PIN_4
#define GYRO_INT2_GPIO_Port GPIOB
#define GYRO_INT1_Pin GPIO_PIN_5
#define GYRO_INT1_GPIO_Port GPIOB
*/

#define HWI2C 0

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif

#define DEBUG_ENABLE                        1
#define LOG_ENABLE                          1

#if DEBUG_ENABLE
//#define LOG_DEBUG(format, args...)    do { \
//                                        printf(format, ##args);\
//                                      } while(0)
#define LOG_DEBUG                       Drv_SERIAL_Log

#else
#define LOG_DEBUG(format, args...)    do {\
                                      } while(0)
#endif

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
