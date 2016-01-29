/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_IODeal.c  2013-06-27
���ļ�������GPIO��صĲ�����
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

t_Gpio_Ctrl_Manage	t_GpioCtrlManage;
u8   ShutDownLedFlag=0;
u8	PowKeyMsgFlag=0;

//�ͻ��˳���LED ��ʾ���ƶ�ʱ��������
void YS_IODealLedTimerHandler(void)
{
    static u8 GsmLedCount=0;
    static u8 GpsLedCount=0;

    t_Sys_Run_Status t_GetStatus;
    t_Gps_Data_Info	t_GetGps;


    YS_GpsGetPosData(&t_GetGps);
    YS_RunGetSystemStatus(&t_GetStatus);

    if (t_GetStatus.SleepStatus == 1)
    {
        sjfun_SetKPLED(0);
        return;
    }
    if(t_GetGps.Effective==TRUE)	//GPS ָʾ�ƿ���
    {
        t_GpioCtrlManage.GpsLedStatus=1;				//GPS ��λʱ��ָʾ�Ƴ���
        sjfun_SetKPLED(1);
    }
    if(t_GetStatus.RunFlow == YS_RUN_FLOW_IDLE_DEAL)
    {
        GsmLedCount++;
        if(GsmLedCount==1)
        {
            sjfun_SetKPLED(1);
        }
        else if(GsmLedCount==3)
        {
            GsmLedCount=0;
            sjfun_SetKPLED(0);
        }
        else if  (GsmLedCount>=5)
        {
            GsmLedCount = 0;
        }
    }
    else
    {
        GsmLedCount++;
        if(GsmLedCount==3)
        {
        }
        else if(GsmLedCount>=6)
        {
            GsmLedCount=0;
        }

        GpsLedCount++;
        if(GpsLedCount==10)
        {
            sjfun_SetKPLED(0);
        }
        else if(GpsLedCount>=20)
        {
            GpsLedCount=0;
            sjfun_SetKPLED(1);
        }
    }


    sjfun_timer(GIS_TIMER_ID_4,200,YS_IODealLedTimerHandler);
}


/*-------------------------------------------------------------------------------------------
��������YS_IODealInit
����˵����IO ��ʼ��������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_IODealInit(void)
{
//    sjfun_Gpio_Set_Mode(YS_PIN_NO_LED_GPS, 0);
//    sjfun_Gpio_Set_Direction(YS_PIN_NO_LED_GPS,0);
//    sjfun_Gpio_Write_Value(YS_PIN_NO_LED_GPS,0);

//    sjfun_Gpio_Set_Mode(YS_PIN_NO_LED_GSM, 0);
//    sjfun_Gpio_Set_Direction(YS_PIN_NO_LED_GSM,0);
//    sjfun_Gpio_Write_Value(YS_PIN_NO_LED_GSM,0);

//    sjfun_Gpio_Set_Mode(YS_PIN_NO_WAKEUP, 0);
//    sjfun_Gpio_Set_Direction(YS_PIN_NO_WAKEUP,1);
//    sjfun_Gpio_Write_Value(YS_PIN_NO_WAKEUP,0);

    sjfun_Gpio_Set_Mode(YS_PIN_NO_WAKEUP, 0);
    sjfun_Gpio_Set_PullEN(YS_PIN_NO_WAKEUP,1);
    sjfun_Gpio_Set_PullValue(YS_PIN_NO_WAKEUP,1);
    sjfun_Gpio_Set_Direction(YS_PIN_NO_WAKEUP,1);
    sjfun_Gpio_Write_Value(YS_PIN_NO_WAKEUP,1);


    sjfun_Gpio_Set_Mode(YS_PIN_NO_OBD_RST, 0);
    sjfun_Gpio_Set_Direction(YS_PIN_NO_OBD_RST,0);
    sjfun_Gpio_Write_Value(YS_PIN_NO_OBD_RST,1);

    t_GpioCtrlManage.GpsLedStatus=0;
    t_GpioCtrlManage.GsmLedStatus=0;
    t_GpioCtrlManage.ChargeLedStatus=0;
    t_GpioCtrlManage.AccOffTime=0;
    t_GpioCtrlManage.AccOnTime=0;
    t_GpioCtrlManage.AccBankStatus=0;
    t_GpioCtrlManage.AccRstDeal=1;
    t_GpioCtrlManage.AccCalcFlag=ACC_LOCK_ACT_NONE;
    t_GpioCtrlManage.VibRedoEnable=1;
    t_GpioCtrlManage.VibRedoCount=0;
    t_GpioCtrlManage.VibSignFlag=0;
    t_GpioCtrlManage.VibDataCount=0;
    t_GpioCtrlManage.PofRedoEnable=0;
    t_GpioCtrlManage.PofReHighFlag=0;
    t_GpioCtrlManage.PofRedoCount=POF_RE_WARN_DELAY;	//ϵͳ��λ�����⵽����������ϵ籨��

    t_GpioCtrlManage.VibWakeSignFlag=0;					//�񶯻��ѿ���
    t_GpioCtrlManage.VibWakeCount=0;

    t_GpioCtrlManage.LedDelay=0;
    sjfun_timer(GIS_TIMER_ID_9,100,YS_IODealLedTimerHandler);
}

void YS_SetPowoffKeyMsg(void)
{
    PowKeyMsgFlag=1;
}

void YS_SetShutDownLedFlag(void)
{
    ShutDownLedFlag=1;
}

/*-----------------------------------------------------------------------------------------
��������YS_IOGetRFPowStatue
����˵������ȡRF��Դ״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
/*bool YS_IOGetRFPowStatue(void)
{
	return t_GpioCtrlManage.RFPowStatue;
}*/

/*-----------------------------------------------------------------------------------------
��������YS_IOSetRFPowStatue
����˵��������RF��Դ״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
/*void YS_IOSetRFPowStatue(bool Flag)
{
	t_GpioCtrlManage.RFPowStatue = Flag;
}*/

/*-----------------------------------------------------------------------------------------
��������YS_IOSetKeyStatue
����˵��������Կ�׿�״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
/*void YS_IOSetKeyStatue(bool ID)
{
	u8 fbuf[4];

	YS_PrmReadOneItem(FLH_PRM_RF_CHECK_ENABLE, FLH_PRM_RF_CHECK_ENABLE_LEN,fbuf);

	if(fbuf[0]==0)
	{
		return;
	}
	if(ID==TRUE)
	{
		t_GpioCtrlManage.KeyStatue=TRUE;
	}
	else
	{
		t_GpioCtrlManage.KeyStatue=FALSE;
	}
	//YS_UartDebugString("SET THE KEY IS EXSIT!");
}*/

/*-----------------------------------------------------------------------------------------
��������YS_IOGetKeyStatue
����˵������ȡԿ�׿�״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
/*bool YS_IOGetKeyStatue(void)
{
	return t_GpioCtrlManage.KeyStatue;
}*/

/*bool YS_GetRFAddFlag(void)
{
	return(t_GpioCtrlManage.AddRfKeyFlag);
}

void YS_SetRFAddFlag(bool ID)
{
	t_GpioCtrlManage.AddRfKeyFlag=ID;
}*/

/*bool YS_GetRFAddOKFlag(void)
{
	return(t_GpioCtrlManage.AddRfKeyOkFlag);
}

void YS_SetRFAddOKFlag(bool ID)
{
	t_GpioCtrlManage.AddRfKeyOkFlag=ID;
}*/

void YS_IOSetVibLedSet(void)
{
    t_GpioCtrlManage.VibLedHit=TRUE;
    //t_GpioCtrlManage.GsmLedCount=0;
}

/*-----------------------------------------------------------------------------------------
��������YS_IODealVibSwitchInput
����˵�����񶯿����ź�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_IODealVibSwitchInput(char VibID)
{
    t_GpioCtrlManage.VibSignFlag=1; 			//�������ź�����
    t_GpioCtrlManage.VibSignChk=1;
    t_GpioCtrlManage.VibWakeSignFlag=1;
}


/*-----------------------------------------------------------------------------------------
��������YS_IOvidJuege
����˵�����ж��ն��Ƿ�ﵽ������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_IOVibJudgeInit(void)
{
    t_GpioCtrlManage.VibSignFlag=0;
    t_GpioCtrlManage.VibDataCount=0;
    t_GpioCtrlManage.VibRedoCount=0;
    t_GpioCtrlManage.VibRedoEnable=1;
    t_GpioCtrlManage.VibWarnFlag=0;
    t_GpioCtrlManage.VibWarnDelay=0;
}

/*-----------------------------------------------------------------------------------------
��������YS_IOBattLowDeal
����˵�����ڲ���ص͵�ѹ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_IOBattLowDeal(u16 BattValue)
{

}


/*-----------------------------------------------------------------------------------------
��������YS_IOCalcBattLow
����˵������ص͵�ѹ����������ں���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_IOCalcBattLow(void)
{
    t_Sys_Run_Status t_GetStatus;

    YS_RunGetSystemStatus(&t_GetStatus);
    YS_IOBattLowDeal(t_GetStatus.BattValue);
}

/*-----------------------------------------------------------------------------------------
��������YS_IODealVibSwitchInput
����˵�����񶯿����ź�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_IOInputInfoDeal(void)
{
    bool BankACCstatus= FALSE;

    if(t_GpioCtrlManage.VibSignFlag==1)
    {
        t_GpioCtrlManage.VibSignFlag=0;
        YS_RunVibCountAdd();
    }

    if(sjfun_Get_Power_Statue()==TRUE) //���״̬��Ч
    {
        YS_RunPowerStatusBrush(1);
    }
    else //���״̬��Ч
    {
        YS_RunPowerStatusBrush(0);
        t_GpioCtrlManage.ISChargingStatus=0;
    }

    if(sjfun_Get_Chr_Statue(1)==TRUE) //������
    {
        t_GpioCtrlManage.ISChargingStatus=0;
    }
    if(sjfun_Get_Chr_Statue(0)==TRUE)  //���ڳ��
    {
        t_GpioCtrlManage.ISChargingStatus=1;
    }

//    YS_RunAccStatusBrush(1);
//    sjfun_Gpio_Read_Value(YS_PIN_NO_WAKEUP)
#if 1
    if (sjfun_Gpio_Read_Value(YS_PIN_NO_WAKEUP) == 1)//ACC �ӿ�״̬
    {
        YS_RunAccStatusBrush(1);
        if (BankACCstatus == FALSE)
        {
            BankACCstatus = TRUE;
        }
    }
    else
    {
        YS_RunAccStatusBrush(0);
        if (BankACCstatus == TRUE)
        {
            BankACCstatus = FALSE;
        }
    }
#endif
}

/*-----------------------------------------------------------------------------------------
��������YS_IOGeneralInit
����˵������ʼ��ͨ��IO
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_IOGeneralInit(u8 *dbuf)
{
    sjfun_Gpio_Set_Mode(dbuf[0], 0);				//����IO ����ģʽ
    sjfun_Gpio_Set_Direction(dbuf[0],dbuf[1]);		//���÷���Ϊ���
    sjfun_Gpio_Set_PullEN(dbuf[0],0);				//��ֹ�Ͻӵ���
//	sjfun_Gpio_Set_PullValue(dbuf[0],1);
}

/*-----------------------------------------------------------------------------------------
��������YS_IOGeneralWrite
����˵��������ͨ��IO ���״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_IOGeneralWrite(u8 *dbuf)
{
    sjfun_Gpio_Write_Value(dbuf[0],dbuf[1]);
}

/*-----------------------------------------------------------------------------------------
��������YS_IOGeneralRead
����˵������ȡͨ��IO ����״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_IOGeneralRead(u8 *dbuf)
{
    u8 Value;

    Value=sjfun_Gpio_Read_Value(dbuf[0]);
    return(Value);
}


u8 YS_GetChargeStatue(void)
{
    return t_GpioCtrlManage.ISChargingStatus;
}

/*-----------------------------------------------------------------------------------------
��������YS_IOAccThiefDeal
����˵����ACC ���/�������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_IOACCDydReCount(void)
{

}

