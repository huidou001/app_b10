#ifndef _YS_BLIND_H
#define _YS_BLIND_H
#include "YS_TypeDef.h"

#define BLIND_DATA_ZERO_POS		20			//盲区记录的文件启始存贮位置
#define BLIND_ONE_RECORD_SIZE		20
#define BLIND_FILE_SIZE				4000		//盲区补传文件大小
#define BLIND_RECORD_MAX_VALUE	199			//盲区记录的最大条数
#define BLIND_DATA_SEND_DELAY		3			//盲区数据上报间隔时间
#define BLIND_SEND_SAVE_MAX		20

#define BLIND_FLAG_ACC				0x0001		//ACC 状态
#define BLIND_FLAG_POWER			0x0002		//电源状态
#define BLIND_FLAG_SLEEP			0x0004		//休眠状态
#define BLIND_FLAG_THIEF			0x0008		//设防/撤防状态
#define BLIND_FLAG_GPS_EFFEC		0x0010		//定位标志
#define BLIND_FLAG_GPS_EWINFO		0x0020		//东西经标志
#define BLIND_FLAG_GPS_SNINFO		0x0040		//南北纬标志

#define BLIND_SAVE_NO_CON_DEALY	15			//未正常连接到服务器的存贮时间
#define BLIND_SAVE_NO_ACK_DELAY	45			//连接平台期间，位置数据包发送后，应答超时

enum
{
	BLIND_RECORD_TYPE_POS,
	BLIND_RECORD_TYPE_WARN,
	BLIND_RECORD_TYPE_NUM
};

typedef struct
{
	u16	RecordNum;
	u16	ReadPoint;
	u16  WritePoint;	
	u16	NoConSaveDelay;		//未正常连接平台，时记录补传数据
	u16	PosSendSaveDelay;		//正常连接平台，定位包未应答的补传数据
	u8	PosSendFlag;			//已发送位置数据的标志
	u8	PosAckFlag;				//位置数据已应答的标志
	u16	BlindSendDelay;			//盲区数据上报延时
	u8	EnableFlag;				//允许盲区处理的标志,当系统进入复位TIMER 后，将禁止盲区管理功能
}t_Blind_Manage;


typedef struct
{
	u8 		RecordType;
	u8		DateTime[6];
	u8		Longitude[4];
	u8		Latitude[4];
	u8		Speed;
	u8		Course;
	u16		StatusInfo;
	u8		Reserve1;
}t_Blind_Record;

typedef struct
{
	u8		RecNum;
	u8		AddLoseFlag;
	u16		NoNewLoseDelay;	
	u8		SaveBuf[BLIND_SEND_SAVE_MAX][BLIND_ONE_RECORD_SIZE];
}t_Blind_Send_Save;

extern void YS_BlindDealInit(void);

extern void YS_BlindPosSendInterface(void);

extern void YS_BlindPosAckInterface(void);

extern void YS_BlindTimerInput(void);

extern void YS_BlindSendSaveDataAdd(void);

extern void YS_BlindManageResetInterface(void);

extern u16 YS_BlindGetRunInfo(u8 *dbuf);

extern void YS_BlindFormatInfoDeal(void);

#endif
