#include "stdlib.h"
#include "sys.h"
#include "delay.h"
#include "drv_usart1.h"
#include "drv_usart2.h"
//#include "drv_usart.h"
#include "drv_led.h"
#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "cmd_fun.h"
#include "task.h"
#include "sfud.h"
//#include "drv_flash.h"
#include "drv_key.h"
#include "easyflash.h"
#include "drv_time.h"

#include "ini.h"
#include "test.h"

//任务优先级    
#define CMD_TASK_PRIO		( tskIDLE_PRIORITY + 4)
#define KEY_TASK_PRIO		( tskIDLE_PRIORITY + 4)
#define VKEY_TASK_PRIO		( tskIDLE_PRIORITY + 3)
#define START_TASK_PRIO		( tskIDLE_PRIORITY + 2)
#define LED0_TASK_PRIO		( tskIDLE_PRIORITY + 1)
#define LED1_TASK_PRIO		( tskIDLE_PRIORITY + 1)
#define FLOAT_TASK_PRIO		( tskIDLE_PRIORITY )

//任务堆栈大小	
#define START_STK_SIZE 		128  
#define LED0_STK_SIZE 		50 
#define LED1_STK_SIZE 		50  
#define FLOAT_STK_SIZE 		128
#define VKEY_STK_SIZE 		128
#define CMD_STK_SIZE 		128
#define KEY_STK_SIZE 		1024*2



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
    drv_key_Init();
    //  sfud_init();
    
	drv_Usart1Init(115200);     	//初始化串口
//	drv_Usart2Init(115200);     	//初始化串口	
	drv_LED_Init();		            //初始化LED端口	
    drv_TIM6_Init();                //定时器6初始化
    easyflash_init();
//    drv_InitKey();
    //vRegisterSampleCLICommands();
    CMD_Init();
    

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
                (const char*    )"vTaskMsgPro",   
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
    
#if V5 == 0
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

                    test_env();
					break;				
				/* K2键按下，打印串口操作命令 */
				case 2:
					FlashTest();
					break;
				case 3:
					ef_print_env();
					break;
				case 4:
                    printf("KEY_DOWN_K4\r\n");
                    RestoreDefaultSetting();
					break;                
				
				/* 其他的键值不处理 */
				default:   
				printf("KEY_default\r\n");
					break;
			}
		}
		
		vTaskDelay(20);
	}
    
    #else
    
    while(1)
    {
		ucKeyCode = drv_GetKey();
		
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				/* K1键按下 打印任务执行情况 */
				case KEY_DOWN_K1:			 
					printf("=================================================\r\n");
					printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
					vTaskList((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
                    
					printf("\r\n任务名       运行计数         使用率\r\n");
					vTaskGetRunTimeStats((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
					break;

				
				/* K2键按下，打印串口操作命令 */
				case KEY_DOWN_K2: 
                    FlashTest();
					break;
				case KEY_DOWN_K3:
                    ef_print_env();
					break;
				case KEY_DOWN_K4:
                    printf("KEY_DOWN_K4\r\n");
//					test_env();
                    break;
				case SYS_DOWN_K1K2:
					printf("组合按键\r\n");    
                    //RestoreDefaultSetting();
					break;                
				
				/* 其他的键值不处理 */
				default:                     
					break;
			}
		}
		
		vTaskDelay(20);
	}

    #endif
}


void vTaskMsgPro(void *pvParameters)
{
	uint8_t ucCMD;	
    while(1)
    {
		if (drv_Usart1RecvOne(&ucCMD))
		{
			switch (ucCMD)
			{
				case '1':
					printf( "接收到串口命令1\r\n");					
					break;

				case '2':
					printf( "接收到串口命令2\r\n");					
					break;

				case '3':
					printf( "接收到串口命令3\r\n");				
					break;

				case '4':
					printf( "接收到串口命令4\r\n");					
					break;	
			}
		}
		
		vTaskDelay(20);
    }
}





