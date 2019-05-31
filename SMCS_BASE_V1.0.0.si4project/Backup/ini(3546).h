/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : ini.h
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年5月28日
  最近修改   :
  功能描述   : 对参数进行操作
  函数列表   :
  修改历史   :
  1.日    期   : 2019年5月28日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

#ifndef __INI_H
#define __INI_H
#include "easyflash.h"
#include "sfud.h"
#include "stdio.h"


#define SF_VERSION "V1.0.0.1"
#define HD_VERSION "V1.0"

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
    


EfErrCode RestoreDefaultSetting(ef_env const ** defaultValue, size_t *defaultSize);





#endif
