/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : drv_usart1.h
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年5月22日
  最近修改   :
  功能描述   : 串口1初始化
  函数列表   :
  修改历史   :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

#ifndef __DRV_USART1_H
#define __DRV_USART1_H

#include "stdio.h"	
#include "string.h"
#include "stm32f4xx_conf.h"

#define USART1MAXBUFFSIZE 128

void drv_Usart1Init (u32 BaudRate);
void drv_Usart1SendString (const u8 *Buff);
void drv_Usart1SendData (const u8 *Buff, u16 SendSize);
void drv_Usart1RecvReset (void);
u8 drv_Usart1RecvOne (u8 *Str);
u16 drv_Usart1RecvAtTime (u8 *Buff, u16 RecvSize, u32 timeout_MilliSeconds);
u8 drv_Usart1Read(u8 *buf, u16 len);
    
#endif


