/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Gprs_obd.c  2013-06-25
此文件用管理与GPS 服务器通迅协议的处理。基于OBD协议
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"
#define BOOTVER         "MT61D_V1.0"
#define HARDVER         "A10"
#define EQUIPTYPE       "0"
#define CHANNELCODE     "XX"

extern t_avltra_Parase		t_AvlSckParase;
extern t_avltra_Parase         t_AvlSckConver;

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsAddPosData
功能说明：向缓冲区中添加经纬度数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsAddPosData(u8 *PosValue, u8 *PackBuf)
{
    u16 i,len;
    char StrDat[50];
    double f;

    f=PosValue[1]*10000+PosValue[2]*100+PosValue[3];
    f/=10000;
    f/=60;
    f+=PosValue[0];
    sprintf(StrDat,"%.06f",f);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        PackBuf[i]=StrDat[i];
    }
    return(len);
}

#if 0
/*-----------------------------------------------------------------------------------------
函数名：YS_GprsAddPosData
功能说明：向缓冲区中添加经纬度数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsAddUnixTime(u8 *PackBuf)
{
    u16 i,len;
    time_t t;
    int j=0;
    char StrDat[50];

//    j = time(&t);
    sprintf(StrDat,"1448532363");
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        PackBuf[i]=StrDat[i];
    }
    return(len);
}
#endif

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsAddUnixTime
功能说明：Unix时间戳
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsAddUnixTime(u8 *PackBuf)
{
    u16 i,len;
    u32 t;
    int j=0;
    char StrDat[50];

    t = sjfun_get_unix_time();
    t = t - 28800;

    sprintf(StrDat,"%d",t);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        PackBuf[i]=StrDat[i];
    }
    return(len);
}
/*-----------------------------------------------------------------------------------------
函数名：YS_GprsAddPosData
功能说明：向缓冲区中添加速度数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsAddSpeedInfo(u16 speed, u8 *PackBuf)
{
    u16 i,len;
    char StrDat[50];
    sprintf(StrDat,"%d.0",speed);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        PackBuf[i]=StrDat[i];
    }
    return(len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsAddCourseInfo
功能说明：向缓冲区中添加航向数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsAddCourseInfo(u16 course, u8 *PackBuf)
{
    u16 i,len;
    char StrDat[50];
    sprintf(StrDat,"%d.0",course);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        PackBuf[i]=StrDat[i];
    }
    return(len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDealPtlSend
功能说明：封装一个数据包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsDealPtlSend(u8 CmdID, u8 *dbuf, u16 dlen, u8 *PackBuf)
{
    u16 i,pos;

    pos=0;
    PackBuf[pos]=0x7E; //添加包头
    pos++;
    PackBuf[pos]=0x7D; //添加包头
    pos++;
    PackBuf[pos]=0x01; //协议版本号
    pos++;
    PackBuf[pos]=CmdID; //指令码
    pos++;

    PackBuf[pos]=dlen/256; //消息体长度
    pos++;
    PackBuf[pos]=dlen%256; //
    pos++;

    if(dlen>0)  //添加消息体部份
    {
        for(i=0; i<dlen; i++)
        {
            PackBuf[pos]=dbuf[i];
            pos++;
        }
    }
    return(pos);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackRsqServerPackage
功能说明：封装一个请求服务器地址数据包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackRsqServerPackage(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];

    pos = 0;
    YS_RunAddIDInfo(fbuf);//设备ID
    len =strlen((char*)fbuf);
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = fbuf[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(EBIKE_SOFT_VER);//软件版本号
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = EBIKE_SOFT_VER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(BOOTVER);//设备固件版本号
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = BOOTVER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(HARDVER);//设备硬件版本号
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = HARDVER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(EQUIPTYPE);//设备代码类型
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = EQUIPTYPE[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(CHANNELCODE);//渠道识别码
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = CHANNELCODE[i];
        pos++;
    }

    PackLen=YS_GprsDealPtlSend(0xf0,tempbuf,pos,PackBuf);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackLogPackage
功能说明：封装一个登入平台数据包(验证设备合法性)
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackLogPackage_obd(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];

    pos = 0;
    YS_RunAddIDInfo(fbuf);//设备ID
    len =strlen((char*)fbuf);
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = fbuf[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(EBIKE_SOFT_VER);//软件版本号
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = EBIKE_SOFT_VER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(BOOTVER);//设备固件版本号
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = BOOTVER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(HARDVER);//设备硬件版本号
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = HARDVER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(EQUIPTYPE);//设备代码类型
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = EQUIPTYPE[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(CHANNELCODE);//渠道识别码
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = CHANNELCODE[i];
        pos++;
    }

    PackLen=YS_GprsDealPtlSend(0x01,tempbuf,pos,PackBuf);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackHeartPackage
功能说明：封装一个登入平台数据包(验证设备合法性)
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackHeartPackage_obd(u8 *PackBuf)
{
    u16 PackLen;

    PackLen=YS_GprsDealPtlSend(0x7f,NULL,0,PackBuf);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackGPSPackage
功能说明：封装一个实时位置数据包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackGpsPackage(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];
    t_Gps_Data_Info t_GpsData;

    YS_GpsGetPosData(&t_GpsData);
    pos = 0;
    len=YS_GprsAddPosData(t_GpsData.Longitude,&tempbuf[pos]);//经度
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    len=YS_GprsAddPosData(t_GpsData.Latitude,&tempbuf[pos]);//纬度
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    tempbuf[pos] = '|';
    pos++;

    len=YS_GprsAddUnixTime(&tempbuf[pos]);//UNIX时间
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    len=YS_GprsAddSpeedInfo(t_GpsData.Speed,&tempbuf[pos]);//速度
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    len=YS_GprsAddCourseInfo(t_GpsData.Course,&tempbuf[pos]);//方向
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    tempbuf[pos] = '|';//坡度
    pos++;

    tempbuf[pos] = '|';//海拔
    pos++;

    tempbuf[pos] = '|';//定位精度
    pos++;

    tempbuf[pos] = '2';
    pos++;
    tempbuf[pos] = '|';//定位来源
    pos++;

    tempbuf[pos] = '|';//信号状态
    pos++;

    tempbuf[pos] = '|';//卫星数量
    pos++;

    tempbuf[pos] = '|';//道路类型
    pos++;
    PackLen=YS_GprsDealPtlSend(0x20,tempbuf,pos,PackBuf);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackCarObdPackage
功能说明：封装一个实时车况数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackCarObdPackage(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];
    t_Obd_Main_Info t_GetObdStatus;

    YS_ObdGetCarData(&t_GetObdStatus);
    pos = 0;
    len=YS_GprsAddUnixTime(&tempbuf[pos]);//UNIX时间
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    len = YS_ObdCarPackage(&tempbuf[pos]);
    pos += len;

    PackLen=YS_GprsDealPtlSend(0x30,tempbuf,pos,PackBuf);

    if (len<100)
    {
        return 0;
    }
    else
    {
        return(PackLen);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackWarnPackage
功能说明：封装一个车辆告警数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackWarnPackage(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];

    pos = 0;
    len=YS_GprsAddUnixTime(&tempbuf[pos]);//UNIX时间
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    PackLen=YS_GprsDealPtlSend(0x40,tempbuf,pos,PackBuf);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackObdDstPackage
功能说明：封装一个车辆OBD行程数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackObdDstPackage(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];

    pos = 0;

    PackLen=YS_GprsDealPtlSend(0x60,tempbuf,pos,PackBuf);
    return(PackLen);
}


/*-----------------------------------------------------------------------------------------
函数名：YS_GprsParaseHeadTail
功能说明：消息体包头处理
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsParaseHeadTail_obd(u8 *dbuf, u16 dlen)
{
    bool result;
    u16 i;

    t_AvlSckParase.v_CmdID=dbuf[3];
    t_AvlSckParase.v_dlen=dbuf[4]*256+dbuf[5];
    if(t_AvlSckParase.v_dlen>0)
    {
        for(i=0; i<t_AvlSckParase.v_dlen; i++)
        {
            t_AvlSckParase.a_dbuf[i]=dbuf[6+i];
        }
    }
    result=TRUE;
    return(result);
}


/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDealJTBServRecData
功能说明：从数据缓存区中提取数据包
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsDealServRecData(u8 *buf, u16 len)
{
    bool result;
    u16 i,pos,count;

    if((buf[0]==0x7E)&&(buf[len-2]==0x0D)&&(buf[len-1]==0x0A))  //如果包头包尾正确
    {
        pos=0;
        count=0;
        for(i=0; i<len; i++)
        {
            pos++;
            if((buf[i]==0x0D)&&((buf[i+1]==0x0A)))
            {
                pos++;
                break;
            }
        }

        if(YS_GprsParaseHeadTail_obd(buf,pos)==TRUE)
        {
            t_AvlSckParase.v_GetLen=pos;
            result=TRUE;
        }
        else
        {
            result=FALSE;
        }
    }
    else
    {
        result=FALSE;
    }
    return(result);
}



/*-----------------------------------------------------------------------------------------
函数名：YS_GprsJBTSckPtlUpConver
功能说明：平台下发数据包解析指令
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsSckPtlUpConver(void)
{
    bool result,ack;
    u16 i,AckCmd;
    u8 fbuf[5];

    result=TRUE;
    memcpy(t_AvlSckConver.a_AddInfo,t_AvlSckParase.a_AddInfo,AVL_ADD_INFO_LEN);

    switch(t_AvlSckParase.v_CmdID)
    {
        case 0x01: //登入平台应答
            if (t_AvlSckParase.a_dbuf[1] == 0x31)
            {
                YS_RunLoginServerAck();
                ycsj_debug("log ack!\r\n");
                YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)"log ack!\r\n",10);
            }
            break;

        case 0x7f: //心跳应答
            YS_RunIdleHeartAck();
            ycsj_debug("heart ack!\r\n");
            YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)"log ack!\r\n",10);
            break;

        case 0x40: //报警应答

            break;

        default:
            result=FALSE;
            break;
    }

    return(result);
}

/*************************808 OBD分割线 begin*****************************/

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackObdInfo
功能说明：封装OBD信息
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackObdInfo(u8 *ObdBuf)
{
    u16 pos;

    pos = 0;

    return pos;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackCarObdPackage
功能说明：封装一个实时车况数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackCarObdPackage_808(u8 *PackBuf)
{
    u8 ObdBuf[300];
    u16 ObdLen,PackLen;

    ObdLen=YS_GprsPackObdInfo(ObdBuf);
    PackLen=YS_GprsDealJTBPtlSend(0x8f03,ObdBuf,ObdLen,PackBuf,1,1);
    return(PackLen);
}
/*************************808 OBD分割线 end  *****************************/
