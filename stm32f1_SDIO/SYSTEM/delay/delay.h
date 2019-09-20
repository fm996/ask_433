/**********************************************************
* @ File name -> delay.h
* @ Version   -> V1.0
* @ Date      -> 12-26-2013
* @ Brief     -> ϵͳ��ʱ��صĺ���ͷ�ļ�

* @ ��ϸ˵����ο���Cortex-M3Ȩ��ָ��(����)����133ҳ ��8�� SysTick��ʱ������
**********************************************************/

#ifndef _delay_h_
#define _delay_h_

/**********************************************************
                     �ⲿ����ͷ�ļ�                        
**********************************************************/

#include "sys.h"

/**********************************************************
                     �ⲿ���ܺ���
**********************************************************/

void delay_init(uint8_t SYSCLK);	//��ʼ����ʱ����
void delay_us(uint32_t nus);	//��ʱn��us
void delay_ms(uint16_t nms);	//��ʱn��ms


#endif
