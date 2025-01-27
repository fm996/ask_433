/******************************************************************************* 
*  本文件为SPI操作SD卡的底层驱动文件
*  包括SPI模块及相关IO的初始化，SPI读写SD卡（写指令，读数据等）
*******************************************************************************/ 
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
#include "SPI_SD_driver.h"
#include "diskio.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static u8  SD_Type=0;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

extern SPI_HandleTypeDef        hspi2;

/*********************************************************************************************************
** Functoin name:       ReadSDFile
** Descriptions:        将buff_form的字符拷贝到buff_to
** input paraments:     buff_to:目标地址
                        buff_from：来源地址
** output paraments:    无    
** Returned values:     无
*********************************************************************************************************/
u8 stringcopy(BYTE *buff_to,BYTE *buff_from)
{
    u8 i=0;

    for(i=0;i<13;i++)
        buff_to[i] = buff_from[i];

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// 以下是SPI模块的初始化代码，配置成主机模式，访问SD卡
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* Function Name  : SPI_Configuration
* Description    : SPI模块初始化，【包括相关IO口的初始化】
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    SPI_GPIO_Hard_Init();

    //////SPI模块配置//////
    //一开始SD初始化阶段，SPI时钟频率必须<400K
    SD_CS_DISABLE();
/*
    //TP_CS 失能
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_PIN_12);
    
    //Flash 失能
    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
    GPIO_SetBits(GPIOG, GPIO_PIN_11);
    */
/*
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
*/
    MX_SPI2_Init();
    __HAL_SPI_ENABLE(&hspi2);
    return;
}

/*******************************************************************************
* Function Name  : SPI_SetSpeed
* Description    : SPI设置速度为高速
* Input          : u8 SpeedSet 
*                  如果速度设置输入0，则低速模式，非0则高速模式
*                  SPI_SPEED_HIGH   1
*                  SPI_SPEED_LOW    0
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_SetSpeed(u8 SpeedSet)
{
    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;//主模式
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;//全双工
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;//数据位为8位
    hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;//CPOL=0,low
    hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;//CPHA为数据线的第一个变化沿
    hspi2.Init.NSS = SPI_NSS_SOFT;//软件控制NSS

    //如果速度设置输入0，则低速模式，非0则高速模式
    if(SpeedSet==SPI_SPEED_LOW)
    {
        hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    }
    else
    {
        hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    }
    //moon.mp3: 4707774 Byte size 目标文件 设为buffer[512]     
    //speed:实验测试数据，最大速度 392314 Byte/S，
    //Prescaler_128, 59592 Byte/S
    //Prescaler_64, 104617 Byte/S
    //Prescaler_32, 168134 Byte/S    162337 Byte/S
    //Prescaler_16, 261543 Byte/S    247777 Byte/S
    //Prescaler_8,  313851 Byte/S    336269 Byte/S
    //Prescaler_4,  392314 Byte/S    392314 Byte/S
    //Prescaler_2,  392314 Byte/S

    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;//最高位先发送
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;//TIMODE模式关闭
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;//CRC关闭
    hspi2.Init.CRCPolynomial = 7;//默认值，无效

    HAL_SPI_Init(&hspi2);
    __HAL_SPI_ENABLE(&hspi2);

    return;
}


/*******************************************************************************
* Function Name  : SPI_ReadWriteByte
* Description    : SPI读写一个字节（发送完成后返回本次通讯读取的数据）
* Input          : u8 TxData 待发送的数
* Output         : None
* Return         : u8 RxData 收到的数
*******************************************************************************/
u8 SPI_ReadWriteByte(u8 TxData)
{
    HAL_StatusTypeDef res;
    u8 RxData = 0;
    //SPI_HandleTypeDef hspi;
/*
    //等待发送缓冲区空
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    //发一个字节
    SPI_I2S_SendData(SPI2, TxData);

    //等待数据接收
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    //取数据
    RxData = SPI_I2S_ReceiveData(SPI2);
*/
#if 1

    //HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, BUFFERSIZE, 5000);
    HAL_SPI_TransmitReceive(&hspi2, &TxData, &RxData, 1, 5000);
#else
    //hspi.Instance = SPI2;
    res = HAL_SPI_Transmit(&hspi2, &TxData, 1, 0xffff);
    //printf("%s[%d]res=%d\r\n",__FUNCTION__,__LINE__,res);
    res = HAL_SPI_Receive(&hspi2, &RxData, 1, 0xffff);
    //printf("%s[%d]res=%d\r\n",__FUNCTION__,__LINE__,res);
#endif
    return (u8)RxData;
}

/*******************************************************************************
* Function Name  : SD_WaitReady
* Description    : 等待SD卡Ready
* Input          : None
* Output         : None
* Return         : u8 
*                   0： 成功
*                   other：失败
*******************************************************************************/
u8 SD_WaitReady(void)
{
    u8 r1;
    u16 retry;
    retry = 0;
    do
    {
        r1 = SPI_ReadWriteByte(0xFF);
        if(retry==0xfffe)
        {
            return 1;
        }
    }while(r1!=0xFF);

    return 0;
}


/*******************************************************************************
* Function Name  : SD_SendCommand
* Description    : 向SD卡发送一个命令
* Input          : u8 cmd   命令 
*                  u32 arg  命令参数
*                  u8 crc   crc校验值
* Output         : None
* Return         : u8 r1 SD卡返回的响应
*******************************************************************************/
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc)
{
    unsigned char r1;
    unsigned char Retry = 0;

    //????????
    SPI_ReadWriteByte(0xff);
    //片选端置低，选中SD卡
    SD_CS_ENABLE();

    //发送
    SPI_ReadWriteByte(cmd | 0x40);                         //分别写入命令
    SPI_ReadWriteByte(arg >> 24);
    SPI_ReadWriteByte(arg >> 16);
    SPI_ReadWriteByte(arg >> 8);
    SPI_ReadWriteByte(arg);
    SPI_ReadWriteByte(crc);
    
    //等待响应，或超时退出
    while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
    {
        Retry++;
        //if(Retry > 200)
        if(Retry >= 10)
        {
            break;
        }
    }
    

    //关闭片选
    SD_CS_DISABLE();
    //在总线上额外增加8个时钟，让SD卡完成剩下的工作
    SPI_ReadWriteByte(0xFF);

    //返回状态值
    return r1;
}


/*******************************************************************************
* Function Name  : SD_SendCommand_NoDeassert
* Description    : 向SD卡发送一个命令(结束是不失能片选，还有后续数据传来）
* Input          : u8 cmd   命令 
*                  u32 arg  命令参数
*                  u8 crc   crc校验值
* Output         : None
* Return         : u8 r1 SD卡返回的响应
*******************************************************************************/
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc)
{
    unsigned char r1;
    unsigned char Retry = 0;

    //????????
    SPI_ReadWriteByte(0xff);
    //片选端置低，选中SD卡
    SD_CS_ENABLE();

    //发送
    SPI_ReadWriteByte(cmd | 0x40);                         //分别写入命令
    SPI_ReadWriteByte(arg >> 24);
    SPI_ReadWriteByte(arg >> 16);
    SPI_ReadWriteByte(arg >> 8);
    SPI_ReadWriteByte(arg);
    SPI_ReadWriteByte(crc);

    //等待响应，或超时退出
    while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
    {
        Retry++;
        if(Retry > 200)
        {
            break;
        }
    }
    //返回响应值
    return r1;
}

/*******************************************************************************
* Function Name  : SD_Init
* Description    : 初始化SD卡
* Input          : None
* Output         : None
* Return         : u8 
*                  0：NO_ERR
*                  1：TIME_OUT
*                  99：NO_CARD
*******************************************************************************/
u8 SD_Init(void)
{
    u16 i;      // 用来循环计数
    u8 r1;      // 存放SD卡的返回值
    u16 retry;  // 用来进行超时计数
    u8 buff[6];

    //如果没有检测到卡插入，直接退出，返回错误标志
    if(!SD_DET())
    {
        //return 99;
        printf("No tf card.\r\n");
        return STA_NODISK;  //  FatFS错误标志：没有插入磁盘
    }

    //SD卡上电
    //SD_PWR_ON();
    // 纯延时，等待SD卡上电完成
    //for(i=0;i<0xf00;i++);

    /******************************************************* 
    //这个地方要加一句,设置SPI速度为低速。 
    //为什么有的卡可以呢？因为SPI初始化时是低速的，SD卡初始化 
    //完成后设置为高速，有的卡只要初始化一次就行，程序就ok； 
    //但有的卡需要多次复位，呵呵，这个地方差这一句， 
    //这种卡就用不成咯！ 
    *******************************************************/ 
    SPI_SetSpeed(SPI_SPEED_LOW); //设置SPI速度为低速 
    printf("%s[%d]\r\n",__FUNCTION__,__LINE__);

    //先产生>74个脉冲，让SD卡自己初始化完成
    for(i=0;i<10;i++)
    {
        SPI_ReadWriteByte(0xFF);
    }
    printf("%s[%d]\r\n",__FUNCTION__,__LINE__);

    //-----------------SD卡复位到idle开始-----------------
    //循环连续发送CMD0，直到SD卡返回0x01,进入IDLE状态
    //超时则直接退出
    retry = 0;
    do
    {
        Delay_us(10);
        //发送CMD0，让SD卡进入IDLE状态
        r1 = SD_SendCommand(CMD0, 0, 0x95);
        retry++;
    }while((r1 != 0x01) && (retry<10));
    //}while((r1 != 0x01) && (retry<5));
    //跳出循环后，检查原因：初始化成功？or 重试超时？

    printf("%s[%d] res=%d\r\n",__FUNCTION__,__LINE__,r1);
    if(retry==10)
    //if(retry==5)
    {
        printf("SD reset time out!\r\n");
        return 1;   //超时返回1
    }
    //-----------------SD卡复位到idle结束-----------------

    //获取卡片的SD版本信息
    r1 = SD_SendCommand_NoDeassert(8, 0x1aa, 0x87);
    printf("tf card rev 0x%x\r\n",r1);

    //如果卡片版本信息是v1.0版本的，即r1=0x05，则进行以下初始化
    if(r1 == 0x05)
    {
        //设置卡类型为SDV1.0，如果后面检测到为MMC卡，再修改为MMC
        SD_Type = SD_TYPE_V1;

        //如果是V1.0卡，CMD8指令后没有后续数据
        //片选置高，结束本次命令
        SD_CS_DISABLE();
        //多发8个CLK，让SD结束后续操作
        SPI_ReadWriteByte(0xFF);

        //-----------------SD卡、MMC卡初始化开始-----------------

        //发卡初始化指令CMD55+ACMD41
        // 如果有应答，说明是SD卡，且初始化完成
        // 没有回应，说明是MMC卡，额外进行相应初始化
        retry = 0;
        do
        {
            //先发CMD55，应返回0x01；否则出错
            r1 = SD_SendCommand(CMD55, 0, 0);
            if(r1 != 0x01)
            {
                return r1;  
            }
            //得到正确响应后，发ACMD41，应得到返回值0x00，否则重试200次
            r1 = SD_SendCommand(ACMD41, 0, 0);
            retry++;
        }while((r1!=0x00) && (retry<400));
        // 判断是超时还是得到正确回应
        // 若有回应：是SD卡；没有回应：是MMC卡
        
        //----------MMC卡额外初始化操作开始------------
        if(retry==400)
        {
            retry = 0;
            //发送MMC卡初始化命令（没有测试）
            do
            {
                r1 = SD_SendCommand(1, 0, 0);
                retry++;
            }while((r1!=0x00)&& (retry<400));
            if(retry==400)
            {
                return 1;   //MMC卡初始化超时
            }
            //写入卡类型
            SD_Type = SD_TYPE_MMC;
        }
        //----------MMC卡额外初始化操作结束------------
        
        //设置SPI为高速模式
        SPI_SetSpeed(SPI_SPEED_HIGH);

        SPI_ReadWriteByte(0xFF);
        
        //禁止CRC校验
        /*
        r1 = SD_SendCommand(CMD59, 0, 0x01);
        if(r1 != 0x00)
        {
            return r1;  //命令错误，返回r1
        }
        */    
        //设置Sector Size
        r1 = SD_SendCommand(CMD16, 512, 0xff);
        if(r1 != 0x00)
        {
            return r1;  //命令错误，返回r1
        }
        //-----------------SD卡、MMC卡初始化结束-----------------

    }//SD卡为V1.0版本的初始化结束
    

    //下面是V2.0卡的初始化
    //其中需要读取OCR数据，判断是SD2.0还是SD2.0HC卡
    else if(r1 == 0x01)
    {
        //V2.0的卡，CMD8命令后会传回4字节的数据，要跳过再结束本命令
        buff[0] = SPI_ReadWriteByte(0xFF);  //should be 0x00
        buff[1] = SPI_ReadWriteByte(0xFF);  //should be 0x00
        buff[2] = SPI_ReadWriteByte(0xFF);  //should be 0x01
        buff[3] = SPI_ReadWriteByte(0xFF);  //should be 0xAA
     
        SD_CS_DISABLE();
        //the next 8 clocks
        SPI_ReadWriteByte(0xFF);
        
        //判断该卡是否支持2.7V-3.6V的电压范围
        if(buff[2]==0x01 && buff[3]==0xAA)
        {
            //支持电压范围，可以操作
            retry = 0;
            //发卡初始化指令CMD55+ACMD41
            do
            {
                r1 = SD_SendCommand(CMD55, 0, 0);
                if(r1!=0x01)
                {
                    return r1;
                }
                r1 = SD_SendCommand(ACMD41, 0x40000000, 0);
                if(retry>200)   
                {
                    return r1;  //超时则返回r1状态
                }
            }while(r1!=0);
          
            //初始化指令发送完成，接下来获取OCR信息

            //-----------鉴别SD2.0卡版本开始-----------
            r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
            if(r1!=0x00)
            {
                return r1;  //如果命令没有返回正确应答，直接退出，返回应答
            }
            //读OCR指令发出后，紧接着是4字节的OCR信息
            buff[0] = SPI_ReadWriteByte(0xFF);
            buff[1] = SPI_ReadWriteByte(0xFF); 
            buff[2] = SPI_ReadWriteByte(0xFF);
            buff[3] = SPI_ReadWriteByte(0xFF);

            //OCR接收完成，片选置高
            SD_CS_DISABLE();
            SPI_ReadWriteByte(0xFF);

            //检查接收到的OCR中的bit30位（CCS），确定其为SD2.0还是SDHC
            //如果CCS=1：SDHC   CCS=0：SD2.0
            if(buff[0]&0x40)    //检查CCS
            {
                SD_Type = SD_TYPE_V2HC;
            }
            else
            {
                SD_Type = SD_TYPE_V2;
            }
            //-----------鉴别SD2.0卡版本结束-----------

            
            //设置SPI为高速模式
            SPI_SetSpeed(1);  
        }

    }
    return r1;
}



/*******************************************************************************
* Function Name  : SD_ReceiveData
* Description    : 从SD卡中读回指定长度的数据，放置在给定位置
* Input          : u8 *data(存放读回数据的内存>len)
*                  u16 len(数据长度）
*                  u8 release(传输完成后是否释放总线CS置高 0：不释放 1：释放）
* Output         : None
* Return         : u8 
*                  0：NO_ERR
*                  other：错误信息
*******************************************************************************/
u8 SD_ReceiveData(u8 *data, u16 len, u8 release)
{
    u16 retry;
    u8 r1;

    // 启动一次传输
    SD_CS_ENABLE();
    //等待SD卡发回数据起始令牌0xFE
    retry = 0;
    do
    {
        r1 = SPI_ReadWriteByte(0xFF);
        retry++;
        if(retry>2000)  //2000次等待后没有应答，退出报错
        {
            SD_CS_DISABLE();
            return 1;
        }
    }while(r1 != 0xFE);
    //开始接收数据
    while(len--)
    {
        *data = SPI_ReadWriteByte(0xFF);
        data++;
    }
    //下面是2个伪CRC（dummy CRC）
    SPI_ReadWriteByte(0xFF);
    SPI_ReadWriteByte(0xFF);
    //按需释放总线，将CS置高
    if(release == RELEASE)
    {
        //传输结束
        SD_CS_DISABLE();
        SPI_ReadWriteByte(0xFF);
    }

    return 0;
}


/*******************************************************************************
* Function Name  : SD_GetCID
* Description    : 获取SD卡的CID信息，包括制造商信息
* Input          : u8 *cid_data(存放CID的内存，至少16Byte）
* Output         : None
* Return         : u8 
*                  0：NO_ERR
*                  1：TIME_OUT
*                  other：错误信息
*******************************************************************************/
u8 SD_GetCID(u8 *cid_data)
{
    u8 r1;

    //发CMD10命令，读CID
    r1 = SD_SendCommand(CMD10, 0, 0xFF);
    if(r1 != 0x00)
    {
        return r1;  //没返回正确应答，则退出，报错
    }
    //接收16个字节的数据
    SD_ReceiveData(cid_data, 16, RELEASE);

    return 0;
}


/*******************************************************************************
* Function Name  : SD_GetCSD
* Description    : 获取SD卡的CSD信息，包括容量和速度信息
* Input          : u8 *cid_data(存放CID的内存，至少16Byte）
* Output         : None
* Return         : u8 
*                  0：NO_ERR
*                  1：TIME_OUT
*                  other：错误信息
*******************************************************************************/
u8 SD_GetCSD(u8 *csd_data)
{
    u8 r1;

    //发CMD9命令，读CSD
    r1 = SD_SendCommand(CMD9, 0, 0xFF);
    if(r1 != 0x00)
    {
        return r1;  //没返回正确应答，则退出，报错
    }
    //接收16个字节的数据
    SD_ReceiveData(csd_data, 16, RELEASE);

    return 0;
}


/*******************************************************************************
* Function Name  : SD_GetCapacity
* Description    : 获取SD卡的容量
* Input          : None
* Output         : None
* Return         : u32 capacity 
*                   0： 取容量出错 
*******************************************************************************/
u32 SD_GetCapacity(void)
{
    u8 csd[16];
    u32 Capacity;
    u8 r1;
    u16 i;
    u16 temp;

    //取CSD信息，如果期间出错，返回0
    if(SD_GetCSD(csd)!=0)
    {
        return 0;
    }
       
    //如果为SDHC卡，按照下面方式计算
    if((csd[0]&0xC0)==0x40)
    {
        Capacity =  (((u32)csd[8])<<8 + (u32)csd[9] +1)*(u32)1024;
    }
    else
    {
        //下面代码为网上版本
        ////////////formula of the capacity///////////////
        //
        //  memory capacity = BLOCKNR * BLOCK_LEN
        //  
        //  BLOCKNR = (C_SIZE + 1)* MULT
        //
        //           C_SIZE_MULT+2
        //  MULT = 2
        //
        //               READ_BL_LEN
        //  BLOCK_LEN = 2
        /**********************************************/
        //C_SIZE
        i = csd[6]&0x03;
        i<<=8;
        i += csd[7];
        i<<=2;
        i += ((csd[8]&0xc0)>>6);
    
        //C_SIZE_MULT
        r1 = csd[9]&0x03;
        r1<<=1;
        r1 += ((csd[10]&0x80)>>7);
    
        //BLOCKNR
        r1+=2;
        temp = 1;
        while(r1)
        {
            temp*=2;
            r1--;
        }
        Capacity = ((u32)(i+1))*((u32)temp);
    
        // READ_BL_LEN
        i = csd[5]&0x0f;
        //BLOCK_LEN
        temp = 1;
        while(i)
        {
            temp*=2;
            i--;
        }
        //The final result
        Capacity *= (u32)temp; 
        //Capacity /= 512;
    }
    return (u32)Capacity;
}


/*******************************************************************************
* Function Name  : SD_ReadSingleBlock
* Description    : 读SD卡的一个block
* Input          : u32 sector 取地址（sector值，非物理地址） 
*                  u8 *buffer 数据存储地址（大小至少512byte） 
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer)
{
    u8 r1;

    //设置为高速模式
    SPI_SetSpeed(SPI_SPEED_HIGH);
    
    //如果不是SDHC，将sector地址转成byte地址
    sector = sector<<9;

    r1 = SD_SendCommand(CMD17, sector, 0);//读命令
    
    if(r1 != 0x00)
    {
        return r1;
    }
    
    r1 = SD_ReceiveData(buffer, 512, RELEASE);
    if(r1 != 0)
    {
        return r1;   //读数据出错！
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
* Function Name  : SD_WriteSingleBlock
* Description    : 写入SD卡的一个block
* Input          : u32 sector 扇区地址（sector值，非物理地址） 
*                  u8 *buffer 数据存储地址（大小至少512byte） 
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
u8 SD_WriteSingleBlock(u32 sector, const u8 *data)
{
    u8 r1;
    u16 i;
    u16 retry;

    //设置为高速模式
    SPI_SetSpeed(SPI_SPEED_HIGH);

    //如果不是SDHC，给定的是sector地址，将其转换成byte地址
    if(SD_Type!=SD_TYPE_V2HC)
    {
        sector = sector<<9;
    }

    r1 = SD_SendCommand(CMD24, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1;  //应答不正确，直接返回
    }
    
    //开始准备数据传输
    SD_CS_ENABLE();
    //先放3个空数据，等待SD卡准备好
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    //放起始令牌0xFE
    SPI_ReadWriteByte(0xFE);

    //放一个sector的数据
    for(i=0;i<512;i++)
    {
        SPI_ReadWriteByte(*data++);
    }
    //发2个Byte的dummy CRC
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    
    //等待SD卡应答
    r1 = SPI_ReadWriteByte(0xff);
    if((r1&0x1F)!=0x05)
    {
        SD_CS_DISABLE();
        return r1;
    }
    
    //等待操作完成
    retry = 0;
    while(!SPI_ReadWriteByte(0xff))
    {
        retry++;
        if(retry>0xfffe)        //如果长时间写入没有完成，报错退出
        {
            SD_CS_DISABLE();
            return 1;           //写入超时返回1
        }
    }

    //写入完成，片选置1
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xff);

    return 0;
}


/*******************************************************************************
* Function Name  : SD_ReadMultiBlock
* Description    : 读SD卡的多个block
* Input          : u32 sector 取地址（sector值，非物理地址） 
*                  u8 *buffer 数据存储地址（大小至少512byte）
*                  u8 count 连续读count个block
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count)
{
    u8 r1;

    //设置为高速模式
    SPI_SetSpeed(SPI_SPEED_HIGH);
    
    //如果不是SDHC，将sector地址转成byte地址
    sector = sector<<9;
    //SD_WaitReady();
    //发读多块命令
    r1 = SD_SendCommand(CMD18, sector, 0);//读命令
    if(r1 != 0x00)
    {
        return r1;
    }
    //开始接收数据
    do
    {
        if(SD_ReceiveData(buffer, 512, NO_RELEASE) != 0x00)
        {
            break;
        }
        buffer += 512;
    } while(--count);

    //全部传输完毕，发送停止命令
    SD_SendCommand(CMD12, 0, 0);
    //释放总线
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xFF);
    
    if(count != 0)
    {
        return count;   //如果没有传完，返回剩余个数
    }
    else
    {
        return 0;
    }
}


/*******************************************************************************
* Function Name  : SD_WriteMultiBlock
* Description    : 写入SD卡的N个block
* Input          : u32 sector 扇区地址（sector值，非物理地址） 
*                  u8 *buffer 数据存储地址（大小至少512byte）
*                  u8 count 写入的block数目
* Output         : None
* Return         : u8 r1 
*                   0： 成功
*                   other：失败
*******************************************************************************/
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count)
{
    u8 r1;
    u16 i;

    //设置为高速模式
    SPI_SetSpeed(SPI_SPEED_HIGH);

    //如果不是SDHC，给定的是sector地址，将其转换成byte地址
    if(SD_Type != SD_TYPE_V2HC)
    {
        sector = sector<<9;
    }
    //如果目标卡不是MMC卡，启用ACMD23指令使能预擦除
    if(SD_Type != SD_TYPE_MMC)
    {
        r1 = SD_SendCommand(ACMD23, count, 0x00);
    }
    //发多块写入指令
    r1 = SD_SendCommand(CMD25, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1;  //应答不正确，直接返回
    }
    
    //开始准备数据传输
    SD_CS_ENABLE();
    //先放3个空数据，等待SD卡准备好
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);

    //--------下面是N个sector写入的循环部分
    do
    {
        //放起始令牌0xFC 表明是多块写入
        SPI_ReadWriteByte(0xFC);
    
        //放一个sector的数据
        for(i=0;i<512;i++)
        {
            SPI_ReadWriteByte(*data++);
        }
        //发2个Byte的dummy CRC
        SPI_ReadWriteByte(0xff);
        SPI_ReadWriteByte(0xff);
        
        //等待SD卡应答
        r1 = SPI_ReadWriteByte(0xff);
        if((r1&0x1F)!=0x05)
        {
            SD_CS_DISABLE();    //如果应答为报错，则带错误代码直接退出
            return r1;
        }

        //等待SD卡写入完成
        if(SD_WaitReady()==1)
        {
            SD_CS_DISABLE();    //等待SD卡写入完成超时，直接退出报错
            return 1;
        }

        //本sector数据传输完成
    }while(--count);
    
    //发结束传输令牌0xFD
    r1 = SPI_ReadWriteByte(0xFD);
    if(r1==0x00)
    {
        count =  0xfe;
    }

    if(SD_WaitReady())
    {
        while(1)
        {
        }
    }
    
    //写入完成，片选置1
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xff);

    return count;   //返回count值，如果写完则count=0，否则count=1
}

