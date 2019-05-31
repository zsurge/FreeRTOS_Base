#ifndef __DRV_TIME_H
#define __DRV_TIME_H
#include "stm32f0xx.h"

extern __IO unsigned int g1msTimerUART1;
extern __IO unsigned int g1msTimerUART2;
extern __IO unsigned int g1msTimerUART4;

extern __IO unsigned int gTcpTimeout;
extern __IO unsigned int gHeartbeatTimeout;



//unsigned char drv_SysTickInit (void);
void drv_TimeSysTickHandler (void);
void sleep (unsigned int second);
void msleep (unsigned int ms);
void drv_TIM3_Init(void);
uint16_t drv_Usart1RecvAtTime (uint8_t *Buff, uint16_t RecvSize, uint32_t timeout_MilliSeconds);

#endif

