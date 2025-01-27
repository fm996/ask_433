/*********************************************************
//说明：ASK无线解码程序，解EV1527编码，最小脉宽为300~500us。使用前需要对码，即按一下按键后松开，
//      4个LED全亮后熄灭只留LED2，LED3常亮，5s钟内按下遥控器，可以发现LED分两组闪烁三次后熄灭即可完成对码。
//      以后该遥控器就可以对该ASK_Demo板进行遥控。该ASK_Demo板收到无线信号后，先输出串口，然后判断ID，ok后点亮对应的LED，100ms后熄灭。     
//单片机：STM8S003F3P6
//晶振：内部16Mhz
//作者：少凯同学，2168916131@qq.com
//时间：20170708
***********************************************************/
#include "All_Includes.h"

extern unsigned char timer_4_count;
extern unsigned char timer_4_countover;

unsigned char in_bit = 0;       
unsigned char rx_start = 0;     
unsigned char rx_data_ok = 0;

unsigned char recvbit[4];
unsigned char recvbitcount = 0;

unsigned char recvbyte[40];
unsigned char recvbytecount = 0;
unsigned char Recv_data[5];

unsigned char in_bit_n = 0;       

unsigned char SelfAddr[2]={0, 0};
///////////////////ASK初始化函数///////////////////////
void Ask_Init()
{
  PC_DDR_DDR4=0;        //KEY1输入
  PC_CR1_C14=0;         //悬浮输入
  PC_CR2_C24=0;         //禁止外部中断
}

void Uart_Sendbyte(unsigned char ch)
{
    return;
}

///////////////////ASK处理函数///////////////////////
void Ask_process()
{
  unsigned char key_value=0;
  
  Ask_Init();       //ASK初始化
  ReadSelfAddr();   //读eeprom里存储的ID
  
  while(1)
  {
    ProcessRecv();      //处理接收函数
    key_value=key_scan();
    
    Uart_Sendbyte(key_value);
    
    if(key_value==0x01)
    {//对码
      Led_on_all();
      Learn_Sender();
      Led_off_all();
    }
    else if(key_value==0x02)
    {//删除对码
      Dele_Sender();
    }
  }
}
///////////////////接收函数///////////////////////
void Recieve()
{
    //一进来就先把引脚的状态读取了，然后判断跟前面的是否一样，不一样的时候才进行后续运算
    in_bit_n = inport;  //inport是ASK模块的数据脚
    if(in_bit == in_bit_n)
    {
        return;
    }
    in_bit = in_bit_n;
    //P3_7 = in_bit;     //把值丢给LED口
    if(timer_4_countover)
    {
        //超时错误
        RecieveError();
        return;
    }
    // 接收4 次电平变化，才能确定1 bit
    if((timer_4_count > min_time_l)&&(timer_4_count < max_time_l))
    {
        //窄脉冲,4~14,就是200us~700us
        if(in_bit) //高电平,现在为高电平，其实之前是低电平的
        {
            recvbit[recvbitcount] = 0x00;   //低短
        }
        else    //低电平
        {
            recvbit[recvbitcount] = 0x01;   //高短
        }
    }
    else if((timer_4_count > min_time_h)&&(timer_4_count < max_time_h))
    {
        //宽脉冲，16~60，就是800us~3000us
        if(in_bit)
        {
            recvbit[recvbitcount] = 0x02;    //低长
        }
        else
        {
            recvbit[recvbitcount] = 0x03;   //高长
        }
    }
    else
    {
        //出错
        RecieveError();
        return;
    }
    timer_4_count = 0;
    timer_4_countover = 0;

    // 1527
    recvbitcount++;
    if(recvbitcount < 2) 
    {
        return;
    }
    else
    {
        //这里判断的电平，应该是跟实际的相反的，因为只有电平变化了，才会做相应处理，不变化的话是直接退出的。
        if((recvbit[0] == 1)&&(recvbit[1] == 2))   //高短低长
        {
            recvbyte[recvbytecount] = 0;
        }
        else if((recvbit[0] == 3)&&(recvbit[1] == 0))  //高长低短
        {
            recvbyte[recvbytecount] = 1;
        }
        else
        {
            RecieveError();
            return;
        }
    }
    
    recvbytecount++;    //接收到的字节数加1。
    recvbitcount = 0;   //
    if(recvbytecount < RECV_BIT_NUMBER)
    {
        // 未接收完
        return;
    }
    recvbytecount = 0;
    timer_4_count = 0;
    rx_data_ok = 1;
}
///////////////////接收错误函数///////////////////////
void RecieveError()
{
  rx_start = 0;
  rx_data_ok = 0;

  recvbitcount = 0;
  recvbytecount = 0;

  timer_4_count = 0;
  timer_4_countover = 0;

}
///////////////////处理接收函数///////////////////////
void ProcessRecv()
{
  unsigned char i,j;
  unsigned char p=0;
  unsigned char temp;
  
  Recieve();

  if(rx_data_ok)
  {
    rx_data_ok = 0;
    for(i=0;i<RECV_BIT_NUMBER;i=i+8)
    {//融合
      temp=0;
      for(j=i;j<(i+8);j++)
      {
        temp += (recvbyte[j]<<(7-(j-i)));
      }
      Recv_data[p++]=temp;
      //UART0_TX(temp);
    }
    Uart_Sendbyte('{');
    Uart_Sendbyte(Recv_data[0]);
    Uart_Sendbyte(Recv_data[1]);
    Uart_Sendbyte(Recv_data[2]);
    Uart_Sendbyte('}');
    ProcessOut();
  }
  else 
  {
    Uart_Sendbyte(8);
  }
}

//////////////////对码函数///////////////////
void Learn_Sender()
{
  unsigned long LearnDelay;
  unsigned char i,j;
  unsigned char p=0;
  unsigned char temp;
  
  LearnDelay = GetTimer();
  rx_data_ok = 0;
  
  while(1)
  {
    //5秒钟对码时间
    //WDT_CountClear();
    Recieve();
    if(rx_data_ok)
    {
        rx_data_ok = 0;

        EEPROM_EREASE();
        for(i=0;i<RECV_BIT_NUMBER;i=i+8)
        {//融合
          temp=0;
          for(j=i;j<(i+8);j++)
          {
            temp += (recvbyte[j]<<(7-(j-i)));
          }
          Recv_data[p++]=temp;
        }
        Uart_Sendbyte(Recv_data[0]);
        Uart_Sendbyte(Recv_data[1]);
        Uart_Sendbyte(Recv_data[2]);
        EEPROM_Byte_Write(EE_ADDR0, Recv_data[0]);
        delay_ms(1);
        EEPROM_Byte_Write(EE_ADDR1, Recv_data[1]);

        ReadSelfAddr();	
        for(i=0; i<5; i++)
        {//对码完成，跑马灯指示
          Led_on(1);
          delay_ms(200);
          Led_off(1);
          delay_ms(200);
        }
        //EX1 = 1;
        return;
    }
    delay_ms(10);
    if(SpanTime(LearnDelay) > 15000)//1ms
        return;
  }
}
///////////////////读取ID函数///////////////////////
void ReadSelfAddr()
{
  SelfAddr[0]= EEPROM_Byte_Read(EE_ADDR0);
  SelfAddr[1]= EEPROM_Byte_Read(EE_ADDR1);
}
///////////////////清除对码函数///////////////////////
void Dele_Sender()
{
  EEPROM_Byte_Write(EE_ADDR0, 0x00);
  EEPROM_Byte_Write(EE_ADDR0, 0x00);
  ReadSelfAddr();
}
///////////////////输出处理函数///////////////////////
void ProcessOut()
{
  if((Recv_data[0]==SelfAddr[0])&&(Recv_data[1]==SelfAddr[1]))
  {//匹配ID
    switch(Recv_data[2]&0xff)
    {
        case 0x01:
            Led_on(1);
            delay_ms(100);
            Led_off(1);
            delay_ms(100);
            break;
        case 0x02:
            Led_on(2);
            delay_ms(100);
            Led_off(2);
            delay_ms(100);
            break;
        case 0x04:
            Led_on(3);
            delay_ms(100);
            Led_off(3);
            delay_ms(100);
            break;
        case 0x08: 
            Led_on(4);
            delay_ms(100);
            Led_off(4);
            delay_ms(100);
            break;
        case 0x10: 
        case 0x20: 
        case 0x40: 
        case 0x80: 
            Led_on(4);
            delay_ms(100);
            Led_off(4);
            delay_ms(100);
            break;
        default:
            break;
    }
  }
	
}


