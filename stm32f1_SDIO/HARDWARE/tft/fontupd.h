/**********************************************************
* @ File name -> fontupd.h
* @ Version   -> V1.0
* @ Date      -> 13-02-2014
* @ Brief     -> 字库相关函数头文件

 V1.
* @ Revise    ->
**********************************************************/

#ifndef _fontupd_h_
#define _fontupd_h_

/**********************************************************
                     外部函数头文件                        
**********************************************************/

#include "sys.h"

/**********************************************************
                        定义字库来源
**********************************************************/

#define _USER_SD_FON					1	//0：不使用SD字库
                                            //1：使用SD字库

//=========================================================
#if !_USER_SD_FON	/* 在不使用SD卡字库的时候才能定义 */
//=========================================================
#define _USER_BufforFLASH				0	//0：使用写入MCU-ROM的字库数组
                                            //1：使用外部SPI-FLASH做字库芯片
											//2：使用外部SPI-FLASH文件系统内字库
//=========================================================
#endif	/* end _USER_SD_FON */
//=========================================================

/**********************************************************
                       外部功能函数                      
**********************************************************/

void Get_Hzlib(u8 *ch, u8 *c_HanZibuff,u8 size);	//取得中文字库数据


#endif

