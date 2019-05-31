#include "sys.h"
#include "delay.h"
#include "drv_usart1.h"
#include "drv_usart2.h"
#include "drv_led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sfud.h"

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		512  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

//任务优先级
#define FLOAT_TASK_PRIO		4
//任务堆栈大小	
#define FLOAT_STK_SIZE 		128
//任务句柄
TaskHandle_t FLOATTask_Handler;
//任务函数
void float_task(void *pvParameters);


//任务优先级
#define FLASH_TASK_PRIO		4
//任务堆栈大小	
#define FLASH_STK_SIZE 		256
//任务句柄
TaskHandle_t FLASHTask_Handler;
//任务函数
void flash_task(void *pvParameters);






int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);		//初始化延时函数
	drv_Usart1Init(115200);     	//初始化串口
	drv_Usart2Init(115200);     	//初始化串口	
	sfud_device_init(&gMyFlash);
	drv_LED_Init();		        //初始化LED端口	

	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
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
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //创建LED1任务
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);        
    //浮点测试任务
    xTaskCreate((TaskFunction_t )float_task,     
                (const char*    )"float_task",   
                (uint16_t       )FLOAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLOAT_TASK_PRIO,
                (TaskHandle_t*  )&FLOATTask_Handler);  

    //flash测试任务
    xTaskCreate((TaskFunction_t )flash_task,     
                (const char*    )"flash_task",   
                (uint16_t       )FLASH_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLASH_TASK_PRIO,
                (TaskHandle_t*  )&FLASHTask_Handler);                  

                
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

//串口收发
void float_task(void *pvParameters)
{
    u8 tmp[2] = {0};
	
	int i = 0;
	
	printf("i = %d\r\n",i++);
    
	while(1)
	{
        if(drv_Usart2RecvOne(tmp) == 1)
        {
            printf("receive data = %c\r\n",tmp[0]);
        }
        
        vTaskDelay(100);
	}    
}



//FLASH测试
void flash_task(void *pvParameters)
{
    uint8_t data[1024] = {0};    
    sfud_err result = SFUD_SUCCESS;    
    size_t size = 1024;
    uint32_t addr = 0;
    size_t i;
    
//    if(sfud_device_init(&gMyFlash) != SFUD_SUCCESS)
//    {
//        printf("flash initial error!\r\n");
//        return;
//    } 
    
    /* prepare write data */
    for (i = 0; i < size; i++) {
        data[i] = i;
    }
    /* erase test */
    result = sfud_erase(&gMyFlash, addr, size);
    if (result == SFUD_SUCCESS) {
        printf("Erase the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", gMyFlash.name, addr,
                size);
    } else {
        printf("Erase the %s flash data failed.\r\n", gMyFlash.name);
        return;
    }
    /* write test */
    result = sfud_write(&gMyFlash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        printf("Write the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", gMyFlash.name, addr,
                size);
    } else {
        printf("Write the %s flash data failed.\r\n", gMyFlash.name);
        return;
    }
    /* read test */
    result = sfud_read(&gMyFlash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        printf("Read the %s flash data success. Start from 0x%08X, size is %ld. The data is:\r\n", gMyFlash.name, addr,
                size);
        printf("Offset (h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
        for (i = 0; i < size; i++) {
            if (i % 16 == 0) {
                printf("[%08X] ", addr + i);
            }
            printf("%02X ", data[i]);
            if (((i + 1) % 16 == 0) || i == size - 1) {
                printf("\r\n");
            }
        }
        printf("\r\n");
    } else {
        printf("Read the %s flash data failed.\r\n", gMyFlash.name);
    }
    
    /* data check */
    for (i = 0; i < size; i++) {
        if (data[i] != i % 256) {
            printf("Read and check write data has an error. Write the %s flash data failed.\r\n", gMyFlash.name);
			break;
        }
    }

    if (i == size) {
        printf("The %s flash test is success.\r\n", gMyFlash.name);
    }    
}


