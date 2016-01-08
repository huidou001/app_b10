#ifndef _YS_BLIND_H
#define _YS_BLIND_H
#include "YS_TypeDef.h"

#define BLIND_DATA_ZERO_POS		20			//ä����¼���ļ���ʼ����λ��
#define BLIND_ONE_RECORD_SIZE		20
#define BLIND_FILE_SIZE				4000		//ä�������ļ���С
#define BLIND_RECORD_MAX_VALUE	199			//ä����¼���������
#define BLIND_DATA_SEND_DELAY		3			//ä�������ϱ����ʱ��
#define BLIND_SEND_SAVE_MAX		20

#define BLIND_FLAG_ACC				0x0001		//ACC ״̬
#define BLIND_FLAG_POWER			0x0002		//��Դ״̬
#define BLIND_FLAG_SLEEP			0x0004		//����״̬
#define BLIND_FLAG_THIEF			0x0008		//���/����״̬
#define BLIND_FLAG_GPS_EFFEC		0x0010		//��λ��־
#define BLIND_FLAG_GPS_EWINFO		0x0020		//��������־
#define BLIND_FLAG_GPS_SNINFO		0x0040		//�ϱ�γ��־

#define BLIND_SAVE_NO_CON_DEALY	15			//δ�������ӵ��������Ĵ���ʱ��
#define BLIND_SAVE_NO_ACK_DELAY	45			//����ƽ̨�ڼ䣬λ�����ݰ����ͺ�Ӧ��ʱ

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
	u16	NoConSaveDelay;		//δ��������ƽ̨��ʱ��¼��������
	u16	PosSendSaveDelay;		//��������ƽ̨����λ��δӦ��Ĳ�������
	u8	PosSendFlag;			//�ѷ���λ�����ݵı�־
	u8	PosAckFlag;				//λ��������Ӧ��ı�־
	u16	BlindSendDelay;			//ä�������ϱ���ʱ
	u8	EnableFlag;				//����ä������ı�־,��ϵͳ���븴λTIMER �󣬽���ֹä��������
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
