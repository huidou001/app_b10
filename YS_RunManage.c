/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_RunManage.c  2013-06-22
ϵͳ����״̬ά��
�޸ļ�¼��
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

extern bool SysInfoInitOk=FALSE;

t_Sys_Run_Status		t_SysRunStatus;
t_Flow_Info			t_FlowInfo;
t_Gprs_Warn_Manage	t_GprsWarnFlag;
t_Avl_Auto_Reset		t_AvlAutoReset;
t_Ys_Trace_Manage	t_ysTraceManage;
t_VIB_Finter_Deal		t_VibFinterDeal;
//t_Led_Re_Show_Ctrl	t_LedReShow;
t_Net_Info 			t_NetInfo;

#define RUN_WARN_DELAY_DEF		10


/*-----------------------------------------------------------------------------------------
��������YS_FactoryMode
����˵��������ģʽ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_FactoryMode(void)
{
    char DispBuf[200];
    char fbuf[100],StrDat[20];
    u16 i,len,pos,dat16;
    static u8 times=0;
    t_Gps_Data_Info t_GpsData;
    YS_GpsGetPosData(&t_GpsData);

    pos=0;
    if (times >= 3)
    {
        times=0;
        sprintf(StrDat,"Ver:");//�汾
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=StrDat[i];
            pos++;
        }

        len=strlen(EBIKE_SOFT_VER);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=EBIKE_SOFT_VER[i];
            pos++;
        }
        DispBuf[pos]=' ';
        pos++;

        sprintf(StrDat,"ip:");
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=StrDat[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,(u8 *)fbuf);//ip��ַ�˿�
        len=YS_CodeBufRealLen((u8 *)fbuf,FLH_PRM_SERV_ADDR_LEN);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=fbuf[i];
            pos++;
        }
        DispBuf[pos]=':';
        pos++;
        YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,(u8 *)fbuf);
        dat16=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat16);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=StrDat[i];
            pos++;
        }
        DispBuf[pos]=' ';
        pos++;
        DispBuf[pos]=' ';
        pos++;

        sprintf(StrDat,"imei=");
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=StrDat[i];
            pos++;
        }
        len=strlen((char *)t_FlowInfo.ImeiInfo);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=t_FlowInfo.ImeiInfo[i];
            pos++;
        }
        DispBuf[pos]=' ';
        pos++;

        sprintf(StrDat,"id=");
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=StrDat[i];
            pos++;
        }
        YS_RunAddIDInfo((u8 *)fbuf);
        len=YS_CodeBufRealLen((u8 *)fbuf,FLH_PRM_SIM_CODE_LEN);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=fbuf[i];
            pos++;
        }
        DispBuf[pos]=' ';
        pos++;

        sprintf(StrDat,"gsm=");
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=StrDat[i];
            pos++;
        }

        DispBuf[pos]=t_SysRunStatus.CsqValue/10+0x30;                                      //GSM�ź�
        pos++;
        DispBuf[pos]=t_SysRunStatus.CsqValue%10+0x30;                                      //GSM�ź�
        pos++;
        DispBuf[pos]=' ';
        pos++;

        sprintf(StrDat,"gpspwr=");
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=StrDat[i];
            pos++;
        }
        DispBuf[pos]=YS_RunGetGpsPower()+0x30;                               			//GPSpwr
        pos++;
        DispBuf[pos]=' ';
        pos++;

        sprintf(StrDat,"gps=");
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=StrDat[i];
            pos++;
        }
        DispBuf[pos]=t_GpsData.Effective+0x30;                               			//GPS
        pos++;
        DispBuf[pos]=' ';
        pos++;

        sprintf(StrDat,"ACC=");
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            DispBuf[pos]=StrDat[i];
            pos++;
        }
        DispBuf[pos]=t_SysRunStatus.AccStatus+0x30;                          //acc
        pos++;
        DispBuf[pos]=' ';
        pos++;

    }
    else
    {

        times++;
        DispBuf[pos]= t_SysRunStatus.SimStatus+0x30;                                      //SIM��
        pos++;
        DispBuf[pos]=',';
        pos++;

        if (t_SysRunStatus.RunFlow == YS_RUN_FLOW_IDLE_DEAL)
        {
            DispBuf[pos]= 0x31;                                      //GSM�ź�
        }
        else
        {
            DispBuf[pos]= 0x30;                                      //GSM�ź�
        }

        pos++;
        DispBuf[pos]=',';
        pos++;

        DispBuf[pos]=YS_RunGetGpsPower()+0x30;                               			//GPSPWR
        pos++;
        DispBuf[pos]=',';
        pos++;

        DispBuf[pos]=t_GpsData.Effective+0x30;                               			//GPS
        pos++;
        DispBuf[pos]=',';
        pos++;

        DispBuf[pos]=t_SysRunStatus.AccStatus+0x30;                          		//acc
        pos++;
        DispBuf[pos]=',';
        pos++;

        DispBuf[pos]=sjfun_Gpio_Read_Value(YS_PIN_NO_WAKEUP)+0x30;                          		//acc
        pos++;
        DispBuf[pos]=',';
        pos++;
    }
    DispBuf[pos]=0x0d;                                 //�س�����
    pos++;
    DispBuf[pos]=0x0a;
    pos++;
    DispBuf[pos]='\0';
    pos++;
    if (t_FlowInfo.DebugLog == TRUE)
    {
        ycsj_debug((char *)(DispBuf));
    }

//#endif
}

void SetDebugLog(void)
{
    if (t_FlowInfo.DebugLog == FALSE)
    {
        t_FlowInfo.DebugLog = TRUE;
    }
    else
    {
        t_FlowInfo.DebugLog = FALSE;
    }
}
/*-----------------------------------------------------------------------------------------
��������YS_RunCleanWarmFlag
����˵�������������־
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunCleanWarmFlag(void)
{
    u8 i;
    for (i=0; i<AVL_WARM_GPSEXCEPT; i++)
    {
        t_SysRunStatus.WarnStatus[i]=FALSE;
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunCleanWarmFlag
����˵�������������־
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunCleanWarmACK(void)
{
    if (t_SysRunStatus.WarnStatus[AVL_WARM_EXIGEN] == TRUE)
    {
        t_SysRunStatus.WarnStatus[AVL_WARM_EXIGEN] = FALSE;
    }
    if (t_SysRunStatus.WarnStatus[AVL_WARM_INAREA] == TRUE)
    {
        t_SysRunStatus.WarnStatus[AVL_WARM_INAREA] = FALSE;
    }
    if (t_SysRunStatus.WarnStatus[AVL_WARM_OUTAREA] == TRUE)
    {
        t_SysRunStatus.WarnStatus[AVL_WARM_OUTAREA] = FALSE;
    }
    if (t_SysRunStatus.WarnStatus[AVL_WARM_MOVE] == TRUE)
    {
        t_SysRunStatus.WarnStatus[AVL_WARM_MOVE] = FALSE;
    }

}

/*-----------------------------------------------------------------------------------------
��������YS_GPRSAddIDInfo
����˵����id��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunAddIDInfo(u8 *IDBuf)
{
    u8 i,len;
    u8 fbuf[50];
//    u8 Simcode[]="12800000003";
    YS_PrmReadOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_SIM_CODE_LEN);
    if(len>1)
    {
        for(i=0; i<len; i++)
        {
            IDBuf[i]=fbuf[i];
        }
    }
    else
    {
        for(i=0; i<11; i++)
        {
            IDBuf[i]=t_FlowInfo.ImeiInfo[3+i];
        }
        len=11;

    }
    IDBuf[len]=0;
}

u8 YS_RunGetCellNum(void)
{
    return t_NetInfo.Cell_Num;
}

void YS_SetNetInfo(u8 CellNum,u16 *InputBuf)
{
    u8 i;

    t_NetInfo.Cell_Num = CellNum;

    t_NetInfo.t_MainCellInfo.arfcn  	= InputBuf[0];
    t_NetInfo.t_MainCellInfo.bsic   	= InputBuf[1];
    t_NetInfo.t_MainCellInfo.rxlev    = InputBuf[2];
    t_NetInfo.t_MainCellInfo.mcc	= InputBuf[3];
    t_NetInfo.t_MainCellInfo.mnc	= InputBuf[4];
    t_NetInfo.t_MainCellInfo.lac		= InputBuf[5];
    t_NetInfo.t_MainCellInfo.ci		= InputBuf[6];
    t_NetInfo.t_MainCellInfo.ta		= InputBuf[7];
    for(i=0; i<CellNum; i++)
    {
        t_NetInfo.t_CellInfo[i].arfcn		= InputBuf[8+i*7];
        t_NetInfo.t_CellInfo[i].bsic		= InputBuf[9+i*7];
        t_NetInfo.t_CellInfo[i].rxlev		= InputBuf[10+i*7];
        t_NetInfo.t_CellInfo[i].mcc		= InputBuf[11+i*7];
        t_NetInfo.t_CellInfo[i].mnc		= InputBuf[12+i*7];
        t_NetInfo.t_CellInfo[i].lac		= InputBuf[13+i*7];
        t_NetInfo.t_CellInfo[i].ci		= InputBuf[14+i*7];
    }
}


extern void YS_SetShutDownLedFlag(void);

void YS_SetShutDownFlag(void)
{
    YS_SetShutDownLedFlag();
    YS_UartDebugInterfacel(0x8110, (u8*)"set shut down flag", sizeof("set shut down flag"));
    t_FlowInfo.ShutDownFlag=TRUE;
}

bool YS_GetShutDownFlag(void)
{
    return t_FlowInfo.ShutDownFlag;
}

void YS_GetNetInfo(t_Net_Info *PackBuf)
{
    u16 i,len;
    u8 *pSour, *pAim;

    len=sizeof(t_Net_Info);
    pSour=(u8 *)(&t_NetInfo);
    pAim=(u8 *)PackBuf;
    for(i=0; i<len; i++)
    {
        pAim[i]=pSour[i];
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_SysGetNetInfo
����˵������ȡ������Ϣ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_SysGetNetInfo(u8 *PackBuf)
{
    u16 i,pos;

    t_Gps_Data_Info	t_GetGps;

    pos=0;
    memset(PackBuf,0,300);
    YS_GpsGetPosData(&t_GetGps);  //��Ӷ�λ��Ϣ
    PackBuf[pos]=t_GetGps.Effective;
    pos++;
    PackBuf[pos]=t_GetGps.Speed/256;
    pos++;
    PackBuf[pos]=t_GetGps.Speed%256;
    pos++;
    PackBuf[pos]=t_GetGps.Course/256;
    pos++;
    PackBuf[pos]=t_GetGps.Course%256;
    pos++;
    for(i=0; i<4; i++)
    {
        PackBuf[pos]=t_GetGps.Longitude[i];
        pos++;
    }

    for(i=0; i<4; i++)
    {
        PackBuf[pos]=t_GetGps.Latitude[i];
        pos++;
    }

    PackBuf[pos]=t_NetInfo.t_MainCellInfo.arfcn/256; //�����С����Ϣ
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.arfcn%256;
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.rxlev;
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.mcc/256;
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.mcc%256;
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.mnc/256;
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.mnc%256;
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.lac/256;
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.lac%256;
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.ci/256;
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.ci%256;
    pos++;
    PackBuf[pos]=t_NetInfo.t_MainCellInfo.ta;
    pos++;
    PackBuf[pos]=t_NetInfo.Cell_Num;
    pos++;
    if(t_NetInfo.Cell_Num>0)
    {
        for(i=0; i<t_NetInfo.Cell_Num; i++)
        {
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].arfcn/256; //�����С����Ϣ
            pos++;
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].arfcn%256;
            pos++;
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].rxlev;
            pos++;
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].mcc/256;
            pos++;
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].mcc%256;
            pos++;
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].mnc/256;
            pos++;
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].mnc%256;
            pos++;
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].lac/256;
            pos++;
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].lac%256;
            pos++;
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].ci/256;
            pos++;
            PackBuf[pos]=t_NetInfo.t_CellInfo[i].ci%256;
            pos++;
        }
    }
    return(pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetStatusInfo
����˵������ȡ����״̬����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunGetStatusInfo(u8 ID, u8 *FillBuf)
{
    switch(ID)
    {
        case FJYD_STATUS_ID_POWER:
            FillBuf[0]=t_SysRunStatus.PowerStatus;
            break;

        case FJYD_STATUS_ID_THIEF:
            FillBuf[0]=t_SysRunStatus.ThiefStatus;
            break;

        case FJYD_STATUS_ID_SLEEP:
            FillBuf[0]=t_SysRunStatus.SleepStatus;
            break;

        case FJYD_STATUS_ID_ACC:
            FillBuf[0]=t_SysRunStatus.AccStatus;
            break;

        case FJYD_STATUS_ID_LACID:
            FillBuf[0]=t_SysRunStatus.AreaID/256;
            FillBuf[1]=t_SysRunStatus.AreaID%256;
            break;

        case FJYD_STATUS_ID_CELLID:
            FillBuf[0]=t_SysRunStatus.CellID/256;
            FillBuf[1]=t_SysRunStatus.CellID%256;
            break;

        case FJYD_STATUS_ID_COUNTRYNAME:
            FillBuf[0]=t_SysRunStatus.CountryName/256;
            FillBuf[1]=t_SysRunStatus.CountryName%256;
            break;

        case FJYD_STATUS_ID_NETNAME:
            FillBuf[0]=t_SysRunStatus.NetName/256;
            FillBuf[1]=t_SysRunStatus.NetName%256;
            break;

        default:
            break;
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetCsqValue
����˵������ȡ�����ź�ǿ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_RunGetCsqValue(void)
{
    return(t_SysRunStatus.CsqValue);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetBattValue
����˵������ȡ��ǰ�ǳص���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_RunGetBattValue(void)
{
    return(t_SysRunStatus.BattValue);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetACCStatus
����˵������ȡ��ǰ��ص���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_RunGetACCStatus(void)
{
    return(t_SysRunStatus.AccStatus);
}

void YS_RunJudgeGpsEntryPSMOK(void)
{
    static u8 EqualCount=0;
    //static u8 GPSPSMOK = 0;
    static u16 BankGpsCollectTime=0;
    u16 CurGpsCollectTime;

    if(t_FlowInfo.GpsPSMStatue==TRUE)
    {
        return;
    }
    CurGpsCollectTime = YS_GpsGetCollectTimes();
    if(BankGpsCollectTime == CurGpsCollectTime)
    {
        EqualCount++;
        if(EqualCount>=9)
        {
            t_FlowInfo.GpsPSMStatue=TRUE;
        }
        else
        {
            t_FlowInfo.GpsPSMStatue=FALSE;
        }
    }
    else
    {
        BankGpsCollectTime = CurGpsCollectTime;
        EqualCount = 0;
    }
    return;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGpsPSMCtrl
����˵����GPS  PSM ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunGpsPSMCtrl(void)
{
    t_Gps_Data_Info	t_GetGps;

    YS_GpsGetPosData(&t_GetGps);
    YS_RunJudgeGpsEntryPSMOK();
    if((t_FlowInfo.GpsPSMStatue==FALSE)&&(t_GetGps.Effective == TRUE))
    {
        //t_FlowInfo.GpsPSMStatue=TRUE;
        //YS_UartDebugString("entry PSM");
        sjfun_GPS_Entry_PSM();
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_DWServerPackAddID
����˵��������ն�ID��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunAddImeiInfo(void)
{
    u8 i,len;
    u8 fbuf[20],OutBuf[20];
    bool rlt=FALSE;

    sjfun_Get_NV_IMEI(fbuf,8);
    YS_CodeBCDCharConver(fbuf,8,OutBuf);
    OutBuf[15] = 0;
    rlt = YS_CodePDNumString((char *)OutBuf);
//    YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,OutBuf,20);
    if(rlt==TRUE)
    {
        for(i=0; i<15; i++)
        {
            t_FlowInfo.ImeiInfo[i] = OutBuf[i];
        }
        t_FlowInfo.ImeiInfo[15]=0;
//        YS_RunAddImeiOwn();
    }

}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetIMSI
����˵������ȡ��ǰ��SIM ��IMSI��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunGetIMSI(char *ImsiStr)
{
    strcpy(ImsiStr,t_SysRunStatus.IMSI);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunAccStatusBrush
����˵��������ϵͳACC ״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunPowerStatusBrush(u8 ID)
{
    t_SysRunStatus.PowerStatus=ID;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunAccStatusBrush
����˵��������ϵͳACC ״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunAccStatusBrush(u8 ID)
{
    t_SysRunStatus.AccStatus =ID;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunVibCountAdd
����˵�������ϵͳ�����񶯵�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunVibCountAdd(void)
{
    t_SysRunStatus.VibTotalCount++;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetIMEI
����˵������ȡ��ǰ�ն�IMEI��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunGetIMEI(char *ImeiStr)
{
    u8 fbuf[30];
    u16 i,len;

    len=strlen((char *)t_FlowInfo.ImeiInfo);
    for(i=0; i<len; i++)
    {
        ImeiStr[i]=t_FlowInfo.ImeiInfo[i];
        i++;
    }
    ImeiStr[len]=0;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunAccStatusBrush
����˵��������ϵͳACC ״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunSetIMSIInfo(u8 *IMSIBuf, u8 IMSILen)
{
    u8 i;

    for(i=0; i<IMSILen; i++)
    {
        t_SysRunStatus.IMSI[i]=IMSIBuf[i];
    }
    t_SysRunStatus.IMSI[IMSILen]=0;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSetSimStatus
����˵��������SIM����״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void	YS_RunSetSimStatus(u8 ID)
{
    t_SysRunStatus.SimStatus=ID;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSetBattValue
����˵��������SIM����״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunSetBattValue(u16 BattValue)
{
    t_SysRunStatus.BattValue=BattValue;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetSystemStatus
����˵������ȡ��ǰϵͳ������״̬����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunGetSystemStatus(t_Sys_Run_Status *pt_SystemStatus)
{
    u16 i,len;
    u8 *pSour, *pAim;

    len=sizeof(t_Sys_Run_Status);
    pSour=(u8 *)(&t_SysRunStatus);
    pAim=(u8 *)pt_SystemStatus;
    for(i=0; i<len; i++)
    {
        pAim[i]=pSour[i];
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSetDTSyncFlag
����˵��������ϵͳʱ����ͬ����־
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunSetDTSyncFlag(void)
{
    t_FlowInfo.SysDTSyncFlag=1;
}

void YS_RunSetDTSyncFlagFalse(void)
{
    t_FlowInfo.SysDTSyncFlag=0;
}

/*-----------------------------------------------------------------------------------------
��������YS_GetDTSyncStatus
����˵������ȡϵͳʱ����ͬ����־
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_RunGetDTSyncStatus(void)
{
    if(t_FlowInfo.SysDTSyncFlag==1)
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

/*------------------------------------------------------------------------------------------
��������YS_RunGsmColckisOK
����˵�����ж�ϵͳʱ���Ƿ���ȷ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_RunGsmColckisOK(void)
{
    u8 dtbuf[6];

    if(t_FlowInfo.SysDTSyncFlag==1) //�����У׼����Ϊʱ����Ч
    {
        return(TRUE);
    }

    sjfun_Get_System_DateTime(dtbuf);
    if(dtbuf[0]<14)
    {
        return(FALSE);
    }

    if((dtbuf[1]==0)||(dtbuf[1]>12))
    {
        return(FALSE);
    }

    if((dtbuf[2]==0)||(dtbuf[2]>31))
    {
        return(FALSE);
    }

    if((dtbuf[3]>=24)||(dtbuf[4]>=60)||(dtbuf[5]>=60))
    {
        return(FALSE);
    }

    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSocketDataInc
����˵�����ۼ�SOCKET �շ�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunSocketDataInc(u8 Type)
{
    switch(Type)
    {
        case DATA_FLAG_REC_ADD:
            t_SysRunStatus.SckRecTimes++;
            break;

        case DATA_FLAG_SEND_ADD:
            t_SysRunStatus.SckSendTimes++;
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSmsDataInc
����˵�����ۼӶ����շ�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunSmsDataInc(u8 Type)
{
    switch(Type)
    {
        case DATA_FLAG_REC_ADD:
            t_SysRunStatus.SmsRecTimes++;
            break;

        case DATA_FLAG_SEND_ADD:
            t_SysRunStatus.SmsSendTimes++;
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunCallDataInc
����˵�����ۼ�ͨ���������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunCallDataInc(u8 Type)
{
    switch(Type)
    {
        case DATA_FLAG_REC_ADD:
            t_SysRunStatus.CallInTimes++;
            break;

        case DATA_FLAG_SEND_ADD:
            t_SysRunStatus.CallOutTimes++;
            break;
        default:
            break;
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSetBattValueReflashFlag
����˵�������õ�ǰ��ص������±�־
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunSetBattValueReflashFlag(void)
{
    t_FlowInfo.BattValueReflashFlag=1;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunReadBattValueReflashFlag
����˵������ȡ��ǰ��ص������±�־
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_RunReadBattValueReflashFlag(void)
{
    return(t_FlowInfo.BattValueReflashFlag);
}

void YS_RunSetJbWorkStatue(bool Flag)
{
    if(Flag==TRUE)
    {
        t_FlowInfo.gSensorJbWorkStatue=TRUE;
    }
    else
    {
        t_FlowInfo.gSensorJbWorkStatue=FALSE;
    }
}

void YS_RunSetTerWearStatue(bool Flag)
{
    t_FlowInfo.TerWearStatue = Flag;
}

bool YS_RunGetTerWearStatue(void)
{
    return t_FlowInfo.TerWearStatue;
}
/*-----------------------------------------------------------------------------------------
��������YS_RunInitSysInfo
����˵������ʼ��ϵͳ����״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunInitSysInfo(void)
{
    u8 i;

    t_SysRunStatus.RunFlow=YS_RUN_FLOW_INIT;
    t_SysRunStatus.RegStatus=0;
    t_SysRunStatus.ThiefStatus=1;
    t_SysRunStatus.CsqValue=0;
    t_SysRunStatus.AccStatus=0;
    t_SysRunStatus.AreaID=0;
    t_SysRunStatus.CellID=0;
    t_SysRunStatus.PPPTimes=0;
    t_SysRunStatus.SmsSendTimes=0;
    t_SysRunStatus.SmsRecTimes=0;
    t_SysRunStatus.SckSendTimes=0;
    t_SysRunStatus.SckRecTimes=0;
    t_SysRunStatus.CallOutTimes=0;
    t_SysRunStatus.CallInTimes=0;
    t_SysRunStatus.Distance=0;
    t_SysRunStatus.TotalRunTime=0;
    t_SysRunStatus.PowerStatus=0;
    t_SysRunStatus.SleepStatus=0;
    t_SysRunStatus.VibTotalCount=0;
    t_SysRunStatus.GsmBandValue=0;

    t_SysRunStatus.GsmSleepStatus = 0;
    t_SysRunStatus.FactoryMode=FALSE;

    t_FlowInfo.SocketID=SOCKET_ID_INIT_VALUE;
    t_FlowInfo.WebSckID=SOCKET_ID_INIT_VALUE;
    t_FlowInfo.RouteSckID=SOCKET_ID_INIT_VALUE;
    t_FlowInfo.AutoUpdateSckID=SOCKET_ID_INIT_VALUE;
    t_FlowInfo.ServLogErrTimes = 0;
    t_FlowInfo.PowSleepCount=0;
    t_FlowInfo.SleepCount=0;
    t_FlowInfo.VibSleepCount=0;
    t_FlowInfo.SocketErrTimes= 0;
    t_FlowInfo.SysDTSyncFlag=0;
    t_FlowInfo.SocOk = 0;
    t_FlowInfo.ExActiveFlag=FALSE;
    t_FlowInfo.LostNetCount=0;

    t_FlowInfo.SeverRegErrTime=0;


    //t_FlowInfo.TerStatueChang=FALSE;
    t_FlowInfo.BattValueReflashFlag = 0;
    //t_FlowInfo.TerStaReportCount=0;
    //t_FlowInfo.ActionAfterReptSta = 0x03;
    t_FlowInfo.AppIPExsitFlag=FALSE;
    t_FlowInfo.UbloxIPExsitFlag=FALSE;
    //t_FlowInfo.StartParkingCheck=0;
//    t_FlowInfo.GpsPowerFlag=FALSE;
    t_FlowInfo.GpsPowerFlag=TRUE;
    t_FlowInfo.GpsPSMStatue=FALSE;
    t_FlowInfo.IRSensorWorkStatue=FALSE;
    t_FlowInfo.gSensorJbWorkStatue=FALSE;
    t_FlowInfo.TerWearStatue=FALSE;
    t_GprsWarnFlag.PowerOffFlag=FALSE;
    t_GprsWarnFlag.BattLowFlag=FALSE;
    t_GprsWarnFlag.VibFlag=FALSE;
    t_GprsWarnFlag.PosOut=FALSE;
    t_GprsWarnFlag.AreaInFlag=FALSE;
    t_GprsWarnFlag.AreaOutFlag=FALSE;
    t_FlowInfo.WarnFlag=0;					//��ʼ��GPRS ��������
    t_FlowInfo.ShutDownFlag=FALSE;
    t_GprsWarnFlag.NewWarnFlag=FALSE;		//�豸�����±����ı�־
    t_FlowInfo.BankAccStatus = FALSE;
    for(i=0; i<16; i++)
    {
        t_SysRunStatus.WarnStatus[i]=0;
    }
    t_FlowInfo.DebugLog = TRUE;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetSysStatusFormPlat
����˵������ʱ��ȡ״̬��Ϣ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunGetSysStatusFormPlat(void)
{
    u8 *p8;
    u16 *p16;
    u8 i,BattLev;

    t_SysRunStatus.TotalRunTime++;
    p8=(u8 *)sjfun_Get_SysVar("SIM_Ready_Flag");
    t_SysRunStatus.SimStatus=*p8;

    if(t_SysRunStatus.SimStatus==1)
    {
        p8=(u8 *)sjfun_Get_SysVar("Gsm_IMSI_Info");
        for(i=0; i<15; i++)
        {
            t_SysRunStatus.IMSI[i]=p8[i];
        }
        t_SysRunStatus.IMSI[15]=0;
    }

    p8=(u8 *)sjfun_Get_SysVar("GSM_Ready_Flag");
    t_SysRunStatus.RegStatus=*p8;

    p8=(u8 *)sjfun_Get_SysVar("GSM_Singal_Level");
    t_SysRunStatus.CsqValue=*p8;

    p8=(u8 *)sjfun_Get_SysVar("Gsm_Band_Info");
    t_SysRunStatus.GsmBandValue=*p8;

    p8=(u8 *)sjfun_Get_SysVar("cur_bat_level");
    BattLev=*p8;
    YS_RunSetBattValueReflashFlag();  //modify by zcj at 2014-09-27
    switch(BattLev)
    {
        case 0:
            t_SysRunStatus.BattValue=3350;
            break;
        case 1:
            t_SysRunStatus.BattValue=3400;
            break;
        case 2:
            t_SysRunStatus.BattValue=3550;
            break;
        case 3:
            t_SysRunStatus.BattValue=3640;
            break;
        case 4:
            t_SysRunStatus.BattValue=3740;
            break;
        case 5:
            YS_SetPowLowFlag();
            t_SysRunStatus.BattValue=3870;
            break;
        default:
            YS_SetPowLowFlag();
            t_SysRunStatus.BattValue=4200;
            break;
    }
    p16=(u16 *)sjfun_Get_SysVar("Gsm_Get_CountryName");
    t_SysRunStatus.CountryName=*p16;

    p16=(u16 *)sjfun_Get_SysVar("Gsm_Get_NetName");
    t_SysRunStatus.NetName=*p16;

    p16=(u16 *)sjfun_Get_SysVar("Gsm_Get_AreaID");
    t_SysRunStatus.AreaID=*p16;

    p16=(u16 *)sjfun_Get_SysVar("Gsm_Get_CellID");
    t_SysRunStatus.CellID=*p16;

//    ycsj_debug("t_SysRunStatus.SimStatus=%d,t_SysRunStatus.RegStatus=%d,t_SysRunStatus.CsqValue=%d\r\n", t_SysRunStatus.SimStatus,t_SysRunStatus.RegStatus, t_SysRunStatus.CsqValue);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSocketSendData
����˵����SOCKET ���ݷ��ͽӿ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunSocketSendData(u8 *dbuf, u16 dlen)
{
    u32 SendRlt;
    u8 DbgBuf[600];
    YS_RunSocketDataInc(DATA_FLAG_SEND_ADD);
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_GPRSSEND,dbuf,dlen);
    YS_CodeHextoString(dbuf, dlen, DbgBuf);
    ycsj_debug((char *)DbgBuf);

    SendRlt=sjfun_Socket_Send(t_FlowInfo.SocketID, dbuf, dlen);
    if (SendRlt < 0)
    {
        ycsj_debug("gprs send fail %d", SendRlt);
        t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
    }
}

void YS_RunSetSeverReg(void)
{
    t_FlowInfo.SeverRegOk=TRUE;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunIdleHeartCtrl
����˵�������������ʹ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunIdleHeartCtrl(void)
{
    u16	HeartDelaySet;
    u8 	fbuf[4];

    YS_PrmReadOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);
    HeartDelaySet=fbuf[0]*256+fbuf[1];
    t_FlowInfo.HeartDelay++;
    if(t_FlowInfo.HeartDelay>=HeartDelaySet)
    {
        t_FlowInfo.HeartDelay=0;
        t_FlowInfo.HeartTimes++;
        if(t_FlowInfo.HeartTimes<=RUN_HEART_TIMES_DEF)
        {
            if (YS_GprsServerSendInterface(SERV_UP_CMD_HEART, NULL,0) == FALSE)
            {
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
            }
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunIdleHeartCtrl
����˵�������������ʹ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunIdleHeartAck(void)
{
    t_FlowInfo.HeartTimes=0;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunIdlePosCtrl
����˵������λ���ݰ����ʹ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunIdlePosCtrl(void)
{
    u16	PosDelaySet;
    u8 fbuf[4];

    YS_PrmReadOneItem(FLH_PRM_TRACE_ENABLE,FLH_PRM_TRACE_ENABLE_LEN,fbuf);
    if(fbuf[0]==0)
    {
        return;
    }

    YS_PrmReadOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);
    PosDelaySet=fbuf[0]*256+fbuf[1];

    if (t_SysRunStatus.AccStatus == 0)//ACC�ر� λ�ñ���ΪACC���ϴ�ʱ������6��
    {
        PosDelaySet=PosDelaySet*6;
    }
    t_FlowInfo.PosDelay++;
    if(t_FlowInfo.PosDelay >= 5)
    {
        t_FlowInfo.PosDelay=0;
//        YS_GprsServerSendInterface(SERV_UP_CMD_POS, NULL,0);
        if (YS_GprsServerSendInterface(SERV_UP_CMD_POS, NULL,0) == FALSE)
        {
            t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
        }

    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunIdleCANCtrl
����˵����CAN���ݰ����ʹ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunIdleCANCtrl(void)
{
    u16	PosDelaySet;
    u8 fbuf[4];

    YS_PrmReadOneItem(FLH_PRM_CAN_ENABLE,FLH_PRM_CAN_ENABLE_LEN,fbuf);
    if(fbuf[0]==0)
    {
        return;
    }

    YS_PrmReadOneItem(FLH_PRM_CAN_TIME,FLH_PRM_CAN_TIME_LEN,fbuf);
    PosDelaySet=fbuf[0]*256+fbuf[1];
    t_FlowInfo.ObdDelay++;
    if(t_FlowInfo.ObdDelay >= 5)
    {
        t_FlowInfo.ObdDelay=0;
//        YS_GprsServerSendInterface(SERV_UP_CMD_CAN, NULL,0);
        if (YS_GprsServerSendInterface(SERV_UP_CMD_CAN, NULL,0) == FALSE)
        {
            t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
        }
//        if (YS_OBDGetFault()>0)
//        {
//            YS_GprsServerSendInterface(SERV_UP_CMD_FAULT, NULL,0);
//        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunAppSocketBeing
����˵��������Ӧ�ó���SOCKET
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_RunAppSocketBeing(void)
{
    u8 fbuf[40],IPBuf[4];
    u16 i,len,count,Port;
    char StrDat[10];
    u8 rlt;
    if(t_FlowInfo.DnsAddrFlag==FALSE)  //����IP ��ʽ
    {
        YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
        count=0;

        for(i=0; i<4; i++)
        {
            count=YS_CodeGetItemInBuf(fbuf,len,(u8 *)StrDat,i,'.',4);
            StrDat[count]=0;
            IPBuf[i]=atoi(StrDat);
        }
    }
    else //����������ʽ
    {
        for(i=0; i<4; i++)
        {
            IPBuf[i]=t_FlowInfo.DnsIPInfo[i];
        }
    }
    YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
    Port=fbuf[0]*256+fbuf[1];
    rlt=sjfun_Socket_Create(t_FlowInfo.AcctID, &t_FlowInfo.SocketID, IPBuf,Port,0);
    return(rlt);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSocketEventHandler
����˵����SOCKET �����¼�������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunSocketEventHandler(U8 SocketID, U8 EventID, U8 *dbuf, U16 dlen)
{

    if(SocketID==t_FlowInfo.SocketID) //Ӧ��ƽ̨����
    {
        if(EventID==SOC_CONNECT)
        {
            YS_UartDebugString("set soc ok flag");
            t_FlowInfo.SocketRlt=TRUE;
        }
        else if(EventID==SOC_READ)
        {
            YS_RunSocketDataInc(DATA_FLAG_REC_ADD);
            YS_GprsGpsServerDataInput(dbuf,dlen);
        }
        else if (EventID == SOC_CLOSE)
        {
            sjfun_Socket_Close(t_FlowInfo.SocketID);
            t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
        }
    }
    else if(SocketID==t_FlowInfo.DWSckID) //����ƽ̨����
    {
        if(EventID==SOC_CONNECT)
        {
//            YS_DWAppSocketAck();
        }
        else if(EventID==SOC_READ)
        {
            YS_RunSocketDataInc(DATA_FLAG_REC_ADD);
//            YS_DWAppServerDataInput(dbuf,dlen);
        }
    }
    else if(SocketID==t_FlowInfo.AGpsSckID)
    {
        if(EventID==SOC_CONNECT)
        {
            YS_AGpsServerConnectMsgDeal();
        }
        else if(EventID==SOC_READ)
        {
            YS_RunSocketDataInc(DATA_FLAG_REC_ADD);
            YS_AGpsServerDataParase(dbuf,dlen);
        }
    }
    else if(SocketID==t_FlowInfo.WebSckID)
    {
        if(EventID==SOC_CONNECT)
        {
            YS_GprsDebugString("web socket msg rec",0);
            YS_WebServiceSckConnectMsg();
        }
        else if(EventID==SOC_READ)
        {
            YS_RunSocketDataInc(DATA_FLAG_REC_ADD);
            YS_WebServiceDataParase(dbuf,dlen);
        }
        else if (EventID == SOC_CLOSE)
        {
            YS_GprsDebugString("web socket close",0);
            YS_WebSocketClose();
        }
    }
    else if(SocketID==t_FlowInfo.AutoUpdateSckID) //�Զ�����ƽ̨����
    {
        if(EventID==SOC_CONNECT)
        {
//            YS_AutoUpdateSocketAck();
        }
        else if(EventID==SOC_READ)
        {
            YS_RunSocketDataInc(DATA_FLAG_REC_ADD);
//            YS_DWAppServerDataInput(dbuf,dlen);	//��������ƽ̨��������
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunTraceInterFace
����˵����ƽ̨�·�TRACE ����ڳ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunTraceInterFace(u16 TraceTimes, u16 TraceDelay)
{
    t_ysTraceManage.MoniFlag=TRUE;
    t_ysTraceManage.SendSetTimes=TraceTimes;
    t_ysTraceManage.SendSetDelay=TraceDelay;
    t_ysTraceManage.SendCountDelay=TraceDelay;
    t_ysTraceManage.SendCountTimes=0;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunEntrySleepMode
����˵�����������߹���ģʽ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunEntrySleepMode(void)
{
    u8 fbuf[4];

    YS_PrmReadOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf); //��ȡ���߿���ʹ��
    if((fbuf[0]==0)||(t_SysRunStatus.FactoryMode==TRUE))  //��ֹ���߿���
    {
        return;
    }

    sjfun_Sys_Sleep_Enable();						//����MTK ��������ģʽ
    t_SysRunStatus.SleepStatus=1; 				//��������״̬

    sjfun_VmcSignControl(0);
    sjfun_Gps_Stop(TRUE);  						//GPS ����ģ��ر�
    t_FlowInfo.GpsPowerFlag=FALSE;
    t_FlowInfo.OptDelay=0;
    t_FlowInfo.SeverRegErrTime=0;
    t_FlowInfo.GpsPSMStatue=FALSE;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSleepEntryWakeupMode
����˵�������뻽�ѹ���ģʽ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunEntryWakeupMode(void)
{
    sjfun_Sys_Sleep_Disable(); //��ֹMTK ��������ģʽ
    if(t_SysRunStatus.SleepStatus==1) //���ϵͳ�ѽ�������״̬
    {
        t_FlowInfo.GpsPowerFlag=TRUE;
        sjfun_VmcSignControl(1);
        sjfun_Gps_Start(TRUE);  //GPS ����ģ�鿪��
        if(t_SysRunStatus.RunFlow==YS_RUN_FLOW_IDLE_DEAL)  //���ϵͳ�Ѵ�����������״̬������AGPS����
        {
            YS_AGpsDealInterFace();
        }
        sjfun_timer(GIS_TIMER_ID_4,200,YS_IODealLedTimerHandler);
    }
    t_SysRunStatus.SleepStatus=0;
    t_FlowInfo.SleepCount=0;
    t_FlowInfo.PowSleepCount=0;
    t_FlowInfo.VibSleepCount = 0;
}

void YS_EntryGsmSleepMode(void)//bool flag)
{
    u8 fbuf[4];

    YS_PrmReadOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf); //��ȡ���߿���ʹ��
    if((fbuf[0]==0)||(t_SysRunStatus.FactoryMode==TRUE))  //��ֹ���߿���
    {
        return;
    }

    t_SysRunStatus.GsmSleepStatus=1;
    if(t_SysRunStatus.SleepStatus==0)
    {
        YS_RunEntrySleepMode();
    }

    t_FlowInfo.SleepDelayWait=0;
    t_SysRunStatus.RunFlow=YS_RUN_FLOW_FIGHT_BEGIN;
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)"ENTRY GSM SLEEP", sizeof("ENTRY GSM SLEEP"));
}

void YS_ExitGsmSleepMode(void)
{
    t_SysRunStatus.GsmSleepStatus=0;
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)"EXIT GSM SLEEP", sizeof("EXIT GSM SLEEP"));
}


/*-----------------------------------------------------------------------------------------
��������YS_RunWebSocketInterFace
����˵��������λ������SOCKET �����ӿ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_RunWebSocketInterFace(void)
{
    u8 IPBuf[5];
    u16 Port;

//120.24.255.230:80
    IPBuf[0]=120;
    IPBuf[1]=24;
    IPBuf[2]=255;
    IPBuf[3]=230;
    IPBuf[4]=0x00;

    Port=80;
    sjfun_Socket_Create(t_FlowInfo.AcctID, &t_FlowInfo.WebSckID,IPBuf, Port,0);
    return TRUE;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunWebSocketClose
����˵��������λ������ر�SOCKET
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunWebSocketClose(void)
{
    sjfun_Socket_Close(t_FlowInfo.WebSckID);
    t_FlowInfo.WebSckID=SOCKET_ID_INIT_VALUE;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunAutoRstCtrl
����˵����ϵͳ�Զ���λ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
//AVL_AUTO_RESET_COUNT
void YS_RunAutoRstCtrl(void)
{

}

/*-----------------------------------------------------------------------------------------
��������YS_RunNetLostRstCtrl
����˵��������ע���쳣��λ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/

void YS_RunNetLostRstCtrl(void)
{
    static u8 BankNetStatus=0;
    static bool AllowRst=TRUE;

    if((t_SysRunStatus.RunFlow == YS_RUN_FLOW_FIGHT_WAIT)
            ||(t_SysRunStatus.RunFlow == YS_RUN_FLOW_FIGHT_BEGIN))
    {
        BankNetStatus=0;
        //AllowRst=TRUE;
    }
    else
    {
        if(BankNetStatus==0) //���������ĵ�һ������ע��״̬
        {
            if(t_SysRunStatus.RegStatus==1)
            {
                BankNetStatus=1;
            }
        }
        else if(AllowRst==TRUE) //�����������ע�����磬�����е���ֱ������������
        {
            if((t_SysRunStatus.RegStatus==0)&&(AllowRst==TRUE))
            {
                AllowRst=FALSE;
                YS_UartDebugString("RST BY LOST NET");
                YS_SysRsqSystemRST(YS_RST_FLAG_LOSE_NET);
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_RST_EXITWAIT;
            }
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSleepCtrl
����˵��������״̬����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunSleepCtrl(void)
{
    u8	fbuf[10];
    u16	SleepSetTime;

    YS_PrmReadOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf); //��ȡ���߿���ʹ��
    if(fbuf[0]==0)  //��ֹ���߿���
    {
        return;
    }

    if(t_SysRunStatus.SleepStatus==0)  //����״̬
    {
        if (t_SysRunStatus.AccStatus == 0)
        {
            YS_PrmReadOneItem(FLH_PRM_SLEEP_TIME,FLH_PRM_SLEEP_TIME_LEN,fbuf); //�Զ�����ʱ��
            SleepSetTime=fbuf[0]+fbuf[1];

            t_FlowInfo.VibSleepCount++;
            if(t_FlowInfo.VibSleepCount>=10)
            {
                t_FlowInfo.VibSleepCount = 0;
                YS_RunEntrySleepMode();
            }
        }
    }
    else
    {
        if (t_SysRunStatus.AccStatus == 1)
        {
            YS_RunEntryWakeupMode();
            t_FlowInfo.VibSleepCount =0;
        }
    }


}

/*-----------------------------------------------------------------------------------------
��������YS_RunMainIPString
����˵�����ж���IP����������IP��ַ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_RunMainIPString(void)
{
    char ServIP[FLH_PRM_SERV_ADDR_LEN+2];
    u8	fbuf[FLH_PRM_SERV_ADDR_LEN];
    u16 i,flen;
    bool Result;

    YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
    flen=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
    for(i=0; i<flen; i++)
    {
        ServIP[i]=fbuf[i];
    }
    ServIP[flen]=0;
    Result=YS_CodeTestIPString(ServIP);
    return(Result);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetDiaIPFromString
����˵���������õ�IP �ַ�����ȡ��IPV4����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_RunGetDiaIPFromString(u8 *IPBuf)
{
    u8 fbuf[40];
    u16 i,len,count;
    char StrDat[10];

    if(t_FlowInfo.DnsAddrFlag==FALSE) //���������ΪIP��ʽ
    {
        YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
        count=0;

        for(i=0; i<4; i++)
        {
            count=YS_CodeGetItemInBuf(fbuf,len,(u8 *)StrDat,i,'.',4);
            StrDat[count]=0;
            IPBuf[i]=atoi(StrDat);
        }
    }
    else
    {
        for(i=0; i<4; i++)
        {
            IPBuf[i]=t_FlowInfo.DnsIPInfo[i];
        }
    }
    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunDnsInterface
����˵������ƽ̨����������������ں���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_RunDnsInterface(void)
{
    u8 fbuf[50],DnsUrl[50];
    u16 flen,i;
    u8 Rlt;

    YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
    flen=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
    for(i=0; i<flen; i++)
    {
        DnsUrl[i] = fbuf[i];
    }
    DnsUrl[i] = 0;
    Rlt = sjfun_Dns_ParaseInterface((char*)DnsUrl, t_FlowInfo.AcctID,t_FlowInfo.DnsIPInfo,1);
    return Rlt;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunLoginServerInit
����˵������¼ƽ̨��ʼ������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunLoginServerInit(void)
{
    t_FlowInfo.LogOKFlag=FALSE;
    t_FlowInfo.LogFailFlag=FALSE;
    t_FlowInfo.LogTimes=0;
    t_FlowInfo.LogDelay=RUN_LOGIN_DELAY_DEF;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunLoginServerAck
����˵������¼ƽ̨Ӧ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunIdelModeInit(void)
{
    t_FlowInfo.PosDelay=0;
    t_FlowInfo.PosTimes=0;
    t_FlowInfo.HeartDelay=30;
    t_FlowInfo.HeartTimes=0;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunTraceInit
����˵������ʼ��TRACE ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunTraceInit(void)
{
    t_ysTraceManage.MoniFlag=FALSE;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGprsReleaseWarn
����˵�������GPRS ������Ŀ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunGprsReleaseWarn(void)
{
    u8 i;
    t_FlowInfo.WarnFlag=0;				//����ϱ�״̬
    for(i=0; i<16; i++)					//��������־
    {
        t_SysRunStatus.WarnStatus[i]=0;
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGprsWarnCtrl
����˵����GPRS�������ʹ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunGprsWarnCtrl(void)
{
    if(t_FlowInfo.WarnFlag==1)
    {
        if(t_FlowInfo.WarnTimes<3)
        {
            t_FlowInfo.WarnDelay++;
            if(t_FlowInfo.WarnDelay>=RUN_WARN_DELAY_DEF)
            {
                t_FlowInfo.WarnDelay=0;
                t_FlowInfo.WarnTimes++;
                if(t_SysRunStatus.WarnStatus[FJYD_WARN_ID_BATTLOW]==1)
                {
                    YS_UartDebugInterfacel(0X8110,(u8*)"SEND POF WARN",strlen("SEND POF WARN"));
//					YS_GprsServerSendInterface(SERV_UP_WARN_BATTLOW_ID,NULL,0);
                }
                if(t_SysRunStatus.WarnStatus[FJYD_WARN_ID_POSOUT]==1)
                {
                    YS_UartDebugInterfacel(0X8110,(u8*)"SEND POSOUT WARN",strlen("SEND POSOUT WARN"));
                }
                if(t_SysRunStatus.WarnStatus[FJYD_WARN_ID_DISMATLE]==1)
                {
//					YS_GprsServerSendInterface(SERV_UP_WARN_LOST_ID,NULL,0);
                }
            }
        }
        else
        {
            YS_RunGprsReleaseWarn();
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunTraceCtrl
����˵��������TRACE �Ĵ������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunTraceCtrl(void)
{
    if(t_ysTraceManage.MoniFlag==TRUE)
    {
        t_ysTraceManage.SendCountDelay++;
        if(t_ysTraceManage.SendCountDelay>=t_ysTraceManage.SendSetDelay)
        {
            t_ysTraceManage.SendCountDelay=0;
            t_ysTraceManage.SendCountTimes++;
            if(t_ysTraceManage.SendCountTimes<=t_ysTraceManage.SendSetTimes)
            {
                YS_GprsServerSendInterface(SERV_UP_CMD_ONEGPSACK, NULL,0); //�ϱ�һ��TRACE ����
            }
            else
            {
                t_ysTraceManage.MoniFlag=FALSE;
            }
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunPPPInterFaceDeal
����˵����ϵͳGPRS PPP��ں���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u32 YS_RunPPPInterFaceDeal(void)
{
    u32 Rlt;
    u8 fbuf[30],i,len;
    char apn[30];
    char ApnUser[30];
    char ApnPassWord[30];

    YS_PrmReadOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,fbuf); //SLEEP ENABLE
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_PPP_APN_LEN);
    for(i=0; i<len; i++)
    {
        apn[i]=fbuf[i];
    }
    apn[len]=0;
    ApnUser[0]=0;
    ApnPassWord[0]=0;
    Rlt=sjfun_PPP_Interface(apn,ApnUser,ApnPassWord);
    return(Rlt);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunWarnSignInput
����˵�������������ϱ�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunWarnSignInput(u8 WarnID)
{
    t_SysRunStatus.WarnStatus[WarnID]=1;
    t_FlowInfo.WarnFlag=1;
    t_FlowInfo.WarnTimes=0;
    t_FlowInfo.WarnDelay=RUN_WARN_DELAY_DEF;
}

void YS_RunDwUpdataSmsCrtl(void)
{
    u8 fbuf[20];
    u8 fbufLen;

    YS_PrmReadOneItem(FLH_PRM_DW_PHONE,FLH_PRM_DW_PHONE_LEN,fbuf);
    fbufLen = YS_CodeBufRealLen(fbuf,FLH_PRM_DW_PHONE_LEN);

    if((fbufLen!=0)&&(t_SysRunStatus.RegStatus==1))
    {
        YS_PrmReadOneItem(FLH_PRM_NV_DWFLAG,FLH_PRM_NV_DWFLAG_LEN,fbuf);
        if(fbuf[0]==0)//&&(FirstRun==1))
        {
//            YS_DWUpdateResultDeal(DW_RLT_UPDATE_SUCCESS);
        }
    }
    return;
}


/*-----------------------------------------------------------------------------------------
��������YS_RunRegServerCtrl
����˵����ע��ƽ̨����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunRegServerCtrl(void)
{
    t_FlowInfo.RegDelay++;
    if(t_FlowInfo.RegDelay>=RUN_LOGIN_DELAY_DEF)
    {
        t_FlowInfo.RegDelay=0;
        t_FlowInfo.RegTimes++;
        if(t_FlowInfo.RegTimes<=RUN_LOGIN_TIMES_DEF)
        {
            YS_GprsDebugString("SEND Reg Package",0);
            YS_GprsServerSendInterface(SERV_UP_CMD_REG,NULL,0);
        }
        else
        {
            t_FlowInfo.RegFailFlag=TRUE;
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunRegServerAck
����˵����ע��ƽ̨Ӧ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunRegServerAck(void)
{
    u8 fbuf[5];

    fbuf[0]=FJYD_TERI_STATUS_OK;
    YS_PrmWriteOneItem(FLH_PRM_ACTIVE_FLAG, FLH_PRM_ACTIVE_FLAG_LEN, fbuf);
    t_FlowInfo.RegOKFlag=TRUE;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunLoginServerCtrl
����˵������¼ƽ̨����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunLoginServerCtrl(void)
{
    t_FlowInfo.LogDelay++;
    if(t_FlowInfo.LogDelay>=RUN_LOGIN_DELAY_DEF)
    {
        t_FlowInfo.LogDelay=0;
        t_FlowInfo.LogTimes++;
        if(t_FlowInfo.LogTimes<=RUN_LOGIN_TIMES_DEF)
        {
            YS_GprsDebugString("SEND Log Package",0);
            YS_GprsServerSendInterface(SERV_UP_CMD_LOG, NULL, 0);
//            t_FlowInfo.LogOKFlag=TRUE;
        }
        else
        {
//            YS_RunSocketClose();
            t_FlowInfo.LogFailFlag=TRUE;
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunMotorCtrlInterface
����˵����������Ƴ�ʼ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunMotorCtrlInterface(void)
{

}

/*-----------------------------------------------------------------------------------------
��������YS_RunMotorCtrl
����˵�����������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunMotorCtrl(void)
{

}

/*-----------------------------------------------------------------------------------------
��������YS_MotorCtrl
����˵�������Ƽ̵���IO�ڵ�ƽ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_MotorIOCtrl(bool lever)
{

}

void YS_RunUpdateRequest(void)
{
    if ((t_SysRunStatus.AccStatus ==TRUE)&&(t_FlowInfo.BankAccStatus==FALSE))
    {
        t_FlowInfo.BankAccStatus = TRUE;
        YS_WebAddRequest();
    }
    else if((t_SysRunStatus.AccStatus ==FALSE)&&(t_FlowInfo.BankAccStatus==TRUE))
    {
        t_FlowInfo.BankAccStatus = FALSE;
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunAppWorkFlowManage
����˵����ϵͳ����ʱ�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunAppWorkFlowManage(void)
{
    u8 Rlt;
    static bool FirstRegFlag=TRUE;
    static u32 n=0;
    u8 fbuf[40];
    u8 DnsRlt;

    YS_IOInputInfoDeal();
    YS_OBDRstDeal();
    YS_WebServiceRsqFlow();
    YS_RunAutoRstCtrl(); 			//��ʱ��λ����
//    YS_AGpsGetRunFlow();
    YS_SmsSendComMoniCtrl();
    YS_SysGetBarCodeCtrl();
    YS_RunDwUpdataSmsCrtl();
    YS_RunSleepCtrl();  				//���߻��ѿ���
    YS_FactoryMode();

    switch(t_SysRunStatus.RunFlow)
    {
        case YS_RUN_FLOW_INIT: 			//ϵͳ��ʼ��
            ycsj_debug("YS_RUN_FLOW_INIT1");
            sjfun_Begin_RsqNetInfo();
            t_FlowInfo.OptDelay=0;
//            t_SysRunStatus.RunFlow=YS_RUN_FLOW_INIT;
            t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_REG;
            break;

        case YS_RUN_FLOW_WAIT_REG: //����ע��ȴ�
#if 0
            if(t_SysRunStatus.RegStatus==1)
            {
                YS_WebAddRequest();
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_RST_EXITWAIT;
//                t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_PPP;
            }
#else
            if(t_SysRunStatus.RegStatus==1)
            {
                ycsj_debug("YS_RUN_FLOW_WAIT_REG OK\n");
                if(FirstRegFlag==TRUE)
                {
                    sjfun_req_mlocate_ncinfo();	//�����ȡ���վ��Ϣ
                    FirstRegFlag=FALSE;
                }

                if(sjfun_SkyLine_Mode()==0)		//����������ߵ���ģʽ
                {
                    t_SysRunStatus.RunFlow=YS_RUN_FLOW_PPP_BEIGN;
                }
            }
            else
            {
                t_FlowInfo.OptDelay++;
                if(FirstRegFlag==TRUE)  //��һ�ο�������
                {
                    if(t_FlowInfo.OptDelay==1)  //��ֹ�ڸ�λǰϵͳ�Ѵ��ڷ���ģʽ
                    {
                        sjfun_Flight_Mode_Cut(RUN_MODE_NORMAL); //��ϵͳת����������ģʽ
                    }

                    if(t_FlowInfo.OptDelay>=3600) //����һСʱ��ע����ֱ�Ӹ�λ
                    {
                        t_FlowInfo.OptDelay=10;
                        YS_SysRsqSystemRST(YS_RST_FLAG_REG);
                        t_SysRunStatus.RunFlow=YS_RUN_FLOW_RST_EXITWAIT;
                    }
                    else if(t_FlowInfo.OptDelay>=REG_FULL_TIME_DFE)  //��������3���ӣ�����ź�ֵ����15ֱ�Ӹ�λ
                    {
                        if(t_SysRunStatus.CsqValue>=15)
                        {
                            t_FlowInfo.OptDelay=10;
                            t_SysRunStatus.RunFlow=YS_RUN_FLOW_RST_EXITWAIT;
                        }
                    }
                }
            }
#endif
            break;

        case YS_RUN_FLOW_PPP_BEIGN: //��ʼ����PPP ����
            ycsj_debug("YS_RUN_FLOW_PPP_BEIGN\n");
            t_SysRunStatus.PPPTimes++;
            t_FlowInfo.AcctID = YS_RunPPPInterFaceDeal();
            t_FlowInfo.OptDelay=0;
            t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_PPP;
            break;

        case YS_RUN_FLOW_WAIT_PPP: //PPPǿ���ӵȴ�
            t_FlowInfo.OptDelay++;
            if(t_FlowInfo.OptDelay>=5)
            {
                if(YS_RunMainIPString()==TRUE)
                {
                    t_FlowInfo.DnsAddrFlag=FALSE;
                    t_SysRunStatus.RunFlow=YS_RUN_FLOW_SOCKET_BEGIN;
                }
                else //ƽ̨��ַΪ��IP ��ʽ������DNS ���н���
                {
                    t_FlowInfo.DnsAddrFlag=TRUE;
                    t_SysRunStatus.RunFlow=YS_RUN_FLOW_DNS_BEGIN;
                }
            }
            break;

        case YS_RUN_FLOW_DNS_BEGIN: //DNS�������
            t_FlowInfo.DnsOKFlag=FALSE;
            t_FlowInfo.OptDelay=0;

            if(t_FlowInfo.AppIPExsitFlag==TRUE)
            {
                YS_UartDebugString("App Dns ok with ip exist");
                t_FlowInfo.DnsErrTimes=0;
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_SOCKET_BEGIN;
            }
            else
            {
                DnsRlt=YS_RunDnsInterface();
                YS_UartDebugString("YS_RUN_FLOW_DNS_BEGIN");
                switch(DnsRlt)
                {
                    case DNS_RLT_OK_WAIT:	//�ɹ�����ȴ���Ƶ�ص�
                        t_FlowInfo.DnsErrTimes=0;
                        t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_DNS;
                        break;

                    case DNS_RLT_OK_GETIP:	//�ɹ����ѻ�ȡ��IP
                        t_SysRunStatus.RunFlow=YS_RUN_FLOW_SOCKET_BEGIN;
                        break;

                    case DNS_RLT_FAIL://���ܵ����쳣
                    default:
                        t_FlowInfo.DnsErrTimes++;
                        if(t_FlowInfo.DnsErrTimes<=3)
                        {
                            t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
                        }
                        else
                        {
                            YS_SysRsqSystemRST(YS_RST_FLAG_LOSE_NET);
//                            YS_PrmReadOneItem(FLH_PRM_FLI_ENABLE,FLH_PRM_FLI_ENABLE_LEN,fbuf);
//                            if(fbuf[0]==1)
//                            {
//                                YS_EntryGsmSleepMode();
//                            }
//                            else
//                            {
//                                YS_RunEntrySleepMode();	//��������ģʽ
//                            }
                        }
                        break;
                }
            }
            break;

        case YS_RUN_FLOW_WAIT_DNS: //DNS�����ȴ�
            if(t_FlowInfo.DnsOKFlag==TRUE)  //�첽��Ϣ�ѳɹ�����
            {
                t_FlowInfo.DnsErrTimes=0;
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_SOCKET_BEGIN;
            }
            else
            {
                t_FlowInfo.OptDelay++;
                if(t_FlowInfo.OptDelay>=RUN_DNS_DELAY_DEF)
                {
                    t_FlowInfo.DnsErrTimes++;
                    if(t_FlowInfo.DnsErrTimes<=3)
                    {
                        t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
                    }
                    else
                    {
                        YS_PrmReadOneItem(FLH_PRM_FLI_ENABLE,FLH_PRM_FLI_ENABLE_LEN,fbuf);
                        if(fbuf[0]==1)
                        {
                            YS_EntryGsmSleepMode();
                        }
                        else
                        {
                            YS_RunEntrySleepMode();	//��������ģʽ
                        }
                    }
                }
            }
            break;

        case YS_RUN_FLOW_SOCKET_BEGIN: //����SOCKET ���

            t_FlowInfo.OptDelay=0;
            t_FlowInfo.SocketRlt=FALSE;
            Rlt=YS_RunAppSocketBeing();
            ycsj_debug("App Socket rlt:%d", Rlt);
            if(Rlt==0)
            {
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_SOCKET;
            }
            else
            {
                t_FlowInfo.RstCodeFlag=YS_RST_FLAG_SOCKET_FULLTIMES;
                t_FlowInfo.RstDelayCount=0;
                YS_SysRsqSystemRST(t_FlowInfo.RstCodeFlag);
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_RST_EXITWAIT;
            }
            break;

        case YS_RUN_FLOW_WAIT_SOCKET: //����SOCKET �ȴ�
            if(t_FlowInfo.SocketRlt==TRUE)
            {
                t_FlowInfo.SocOk = 1;
                t_FlowInfo.SocketErrTimes=0;
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_SEVLOG_BEGIN;
//                t_SysRunStatus.RunFlow=YS_RUN_FLOW_REGSERVER_BEGIN;
            }
            else
            {
                t_FlowInfo.OptDelay++;
                if(t_FlowInfo.OptDelay>=RUN_SOCKET_DELAY_DEF)
                {
                    t_FlowInfo.AppIPExsitFlag=FALSE;
                    t_FlowInfo.SocketErrTimes++;
                    if (3 <= t_FlowInfo.SocketErrTimes)
                    {
                        YS_PrmReadOneItem(FLH_PRM_FLI_ENABLE,FLH_PRM_FLI_ENABLE_LEN,fbuf);
                        if(fbuf[0]==1)
                        {
                            YS_EntryGsmSleepMode();
                        }
                        else
                        {
                            YS_RunEntrySleepMode();	//��������ģʽ
                        }
                    }
                    else
                    {
                        sjfun_Socket_Close(t_FlowInfo.SocketID);
                        t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
                    }
                }
            }
            break;

        case YS_RUN_FLOW_REGSERVER_BEGIN: //ע�����������
            YS_PrmReadOneItem(FLH_PRM_ACTIVE_FLAG, FLH_PRM_ACTIVE_FLAG_LEN, fbuf);
            if(FJYD_TERI_STATUS_OK==fbuf[0])
            {
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_SEVLOG_BEGIN;
            }
            else
            {
                YS_GprsDebugString("YS_RUN_FLOW_SEVREG_BEGIN",0);
                t_FlowInfo.RegOKFlag=FALSE;
                t_FlowInfo.RegFailFlag=FALSE;
                t_FlowInfo.RegTimes=0;
                t_FlowInfo.RegDelay=RUN_LOGIN_DELAY_DEF;
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_SERVREG;
            }
            break;

        case YS_RUN_FLOW_WAIT_SERVREG:  //ע��������ȴ�
            if(t_FlowInfo.RegOKFlag==TRUE)
            {
                t_FlowInfo.ServRegErrTimes = 0;
                YS_GprsDebugString("YS_RUN_FLOW_SEVLOG  OK",0);
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_SEVLOG_BEGIN;
            }
            else if(t_FlowInfo.RegFailFlag==TRUE)
            {
                t_FlowInfo.ServRegErrTimes ++;
                if (3 <= t_FlowInfo.ServRegErrTimes)
                {
                    //��λ����
                    YS_SysRsqSystemRST(YS_RST_FLAG_SER_LOG_FAILLTIMES);
                }
                YS_GprsDebugString("YS_RUN_FLOW_SEVLOG  FAIL",0);
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
            }
            else
            {
                YS_RunRegServerCtrl();
            }
            break;

        case YS_RUN_FLOW_SEVLOG_BEGIN://��¼�������
            YS_GprsDebugString("YS_RUN_FLOW_SEVLOG_BEGIN",0);
            YS_RunLoginServerInit();
            t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_SERVLOG;
            break;

        case YS_RUN_FLOW_WAIT_SERVLOG: //��¼����������
            if(t_FlowInfo.LogOKFlag==TRUE)
            {
                t_FlowInfo.ServLogErrTimes = 0;
                YS_GprsDebugString("YS_RUN_FLOW_SEVLOG OK", 0);
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_IDLE_INIT;
            }
            else if(t_FlowInfo.LogFailFlag==TRUE)
            {
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_REGSERVER_BEGIN;
                fbuf[0]=FJYD_TERI_STATUS_INIT;
                YS_PrmWriteOneItem(FLH_PRM_ACTIVE_FLAG,FLH_PRM_ACTIVE_FLAG_LEN,fbuf);
                YS_GprsDebugString("YS_RUN_FLOW_SEVLOG  FAIL",0);
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
            }
            else
            {
                YS_RunLoginServerCtrl();
            }
            break;

        case YS_RUN_FLOW_IDLE_INIT://׼������IDLE ģʽ
            YS_GprsDebugString("YS_RUN_FLOW_IDLE_INIT ",0);
            YS_RunIdelModeInit();
            YS_RunTraceInit();
//            YS_AGpsDealInterFace();
            t_SysRunStatus.RunFlow=YS_RUN_FLOW_IDLE_DEAL;
            YS_WebAddRequest();
            break;

        case YS_RUN_FLOW_IDLE_DEAL: 	//ϵͳIDLEģʽ����
            YS_RunIdleHeartCtrl();
//            YS_RunGpsPSMCtrl();
//            YS_RunGprsWarnCtrl();		//�����ϱ�����
//            YS_RunTraceCtrl();			//�ϱ�TRACE ����
            YS_RunIdlePosCtrl();	//��ʱ�����ϱ�����
            YS_RunIdleCANCtrl();
//            if(t_FlowInfo.HeartTimes>=RUN_HEART_TIMES_DEF)
//            {
//                sjfun_Socket_Close(t_FlowInfo.SocketID);
//                t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
//            }
            if (t_SysRunStatus.SleepStatus==1)
            {
//                t_SysRunStatus.RunFlow=YS_RUN_FLOW_FIGHT_BEGIN;
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_SLEEP_BEGIN;
            }
            break;

        case YS_RUN_FLOW_RDCON_BEGIN: //��ʼ��������
            YS_GprsDebugString("YS_RUN_FLOW_RDCON_BEGIN ",0);
            t_FlowInfo.OptDelay=0;
            t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_WAIT;
            break;

        case YS_RUN_FLOW_RDCON_WAIT: //����ƽ̨�ȴ�
            t_FlowInfo.OptDelay++;
            if(t_FlowInfo.OptDelay>=RUN_RECON_DELAY_DEF)
            {
                YS_GprsDebugString("YS_RUN_FLOW_RDCON OK ",0);
                t_FlowInfo.OptDelay=0;
                if(t_SysRunStatus.RegStatus==1) //���������ע������
                {
                    t_SysRunStatus.RunFlow=YS_RUN_FLOW_PPP_BEIGN;
                }
                else //���ǵ�ǰδע������
                {
                    t_FlowInfo.OptDelay=REG_FULL_TIME_DFE-60; //����ն����й����е�����1���Ӻ�λ
                    t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_REG;
                }
            }
            break;
        case YS_RUN_FLOW_SLEEP_BEGIN: //��ʼ��������ģʽ
            t_FlowInfo.SleepDelayWait++;
            if(t_FlowInfo.SleepDelayWait>=3)
            {
                if(t_FlowInfo.SocOk == 1)
                {
                    sjfun_Socket_Close(t_FlowInfo.SocketID);
                }
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_SLEEP_WAIT;
            }
            break;

        case YS_RUN_FLOW_SLEEP_WAIT: //����ģʽ�ȴ�
            YS_GpsSetPosStatusNot();
            if(t_SysRunStatus.SleepStatus == 0)
            {
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_PPP_BEIGN;
            }
            break;

        case YS_RUN_FLOW_RST_WAITING: //�ն˸�λ�ȴ���ʱ
            if(t_GprsWarnFlag.NewWarnFlag==TRUE) //������µı���״̬����,���ٸ�λ
            {
                t_GprsWarnFlag.NewWarnFlag=FALSE;
                YS_SysRsqSystemRST(t_FlowInfo.RstCodeFlag);
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_RST_EXITWAIT;
            }
            else //����1Сʱ��ֱ�Ӹ�λ
            {
                t_FlowInfo.RstDelayCount++;
                if(t_FlowInfo.RstDelayCount>=3600)
                {
                    YS_SysRsqSystemRST(t_FlowInfo.RstCodeFlag);
                    t_SysRunStatus.RunFlow=YS_RUN_FLOW_RST_EXITWAIT;
                }
            }
            break;

        case YS_RUN_FLOW_FIGHT_BEGIN:
            t_FlowInfo.SleepDelayWait++;
            if(t_FlowInfo.SleepDelayWait>=3)
            {
                if(t_FlowInfo.SocOk == 1)
                {
                    sjfun_Socket_Close(t_FlowInfo.SocketID);
                }
                sjfun_Flight_Mode_Cut(RUN_MODE_FLIGHT);
                t_SysRunStatus.RunFlow = YS_RUN_FLOW_FIGHT_WAIT;
                t_FlowInfo.OptDelay=0;
            }
            break;

        case YS_RUN_FLOW_FIGHT_WAIT:
            YS_GpsSetPosStatusNot();
            if(t_SysRunStatus.GsmSleepStatus==0)
            {
                YS_RunEntryWakeupMode();
                sjfun_Flight_Mode_Cut(RUN_MODE_NORMAL);
                t_SysRunStatus.RunFlow=YS_RUN_FLOW_PPP_BEIGN;
            }
            break;

        case YS_RUN_FLOW_RST_EXITWAIT: //�ȴ���λ�ӿ���ɸ�λ����
            t_FlowInfo.RstDelayCount++;
            break;

        default:
            break;
    }
}

void YS_RunSetFlowRst(void)
{
    sjfun_Socket_Close(t_FlowInfo.SocketID);
    t_SysRunStatus.RunFlow = YS_RUN_FLOW_RST_EXITWAIT;
}

//�ͻ��˳�������ʱ��������
void YS_RunAppMainTimerHandler(void)
{
    //static U16 DelayCount=0;
    static U8 RunModeGet=1;
    static U8 RunModeFlag=0;
    static U8 Count=0;
    u8 fbuf[4];

    YS_RunGetSysStatusFormPlat();
    YS_SysRstManageCtrl();
    YS_SmsDealTimerInput();
    YS_RunAppWorkFlowManage();
    sjfun_timer(GIS_TIMER_ID_3,1000,YS_RunAppMainTimerHandler);
}

void YS_RunGetHostNameDebug(char *HitStr,u8 *dbuf)
{
    char DispBuf[100];

    sprintf(DispBuf,"%s:%d.%d.%d.%d",HitStr,dbuf[0],dbuf[1],dbuf[2],dbuf[3]);
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8 *)DispBuf,strlen(DispBuf));
}

/*-----------------------------------------------------------------------------------------
��������YS_RunDnsEventHandler
����˵���������������¼�������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunDnsEventHandler(u8 status, u8 *IPBuf)
{
    u8 i;

    if(status==1)
    {
        t_FlowInfo.DnsOKFlag=1;
        for(i=0; i<4; i++)
        {
            t_FlowInfo.DnsIPInfo[i]=IPBuf[i];
        }
        YS_RunGetHostNameDebug("Serv Dns OK",t_FlowInfo.DnsIPInfo);
    }
    else
    {
        YS_UartDebugString("DNS Parase is Fail!!!");
    }
}


/*-----------------------------------------------------------------------------------------
��������YS_RunUbloxHostNamehandler
����˵�����첽�ȴ�DSN��������Ļص�����Ubloxר��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunUbloxDnsEventHandler(u8 status, u8 *IPBuf)
{
    u8 i;

    if(status==1)
    {
        YS_AGpsSetDnsOKFlag();
        t_FlowInfo.UbloxDnsOKFlag=1;
        for(i=0; i<4; i++)
        {
            t_FlowInfo.UbloxDnsIP[i]=IPBuf[i];
        }
        YS_RunGetHostNameDebug("Ublox Serv Dns OK",t_FlowInfo.UbloxDnsIP);
    }
    else
    {
        YS_AGpsSetDnsFailFlag();
        YS_UartDebugString("Ublox DNS Parase is Fail!!!");
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_RunWebDnsEventHandler
����˵�����첽�ȴ�DSN��������Ļص�����Webר��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunWebDnsEventHandler(u8 status, u8 *IPBuf)
{
    u8 i;

    if(status==1)
    {
        YS_WebSetDnsOKFlag();
        t_FlowInfo.WebDnsOKFlag=1;
        for(i=0; i<4; i++)
        {
            t_FlowInfo.WebDnsIP[i]=IPBuf[i];
        }
        YS_RunGetHostNameDebug("web Serv Dns OK",t_FlowInfo.WebDnsIP);
    }
    else
    {
        YS_WebSetDnsFailFlag();
        YS_UartDebugString("web DNS Parase is Fail!!!");
    }

}
/*-----------------------------------------------------------------------------------------
��������YS_RunYSAppEntry
����˵������ʤ��̬����Ӧ�ó������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunYSAppEntry(void)
{
    sjfun_Sys_Sleep_Disable();  			//��ֹMTK����
    YS_PrmSetFactroyWorkParam(); 		//��ʼ����������
    sjfun_timer(GIS_TIMER_ID_3,1000,YS_RunAppMainTimerHandler);
    YS_RunInitSysInfo();
    YS_RunAddImeiInfo();
    YS_GpsSysInitDeal();
    YS_IODealInit();
    YS_WebServiceInitParam();
    YS_SysRstManageInit();
    YS_RunGpsVibInit();
    YS_AGpsDealInitDeal();
    YS_IOVibJudgeInit();
//    YS_DWAppInitDeal();
    YS_SmsManageInit();
//    YS_GSensorInitDeal();
    YS_AuxUartBufInit();
    SysInfoInitOk=TRUE;
}

bool YS_RunGetSysInfoInit(void)
{
    return SysInfoInitOk;
}

void YS_GprsDebugString(char *DebugStr, kal_int32 Value)
{
    u8 DispBuf[100];
    u8 fbuf[4];

    memset(DispBuf,0x00,100);
    YS_PrmReadOneItem(FLH_PRM_DEBUG_ENABLE,FLH_PRM_DEBUG_ENABLE_LEN,fbuf);
//    if(fbuf[0]==1)
    {
        sprintf((char*)DispBuf,"\n%s:%d\r\n",(u8*)DebugStr,Value);
        ycsj_debug((char*)DispBuf);
    }
}

void YS_RunSetFactoryMode(bool ID)
{
    t_SysRunStatus.FactoryMode=ID;
}

bool YS_RunGetFactoryMode(void)
{
    return t_SysRunStatus.FactoryMode;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetRunFlow
����˵����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_RunGetRunFlow(void)
{
    return(t_SysRunStatus.RunFlow);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetGpsPower
����˵������ȡGPS ��Դ�Ĺ���״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_RunGetGpsPower(void)
{
    return(t_FlowInfo.GpsPowerFlag);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunAGpsSendInterface
����˵����GPS��λ����ƽ̨���ݷ��ͽӿ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunAGpsSendInterface(u8 *dbuf, u16 dlen)
{
    u32 SetRlt;

    YS_UartDebugInterfacel(INTER_PTL_UPDATE_GPRSSEND,(u8*)dbuf,dlen);
    YS_RunSocketDataInc(DATA_FLAG_SEND_ADD);
    SetRlt=sjfun_Socket_Send(t_FlowInfo.AGpsSckID,(u8*)dbuf, dlen);
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsSocketClose
����˵����GPS��λ����ƽ̨�ر�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsSocketClose(void)
{
    sjfun_Socket_Close(t_FlowInfo.AGpsSckID);
    t_FlowInfo.AGpsSckID=SOCKET_ID_INIT_VALUE;
}

bool YS_RunGetAGpsAPExistFlag(void)
{
    return(t_FlowInfo.UbloxIPExsitFlag);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunUbloxDnsInterface
����˵����Ublox DNS �ӿ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_RunUbloxDnsInterface(void)
{
    char ServIP[40];
    u16 i,flen;

    flen=strlen(UBLOX_SERVER_DNS_STR);
    for(i=0; i<flen; i++)
    {
        ServIP[i]=UBLOX_SERVER_DNS_STR[i];
    }
    ServIP[flen]=0;

    sjfun_Dns_ParaseInterface(ServIP, t_FlowInfo.AcctID,t_FlowInfo.UbloxDnsIP,2);
    return 0;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunWebDnsInterface
����˵����web DNS �ӿ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_RunWebDnsInterface(void)
{
#if 0
    char ServIP[40];
    u16 i,flen;
    u8 rlt;

    flen=strlen(WEB_SERVER_DNS_STR);
    for(i=0; i<flen; i++)
    {
        ServIP[i]=WEB_SERVER_DNS_STR[i];
    }
    ServIP[flen]=0;

    rlt = sjfun_Dns_ParaseInterface(ServIP, t_FlowInfo.AcctID,t_FlowInfo.WebDnsIP,3);
    return rlt;
#endif
    return 0;
}

#if 0
/*-----------------------------------------------------------------------------------------
��������YS_RunAGpsSocketInterface
����˵����GPS��λ����ƽ̨SOCKET ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_RunAGpsSocketInterface(void)
{
    u16 Port;
    u8 IPBuf[5];
    u8 fbuf[10];

    YS_PrmReadOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);	//AGPS ����
    switch(fbuf[0])
    {
        case AGPS_FUNC_UBLOX:
            IPBuf[0]=t_FlowInfo.UbloxDnsIP[0];	//U-BLOX-AGPS
            IPBuf[1]=t_FlowInfo.UbloxDnsIP[1];
            IPBuf[2]=t_FlowInfo.UbloxDnsIP[2];
            IPBuf[3]=t_FlowInfo.UbloxDnsIP[3];
            IPBuf[4]=0;
            Port=46434;//U-BLOX-AGPS
            break;

        case AGPS_FUNC_XINDOU:
            IPBuf[0]=222; //�·�������ַmodify by hehongjun at 2014-05-15
            IPBuf[1]=77;
            IPBuf[2]=181;
            IPBuf[3]=73;
            IPBuf[4]=0;
            Port=9120;
            break;

        case AGPS_FUNC_BDUC221:
            IPBuf[0]=114; //�·�������ַmodify by hehongjun at 2014-05-15
            IPBuf[1]=242;
            IPBuf[2]=203;
            IPBuf[3]=140;
            IPBuf[4]=0;
            Port=3701;
            break;

        default:
            return(FALSE);
            break;
    }

    sjfun_Socket_Create(t_FlowInfo.AcctID, &t_FlowInfo.AGpsSckID, IPBuf,Port, 0);
    return(TRUE);
}
#endif
/*-----------------------------------------------------------------------------------------
��������YS_RunAGpsSocketInterface
����˵����GPS��λ����ƽ̨SOCKET ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_RunAGpsSocketInterface(void)
{
    u16 Port;
    u8 IPBuf[5];
    u8 fbuf[10];
    /*
        YS_PrmReadOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);	//AGPS ����
        switch(fbuf[0])
        {
            case AGPS_FUNC_UBLOX:
                IPBuf[0]=t_FlowInfo.UbloxDnsIP[0];	//U-BLOX-AGPS
                IPBuf[1]=t_FlowInfo.UbloxDnsIP[1];
                IPBuf[2]=t_FlowInfo.UbloxDnsIP[2];
                IPBuf[3]=t_FlowInfo.UbloxDnsIP[3];
                IPBuf[4]=0;
                Port=46434;//U-BLOX-AGPS
                break;

            case AGPS_FUNC_XINDOU:
                IPBuf[0]=222; //�·�������ַmodify by hehongjun at 2014-05-15
                IPBuf[1]=77;
                IPBuf[2]=181;
                IPBuf[3]=73;
                IPBuf[4]=0;
                Port=9120;
                break;

            default:
                return(FALSE);
                break;
        }*/

    IPBuf[0]=114;	//U-BLOX-AGPS
    IPBuf[1]=242;
    IPBuf[2]=203;
    IPBuf[3]=140;
    IPBuf[4]=0;
    Port=3701;//U-BLOX-AGPS
    sjfun_Socket_Create(t_FlowInfo.AcctID, &t_FlowInfo.AGpsSckID, IPBuf,Port, 0);
    return(TRUE);
}

void YS_RunSetAGpsAPExitFlag(bool IPFlag)
{
    t_FlowInfo.UbloxIPExsitFlag=IPFlag;
}

bool YS_GetExActiveFlag(void)
{
    return t_FlowInfo.ExActiveFlag;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunExActiveInterface
����˵�����ⲿ�����Ѳ�����һ��ʱ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunExActiveInterface(u16 DelaySet)
{
    if(YS_RunGetRunFlow()==YS_RUN_FLOW_FIGHT_WAIT)
    {
        YS_ExitGsmSleepMode();
    }
    else if(YS_RunGetRunFlow()==YS_RUN_FLOW_SLEEP_WAIT)
    {
        YS_RunEntryWakeupMode();	//
    }
    t_FlowInfo.ExActiveFlag=TRUE;
    t_FlowInfo.ExActiveCount=DelaySet;
}

void YS_RunWebPPPInterface(void)
{
    t_FlowInfo.AcctID = YS_RunPPPInterFaceDeal();
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGetVibCount
����˵������ȡϵͳ�����񶯵Ĵ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_RunGetVibCount(void)
{
    return t_SysRunStatus.VibTotalCount;
}

bool YS_RunStaticCount(void)
{
    static u16 BankVibCount=0;
    static U16 CountTime=0;
    static bool FirstCount=TRUE;

    if(FirstCount==TRUE)
    {
        FirstCount=FALSE;
        BankVibCount=t_SysRunStatus.VibTotalCount;
        return(FALSE);
    }
    else
    {
        CountTime++;
        if(CountTime>50)
        {
            CountTime=50;
            return TRUE;
        }
        else	if(BankVibCount!=t_SysRunStatus.VibTotalCount)
        {
            FirstCount=TRUE;
            CountTime=0;
            return FALSE;
        }
    }
    return(FALSE);
}

bool YS_GetSysSleepFlag(void)
{
    if(t_SysRunStatus.SleepStatus==0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/****************************************************

��ȡϵͳ����ʱ��
*****************************************************/
u32 YS_GetSysRunTime(void)
{
    return t_SysRunStatus.TotalRunTime;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunGpsVibInit
����˵����GPS ��̬Ư�ƴ�������ʼ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunGpsVibInit(void)
{
    t_VibFinterDeal.VibOKFlag=FALSE;
    t_VibFinterDeal.PosFlag=FALSE;
    t_VibFinterDeal.DelayCount=0;
}


/*-----------------------------------------------------------------------------------------
��������YS_RunWebSocketClose
����˵��������λ������ر�SOCKET
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunWebSendInterface(u8 *dbuf, u16 dlen)
{
//    YS_UartDebugInterfacel(INTER_PTL_UPDATE_GPRSSEND,dbuf,dlen);
    sjfun_Socket_Send(t_FlowInfo.WebSckID,dbuf, dlen);
}

/*-----------------------------------------------------------------------------------------
��������YS_RunSetGpsPower
����˵��������GPS ��Դ�Ĺ���״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunSetGpsPower(bool Flag)
{
    t_FlowInfo.GpsPowerFlag = Flag;
}

/*-----------------------------------------------------------------------------------------
��������YS_RunLoginServerAck
����˵������¼ƽ̨Ӧ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_RunLoginServerAck(void)
{
    t_FlowInfo.LogOKFlag=TRUE;
}

