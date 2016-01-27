/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_IODeal.c  2013-06-27
此文件管理与GPIO相关的操作。
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

t_Gpio_Ctrl_Manage	t_GpioCtrlManage;
u8   ShutDownLedFlag=0;
u8	PowKeyMsgFlag=0;

//客户端程序LED 显示控制定时器处理函数
void YS_IODealLedTimerHandler(void)
{
    static u8 GsmLedCount=0;
    static u8 GpsLedCount=0;

    t_Sys_Run_Status t_GetStatus;
    t_Gps_Data_Info	t_GetGps;


    YS_GpsGetPosData(&t_GetGps);
    YS_RunGetSystemStatus(&t_GetStatus);

    if (t_GetStatus.AccStatus == 0)
    {
        sjfun_SetKPLED(0);
        return;
    }
    if(t_GetGps.Effective==TRUE)	//GPS 指示灯控制
    {
        t_GpioCtrlManage.GpsLedStatus=1;				//GPS 定位时，指示灯常亮
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
函数名：YS_IODealInit
功能说明：IO 初始化处理函数
修改记录：
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
    t_GpioCtrlManage.PofRedoCount=POF_RE_WARN_DELAY;	//系统复位后，需检测到主电才启动断电报警

    t_GpioCtrlManage.VibWakeSignFlag=0;					//振动唤醒控制
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
函数名：YS_IOGetRFPowStatue
功能说明：获取RF电源状态
修改记录：
-------------------------------------------------------------------------------------------*/
/*bool YS_IOGetRFPowStatue(void)
{
	return t_GpioCtrlManage.RFPowStatue;
}*/

/*-----------------------------------------------------------------------------------------
函数名：YS_IOSetRFPowStatue
功能说明：设置RF电源状态
修改记录：
-------------------------------------------------------------------------------------------*/
/*void YS_IOSetRFPowStatue(bool Flag)
{
	t_GpioCtrlManage.RFPowStatue = Flag;
}*/

/*-----------------------------------------------------------------------------------------
函数名：YS_IOSetKeyStatue
功能说明：设置钥匙扣状态
修改记录：
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
函数名：YS_IOGetKeyStatue
功能说明：获取钥匙扣状态
修改记录：
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
函数名：YS_IODealVibSwitchInput
功能说明：振动开关信号输入
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_IODealVibSwitchInput(char VibID)
{
    t_GpioCtrlManage.VibSignFlag=1; 			//设置振动信号输入
    t_GpioCtrlManage.VibSignChk=1;
    t_GpioCtrlManage.VibWakeSignFlag=1;
}


/*-----------------------------------------------------------------------------------------
函数名：YS_IOvidJuege
功能说明：判断终端是否达到振动条件
修改记录：
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
函数名：YS_IOBattLowDeal
功能说明：内部电池低电压报警
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_IOBattLowDeal(u16 BattValue)
{

}


/*-----------------------------------------------------------------------------------------
函数名：YS_IOCalcBattLow
功能说明：电池低电压报警计算入口函数
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_IOCalcBattLow(void)
{
    t_Sys_Run_Status t_GetStatus;

    YS_RunGetSystemStatus(&t_GetStatus);
    YS_IOBattLowDeal(t_GetStatus.BattValue);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_IODealVibSwitchInput
功能说明：振动开关信号输入
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_IOInputInfoDeal(void)
{
    bool BankACCstatus= FALSE;

    if(t_GpioCtrlManage.VibSignFlag==1)
    {
        t_GpioCtrlManage.VibSignFlag=0;
        YS_RunVibCountAdd();
    }

    if(sjfun_Get_Power_Statue()==TRUE) //外电状态有效
    {
        YS_RunPowerStatusBrush(1);
    }
    else //外电状态无效
    {
        YS_RunPowerStatusBrush(0);
        t_GpioCtrlManage.ISChargingStatus=0;
    }

    if(sjfun_Get_Chr_Statue(1)==TRUE) //充电完成
    {
        t_GpioCtrlManage.ISChargingStatus=0;
    }
    if(sjfun_Get_Chr_Statue(0)==TRUE)  //正在充电
    {
        t_GpioCtrlManage.ISChargingStatus=1;
    }

//    YS_RunAccStatusBrush(1);
#if 0
    if (sjfun_Gpio_Read_Value(YS_PIN_NO_ACC_PWR) == 0)//ACC 接口状态
    {
        YS_RunAccStatusBrush(1);
        if (BankACCstatus == FALSE)
        {
            BankACCstatus = TRUE;
//            YS_WebAddRequest(FALSE);
        }
        if (YS_RunGetGpsPower()==FALSE)
        {
//            YS_RunSetGpsPower(TRUE);
//            sjfun_Gpio_Write_Value(YS_PIN_NO_GPS_PWR,1);//打开GPS电源
//            sjfun_Gps_Start(0);
        }
    }
    else
    {
        YS_RunAccStatusBrush(0);
        if (BankACCstatus == TRUE)
        {
            BankACCstatus = FALSE;
        }
        if (YS_RunGetGpsPower()==TRUE)
        {
//            YS_RunSetGpsPower(FALSE);
//            sjfun_Gpio_Write_Value(YS_PIN_NO_GPS_PWR,0);//关闭GPS电源
//            sjfun_Gps_Stop(0);
        }
    }
#endif
}

/*-----------------------------------------------------------------------------------------
函数名：YS_IOGeneralInit
功能说明：初始化通用IO
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_IOGeneralInit(u8 *dbuf)
{
    sjfun_Gpio_Set_Mode(dbuf[0], 0);				//设置IO 工作模式
    sjfun_Gpio_Set_Direction(dbuf[0],dbuf[1]);		//设置方向为输出
    sjfun_Gpio_Set_PullEN(dbuf[0],0);				//禁止上接电阻
//	sjfun_Gpio_Set_PullValue(dbuf[0],1);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_IOGeneralWrite
功能说明：设置通用IO 输出状态
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_IOGeneralWrite(u8 *dbuf)
{
    sjfun_Gpio_Write_Value(dbuf[0],dbuf[1]);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_IOGeneralRead
功能说明：读取通用IO 输入状态
修改记录：
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
函数名：YS_IOAccThiefDeal
功能说明：ACC 设防/撤防检测
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_IOACCDydReCount(void)
{

}

