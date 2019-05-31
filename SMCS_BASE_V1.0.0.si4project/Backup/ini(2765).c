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



const ef_env default_param_value[] = {
    {"iap_need_copy_app","0"},
    {"iap_copy_app_size","0"},
    {"stop_in_bootloader","0"},
    {"device_id","1"},
    {"boot_times","0"},
//以上为base版通用参数    
    {"HardWare Version",HD_VERSION},
    {"SoftWare Version",SF_VERSION}

};



/*****************************************************************************
 函 数 名  : RestoreDefaultSetting
 功能描述  : 恢复默认设置
 输入参数  : defaultValue
             defaultSize
 输出参数  : 无
 返 回 值  : void
 
 修改历史      :
  1.日    期   : 2019年5月28日
    作    者   : 张舵

    修改内容   : 新生成函数

*****************************************************************************/
void RestoreDefaultSetting(void)
{
    ef_env const ** defaultValue;
    size_t *defaultSize;
    
    *defaultValue = default_param_value;
    *defaultSize = sizeof(default_param_value) / sizeof(default_param_value[0]);
     sfud_init();

    
}

