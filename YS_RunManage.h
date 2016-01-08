#ifndef _YS_RUNMANAGE_H
#define _YS_RUNMANAGE_H
#include "Data_type.h"

#define MAIN_TIMER_TIME_DEF		1000
#define MAIN_TIMER_SLEEP_DEF		830
#define SOCKET_ID_INIT_VALUE		0xFF
#define REG_FULL_TIME_DFE			180
#define AVL_AUTO_RESET_COUNT		90000
#define AVL_RST_HOUR_L        		2
#define AVL_RST_HOUR_H       			3
#define COURSE_SEND_VALUE_DEF		15
#define CAR_RUN_VIB_BUF_MAX		60
#define CAR_RUN_VIB_STOP_VALUE	5
#define CAR_RUN_VIB_RUN_VALUE		15

#define DEFAULT_IMSI_DEF	"460000123456789"


typedef struct
{
	bool		MoniFlag;
	u16 		SendSetDelay;
	u16 		SendSetTimes;
	u16		SendCountDelay;
	u16		SendCountTimes;
}t_Ys_Trace_Manage;

enum
{
	FJYD_STATUS_ID_POWER,
	FJYD_STATUS_ID_THIEF,
	FJYD_STATUS_ID_SLEEP,
	FJYD_STATUS_ID_ACC,
	FJYD_STATUS_ID_LACID,
	FJYD_STATUS_ID_CELLID,
	FJYD_STATUS_ID_COUNTRYNAME,
	FJYD_STATUS_ID_NETNAME,
	FJYD_STATUS_ID_BATTVALUE
};

enum
{
	YS_RUN_FLOW_INIT=0,
	YS_RUN_FLOW_WAIT_REG,
	YS_RUN_FLOW_PPP_BEIGN,
	YS_RUN_FLOW_WAIT_PPP,
	YS_RUN_FLOW_CALL_BEGIN,
	YS_RUN_FLOW_WAIT_CALL,
	YS_RUN_FLOW_DNS_BEGIN,
	YS_RUN_FLOW_WAIT_DNS,
	YS_RUN_FLOW_SOCKET_BEGIN,
	YS_RUN_FLOW_WAIT_SOCKET,

	YS_RUN_FLOW_REGSERVER_BEGIN,
	YS_RUN_FLOW_WAIT_SERVREG,
	YS_RUN_FLOW_SEVLOG_BEGIN,
	YS_RUN_FLOW_WAIT_SERVLOG,
	YS_RUN_FLOW_IDLE_INIT,
	YS_RUN_FLOW_IDLE_DEAL,
	YS_RUN_FLOW_RDCON_BEGIN,
	YS_RUN_FLOW_RDCON_WAIT,
	YS_RUN_FLOW_SLEEP_BEGIN,
	YS_RUN_FLOW_SLEEP_WAIT,

	YS_RUN_FLOW_WAIT_ACTIVE,
	YS_RUN_FLOW_RST_WAITING,
	YS_RUN_FLOW_RST_DOING,
	YS_RUN_FLOW_FIGHT_BEGIN,
	YS_RUN_FLOW_FIGHT_WAIT,
	YS_RUN_FLOW_ATTACH_NET_BEGIN,
	YS_RUN_FLOW_ATTACH_NET_WAIT,
	YS_RUN_FLOW_DW_RSQINFO_BEGIN,
	YS_RUN_FLOW_DW_RSQINFO_WAIT,
	YS_RUN_FLOW_DW_RSQDATA_BEGIN,

	YS_RUN_FLOW_DW_RSQDATA_WAIT,
	YS_RUN_FLOW_DW_UPDATE_BEGIN,
	YS_RUN_FLOW_DW_UPDATE_WAIT,
	YS_RUN_FLOW_DW_EXIT,
	YS_RUN_FLOW_RST_EXITWAIT
};

enum
{
	DNS_RLT_OK_WAIT,
	DNS_RLT_OK_GETIP,
	DNS_RLT_FAIL,
	DNS_RLT_MAX
};

enum
{
	DATA_FLAG_REC_ADD,
	DATA_FLAG_SEND_ADD
};

typedef struct
{
	u8          RunFlow;
	u8		SimStatus;
	u8		RegStatus;
	u8		ThiefStatus;
	u8		CsqValue;
	u8		AccStatus;
	u8		PowerStatus;
	u8		SleepStatus;
	u16		AreaID;
	u16		CellID;
	u16 		BattValue;
	u16		PPPTimes;
	u16 		SmsSendTimes;
	u16 		SmsRecTimes;
	u16		SckSendTimes;
	u16		SckRecTimes;
	u16		CallOutTimes;
	u16		CallInTimes;
	u32		Distance;
	u32		TotalRunTime;
	char       IMSI[16];
	u16		NetName;
	u16		CountryName;
	u16        VibTotalCount;
	u8 		GsmBandValue;
	u8		Reserve1;
	u8		WarnStatus[30];

	u8		GsmSleepStatus;
	bool 	FactoryMode;
}t_Sys_Run_Status;


typedef struct
{
	u16		OptDelay;
	u32		AcctID;
	u8		SocketID;
	u8           DWSckID;
	u8		WebSckID;
	u8		AutoUpdateSckID;
	bool        SocketRlt;
	u8		LogTimes;
	u8		LogDelay;
	bool		LogOKFlag;
	bool		LogFailFlag;
	u8		RsqFileTimes;
	u8		RsqFileDelay;
	u8		RsqFileOKFlag;
	u8		RsqFileFailFlag;
	u16 		PosDelay;
	u16         ObdDelay;
	u16 		PosTimes;
	u16 		HeartDelay;
	u16     CanDelay;
	u16		HeartTimes;
	u16		SleepCount;
	u16     	PowSleepCount;
	u8      	BankPowerStatus;
	u8		BankAccStatus;
	u16		WakeCount;
	u16         ExActiveCount;
	bool		ExActiveFlag;
	bool		GpsPowerFlag;
	u8		LedMoni;
	u8		RouteSckID;
	u8		WarnFlag;
	u8		WarnDelay;
	u8		WarnTimes;
	u8      	ImeiInfo[16];
	u8      	ServLogErrTimes;
	u16     	VibSleepCount;
	u8      	SocketErrTimes;
	u8		AGpsSckID;
	bool		DnsAddrFlag;
	u8		DnsIPInfo[4];
	bool		DnsOKFlag;
	u8		DnsErrTimes;
	u8		SysDTSyncFlag;
	u8 		SocOk;
	u8		UbloxDnsOKFlag;
	u8		UbloxDnsIP[4];
	u8		WebDnsOKFlag;
	u8		WebDnsIP[4];
	u8		RstCodeFlag;
	u16		RstDelayCount;
	u16		LostNetCount;

	bool 	SeverRegOk;
	u8		SeverRegTime;
	u8 		SeverRegErrTime;
	bool 	SeverRegFail;
	u8 		SeverRegDelay;

	u8		BattValueReflashFlag;

	u8          SleepDelayWait;

	bool		AppIPExsitFlag;
	bool		UbloxIPExsitFlag;

	bool 	GpsPSMStatue;

	bool		LogFullFlag;
	u8		RsqFileUnknowFlag;

	bool 	IRSensorWorkStatue;		//红外传感工作状态
	bool 	gSensorJbWorkStatue;	//计步工作状态

	bool 	TerWearStatue;			//终端佩戴状态

	bool 	ShutDownFlag;

	u16 		RegDelay;
	u16 		RegTimes;
	bool        RegFailFlag;
	bool 	RegOKFlag;

	u8		ServRegErrTimes;
}t_Flow_Info;

typedef struct
{
	bool		VibFlag;
	bool		PowerOffFlag;
	bool		PosOut;
	bool		BattLowFlag;
	bool		AreaInFlag;
	bool		AreaOutFlag;
	bool 	NewWarnFlag;
	u8		Reserve8Bit1;
	u8		WarnDateTime[40];
}t_Gprs_Warn_Manage;

typedef struct
{
	bool 	LowClock;
	bool 	HighClock;
	u16  	DelayCount;
	u16  	DelaySet;
	bool  	RstEnable;
	u8	 	Reserve1;
}t_Avl_Auto_Reset;


typedef struct
{
	bool		VibOKFlag;
	bool		PosFlag;
	u16		DelayCount;
	u8          Longitude[4];
	u8	      Latitude[4];

	u8 		VibStatusBuf[CAR_RUN_VIB_BUF_MAX];  //用于检测车辆是否正常运行
	u8		VibStatusNum;
	u8		VibStatusWP;
	u8		VibStatusFlag;
	u16		NoVibDelayCount;
}t_VIB_Finter_Deal;

/*typedef struct
{
	bool		LedCtrlFlag;
	u8		LedCtrlDelay;
}t_Led_Re_Show_Ctrl;*/

typedef struct
{
	u16 	 	arfcn;           /*ARFCN*/
	u8 		bsic;              /*BSIC*/
	u8 		rxlev;            /*Received signal level*/
	u16 		mcc;
    	u16 		mnc;
    	u16 		lac;
    	u16 		ci;
	u8      	ta;
}t_Cell_Info;

typedef struct
{
	u8 Cell_Num;
	t_Cell_Info t_MainCellInfo;
	t_Cell_Info t_CellInfo[6];
}t_Net_Info;

extern void YS_RunGetStatusInfo(u8 ID, u8 *FillBuf);

extern u8 YS_RunGetCsqValue(void);

extern u16 YS_RunGetBattValue(void);

extern u8 YS_RunGetACCStatus(void);

extern void YS_RunGetIMEI(char *ImeiStr);

extern void YS_RunGetIMSI(char *ImsiStr);

//extern void YS_RunExternThiefInput(u8 ID);

extern void YS_RunGetSystemStatus(t_Sys_Run_Status *pt_SystemStatus);

extern void YS_RunSocketDataInc(u8 Type);

extern void YS_RunSmsDataInc(u8 Type);

extern void YS_RunCallDataInc(u8 Type);

extern void YS_RunIdleHeartAck(void);

extern void YS_RunSocketSendData(u8 *dbuf, u16 dlen);

extern void YS_RunPowerStatusBrush(u8 ID);

extern bool YS_RunWebSocketInterFace(void);

extern void YS_RunWebSocketClose(void);

extern void YS_RunWebSendInterface(u8 *dbuf, u16 dlen);

extern void YS_RunEntrySleepMode(void);

extern void YS_SysGetDTTest(void);

extern bool YS_RunGetGpsPower(void);

extern void YS_RunSetIMSIInfo(u8 *IMSIBuf, u8 IMSILen);

extern void YS_RunSetSimStatus(u8 ID);

extern void YS_RunSetBattValue(u16 BattValue);

extern void YS_RunGprsWarnInput(u8 WarnID);

extern void YS_RunTraceInterFace(u16 TraceTimes, u16 TraceDelay);

extern void YS_RunGpsVibInit(void);

extern void YS_RunGetFinterGpsData(u8 *Longitude, u8 *Latitude);

extern u8 YS_RunGetThiefStatus(void);

//extern void YS_RunLedReShowInit(void);

extern void YS_RunRouteSendInterface(u8 *dbuf, u16 dlen);

extern void YS_RunRouteSocketClose(void);

extern bool YS_RunRouteSocketInterface(void);

extern void YS_RunVibCountAdd(void);


extern void YS_CarRunStatusVibInput(void);

extern void YS_RunSetDTSyncFlag(void);

extern bool YS_RunGetDTSyncStatus(void);

extern void YS_RunSetVerChannelID(u8 ID);

extern u8 YS_RunReadVerChannelID(void);

extern void YS_RunRstSaveWarnFlag(void);

extern void YS_RunSetBattValueReflashFlag(void);

extern u8 YS_RunReadBattValueReflashFlag(void);

extern u8 YS_RunGetWarnTimes(u8 WarnID);

extern u8 YS_RunGetWarnEnable(u8 WarnID);

extern void YS_RunAddOneWarnTimes(u8 WarnID);

//extern void YS_RunPPPInterfaceNew(void);

extern void YS_RunEntryWakeupMode(void);

extern u8 YS_RunUbloxDnsInterface(void);

extern u16 YS_RunGetRstWarnInfo(u8 *dbuf);

extern bool YS_RunGsmColckisOK(void);

extern void YS_RunYSAppEntry(void);

extern void YS_RunSocketEventHandler(U8 SocketID, U8 EventID, U8 *dbuf, U16 dlen);

extern void YS_GprsDebugString(char *DebugStr, kal_int32 Value);

extern void YS_RunSetFactoryMode(bool ID);

extern bool YS_RunGetFactoryMode(void);

extern u8 YS_RunGetRunFlow(void);

extern void YS_ExitGsmSleepMode(void);

extern void YS_EntryGsmSleepMode(void);

extern void YS_RunDnsEventHandler(u8 status, u8 *IPBuf);

extern bool YS_RunGetAGpsAPExistFlag(void);

extern bool YS_RunAGpsSocketInterface(void);

extern void YS_RunSetAGpsAPExitFlag(bool IPFlag);


extern bool YS_GetExActiveFlag(void);

extern void YS_RunExActiveInterface(u16 DelaySet);

extern u32 YS_RunPPPInterFaceDeal(void);

extern void YS_RunWebPPPInterface(void);

//extern void YS_SetParkingCheckFlag(u8 ID);

extern void YS_RunWarnSignInput(u8 WarnID);

extern void YS_RunSet_TerStatueChang(bool ID);

extern u8 YS_RunGet_ActionAfterReptSta(void);

extern void YS_RunSet_ActionAfterReptSta(u8 ID);

extern u16 YS_RunGetVibCount(void);

extern bool YS_RunStaticCount(void);

extern u8 YS_GetParkingCheckFlag(void);

extern void YS_SetParkStatueFlag(u8 ID);

extern void YS_SetThrStaticStatueFlag(u8 ID);

extern bool YS_GetSysSleepFlag(void);

extern u32 YS_GetSysRunTime(void);

extern void YS_RunSetSeverReg(void);

extern bool YS_RunGetSysInfoInit(void);

extern void YS_RunUbloxDnsEventHandler(u8 status, u8 *IPBuf);

extern void YS_RunWebDnsEventHandler(u8 status, u8 *IPBuf);

extern u8 YS_RunWebDnsInterface(void);

extern void YS_SetNetInfo(u8 CellNum,u16 *InputBuf);

extern void YS_GetNetInfo(t_Net_Info *PackBuf);

extern void YS_RunGprsReleaseWarn(void);

extern void YS_RunSetGpsPower(bool Flag);

extern void YS_RunSetDTSyncFlagFalse(void);

extern void YS_RunLoginServerAck(void);

extern u8 YS_RunGetCellNum(void);

extern  void YS_RunSetJbWorkStatue(bool Flag);

extern  bool YS_RunGetTerWearStatue(void);

extern void YS_RunSetTerWearStatue(bool Flag);

extern void YS_SetShutDownFlag(void);

extern bool YS_GetShutDownFlag(void);

extern u16 YS_SysGetNetInfo(u8 *PackBuf);

extern void YS_RunAddIDInfo(u8 *IDBuf);

extern void YS_RunCleanWarmFlag(void);

extern void YS_RunCleanWarmACK(void);

extern void YS_RunRegServerAck(void);

extern void YS_RunMotorCtrlInterface(void);

extern void YS_MotorIOCtrl(bool lever);

extern void YS_RunSetFlowRst(void);
#endif

