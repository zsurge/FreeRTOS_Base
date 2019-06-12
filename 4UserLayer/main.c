//标准库头文件
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


//FreeRTOS头文件
#include "FreeRTOS.h"
#include "task.h"

//驱动层头文件
#include "drv_usart1.h"
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
#include "iap.h"


//任务优先级    
#define KEY_TASK_PRIO		( tskIDLE_PRIORITY + 4)
#define LED0_TASK_PRIO		( tskIDLE_PRIORITY + 3)
#define LED1_TASK_PRIO		( tskIDLE_PRIORITY + 3)
#define CheckUP_TASK_PRIO	( tskIDLE_PRIORITY + 2)
#define START_TASK_PRIO		( tskIDLE_PRIORITY )

//任务堆栈大小	
#define START_STK_SIZE 		128  
#define LED0_STK_SIZE 		50 
#define LED1_STK_SIZE 		50  
#define KEY_STK_SIZE 		1024
#define CheckUP_STK_SIZE 		1024




//任务句柄
TaskHandle_t StartTask_Handler = NULL;
TaskHandle_t LED0Task_Handler = NULL;
TaskHandle_t LED1Task_Handler = NULL;
TaskHandle_t KEYTask_Handler = NULL;
TaskHandle_t CheckUP_Handler = NULL;





//任务函数
void start_task(void *pvParameters);
void led0_task(void *pvParameters);
void led1_task(void *pvParameters);
void Key_task(void *pvParameters);
void CheckUP_task(void *pvParameters);




int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);		        //初始化延时函数
	
    drv_TIM6_Init();                //定时器6初始化    
    STM_FLASH_Init();               //芯片内部FLASH初始化
    
	drv_Usart1_Init(115200);     	//初始化串口 YMODEM使用
	xUsart2Init(115200);            //USART2初始化，FreeRTOS CLI 使用
//	drv_Usart3_Init(115200);     	//初始化串口3	
	drv_key_Init();                 //按键初始化
	easyflash_init();               //外部FLASH初始化
    CMD_Init();                     //FreeRTOS CLI 注册
    
	drv_LED_Init();		            //初始化LED端口	
    

    

	//创建开始任务
    xTaskCreate((TaskFunction_t )CheckUP_task,            //任务函数
                (const char*    )"CheckUP_task",          //任务名称
                (uint16_t       )CheckUP_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )CheckUP_TASK_PRIO,        //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}
 
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区

    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,       
                (const char*    )"led0_task",     
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,                
                (UBaseType_t    )CheckUP_TASK_PRIO,  
                (TaskHandle_t*  )&CheckUP_Handler);   




//    vUARTCommandConsoleStart(1024,4); //FreeRTOS CLI
    
//    //创建LED0任务
//    xTaskCreate((TaskFunction_t )led0_task,     	
//                (const char*    )"led0_task",   	
//                (uint16_t       )LED0_STK_SIZE, 
//                (void*          )NULL,				
//                (UBaseType_t    )LED0_TASK_PRIO,	
//                (TaskHandle_t*  )&LED0Task_Handler);   
//    //创建LED1任务
//    xTaskCreate((TaskFunction_t )led1_task,     
//                (const char*    )"led1_task",   
//                (uint16_t       )LED1_STK_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )LED1_TASK_PRIO,
//                (TaskHandle_t*  )&LED1Task_Handler);                  
//    
//    //键盘测试
//    xTaskCreate((TaskFunction_t )Key_task,     
//                (const char*    )"Key_task",   
//                (uint16_t       )KEY_STK_SIZE, 
//                (void*          )NULL,
//                (UBaseType_t    )KEY_TASK_PRIO,
//                (TaskHandle_t*  )&KEYTask_Handler);    

                
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
					ef_print_env();
					break;
				case 4:
                    printf("KEY_DOWN_K4\r\n");
                    //RestoreDefaultSetting();
                    IAP_DownLoadToFlash();
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


void CheckUP_task(void *pvParameters)
{
    uint32_t upValue1 = 0;
    uint32_t upValue2 = 0;
    char *UpgradeFlag1;
    char *UpgradeFlag2;

    while(1)
    {   

        UpgradeFlag1 = ef_get_env("UpgradeFlag");

        upValue1 = atol(UpgradeFlag1);

        UpgradeFlag2 = ef_get_env("UserAPPFlag");

        upValue2 = atol(UpgradeFlag2);
        
        if(upValue1 == 0 && upValue2 == 1)
        {
            //不需要升级，进入APP
			printf("APP STATR!\r\n");
			IAP_JumpToApplication();
        }
        else
        {
			printf("IAP STATR! %d   %d\r\n",upValue1,upValue2);+

            //如果外部FLASH写完，但是内部FLASH没写（正在写内部FLASH时断电）
            
            //需要升级，进入IAP升级流程
            IAP_DownLoadToFlash();

            printf("write stm flash\r\n");
            IAP_DownLoadToSTMFlash();
        }
        

        vTaskDelay(100);
    }
}





