/******************************************************************************

                  版权所有 (C), 2001-2011, 博思高科技有限公司

 ******************************************************************************
  文 件 名   : drv_usart.h
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年5月23日
  最近修改   :
  功能描述   :  串口验动
  函数列表   :
*
*

  修改历史   :
  1.日    期   : 2019年5月23日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/
#ifndef __DRV_USART_H
#define __DRV_USART_H
	
#include "stdio.h"	
#include "string.h"
#include "stm32f4xx_conf.h"

#if SYSTEM_SUPPORT_OS
	#include "FreeRTOS.h"
	#include "task.h"
	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* 开关全局中断的宏 */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */
#endif

/* 定义使能的串口, 0 表示不使能（不增加代码大小）， 1表示使能 */
#define UART1_FIFO_EN	1
#define UART2_FIFO_EN	1
#define UART3_FIFO_EN	0
#define UART4_FIFO_EN	0
#define UART5_FIFO_EN	0
#define UART6_FIFO_EN	0

/* RS485芯片发送使能GPIO, PB2 */
//#define RCC_RS485_TXEN	RCC_AHB1Periph_GPIOB
//#define PORT_RS485_TXEN  GPIOB
//#define PIN_RS485_TXEN	 GPIO_Pin_2

//#define RS485_RX_EN()	PORT_RS485_TXEN->BSRRH = PIN_RS485_TXEN
//#define RS485_TX_EN()	PORT_RS485_TXEN->BSRRL = PIN_RS485_TXEN

/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 或  PB6, PB7*/
	COM2 = 1,	/* USART2, PD5,PD6 或 PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
	COM6 = 5	/* USART6, PG14, PG7 */
}COM_PORT_E;


/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			115200
	#define UART1_TX_BUF_SIZE	1*1024
	#define UART1_RX_BUF_SIZE	1*1024
#endif

#if UART2_FIFO_EN == 1
	#define UART2_BAUD			115200
	#define UART2_TX_BUF_SIZE	1
	#define UART2_RX_BUF_SIZE	2*1024
#endif

#if UART3_FIFO_EN == 1
	#define UART3_BAUD			115200
	#define UART3_TX_BUF_SIZE	1*1024
	#define UART3_RX_BUF_SIZE	1*1024
#endif

#if UART4_FIFO_EN == 1
	#define UART4_BAUD			115200
	#define UART4_TX_BUF_SIZE	1*1024
	#define UART4_RX_BUF_SIZE	1*1024
#endif

#if UART5_FIFO_EN == 1
	#define UART5_BAUD			115200
	#define UART5_TX_BUF_SIZE	1*1024
	#define UART5_RX_BUF_SIZE	1*1024
#endif

#if UART6_FIFO_EN == 1
	#define UART6_BAUD			115200
	#define UART6_TX_BUF_SIZE	1*1024
	#define UART6_RX_BUF_SIZE	1*1024
#endif

/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
	uint8_t *pTxBuf;			/* 发送缓冲区 */
	uint8_t *pRxBuf;			/* 接收缓冲区 */
	uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	__IO uint16_t usTxWrite;	/* 发送缓冲区写指针 */
	__IO uint16_t usTxRead;		/* 发送缓冲区读指针 */
	__IO uint16_t usTxCount;	/* 等待发送的数据个数 */

	__IO uint16_t usRxWrite;	/* 接收缓冲区写指针 */
	__IO uint16_t usRxRead;		/* 接收缓冲区读指针 */
	__IO uint16_t usRxCount;	/* 还未读取的新数据个数 */

	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
}UART_T;


void drv_UartInit(void);
void drv_UartSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void drv_UartSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t drv_UartGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);
void drv_UartClearTxFifo(COM_PORT_E _ucPort);
void drv_UartClearRxFifo(COM_PORT_E _ucPort);
void drv_UartSetBaud(COM_PORT_E _ucPort, uint32_t _BaudRate);



//void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen);
//void RS485_SendStr(char *_pBuf);
//void RS485_SetBaud(uint32_t _baud);

#endif

