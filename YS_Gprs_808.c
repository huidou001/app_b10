/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Gprs_808.c  2013-06-25
此文件用管理与GPS 服务器通迅协议的处理。基于808协议
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"
extern t_avltra_Parase		t_AvlSckParase;
extern t_avltra_Parase         t_AvlSckConver;

void YS_GprsConverSimCode(u8 *OutBuf, char *SimCode)
{
    char StrDat[20];
    char StrSim[20];
    u8 i,dat,pos;
    u8 len;
    pos=0;

    len=strlen(SimCode);
    for(i=0; i<12-len; i++)
    {
        StrSim[pos]='0';
        pos++;
    }
    for(i=0; i<len; i++)
    {
        StrSim[pos]=SimCode[i];
        pos++;
    }
    for(i=0; i<6; i++)
    {
        StrDat[0]=StrSim[i*2];
        StrDat[1]=StrSim[i*2+1];
        StrDat[2]=0;
        dat=atoi(StrDat);
        OutBuf[i]=YS_CodeOneByteToBCD(dat);
    }
}

u32  YS_GprsPosToMillionDU(u8 *PosBuf)
{
    double  f;
    char StrDat[20];
    u32 result;

    sprintf(StrDat,"%d.%02d%02d",PosBuf[1],PosBuf[2],PosBuf[3]);
    f=atof(StrDat);
    f=f*100; //转换成度小数
    f=f/6000;
    f=f+PosBuf[0];
    f*=1000000;
    result=(u32)f;
    return(result);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsAddOneByte
功能说明：转义字符处理  0x7e<---->0x7d 0x02    0x7d<---->0x7d 0x01
修改记录：
-------------------------------------------------------------------------------------------*/
u8 YS_GprsAddOneByte(u8 dat, u8 *buf, u8 *Verify)
{

    u8 tVer;

    tVer=*Verify;
    *Verify=tVer^dat;
    if(dat==0x7E)
    {
        buf[0]=0x7D;
        buf[1]=0x02;
        return(2);
    }
    else if(dat==0x7D)
    {
        buf[0]=0x7D;
        buf[1]=0x01;
        return(2);
    }
    else
    {
        buf[0]=dat;
        return(1);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDealJTBPtlSend
功能说明：交通部808协议封装一个数据包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsDealJTBPtlSend(u16 CmdID, u8 *dbuf, u16 dlen, u8 *PackBuf, u16 TotalNum, u16 CurNO)
{
    u8 Verify,SimCode[6],tdat;
    u16 i,pos,td16;
    static u16 SendFlow=0;
    u8 fbuf[20];

    pos=0;
    PackBuf[pos]=0x7E; //添加包头
    pos++;
    Verify=0x00;
    pos+=YS_GprsAddOneByte(CmdID/256,&PackBuf[pos],&Verify);
    pos+=YS_GprsAddOneByte(CmdID%256,&PackBuf[pos],&Verify);

    td16=dlen;
    if(TotalNum>1) //如果采用分包方式
    {
        td16|=0x2000;
    }
    pos+=YS_GprsAddOneByte(td16/256,&PackBuf[pos],&Verify);
    pos+=YS_GprsAddOneByte(td16%256,&PackBuf[pos],&Verify);

//    YS_PrmReadOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf);
    YS_RunAddIDInfo(fbuf);
    YS_GprsConverSimCode(SimCode,(char *)fbuf);
    for(i=0; i<6; i++)  //添加SIM 卡号
    {
        pos+=YS_GprsAddOneByte(SimCode[i],&PackBuf[pos],&Verify);
    }

    pos+=YS_GprsAddOneByte(SendFlow/256,&PackBuf[pos],&Verify); //添加流水号
    pos+=YS_GprsAddOneByte(SendFlow%256,&PackBuf[pos],&Verify);
    SendFlow++;

    if(TotalNum>1) //如果采用分包传输
    {
        pos+=YS_GprsAddOneByte(TotalNum/256,&PackBuf[pos],&Verify);  //总包数
        pos+=YS_GprsAddOneByte(TotalNum%256,&PackBuf[pos],&Verify);
        pos+=YS_GprsAddOneByte(CurNO/256,&PackBuf[pos],&Verify);  //当前包号
        pos+=YS_GprsAddOneByte(CurNO%256,&PackBuf[pos],&Verify);
    }

    if(dlen>0)  //添加消息体部份
    {
        for(i=0; i<dlen; i++)
        {
            pos+=YS_GprsAddOneByte(dbuf[i],&PackBuf[pos],&Verify);
        }
    }

    /*
    	PackBuf[pos]=Verify;  //添加效验
    	pos++;*/
//	tdat

    pos+=YS_GprsAddOneByte(Verify,&PackBuf[pos],&tdat);   //处理校验数据的转义处理。

    PackBuf[pos]=0x7E;   //添加包尾信息
    pos++;

    return(pos);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsSendAckPackage
功能说明：终端通用应答包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsSendAckPackage(u8 *PackBuf, u8 *AddInfo, u8 rlt)
{
    u8 GpsBuf[300];
    u16 GpsLen,PackLen;

    GpsBuf[0]=AddInfo[10];//应答流水号
    GpsBuf[1]=AddInfo[11];
    GpsBuf[2]=AddInfo[0];//应答ID
    GpsBuf[3]=AddInfo[1];
    GpsBuf[4]=rlt;//结果
    GpsLen=5;
//	GpsLen=YS_GprsPackGpsInfo(&GpsBuf[2]);
//	GpsLen+=2;
    PackLen=YS_GprsDealJTBPtlSend(0x0001,GpsBuf,GpsLen,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackRegPackage
功能说明：封装一个注册数据包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackRegPackage(u8 *PackBuf)
{
    u8 tempbuf[200],fbuf[20],len;
    u16 i,pos,PackLen;

    pos=0;
    YS_PrmReadOneItem(FLH_JTB_PROVINCE_NAME,FLH_JTB_PROVINCE_NAME_LEN,fbuf);  //省域ID
    tempbuf[pos]=fbuf[0];
    pos++;
    tempbuf[pos]=fbuf[1];
    pos++;

    YS_PrmReadOneItem(FLH_JTB_CITY_NAME,FLH_JTB_CITY_NAME_LEN,fbuf);  //市域ID
    tempbuf[pos]=fbuf[0];
    pos++;
    tempbuf[pos]=fbuf[1];
    pos++;

    YS_PrmReadOneItem(FLH_JTB_PRODUCER_NAME,FLH_JTB_PRODUCER_NAME_LEN,fbuf);  //制造商ID
    for(i=0; i<5; i++)
    {
        tempbuf[pos]=fbuf[i];
        pos++;
    }

    YS_PrmReadOneItem(FLH_JTB_TERMINAL_TYPE,FLH_JTB_TERMINAL_TYPE_LEN,fbuf);  //终端型号
    for(i=0; i<8; i++)
    {
        tempbuf[pos]=fbuf[i];
        pos++;
    }
    for(i=0; i<12; i++)
    {
        tempbuf[pos]=0;
        pos++;
    }

//    YS_PrmReadOneItem(FLH_JTB_TERMINAM_ID,FLH_JTB_TERMINAM_ID_LEN,fbuf);  //终端ID
    for(i=0; i<7; i++)
    {
        tempbuf[pos]=0x31;
        pos++;
    }

//    YS_PrmReadOneItem(FLH_JTB_PLATE_COLOR,FLH_JTB_PLATE_COLOR_LEN,fbuf);  //车牌颜色
//    tempbuf[pos]=fbuf[0];
    tempbuf[pos]=0x01;
    pos++;

    YS_PrmReadOneItem(FLH_JTB_PLATE_STRING,FLH_JTB_PLATE_STRING_LEN,fbuf);  //车牌号码
    len=YS_CodeBufRealLen(fbuf,FLH_JTB_PLATE_STRING_LEN);
    for(i=0; i<len; i++)
    {
        tempbuf[pos]=fbuf[i];
        pos++;
    }
    PackLen=YS_GprsDealJTBPtlSend(0x0100,tempbuf,pos,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackLogPackage
功能说明：封装一个登录数据包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackLogPackage(u8 *PackBuf)
{
    u8 fbuf[40];
    u16 len,PackLen;

    YS_PrmReadOneItem(FLH_JTB_SERV_DOWN_SN,FLH_JTB_SERV_DOWN_SN_LEN,fbuf);  //鉴权码
    len=YS_CodeBufRealLen(fbuf,FLH_JTB_SERV_DOWN_SN_LEN);
    PackLen=YS_GprsDealJTBPtlSend(0x0102,fbuf,len,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackHeartPackage
功能说明：封装一个心跳数据包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackHeartPackage(u8 *PackBuf)
{
    u16 PackLen;
    PackLen=YS_GprsDealJTBPtlSend(0x002,NULL,0,PackBuf,1,1);
    return(PackLen);
}

u16 YS_GprsAddOnePrmID(u16 CmdID,u8 *PackBuf)
{
    PackBuf[0]=0;
    PackBuf[1]=0;
    PackBuf[2]=CmdID/256;
    PackBuf[3]=CmdID%256;
    return(4);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsWriteJTBPrm
功能说明：读取终端 参数
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsReadJTBPrmServ(u8 *PackBuf)
{
    u16 i,pos,len;
    u8 fbuf[60];
    Avl_ULong ldat;
    t_Sys_Run_Status t_GetStatus;

    YS_RunGetSystemStatus(&t_GetStatus);
    pos=0;
    pos+=YS_GprsAddOnePrmID(0x0001,&PackBuf[pos]);  //终端心跳包
    PackBuf[pos]=4;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=fbuf[0];
    pos++;
    PackBuf[pos]=fbuf[1];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0013,&PackBuf[pos]); //主服务器地址
    YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
    PackBuf[pos]=len;
    pos++;
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=fbuf[i];
        pos++;
    }

    pos+=YS_GprsAddOnePrmID(0x0018,&PackBuf[pos]); //服务器端口
    PackBuf[pos]=4;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=fbuf[0];
    pos++;
    PackBuf[pos]=fbuf[1];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0029,&PackBuf[pos]); //ACC开上报时间间隔
    PackBuf[pos]=4;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=fbuf[0];
    pos++;
    PackBuf[pos]=fbuf[1];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0055,&PackBuf[pos]); //超速报警最高时速
    PackBuf[pos]=4;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_SPEED_VALUE,FLH_PRM_SPEED_VALUE_LEN,fbuf);
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=fbuf[0];
    pos++;
    PackBuf[pos]=fbuf[1];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0056,&PackBuf[pos]); //超速持续时间
    PackBuf[pos]=4;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_SPEED_TIME,FLH_PRM_SPEED_TIME_LEN,fbuf);
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=fbuf[0];
    pos++;
    PackBuf[pos]=fbuf[1];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0080,&PackBuf[pos]); //读取里程
    PackBuf[pos]=4;
    pos++;
    ldat.l = (t_GetStatus.Distance/100);
    PackBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    PackBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    PackBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    PackBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0083,&PackBuf[pos]); //读取车牌号
    YS_PrmReadOneItem(FLH_JTB_PLATE_STRING,FLH_JTB_PLATE_STRING_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_JTB_PLATE_STRING_LEN);
    PackBuf[pos]=len;
    pos++;
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=fbuf[i];
        pos++;
    }

    return(pos);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackPrmAckPackage
功能说明：查询参数应答包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackPrmAckPackage(u8 *PackBuf, u8 *AddInfo)
{
    u16 len;
    u8 fbuf[400];
    u8 PrmBuf[300];
    u16 i,pos,PackLen;

    pos=0;
    PrmBuf[pos]=AddInfo[10];
    pos++;
    PrmBuf[pos]=AddInfo[11];
    pos++;
    PrmBuf[pos]=0x08;
    pos++;
    len = YS_GprsReadJTBPrmServ(fbuf);
    for(i=0; i<len; i++)
    {
        PrmBuf[pos]=fbuf[i];
        pos++;
    }
    PackLen=YS_GprsDealJTBPtlSend(0x0104,PrmBuf,pos,PackBuf,1,1);
    return(PackLen);

}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackGpsInfo
功能说明：封装Gps信息
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackGpsInfo(u8 *GpsBuf)
{
    t_Sys_Run_Status	t_GetStatus;
    t_Gps_Data_Info	t_GetGps;
    t_Obd_Main_Info t_GetObd;
    Avl_ULong  ldat;
    u32 ValueTmp=0;
    u16 pos,i,len;
    u8 fbuf[10];
    u16 SpeedSet;

    pos=0;
    YS_RunGetSystemStatus(&t_GetStatus);
    YS_GpsGetPosData(&t_GetGps);
    YS_OBDGetMainInfo(&t_GetObd);

    /*------------------------报警状态字封装------------------------*/
    ldat.l=0;
    if(t_GetStatus.WarnStatus[AVL_WARM_EXIGEN]==TRUE)  //紧急报警
    {
        ldat.l=ldat.l|0x00000001;
    }

    if(t_GetGps.Speed>=30)
    {
        if(t_GetStatus.WarnStatus[AVL_WARM_SPEED]==TRUE) //超速报警
        {
            ldat.l=ldat.l|0x00000002;
        }
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_LONGDRIVE]==TRUE) //疲劳驾驶报警
    {
        ldat.l=ldat.l|0x00000004;
    }


    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_STATION]==TRUE) //GPS 模块损坏
    {
        ldat.l=ldat.l|0x00000010;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_GPSEXCEPT]==TRUE) //GPS 天线被剪
    {
        ldat.l=ldat.l|0x00000020;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_WRANGLE]==TRUE) //GPS 天线短路
    {
        ldat.l=ldat.l|0x00000040;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_LOWVOL]==TRUE) //低电压报警
    {
        ldat.l=ldat.l|0x00000080;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_OFFVOL]==TRUE) //断电报警
    {
        ldat.l=ldat.l|0x00000100;
    }
    if(t_GetStatus.WarnStatus[AVL_WARM_MOVE]==TRUE) //位移报警
    {
        ldat.l=ldat.l|0x10000000;
    }
    if(t_GetStatus.WarnStatus[AVL_WARM_INAREA]==TRUE) //区域内报警
    {
//        ldat.l=ldat.l|0x00000100;
    }
    if(t_GetStatus.WarnStatus[AVL_WARM_OUTAREA]==TRUE) //区域外报警
    {
//        ldat.l=ldat.l|0x00000100;
    }

    //LCD 故障
    //TTS 故障
    //摄像头
    //保留
    //当天累计驾驶超时

    if(t_GetStatus.WarnStatus[AVL_WARM_STOP]==TRUE) //停车超时
    {
        ldat.l=ldat.l|0x00080000;
    }

    if((t_GetStatus.WarnStatus[AVL_WARM_INAREA])||(t_GetStatus.WarnStatus[AVL_WARM_OUTAREA])) //进出区域
    {
        ldat.l=ldat.l|0x00100000;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_FULL]==TRUE) //进出路线报警
    {
        ldat.l=ldat.l|0x00200000;
    }

    //路线行驶时间不足/过长
    if(t_GetStatus.WarnStatus[AVL_WARM_COURSE]==TRUE) //偏航报警
    {
        ldat.l=ldat.l|0x00800000;
    }

    //车辆VSS 故障(速度传感器)
    //油量异常
    if(t_GetStatus.WarnStatus[AVL_WARM_THIEF]==TRUE) //防盗报警
    {
        ldat.l=ldat.l|0x04000000;
    }
    //非法点火
    //非法移位
    //碰撞侧翻报警
    //保留30-31
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    /*--------------------------------系统状态字封装--------------------*/
    ldat.l=0;
    if(t_GetStatus.AccStatus) //设置ACC 状态
    {
        ldat.l|=0x00000001;
    }

    if(t_GetGps.Effective) //设置GPS 定位标志
    {
        ldat.l|=0x00000002;
    }

    if(t_GetGps.SouthORNorth==FALSE) //南北纬标志
    {
        ldat.l|=0x00000004;
    }

    if(t_GetGps.EastORWest==FALSE) //东西经标志
    {
        ldat.l|=0x00000008;
    }

    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    /*------------------------添加GPS 数据------------------------*/
    ldat.l=YS_GprsPosToMillionDU(t_GetGps.Latitude); //纬度
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    ldat.l=YS_GprsPosToMillionDU(t_GetGps.Longitude); //经度
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    GpsBuf[pos]=0;  //海拔高度
    pos++;
    GpsBuf[pos]=0;
    pos++;

    if(t_GetStatus.WarnStatus[AVL_WARM_SPEED]==TRUE)
    {
        YS_PrmReadOneItem(FLH_PRM_SPEED_VALUE, FLH_PRM_SPEED_VALUE_LEN, fbuf);
        SpeedSet=fbuf[0]*256+fbuf[1];
        if(t_GetGps.Speed<=SpeedSet)
        {
            t_GetGps.Speed=SpeedSet*10;
        }
        else
        {
            t_GetGps.Speed=t_GetGps.Speed*10;
        }
    }
    else
    {
        t_GetGps.Speed=t_GetGps.Speed*10;   //转换为0.1千米
    }

    GpsBuf[pos]=t_GetGps.Speed/256;  //速度
    pos++;
    GpsBuf[pos]=t_GetGps.Speed%256;
    pos++;

    GpsBuf[pos]=t_GetGps.Course/256; //航向
    pos++;
    GpsBuf[pos]=t_GetGps.Course%256;
    pos++;

    if(YS_RunGsmColckisOK()==TRUE) //如果系统时间已同步
    {
        sjfun_Get_System_DateTime(t_GetGps.DataTime);  //取系统时间
    }
    else
    {
        YS_CodeCretaeBJDT(t_GetGps.DataTime);	//取GPS 时间，并转换为北京格式
    }

    for(i=0; i<6; i++)
    {
        GpsBuf[pos]=YS_CodeOneByteToBCD(t_GetGps.DataTime[i]);
        pos++;
    }

    GpsBuf[pos]=0x01;     //附加信息1 里程
    pos++;
    GpsBuf[pos]=0x04;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_DIS_RATIO, FLH_PRM_DIS_RATIO_LEN, fbuf);//读取里程比例系数

    ValueTmp=t_GetStatus.Distance*fbuf[0]/100000;

    ldat.l=(t_GetStatus.Distance/100)+ValueTmp;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    //附加信息二油量
    GpsBuf[pos]=0x02;
    pos++;
    GpsBuf[pos]=0x02;
    pos++;
    GpsBuf[pos]=t_GetObd.Fli/256;
    pos++;
    GpsBuf[pos]=t_GetObd.Fli%256;
    pos++;

    //附加信息三记录仪速度
    GpsBuf[pos]=0x03;
    pos++;
    GpsBuf[pos]=0x02;
    pos++;
//	GpsBuf[pos]=(t_GetStatus.McuSpeed*10)/256;
    GpsBuf[pos]=0;
    pos++;
//	GpsBuf[pos]=(t_GetStatus.McuSpeed*10)%256;
    GpsBuf[pos]=0;
    pos++;

    //超速报警附加信息
    if(t_GetStatus.WarnStatus[AVL_WARM_SPEED]==TRUE) //超速报警
    {
        GpsBuf[pos]=0x11;
        pos++;
        GpsBuf[pos]=0x01;
        pos++;
        GpsBuf[pos]=0;
        pos++;
    }

    //区域及路给报警附加信息
    if ((t_GetStatus.WarnStatus[AVL_WARM_INAREA]==TRUE)||(t_GetStatus.WarnStatus[AVL_WARM_OUTAREA]==TRUE))
    {

        YS_PrmReadOneItem(FLH_PRM_AREA_STOR, FLH_PRM_AREA_STOR_LEN, fbuf);
        GpsBuf[pos]=0x12;
        pos++;
        GpsBuf[pos]=0x06;
        pos++;
        GpsBuf[pos]=fbuf[2];//区域类型
        pos++;
        for ( i = 0; i <4 ; i++ )//区域ID
        {
            GpsBuf[pos]=fbuf[3+i];
            pos++;
        }
        GpsBuf[pos]=fbuf[1];//进、出区域
        pos++;
    }

    //Obd 数据
    GpsBuf[pos]=0xE3;
    pos++;
    GpsBuf[pos]=0x18;
    pos++;
    //电压
    GpsBuf[pos]=t_GetObd.Vmp;
    pos++;
    //转速
    GpsBuf[pos]=t_GetObd.Rpm/256;
    pos++;
    GpsBuf[pos]=t_GetObd.Rpm%256;
    pos++;
    //车速
    GpsBuf[pos]=t_GetObd.Spd;
    pos++;
    //节气门开度
    GpsBuf[pos]=t_GetObd.Toh;
    pos++;
    //发动机负荷
    GpsBuf[pos]=t_GetObd.Load;
    pos++;
    //冷却液温度
    GpsBuf[pos]=t_GetObd.Ect;
    pos++;
    //本次累计油耗
    GpsBuf[pos]=t_GetObd.Fs/256;
    pos++;
    GpsBuf[pos]=t_GetObd.Fs%256;
    pos++;
    //瞬时油耗
    GpsBuf[pos]=t_GetObd.XM;
    pos++;
    //总里程
    GpsBuf[pos]=0;
    pos++;
    GpsBuf[pos]=0;
    pos++;
    GpsBuf[pos]=0;
    pos++;
    GpsBuf[pos]=0;
    pos++;
    //本次里程
    ldat.l = t_GetObd.Ms;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;
#if ENABLE_X1_MODLE == 4
    //多基站定位附加信息
    len = YS_SysPackNetInfo(&GpsBuf[pos]);
    pos+=len;
#endif
    return(pos);

}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackCarCtlAckPackage
功能说明：车辆控制应答包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackCarCtlAckPackage(u8 *PackBuf, u8 *AddInfo)
{
    u8 GpsBuf[300],PrmBuf[300];

    u16 i,pos,PackLen;
    u16 GpsLen;

    pos=0;
    PrmBuf[pos]=AddInfo[10];
    pos++;
    PrmBuf[pos]=AddInfo[11];
    pos++;

    GpsLen=YS_GprsPackGpsInfo(GpsBuf);
    for ( i = 0; i < GpsLen; i++ )
    {
        PrmBuf[pos]=GpsBuf[i];
        pos++;
    }

    PackLen=YS_GprsDealJTBPtlSend(0x0500,PrmBuf,pos,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackPosPackage
功能说明：封装一个定位数据上传包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackPosPackage(u8 *PackBuf)
{
    u8 GpsBuf[300];
    u16 GpsLen,PackLen;

    GpsLen=YS_GprsPackGpsInfo(GpsBuf);
    PackLen=YS_GprsDealJTBPtlSend(0x0200,GpsBuf,GpsLen,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackDeadGpsInfo
功能说明：封装Gps信息
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackDeadGpsInfo(u8 *GpsBuf)
{
    t_Sys_Run_Status	t_GetStatus;
    t_Gps_Data_Info	t_GetGps;
    t_Gprs_DeadPos_Pack t_GetDeadPosPack;
    Avl_ULong  ldat;
    u16 pos,i;
    u8 fbuf[10];
    u16 SpeedSet;

    pos=0;
    YS_RunGetSystemStatus(&t_GetStatus);
    YS_GpsGetPosData(&t_GetGps);
    YS_GprsDeadPosGetPackInfo(&t_GetDeadPosPack);

    /*------------------------报警状态字封装------------------------*/
    ldat.l=0;
    if(t_GetStatus.WarnStatus[AVL_WARM_EXIGEN]==TRUE)  //紧急报警
    {
        ldat.l=ldat.l|0x00000001;
    }

    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_SPEED]==TRUE) //超速报警
    {
        ldat.l=ldat.l|0x00000002;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_LONGDRIVE]==TRUE) //疲劳驾驶报警
    {
        ldat.l=ldat.l|0x00000004;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_STATION]==TRUE) //GPS 模块损坏
    {
        ldat.l=ldat.l|0x00000010;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_GPSEXCEPT]==TRUE) //GPS 天线被剪
    {
        ldat.l=ldat.l|0x00000020;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_WRANGLE]==TRUE) //GPS 天线短路
    {
        ldat.l=ldat.l|0x00000040;
    }

    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_LOWVOL]==TRUE) //低电压报警
    {
        ldat.l=ldat.l|0x00000080;
    }

    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_OFFVOL]==TRUE) //断电报警
    {
        ldat.l=ldat.l|0x00000100;
    }
    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_MOVE]==TRUE) //位移报警
    {
        ldat.l=ldat.l|0x10000000;
    }
    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_INAREA]==TRUE) //区域内报警
    {
//        ldat.l=ldat.l|0x00000100;
    }
    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_OUTAREA]==TRUE) //区域外报警
    {
//        ldat.l=ldat.l|0x00000100;
    }
    //LCD 故障
    //TTS 故障
    //摄像头
    //保留
    //当天累计驾驶超时

    if(t_GetStatus.WarnStatus[AVL_WARM_STOP]==TRUE) //停车超时
    {
        ldat.l=ldat.l|0x00080000;
    }

    if((t_GetStatus.WarnStatus[AVL_WARM_INAREA])||(t_GetStatus.WarnStatus[AVL_WARM_OUTAREA])) //进出区域
    {
        ldat.l=ldat.l|0x00100000;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_FULL]==TRUE) //进出路线报警
    {
        ldat.l=ldat.l|0x00200000;
    }

    //路线行驶时间不足/过长
    if(t_GetStatus.WarnStatus[AVL_WARM_COURSE]==TRUE) //偏航报警
    {
        ldat.l=ldat.l|0x00800000;
    }

    //车辆VSS 故障(速度传感器)
    //油量异常
    if(t_GetStatus.WarnStatus[AVL_WARM_THIEF]==TRUE) //防盗报警
    {
        ldat.l=ldat.l|0x04000000;
    }
    //非法点火
    //非法移位
    //碰撞侧翻报警
    //保留30-31
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    /*--------------------------------系统状态字封装--------------------*/
    ldat.l=0;
//    if(t_GetStatus.AccStatus) //设置ACC 状态
//    {
    ldat.l|=0x00000001;
//    }

//    if(t_GetGps.Effective) //设置GPS 定位标志
//    {
    ldat.l|=0x00000002;
//    }

    if(t_GetGps.SouthORNorth==FALSE) //南北纬标志
    {
        ldat.l|=0x00000004;
    }

    if(t_GetGps.EastORWest==FALSE) //东西经标志
    {
        ldat.l|=0x00000008;
    }

    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    /*------------------------添加GPS 数据------------------------*/
    ldat.l=YS_GprsPosToMillionDU(t_GetDeadPosPack.DeadPosLati); //纬度
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    ldat.l=YS_GprsPosToMillionDU(t_GetDeadPosPack.DeadPosLongi); //经度
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    GpsBuf[pos]=0;  //海拔高度
    pos++;
    GpsBuf[pos]=0;
    pos++;

    if(t_GetStatus.WarnStatus[AVL_WARM_SPEED]==TRUE)
    {
        YS_PrmReadOneItem(FLH_PRM_SPEED_VALUE, FLH_PRM_SPEED_VALUE_LEN, fbuf);
        SpeedSet=fbuf[0]*256+fbuf[1];
        if(t_GetGps.Speed<=SpeedSet)
        {
            t_GetGps.Speed=SpeedSet*10;
        }
    }
    else
    {
        t_GetGps.Speed=t_GetGps.Speed*10;   //转换为0.1千米
    }

    GpsBuf[pos]=t_GetDeadPosPack.DeadPosSpeed/256;  //速度
    pos++;
    GpsBuf[pos]=t_GetDeadPosPack.DeadPosSpeed%256;
    pos++;

    GpsBuf[pos]=t_GetDeadPosPack.DeadPosCourse/256; //航向
    pos++;
    GpsBuf[pos]=t_GetDeadPosPack.DeadPosCourse%256;
    pos++;

    //已转化为北京时间
    for(i=0; i<6; i++)
    {
        GpsBuf[pos]=YS_CodeOneByteToBCD(t_GetDeadPosPack.DeadPosTime[i]);
        pos++;
    }

    GpsBuf[pos]=0x01;     //附加信息1 里程
    pos++;
    GpsBuf[pos]=0x04;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_DIS_RATIO, FLH_PRM_DIS_RATIO_LEN, fbuf);//读取里程比例系数

    ldat.l=(t_GetStatus.Distance*(fbuf[0]+1000)/100000);//
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    //附加信息二油量
    GpsBuf[pos]=0x02;
    pos++;
    GpsBuf[pos]=0x02;
    pos++;
    GpsBuf[pos]=0x01;
    pos++;
    GpsBuf[pos]=0x01;
    pos++;

    //附加信息三记录仪速度
    GpsBuf[pos]=0x03;
    pos++;
    GpsBuf[pos]=0x02;
    pos++;
//	GpsBuf[pos]=(t_GetStatus.McuSpeed*10)/256;
    GpsBuf[pos]=0;
    pos++;
//	GpsBuf[pos]=(t_GetStatus.McuSpeed*10)%256;
    GpsBuf[pos]=0;
    pos++;

    //超速报警附加信息
    if(t_GetStatus.WarnStatus[AVL_WARM_SPEED]==TRUE) //超速报警
    {
        GpsBuf[pos]=0x11;
        pos++;
        GpsBuf[pos]=0x01;
        pos++;
        GpsBuf[pos]=0;
        pos++;
    }

    //区域及路给报警附加信息
    if ((t_GetStatus.WarnStatus[AVL_WARM_INAREA]==TRUE)||(t_GetStatus.WarnStatus[AVL_WARM_OUTAREA]==TRUE))
    {

        YS_PrmReadOneItem(FLH_PRM_AREA_STOR, FLH_PRM_AREA_STOR_LEN, fbuf);
        GpsBuf[pos]=0x12;
        pos++;
        GpsBuf[pos]=0x06;
        pos++;
        GpsBuf[pos]=fbuf[2];//区域类型
        pos++;
        for ( i = 0; i <4 ; i++ )//区域ID
        {
            GpsBuf[pos]=fbuf[3+i];
            pos++;
        }
        GpsBuf[pos]=fbuf[1];//进、出区域
        pos++;
    }
    //路线行驶不足过长附加信息

    return(pos);

}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackDeadPosPackage
功能说明：封装一个定位数据上传包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackDeadPosPackage(u8 *PackBuf)
{
    u8 GpsBuf[300];
    u16 GpsLen,PackLen;

    GpsLen=YS_GprsPackDeadGpsInfo(GpsBuf);
    PackLen=YS_GprsDealJTBPtlSend(0x0200,GpsBuf,GpsLen,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsSendPointPackage
功能说明：点名应答包
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsSendPointPackage(u8 *PackBuf, u8 *AddInfo)
{
    u8 GpsBuf[300];
    u16 GpsLen,PackLen;

    GpsBuf[0]=AddInfo[10];
    GpsBuf[1]=AddInfo[11];
    GpsLen=YS_GprsPackGpsInfo(&GpsBuf[2]);
    GpsLen+=2;
    PackLen=YS_GprsDealJTBPtlSend(0x0201,GpsBuf,GpsLen,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackGpsRsqInfo
功能说明：封装Gps信息
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackGpsRsqInfo(u8 *GpsBuf)
{
    t_Sys_Run_Status	t_GetStatus;
    t_Gps_Data_Info	t_GetGps;
    Avl_UWord  wdat;
    Avl_ULong  ldat;
    u16 pos,i;

    pos=0;
    YS_RunGetSystemStatus(&t_GetStatus);
    YS_GpsGetPosData(&t_GetGps);

    if(YS_RunGsmColckisOK()==TRUE) //如果系统时间已同步
    {
        sjfun_Get_System_DateTime(t_GetGps.DataTime);  //取系统时间
    }
    else
    {
        YS_CodeCretaeBJDT(t_GetGps.DataTime);	//取GPS 时间，并转换为北京格式
    }

    for(i=0; i<6; i++)
    {
        GpsBuf[pos]=YS_CodeOneByteToBCD(t_GetGps.DataTime[i]);
        pos++;
    }

    GpsBuf[pos]=0;
    if(t_GetGps.Effective) //设置GPS 定位标志
    {
        GpsBuf[pos]|=0x01;
    }

    if(t_GetGps.SouthORNorth==FALSE) //南北纬标志
    {
        GpsBuf[pos]|=0x02;
    }

    if(t_GetGps.EastORWest==FALSE) //东西经标志
    {
        GpsBuf[pos]|=0x04;
    }
    pos++;


    /*------------------------添加GPS 数据------------------------*/
    ldat.l=YS_GprsPosToMillionDU(t_GetGps.Latitude); //纬度
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    ldat.l=YS_GprsPosToMillionDU(t_GetGps.Longitude); //经度
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    ldat.l=t_GetStatus.CellID;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    wdat.w=t_GetStatus.AreaID;
    GpsBuf[pos]=wdat.b[AVL_WSTOR_FIR];
    pos++;
    GpsBuf[pos]=wdat.b[AVL_WSTOR_SEC];
    pos++;

    wdat.w=t_GetStatus.CountryName;
    GpsBuf[pos]=wdat.b[AVL_WSTOR_FIR];
    pos++;
    GpsBuf[pos]=wdat.b[AVL_WSTOR_SEC];
    pos++;

    ldat.l=t_GetStatus.NetName;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    return(pos);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsCreatePosRsqInfo
功能说明：808中文位置获取
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsCreatePosRsqInfo(u8 *PackBuf)
{
    u8 GpsBuf[300];
    u16 GpsLen,PackLen;

    GpsLen=YS_GprsPackGpsRsqInfo(GpsBuf);
    PackLen=YS_GprsDealJTBPtlSend(0xB000,GpsBuf,GpsLen,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsTraceParase
功能说明：平台下发的TRACE 指令解析处理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsTraceParase(u8 *dbuf, u16 dlen)
{
    u16 TraceTimes,TraceDelay;
    u32 TraceEdtime;

    TraceDelay = dbuf[0]*256+dbuf[1];
    TraceEdtime=dbuf[2]*256*256*256+dbuf[3]*256*256+dbuf[4]*256+dbuf[5];
    TraceTimes = (u16)(TraceEdtime/TraceDelay);
    YS_RunTraceInterFace(TraceTimes,TraceDelay);
}

void YS_GprsJTBRegAckDeal(u8 *dbuf, u16 dlen)
{
    u8  fbuf[40];
    u16 i,len;

    switch(dbuf[2])
    {
        case 0: //注册成功
            len=dlen-3;
            if(len>=40) len=39;
            for(i=0; i<len; i++)
            {
                fbuf[i]=dbuf[3+i];
            }
            fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_JTB_SERV_DOWN_SN,FLH_JTB_SERV_DOWN_SN_LEN,fbuf); //存贮鉴权码
            break;

        case 1://车辆已被注册
            YS_GprsDebugString("\nCar already reg",0);
            break;

        case 2://数据库中无该车辆
            YS_GprsDebugString("\nno car in lib",0);
            break;

        case 3://终端恺已被注册
            YS_GprsDebugString("\nTeri already reg",0);
            break;

        case 4://数据库中无该终端
            YS_GprsDebugString("\nno Teri in lib",0);
            break;

        default:
            YS_GprsDebugString("\nrec unknow reg ack",0);
            break;

    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsGetStrChePosUnit
功能说明：位置请求
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsGetStrChePosUnit(u8 *dbuf, u16 dlen, u8 StrID, char *StrRlt)
{
    u16 i,IDCount,pos,DataCount;
    bool Result1;

    IDCount=0;
    Result1=FALSE;
    pos=0;
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]==':')
        {
            IDCount++;
            if(IDCount>=StrID)
            {
                pos=i+1;
                Result1=TRUE;
                break;
            }
        }
    }

    DataCount=0;
//    Result2=FALSE;
    for(i=pos; i<dlen; i++)
    {
        StrRlt[i-pos]=dbuf[i];
        DataCount++;
    }

    if(Result1==TRUE)
    {
        StrRlt[DataCount]=0;
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsParaseHeadTail
功能说明：消息体包头处理
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsParaseHeadTail(u8 *dbuf, u16 dlen)
{
    bool result;
    u16 i,pos,newlen;
    u8	verify;

//	AVL_TRACE_BIN(dbuf,dlen);
//	YS_GprsDebugString("\nJTB parase3");
    pos=0;  //数据转义还原
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]==0x7D)
        {
            if(dbuf[i+1]==0x01)
            {
                dbuf[pos]=0x7D;
                i++;
            }
            else if(dbuf[i+1]==0x02)
            {
                dbuf[pos]=0x7E;
                i++;
            }
            else
            {
                dbuf[pos]=dbuf[i];
            }
        }
        else
        {
            dbuf[pos]=dbuf[i];
        }
        pos++;
    }

    newlen=pos;
    verify=0;
    for(i=1; i<newlen-2; i++)
    {
        verify^=dbuf[i];
    }


    if(verify==dbuf[newlen-2])
    {
        for(i=1; i<16; i++)  //保存信息头部份
        {
            t_AvlSckParase.a_AddInfo[i-1]=dbuf[i];
        }

        if((dbuf[3]&0x20)==0x20)
        {
            pos=17;
        }
        else
        {
            pos=13;
        }

        t_AvlSckParase.v_CmdID=dbuf[1]*256+dbuf[2];
        t_AvlSckParase.v_dlen=newlen-(pos+2);
        if(t_AvlSckParase.v_dlen>0)
        {
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckParase.a_dbuf[i]=dbuf[pos+i];
            }
        }
        result=TRUE;
    }
    else
    {
        result=FALSE;
    }
    return(result);
}

void YS_GprsLongToPos(u8 *Lonbuf, u8 *aimbuf)
{
    Avl_ULong ldat;
    double f1;
    char strdat[20];
    char strbuf[20];
    u16 i;

    for(i=0; i<4; i++)
    {
        ldat.b[3-i] =Lonbuf[i];
    }

    aimbuf[0]=(u8)(ldat.l/1000000);
    f1 = ldat.l-aimbuf[0]*1000000;
    f1=f1/1000000;
    f1*=60;
    f1/=100;
    sprintf(strbuf,"%.6f",f1);
    for(i=0; i<3; i++)
    {
        strdat[0]=strbuf[2*(i+1)];
        strdat[1]=strbuf[2*(i+1)+1];
        strdat[2]=0;
        aimbuf[i+1]=atoi(strdat);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsAreaSet_808
功能说明：平台区域下发设置
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsAreaSet_808(u8 area, u8 *dbuf, u16 dlen)
{
    u8 pos=0,i;
    u8 AreaBuf[40];
    memset(AreaBuf, 0x00, 40);

    switch (area)
    {
        case 1 ://圆形区域
            AreaBuf[pos]=1;//区域有效
            pos++;
            if(((dbuf[7]&0x04)==0x04)||((dbuf[7]&0x08)==0x08))//进区域
            {
                AreaBuf[pos]=0;//入区域
                pos++;
            }
            else if(((dbuf[7]&0x10)==0x10)||((dbuf[7]&0x20)==0x20))//出区域
            {
                AreaBuf[pos]=1;//出区域
                pos++;
            }
            else
            {
                AreaBuf[pos]=3;
                pos++;
            }
            AreaBuf[pos]=1;//圆形区域
            pos++;

            for(i=0; i<4; i++) //区域ID
            {
                AreaBuf[pos]=dbuf[2+i];
                pos++;
            }
            YS_GprsLongToPos(&dbuf[12],&AreaBuf[pos]);//经度
            pos+=4;

            YS_GprsLongToPos(&dbuf[8],&AreaBuf[pos]);//经度
            pos+=4;

            for(i=0; i<4; i++) //半径
            {
                AreaBuf[pos]=dbuf[16+i];
                pos++;
            }
            AreaBuf[FLH_PRM_AREA_STOR_LEN-1]=0;//重新获得报警机会
            break;

        case 2 ://矩形区域
            AreaBuf[pos]=1;//区域有效
            pos++;
            if(((dbuf[7]&0x04)==0x04)||((dbuf[7]&0x08)==0x08))//进区域
            {
                AreaBuf[pos]=0;//入区域
                pos++;
            }
            else if(((dbuf[7]&0x10)==0x10)||((dbuf[7]&0x20)==0x20))//出区域
            {
                AreaBuf[pos]=1;//出区域
                pos++;
            }
            else
            {
                AreaBuf[pos]=3;
                pos++;
            }
            AreaBuf[pos]=2;//矩形区域
            pos++;

            for(i=0; i<4; i++) //区域ID
            {
                AreaBuf[pos]=dbuf[2+i];
                pos++;
            }
            YS_GprsLongToPos(&dbuf[12],&AreaBuf[pos]);//左上角经度
            pos+=4;

            YS_GprsLongToPos(&dbuf[8],&AreaBuf[pos]);//左上角纬度
            pos+=4;

            YS_GprsLongToPos(&dbuf[20],&AreaBuf[pos]);//右下角经度
            pos+=4;

            YS_GprsLongToPos(&dbuf[16],&AreaBuf[pos]);//右下角纬度
            pos+=4;
            AreaBuf[FLH_PRM_AREA_STOR_LEN-1]=0;//重新获得报警机会
            break;

        case 3 ://多边形区域

            break;
        case 0 ://删除区域
            AreaBuf[pos]=0;//区域 无效
        default:
            break;
    }
    YS_GpsInitAreaDeal();
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, AreaBuf, 40);
    YS_PrmWriteOneItem(FLH_PRM_AREA_STOR, FLH_PRM_AREA_STOR_LEN, AreaBuf);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsAreaDel_808
功能说明：平台区域下发设置
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsAreaDel_808(u8 area, u8 *dbuf, u16 dlen)
{
    u8 AreaBuf[40];
    memset(AreaBuf, 0, 40);
    YS_PrmWriteOneItem(FLH_PRM_AREA_STOR, FLH_PRM_AREA_STOR_LEN, AreaBuf);
    return TRUE;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsWriteJTBPrm
功能说明：平台参数下发设置
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GprsWriteJTBPrm(u8 *dbuf, u16 dlen)
{
    u16 pos,PrmID;
    u16 PrmLen;
    u8  fbuf[60];
    Avl_ULong ldat;
    bool ResetFlag = FALSE;

    pos=0;
    while(pos<dlen)
    {
        pos+=2;
        PrmID=dbuf[pos]*256+dbuf[pos+1];
        pos+=2;
        PrmLen=dbuf[pos];
        pos++;
        memset(fbuf,0,60);
        memcpy(fbuf,&dbuf[pos],PrmLen);
        switch(PrmID)
        {
            case 0x0001:  //心跳包周期
                YS_PrmWriteOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,&dbuf[pos+2]);
                pos+=PrmLen;
                break;

            case 0x0013://IP地址
                YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,&dbuf[pos]);
                ResetFlag = TRUE;
                pos+=PrmLen;
                break;

            case 0x0018://端口
                YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,&dbuf[pos+2]);
                ResetFlag = TRUE;
                pos+=PrmLen;
                break;

            case 0x0029://GPS上传频率
                YS_PrmWriteOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,&dbuf[pos+2]);
                pos+=PrmLen;
                break;

            case 0x0055: //超速门限
                YS_PrmWriteOneItem(FLH_PRM_SPEED_VALUE,FLH_PRM_SPEED_VALUE_LEN,&dbuf[pos+2]);
                pos+=PrmLen;
                break;

            case 0x0056: //超速时间
                YS_PrmWriteOneItem(FLH_PRM_SPEED_TIME,FLH_PRM_SPEED_TIME_LEN,&dbuf[pos+2]);
                pos+=PrmLen;
                break;

            case 0x0080://设置里程
                ldat.l=dbuf[pos]*256*256*256+dbuf[pos+1]*256*256+dbuf[pos+2]*256+dbuf[pos+3];
                ldat.l *= 100;
                fbuf[0] = ldat.b[AVL_LSTOR_FIR];
                fbuf[1] = ldat.b[AVL_LSTOR_SEC];
                fbuf[2] = ldat.b[AVL_LSTOR_THI];
                fbuf[3] = ldat.b[AVL_LSTOR_FOR];

                YS_PrmWriteOneItem(FLH_PRM_DIS_VALUE,FLH_PRM_DIS_VALUE_LEN,fbuf);
                YS_GpsInitDistance();
                pos+=PrmLen;
                break;

            case 0x0083://设置车牌号
                YS_PrmWriteOneItem(FLH_JTB_PLATE_STRING,FLH_JTB_PLATE_STRING_LEN,&dbuf[pos]);
                pos+=PrmLen;
                break;

            default:
                break;

        }

    }
    if(ResetFlag == TRUE)
    {
        YS_SysRsqSystemRST(YS_RST_FLAG_USER);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsDealJTBServRecData
功能说明：从数据缓存区中提取数据包
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsDealJTBServRecData(u8 *buf, u16 len)
{
    bool result;
    u16 i,pos,count;

    if((buf[0]==0x7E)&&(buf[len-1]==0x7E))  //如果包头包尾正确
    {
        pos=0;
        count=0;
        for(i=0; i<len; i++)
        {
            pos++;
            if(buf[i]==0x7E)
            {
                count++;
                if(count>=2)
                {
                    break;
                }
            }
        }

        if(YS_GprsParaseHeadTail(buf,pos)==TRUE)
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

/****************OBD 应答包*****************/
/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackObdInfoControl
功能说明：读取OBD信息
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackObdInfoControl(u8 *ObdBuf, u8 *AddInfo, u16 obdlen, u8 *obdPrm)
{
    u16 i,pos,PackLen;
    u16 GpsLen;
    t_Obd_Main_Info t_GetObd;

    YS_GprsDebugString("\nServ Obd control",0);
    YS_OBDGetMainInfo(&t_GetObd);

    pos=0;
    ObdBuf[pos]=AddInfo[10];
    pos++;
    ObdBuf[pos]=AddInfo[11];
    pos++;

    ObdBuf[pos]=obdlen;
    pos++;

    for ( i = 0; i < obdlen; i++ )
    {
        switch (obdPrm[i])
        {
            case 0x04 ://发动机负荷
                ObdBuf[pos]=obdPrm[i]; //参数ID
                pos++;
                ObdBuf[pos]=0x01; //参数长度
                pos++;
                ObdBuf[pos]=t_GetObd.Load; //参数值
                pos++;
                break;

            case 0x05 ://冷却液温度
                ObdBuf[pos]=obdPrm[i]; //参数ID
                pos++;
                ObdBuf[pos]=0x01; //参数长度
                pos++;
                ObdBuf[pos]=t_GetObd.Ect+40; //参数值
                pos++;
                break;

            case 0x0c ://发动机转速
                ObdBuf[pos]=obdPrm[i]; //参数ID
                pos++;
                ObdBuf[pos]=0x02; //参数长度
                pos++;
                ObdBuf[pos]=t_GetObd.Rpm/256; //参数值
                pos++;
                ObdBuf[pos]=t_GetObd.Rpm%256; //参数值
                pos++;
                break;

            case 0x0d ://车速
                ObdBuf[pos]=obdPrm[i]; //参数ID
                pos++;
                ObdBuf[pos]=0x01; //参数长度
                pos++;
                ObdBuf[pos]=t_GetObd.Spd; //参数值
                pos++;
                break;

            case 0x1f ://发动机启动后运行时间
                ObdBuf[pos]=obdPrm[i]; //参数ID
                pos++;
                ObdBuf[pos]=0x02; //参数长度
                pos++;
                ObdBuf[pos]=t_GetObd.Ts/256; //参数值
                pos++;
                ObdBuf[pos]=t_GetObd.Ts%256; //参数值
                pos++;
                break;

            case 0x42://电瓶电压
                ObdBuf[pos]=obdPrm[i]; //参数ID
                pos++;
                ObdBuf[pos]=0x01; //参数长度
                pos++;
                ObdBuf[pos]=t_GetObd.Vmp; //参数值
                pos++;
                break;

            case 0x06 :
            case 0x07 :
            case 0x08 :
            case 0x09 :
            case 0x0a :
            case 0x0b :
            case 0x0e :
            case 0x0f :
            case 0x11 :
            case 0x12 :
            case 0x13 :
            case 0x1c :
            case 0x1d :
            case 0x1e :
            case 0x2c :
            case 0x2e :
            case 0x2f :
            case 0x30 :
            case 0x33 :
            case 0x44 :
            case 0x45 :
            case 0x46 :
            case 0x47 :
            case 0x48 :
            case 0x49 :
            case 0x4a :
            case 0x4b :
            case 0x4c :
            case 0x51 :
            case 0x52 :
            case 0x5a :
            case 0x5c :
                ObdBuf[pos]=obdPrm[i]; //参数ID
                pos++;
                ObdBuf[pos]=0x01; //参数长度
                pos++;
                ObdBuf[pos]=0+100; //参数值
                pos++;
                break;

            case 0x10 :
            case 0x14 :
            case 0x15 :
            case 0x16 :
            case 0x17 :
            case 0x18 :
            case 0x19 :
            case 0x1a :
            case 0x1b :
            case 0x21 :
            case 0x23 :
            case 0x31 :
            case 0x32 :
            case 0x3c :
            case 0x3d :
            case 0x3e :
            case 0x3f :
            case 0x4d :
            case 0x4e :
            case 0x54 :
            case 0x55 :
            case 0x56 :
            case 0x57 :
            case 0x58 :
            case 0x59 :
                ObdBuf[pos]=obdPrm[i]; //参数ID
                pos++;
                ObdBuf[pos]=0x02; //参数长度
                pos++;
                ObdBuf[pos]=0; //参数值
                pos++;
                ObdBuf[pos]=0; //参数值
                pos++;
                break;

            case 0x22 :
            case 0x24 :
            case 0x25 :
            case 0x26 :
            case 0x27 :
            case 0x28 :
            case 0x29 :
            case 0x2a :
            case 0x2b :
            case 0x53 :
            case 0x5d :
            case 0x5e :
                ObdBuf[pos]=obdPrm[i]; //参数ID
                pos++;
                ObdBuf[pos]=0x04; //参数长度
                pos++;
                ObdBuf[pos]=0; //参数值
                pos++;
                ObdBuf[pos]=0; //参数值
                pos++;
                ObdBuf[pos]=0; //参数值
                pos++;
                ObdBuf[pos]=0; //参数值
                pos++;
                break;

            default:
                break;
        }
    }

    return(pos);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GprsPackObdInfoAckPackage
功能说明：读取OBD信息
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackObdInfoAckPackage(u8 *PackBuf, u8 *dbuf, u16 dlen)
{
    u16 PackLen;
    YS_GprsDebugString("\nServ Obd ack",0);
    PackLen=YS_GprsDealJTBPtlSend(0x0F03,dbuf,dlen,PackBuf,1,1);
    return PackLen;
}

/*********************************/


/*-----------------------------------------------------------------------------------------
函数名：YS_GprsJBTSckPtlUpConver
功能说明：平台下发数据包解析指令
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GprsJBTSckPtlUpConver(void)
{
    bool result,ack;
    u16 i,AckCmd,len;
    u8 fbuf[5],buf[500],DbgBuf[100];

    result=TRUE;
    memcpy(t_AvlSckConver.a_AddInfo,t_AvlSckParase.a_AddInfo,AVL_ADD_INFO_LEN);

    switch(t_AvlSckParase.v_CmdID)
    {
        case 0x8001: //平台通用应答
            AckCmd=t_AvlSckParase.a_dbuf[2]*256+t_AvlSckParase.a_dbuf[3];
            t_AvlSckConver.a_dbuf[0]=t_AvlSckParase.a_dbuf[4];
            t_AvlSckConver.v_dlen=1;
            switch(AckCmd)
            {
                case 0x0002: //服务器心跳应答
                    t_AvlSckConver.v_dlen=0;
                    t_AvlSckConver.v_CmdID=INTER_CMD_GPS_HEART_ACK;
                    YS_RunIdleHeartAck();
                    break;

                case 0x0003: //服务器注销应答
                    t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_UNREG;
                    break;
                case 0x0102: //服务器登录应答
                    YS_GprsDebugString("\nServ log ack cmd",0);
                    t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_LOGACK;
                    if (t_AvlSckConver.a_dbuf[0]==0)
                    {
                        YS_RunLoginServerAck();
                    }
                    else
                    {
                        fbuf[0]=FJYD_TERI_STATUS_INIT;
                        YS_PrmWriteOneItem(FLH_PRM_ACTIVE_FLAG, FLH_PRM_ACTIVE_FLAG_LEN, fbuf);
                    }
                    break;

                case 0x0200: //位置及报警信息应答
                    YS_GprsDebugString("\nServ pos ack",0);
                    if(4==t_AvlSckConver.a_dbuf[0])
                    {
                        YS_RunCleanWarmFlag();
                    }
                    else if(0==t_AvlSckConver.a_dbuf[0])
                    {
                        YS_RunCleanWarmACK();
                    }
                    t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_POSACK;
                    break;

                case 0x0800: //图片上传应答
                    t_AvlSckConver.v_dlen=0;
                    t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_MULTINFOACK;
                    break;

                case 0x0801: //图片数据应答
                    t_AvlSckConver.v_dlen=0;
                    t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_MULSENDACK;
                    break;
                default:
                    result=FALSE;
                    break;
            }
            break;

        case 0x8100: //终端注册应答
            YS_GprsDebugString("\nServ reg ack cmd",0);
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_REGACK;

            if((2==t_AvlSckConver.a_dbuf[2])||(t_AvlSckConver.a_dbuf[2]>=4))//注册失败
            {
                break;
            }
            YS_GprsJTBRegAckDeal(t_AvlSckConver.a_dbuf,t_AvlSckParase.v_dlen);
            YS_RunRegServerAck();
            break;

        case 0x8103: //设置终端参数
            for(i=0; i<t_AvlSckParase.v_dlen-1; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i+1];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen-1;
            t_AvlSckConver.v_CmdID=INTER_CMD_SET_JTB_PRM;
            YS_GprsWriteJTBPrm(t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckParase.a_AddInfo, 17);
            break;

        case 0x8104: //查询终端参数
            t_AvlSckConver.v_dlen=0;
            t_AvlSckConver.v_CmdID=INTER_CMD_READ_JTB_PRM_SERV;
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_READPRMACK, t_AvlSckParase.a_AddInfo, 17);
            break;

        case 0x8105: //终端控制
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            if(t_AvlSckConver.v_dlen>0)
            {
                for(i=0; i<t_AvlSckConver.v_dlen; i++)
                {
                    t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
                }
                if(0x64==t_AvlSckConver.a_dbuf[0])//断油电
                {
                    fbuf[0]=0;
                    YS_PrmWriteOneItem(FLH_PRM_RELAY_STOR, FLH_PRM_RELAY_STOR_LEN, fbuf);
#if ENABLE_X1_MODLE == 3
                    YS_MotorIOCtrl(1);
#else
                    YS_RunMotorCtrlInterface();
#endif
                }
                else if(0x65==t_AvlSckConver.a_dbuf[0])//通油电
                {
                    fbuf[0]=1;
                    YS_PrmWriteOneItem(FLH_PRM_RELAY_STOR, FLH_PRM_RELAY_STOR_LEN, fbuf);
                    YS_MotorIOCtrl(0);
                    YS_IOACCDydReCount();
                }
            }
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckParase.a_AddInfo, 17);
            t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_CTR;
            break;

        case 0x8201: //终端位置查询
            t_AvlSckConver.v_CmdID=INTER_CMD_GET_ONE_GPS;
            YS_RunEntryWakeupMode();
            t_AvlSckParase.a_AddInfo[16]=1;
            YS_GprsServerSendInterface(SERV_UP_CMD_ONEPOS, t_AvlSckParase.a_AddInfo, 17);
            break;

        case 0x8202: //位置跟踪
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_GPRS_GETPOS_BYTIME;
            YS_RunEntryWakeupMode();
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckParase.a_AddInfo, 17);
            YS_GprsTraceParase(t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            break;

        case 0x8400: //电话回拔
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckParase.a_AddInfo, 17);
//         YS_CallOutInterface((char *)(&t_AvlSckConver.a_dbuf[1]));
            break;

        case 0x8500://车辆控制
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            if(0x03&t_AvlSckConver.a_dbuf[0])//断油电
            {
                fbuf[0]=0;
                YS_PrmWriteOneItem(FLH_PRM_RELAY_STOR, FLH_PRM_RELAY_STOR_LEN, fbuf);
#if ENABLE_X1_MODLE == 3
                    YS_MotorIOCtrl(1);
#else
                    YS_RunMotorCtrlInterface();
#endif
            }
            else
            {
                fbuf[0]=1;
                YS_PrmWriteOneItem(FLH_PRM_RELAY_STOR, FLH_PRM_RELAY_STOR_LEN, fbuf);
                YS_MotorIOCtrl(0);
                YS_IOACCDydReCount();
            }
            YS_GprsServerSendInterface(SERV_UP_CMD_CARCTL, t_AvlSckParase.a_AddInfo, 17);
            break;

        case 0x8600: //设置圆形区域
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            YS_GprsAreaSet_808(1,t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckConver.a_AddInfo, 17);
            break;

        case 0x8601: //删除圆形区域
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            ack=YS_GprsAreaDel_808(1,t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            if(ack==TRUE)
            {
                t_AvlSckParase.a_AddInfo[16]=0;
                YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckConver.a_AddInfo, 17);
            }

            break;

        case 0x8602: //设置矩形区域
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            YS_GprsAreaSet_808(2,t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckConver.a_AddInfo, 17);
            break;

        case 0x8603: //删除矩形区域
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            ack=YS_GprsAreaDel_808(2,t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            if(ack==TRUE)
            {
                t_AvlSckParase.a_AddInfo[16]=0;
                YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckConver.a_AddInfo, 17);
            }
            break;

        case 0x8604: //设置多边形区域
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            break;

        case 0x8605: //删除多边形区域
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            break;

        case 0xA800://中文位置请求
            if(TRUE==YS_GprsGetStrChePosUnit(t_AvlSckParase.a_dbuf,t_AvlSckParase.v_dlen,1,(char *)t_AvlSckConver.a_dbuf))
            {
                t_AvlSckConver.v_dlen=strlen((char *)t_AvlSckConver.a_dbuf);
//                YS_SmsSendRsqPosMsg(t_AvlSckConver.a_dbuf, t_AvlSckConver.v_dlen);
 //               YS_SmsSendUTF8Interface(t_AvlSckConver.a_dbuf, t_AvlSckConver.v_dlen);
                YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            }
            break;

        case 0x8F03: //OBD读取指定数据流
            YS_GprsDebugString("\nServ Obd chk",0);
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;

            YS_CodeHextoString(t_AvlSckConver.a_dbuf, t_AvlSckConver.v_dlen, DbgBuf);
            ycsj_debug((char *)DbgBuf);
            len = YS_GprsPackObdInfoControl(buf, t_AvlSckConver.a_AddInfo, t_AvlSckParase.v_dlen, t_AvlSckConver.a_dbuf);
            YS_GprsServerSendInterface(SERV_UP_CMD_OBDINFO, buf, len);
            break;
#if 0
        case 0x8300: //文本信息下发
            YS_GprsDebugString("\nGet center text down",0);
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen-1;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i+1];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_STOLCD_MSG;
            break;

        case 0x8301: //事件设置
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_EVENT_SET;
            break;

        case 0x8302: //提问下发
            /*
            		for(i=0; i<t_AvlSckParase.v_dlen; i++)
            		{
            			t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            		}
            		t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;*/
            YS_GprsDebugString("\nGet center rsq down");
            t_AvlSckConver.v_dlen=avltra69_41ParaseRsqDown(t_AvlSckParase.a_dbuf,t_AvlSckParase.v_dlen,t_AvlSckConver.a_dbuf);
            t_AvlSckConver.v_CmdID=INTER_CMD_STOLCD_MSG;
            break;

        case 0x8303: //信息点播菜单设置
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_INFO_MENU;
            break;

        case 0x8304: //信息服务
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen-3;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i+3];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_STOLCD_MSG;
            break;

        case 0x8400: //电话回拔
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_CALL_DEAL;
            break;

        case 0x8401: //设置电话簿
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_PHONE_SET;
            break;

        case 0x8500: //车辆控制
            t_AvlSckConver.a_dbuf[0]=t_AvlSckParase.a_dbuf[0];
            t_AvlSckConver.a_dbuf[1]=t_AvlSckConver.a_AddInfo[10];
            t_AvlSckConver.a_dbuf[2]=t_AvlSckConver.a_AddInfo[11];
            t_AvlSckConver.v_dlen=3;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_CAR_CTRL;
            break;

        case 0x8606: //设置线路
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTBSET_ROUTE;
            break;

        case 0x8607: //删除线路
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTBDEL_ROUTE;
            break;

        case 0x8700: //记录仪采集命令
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_BOXGET;
            break;

        case 0x8701: //记录仪参数设置命令
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_BOXPRM;
            break;

        case 0x8800: //多媒体数据上传应答
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_MULDATAACK;
            break;

        case 0x8801: //摄像头立即拍命令
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_SNOPPIC;
            break;

        case 0x8802: //存贮多媒体数据检索
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_MULRSQ;
            break;

        case 0x8803: //存贮多媒体数据上传
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_MULGET;
            break;

        case 0x8804: //录音开始命令
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_VOICEBEIGN;
            break;

        case 0x8900: //数据下行透传
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_CLRDOWN;
            break;

        case 0x8A00: //平台公钥
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_RSAINFO;
            break;
#endif


        default:
            result=FALSE;
            break;
    }

    return(result);
}
