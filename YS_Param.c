/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Param.c  2013-06-22
此文件用管理掉电参数的读写工作
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

t_Param_Manage t_ParamManage;

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsCompString
功能说明：验证字符串是事为纯数字字符串
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsCompString(u8 *PackBuf, u16 dlen, u16 CompLen)
{
    u16 i;
    if(dlen!=CompLen)
    {
        return(FALSE);
    }

    for(i=0; i<dlen; i++)
    {
        if((PackBuf[i]<0x30)||(PackBuf[i]>0x39))
        {
            return(FALSE);
        }
    }
    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmReadOneItem
功能说明：分解处理GPRMC数据，并存贮到相关数据结构中
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PrmReadOneItem(u16 PrmID, u16 dlen, u8 *FillBuf )
{
    sjfun_param_read(PrmID,dlen,FillBuf);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmWriteOneItem
功能说明：写入一个参数到系统参数区，并触发NV操作
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PrmWriteOneItem(u16 PrmID, u16 dlen, u8 *dbuf )
{
    sjfun_param_write(PrmID,dlen,dbuf);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PamFormatRunParam
功能说明：客户发送恢愎出厂设置指令
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PamFormatRunParam(void)
{
    char StrDat[40];
    u8	fbuf[40];
    u16 i,len;

    //fbuf[0]=FJYD_TERI_TYPE_EBIKE;				//终端类型
    //YS_PrmWriteOneItem(FLH_PRM_TERI_TYPE,FLH_PRM_TERI_TYPE_LEN,fbuf);

    fbuf[0]=FJYD_TERI_STATUS_INIT;			//初始化系统为未激活状态
    YS_PrmWriteOneItem(FLH_PRM_ACTIVE_FLAG,FLH_PRM_ACTIVE_FLAG_LEN,fbuf);

    sprintf(StrDat,"cmnet");					//APN信息
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,fbuf);

    /*fbuf[0]=0x01;								//越界报警500米
    fbuf[1]=0xF4;
    YS_PrmWriteOneItem(FLH_PRM_MOVE_WARN,FLH_PRM_MOVE_WARN_LEN,fbuf);
    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_OUT_SMS,FLH_PRM_OUT_SMS_LEN,fbuf);		//OUT
    YS_PrmWriteOneItem(FLH_PRM_OUT_GPRS,FLH_PRM_OUT_GPRS_LEN,fbuf);	//OUTS*/

    fbuf[0]=1;								//位置上报使能
    YS_PrmWriteOneItem(FLH_PRM_TRACE_ENABLE,FLH_PRM_TRACE_ENABLE_LEN,fbuf);

    //fbuf[0]=0x40;	//振动传感器，基数值
    //YS_PrmWriteOneItem(FLH_PRM_VIB_BASEVALUE,FLH_PRM_VIB_BASEVALUE_LEN,fbuf);

    fbuf[0]=0x00;								//定位包周期15秒
    fbuf[1]=0x1e;
    YS_PrmWriteOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);

    fbuf[0]=0x00;								//心跳包周期两分钟
    fbuf[1]=0x78;
    YS_PrmWriteOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_VIBWARN_SMS,FLH_PRM_VIBWARN_SMS_LEN,fbuf); //VIB
//	fbuf[0]=1;
//	YS_PrmWriteOneItem(FLH_PRM_VIBWARN_GPRS,FLH_PRM_VIBWARN_GPRS_LEN,fbuf); //VIBS
    fbuf[0]=4;
    YS_PrmWriteOneItem(FLH_PRM_VIB_LEVEL,FLH_PRM_VIB_LEVEL_LEN,fbuf); ////VIBL
    fbuf[0]=10;  //10:4
    fbuf[1]=4;
    YS_PrmWriteOneItem(FLH_PRM_VIB_CHK,FLH_PRM_VIB_CHK_LEN,fbuf); ////VIBL

    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_SMS,FLH_PRM_POWOFFWARN_SMS_LEN,fbuf); //POF
    YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_GPRS,FLH_PRM_POWOFFWARN_GPRS_LEN,fbuf); //POFS
    /*fbuf[0]=0x00; //POFT 120
    fbuf[1]=0x78;
    YS_PrmWriteOneItem(FLH_PRM_POWOFF_TIME,FLH_PRM_POWOFF_TIME_LEN,fbuf); //POFT*/

    /*fbuf[0]=0x00; //POFSLEEPT 120
    fbuf[1]=0x78;
    YS_PrmWriteOneItem(FLH_PRM_POWOFFSLEEP_TIME,FLH_PRM_POWOFFSLEEP_TIME_LEN,fbuf); //POFSLEEPT*/

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_ACC_LOCK,FLH_PRM_ACC_LOCK_LEN,fbuf); //ACCLOCK
    /*fbuf[0]=0;	//ACCLT	180	second
    fbuf[1]=0xB4;
    YS_PrmWriteOneItem(FLH_PRM_ACC_TIME,FLH_PRM_ACC_TIME_LEN,fbuf); //ACCLT*/

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf); //SLEEP ENABLE
    fbuf[0]=0x00;
    fbuf[1]=0x3c;
    YS_PrmWriteOneItem(FLH_PRM_SLEEP_TIME,FLH_PRM_SLEEP_TIME_LEN,fbuf);	//SLEEP TIME  180 second

    fbuf[0]=0x0E;
    fbuf[1]=0x74;
    YS_PrmWriteOneItem(FLH_PRM_BATT_LOW_LIMIT,FLH_PRM_BATT_LOW_LIMIT_LEN,fbuf);	//BATT LOW LIMIT


    //fbuf[0]=1;//继电器默认合上
    //YS_PrmWriteOneItem(FLH_PRM_RELAY_STOR,FLH_PRM_RELAY_STOR_LEN,fbuf);

    memset(fbuf,0,40);
    YS_PrmWriteOneItem(FLH_PRM_AREA_STOR,FLH_PRM_AREA_STOR_LEN,fbuf);	//AREA 报警数据
    YS_PrmWriteOneItem(FLH_PRM_RST_RECORD,FLH_PRM_RST_RECORD_LEN,fbuf);//复位记录数据
    YS_PrmWriteOneItem(FLH_PRM_DIS_VALUE,FLH_PRM_DIS_VALUE_LEN,fbuf);//里程数据

    fbuf[0]=AGPS_FUNC_UBLOX;
    YS_PrmWriteOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);	//AGPS 功能

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_NV_GPS_PYYH,FLH_PRM_NV_GPS_PYYH_LEN,fbuf);	//开GPS优化

    fbuf[0]=0;  //调试开关
    YS_PrmWriteOneItem(FLH_PRM_DEBUG_ENABLE,FLH_PRM_DEBUG_ENABLE_LEN,fbuf);

    fbuf[0]=0x00; //拐点补传控制
    YS_PrmWriteOneItem(FLH_PRM_ANGLE_SEND_FLAG,FLH_PRM_ANGLE_SEND_FLAG_LEN,fbuf);

    fbuf[0]=1;              //初始化低电量报警开关
    YS_PrmWriteOneItem(FLH_PRM_NV_POWLOW_FLAG,FLH_PRM_NV_POWLOW_FLAG_LEN,fbuf);

    /*fbuf[0]=0x02;			//初始化状态上报时间
    fbuf[1]=0x58;
    YS_PrmWriteOneItem(FLH_PRM_NV_REPORT_TER_STA_TIME,FLH_PRM_NV_REPORT_TER_STA_TIME_LEN,fbuf);*/



    //fbuf[0]=0x01;
    //YS_PrmWriteOneItem(FLH_PRM_NV_ANYNUM_FLAG, FLH_PRM_NV_ANYNUM_FLAG_LEN,fbuf);

    fbuf[0]=0x00;
    YS_PrmWriteOneItem(FLH_PRM_FLI_ENABLE, FLH_PRM_FLI_ENABLE_LEN,fbuf);

    /*fbuf[0]=0x00;        //振动检测时间默认50秒
    fbuf[1]=0x32;
    YS_PrmWriteOneItem(FLH_PRM_RF_CHECK_TEME,FLH_PRM_RF_CHECK_TEME_LEN,fbuf);*/


    fbuf[0]=0x00; //复位次数
    fbuf[1]=0x00;
    YS_PrmWriteOneItem(FLH_PRM_RST_NUM, FLH_PRM_RST_NUM_LEN, fbuf);

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_GPS_DEBUG_EN, FLH_PRM_GPS_DEBUG_EN_LEN, fbuf);

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_OBD_DEBUG_EN, FLH_PRM_OBD_DEBUG_EN_LEN, fbuf);

}

void YS_PrmNewNVTest(u8 *dbuf, u16 dlen)
{
#if 0
    u16 i;
    for(i=0; i<dlen; i++)
    {
        YS_Param_Buf[i]=dbuf[i];
    }
    t_ParamManage.WriteFlag=1;
    t_ParamManage.WriteDelay=0;
#endif
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmQuickWriteNV
功能说明：NV的快速写入函数，可在系统复位2秒调用
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PrmQuickWriteNV(void)
{
    sjfun_param_Quickwr();
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmJudgeParamNeedUpdate
功能说明：验证当前软件版本是否需要更新默认工作参数
                           如果需要更新，设置新版本标志，并返回TRUE
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_PrmJudgeParamNeedUpdate(void)
{
    u16 i,len1,len2;
    u8 fbuf[40];
    //bool CompRlt;

    len1=strlen(EBIKE_SOFT_VER);
    YS_PrmReadOneItem(FLH_PRM_SOFT_VER_INFO,FLH_PRM_SOFT_VER_INFO_LEN,fbuf); //检测是否初始化过的标志
    len2=YS_CodeBufRealLen(fbuf,FLH_PRM_SOFT_VER_INFO_LEN);

    if(len2==0)
    {
        for(i=0; i<len1; i++)
        {
            fbuf[i]=EBIKE_SOFT_VER[i];
        }
        fbuf[len1]=0;
        YS_PrmWriteOneItem(FLH_PRM_SOFT_VER_INFO,FLH_PRM_SOFT_VER_INFO_LEN,fbuf);
        return(TRUE);
    }
    else
    {
        for(i=0; i<len1; i++)
        {
            fbuf[i]=EBIKE_SOFT_VER[i];
        }
        fbuf[len1]=0;
        YS_PrmWriteOneItem(FLH_PRM_SOFT_VER_INFO,FLH_PRM_SOFT_VER_INFO_LEN,fbuf);
        return(FALSE);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmGetTotalSoftVer
功能说明：获取当前系统的全版本号
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PrmGetTotalSoftVer(u8 *fbuf)
{
    u16 i,len,pos;
    char StrDat[40];

    pos = 0;
    YS_GetModuleVer(StrDat);
    len = strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[pos] = StrDat[i];
        pos++;
    }
    fbuf[pos]='_';
    pos++;
    len=strlen(EBIKE_SOFT_VER);
    for(i=0; i<len; i++)
    {
        fbuf[pos] = EBIKE_SOFT_VER[i];
        pos++;
    }
    fbuf[pos]=0;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmSetFactroyWorkParam
功能说明：下载程序后的初始化工作参数
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PrmSetFactroyWorkParam(void)
{
    char StrDat[40];
    u8	fbuf[50];
    u16 i,len;

    if(YS_PrmJudgeParamNeedUpdate()==FALSE)
    {
        return;
    }

    fbuf[0]=0;  //禁止调试功能
    YS_PrmWriteOneItem(FLH_PRM_DEBUG_ENABLE,FLH_PRM_DEBUG_ENABLE_LEN,fbuf);

    sprintf(StrDat,"356823030000000");		//终端IMEI
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_TERI_IMEI,FLH_PRM_TERI_IMEI_LEN,fbuf);

//    memset(fbuf,0,FLH_PRM_SIM_CODE_LEN);
    sprintf(StrDat,"1353789600");		//终端IMEI
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf); 			//初始化SIM卡号为空
    YS_PrmWriteOneItem(FLH_PRM_OWNER_CODE,FLH_PRM_OWNER_CODE_LEN,fbuf);		//初始化车主号码为空

    sprintf(StrDat,"123456");					//测试密码
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,fbuf);

//    sprintf(StrDat,"dev.comobot.com");		//服务器IP
    sprintf(StrDat,"219.234.95.56");		//服务器IP

    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
    fbuf[0]=0x17;								//服务器端口6000
    fbuf[1]=0x70;
    YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);

    sprintf(StrDat,"cmnet");					//APN信息
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,fbuf);

    /*fbuf[0]=0x01;								//越界报警500米
    fbuf[1]=0xF4;
    YS_PrmWriteOneItem(FLH_PRM_MOVE_WARN,FLH_PRM_MOVE_WARN_LEN,fbuf);
    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_OUT_SMS,FLH_PRM_OUT_SMS_LEN,fbuf);		//OUT
    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_OUT_GPRS,FLH_PRM_OUT_GPRS_LEN,fbuf);	//OUTS*/

    fbuf[0]=1;								//位置上报使能
    YS_PrmWriteOneItem(FLH_PRM_TRACE_ENABLE,FLH_PRM_TRACE_ENABLE_LEN,fbuf);

    fbuf[0]=0x00;								//定位包周期30秒
    fbuf[1]=0x1E;
    YS_PrmWriteOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);

    fbuf[0]=0x00;								//心跳包周期两分钟
    fbuf[1]=0x78;
    YS_PrmWriteOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_VIBWARN_SMS,FLH_PRM_VIBWARN_SMS_LEN,fbuf); //VIB
//	fbuf[0]=1;
//	YS_PrmWriteOneItem(FLH_PRM_VIBWARN_GPRS,FLH_PRM_VIBWARN_GPRS_LEN,fbuf); //VIBS

    fbuf[0]=4;
    YS_PrmWriteOneItem(FLH_PRM_VIB_LEVEL,FLH_PRM_VIB_LEVEL_LEN,fbuf); ////VIBL
    fbuf[0]=10;  //10:4
    fbuf[1]=4;
    YS_PrmWriteOneItem(FLH_PRM_VIB_CHK,FLH_PRM_VIB_CHK_LEN,fbuf); ////VIBCHK

    //fbuf[0]=5;  //5秒
    //YS_PrmWriteOneItem(FLH_PRM_VIBWARN_DELAY,FLH_PRM_VIBWARN_DELAY_LEN,fbuf); ////VIBT

    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_SMS,FLH_PRM_POWOFFWARN_SMS_LEN,fbuf); //POF
    YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_GPRS,FLH_PRM_POWOFFWARN_GPRS_LEN,fbuf); //POFS

    /*fbuf[0]=0x00; //POFT 120
    fbuf[1]=0x78;
    YS_PrmWriteOneItem(FLH_PRM_POWOFF_TIME,FLH_PRM_POWOFF_TIME_LEN,fbuf); //POFT*/

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_ACC_LOCK,FLH_PRM_ACC_LOCK_LEN,fbuf); //ACCLOCK
    /*fbuf[0]=0x00;	//ACCLT	180秒
    fbuf[1]=0xB4;
    YS_PrmWriteOneItem(FLH_PRM_ACC_TIME,FLH_PRM_ACC_TIME_LEN,fbuf); //ACCLT*/

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf); //SLEEP ENABLE
    fbuf[0]=0x00;
    fbuf[1]=0x3c;
    YS_PrmWriteOneItem(FLH_PRM_SLEEP_TIME,FLH_PRM_SLEEP_TIME_LEN,fbuf);	//SLEEP TIME

    fbuf[0]=0x0E;
    fbuf[1]=0x74;
    YS_PrmWriteOneItem(FLH_PRM_BATT_LOW_LIMIT,FLH_PRM_BATT_LOW_LIMIT_LEN,fbuf);	//BATT LOW LIMIT




    //fbuf[0]=0; //默认禁止拔打定位功能
    //YS_PrmWriteOneItem(FLH_PRM_CALL_FUNC,FLH_PRM_CALL_FUNC_LEN,fbuf);	//CALL FUNC

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_CALL_DISP,FLH_PRM_CALL_DISP_LEN,fbuf);	//CALL DISP

    //fbuf[0]=0;
    //YS_PrmWriteOneItem(FLH_PRM_WAKEUP_EN,FLH_PRM_WAKEUP_EN_LEN,fbuf);	//WAKEUP ENABLE
    //fbuf[0]=0x54;	//6小时
    //fbuf[1]=0x60;
    //YS_PrmWriteOneItem(FLH_PRM_WAKEUP_TIME,FLH_PRM_WAKEUP_TIME_LEN,fbuf);	//WAKEUP TIME



    memset(fbuf,0,50);
    //YS_PrmWriteOneItem(FLH_PRM_AREA_STOR,FLH_PRM_AREA_STOR_LEN,fbuf);	//AREA 报警数据
    YS_PrmWriteOneItem(FLH_PRM_RST_RECORD,FLH_PRM_RST_RECORD_LEN,fbuf);//复位记录数据


    fbuf[0]=0x40;	//振动传感器，基数值
    YS_PrmWriteOneItem(FLH_PRM_VIB_BASEVALUE,FLH_PRM_VIB_BASEVALUE_LEN,fbuf);

    //fbuf[0]=1;	//默认车辆为停车状态
    //YS_PrmWriteOneItem(FLH_PRM_CAR_RUNSTATUS,FLH_PRM_CAR_RUNSTATUS_LEN,fbuf);

    //fbuf[0]=0x10;	//深度休眠时间设定
    //fbuf[1]=0xE0;
    //YS_PrmWriteOneItem(FLH_PRM_FLIT_SET,FLH_PRM_FLIT_SET_LEN,fbuf);

    //fbuf[0]=0x00;	//深度休眠时间计数
    //fbuf[1]=0x00;
    //YS_PrmWriteOneItem(FLH_PRM_FLIT_COUNT,FLH_PRM_FLIT_COUNT_LEN,fbuf);



    fbuf[0]=0x00; //拐点补传控制
    YS_PrmWriteOneItem(FLH_PRM_ANGLE_SEND_FLAG,FLH_PRM_ANGLE_SEND_FLAG_LEN,fbuf);

    //fbuf[0]=10; //报警次数限制
    //YS_PrmWriteOneItem(FLH_PRM_WARN_TIMES_MAX,FLH_PRM_WARN_TIMES_MAX_LEN,fbuf);

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_NV_GPS_PYYH,FLH_PRM_NV_GPS_PYYH_LEN,fbuf);	//开GPS优化

    fbuf[0]=113;	//初始化默认的经纬度信息
    fbuf[1]=52;
    fbuf[2]=79;
    fbuf[3]=66;
    fbuf[4]=22;
    fbuf[5]=34;
    fbuf[6]=27;
    fbuf[7]=05;
    YS_PrmWriteOneItem(FLH_PRM_LAST_GPSPOINT,FLH_PRM_LAST_GPSPOINT_LEN,fbuf);

    /*fbuf[0]=0x00;        //振动检测时间默认50秒
    fbuf[1]=0x32;
    YS_PrmWriteOneItem(FLH_PRM_RF_CHECK_TEME,FLH_PRM_RF_CHECK_TEME_LEN,fbuf);*/



    //fbuf[0]=1;				//RF关闭使能,默认允许关闭
    //YS_PrmWriteOneItem(FLH_PRM_RF_CLOSE_ENABLE,FLH_PRM_RF_CLOSE_ENABLE_LEN,fbuf);

    fbuf[0]=2;  //2002-01-01 12:30:0
    fbuf[1]=1;
    fbuf[2]=1;
    fbuf[3]=12;
    fbuf[4]=30;
    fbuf[5]=0;
    YS_PrmWriteOneItem(FLH_PRM_NV_LAST_POINT_DT,FLH_PRM_NV_LAST_POINT_DT_LEN,fbuf);

    //fbuf[0]=0;
    //YS_PrmWriteOneItem(FLH_PRM_NV_LED_CTRL,FLH_PRM_NV_LED_CTRL_LEN,fbuf);	//LED指示灯控制

    //fbuf[0]=0;
    //	YS_PrmWriteOneItem(FLH_PRM_NV_GPRS_CTRL,FLH_PRM_NV_GPRS_CTRL_LEN,fbuf);	//GPRS 连接控制

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_RF_CHECK_ENABLE,FLH_PRM_RF_CHECK_ENABLE_LEN,fbuf);

    //fbuf[0]=0x01;
    //YS_PrmWriteOneItem(FLH_PRM_NV_ANYNUM_FLAG, FLH_PRM_NV_ANYNUM_FLAG_LEN,fbuf);

    fbuf[0]=AGPS_FUNC_UBLOX;
    YS_PrmWriteOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);	//AGPS 功能

    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_NV_POWLOW_FLAG,FLH_PRM_NV_POWLOW_FLAG_LEN,fbuf);



    //fbuf[0]=0x00;
    //fbuf[1]=0xB4;
    //YS_PrmWriteOneItem(FLH_PRM_SF_TIME, FLH_PRM_SF_TIME_LEN, fbuf);

    //fbuf[0]=0x00;
    //fbuf[1]=0x78;
    //YS_PrmWriteOneItem(FLH_PRM_TC_TIME,FLH_PRM_TC_TIME_LEN,fbuf);

//	fbuf[0]=0;					//默认安防状态
//	YS_PrmWriteOneItem(FLH_PRM_AF_OR_JK_FLAG, FLH_PRM_AF_OR_JK_FLAG_LEN, fbuf);

    //fbuf[0]=1;			//默认允许跳频
    //YS_PrmWriteOneItem(FLH_PRM_RF_AUTO_FREQ_EN, FLH_PRM_RF_AUTO_FREQ_EN_LEN,fbuf);

    /*memset(fbuf,0,40);
    YS_PrmWriteOneItem(FLH_PRM_NV_KEYID1,FLH_PRM_NV_KEYID1_LEN,fbuf);
    YS_PrmWriteOneItem(FLH_PRM_NV_KEYID2,FLH_PRM_NV_KEYID2_LEN,fbuf);
    YS_PrmWriteOneItem(FLH_PRM_NV_KEYNUM,FLH_PRM_NV_KEYNUM_LEN,fbuf);	*/

    fbuf[0]=0; //远程升级标志
    YS_PrmWriteOneItem(FLH_PRM_NV_DWFLAG,FLH_PRM_NV_DWFLAG_LEN,fbuf);

    fbuf[0]=0x23; //远程升级端口9160
    fbuf[1]=0xC8;
    YS_PrmWriteOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);

    sprintf(StrDat,"117.27.136.205");			//远程升级服务器IP
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);

    fbuf[0]=1; //自动升级使能
    YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_ENABLE,FLH_PRM_NV_AUTOPLAT_ENABLE_LEN,fbuf);

    fbuf[0]=0x23; //远程升级端口9162
    fbuf[1]=0xCA;
    YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_PORT,FLH_PRM_NV_AUTOPLAT_PORT_LEN,fbuf);

    sprintf(StrDat,"117.27.136.205");			//自动升级服务器IP
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_ADDR,FLH_PRM_NV_AUTOPLAT_ADDR_LEN,fbuf);

    fbuf[0]=0; //自动升级连接标志
    YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_FLAG,FLH_PRM_NV_AUTOPLAT_FLAG_LEN,fbuf);

    fbuf[0]=0x00;
    YS_PrmWriteOneItem(FLH_PRM_FLI_ENABLE, FLH_PRM_FLI_ENABLE_LEN,fbuf);

    fbuf[0]=0x00;	//默认为正常工作版本   0为正常 1为调天线
    YS_PrmWriteOneItem(FLH_PRM_SKYLINE_FLAG, FLH_PRM_SKYLINE_FLAG_LEN,fbuf);

    fbuf[0]=0x00; //复位次数
    fbuf[1]=0x00;
    YS_PrmWriteOneItem(FLH_PRM_RST_NUM, FLH_PRM_RST_NUM_LEN, fbuf);

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_GPS_DEBUG_EN, FLH_PRM_GPS_DEBUG_EN_LEN, fbuf);

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_OBD_DEBUG_EN, FLH_PRM_OBD_DEBUG_EN_LEN, fbuf);

    fbuf[0]=1;								//CAN上报使能
    YS_PrmWriteOneItem(FLH_PRM_CAN_ENABLE,FLH_PRM_CAN_ENABLE_LEN,fbuf);

    fbuf[0]=0x00;								//CAN包周期30秒
    fbuf[1]=0x0a;
    YS_PrmWriteOneItem(FLH_PRM_CAN_TIME,FLH_PRM_CAN_TIME_LEN,fbuf);

    fbuf[0] = 0xbe;
    fbuf[1] = 0xa9;
    fbuf[2] = 0x46;
    fbuf[3] = 0x31;
    fbuf[4] = 0x32;
    fbuf[5] = 0x33;
    fbuf[6] = 0x34;
    fbuf[7] = 0x35;
    fbuf[8] = 0;
    YS_PrmWriteOneItem(FLH_JTB_PLATE_STRING,FLH_JTB_PLATE_STRING_LEN,fbuf);

}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmFindStringInbuf
功能说明：查找缓冲区特殊标识的字符串
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_PrmFindStringInbuf(u8 *dbuf, u16 dlen, char *IndexStr, char *BackStr)
{
    u16 i,j,pos,len,count;
    bool result;
    u8	tdat;

    len=strlen(IndexStr);
    result=FALSE;
    pos=0;
    for(i=0; i<dlen; i++)
    {
        result=TRUE;
        for(j=0; j<len; j++)
        {
            tdat=toupper(dbuf[i+j]);  //不检测字母的大小写
            if(tdat!=IndexStr[j])
            {
                result=FALSE;
                break;
            }
        }

        if(result==TRUE)
        {
            pos=i+len;
            break;
        }
    }

    if(result==TRUE)
    {
        count=0;
        for(i=pos; i<dlen; i++)
        {
            if(dbuf[i]==']')
            {
                break;
            }
            BackStr[i-pos]=dbuf[i];
            count++;
        }
        BackStr[count]=0;
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmPDNumString
功能说明：判断输入的字符串是否为有效的数字节符串
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_PrmPDNumString(char *InputStr, bool EnLenLimit, u16 AskLen, u32 MinValue, u32 MaxValue)
{
    u16 Len;
    u32 RealValue;

    if(YS_CodePDNumString(InputStr)==FALSE)
    {
        return(FALSE);
    }

    Len=strlen(InputStr);

    if(EnLenLimit==TRUE)
    {
        if(AskLen!=Len)
        {
            return(FALSE);
        }
    }

    RealValue=atoi(InputStr);
    if(RealValue<MinValue)
    {
        return(FALSE);
    }

    if(RealValue>MaxValue)
    {
        return(FALSE);
    }

    return(TRUE);
}


/*-----------------------------------------------------------------------------------------
函数名：YS_PrmCompTwoString
功能说明：忽略大小写方式比较两个字符串
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_PrmCompTwoString(char *Str1, char *Str2)
{
    u16 i,len1,len2;
    bool result;
    u8 dat1,dat2;

    len1=strlen(Str1);
    len2=strlen(Str2);
    if(len1!=len2)
    {
        return(FALSE);
    }
    result=TRUE;
    for(i=0; i<len1; i++)
    {
        dat1=toupper(Str1[i]);
        dat2=toupper(Str2[i]);
        if(dat1!=dat2)
        {
            result=FALSE;
            break;
        }
    }
    return(result);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmCreateGpsInfo
功能说明：生在GPS 状态数据
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PrmCreateIMEI(char *Imeistr)
{
    u8 fbuf[30];
    u16 i,len;

    YS_PrmReadOneItem(FLH_PRM_TERI_IMEI,FLH_PRM_TERI_IMEI_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_TERI_IMEI_LEN);
    for(i=0; i<len; i++)
    {
        Imeistr[i]=fbuf[i];
    }
    Imeistr[len]=0;
}



/*-----------------------------------------------------------------------------------------
函数名：YS_PrmCreateGpsInfo
功能说明：生在GPS 状态数据
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PrmCreateGpsInfo(char *GpsInfo)
{
    u16 i,j,loop1,loop2,len;
    u8 dat1,dat2;
    u16 pos;
    char StrDat[20];

    t_Gps_Star_Info t_StarInfo;
    YS_GpsGetStarInfo(&t_StarInfo);
    if(t_StarInfo.StarNum>2)
    {
        loop1=t_StarInfo.StarNum;
        loop2=t_StarInfo.StarNum-1;
        for(i=0; i<loop1; i++)
        {
            for(j=0; j<loop2; j++)
            {
                if(t_StarInfo.StarSign[j+1]>t_StarInfo.StarSign[j])
                {
                    dat1=t_StarInfo.StarSign[j];
                    dat2=t_StarInfo.StarCode[j];
                    t_StarInfo.StarSign[j]=t_StarInfo.StarSign[j+1];
                    t_StarInfo.StarCode[j]=t_StarInfo.StarCode[j+1];
                    t_StarInfo.StarSign[j+1]=dat1;
                    t_StarInfo.StarCode[j+1]=dat2;
                }
            }
        }
    }

    pos=0;
    for(i=0; i<12; i++)
    {
        sprintf(StrDat,"%02d%02d ",t_StarInfo.StarCode[i],t_StarInfo.StarSign[i]);
        len=strlen(StrDat);
        for(j=0; j<len; j++)
        {
            GpsInfo[pos]=StrDat[j];
            pos++;
        }
    }
    pos--;
    GpsInfo[pos]=0;
}


/*
	RdBuf[0]=node->Hour;
	RdBuf[1]=node->Min;
	RdBuf[2]=node->State;
	RdBuf[3]=node->Freq;
	RdBuf[4]=node->Days;
	RdBuf[5]=node->Snooze;*/
/*-----------------------------------------------------------------------------------------
函数名：YS_PrmGetSysAlarmClock
功能说明：读取系统时钟
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_PrmGetSysAlarmClock(char *PackBuf)
{
    char StrDat[20];
    u8	ClockBuf[40];
    u8	IDBuf[8];
    u16  i,k,len,pos;

    for(i=0; i<4; i++)
    {
        IDBuf[i]=i;
    }

    sjfun_Read_Alm_Warn(IDBuf,4,ClockBuf);
    pos=0;
    for(i=0; i<4; i++)
    {
        sprintf(StrDat,"%d:%d:%d:%d|",i,ClockBuf[i*6+2],ClockBuf[i*6],ClockBuf[i*6+1]);
        len=strlen(StrDat);
        YS_GprsDebugString(StrDat,0);
        for(k=0; k<len; k++)
        {
            PackBuf[pos]=StrDat[k];
            pos++;
        }
    }
    pos--;
    return(pos);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WorkPrmSetDeal
功能说明：根据输入的字符串，进行参数写入操作的处理
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_WorkPrmReadDeal(u8 *dbuf, u16 dlen, u8 *PackBuf, u16 *PackLen)
{
    char	StrDat[100];
    u8 PowStatue;
    u16 i,len,Value,dat,pos;
    u8	fbuf[50];
    //u32 Time;
    //u8 	Chnanel;
    dbuf[dlen]=0;

    pos=0;
    if(YS_PrmCompTwoString(HBYD_PARAM_READ_SOFTVER,(char *)dbuf)) //读取软件版本号
    {
        len=strlen(HBYD_PARAM_READ_SOFTVER);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_SOFTVER[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;

        memset(fbuf,0x00,50);
        YS_PrmGetTotalSoftVer(fbuf);
        len = YS_CodeBufRealLen(fbuf,50);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_GSM,(char *)dbuf)) //读取GSM 信号强度
    {
        len=strlen(HBYD_PARAM_READ_GSM);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_GSM[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        Value=YS_RunGetCsqValue();
        Value/=3;
        dat=80+Value;
        if(dat>90)
        {
            dat=100;
        }
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_GPS,(char *)dbuf)) //获取GPS 运行数据
    {
        len=strlen(HBYD_PARAM_READ_GPS);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_GPS[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        YS_PrmCreateGpsInfo(StrDat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VBAT,(char *)dbuf)) //读取当前工作电压
    {
        len=strlen(HBYD_PARAM_READ_VBAT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_VBAT[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        dat=YS_RunGetBattValue();
        sprintf(StrDat,"%04d000:5000000:000000",dat);	//第二项为外部电源值
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_LOGIN,(char *)dbuf)) //读取登录状态???
    {
        len=strlen(HBYD_PARAM_READ_LOGIN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_LOGIN[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        PackBuf[pos]='1';
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_IMSI,(char *)dbuf)) //读取SIM 卡IMSI
    {
        len=strlen(HBYD_PARAM_READ_IMSI);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_IMSI[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        YS_RunGetIMSI(StrDat); 	//IMSI 号
        for(i=0; i<15; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_IMEI,(char *)dbuf)) //读取设备IMEI
    {
        len=strlen(HBYD_PARAM_READ_IMEI);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_IMEI[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        YS_PrmReadOneItem(FLH_PRM_TERI_IMEI,FLH_PRM_TERI_IMEI_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_TERI_IMEI_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_ACC,(char *)dbuf)) //读取ACC 当前状态
    {
        len=strlen(HBYD_PARAM_READ_ACC);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_ACC[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        if(YS_RunGetACCStatus()==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_UPDELAY,(char *)dbuf)) //读取数据上报频率
    {
        len=strlen(HBYD_PARAM_NAME_UPDELAY);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_UPDELAY[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_CIRCLEAREA,(char *)dbuf)) //读取越界报警参数
    {
    	len=strlen(HBYD_PARAM_NAME_CIRCLEAREA);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_CIRCLEAREA[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_MOVE_WARN,FLH_PRM_MOVE_WARN_LEN,fbuf);
    	dat=fbuf[0]*256+fbuf[1];
    	sprintf(StrDat,"%d",dat);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_DOMAIN,(char *)dbuf)) //读取服务器参数
    {
        len=strlen(HBYD_PARAM_READ_DOMAIN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_DOMAIN[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
        PackBuf[pos]=':';
        pos++;
        YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_PWM,(char *)dbuf)) //读取操作密码
    {
        len=strlen(HBYD_PARAM_NAME_PWM);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_PWM[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_PASSWORD_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_TRACE,(char *)dbuf))
    {
        len=strlen(HBYD_PARAM_NAME_TRACE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_TRACE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_TRACE_ENABLE,FLH_PRM_TRACE_ENABLE_LEN,fbuf);
        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_PULSE,(char *)dbuf)) //读取心跳包周期
    {
        len=strlen(HBYD_PARAM_NAME_PULSE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_PULSE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_PHONE,(char *)dbuf)) //读取SIM 卡号
    {
        len=strlen(HBYD_PARAM_NAME_PHONE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_PHONE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_SIM_CODE_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_USER,(char *)dbuf)) //读取用户号码
    {
        len=strlen(HBYD_PARAM_NAME_USER);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_USER[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_OWNER_CODE,FLH_PRM_OWNER_CODE_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_OWNER_CODE_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VIB,(char *)dbuf)) //振动报警短信使能
    {
    	len=strlen(HBYD_PARAM_NAME_VIB);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_VIB[i];
    		pos++;
    	}
    	//YS_PrmReadOneItem(FLH_PRM_VIBWARN_SMS,FLH_PRM_VIBWARN_SMS_LEN,fbuf);
    	if(fbuf[0]==1)
    	{
    		PackBuf[pos]='1';
    	}
    	else
    	{
    		PackBuf[pos]='0';
    	}
    	pos++;
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VIBS,(char *)dbuf)) //振动报警服务器使能
    {
    	len=strlen(HBYD_PARAM_NAME_VIBS);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_VIBS[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_VIBWARN_GPRS,FLH_PRM_VIBWARN_GPRS_LEN,fbuf);
    	if(fbuf[0]==1)
    	{
    		PackBuf[pos]='1';
    	}
    	else
    	{
    		PackBuf[pos]='0';
    	}
    	pos++;
    }*/
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VIBL,(char *)dbuf)) //读取振动级别
    {
        len=strlen(HBYD_PARAM_NAME_VIBL);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_VIBL[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_VIB_LEVEL,FLH_PRM_VIB_LEVEL_LEN,fbuf);
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_POF,(char *)dbuf)) //读取断电报警短信使能
    {
        len=strlen(HBYD_PARAM_NAME_POF);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_POF[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_POWOFFWARN_SMS,FLH_PRM_POWOFFWARN_SMS_LEN,fbuf);
        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_POFS,(char *)dbuf)) //读取断电报警服务器命使能
    {
        len=strlen(HBYD_PARAM_NAME_POFS);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_POFS[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_POWOFFWARN_GPRS,FLH_PRM_POWOFFWARN_GPRS_LEN,fbuf);
        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_LBV,(char *)dbuf)) //读取电池低电压报警门限
    {
        len=strlen(HBYD_PARAM_NAME_LBV);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_LBV[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_BATT_LOW_LIMIT,FLH_PRM_BATT_LOW_LIMIT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d000",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SLEEP,(char *)dbuf)) //读取休眠控制
    {
        len=strlen(HBYD_PARAM_NAME_SLEEP);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SLEEP[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf);
        Value=fbuf[0];
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }

    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_CALLLOCK,(char *)dbuf)) //读取来电功能
    {
    	len=strlen(HBYD_PARAM_NAME_CALLLOCK);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_CALLLOCK[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_CALL_FUNC,FLH_PRM_CALL_FUNC_LEN,fbuf);
    	Value=fbuf[0];
    	sprintf(StrDat,"%d",fbuf[0]);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_CALLDISP,(char *)dbuf)) //读取来显检测功能
    {
    	len=strlen(HBYD_PARAM_NAME_CALLDISP);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_CALLDISP[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_CALL_DISP,FLH_PRM_CALL_DISP_LEN,fbuf);
    	Value=fbuf[0];
    	sprintf(StrDat,"%d",fbuf[0]);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/


    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VIBCHK,(char *)dbuf))  //振动报警检测参数
    {
        len=strlen(HBYD_PARAM_NAME_VIBCHK);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_VIBCHK[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_VIB_CHK,FLH_PRM_VIB_CHK_LEN,fbuf);
        sprintf(StrDat,"%d:%d",fbuf[0],fbuf[1]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_POFT,(char *)dbuf)) //断电报警检测时间
    {
    	len=strlen(HBYD_PARAM_NAME_POFT);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_POFT[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_POWOFF_TIME,FLH_PRM_POWOFF_TIME_LEN,fbuf);
    	Value=fbuf[0]*256+fbuf[1];
    	sprintf(StrDat,"%d",Value);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SLEEPT,(char *)dbuf)) //休眠检测时间
    {
        len=strlen(HBYD_PARAM_NAME_SLEEPT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SLEEPT[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SLEEP_TIME,FLH_PRM_SLEEP_TIME_LEN,fbuf);
        Value=fbuf[0]*256+fbuf[1];
        Value/=60;	//转换为分钟
        sprintf(StrDat,"%d",Value);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_ACCLT,(char *)dbuf)) //ACC 检测时间
    {
    	len=strlen(HBYD_PARAM_NAME_ACCLT);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_ACCLT[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_ACC_TIME,FLH_PRM_ACC_TIME_LEN,fbuf);
    	Value=fbuf[0]*256+fbuf[1];
    	sprintf(StrDat,"%d",Value);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_ACCLOCK,(char *)dbuf)) //ACC 设防使能
    {
    	len=strlen(HBYD_PARAM_NAME_ACCLOCK);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_ACCLOCK[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_ACC_LOCK,FLH_PRM_ACC_LOCK_LEN,fbuf);
    	Value=fbuf[0];
    	sprintf(StrDat,"%d",fbuf[0]);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_OUT,(char *)dbuf)) //读取越界报警短信参数
    {
    	len=strlen(HBYD_PARAM_NAME_OUT);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_OUT[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_OUT_SMS,FLH_PRM_OUT_SMS_LEN,fbuf);

    	if(fbuf[0]==1)
    	{
    		PackBuf[pos]='1';
    	}
    	else
    	{
    		PackBuf[pos]='0';
    	}
    	pos++;
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_OUTS,(char *)dbuf)) //读取越界服务器使能
    {
    	len=strlen(HBYD_PARAM_NAME_OUTS);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_OUTS[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_OUT_GPRS,FLH_PRM_OUT_GPRS_LEN,fbuf);

    	if(fbuf[0]==1)
    	{
    		PackBuf[pos]='1';
    	}
    	else
    	{
    		PackBuf[pos]='0';
    	}
    	pos++;
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_WAKEUP,(char *)dbuf)) //读取自动唤醒使能
    {
    	len=strlen(HBYD_PARAM_NAME_WAKEUP);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_WAKEUP[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_WAKEUP_EN,FLH_PRM_WAKEUP_EN_LEN,fbuf);
    	dat=fbuf[0];
    	sprintf(StrDat,"%d",dat);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_WAKEUPT,(char *)dbuf)) //读取自动唤醒周期
    {
    	len=strlen(HBYD_PARAM_NAME_WAKEUPT);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_WAKEUPT[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_WAKEUP_TIME,FLH_PRM_WAKEUP_TIME_LEN,fbuf);
    	dat=fbuf[0]*256+fbuf[1];
    	dat/=60;	//转换为分钟
    	sprintf(StrDat,"%d",dat);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/

    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_APN,(char *)dbuf)) //读取APN 信息
    {
        len=strlen(HBYD_PARAM_NAME_APN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_APN[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_PPP_APN_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SERVIP,(char *)dbuf)) //读取服务器IP
    {
        len=strlen(HBYD_PARAM_NAME_SERVIP);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SERVIP[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SERVPORT,(char *)dbuf)) //读取服务器端口
    {
        len=strlen(HBYD_PARAM_NAME_SERVPORT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SERVPORT[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_EQUTYPE,(char *)dbuf)) //读取终端类型
    {
        len=strlen(HBYD_PARAM_NAME_EQUTYPE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_EQUTYPE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_TERI_TYPE,FLH_PRM_TERI_TYPE_LEN,fbuf);
        dat=fbuf[0];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_AGPS_EN,(char *)dbuf)) //AGPS 功能参数设置
    {
        len=strlen(HBYD_PARAM_NAME_AGPS_EN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_AGPS_EN[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_GPS_FINTER_EN,(char *)dbuf)) //是否有漂移优化
    {
        len=strlen(HBYD_PARAM_NAME_GPS_FINTER_EN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_GPS_FINTER_EN[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_GPS_PYYH,FLH_PRM_NV_GPS_PYYH_LEN,fbuf);
        dat=fbuf[0];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VIB_BASE,(char *)dbuf)) //读取振动报警基数值
    {
        len=strlen(HBYD_PARAM_NAME_VIB_BASE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_VIB_BASE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_VIB_BASEVALUE,FLH_PRM_VIB_BASEVALUE_LEN,fbuf);
        dat=fbuf[0];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_ANGLE_SEND,(char *)dbuf)) //拐点补传使能
    {
        len=strlen(HBYD_PARAM_NAME_ANGLE_SEND);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_ANGLE_SEND[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_ANGLE_SEND_FLAG,FLH_PRM_ANGLE_SEND_FLAG_LEN,fbuf);

        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }

    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_CHARGE_STATUS,(char *)dbuf)) //读取充电状态
    {
        len=strlen(HBYD_PARAM_READ_CHARGE_STATUS);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_CHARGE_STATUS[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        if(YS_GetChargeStatue()==1)
        {
            PackBuf[pos]='1';
        }
        else if(YS_GetChargeStatue()==0)
        {
            YS_RunGetStatusInfo(FJYD_STATUS_ID_POWER,&PowStatue);
            if(PowStatue==1)
            {
                PackBuf[pos]='2';
            }
            else
            {
                PackBuf[pos]='0';
            }
        }
        pos++;
        //YS_GprsDebugString(char * DebugStr, kal_uint32 Value)
    }
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_FACMODE,(char *)dbuf)) //读取工程模式状态
    {
    	len=strlen(HBYD_PARAM_NAME_FACMODE);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_FACMODE[i];
    		pos++;
    	}
    	if(YS_RunGetFactoryMode()==TRUE)
    	{
    		PackBuf[pos]='1';
    	}
    	else
    	{
    		PackBuf[pos]='0';
    	}
    	pos++;
    }*/
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_DW_ADDR,(char *)dbuf)) //读取服务器IP
    {
        len=strlen(HBYD_PARAM_NAME_DW_ADDR);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_DW_ADDR[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_NV_DWADDR_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SJIP,(char *)dbuf)) //读取服务器IP
    {
        len=strlen(HBYD_PARAM_NAME_SJIP);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SJIP[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_NV_DWADDR_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }

    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_DW_PORT,(char *)dbuf)) //读取服务器端口
    {
        len=strlen(HBYD_PARAM_NAME_DW_PORT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_DW_PORT[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_DLPORT,(char *)dbuf)) //读取服务器端口
    {
        len=strlen(HBYD_PARAM_NAME_DLPORT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_DLPORT[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_DW_FLAG,(char *)dbuf)) //GPRS 功能控制
    {
        len=strlen(HBYD_PARAM_NAME_DW_FLAG);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_DW_FLAG[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_DWFLAG,FLH_PRM_NV_DWFLAG_LEN,fbuf);
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_pARAM_READ_AUTO_ENABLE,(char *)dbuf)) //自动升级功能使能
    {
        len=strlen(HBYD_pARAM_NAME_AUTO_ENABLE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_pARAM_NAME_AUTO_ENABLE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_AUTOPLAT_ENABLE,FLH_PRM_NV_AUTOPLAT_ENABLE_LEN,fbuf);
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_pARAM_READ_AUTO_ADDR,(char *)dbuf)) //读取服务器IP
    {
        len=strlen(HBYD_pARAM_NAME_AUTO_ADDR);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_pARAM_NAME_AUTO_ADDR[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_AUTOPLAT_ADDR,FLH_PRM_NV_AUTOPLAT_ADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_NV_AUTOPLAT_ADDR_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_pARAM_READ_AUTO_PORT,(char *)dbuf)) //自动升级服务器端口读取
    {
        len=strlen(HBYD_pARAM_NAME_AUTO_PORT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_pARAM_NAME_AUTO_PORT[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_AUTOPLAT_PORT,FLH_PRM_NV_AUTOPLAT_PORT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_pARAM_READ_AUTODLPORT,(char *)dbuf)) //自动升级服务器端口读取
    {
        len=strlen(HBYD_pARAM_NAME_AUTODLPORT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_pARAM_NAME_AUTODLPORT[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_AUTOPLAT_PORT,FLH_PRM_NV_AUTOPLAT_PORT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_pARAM_READ_AUTO_FLAG,(char *)dbuf)) //自动升级连接标志
    {
        len=strlen(HBYD_pARAM_NAME_AUTO_FLAG);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_pARAM_NAME_AUTO_FLAG[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_AUTOPLAT_FLAG,FLH_PRM_NV_AUTOPLAT_FLAG_LEN,fbuf);
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_UPDATE_MODE,(char *)dbuf)) //读取工作模式
    {
        len=strlen(HBYD_PARAM_NAME_UPDATE_MODE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_UPDATE_MODE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_UPDATE_MODE,FLH_PRM_UPDATE_MODE_LEN,fbuf);
        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SYNC_DT,(char *)dbuf)) //读取系统时钟
    {
        len=strlen(HBYD_PARAM_NAME_SYNC_DT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SYNC_DT[i];
            pos++;
        }
        len=YS_SysGetSystemTimeToString((char *)&PackBuf[pos]);
        pos+=len;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_ALARM_CLOCK,(char *)dbuf)) //读取闹铃数据
    {
        len=strlen(HBYD_PARAM_NAME_ALARM_CLOCK);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_ALARM_CLOCK[i];
            pos++;
        }
        len=YS_PrmGetSysAlarmClock((char *)&PackBuf[pos]);
        pos+=len;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SKYLINE_FLAG,(char *)dbuf)) //读取是否为天线调试模式
    {
        len=strlen(HBYD_PARAM_NAME_SKYLINE_FLAG);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SKYLINE_FLAG[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SKYLINE_FLAG,FLH_PRM_SKYLINE_FLAG_LEN,fbuf);
        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else
    {
        return(FALSE);
    }
    *PackLen=pos;
    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmSetVibChk
功能说明：根据输入的字符串，设置振动报警参数
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PrmSetVibChk(char *DataString)
{
    u16 i,len,pos,count;
    char StrDat[10];
    u8 fbuf[10];

    pos=0;
    len=strlen(DataString);
    count=0;
    for(i=0; i<len; i++)
    {
        if(DataString[i]==':')
        {
            pos=i+1;
            break;
        }
        count++;
        StrDat[i]=DataString[i];
    }
    StrDat[count]=0;
    fbuf[0]=atoi(StrDat);
    count=0;
    for(i=pos; i<len; i++)
    {
        StrDat[i-pos]=DataString[i];
        count++;
    }
    StrDat[count]=0;
    fbuf[1]=atoi(StrDat);
    YS_PrmWriteOneItem(FLH_PRM_VIB_CHK,FLH_PRM_VIB_CHK_LEN,fbuf);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmSetServerParam
功能说明：根据输入的字符串，服务器SOCKET 参数
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_PrmSetServerParam(char *StrInfo)
{
    u16 i,pos,len,count,dat;
    u8 fbuf[50];
    char StrDat[10];
    bool FindRlt;

    len=strlen(StrInfo);
    FindRlt=FALSE;
    pos=0;
    for(i=0; i<len; i++)
    {
        if(StrInfo[i]==':')
        {
            pos=i;
            FindRlt=TRUE;
            break;
        }
    }

    if(FindRlt==FALSE)
    {
        return(FALSE);
    }

    for(i=0; i<pos; i++)
    {
        fbuf[i]=StrInfo[i];
    }
    fbuf[pos]=0;
    YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);

    pos++;
    count=0;
    for(i=pos; i<len; i++)
    {
        if(StrDat[i]==']')
        {
            break;
        }

        if(count<6)
        {
            StrDat[i-pos]=StrInfo[i];
            count++;
        }
    }
    StrDat[count]=0;
    dat=atoi(StrDat);
    fbuf[0]=dat/256;
    fbuf[1]=dat%256;
    YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
    YS_SysRsqSystemRST(YS_RST_FLAG_ACTIVE);//add zhf 0823
    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmCreateIMEIVerify
功能说明：生成终端IMEI 的校验码
修改记录：
-------------------------------------------------------------------------------------------*/
u8 YS_PrmCreateIMEIVerify(char *IMEIStr)
{
    u16 i,Value;
    u8 dat;

    Value=0;
    for(i=0; i<7; i++) //累加奇数位之和
    {
        Value+=(IMEIStr[i*2]-0x30);
    }

    for(i=0; i<7; i++) //进行偶数位处理
    {
        dat=IMEIStr[i*2+1]-0x30;
        dat*=2;
        Value+=(dat/10);
        Value+=(dat%10);
    }
    dat=Value%10;
    if(dat>0)
    {
        dat=10-dat;
    }
    dat+=0x30;
    return(dat);
}


/*-----------------------------------------------------------------------------------------
函数名：YS_PrmCreateIMEIInfo
功能说明：根据输入终端ID 号自动生成IMEI号
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PrmCreateIMEIInfo()
{

}

void YS_PrmSetOneAlarmByStr(char *ClockDat, u8 *PackBuf)
{
    char StrDat[10];
    u16 len;

    len=strlen(ClockDat);
    YS_CodeGetItemInBuf((u8 *)ClockDat,len,(u8 *)StrDat,0,':',4);	//索引
    PackBuf[0]=atoi(StrDat);
    YS_CodeGetItemInBuf((u8 *)ClockDat,len,(u8 *)StrDat,1,':',4);	//使能
    PackBuf[1]=atoi(StrDat);
    YS_CodeGetItemInBuf((u8 *)ClockDat,len,(u8 *)StrDat,2,':',4);	//时
    PackBuf[2]=atoi(StrDat);
    YS_CodeGetItemInBuf((u8 *)ClockDat,len,(u8 *)StrDat,3,':',4);	//分
    PackBuf[3]=atoi(StrDat);
}

void YS_PrmSetAlarmClockDebug(u8 Flow)
{
    char DispBuf[100];

    sprintf(DispBuf,"Set clock Flow:%d",Flow);
    YS_UartDataOutInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8 *)DispBuf,strlen(DispBuf));
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmSetAlarmClock
功能说明：设置系统闹铃
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_PrmSetAlarmClock(char *InputStr)
{
    u16 i,count,len,pos,ItemLen;
    char ClockStr[20];
    u8	ClockBuf[20];

    len=strlen(InputStr);
    count=0;
    for(i=0; i<len; i++)
    {
        if(InputStr[i]=='|')
        {
            count++;
        }
    }

    if(count>4)
    {
        YS_PrmSetAlarmClockDebug(1);
        return(FALSE);
    }

    pos=0;
    for(i=0; i<4; i++)
    {
        ItemLen=YS_CodeGetItemInBuf((u8 *)InputStr,len,(u8 *)ClockStr,i,'|',10);
        ClockStr[ItemLen]=0;
        YS_PrmSetOneAlarmByStr(ClockStr,&ClockBuf[pos]);
        pos+=4;
    }
    sjfun_Set_Alm_Warn(ClockBuf,pos);
    YS_PrmSetAlarmClockDebug(2);
    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WorkPrmSetDeal
功能说明：根据输入的字符串，进行参数写入操作的处理
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_WorkPrmSetDeal(u8 *dbuf, u16 dlen)
{
    char StrDat[50];
    u16 i,Value,len;
    u8	fbuf[50];
    Avl_ULong ldat;

    if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_UPDELAY,StrDat))  //设置上报周期
    {
        if(YS_PrmPDNumString(StrDat,FALSE,0,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_DOMAIN,StrDat)) //切换服务器地址
    {
        YS_PrmSetServerParam(StrDat);
        return(TRUE);
    }
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_CIRCLEAREA,StrDat)) //设置越界报警参数
     {
    	if(YS_PrmPDNumString(StrDat,FALSE,0,0,0xFFFF)==FALSE)
    	{
    		return(FALSE);
    	}
    	else
    	{
    		Value=atoi(StrDat);
    		fbuf[0]=Value/256;
    		fbuf[1]=Value%256;
    		YS_PrmWriteOneItem(FLH_PRM_MOVE_WARN,FLH_PRM_MOVE_WARN_LEN,fbuf);
    		return(TRUE);
    	}
     }*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_TRACE,StrDat)) //设置轨迹使能
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')
            {
                fbuf[0]=1;
            }
            else
            {
                fbuf[0]=0;
            }
            YS_PrmWriteOneItem(FLH_PRM_TRACE_ENABLE,FLH_PRM_TRACE_ENABLE_LEN,fbuf);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_PULSE,StrDat)) //设置心跳包
    {
        if(YS_PrmPDNumString(StrDat,FALSE,0,10,300)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_TERIID,StrDat)) //设置终端ID号
    {
        if(YS_SmsCompString((u8 *)StrDat,strlen(StrDat),11)==FALSE)
        {
            return(FALSE);
        }
        else
        {
//		    YS_CodeBCDCharConver(StrDat,,fbuf);
//			YS_PrmWriteOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf);
//			YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_PHONE,StrDat)) //设置SIM卡号
    {
        if(YS_SmsCompString((u8 *)StrDat,strlen(StrDat),11)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<11; i++)
            {
                fbuf[i]=StrDat[i];
            }
            fbuf[11]=0;
            YS_PrmWriteOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_USER,StrDat)) //设置车主号码
    {
        if(YS_SmsCompString((u8 *)StrDat,strlen(StrDat),11)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<11; i++)
            {
                fbuf[i]=StrDat[i];
            }
            fbuf[11]=0;
            YS_PrmWriteOneItem(FLH_PRM_OWNER_CODE,FLH_PRM_OWNER_CODE_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIB,StrDat)) //设置振动短信开关
    {
      if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
      {
    	return(FALSE);
      }
      else
      {
    	 if(StrDat[0]=='1')
    	 {
    		 fbuf[0]=1;
    	 }
    	 else
    	 {
    		 fbuf[0]=0;
    	 }
    	// YS_PrmWriteOneItem(FLH_PRM_VIBWARN_GPRS,FLH_PRM_VIBWARN_GPRS_LEN,fbuf); //修改为GPRS 控制
    	// YS_PrmWriteOneItem(FLH_PRM_VIBWARN_SMS,FLH_PRM_VIBWARN_SMS_LEN,fbuf);
    	 return(TRUE);
      }
    }*/
    /*  else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIBS,StrDat)) //设置振动服务器开关
      {
    	  if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	  {
    		return(FALSE);
    	  }
    	  else
    	  {
    		 if(StrDat[0]=='1')
    		 {
    			 fbuf[0]=1;
    		 }
    		 else
    		 {
    			 fbuf[0]=0;
    		 }
    		 YS_PrmWriteOneItem(FLH_PRM_VIBWARN_GPRS,FLH_PRM_VIBWARN_GPRS_LEN,fbuf);
    		 return(TRUE);
    	  }
      }	  */
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIBL,StrDat)) //设置振动灵敏度
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,15)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_VIB_LEVEL,FLH_PRM_VIB_LEVEL_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_POF,StrDat)) //设置断电短信开关
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')
            {
                fbuf[0]=1;
            }
            else
            {
                fbuf[0]=0;
            }
            YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_SMS,FLH_PRM_POWOFFWARN_SMS_LEN,fbuf);
            //YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_GPRS,FLH_PRM_POWOFFWARN_GPRS_LEN,fbuf); //修改为GPRS 控制
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_POFS,StrDat)) //设置断电服务器开关
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')
            {
                fbuf[0]=1;
            }
            else
            {
                fbuf[0]=0;
            }
            YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_GPRS,FLH_PRM_POWOFFWARN_GPRS_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_LBV,StrDat))  //设置电池低电压门限
    {
        if(YS_PrmPDNumString(StrDat,TRUE,7,3000000,4200000)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            ldat.l=atoi(StrDat);
            Value=ldat.l/1000;
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_BATT_LOW_LIMIT,FLH_PRM_BATT_LOW_LIMIT_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SLEEP,StrDat)) //休眠使能控制
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf);
            return(TRUE);
        }
    }

    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_CALLLOCK,StrDat)) //呼入功能设置
    {
     if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
     {
    	return(FALSE);
     }
     else
     {
    	 Value=atoi(StrDat);
    	 fbuf[0]=Value%256;
    	 YS_PrmWriteOneItem(FLH_PRM_CALL_FUNC,FLH_PRM_CALL_FUNC_LEN,fbuf);
    	 return(TRUE);
     }
    }*/
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_CALLDISP,StrDat)) //电主号码验证
     {
    	 if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	else
    	{
    		Value=atoi(StrDat);
    		fbuf[0]=Value%256;
    		YS_PrmWriteOneItem(FLH_PRM_CALL_DISP,FLH_PRM_CALL_DISP_LEN,fbuf);
    		return(TRUE);
    	}
     }*/


    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIBCHK,StrDat)) //振动报警参数
    {
        YS_PrmSetVibChk(StrDat);
        return(TRUE);
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_POFT,StrDat)) //断电报警延时
    {
      if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
      {
    	return(FALSE);
      }
      else
      {
    	  Value=atoi(StrDat);
    	  fbuf[0]=Value/256;
    	  fbuf[1]=Value%256;
    	  YS_PrmWriteOneItem(FLH_PRM_POWOFF_TIME,FLH_PRM_POWOFF_TIME_LEN,fbuf);
    	  return(TRUE);
     }
    }*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SLEEPT,StrDat)) //休眠时间
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            Value*=60;
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_SLEEP_TIME,FLH_PRM_SLEEP_TIME_LEN,fbuf);
            return(TRUE);
        }
    }
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_ACCLT,StrDat))	//设置ACC 设防检测时间
     {
    	 if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {
    		Value=atoi(StrDat);
    		fbuf[0]=Value/256;
    		fbuf[1]=Value%256;
    		YS_PrmWriteOneItem(FLH_PRM_ACC_TIME,FLH_PRM_ACC_TIME_LEN,fbuf);
    		return(TRUE);
    	 }
     }*/
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_ACCLOCK,StrDat)) //设置ACC 设防使能
     {
    	 if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {
    		if(StrDat[0]=='1')
    		{
    			fbuf[0]=1;
    		}
    		else
    		{
    			fbuf[0]=0;
    		}
    		YS_PrmWriteOneItem(FLH_PRM_ACC_LOCK,FLH_PRM_ACC_LOCK_LEN,fbuf);
    		return(TRUE);
    	 }
     }*/
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_OUT,StrDat)) //越界报警短信使能
     {
    	 if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {

    		 if(StrDat[0]=='1')
    		 {
    			 fbuf[0]=1;
    		 }
    		 else
    		 {
    			fbuf[0]=0;
    		 }
    //			 YS_PrmWriteOneItem(FLH_PRM_OUT_SMS,FLH_PRM_OUT_SMS_LEN,fbuf);
    		YS_PrmWriteOneItem(FLH_PRM_OUT_GPRS,FLH_PRM_OUT_GPRS_LEN,fbuf); //修改为GPRS 控制
    		 return(TRUE);
    	 }
     }*/
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_OUTS,StrDat))//越界报警服务器使能
     {
    	 if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {
    		 if(StrDat[0]=='1')
    		 {
    			 fbuf[0]=1;
    		 }
    		 else
    		 {
    			 fbuf[0]=0;
    		 }
    	 	YS_PrmWriteOneItem(FLH_PRM_OUT_GPRS,FLH_PRM_OUT_GPRS_LEN,fbuf);
    	 	return(TRUE);
    	 }
     }*/
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_WAKEUP,StrDat)) //自动唤醒使能
     {
    	 if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {
    		Value=atoi(StrDat);
    		fbuf[0]=Value%256;
    		 if(StrDat[0]=='1')
    		 {
    			 fbuf[0]=1;
    		 }
    		 else
    		 {
    			 fbuf[0]=0;
    		 }
    	 	YS_PrmWriteOneItem(FLH_PRM_WAKEUP_EN,FLH_PRM_WAKEUP_EN_LEN,fbuf);
    		return(TRUE);
    	 }
     }
     else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_WAKEUPT,StrDat))//自动唤醒时间周期
     {
    	 if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {
    		 Value=atoi(StrDat);
    		 Value*=60;  //转换为秒
    		 fbuf[0]=Value/256;
    		 fbuf[1]=Value%256;
    		 YS_PrmWriteOneItem(FLH_PRM_WAKEUP_TIME,FLH_PRM_WAKEUP_TIME_LEN,fbuf);
    		 return(TRUE);
    	 }
     }*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_APN,StrDat))//设置APN 信息
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_PPP_APN_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_PPP_APN_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SERVIP,StrDat))//设置服务器IP
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_SERV_ADDR_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_SERV_ADDR_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SERVPORT,StrDat))//设置服务器端口
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_EQUTYPE,StrDat))//设置服务器端口
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_TERI_TYPE,FLH_PRM_TERI_TYPE_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_IMEI,StrDat))//设置IMEI
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_TERI_IMEI_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_TERI_IMEI_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_TERI_IMEI,FLH_PRM_TERI_IMEI_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SPEED,StrDat))//设置超速报警门限
    {
    	 Value=atoi(StrDat);
    	 fbuf[0]=Value/256;
    	 fbuf[1]=Value%256;
    	 //YS_PrmWriteOneItem(FLH_PRM_SPEED_LIMIT,FLH_PRM_SPEED_LIMIT_LEN,fbuf);
    	 return(TRUE);
    } */
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIB_BASE,StrDat))//设置振动报警基数值
    {
        Value=atoi(StrDat);
        fbuf[0]=Value;
        YS_PrmWriteOneItem(FLH_PRM_VIB_BASEVALUE,FLH_PRM_VIB_BASEVALUE_LEN,fbuf);
        return(TRUE);
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_FLIT,StrDat))//设置深度休眠时间
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_FLIT_SET,FLH_PRM_FLIT_SET_LEN,fbuf);
            return(TRUE);
        }
    }	 */
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIBT,StrDat))//设置振动报警延时
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,180)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value;
            YS_PrmWriteOneItem(FLH_PRM_VIBWARN_DELAY,FLH_PRM_VIBWARN_DELAY_LEN,fbuf);
            return(TRUE);
        }
    }		*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_ANGLE_SEND,StrDat))//设置拐点补传使能
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')
            {
                fbuf[0]=1;
            }
            else
            {
                fbuf[0]=0;
            }
            YS_PrmWriteOneItem(FLH_PRM_ANGLE_SEND_FLAG,FLH_PRM_ANGLE_SEND_FLAG_LEN,fbuf);
            return(TRUE);
        }
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_WARN_TIMES,StrDat))//设置拐点补传使能
    {
    	if(YS_PrmPDNumString(StrDat,FALSE,1,0,100)==FALSE)
    	{
    	    	return(FALSE);
    	}
    	else
    	{
    	    	fbuf[0]=atoi(StrDat);
    	   	YS_PrmWriteOneItem(FLH_PRM_WARN_TIMES_MAX,FLH_PRM_WARN_TIMES_MAX_LEN,fbuf);
    	    	return(TRUE);
    	}
    }	*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_GPS_FINTER_EN,StrDat)) //GPS漂移优化使能
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')//使能
            {
                fbuf[0]=1;
                YS_PrmWriteOneItem(FLH_PRM_NV_GPS_PYYH,FLH_PRM_NV_GPS_PYYH_LEN,fbuf);
            }
            else              //失能
            {
                fbuf[0]=0;
                YS_PrmWriteOneItem(FLH_PRM_NV_GPS_PYYH,FLH_PRM_NV_GPS_PYYH_LEN,fbuf);
            }
            return(TRUE);
        }
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIBSMS_EN,StrDat)) //振动短信报警使能设置
    {
    	if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	{
    		return(FALSE);
    	}
    	else
    	{
    		if(StrDat[0]=='1')
    		{
    			fbuf[0]=1;
    		}
    		else
    		{
    			fbuf[0]=0;
    		}
    		//YS_PrmWriteOneItem(FLH_PRM_VIBWARN_SMS,FLH_PRM_VIBWARN_SMS_LEN,fbuf);
    		return(TRUE);
    	}
    }	*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_POFSMS_EN,StrDat)) //断电/低压报警短信使用设置
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')
            {
                fbuf[0]=1;
            }
            else
            {
                fbuf[0]=0;
            }
            YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_SMS,FLH_PRM_POWOFFWARN_SMS_LEN,fbuf);
            return(TRUE);
        }
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_OUTSMS_EN,StrDat)) //越界报警SMS使用设置
    {
    	if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	{
    		return(FALSE);
    	}
    	else
    	{
    		if(StrDat[0]=='1')
    		{
    			fbuf[0]=1;
    		}
    		else
    		{
    			fbuf[0]=0;
    		}
    		YS_PrmWriteOneItem(FLH_PRM_OUT_SMS,FLH_PRM_OUT_SMS_LEN,fbuf);
    		return(TRUE);
    	}
    }	*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_AGPS_EN,StrDat)) //GPS漂移优化使能
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,2)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            fbuf[0]=atoi(StrDat);
            YS_PrmWriteOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);
            return(TRUE);
        }
    }

    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_DW_ADDR,StrDat))//设置服务器IP
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_NV_DWADDR_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_NV_DWADDR_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SJIP,StrDat))//设置服务器IP
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_NV_DWADDR_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_NV_DWADDR_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_DW_PORT,StrDat))//设置服务器端口
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_DLPORT,StrDat))//设置服务器端口
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_DW_FLAG,StrDat)) //远程升级控制
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            fbuf[0]=atoi(StrDat);
            YS_PrmWriteOneItem(FLH_PRM_NV_DWFLAG,FLH_PRM_NV_DWFLAG_LEN,fbuf);
            YS_PrmQuickWriteNV();
            if(fbuf[0]==1)  //如果是请求开始升级
            {
                YS_SysRsqSystemRST(YS_RST_FLAG_USER);	 //复位系统
            }
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_PWM,StrDat)) 	//设置操作密码
    {
        if(YS_SmsCompString((u8 *)StrDat,strlen(StrDat),6)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<6; i++)
            {
                fbuf[i]=StrDat[i];
            }
            YS_PrmWriteOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_pARAM_NAME_AUTO_ENABLE,StrDat)) //设置自动升级使能
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            fbuf[0]=atoi(StrDat);
            YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_ENABLE,FLH_PRM_NV_AUTOPLAT_ENABLE_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_pARAM_NAME_AUTO_ADDR,StrDat))//设置自动升级IP
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_NV_AUTOPLAT_ADDR_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_NV_AUTOPLAT_ADDR_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_ADDR,FLH_PRM_NV_AUTOPLAT_ADDR_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_pARAM_NAME_AUTODLPORT,StrDat))//设置服务器端口
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_PORT,FLH_PRM_NV_AUTOPLAT_PORT_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_pARAM_NAME_AUTO_FLAG,StrDat)) //设置自动升级连接标志
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            fbuf[0]=atoi(StrDat);
            YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_FLAG,FLH_PRM_NV_AUTOPLAT_FLAG_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_UPDATE_MODE,StrDat)) //设置工作模式
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            fbuf[0]=atoi(StrDat);
            YS_PrmWriteOneItem(FLH_PRM_UPDATE_MODE,FLH_PRM_UPDATE_MODE_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SYNC_DT,StrDat)) //设置系统时钟
    {
        if(strlen(StrDat)!=19)
        {
            return(FALSE);
        }
        else
        {
            YS_SysSyncServerDatetime(StrDat);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_ALARM_CLOCK,StrDat))	//设置系统闹铃
    {
        if(YS_PrmSetAlarmClock(StrDat)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SKYLINE_FLAG,StrDat))//设置系统是否为天线调试模式
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value;
            YS_PrmWriteOneItem(FLH_PRM_SKYLINE_FLAG,FLH_PRM_SKYLINE_FLAG_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else
    {
        return(FALSE);
    }
    return(FALSE);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PamWriteNVDeal
功能说明：参数存贮运行控制
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_PrmWriteNVDeal(void)
{
    //S16 error;

    if(t_ParamManage.WriteFlag==1)
    {
        t_ParamManage.WriteDelay++;
        YS_GprsDebugString("NV Write count",t_ParamManage.WriteDelay);
        if(t_ParamManage.WriteDelay>=10)
        {
            t_ParamManage.WriteFlag=0;
            YS_GprsDebugString("NV Write Opt Flag",0);
            sjfun_WriteFilesToNV();
//   			WriteRecord(NVRAM_EF_GPSTERM1_LID,1,YS_Param_Buf,512,&error);
//			WriteRecord(NVRAM_EF_YSPRM_SETTING_LID,1,YS_Param_Buf,16,&error);
        }
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SetPowLowFlag
功能说明：设置低电压报警使能
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SetPowLowFlag(void)
{
    u8 fbuf[4];

    YS_PrmReadOneItem(FLH_PRM_NV_POWLOW_FLAG,FLH_PRM_NV_POWLOW_FLAG_LEN,fbuf);
    if(fbuf[0]==0)
    {
        YS_UartDebugString("set powlow flag");
        fbuf[0]=1;
        YS_PrmWriteOneItem(FLH_PRM_NV_POWLOW_FLAG, FLH_PRM_NV_POWLOW_FLAG_LEN, fbuf);
    }

}

/*-----------------------------------------------------------------------------------------
函数名：YS_CleanPowLowFlag
功能说明：清除低电压报警使能
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_CleanPowLowFlag(void)
{
    u8 fbuf[4];

    YS_PrmReadOneItem(FLH_PRM_NV_POWLOW_FLAG,FLH_PRM_NV_POWLOW_FLAG_LEN,fbuf);
    if(fbuf[0]==1)
    {
        YS_UartDebugString("clean powlow flag");
        fbuf[0]=0;
        YS_PrmWriteOneItem(FLH_PRM_NV_POWLOW_FLAG, FLH_PRM_NV_POWLOW_FLAG_LEN, fbuf);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_PrmFSFilesISExist
功能说明：判断一个文件是否存在
修改记录：
-------------------------------------------------------------------------------------------*/
U8 YS_PrmFSFilesISExist(const kal_uint16* file_name)
{
	int res = sjfun_File_Open(file_name, FS_READ_ONLY);
	sjfun_File_Close(res);
	if (res < 0)
	{
		return 0;
	}
	return 1;
}

