#ifndef YS_SMS_H
#define YS_SMS_H
#include "Data_type.h"

#define SMS_MUL_RECORD_NUM		5
#define SMS_DEFAULT_SEND_DELAY	15

typedef struct
{
	bool FormChangeFlag;
	u8 	PhoneBuf[40];
	u8 	PhoneLen;
	u16	SmsConLen;
	u8   SmsConBuf[512];
}t_Sms_Record;

typedef struct  //短信多条发送管理,防止短信发送冲突
{
	bool RstFlag;
	u8  Enable;
	u8  ReadPoint;
	u8  WritePoint;
	u8  RecNum;
	u8  AckFlag;
	u8  SendDelay;
	t_Sms_Record SmsRecBuf[SMS_MUL_RECORD_NUM];
}t_Sms_Mul_Send_Manage;

typedef struct
{
	char   RsqPhone[20];
	u8	  RsqFlag;
	u16 	  TextLen;
	u8	  TextBuf[200];
}t_Sms_Pos_Rsq;

typedef struct
{
	bool		ActFlag;	
	bool		OwnAckFlag;
	u8		WaitDelay;
	u8		ActiveType;
	u8		SendOldFlag;
	u8		SendOldDelay;
	u16        GpsOnTime;
}t_Sms_Active_Rsq;

typedef struct
{
	u8		NewFlag;
	char		PhoneBuf[40];
	char		ConBuf[160];
}t_Sms_Com_Moni;

enum  //定义注册短信内容出错代码
{
	ZC_SMS_ERR_ID,
	ZC_SMS_ERR_PW,
	ZC_SMS_ERR_SIM,
	ZC_SMS_ERR_OWNER,
	ZC_SMS_ERR_SIM_SELF,
	ZC_SMS_INFO_OK,
	ZC_SMS_BANK_OWNER,
	ZC_SMS_AUTO_SUCC,
	SF_SMS_ACK,
	SF_SMS_NO_DEAL,
	CF_SMS_ACK,
	CF_SMS_NO_DEAL,
	SZMM_SMS_ACK_ERR1,
	SZMM_SMS_ACK_ERR2,
	SZMM_SMS_ACK_SUCC,
	FORMAT_SMS_ACK,
	RST_SMS_ACK,
	SETPRM_SMS_ACK_PWERR,
	SETPRM_SMS_ACK_CONERR,
	READPRM_SMS_ACK_CONERR,
	SZ_SMS_ACK_LOGSUCC,
	DW_SMS_ERR_NOPOS,
	DW_SMS_ERR_CONFAIL,
	DW_SMS_SUCC_DEAL,
	ZC_SMS_CMD_CHG_PRO
};

extern void YS_SmsManageInit(void);

extern void YS_SmsDealTimerInput(void);

extern void YS_SmsComInputSimuLation(u8 *dbuf, u16 dlen);

extern void YS_SmsSendTestInterface(u8 *dbuf, u16 dlen);

extern void  YS_SmsComeInterface(char* sFromNumber,char* sContent);

extern bool YS_SmsCompString(u8 *PackBuf, u16 dlen, u16 CompLen);

extern void YS_SmsSendRltAck(u8 ID);

extern void YS_SmsPosRsqSendInterface(u8 *dbuf, u16 dlen, u8 *PhoneCode, u8 PhoneLen, u8 SmsEvent,bool FormChangeFlag);

//extern u16 YS_GB2UniCode(u8 *UniBuf, u8 *GbAsiBuf, u16 GbAsiBufLen);

extern void YS_SmsSendComMoniCtrl(void);

extern void YS_SmsAckSmsDeal(u8 RltID, u8 *PhoneCode, u8 PhoneLen);

extern void YS_SmsSendInterface(u8* pContent, u16 pContentLen, u8 * pNumber, u8 pNumberLen,bool FormChangeFlag);

#endif

