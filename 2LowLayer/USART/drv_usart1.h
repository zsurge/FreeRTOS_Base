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

//引脚定义
/*******************************************************/
#define USART1_CLK                         RCC_APB2Periph_USART1
#define USART1_GPIO_PORT                   GPIOA
#define USART1_GPIO_CLK                    RCC_AHB1Periph_GPIOA

#define USART1_RX_PIN                      GPIO_Pin_10
#define USART1_RX_AF                       GPIO_AF_USART1
#define USART1_RX_SOURCE                   GPIO_PinSource10

#define USART1_TX_PIN                      GPIO_Pin_9
#define USART1_TX_AF                       GPIO_AF_USART1
#define USART1_TX_SOURCE                   GPIO_PinSource9



#if 0

#include "stdio.h"	
#include "string.h"
#include "stm32f4xx_conf.h"
#include "stdarg.h"


#define USART1MAXBUFFSIZE 1280 //需要做IAP升级，一包1024个字节

extern volatile u8 USART1RecvBuf[USART1MAXBUFFSIZE];
extern volatile u16 RecvTop1;
extern volatile u16 RecvEnd1;



void drv_Usart1Init (u32 BaudRate);
void drv_Usart1SendString (const u8 *Buff);
void drv_Usart1SendData (const u8 *Buff, u16 SendSize);
void drv_Usart1RecvReset (void);
u8 drv_Usart1RecvOne (u8 *Str);
u16 drv_Usart1RecvAtTime (u8 *Buff, u16 RecvSize, u32 timeout_MilliSeconds);
u8 drv_Usart1Read(u8 *buf, u16 len);

//void UARTprintf(const uint8_t *pcString, ...);
#endif


void drv_Usart1_Init(int BaudRate);
void drv_Usart1_DMA_Send(uint8_t *buf, uint16_t len);
uint8_t drv_Usart1_GetByte(void);



    
#endif


