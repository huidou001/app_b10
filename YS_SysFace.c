/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Gprs.c  2013-06-25
���ļ��ù���Ӧ�ò������ײ����Ľ����ӿڡ�
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

t_Ys_Rst_Manage	t_ysRstManage;
t_Cell_Info_Manage t_MainCellManage;
t_Cell_Info_Manage t_NbrCellManage[6];

/*-----------------------------------------------------------------------------------------
��������YS_SysRstManageInit
����˵������λ�����ʼ��
�޸ļ�¼��
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
��������YS_SysSyncServerDatetime
����˵����ͬ��ϵͳʱ���������ʱ��һ�¡�
�޸ļ�¼��
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
��������YS_SysRstManageCtrl
����˵������λ������ʱ����
�޸ļ�¼��
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
��������YS_SysRstRecordNum
����˵������λ��¼����1
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_SysRstRecordNum(void)
{
    u8 fbuf[2];
    u16 num=0;

    YS_PrmReadOneItem(FLH_PRM_RST_NUM, FLH_PRM_RST_NUM_LEN, fbuf);
    num = fbuf[0]*256+fbuf[1];

    num++; //��λ������1
    if (num==0xffff)
    {
        num = 0xfffe;//���ֵ
    }
    fbuf[0]=num/256;
    fbuf[1]=num%256;
    YS_PrmWriteOneItem(FLH_PRM_RST_NUM, FLH_PRM_RST_NUM_LEN, fbuf);
}

/*-----------------------------------------------------------------------------------------
��������YS_SysRsqSystemRST
����˵�����������ϵͳ��λ����
�޸ļ�¼��
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
		//YS_GpsDistanceSaveExtern();		//�������������
		YS_PrmReadOneItem(FLH_PRM_DEBUG_ENABLE,FLH_PRM_DEBUG_ENABLE_LEN,fbuf);
		if(fbuf[0]==1) //����������
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
��������YS_SysGetBarCodeInterface
����˵����У׼��־��ȡ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_SysGetBarCodeInterface(void)
{
	t_ysRstManage.FactoryParam=1;
}

/*-----------------------------------------------------------------------------------------
��������YS_SysGetBarCodeCtrl
����˵����У׼��־��ȡ��ʱ���ƺ���
�޸ļ�¼��
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
	//ϵͳĬ��ʱ��Ϊ��12-2-1 0:0:0
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
��������YS_SysSyncServerDatetime
����˵����ͬ��ϵͳʱ���������ʱ��һ�¡�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_SysGprsSyncSystemDT(u8 *buf)
{
	//ϵͳĬ��ʱ��Ϊ��12-2-1 0:0:0
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
��������YS_SysSyncServerDatetime
����˵����ͬ��ϵͳʱ���������ʱ��һ�¡�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_SysSyncSystemDT(void)
{
	//ϵͳĬ��ʱ��Ϊ��12-2-1 0:0:0
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
��������YS_SysGetSystemTimeToString
����˵������ȡϵͳʱ�䣬��ת��Ϊʱ���ַ���
�޸ļ�¼��
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
��������YS_SysGetCaliBartionDataInterface
����˵����У׼���ݻ�ȡ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_SysGetCaliBartionDataInterface(void)
{
	t_ysRstManage.CaliDataFlag=1;
}

/*-----------------------------------------------------------------------------------------
��������YS_SysPackNetInfo
����˵������ȡ������Ϣ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_SysPackNetInfo(u8 *PackBuf)
{
    u16 k,pos,Num;
    Avl_UWord wdat;

    pos=0;
    PackBuf[pos]=0x31;
    pos++;
    PackBuf[pos]=11+6*t_MainCellManage.NrbCellNum;//����
    pos++;
    Num=1+t_MainCellManage.NrbCellNum;
    PackBuf[pos]=Num;
    pos++;
    //��С��

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

    //��С��
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

