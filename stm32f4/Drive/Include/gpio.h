#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

/*---------------------- GPIO配置宏 ------------------------*/

#define DCLK_PIN          GPIO_Pin_5             // MBI dclk 引脚
#define GCLK_PIN          GPIO_Pin_9             // MBI gclk 引脚
#define LE_PIN            GPIO_Pin_7             // MBI LE 引脚
#define SDI_PIN           GPIO_Pin_8             // MBI SDI 引脚
#define MBI_PORT          GPIOE                  // MBI驱动 GPIO端口
#define MBI_CLK           RCC_AHB1Periph_GPIOE   // LED1 GPIO端口时钟

#define AG_OE_PIN         GPIO_Pin_1             // AG OE 引脚
#define AG_CLK_PIN        GPIO_Pin_2             // AG Clk 引脚
#define AG_DIN_PIN        GPIO_Pin_3             // AG Din 引脚


/*---------------------- GPIO控制宏 ------------------------*/

#define DCLK_PIN_H        MBI_PORT->BSRRL = DCLK_PIN;       // 输出高电平
#define DCLK_PIN_L        MBI_PORT->BSRRH = DCLK_PIN;       // 输出低电平
#define LE_PIN_H          MBI_PORT->BSRRL = LE_PIN;         // 输出高电平
#define LE_PIN_L          MBI_PORT->BSRRH = LE_PIN;         // 输出低电平
#define SDI_PIN_H         MBI_PORT->BSRRL = SDI_PIN;        // 输出高电平
#define SDI_PIN_L         MBI_PORT->BSRRH = SDI_PIN;        // 输出低电平

#define GCLK_PIN_H        MBI_PORT->BSRRL = GCLK_PIN;       // 输出高电平
#define GCLK_PIN_L        MBI_PORT->BSRRH = GCLK_PIN;       // 输出低电平

#define AG_CLK_PIN_H      MBI_PORT->BSRRL = AG_CLK_PIN;     // 输出高电平
#define AG_CLK_PIN_L      MBI_PORT->BSRRH = AG_CLK_PIN;     // 输出低电平
#define AG_DIN_PIN_H      MBI_PORT->BSRRL = AG_DIN_PIN;     // 输出高电平
#define AG_DIN_PIN_L      MBI_PORT->BSRRH = AG_DIN_PIN;     // 输出低电平
#define AG_OE_PIN_H       MBI_PORT->BSRRL = AG_OE_PIN;      // 输出高电平
#define AG_OE_PIN_L       MBI_PORT->BSRRH = AG_OE_PIN;      // 输出低电平

/*---------------------- 函数声明 ----------------------------*/

void MBI_GPIO_Init(void);	//LED初始化函数


#endif //__GPIO_H

