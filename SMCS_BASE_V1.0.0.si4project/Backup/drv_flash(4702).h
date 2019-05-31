/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : drv_flash.h
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年5月23日
  最近修改   :
  功能描述   : 外部FLASH读写操作
  函数列表   :
  修改历史   :
  1.日    期   : 2019年5月23日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

#ifndef __DRV_FLASH_H
#define __DRV_FLASH_H



void drv_FlashInit (void);



unsigned char drv_FlashSendByte (unsigned char byte);



void drv_FlashEraseSector (unsigned long SectorAddr);


void drv_FlashEraseBulk (void);




void drv_FlashWriteEnable (void);


void drv_FlashWaitForWriteEnd (void);



void drv_FlashWriteBuffer (const unsigned char* pBuffer, unsigned long WriteAddr, unsigned int NumByteToWrite);


void drv_FlashWritePage (const unsigned char* pBuffer, unsigned long WriteAddr, unsigned int NumByteToWrite);





unsigned long drv_FlashReadID (void);


void drv_FlashReadBuffer (unsigned char* pBuffer, unsigned long ReadAddr, unsigned int NumByteToRead);

void drv_FlashEraseBlock(unsigned long SectorAddr);


#endif



