//标准库头文件
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


//FreeRTOS头文件
#include "FreeRTOS.h"
#include "task.h"

//驱动层头文件
//#include "drv_flash.h"
//#include "drv_usart.h"
#include "drv_usart1.h"
//#include "drv_usart2.h"
#include "drv_usart3.h"
#include "drv_key.h"
#include "drv_time.h"
#include "drv_led.h"


//中间件头文件
#include "sys.h"
#include "delay.h"

#include "FreeRTOS_CLI.h"
#include "stmflash.h"
#include "easyflash.h"
#include "cmd_fun.h"
#include "sfud.h"
#include "ini.h"


//用户层头文件
#include "test.h"
#include "iap.h"
#include "comm.h"



//任务优先级    
#define CMD_TASK_PRIO		( tskIDLE_PRIORITY + 1)
#define KEY_TASK_PRIO		( tskIDLE_PRIORITY + 4)
#define VKEY_TASK_PRIO		( tskIDLE_PRIORITY + 3)
#define START_TASK_PRIO		( tskIDLE_PRIORITY + 2)
#define LED0_TASK_PRIO		( tskIDLE_PRIORITY + 3)
#define LED1_TASK_PRIO		( tskIDLE_PRIORITY + 3)
#define FLOAT_TASK_PRIO		( tskIDLE_PRIORITY )

//任务堆栈大小	
#define START_STK_SIZE 		128  
#define LED0_STK_SIZE 		50 
#define LED1_STK_SIZE 		50  
#define FLOAT_STK_SIZE 		128
#define VKEY_STK_SIZE 		128
#define CMD_STK_SIZE 		1024
#define KEY_STK_SIZE 		1024



//任务句柄
TaskHandle_t StartTask_Handler = NULL;
TaskHandle_t LED0Task_Handler = NULL;
TaskHandle_t LED1Task_Handler = NULL;
TaskHandle_t FLOATTask_Handler = NULL;
TaskHandle_t VKEYTask_Handler = NULL;
TaskHandle_t CMDTask_Handler = NULL;
TaskHandle_t KEYTask_Handler = NULL;


//任务函数
void start_task(void *pvParameters);
void led0_task(void *pvParameters);
void led1_task(void *pvParameters);
void float_task(void *pvParameters);
void vkeyTaskStart(void *pvParameters);
void vTaskMsgPro(void *pvParameters);
void Key_task(void *pvParameters);


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);		//初始化延时函数

    //  drv_FlashInit();
    //  drv_UartInit();    
    //  sfud_init();
    //  drv_InitKey();

    
    STM_FLASH_Init();               //芯片内部FLASH初始化
    
	drv_Usart1_Init(115200);     	//初始化串口
	xUsart2Init(115200);            //drv_Usart2_Init(115200);     	//初始化串口	
//	drv_Usart3_Init(115200);     	//初始化串口	
	
	drv_key_Init();                 //按键初始化
	drv_LED_Init();		            //初始化LED端口	
    drv_TIM6_Init();                //定时器6初始化
    easyflash_init();               //外部FLASH初始化
    CMD_Init();                     //FreeRTOS CLI 注册

    init_serial_boot();
    

	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,        //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}
 
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区

    vUARTCommandConsoleStart(1024,4);
    
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    
    //创建LED1任务
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);  

    //按键扫描
//    xTaskCreate((TaskFunction_t )vkeyTaskStart,     
//                (const char*    )"vkeyTaskStart",   
//                (uint16_t       )VKEY_STK_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )VKEY_TASK_PRIO,
//                (TaskHandle_t*  )&VKEYTask_Handler);                       
    
    //键盘测试
    xTaskCreate((TaskFunction_t )Key_task,     
                (const char*    )"Key_task",   
                (uint16_t       )KEY_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t*  )&KEYTask_Handler);                 
                

    //CMD测试任务
    xTaskCreate((TaskFunction_t )vTaskMsgPro,     
                (const char*    )"cmd",   
                (uint16_t       )CMD_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )CMD_TASK_PRIO,
                (TaskHandle_t*  )&CMDTask_Handler);                  

                
    vTaskDelete(StartTask_Handler); //删除开始任务	
				
    taskEXIT_CRITICAL();            //退出临界区
}

//LED0任务函数 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED0=~LED0;
        vTaskDelay(500);
    }
}   

//LED1任务函数
void led1_task(void *pvParameters)
{
    while(1)
    {
        LED1=0;
        vTaskDelay(200);
        LED1=1;
        vTaskDelay(800);
    }
}

//void vkeyTaskStart(void *pvParameters)
//{
//    while(1)
//    {
//		/* 按键扫描 */
//		drv_KeyScan();
//        vTaskDelay(10);
//    }
//}

void Key_task(void *pvParameters)
{
    
	uint8_t ucKeyCode;
	uint8_t pcWriteBuffer[500];
    
    while(1)
    {
		ucKeyCode = drv_key_Scan(0);
		
		if (ucKeyCode != 0)
		{
			switch (ucKeyCode)
			{
				/* K1键按下 打印任务执行情况 */
				case 1:			 
					printf("=================================================\r\n");
					printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
					vTaskList((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
                    
					printf("\r\n任务名       运行计数         使用率\r\n");
					vTaskGetRunTimeStats((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);                    
					break;				
				/* K2键按下，打印串口操作命令 */
				case 2:
					//FlashTest();
					ReadIAP();
					break;
				case 3:
					//ef_print_env();
					//SystemReset();
					break;
				case 4:
                    printf("KEY_DOWN_K4\r\n");
                    drv_Usart1_SendString("1234");
                    //ef_erase_bak_app( 0x10000 ); 
                    //RestoreDefaultSetting();
                    //IAP_DownLoadToFlash();
					break;                
				
				/* 其他的键值不处理 */
				default:   
				printf("KEY_default\r\n");
					break;
			}
		}
		
		vTaskDelay(20);
	}   

}


void vTaskMsgPro(void *pvParameters)
{
    
    while(1)
    {
        DealSerialParse();    
	    DealRxData();
		vTaskDelay(10);
    }
}



