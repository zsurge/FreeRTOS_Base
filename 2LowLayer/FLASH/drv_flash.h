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
#include "sys.h"  
#include "delay.h"

#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

//GPIOA define
#define FLASH_PORT			GPIOB
#define FLASH_CS_PIN		GPIO_Pin_14
#define FLASH_SCK_PIN		GPIO_Pin_3
#define FLASH_MISO_PIN		GPIO_Pin_4
#define FLASH_MOSI_PIN		GPIO_Pin_5

#define FLASH_CS_PINSource		GPIO_PinSource14
#define FLASH_SCK_PINSource		GPIO_PinSource3
#define FLASH_MISO_PINSource	GPIO_PinSource4
#define FLASH_MOSI_PINSource	GPIO_PinSource5

#define FLASH_WriteEnable		0x06 
#define FLASH_WriteDisable		0x04 
#define FLASH_ReadStatusReg		0x05 
#define FLASH_WriteStatusReg	0x01 
#define FLASH_ReadData			0x03 
#define FLASH_FastReadData		0x0B 
#define FLASH_FastReadDual		0x3B 
#define FLASH_PageProgram		0x02 
#define FLASH_BlockErase		0xD8 
#define FLASH_SectorErase		0x20 
#define FLASH_ChipErase			0xC7 
#define FLASH_PowerDown			0xB9 
#define FLASH_ReleasePowerDown	0xAB 
#define FLASH_DeviceID			0xAB 
#define FLASH_ManufactDeviceID	0x90 
#define FLASH_JedecDeviceID		0x9F 


extern u16 FLASH_TYPE;					//定义W25QXX芯片型号		   

#define	FLASH_CS 			PBout(14)  	//W25QXX的片选信号

#define FLASH_USE_MALLOC	0			//定义是否使用动态内存管理

void drv_Flash_Init( void );

u16  drv_FLASH_ReadID(void);  	    		//读取FLASH ID
u8   drv_FLASH_ReadSR(void);        		//读取状态寄存器 
void drv_FLASH_Write_SR(u8 sr);  			//写状态寄存器
void drv_FLASH_Write_Enable(void);  		//写使能 
void drv_FLASH_Write_Disable(void);		//写保护
void drv_FLASH_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void drv_FLASH_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void drv_FLASH_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void drv_FLASH_Erase_Chip(void);    	  	//整片擦除
void drv_FLASH_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void drv_FLASH_Wait_Busy(void);           	//等待空闲
void drv_FLASH_PowerDown(void);        	//进入掉电模式
void drv_FLASH_WAKEUP(void);				//唤醒







#endif



