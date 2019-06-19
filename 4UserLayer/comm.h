/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : comm.h
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年6月18日
  最近修改   :
  功能描述   : 串口通讯协议解析及处理
  函数列表   :
  修改历史   :
  1.日    期   : 2019年6月18日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/
#ifndef __COMM_H
#define __COMM_H

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define FH		    					0x7E	/* frame head */
#define FT                              0xAA    /* frame tail */
#define CMDSUCC	    					0x06	/* command success */
#define CMDERRO	    					0x15	/* command error */
#define UPDAT							0x13	/* Upload data */
    
    
#define SPACE		        			0x00
#define FINISH		       	 			0x55

#define GETSENSOR                       0xA1
#define SETLED                          0xA2
#define GETDEVICESAT                    0xA3
#define GETVER                          0xA4
#define UPSENSOR                        0xA5
#define HEARTBEAT                       0xA6
#define UPGRADE                         0xA7


/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
void DealRxData(void);
void DealSerialParse(void);
void init_serial_boot(void);

void SendData(unsigned char frame_type);


#endif

