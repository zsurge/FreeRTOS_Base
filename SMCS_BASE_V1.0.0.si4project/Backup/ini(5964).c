/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : ini.c
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年5月28日
  最近修改   :
  功能描述   : 参数设置
  函数列表   :
  修改历史   :
  1.日    期   : 2019年5月28日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "ini.h"

static const ef_env default_param[] = 
{
   {"boot_times","0"},
   {"device_id","1"},
   {"software version","v1.0.0.0"},
   {"hardware version","v1.0"},
   {"key1","key1_value"},
   {"key2","key2_value"},
   {"key3","key3_value"},
   {"key4","key4_value"},
   {"key5","key5_value"},
   {"key6","key6_value"},
   {"key7","key7_value"},
   {"key8","key8_value"}, 
};


#define param_start_addr 0x00

/*****************************************************************************
 函 数 名  : RestoreDefaultSetting
 功能描述  : 恢复默认设置,实际是写入系统启动所需要的参数
           若使用ef_env_set_default 刚需要在ef_port.c中对默认值进行修改
 输入参数  : 
             
 输出参数  : 无
 返 回 值  : void
 
 修改历史      :
  1.日    期   : 2019年5月28日
    作    者   : 张舵

    修改内容   : 新生成函数

*****************************************************************************/
void RestoreDefaultSetting(void)
{
    size_t i = 0;
    size_t default_param_size = sizeof(default_param) / sizeof(default_param[0]);
    size_t add = EF_START_ADDR;
    
    //擦除FLASH
    for(add=EF_START_ADDR;add<ENV_AREA_SIZE+EF_START_ADDR;add+=EF_ERASE_MIN_SIZE)
    {
        if(ef_port_erase(EF_START_ADDR, EF_ERASE_MIN_SIZE) != EF_NO_ERR)
        {
            printf("ef_port_erase error\r\n");
            return;
        }        
    }

    printf("ef_port_erase success\r\n");

    //重写默认参数
    for(i= 0;i<default_param_size;i++)
    {
        if(ef_set_env(default_param[i].key, default_param[i].value) != EF_NO_ERR)
        {
            printf("i = %d ef_port_erase error\r\n",i);
            return;
        }

        printf("key = %s, value = %s\r\n",default_param[i].key, default_param[i].value);
    }  

    printf("RestoreDefaultSetting success\r\n");
}

