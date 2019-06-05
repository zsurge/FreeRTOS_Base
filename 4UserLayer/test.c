/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : test.c
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年5月28日
  最近修改   :
  功能描述   : 功能模块测试
  函数列表   :
  修改历史   :
  1.日    期   : 2019年5月28日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#include "test.h"


void FlashTest(void)
{
    uint8_t data[4096] = {0};    
    sfud_err result = SFUD_SUCCESS;    
    size_t size = sizeof(data);
    uint32_t addr = 4096;
    size_t i;

    const sfud_flash * myFlash = sfud_get_device_table() + 0;
    
    /* prepare write data */
    for (i = 0; i < size; i++) {
        data[i] = i;
    }
    /* erase test */
    result = sfud_erase(myFlash, addr, size);
    if (result == SFUD_SUCCESS) {
        printf("Erase the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", myFlash->name, addr,
                size);
    } else {
        printf("Erase the %s flash data failed.\r\n", myFlash->name);
        return;
    }
    /* write test */
    result = sfud_write(myFlash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        printf("Write the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", myFlash->name, addr,
                size);
    } else {
        printf("Write the %s flash data failed.\r\n", myFlash->name);
        return;
    }
    /* read test */
    result = sfud_read(myFlash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        printf("Read the %s flash data success. Start from 0x%08X, size is %ld. The data is:\r\n", myFlash->name, addr,
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
        printf("Read the %s flash data failed.\r\n", myFlash->name);
    }
    
    /* data check */
    for (i = 0; i < size; i++) {
        if (data[i] != i % 256) {
            printf("Read and check write data has an error. Write the %s flash data failed.\r\n", myFlash->name);
			break;
        }
    }

    if (i == size) {
        printf("The %s flash test is success.\r\n", myFlash->name);
    }    
}




void COMProcessHelp(void)
{
	printf("==========================================\r\n");
	printf("请选择串口操作命令，电脑键盘打印数字即可:\r\n");
	printf("1. 接收到命令1，打印消息\r\n");
	printf("2. 接收到命令2，打印消息\r\n");
	printf("3. 接收到命令3，打印消息\r\n");
	printf("4. 接收到命令4，打印消息\r\n");
}

void test_env(void)
{
     uint32_t i_boot_times = NULL;
    char *c_old_boot_times, c_new_boot_times[11] = {0};

    /* get the boot count number from Env */
    c_old_boot_times = ef_get_env("boot_times");
    assert_param(c_old_boot_times);
    i_boot_times = atol(c_old_boot_times);
    
    /* boot count +1 */
    i_boot_times ++;
    printf("The system now boot %d times\r\n", i_boot_times);
    /* interger to string */
    sprintf(c_new_boot_times,"%ld", i_boot_times);
    
    /* set and store the boot count number to Env */
    ef_set_env("boot_times", c_new_boot_times);
    ef_set_env("p1","wangwu");
    ef_set_env("p2","zhangsan");
    ef_set_env("p3","lisi");

    c_old_boot_times = ef_get_env("p1");
    
    printf("p1 = %s\r\n", c_old_boot_times);

    c_old_boot_times = ef_get_env("p2");

    printf("p2 = %s\r\n", c_old_boot_times);

    c_old_boot_times = ef_get_env("p3");

    printf("p3 = %s\r\n", c_old_boot_times);       

    

    printf("------------test_env end----------\r\n");
    
    
}









