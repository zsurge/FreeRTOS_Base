# FreeRTOS_Base
FreeRTOS+SFUD+EasyFlash+FreeRTOS_CLI 的基础版本  
基于FreeRTOS V10.2.1/STM32F407ZGT6/EASYFLASH V4.0  

### V1.0.0.20190605
1. 统一函数命名规范
2. 添加了Ymodem协议
3. 支持IAP升级


### V1.0.0.20190531
1. FreeRTOS+SFUD+EasyFlash+FreeRTOS_CLI 基础版本
2. 串口调试使用的是USART1
3. FreeRTOS CLI 使用的是 USART2
4. port.c中有一个 while(i++<2800)的语句，必须要添加，要不FreeRTOS CLI显示不全
