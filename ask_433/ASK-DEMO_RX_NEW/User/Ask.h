#ifndef     __ASK_H_
#define     __ASK_H_

#include "All_Includes.h"

#define max_time_h       60      //宽脉冲最大允许时间 data*TCC_time
#define min_time_h       16      //宽脉冲最小允许时间
#define max_time_l       14      //窄脉冲最大允许时间
#define min_time_l       4      //窄脉冲最小允许时间

//#define max_time_h       40      //宽脉冲最大允许时间 data*TCC_time
//#define min_time_h       18      //宽脉冲最小允许时间
//#define max_time_l       15      //窄脉冲最大允许时间
//#define min_time_l       6      //窄脉冲最小允许时间
   
#define     RECV_BIT_NUMBER     32       //设定接收的位数
#define     inport              PD_IDR_IDR3
//#define       ASK             PD_IDR_IDR4

void Ask_Init();
void Ask_process();
void Recieve();
void RecieveError();
void ProcessRecv();
void Learn_Sender();
void ReadSelfAddr();
void Dele_Sender();
void ProcessOut();
void Write_Coder();

#endif
