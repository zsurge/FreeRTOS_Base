/******************************************************************************

                  版权所有 (C), 2013-2023, 深圳博思高科技有限公司

 ******************************************************************************
  文 件 名   : comm.c
  版 本 号   : 初稿
  作    者   : 张舵
  生成日期   : 2019年6月18日
  最近修改   :
  功能描述   : 解析串口指令
  函数列表   :
  修改历史   :
  1.日    期   : 2019年6月18日
    作    者   : 张舵
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "comm.h"
#include "drv_usart1.h"

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define MAX_RXD_BUF_LEN        			100
#define MAX_TXD_BUF_LEN					100      
        
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
typedef struct
{
    volatile uint8_t RxdStatus;                 //接收状态
    volatile uint8_t RxCRC;                     //效果值
    volatile uint8_t RxdFrameStatus;            //接收包状态
    volatile uint8_t NowLen;                    //接收字节指针
    volatile uint8_t RxdTotalLen;               //接收包数据长度
    volatile uint8_t RxdBuf[MAX_RXD_BUF_LEN];   //接收包数据缓存    
}COMMAND_T;

static COMMAND_T gcmd;


/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

    
void init_serial_boot(void)
{
    gcmd.RxdStatus = 0;
    gcmd.RxCRC = 0;
    gcmd.RxdFrameStatus = SPACE;
}

/*****************************************************************************
 函 数 名  : DealSerialParse
 功能描述  :     将收到的数据解析成数据包；
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年6月18日
    作    者   : 张舵
    修改内容   : 新生成函数

*****************************************************************************/
// 7E 01 A1 00 AA 74 
// 7E 01 A1 AA 25 48 00 AA 74 7E 01 A1 00 AA 74

static void dbh(int index,char *buf,int len)
{
    int i = 0;
    printf("%d. -----------------\r\n",index);
    for(i=0;i<len;i++)
    {
        printf("%02x ",buf[i]);
    }

    printf("\r\nbuf length = %d ,print end \r\n",len);    
}


void DealSerialParse(void)
{
    uint8_t ch = 0;
    while(1)
    {   
        if(drv_Usart1_RecvOne(&ch) != 1)  //读取串口数据
        {
            return;
        }   
        
        switch (gcmd.RxdStatus)
        { /*接收数据状态*/                
            case 0:
                if(FH == ch) /*接收包头*/
                {
                    gcmd.RxdBuf[0] = ch;
                    gcmd.RxCRC = ch;    
                    gcmd.NowLen = 1;
                    gcmd.RxdStatus = 20;
                }
                break;              
            case 20:      /* 接收整个数据包 */
                if (ch == FT) 
                {
                    gcmd.RxdStatus = 21;
                }
                gcmd.RxdBuf[gcmd.NowLen++] = ch;
                gcmd.RxCRC ^= ch;
//                dbh(3,gcmd.RxdBuf,gcmd.NowLen);
                break;
            case 21:
                if(ch == gcmd.RxCRC)
                {    
                    gcmd.RxdBuf[gcmd.NowLen++] = ch;
                    gcmd.RxdTotalLen = gcmd.NowLen;
                    gcmd.RxdFrameStatus = FINISH;                    
                    gcmd.RxdStatus = 0;

                    printf("Deal Serial Parse finish\r\n");
//                    dbh(1,gcmd.RxdBuf,gcmd.NowLen);
                    break;
                }
                else
                {  
                    gcmd.RxdFrameStatus = FINISH;
                    gcmd.RxdBuf[gcmd.NowLen++] = ch;
                    gcmd.RxCRC ^= ch;
                    gcmd.RxdStatus = 20;
//                    dbh(2,gcmd.RxdBuf,gcmd.NowLen);
                }
				
                break;
            default:                
                if (gcmd.RxdFrameStatus == SPACE) 
                {
                    gcmd.RxdFrameStatus = FINISH;
                    gcmd.RxdStatus = 0;
                }
            break;
         }
    }
}


#if 1
/*****************************************************************************
 函 数 名  : DealRxData
 功能描述  : 解析收到的完整数据包，并响应数据包；
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年6月18日
    作    者   : 张舵
    修改内容   : 新生成函数

*****************************************************************************/
void DealRxData(void)
{
    uint16_t ch;
    uint8_t bcc = 0;       
    
    if (FINISH == gcmd.RxdFrameStatus)
    {
//        printf("\r\n -------------------\r\n");
//        dbh(4,gcmd.RxdBuf,gcmd.RxdTotalLen);
        
        if(gcmd.RxdBuf[0] == FH)                                   //解析02数据包
        {
            for (ch = 0; ch < gcmd.RxdTotalLen - 1; ch++)          //计算异或和
            {
                    bcc ^= gcmd.RxdBuf[ch];
            }
            
            if (bcc == gcmd.RxdBuf[gcmd.RxdTotalLen -1])
            {

                switch (gcmd.RxdBuf[2])
                {
                    case GETSENSOR:                        
                        printf("recv cmd = A1 \r\n");
                        //SendData(gcmd.RxdBuf[2]);
                        //init_serial_boot();
                        SystemReset();
                        break;
                    case SETLED:
                        printf("recv cmd = A2 \r\n");
                        SendData(gcmd.RxdBuf[2]);
                        init_serial_boot();
                        break;                        
                    case GETDEVICESAT:
                        printf("recv cmd = A3 \r\n");
                        SendData(gcmd.RxdBuf[2]);
                        init_serial_boot();
                        break;
                    case GETVER:
                        printf("recv cmd = A4 \r\n");
                        SendData(gcmd.RxdBuf[2]);
                        init_serial_boot();
                        break; 
                    case HEARTBEAT:
                        printf("recv cmd = A6 \r\n");
                        SendData(gcmd.RxdBuf[2]);
                        init_serial_boot();
                        break;
                    case UPGRADE:
                        printf("recv cmd = A7 \r\n");
                        SendData(gcmd.RxdBuf[2]);
                        init_serial_boot();
                        break;                         
                    
                }
            }
            else
            {
                init_serial_boot();
            }
        }
        else
        {
            init_serial_boot();
        }
    }

}

#endif







//void DealSerialParse(void)
//{
//    uint8_t ch = 0;

//        if(drv_Usart1_RecvOne(&ch) != 1)  //读取串口数据
//        {
//            return;
//        }   
//        

//	if(FH == ch)
//	{
//	    gcmd.RxdBuf[0] = ch;
//	    gcmd.RxCRC = ch;    
//	    gcmd.NowLen = 1;
//	    drv_Usart1_Read(gcmd.RxdBuf+1,长度);
//            //判定长度
//            //计算整理个数据CRC
//            //若是出问题，再重新来
//	}
//}

//那这个流程感觉有点笨拙

static uint8_t calcCRC(uint8_t *buf,uint8_t len)
{
    uint8_t i = 0;
    uint8_t bcc = 0;
    bcc = buf[0];

    printf("len = %d\r\n",len);

    for(i=1;i<len;i++)
    {
        bcc ^= buf[i];
    }

    return bcc;
}



void SendData(uint8_t frame_type)
{
    uint8_t i = 0;
    uint8_t TxdBuf[MAX_TXD_BUF_LEN]={0};

    switch (frame_type)
    {
        case GETSENSOR:
            i = 3;
            memcpy(TxdBuf,(uint8_t *)gcmd.RxdBuf,i);
            TxdBuf[i++] = 0x02;
            TxdBuf[i++] = CMDSUCC;
            TxdBuf[i++] = 0x01;
            TxdBuf[i++] = FT;
            TxdBuf[i++] = calcCRC(TxdBuf,i);
            break;
        case SETLED:
            i = 3;
            memcpy(TxdBuf,(uint8_t *)gcmd.RxdBuf,i);
            TxdBuf[i++] = 0x01;
            TxdBuf[i++] = CMDSUCC;
            TxdBuf[i++] = FT;
            TxdBuf[i++] = calcCRC(TxdBuf,i);
            break;                        
        case GETDEVICESAT:
            i = 3;
            memcpy(TxdBuf,(uint8_t *)gcmd.RxdBuf,i);
            TxdBuf[i++] = 0x02;
            TxdBuf[i++] = CMDSUCC;
            TxdBuf[i++] = 0x01;
            TxdBuf[i++] = FT;
            TxdBuf[i++] = calcCRC(TxdBuf,i);
            break;
        case GETVER:
            i = 3;
            memcpy(TxdBuf,(uint8_t *)gcmd.RxdBuf,i);
            TxdBuf[i++] = 0x02;
            TxdBuf[i++] = CMDSUCC;
            TxdBuf[i++] = 0x01;
            TxdBuf[i++] = FT;
            TxdBuf[i++] = calcCRC(TxdBuf,i);
            break; 
        case HEARTBEAT:
            i = 3;
            memcpy(TxdBuf,(uint8_t *)gcmd.RxdBuf,i);
            TxdBuf[i++] = 0x02;
            TxdBuf[i++] = CMDSUCC;
            TxdBuf[i++] = 0x01;
            TxdBuf[i++] = FT;
            TxdBuf[i++] = calcCRC(TxdBuf,i);
            break;
        case UPGRADE:
            i = 3;
            memcpy(TxdBuf,(uint8_t *)gcmd.RxdBuf,i);
            TxdBuf[i++] = 0x02;
            TxdBuf[i++] = CMDSUCC;
            TxdBuf[i++] = 0x01;
            TxdBuf[i++] = FT;
            TxdBuf[i++] = calcCRC(TxdBuf,i);
            break;                         

        default:
            init_serial_boot(); 
            return;
    }

    dbh(100,(char *)TxdBuf,i);
    drv_Usart1_SendData(TxdBuf,i);
}

