/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Gprs.c  2013-06-25
此文件用管理与GPS 服务器通迅协议的处理。
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"


u8 YS_Gprs_DeadPos_Buf[YS_GPRS_DEADPOSBUF_LEN];

t_avltra_Parase			t_AvlSckParase;
t_avltra_Parase        		t_AvlSckConver;
t_Gprs_Server_Manage		t_ServerManage;
t_Gprs_DeadPos_Pack     	t_DeadPosPack;
t_Gprs_DeadPos_Manage   	t_DeadPosMange;

void YS_GprsDeadPosReadFS(void);

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPosPackCompress
功能说明：将时间压缩至5个字节
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosTimeCompress(u8 *buf)
{
    u8 value;

    buf[0]=buf[0];
    buf[1]=buf[1];
    buf[2]<<=3;
    value=(buf[3]>>2)&0x07;
    buf[2]|=value;

    buf[3]<<=6;
    value=buf[4]&0x3f;
    buf[3]|=value;

    buf[4]=buf[5];
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPosPackCompress
功能说明：压缩位置报文--用于盲点补传
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosPackCompress(u8 *pbuf, u16 *plen)
{
    u8 i;
//    u8 DbgBuf[50];
    u16 pos;
    t_Gps_Data_Info t_GetGps;
    t_Sys_Run_Status t_GetStatus;

    YS_GpsGetPosData(&t_GetGps);
    YS_RunGetSystemStatus(&t_GetStatus);
    pos=0;
    for(i=0; i<4; i++) //纬度
    {
        pbuf[pos]=t_GetGps.Latitude[i];
        pos++;
    }
    for(i=0; i<4; i++) //经度
    {
        pbuf[pos]=t_GetGps.Longitude[i];
        pos++;
    }
    pbuf[pos]=(u8)t_GetGps.Speed;//速度
    pos++;
    pbuf[pos]=(u8)t_GetGps.Course/2;//角度
    pos++;

    pbuf[pos]=0;//报警状态
    if(TRUE==t_GetStatus.WarnStatus[AVL_WARM_OFFVOL])//断电报警
    {
        pbuf[pos]|=0x80;
    }
    if(TRUE==t_GetStatus.WarnStatus[AVL_WARM_LOWVOL])//低电压报警
    {
        pbuf[pos]|=0x40;
    }
    if(TRUE==t_GetStatus.WarnStatus[AVL_WARM_SPEED])//超速报警
    {
        pbuf[pos]|=0x20;
    }
    if(TRUE==t_GetStatus.WarnStatus[AVL_WARM_MOVE])//位移报警
    {
        pbuf[pos]|=0x10;
    }
    if(TRUE==t_GetStatus.WarnStatus[AVL_WARM_INAREA])//区域内报警
    {
        pbuf[pos]|=0x08;
    }
    if(TRUE==t_GetStatus.WarnStatus[AVL_WARM_OUTAREA])//区域外报警
    {
        pbuf[pos]|=0x04;
    }
    pos++;
    //时间
    if(YS_RunGsmColckisOK()==TRUE) //如果系统时间已同步
    {
        sjfun_Get_System_DateTime(t_GetGps.DataTime);  //取系统时间
    }
    else
    {
        YS_CodeCretaeBJDT(t_GetGps.DataTime);	//取GPS 时间，并转换为北京格式
    }

    YS_GprsDeadPosTimeCompress(t_GetGps.DataTime);
    for(i=0; i<5; i++)
    {
        pbuf[pos]=t_GetGps.DataTime[i];
        pos++;
    }
    *plen=pos;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPosPackCompress
功能说明：将时间压缩至5个字节
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosTimePickUp(u8 *buf)
{
    u8 value1,value2,value3;

    buf[0]=buf[0];
    buf[1]=buf[1];
    value1=buf[2]&0x07;
    buf[2]>>=3;

    value2=buf[3]&0xC0;
    value3=buf[3]&0x3F;
    buf[3]=(value1<<2)|(value2>>6);

    buf[5]=buf[4];
    buf[4]=value3;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPosPackCompress
功能说明：压缩位置报文--用于盲点补传
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosPackPickUp(u8 *pbuf, u16 plen)
{
    u8 i,pos;

    pos=0;
    for(i=0; i<4; i++) //纬度
    {
        t_DeadPosPack.DeadPosLati[i]=pbuf[pos];
        pos++;
    }
    for(i=0; i<4; i++) //经度
    {
        t_DeadPosPack.DeadPosLongi[i]=pbuf[pos];
        pos++;
    }
    t_DeadPosPack.DeadPosSpeed=pbuf[pos];//速度
    pos++;
    t_DeadPosPack.DeadPosCourse=pbuf[pos]*2;//角度
    pos++;

    if(pbuf[pos]&0x80==0x80)
    {
        t_DeadPosPack.DeadPosWarmStatus[AVL_WARM_OFFVOL]=TRUE;
    }
    if(pbuf[pos]&0x40==0x40)
    {
        t_DeadPosPack.DeadPosWarmStatus[AVL_WARM_LOWVOL]=TRUE;
    }
    if(pbuf[pos]&0x20==0x20)
    {
        t_DeadPosPack.DeadPosWarmStatus[AVL_WARM_SPEED]=TRUE;
    }
    if(pbuf[pos]&0x10==0x10)
    {
        t_DeadPosPack.DeadPosWarmStatus[AVL_WARM_MOVE]=TRUE;
    }
    if(pbuf[pos]&0x0F==0x0F)
    {
        t_DeadPosPack.DeadPosWarmStatus[AVL_WARM_INAREA]=TRUE;
    }
    if(pbuf[pos]&0x04==0x04)
    {
        t_DeadPosPack.DeadPosWarmStatus[AVL_WARM_OUTAREA]=TRUE;
    }
    pos++;

    YS_GprsDeadPosTimePickUp(&pbuf[pos]);
    for(i=0; i<6; i++)
    {
        t_DeadPosPack.DeadPosTime[i]=pbuf[pos];
        pos++;
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDeadPosIni
功能说明：盲点补传参数初始化
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosIni(void)
{
    char DbgBuf[60];
    u16 NumValue;

    t_DeadPosMange.ReadTimeInterval=0;
    t_DeadPosMange.WriteTimeInterval=YS_GPRS_DEADPOS_TIME;
    t_DeadPosMange.DeadPosSize=0;

    YS_GprsDeadPosReadFS();
    t_DeadPosMange.DeadPosNum=YS_Gprs_DeadPos_Buf[0]*256+YS_Gprs_DeadPos_Buf[1];
    t_DeadPosMange.DeadPosPread=YS_Gprs_DeadPos_Buf[2]*256+YS_Gprs_DeadPos_Buf[3];
    t_DeadPosMange.DeadPosPwrite=YS_Gprs_DeadPos_Buf[4]*256+YS_Gprs_DeadPos_Buf[5];

    if(t_DeadPosMange.DeadPosNum>0)
    {
        if(t_DeadPosMange.DeadPosPwrite>t_DeadPosMange.DeadPosPread)
        {
            NumValue=t_DeadPosMange.DeadPosPwrite-t_DeadPosMange.DeadPosPread;
        }
        else
        {
            NumValue=(YS_GPRS_DEADPOS_NUM-t_DeadPosMange.DeadPosPread)+t_DeadPosMange.DeadPosPwrite;
        }

        if(NumValue!=t_DeadPosMange.DeadPosNum)
        {
            t_DeadPosMange.DeadPosNum=0;
            t_DeadPosMange.DeadPosPread=0;
            t_DeadPosMange.DeadPosPwrite=0;
            memset(YS_Gprs_DeadPos_Buf,0,YS_GPRS_DEADPOSBUF_LEN);
            YS_GprsDebugString("Dead Pos Parm check fail", 0);
        }
        else
        {
            YS_GprsDebugString("Dead Pos Parm check succ", 0);
        }
    }

    sprintf(DbgBuf, "the PosNum is %d,read pos is:%d, write pos is:%d",t_DeadPosMange.DeadPosNum,t_DeadPosMange.DeadPosPread,t_DeadPosMange.DeadPosPwrite);
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, (u8 *)DbgBuf, 60);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDeadPosRead
功能说明：从缓存中读取数据
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosRead(u16 pos, u8 *dbuf)
{
    u8 i;
    for(i=0; i<YS_GPRS_DEADPOS_LEN; i++)
    {
        dbuf[i] = YS_Gprs_DeadPos_Buf[pos+i];
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDeadPosWrite
功能说明：将数据写入缓存
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosWrite(u16 pos, u8 *dbuf)
{
    u8 i;
    for(i=0; i<YS_GPRS_DEADPOS_LEN; i++)
    {
        YS_Gprs_DeadPos_Buf[pos+i] = dbuf[i];
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDeadPosReadFS
功能说明：开机将盲区补传文件从文件系统中读出
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosReadFS(void)
{

}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDeadPosWriteFS
功能说明：将数据写入文件系统
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosWriteFS(void)
{

}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDeadPosResetRefresh
功能说明：复位刷新盲区数据至文件系统
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosResetRefresh(void)
{
    if(t_DeadPosMange.DeadPosNum != 0)//如果有保存盲区数据 刷新缓存
    {
        YS_GprsDeadPosWriteFS();
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDeadPosFormat
功能说明：格式化盲区
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosFormat(void)
{
    if(t_DeadPosMange.DeadPosNum != 0)//如果有保存盲区数据 刷新缓存
    {
        t_DeadPosMange.DeadPosNum = 0;
        t_DeadPosMange.DeadPosPread=0;
        t_DeadPosMange.DeadPosPwrite=0;
        YS_GprsDeadPosWriteFS();
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDeadPosGetInfo
功能说明：获取盲点补传包信息
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosGetPackInfo(t_Gprs_DeadPos_Pack *t_DeadPosPackInfo)
{
    u16 i,len;
    u8 *pSour, *pAim;

    len=sizeof(t_Gprs_DeadPos_Pack);
    pSour=(u8 *)(&t_DeadPosPack);
    pAim=(u8 *)t_DeadPosPackInfo;
    for(i=0; i<len; i++)
    {
        pAim[i]=pSour[i];
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_BlindGetRunInfo
功能说明：获取盲区补传模块的运行信息
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsDeadPosRunInfo(u8 *dbuf)
{
    dbuf[0]=t_DeadPosMange.DeadPosNum/256;
    dbuf[1]=t_DeadPosMange.DeadPosNum%256;
    dbuf[2]=t_DeadPosMange.DeadPosPread/256;
    dbuf[3]=t_DeadPosMange.DeadPosPread%256;
    dbuf[4]=t_DeadPosMange.DeadPosPwrite/256;
    dbuf[5]=t_DeadPosMange.DeadPosPwrite%256;
    dbuf[6]=0;
    return(7);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDeadPosGetInfo
功能说明：获取盲点补传信息
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsDeadPosGetManageInfo(t_Gprs_DeadPos_Manage *t_DeadPosInfo)
{
    u16 i,len;
    u8 *pSour, *pAim;

    len=sizeof(t_Gprs_DeadPos_Manage);
    pSour=(u8 *)(&t_DeadPosMange);
    pAim=(u8 *)t_DeadPosInfo;
    for(i=0; i<len; i++)
    {
        pAim[i]=pSour[i];
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPosInNormal
功能说明：发送数据到服务器
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsDeadPosInNormal(void)
{
    u8 Pbuf[20];
    u16 Plen;
    char DbgBuf[100];
    bool rlt=FALSE;

    t_Gps_Data_Info t_GetGps;
    YS_GpsGetPosData(&t_GetGps);
    if (FALSE == t_GetGps.Effective)
    {
        return FALSE;
    }

    if (t_DeadPosMange.DeadPosNum>0)
    {
        YS_GprsDeadPosPackCompress(Pbuf,&Plen);
        YS_GprsDeadPosWrite((t_DeadPosMange.DeadPosPwrite+1)*(YS_GPRS_DEADPOS_LEN),Pbuf);
        t_DeadPosMange.DeadPosNum++;
        if(t_DeadPosMange.DeadPosNum>YS_GPRS_DEADPOS_NUM)
        {
            t_DeadPosMange.DeadPosNum=YS_GPRS_DEADPOS_NUM;

            t_DeadPosMange.DeadPosPwrite++;
            if(t_DeadPosMange.DeadPosPwrite>=YS_GPRS_DEADPOS_NUM)
            {
                t_DeadPosMange.DeadPosPwrite=0;
            }

            t_DeadPosMange.DeadPosPread++;
            if(t_DeadPosMange.DeadPosPread>=YS_GPRS_DEADPOS_NUM)
            {
                t_DeadPosMange.DeadPosPread=0;
            }
        }
        else
        {
            t_DeadPosMange.DeadPosPwrite++;
            if(t_DeadPosMange.DeadPosPwrite>=YS_GPRS_DEADPOS_NUM)
            {
                t_DeadPosMange.DeadPosPwrite=0;
            }
        }
        rlt =TRUE;
        sprintf(DbgBuf, "N-the PosNum is %d,read pos is:%d, write pos is:%d",t_DeadPosMange.DeadPosNum,t_DeadPosMange.DeadPosPread,t_DeadPosMange.DeadPosPwrite);
        YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, (u8 *)DbgBuf, 60);
    }
    else
    {
        rlt =FALSE;
    }
    return rlt;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsSendDataToServer
功能说明：发送数据到服务器
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsSendDataToServer(u8 *dbuf, u16 dlen)
{
    if(0==dlen)
    {
        return FALSE;
    }
    else if(0xffff==dlen)
    {
        return TRUE;
    }

    YS_RunSocketSendData(dbuf,dlen);
    return TRUE;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsServerSendInterface
功能说明：服务器数据包上报应用接口
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsServerSendInterface(u8 CmdID, u8 *dbuf, u16 dlen)
{
    bool 	rlt=FALSE;
    u8		PackBuf[300];
    u16 		PackLen;

    PackLen=0;
#if 0
    switch(CmdID)
    {
        case SERV_UP_CMD_REG:	//注册包
            PackLen=YS_GprsPackRsqServerPackage(PackBuf);
            YS_GprsDebugString("Ys gprs reg pack len",PackLen);
            break;

        case SERV_UP_CMD_LOG:	//登包
            PackLen=YS_GprsPackLogPackage_obd(PackBuf);
            YS_GprsDebugString("Ys gprs log pack len",PackLen);
            break;

        case SERV_UP_CMD_HEART: //心跳包
            PackLen=YS_GprsPackHeartPackage_obd(PackBuf);
            YS_GprsDebugString("Ys gprs heart pack len",PackLen);
            break;

        case SERV_UP_CMD_POS:  //定位数据包
            PackLen=YS_GprsPackGpsPackage(PackBuf);
            YS_GprsDebugString("Ys gprs gps pack len",PackLen);
            break;

        case SERV_UP_CMD_CAN:  //汽车数据包
            PackLen=YS_GprsPackCarObdPackage(PackBuf);
            YS_GprsDebugString("Ys gprs obd pack len",PackLen);
            break;

        case SERV_UP_CMD_ACK:
//            PackLen=YS_GprsSendAckPackage(PackBuf,dbuf,dbuf[dlen-1]);
            break;
        default:
            break;
    }
#else
    switch(CmdID)
    {
        case SERV_UP_CMD_REG:	//注册包
            PackLen=YS_GprsPackRegPackage(PackBuf);
            YS_GprsDebugString("Ys reg pack len",PackLen);
            break;
        case SERV_UP_CMD_LOG:	//登包
            PackLen=YS_GprsPackLogPackage(PackBuf);
            YS_GprsDebugString("Ys log pack len",PackLen);
            break;

        case SERV_UP_CMD_HEART: //心跳包
            PackLen=YS_GprsPackHeartPackage(PackBuf);
            break;

        case SERV_UP_CMD_READPRMACK://参数查询
            PackLen=YS_GprsPackPrmAckPackage(PackBuf,dbuf);
            break;

        case SERV_UP_CMD_POS:  //定位数据包
            PackLen=YS_GprsPackPosPackage(PackBuf);
            break;

        case SERV_UP_CMD_POS_QUEUE://连上平台传盲点的时候需要立刻上传位置报文--立刻上传
            if (TRUE==YS_GprsDeadPosInNormal())//连上平台传盲点的时候插入盲点队列上传位置报文--延后上传
            {
                PackLen=0xffff;
            }
            else
            {
                PackLen=YS_GprsPackPosPackage(PackBuf);
            }
            break;

        case SERV_UP_CMD_ONEPOS: //点名应答包
            PackLen=YS_GprsSendPointPackage(PackBuf,dbuf);
            break;
        case SERV_UP_CMD_DEADPOS:
            PackLen=YS_GprsPackDeadPosPackage(PackBuf);
            break;
        case SERV_UP_CMD_POSRSQ://中文位置请求
            PackLen=YS_GprsCreatePosRsqInfo(PackBuf);
            break;

        case SERV_UP_CMD_CARCTL:
            PackLen = YS_GprsPackCarCtlAckPackage(PackBuf,dbuf);
            break;

        case SERV_UP_CMD_ACK:
            PackLen=YS_GprsSendAckPackage(PackBuf,dbuf,dbuf[dlen-1]);
            break;
        default:
            break;
    }
#endif
    rlt=YS_GprsSendDataToServer(PackBuf,PackLen);
    return rlt;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsGpsServerPtlParase
功能说明：从缓冲区提取一个命令帧，并进行解析转换操作
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsGpsServerDataInput(u8 *dbuf, u16 dlen)
{
    u16 pos=0;
    u8  DbgBuf[600];
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_GPRSREC,dbuf,dlen);

    YS_CodeHextoString(dbuf, dlen, DbgBuf);
    ycsj_debug((char *)DbgBuf);
    while(dlen>0)
    {
#if 0
        if(YS_GprsDealServRecData(dbuf,dlen)==TRUE)
        {
            YS_GprsSckPtlUpConver();
            dlen-=t_AvlSckParase.v_GetLen;
            pos+=t_AvlSckParase.v_GetLen;
            dbuf=dbuf+pos;
        }
        else
        {
            return;
        }
#else
        if(YS_GprsDealJTBServRecData(dbuf,dlen)==TRUE)
        {
            YS_GprsJBTSckPtlUpConver();
            dlen-=t_AvlSckParase.v_GetLen;
            pos+=t_AvlSckParase.v_GetLen;
            dbuf=dbuf+pos;
        }
        else
        {
            return;
        }
#endif
    }
}


