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
EfErrCode RestoreDefaultSetting(void)
{
    EfErrCode result = EF_NO_ERR;
    uint32_t addr, i, value_len;
    struct sector_meta_data sector;

    size_t default_param_size = sizeof(default_param) / sizeof(default_param[0]);
    
    EF_ASSERT(default_param);
    EF_ASSERT(default_param_size);

    /* lock the ENV cache */
    ef_port_env_lock();
    /* format all sectors */
    for (addr = param_start_addr; addr < param_start_addr + ENV_AREA_SIZE; addr += SECTOR_SIZE) {
        result = format_sector(addr, SECTOR_NOT_COMBINED);
        if (result != EF_NO_ERR) {
            goto __exit;
        }
    }
    /* create default ENV */
    for (i = 0; i < default_param_size; i++) {
        /* It seems to be a string when value length is 0.
         * This mechanism is for compatibility with older versions (less then V4.0). */
        if (default_param[i].value_len == 0) {
            value_len = strlen(default_param[i].value);
        } else {
            value_len = default_param[i].value_len;
        }
        sector.empty_env = FAILED_ADDR;
        create_env_blob(&sector, default_param[i].key, default_param[i].value, value_len);
        if (result != EF_NO_ERR) {
            goto __exit;
        }
    }

__exit:
    /* unlock the ENV cache */
    ef_port_env_unlock();

    return result;    
}

