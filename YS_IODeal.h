#ifndef _YS_IODEAL_H
#define _YS_IODEAL_H
#include "Data_type.h"

#define VIB_COLL_TIMES_MAX		60
#define VIB_RE_WARN_DELAY		120
#define POF_RE_WARN_DELAY		300

#define YS_PIN_NO_I2CSCL		43	//gSensor 读写时钟PIN 定义
#define YS_PIN_NO_I2CDAT		44	//gSensor读写数据PIN 定义
#define YS_PIN_NO_GPOW			0	//gSensor电源控制PIN否定义
#define YS_PIN_NO_GINT			6	//gSensor中断信号输入

#define YS_PIN_NO_LED_GPS		33
#define YS_PIN_NO_LED_GSM		34

#define YS_PIN_NO_GPS_PWR       55

#define YS_PIN_NO_ACC_PWR       6

#define YS_PIN_NO_LED_CHARGE	53	//充电指示灯控制

//#define YS_PIN_NO_VCC_SIGN		8	//充电输入检测
#define YS_PIN_NO_OBD_RST       9	//充电状态检测

//红外引脚定义
//#define YS_PIN_IRSENSOR_I2CSCL		1
//#define YS_PIN_IRSENSOR_I2CDAT		2
//#define YS_PIN_IRSENSOR_INT		56

enum
{
	LED_STATUS_DEF_OFF,
	LED_STATUS_DEF_ON
};

enum
{
	ACC_LOCK_ACT_NONE,
	ACC_LOCK_ACT_ON,
	ACC_LOCK_ACT_OFF
};

enum
{
	KEY_INPUT_VALUE_1,
	KEY_INPUT_VALUE_2,
	KEY_INPUT_VALUE_3,
	KEY_INPUT_VALUE_4
};

enum
{
	LED_KEY_FLOW_LIGHT,
	LED_KEY_FLOW_TWINKLE,
	LED_KEY_FLOW_END
};

typedef struct
{
	u8	GpsLedStatus;
	u8	GpsLedDelay;
	u8	GsmLedStatus;
	u8	GsmLedCount;
	u8   ChargeLedStatus;
	u16	ChargeLedCount;
	u8	ISChargingStatus;

	u8 	LedDelay;

	u8	AccBankStatus;
	u8	AccRstDeal;
	u8	AccCalcFlag;
	u16	AccOffTime;
	u16	AccOnTime;

	u8	VibRedoEnable;
	u8	VibSignFlag;
	u8	VibSignChk;
	u8	VibDataCount;
	u16	VibRedoCount;
	u8  VibWarnFlag;
    u16  VibWarnDelay;
	u8	a_VibCollectBuf[VIB_COLL_TIMES_MAX];

	u8	PofRedoEnable;
	u8	PofReHighFlag;
	u16	PofRedoCount;
	u16	PofAccCount;
	u16  PWOffCount;

    	bool VibLedHit;

	u8	VibWakeCount;		//用于振动唤醒的控制
	u8	VibWakeSignFlag;
	u8	a_VibWakeBuf[VIB_COLL_TIMES_MAX];

}t_Gpio_Ctrl_Manage;

extern void YS_IODealInit(void);

extern u8 YS_IOGetVibSign(void);

extern void YS_LedShowCtrl(void);

extern void YS_IOInputInfoDeal(void);

extern void YS_IOVibJudgeInit(void);

extern void YS_IODealVibSwitchInput(char VibID);

extern void YS_IOGeneralInit(u8 *dbuf);

extern void YS_IOGeneralWrite(u8 *dbuf);

extern u8 YS_IOGeneralRead(u8 *dbuf);

extern u8 YS_GetChargeStatue(void);

extern  void YS_SetPowoffKeyMsg(void);

extern void YS_SetShutDownLedFlag(void);

extern void YS_IOACCDydReCount(void);

extern void YS_IODealLedTimerHandler(void);
#endif

