/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : iap.c
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年6月4日
  最近修改   :
  功能描述   : 串口升级程序
  函数列表   :
  修改历史   :
  1.日    期   : 2019年6月4日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "iap.h"
#include "ymodem.h"
#include <stdio.h>
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
typedef  void (*pFunction)(void); 
pFunction Jump_To_Application;

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
uint8_t buf_1k[1024] = {0};

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

static void Int2Str(uint8_t* str, int32_t intnum);




//Convert an interger to a string
//str: converted string
//intnum: integer waiting for converting
static void Int2Str(uint8_t* str, int32_t intnum)
{
  uint32_t i, Div = 1000000000, j = 0, Status = 0;

  for (i = 0; i < 10; i++)
  {
    str[j++] = (intnum / Div) + 48;

    intnum = intnum % Div;
    Div /= 10;
    if ((str[j-1] == '0') & (Status == 0))
    {
      j = 0;
    }
    else
    {
      Status++;
    }
  }
}




void IAP_JumpToApplication(void)
{
    uint32_t JumpAddress = 0;//跳转地址
    
    if ( ( (* (volatile uint32_t*) APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
    {
		/* Jump to user application */
		JumpAddress = *(volatile uint32_t*) (APPLICATION_ADDRESS + 4);
		Jump_To_Application = (pFunction) JumpAddress;

		/* Initialize user application's Stack Pointer */
		__set_PSP(*(volatile uint32_t*) APPLICATION_ADDRESS);

		Jump_To_Application();	
        
		while(1);
    }
}

void IAP_DownLoadToFlash(void)
{
    uint8_t Number[10] = {0};
	int32_t Size = 0;

    printf("------IAP_DownLoadToFlash start----------\n");
    
    Size = Ymodem_Receive(&buf_1k[0],APPLICATION_ADDRESS);

    if (Size > 0)
	{
		printf("-------------------\n");
		printf("\n\r Programming Completed Successfully!\n\r----------------------\r\n Name: ");
		printf((char*)FileName);
		Int2Str(Number, Size);
		printf("\n\r Size: ");
		printf((char*)Number);
		printf(" Bytes\r\n");
	}
}


