/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : drv_usart2.c
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

#include "drv_usart2.h"

volatile u8 USART2RecvBuf[USART2MAXBUFFSIZE] = {0};
volatile u16 RecvTop2 = 0;
volatile u16 RecvEnd2 = 0;



/*****************************************************************************
 函 数 名  : drv_Usart2Init
 功能描述  : 串口初始化
 输入参数  : u32 BaudRate  波特率
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart2Init (u32 BaudRate);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart2Init (u32 BaudRate)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能Usart2时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2
	
	//Usart2端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3

   //Usart2 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2    

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断    
	
	USART_Cmd(USART2, ENABLE);  //使能串口1   

}

void USART2_IRQHandler (void)
{
	u8 temp = 0;

	//Receive data register not empty flag
	if (USART_GetITStatus (USART2, USART_IT_RXNE) != RESET)
	{
		temp = USART_ReceiveData (USART2);		//get received data

		if ( (RecvEnd2 == (RecvTop2 - 1) ) || ( (RecvTop2 == 0) && (RecvEnd2 == (USART2MAXBUFFSIZE - 1) ) ) )
		{
			//缓冲溢出
			RecvTop2 = 0;
			RecvEnd2 = 0;
		}
		else
		{
			USART2RecvBuf[RecvEnd2] = temp;
			RecvEnd2++;

			if (RecvEnd2 >= USART2MAXBUFFSIZE) RecvEnd2 = 0;
		}

		/* Clear the UART1 Recvive interrupt */
		USART_ClearITPendingBit (USART2, USART_IT_RXNE);
	}
	else if (USART_GetITStatus (USART2, USART_IT_NE | USART_IT_PE | USART_IT_FE | USART_IT_ORE) != RESET)
	{
		USART_GetITStatus (USART2, USART_IT_NE | USART_IT_PE | USART_IT_FE | USART_IT_ORE);
		USART_ReceiveData (USART2);
	}
}


void drv_Usart2SendOne (const u8 dat)
{
	USART_SendData (USART2, dat);

	while (USART_GetFlagStatus (USART2, USART_FLAG_TXE) == RESET) {}

	USART_ClearFlag (USART2, USART_FLAG_TXE);

}



/*****************************************************************************
 函 数 名  : drv_Usart2SendString
 功能描述  : 发送字符串
 输入参数  : const u8 *Buff  发送字符串
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart2SendString (const u8 *Buff);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart2SendString (const u8 *Buff)
{
	while (*Buff != 0)
	{
		drv_Usart2SendOne (*Buff);
		Buff++;
	}    
}


/*****************************************************************************
 函 数 名  : drv_Usart2SendData
 功能描述  : 发送指定长度字符
 输入参数  : const u8 *Buff  发送数据的buff
             u16 SendSize  发送数据的长度  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart2SendData (const u8 *Buff, u16 SendSize);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart2SendData (const u8 *Buff, u16 SendSize)
{
	while (SendSize != 0)
	{
		drv_Usart2SendOne (*Buff);
		Buff++;
		SendSize--;
	}

}



/*****************************************************************************
 函 数 名  : drv_Usart2RecvReset
 功能描述  : 清空串口接收缓冲
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart2RecvReset (void);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart2RecvReset (void)
{
	RecvTop2 = 0;
	RecvEnd2 = 0;

	memset((void *)USART2RecvBuf, 0, USART2MAXBUFFSIZE); 
}



/*****************************************************************************
 函 数 名  : drv_Usart2RecvOne
 功能描述  : 读取1个字节
 输入参数  : u8 *Str  读出的字节存储在Str[0]
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
u8 drv_Usart2RecvOne (u8 *Str);
    修改内容   : 新生成函数

*****************************************************************************/
u8 drv_Usart2RecvOne (u8 *Str)
{
	if (RecvTop2 == RecvEnd2) return 0;//read nothing

	*Str = USART2RecvBuf[RecvTop2];
	RecvTop2++;

	if (RecvTop2 >= USART2MAXBUFFSIZE) RecvTop2 = 0;

	return 1;//read one

}



/*****************************************************************************
 函 数 名  : drv_Usart2RecvAtTime
 功能描述  : 接收串口数据
 输入参数  : u8 *Buff 接受数据存放的地方                 
             u16 RecvSize  接受多少个字节            
             u32 timeout_MilliSeconds  超时的毫秒数
 输出参数  : 无
 返 回 值  : 返回接受的总字节数
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
u16 drv_Usart2RecvAtTime (u8 *Buff, u16 RecvSize, u32 timeout_MilliSeconds);
    修改内容   : 新生成函数

*****************************************************************************/
u16 drv_Usart2RecvAtTime (u8 *Buff, u16 RecvSize, u32 timeout_MilliSeconds)
{
//	u16 RecvLen = 0;
//	u8 tmp[1] = {0};

//	if (RecvSize == 0) return 0;

//	g1msTimer5 = timeout_MilliSeconds;

//	while (1)
//	{
//		if (g1msTimer5 == 0) return RecvLen;

//		if (drv_Usart2RecvOne (tmp) == 1) 
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
u8 drv_Usart2Read(u8 *Buff, u16 len)
{

	u16 RecvLen = 0;
	u8 tmp[1] = {0};

	if (len == 0 || Buff == NULL) return 0;

	while (len--)
	{
		if (drv_Usart2RecvOne (tmp) == 1)
		{
			Buff[RecvLen++] = tmp[0];
		}

		if (RecvLen >= len) return RecvLen;
	}

	return RecvLen;
    
}



