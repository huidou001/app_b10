/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Gprs.c  2013-06-25
此文件用管理应用层程序与底层程序的交互接口。
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

t_Ys_Rst_Manage	t_ysRstManage;
t_Cell_Info_Manage t_MainCellManage;
t_Cell_Info_Manage t_NbrCellManage[6];

/*-----------------------------------------------------------------------------------------
函数名：YS_SysRstManageInit
功能说明：复位管理初始化
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SysRstManageInit(void)
{
	//ycsj_debug("YS_SysRstManageInit!!!\n");
	t_ysRstManage.FactoryParam=0;
	t_ysRstManage.RstFlag=0;
	t_ysRstManage.RstDelay=0;
	t_ysRstManage.WatchDogDelay=0;
	t_ysRstManage.CaliDataFlag=0;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SysSyncServerDatetime
功能说明：同步系统时钟与服务器时间一致。
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SysSyncServerDatetime(char *DateStr)
{
	// static bool SyncFlag=TRUE;
	//t_rtc t_Rec;
	u8 DataBuf[6];

	DataBuf[0]=YS_CodeHexCharConver(DateStr[2])*10+YS_CodeHexCharConver(DateStr[3]);
	DataBuf[1]=YS_CodeHexCharConver(DateStr[5])*10+YS_CodeHexCharConver(DateStr[6]);
	DataBuf[2]=YS_CodeHexCharConver(DateStr[8])*10+YS_CodeHexCharConver(DateStr[9]);
	DataBuf[3]=YS_CodeHexCharConver(DateStr[11])*10+YS_CodeHexCharConver(DateStr[12]);
	DataBuf[4]=YS_CodeHexCharConver(DateStr[14])*10+YS_CodeHexCharConver(DateStr[15]);
	DataBuf[5]=YS_CodeHexCharConver(DateStr[17])*10+YS_CodeHexCharConver(DateStr[18]);
	YS_UartDataOutInterfacel(0x8110, (u8*)"sync by gprs TIME", sizeof("sync by gprs TIME"));
	sjfun_Set_System_DateTime(DataBuf);
	YS_RunSetDTSyncFlag();
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SysRstManageCtrl
功能说明：复位管理延时处理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SysRstManageCtrl(void)
{
	if(t_ysRstManage.RstFlag==1)
	{
		ycsj_debug("System Rst Deal by Assert");
		t_ysRstManage.RstDelay++;
		if(t_ysRstManage.RstDelay>=10)
		{
			ycsj_debug("System Rst Deal by Assert");
			t_ysRstManage.RstDelay=0;
			t_ysRstManage.RstFlag=0;
			//sjfun_request_reset();
			//sjfun_sys_reset();
			sjfun_System_Assert();
		}
	}
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SysRstRecordNum
功能说明：复位记录自增1
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SysRstRecordNum(void)
{
    u8 fbuf[2];
    u16 num=0;

    YS_PrmReadOneItem(FLH_PRM_RST_NUM, FLH_PRM_RST_NUM_LEN, fbuf);
    num = fbuf[0]*256+fbuf[1];

    num++; //复位次数加1
    if (num==0xffff)
    {
        num = 0xfffe;//最大值
    }
    fbuf[0]=num/256;
    fbuf[1]=num%256;
    YS_PrmWriteOneItem(FLH_PRM_RST_NUM, FLH_PRM_RST_NUM_LEN, fbuf);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SysRsqSystemRST
功能说明：请求进行系统复位处理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SysRsqSystemRST(u8 RstFlag)
{
	//static u8 	RstEntry=FALSE;
	//static u8	RstDelay=0;
	u8 fbuf[FLH_PRM_RST_RECORD_LEN];
	u8 RdNum,RdWP;

	if(t_ysRstManage.RstFlag==0)
	{
		//YS_BlindManageResetInterface();
		YS_GpsRefreshLastPoint();
		YS_SysRstRecordNum();
		//YS_GpsDistanceSaveExtern();		//存贮里程序数据
		YS_PrmReadOneItem(FLH_PRM_DEBUG_ENABLE,FLH_PRM_DEBUG_ENABLE_LEN,fbuf);
		if(fbuf[0]==1) //如果允许调试
		{
			YS_PrmReadOneItem(FLH_PRM_RST_RECORD, FLH_PRM_RST_RECORD_LEN, fbuf);
			RdNum=fbuf[0];
			RdWP=fbuf[1];
			fbuf[RdWP+2]=RstFlag;
			RdNum++;
			if(RdNum>8)
			RdNum=8;
			RdWP++;
			RdWP%=8;
			fbuf[0]=RdNum;
			fbuf[1]=RdWP;
			YS_PrmWriteOneItem(FLH_PRM_RST_RECORD,FLH_PRM_RST_RECORD_LEN,fbuf);
		}
		YS_PrmQuickWriteNV();
		t_ysRstManage.RstFlag=1;
	}
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SysGetBarCodeInterface
功能说明：校准标志获取入口
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SysGetBarCodeInterface(void)
{
	t_ysRstManage.FactoryParam=1;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SysGetBarCodeCtrl
功能说明：校准标志获取定时控制函数
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SysGetBarCodeCtrl(void)
{
	u8  GetBuf[300];
	u16 pos;
	if(t_ysRstManage.FactoryParam==1)
	{
		t_ysRstManage.FactoryParam=0;
		sjfun_SysGetBarCodeValue(GetBuf);
		YS_UartDataOutInterfacel(INTER_ACK_CALIBRATION_GET,GetBuf,64);
	}

	if(t_ysRstManage.CaliDataFlag==1)
	{
		t_ysRstManage.CaliDataFlag=0;
		pos = sjfun_GetCaliBartionData(GetBuf);
		YS_UartDataOutInterfacel(INTER_ACK_CALIBAR_DATA_GET,GetBuf,pos);
	}
}


void YS_SysAlmSetSystem(u8 *dbuf)
{
	sjfun_Set_System_DateTime(dbuf);
}


void YS_SysAlmReadSystem(u8 *dbuf)
{
	sjfun_Get_System_DateTime(dbuf);
}

/***********
void YS_SysSyncSystemDT(u8 *DateTime)
{
	//系统默认时间为：12-2-1 0:0:0
	t_rtc t_Rec;
	t_Rec.rtc_year=DateTime[0];
	t_Rec.rtc_mon=DateTime[1];
	t_Rec.rtc_day=DateTime[2];
	t_Rec.rtc_hour=DateTime[3];
	t_Rec.rtc_min=DateTime[4];
	t_Rec.rtc_sec=DateTime[5];
	RTC_InitTC_Time(&t_Rec);
}
*/

/*-----------------------------------------------------------------------------------------
函数名：YS_SysSyncServerDatetime
功能说明：同步系统时钟与服务器时间一致。
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SysGprsSyncSystemDT(u8 *buf)
{
	//系统默认时间为：12-2-1 0:0:0
	//static bool SyncFlag=TRUE;
	//t_rtc t_Rec;
	u8 timebuf[6];

    timebuf[0] = buf[3];
    timebuf[1] = buf[4];
    timebuf[2] = buf[5];
    timebuf[3] = buf[0];
    timebuf[4] = buf[1];
    timebuf[5] = buf[2];
	if(YS_RunGetDTSyncStatus()==FALSE)
	{
		YS_UartDataOutInterfacel(0x8110, (u8*)"sync by GPRS TIME", sizeof("sync by GPRS TIME"));
		//SyncFlag=FALSE;
		YS_CodeCretaeBJDT(timebuf);
		sjfun_Set_System_DateTime(timebuf);
		YS_RunSetDTSyncFlag();
	}
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SysSyncServerDatetime
功能说明：同步系统时钟与服务器时间一致。
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SysSyncSystemDT(void)
{
	//系统默认时间为：12-2-1 0:0:0
	//static bool SyncFlag=TRUE;
	t_Gps_Data_Info	t_GetGps;
	//t_rtc t_Rec;

	if(YS_RunGetDTSyncStatus()==FALSE)
	{
		YS_UartDataOutInterfacel(0x8110, (u8*)"sync by gps TIME", sizeof("sync by gps TIME"));
		//SyncFlag=FALSE;
		YS_GpsGetPosData(&t_GetGps);
		YS_CodeCretaeBJDT(t_GetGps.DataTime);
		sjfun_Set_System_DateTime(t_GetGps.DataTime);
		YS_RunSetDTSyncFlag();
	}
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SysGetSystemTimeToString
功能说明：获取系统时间，并转换为时间字符串
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_SysGetSystemTimeToString(char *PackBuf)
{
	u8 dtBuf[12];
	u16 len;

	sjfun_Get_System_DateTime(dtBuf);
	sprintf(PackBuf,"%02d-%02d-%02d %02d:%02d:%02d",dtBuf[0],dtBuf[1],dtBuf[2],dtBuf[3],dtBuf[4],dtBuf[5]);
	len=strlen(PackBuf);
	return(len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SysGetCaliBartionDataInterface
功能说明：校准数据获取入口
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SysGetCaliBartionDataInterface(void)
{
	t_ysRstManage.CaliDataFlag=1;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SysPackNetInfo
功能说明：获取网络信息
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_SysPackNetInfo(u8 *PackBuf)
{
    u16 k,pos,Num;
    Avl_UWord wdat;

    pos=0;
    PackBuf[pos]=0x31;
    pos++;
    PackBuf[pos]=11+6*t_MainCellManage.NrbCellNum;//长度
    pos++;
    Num=1+t_MainCellManage.NrbCellNum;
    PackBuf[pos]=Num;
    pos++;
    //主小区

    wdat.w = t_MainCellManage.mcc;
    PackBuf[pos]=wdat.b[AVL_WSTOR_FIR];
    pos++;
    PackBuf[pos]=wdat.b[AVL_WSTOR_SEC];
    pos++;
    wdat.w = t_MainCellManage.mnc;
    PackBuf[pos]=wdat.b[AVL_WSTOR_FIR];
    pos++;
    PackBuf[pos]=wdat.b[AVL_WSTOR_SEC];
    pos++;
    wdat.w = t_MainCellManage.lac;
    PackBuf[pos]=wdat.b[AVL_WSTOR_FIR];
    pos++;
    PackBuf[pos]=wdat.b[AVL_WSTOR_SEC];
    pos++;
    wdat.w = t_MainCellManage.ci;
    PackBuf[pos]=wdat.b[AVL_WSTOR_FIR];
    pos++;
    PackBuf[pos]=wdat.b[AVL_WSTOR_SEC];
    pos++;
    PackBuf[pos]=t_MainCellManage.ta;
    pos++;
    PackBuf[pos]=t_MainCellManage.rxlev;
    pos++;

    //临小区
    if(t_MainCellManage.NrbCellNum>0)
    {
        for(k=0; k<t_MainCellManage.NrbCellNum; k++)
        {
            wdat.w = t_NbrCellManage[k].lac;
            PackBuf[pos]=wdat.b[AVL_WSTOR_FIR];
            pos++;
            PackBuf[pos]=wdat.b[AVL_WSTOR_SEC];
            pos++;
            wdat.w = t_NbrCellManage[k].ci;
            PackBuf[pos]=wdat.b[AVL_WSTOR_FIR];
            pos++;
            PackBuf[pos]=wdat.b[AVL_WSTOR_SEC];
            pos++;
            PackBuf[pos]=0;
            pos++;
            PackBuf[pos]=t_NbrCellManage[k].rxlev;
            pos++;
        }
    }
    return(pos);
}

