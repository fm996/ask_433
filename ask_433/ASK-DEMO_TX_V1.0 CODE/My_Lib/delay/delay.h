/*************************************************************************
              ��������
���ڱ��깺����30Ԫ���´ι��򼴿ɷ���5�ǣ�
      ��50Ԫ���´ι��򼴿ɷ���1Ԫ��
     ��100Ԫ���´ι��򼴿ɷ���2Ԫ��
     ��150Ԫ���´ι��򼴿ɷ���4Ԫ��
      ��200Ԫ���´ι��򼴿�����
http://shop106001793.taobao.com/search.htm?spm=a1z10.5.w5002-3375901029.1.l1TXSl&search=y
              stm8s105
               V1.1
             2013.9.8
*************************************************************************/

#ifndef __delay_h
#define __delay_h

void delay_ms(unsigned int time);
void delay_us(void);

unsigned long GetTimer();
unsigned long SpanTime(unsigned long OldTime);
void DelayMS(unsigned long ms);

#endif