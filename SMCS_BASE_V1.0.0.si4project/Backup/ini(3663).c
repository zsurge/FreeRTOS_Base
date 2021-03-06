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
EfErrCode RestoreDefaultSetting(ef_env const ** defaultValue, size_t *defaultSize )
{
    EfErrCode result = EF_NO_ERR;
    
    *defaultValue = default_param_value;
    *defaultSize = sizeof(default_param_value) / sizeof(default_param_value[0]);
    sfud_init();

    return result;
}

