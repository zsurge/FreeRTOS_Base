/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : drv_usart1.c
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
#include "drv_usart1.h"

volatile u8 USART1RecvBuf[USART1MAXBUFFSIZE] = {0};
volatile u16 RecvTop1 = 0;
volatile u16 RecvEnd1 = 0;

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif



/*****************************************************************************
 函 数 名  : drv_Usart1Init
 功能描述  : 串口初始化
 输入参数  : u32 BaudRate  波特率
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart1Init (u32 BaudRate);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart1Init (u32 BaudRate)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1    

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断    
	
	USART_Cmd(USART1, ENABLE);  //使能串口1   

}

void USART1_IRQHandler (void)
{
	u8 temp = 0;

	//Receive data register not empty flag
	if (USART_GetITStatus (USART1, USART_IT_RXNE) != RESET)
	{
		temp = USART_ReceiveData (USART1);		//get received data

		if ( (RecvEnd1 == (RecvTop1 - 1) ) || ( (RecvTop1 == 0) && (RecvEnd1 == (USART1MAXBUFFSIZE - 1) ) ) )
		{
			//缓冲溢出
			RecvTop1 = 0;
			RecvEnd1 = 0;
		}
		else
		{
			USART1RecvBuf[RecvEnd1] = temp;
			RecvEnd1++;

			if (RecvEnd1 >= USART1MAXBUFFSIZE) RecvEnd1 = 0;
		}

		/* Clear the UART1 Recvive interrupt */
		USART_ClearITPendingBit (USART1, USART_IT_RXNE);
	}
	else if (USART_GetITStatus (USART1, USART_IT_NE | USART_IT_PE | USART_IT_FE | USART_IT_ORE) != RESET)
	{
		USART_GetITStatus (USART1, USART_IT_NE | USART_IT_PE | USART_IT_FE | USART_IT_ORE);
		USART_ReceiveData (USART1);
	}
}


void drv_Usart1SendOne (const u8 dat)
{
	USART_SendData (USART1, dat);

	while (USART_GetFlagStatus (USART1, USART_FLAG_TXE) == RESET) {}

	USART_ClearFlag (USART1, USART_FLAG_TXE);

}



/*****************************************************************************
 函 数 名  : drv_Usart1SendString
 功能描述  : 发送字符串
 输入参数  : const u8 *Buff  发送字符串
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart1SendString (const u8 *Buff);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart1SendString (const u8 *Buff)
{
	while (*Buff != 0)
	{
		drv_Usart1SendOne (*Buff);
		Buff++;
	}    
}


/*****************************************************************************
 函 数 名  : drv_Usart1SendData
 功能描述  : 发送指定长度字符
 输入参数  : const u8 *Buff  发送数据的buff
             u16 SendSize  发送数据的长度  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart1SendData (const u8 *Buff, u16 SendSize);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart1SendData (const u8 *Buff, u16 SendSize)
{
	while (SendSize != 0)
	{
		drv_Usart1SendOne (*Buff);
		Buff++;
		SendSize--;
	}

}



/*****************************************************************************
 函 数 名  : drv_Usart1RecvReset
 功能描述  : 清空串口接收缓冲
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
void drv_Usart1RecvReset (void);
    修改内容   : 新生成函数

*****************************************************************************/
void drv_Usart1RecvReset (void)
{
	RecvTop1 = 0;
	RecvEnd1 = 0;

	memset((void *)USART1RecvBuf, 0, USART1MAXBUFFSIZE); 
}



/*****************************************************************************
 函 数 名  : drv_Usart1RecvOne
 功能描述  : 读取1个字节
 输入参数  : u8 *Str  读出的字节存储在Str[0]
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
u8 drv_Usart1RecvOne (u8 *Str);
    修改内容   : 新生成函数

*****************************************************************************/
u8 drv_Usart1RecvOne (u8 *Str)
{
	if (RecvTop1 == RecvEnd1) return 0;//read nothing

	*Str = USART1RecvBuf[RecvTop1];
	RecvTop1++;

	if (RecvTop1 >= USART1MAXBUFFSIZE) RecvTop1 = 0;

	return 1;//read one

}



/*****************************************************************************
 函 数 名  : drv_USART1RecvAtTime
 功能描述  : 接收串口数据
 输入参数  : u8 *Buff 接受数据存放的地方                 
             u16 RecvSize  接受多少个字节            
             u32 timeout_MilliSeconds  超时的毫秒数
 输出参数  : 无
 返 回 值  : 返回接受的总字节数
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
u16 drv_USART1RecvAtTime (u8 *Buff, u16 RecvSize, u32 timeout_MilliSeconds);
    修改内容   : 新生成函数

*****************************************************************************/
u16 drv_Usart1RecvAtTime (u8 *Buff, u16 RecvSize, u32 timeout_MilliSeconds)
{
//	u16 RecvLen = 0;
//	u8 tmp[1] = {0};

//	if (RecvSize == 0) return 0;

//	g1msTimer5 = timeout_MilliSeconds;

//	while (1)
//	{
//		if (g1msTimer5 == 0) return RecvLen;

//		if (drv_USART1RecvOne (tmp) == 1) 
//		{
//			Buff[RecvLen++] = tmp[0];
//		}

//		if (RecvLen >= RecvSize) return RecvLen;
//	}
	return 0;
}


/*****************************************************************************
 函 数 名  : dev_Usart1Read
 功能描述  : 读串口数据
 输入参数  : unsigned char *buf  
             int len             
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月22日
    作    者   : 张舵
int dev_Usart1Read(unsigned char *buf, int len)
    修改内容   : 新生成函数

*****************************************************************************/
u8 drv_Usart1Read(u8 *Buff, u16 len)
{

	u16 RecvLen = 0;
	u8 tmp[1] = {0};

	if (len == 0 || Buff == NULL) return 0;

	while (len--)
	{
		if (drv_Usart1RecvOne (tmp) == 1)
		{
			Buff[RecvLen++] = tmp[0];
		}

		if (RecvLen >= len) return RecvLen;
	}

	return RecvLen;
    
}



  
const char * const g_pcHex = "0123456789abcdef";  
  
/** 
* 简介:   一个简单的printf函数,支持\%c, \%d, \%p, \%s, \%u,\%x, and \%X. 
*/  
void UARTprintf(const uint8_t *pcString, ...)  
{  
    uint32_t ulIdx;  
    uint32_t ulValue;       //保存从不定量参数堆栈中取出的数值型变量  
    uint32_t ulPos, ulCount;  
    uint32_t ulBase;        //保存进制基数,如十进制则为10,十六进制数则为16  
    uint32_t ulNeg;         //为1表示从变量为负数  
    uint8_t *pcStr;         //保存从不定量参数堆栈中取出的字符型变量  
    uint8_t pcBuf[32];      //保存数值型变量字符化后的字符  
    uint8_t cFill;          //'%08x'->不足8个字符用'0'填充,cFill='0';    
                            //'%8x '->不足8个字符用空格填充,cFill=' '  
    va_list vaArgP;  
  
    va_start(vaArgP, pcString);  
    while(*pcString)  
    {  
        // 首先搜寻非%核字符串结束字符  
        for(ulIdx = 0; (pcString[ulIdx] != '%') && (pcString[ulIdx] != '\0'); ulIdx++)  
        { }  
        
        drv_Usart1SendData(pcString, ulIdx);  
  
        pcString += ulIdx;  
        if(*pcString == '%')  
        {  
            pcString++;  
  
            ulCount = 0;  
            cFill = ' ';  
again:  
            switch(*pcString++)  
            {  
                case '0': case '1': case '2': case '3': case '4':  
                case '5': case '6': case '7': case '8': case '9':  
                {  
                    // 如果第一个数字为0, 则使用0做填充,则用空格填充)  
                    if((pcString[-1] == '0') && (ulCount == 0))  
                    {  
                        cFill = '0';  
                    }  
                    ulCount *= 10;  
                    ulCount += pcString[-1] - '0';  
                    goto again;  
                }  
                case 'c':          
                {  
                    ulValue = va_arg(vaArgP, unsigned long);  
                    drv_Usart1SendData((unsigned char *)&ulValue, 1);  
                    break;  
                }  
                case 'd':     
                {  
                    ulValue = va_arg(vaArgP, unsigned long);  
                    ulPos = 0;  
                      
                    if((long)ulValue < 0)  
                    {  
                        ulValue = -(long)ulValue;  
                        ulNeg = 1;  
                    }  
                    else  
                    {  
                        ulNeg = 0;  
                    }  
                    ulBase = 10;          
                    goto convert;  
                }  
                case 's':  
                {  
                    pcStr = va_arg(vaArgP, unsigned char *);  
  
                    for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++)  
                    {  
                    }  
                    drv_Usart1SendData(pcStr, ulIdx);  
  
                    if(ulCount > ulIdx)  
                    {  
                        ulCount -= ulIdx;  
                        while(ulCount--)  
                        {  
                            drv_Usart1SendData(" ", 1);  
                        }  
                    }  
                    break;  
                }  
                case 'u':  
                {  
                    ulValue = va_arg(vaArgP, unsigned long);  
                    ulPos = 0;  
                    ulBase = 10;  
                    ulNeg = 0;  
                    goto convert;  
                }  
                case 'x': case 'X': case 'p':  
                {  
                    ulValue = va_arg(vaArgP, unsigned long);  
                    ulPos = 0;  
                    ulBase = 16;  
                    ulNeg = 0;  
         convert:   //将数值转换成字符  
                    for(ulIdx = 1; (((ulIdx * ulBase) <= ulValue) &&(((ulIdx * ulBase) / ulBase) == ulIdx)); ulIdx *= ulBase, ulCount--)       
                    { }  
                    if(ulNeg)  
                    {  
                        ulCount--;                        
                    }  
                    if(ulNeg && (cFill == '0'))  
                    {  
                        pcBuf[ulPos++] = '-';  
                        ulNeg = 0;  
                    }  
                    if((ulCount > 1) && (ulCount < 16))  
                    {  
                        for(ulCount--; ulCount; ulCount--)  
                        {  
                            pcBuf[ulPos++] = cFill;  
                        }  
                    }  
  
                    if(ulNeg)  
                    {  
                        pcBuf[ulPos++] = '-';  
                    }  
  
                    for(; ulIdx; ulIdx /= ulBase)  
                    {  
                        pcBuf[ulPos++] = g_pcHex[(ulValue / ulIdx) % ulBase]; 
                    }  
                    drv_Usart1SendData(pcBuf, ulPos);  
                    break;  
                }  
                case '%':  
                {  
                    drv_Usart1SendData(pcString - 1, 1);                    
                    break;  
                }  
                default:  
                {                      
                    drv_Usart1SendData("ERROR", 5);                    
                    break;  
                }  
            }  
        }  
    }  
    //可变参数处理结束  
    va_end(vaArgP);  
} 




