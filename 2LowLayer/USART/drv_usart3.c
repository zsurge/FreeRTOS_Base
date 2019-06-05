/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : drv_Usart3.c
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年5月22日
  最近修改   :
  功能描述   : 串口3 IAP升级
  函数列表   :
  修改历史   :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

#include "drv_usart3.h"

volatile u8 USART3RecvBuf[USART3MAXBUFFSIZE] = {0};
volatile u16 RecvTop3 = 0;
volatile u16 RecvEnd3 = 0;



/*****************************************************************************
 函 数 名  : drv_Usart3Init
 功能描述  : 串口初始化
 输入参数  : u32 BaudRate  波特率
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart3Init (u32 BaudRate);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart3_Init (u32 BaudRate)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(USART3_GPIO_CLK,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(USART3_CLK,ENABLE);//使能Usart3时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(USART3_GPIO_PORT,USART3_TX_SOURCE,USART3_TX_AF); //GPIOB10复用为USART3
	GPIO_PinAFConfig(USART3_GPIO_PORT,USART3_RX_SOURCE,USART3_RX_AF); //GPIOB11复用为USART3
	
	//Usart2端口配置
	GPIO_InitStructure.GPIO_Pin = USART3_TX_PIN | USART3_RX_PIN; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(USART3_GPIO_PORT,&GPIO_InitStructure); //初始化PB10，PB11

   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口2    

	//USART3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断    
	
	USART_Cmd(USART3, ENABLE);  //使能串口1   

}

void USART3_IRQHandler (void)
{
	u8 temp = 0;

	//Receive data register not empty flag
	if (USART_GetITStatus (USART3, USART_IT_RXNE) != RESET)
	{
		temp = USART_ReceiveData (USART3);		//get received data

		if ( (RecvEnd3 == (RecvTop3 - 1) ) || ( (RecvTop3 == 0) && (RecvEnd3 == (USART3MAXBUFFSIZE - 1) ) ) )
		{
			//缓冲溢出
			RecvTop3 = 0;
			RecvEnd3 = 0;
		}
		else
		{
			USART3RecvBuf[RecvEnd3] = temp;
			RecvEnd3++;

			if (RecvEnd3 >= USART3MAXBUFFSIZE) RecvEnd3 = 0;
		}

		/* Clear the UART1 Recvive interrupt */
		USART_ClearITPendingBit (USART3, USART_IT_RXNE);
	}
	else if (USART_GetITStatus (USART3, USART_IT_NE | USART_IT_PE | USART_IT_FE | USART_IT_ORE) != RESET)
	{
		USART_GetITStatus (USART3, USART_IT_NE | USART_IT_PE | USART_IT_FE | USART_IT_ORE);
		USART_ReceiveData (USART3);
	}
}


void drv_Usart3_SendOne (const u8 dat)
{
	USART_SendData (USART3, dat);

	while (USART_GetFlagStatus (USART3, USART_FLAG_TXE) == RESET) {}

	USART_ClearFlag (USART3, USART_FLAG_TXE);

}



/*****************************************************************************
 函 数 名  : drv_Usart3SendString
 功能描述  : 发送字符串
 输入参数  : const u8 *Buff  发送字符串
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart3SendString (const u8 *Buff);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart3_SendString (const u8 *Buff)
{
	while (*Buff != 0)
	{
		drv_Usart3_SendOne (*Buff);
		Buff++;
	}    
}


/*****************************************************************************
 函 数 名  : drv_Usart3SendData
 功能描述  : 发送指定长度字符
 输入参数  : const u8 *Buff  发送数据的buff
             u16 SendSize  发送数据的长度  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart3SendData (const u8 *Buff, u16 SendSize);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart3_SendData (const u8 *Buff, u16 SendSize)
{
	while (SendSize != 0)
	{
		drv_Usart3_SendOne (*Buff);
		Buff++;
		SendSize--;
	}

}



/*****************************************************************************
 函 数 名  : drv_Usart3RecvReset
 功能描述  : 清空串口接收缓冲
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart3RecvReset (void);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart3_RecvReset (void)
{
	RecvTop3 = 0;
	RecvEnd3 = 0;

	memset((void *)USART3RecvBuf, 0, USART3MAXBUFFSIZE); 
}



/*****************************************************************************
 函 数 名  : drv_Usart3RecvOne
 功能描述  : 读取1个字节
 输入参数  : u8 *Str  读出的字节存储在Str[0]
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
u8 drv_Usart3RecvOne (u8 *Str);
    修改内容   : 新生成函数

*****************************************************************************/
u8 drv_Usart3_RecvOne (u8 *Str)
{
	if (RecvTop3 == RecvEnd3) return 0;//read nothing

	*Str = USART3RecvBuf[RecvTop3];
	RecvTop3++;

	if (RecvTop3 >= USART3MAXBUFFSIZE) RecvTop3 = 0;

	return 1;//read one

}



/*****************************************************************************
 函 数 名  : drv_Usart3RecvAtTime
 功能描述  : 接收串口数据
 输入参数  : u8 *Buff 接受数据存放的地方                 
             u16 RecvSize  接受多少个字节            
             u32 timeout_MilliSeconds  超时的毫秒数
 输出参数  : 无
 返 回 值  : 返回接受的总字节数
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
u16 drv_Usart3RecvAtTime (u8 *Buff, u16 RecvSize, u32 timeout_MilliSeconds);
    修改内容   : 新生成函数

*****************************************************************************/
u16 drv_Usart3_RecvAtTime (u8 *Buff, u16 RecvSize, u32 timeout_MilliSeconds)
{
//	u16 RecvLen = 0;
//	u8 tmp[1] = {0};

//	if (RecvSize == 0) return 0;

//	g1msTimer5 = timeout_MilliSeconds;

//	while (1)
//	{
//		if (g1msTimer5 == 0) return RecvLen;

//		if (drv_Usart3RecvOne (tmp) == 1) 
//		{
//			Buff[RecvLen++] = tmp[0];
//		}

//		if (RecvLen >= RecvSize) return RecvLen;
//	}
	return 0;
}


/*****************************************************************************
 函 数 名  : dev_Usart2Read
 功能描述  : 读串口数据
 输入参数  : unsigned char *buf  
             int len             
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
int dev_Usart2Read(unsigned char *buf, int len)
    修改内容   : 新生成函数

*****************************************************************************/
u8 drv_Usart3_Read(u8 *Buff, u16 len)
{

	u16 RecvLen = 0;
	u8 tmp[1] = {0};

	if (len == 0 || Buff == NULL) return 0;

	while (len--)
	{
		if (drv_Usart3_RecvOne (tmp) == 1)
		{
			Buff[RecvLen++] = tmp[0];
		}

		if (RecvLen >= len) return RecvLen;
	}

	return RecvLen;
    
}



