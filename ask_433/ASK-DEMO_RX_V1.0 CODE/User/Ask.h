#ifndef		__ASK_H_
#define		__ASK_H_

#include "All_Includes.h"

#define max_time_h       60      //�������������ʱ�� data*TCC_time
#define min_time_h       16      //��������С����ʱ��
#define max_time_l       14      //խ�����������ʱ��
#define min_time_l       4      //խ������С����ʱ��

//#define max_time_h       40      //�������������ʱ�� data*TCC_time
//#define min_time_h       18      //��������С����ʱ��
//#define max_time_l       15      //խ�����������ʱ��
//#define min_time_l       6      //խ������С����ʱ��
   
#define RECV_BIT_NUMBER 24  //�趨���յ�λ��
#define inport          PC_IDR_IDR4
//#define ASK             PC_IDR_IDR4

void Ask_Init();
void Ask_process();
void Recieve();
void RecieveError();
void ProcessRecv();
void Learn_Sender();
void ReadSelfAddr();
void Dele_Sender();
void ProcessOut();

#endif