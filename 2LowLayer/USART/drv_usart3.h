/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : drv_usart3.h
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年5月22日
  最近修改   :
  功能描述   : 串口3 指令接收
  函数列表   :
  修改历史   :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

#ifndef __DRV_USART3_H
#define __DRV_USART3_H

#include "stdio.h"	
#include "string.h"
#include "stm32f4xx_conf.h"

#define USART3_CLK                         RCC_APB1Periph_USART3
#define USART3_GPIO_PORT                   GPIOB
#define USART3_GPIO_CLK                    RCC_AHB1Periph_GPIOB

#define USART3_RX_PIN                      GPIO_Pin_11
#define USART3_RX_AF                       GPIO_AF_USART3
#define USART3_RX_SOURCE                   GPIO_PinSource11


#define USART3_TX_PIN                      GPIO_Pin_10
#define USART3_TX_AF                       GPIO_AF_USART3
#define USART3_TX_SOURCE                   GPIO_PinSource10




#define USART3MAXBUFFSIZE 1024

void drv_usart3Init (u32 BaudRate);
void drv_usart3SendString (const u8 *Buff);
void drv_usart3SendData (const u8 *Buff, u16 SendSize);
void drv_usart3RecvReset (void);
u8 drv_usart3RecvOne (u8 *Str);
u16 drv_usart3RecvAtTime (u8 *Buff, u16 RecvSize, u32 timeout_MilliSeconds);
u8 drv_usart3Read(u8 *buf, u16 len);

    
#endif


