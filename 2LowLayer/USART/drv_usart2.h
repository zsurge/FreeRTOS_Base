/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : drv_usart2.h
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年5月22日
  最近修改   :
  功能描述   : 串口2 指令接收
  函数列表   :
  修改历史   :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

#ifndef __DRV_USART2_H
#define __DRV_USART2_H

#include "stdio.h"	
#include "string.h"
#include "stm32f4xx_conf.h"



#define USART2MAXBUFFSIZE 1024

void drv_Usart2_Init (u32 BaudRate);
void drv_Usart2SendString (const u8 *Buff);
void drv_Usart2SendData (const u8 *Buff, u16 SendSize);
void drv_Usart2RecvReset (void);
u8 drv_Usart2RecvOne (u8 *Str);
u16 drv_Usart2RecvAtTime (u8 *Buff, u16 RecvSize, u32 timeout_MilliSeconds);
u8 drv_Usart2Read(u8 *buf, u16 len);

    
#endif


