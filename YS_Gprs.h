#ifndef _YS_GPRS_H
#define _YS_GPRS_H
#include "Data_type.h"

#define HBYD_SERV_CMD_REGACK            		0x01    //ƽ̨����ע��Ӧ��
#define HBYD_SERV_CMD_LOGACK				0x02	//ƽ̨���е�¼Ӧ��
#define HBYD_SERV_CMD_HEARTACK			0x03	//ƽ̨����������Ӧ��
#define HBYD_SERV_CMD_POSACK				0x04	//ƽ̨���ж�λ��Ӧ��
#define HBYD_SERV_CMD_GETPOS				0x05	//ƽ̨���е���
#define HBYD_SERV_CMD_PARAMREAD			0x06	//ƽ̨���в�����ȡ
#define HBYD_SERV_CMD_PARAMSET			0x07	//ƽ̨���в�������
#define HBYD_SERV_CMD_RSTTERI				0x08	//ƽ̨���и�λ�ն�
#define HBYD_SERV_CMD_BEGINTHIEF			0x09	//ƽ̨�������
#define HBYD_SERV_CMD_STOPTHIEF			0x0A	//ƽ̨���г���
#define HBYD_SERV_CMD_AREASET				0x0B	//ƽ̨������������
#define HBYD_SERV_CMD_AREACANLE			0x0C	//ƽ̨����ȡ������


#define SERV_UP_CMD_REG					0x01 	//ע���
#define SERV_UP_CMD_LOG					0x02 	//��¼��
#define SERV_UP_CMD_HEART					0x03	//������
#define SERV_UP_CMD_POS				 	0x04	//��λ���ݰ�
#define SERV_UP_CMD_ONEPOS				0x05	//��λ���ݰ�
#define SERV_UP_CMD_WARN					0x06	//�������ݰ�
#define SERV_UP_CMD_ONEGPSACK		    	0x07	//����Ӧ��
#define SERV_UP_CMD_THIEFACK				0x08	//���Ӧ��
#define SERV_UP_CMD_UNTHIEFACK		    	0x09	//����Ӧ��
#define SERV_UP_CMD_SETPRMACK				0x0A	//���ò���Ӧ��
#define SERV_UP_CMD_READPRMACK		    	0x0B	//��ȡ����Ӧ��
#define SERV_UP_CMD_SETAREAACK		    	0x0C	//��������Ӧ��
#define SERV_UP_CMD_CANLEAREAACK			0x0D	//����ȡ��Ӧ��
#define SERV_UP_CMD_ACK				    	0x0E 	//�ն�ͨ��Ӧ��
#define SERV_UP_CMD_MODEACK             		0x0f    //ģʽ�л�Ӧ��
#define SERV_UP_CMD_POSFRE				0x10	//λ�ñ���Ƶ��Ӧ��
#define SERV_UP_CMD_POSRSQ			    	0x11	//λ����Ϣ����
#define SERV_UP_CMD_DEADPOS			    	0x12	//λ����Ϣ����
#define SERV_UP_CMD_POS_QUEUE			0x13	//��λ���ݰ��������
#define SERV_UP_CMD_CARCTL              		0x14        //��������
#define SERV_UP_CMD_CAN                 0x15        //��̬CAN���ݰ�
#define SERV_UP_CMD_SIMINFO                 0x16    //SIM����Ϣ��

#define INTER_CMD_SET_JTB_PRM				0x1000
#define INTER_CMD_READ_JTB_PRM_COM		0x1001
#define INTER_CMD_READ_JTB_PRM_SERV		0x1002
#define INTER_CMD_SERV_JTB_REGACK			0x1004
#define INTER_CMD_SERV_JTB_POSACK			0x1005
#define INTER_CMD_SERV_JTB_HEARTACK		0x1006
#define INTER_CMD_SERV_JTB_UNREG			0x1007
#define INTER_CMD_SERV_JTB_LOGACK			0x1008
#define INTER_CMD_SERV_JTB_CTR			0x1009
#define INTER_CMD_SERV_JTB_MULTINFOACK	0x100A
#define INTER_CMD_SERV_JTB_MULDATAACK	0x100B
#define INTER_CMD_SERV_JTB_MULSENDACK	0x100C

#define INTER_CMD_GET_ONE_GPS				0x080A
#define INTER_CMD_GPS_HEART_ACK			0x0813
#define INTER_CMD_GPRS_GETPOS_BYTIME		0x0835
#define INTER_CMD_SET_HEART_TIME			0x0804

#define AVL_ADD_INFO_LEN        				150
#define AVL_MSG_BUF_MAX					1024

#define YS_GPRS_DEADPOS_LEN                 		16      //ä�� ���ĳ���
#define YS_GPRS_DEADPOS_TIME                		30      //ä�� ��¼��ʱ����
#define YS_GPRS_DEADPOS_NUM                 		500     //ä�� ������
#define YS_GPRS_DEADPOSBUF_LEN              	8016    //ä�� ���泤��

enum
{
    YS_CAR_PROTCOL_808, //808Э��
    YS_CAR_PROTCOL_TQ,  //����Э��

    YS_CAR_PROTCOL_NONE
};

enum
{
	AVL_WARM_SPEED,
	AVL_WARM_EXIGEN,
	AVL_WARM_LOWVOL,
	AVL_WARM_OFFVOL,
	AVL_WARM_THIEF,
	AVL_WARM_INAREA,
	AVL_WARM_OUTAREA,
	AVL_WARM_COURSE,
	AVL_WARM_STOP,
	AVL_WARM_MOVE,
	AVL_WARM_HIT,
	AVL_WARM_BUS_STATION,
	AVL_WARM_BUS_LOADBLOCK,
	AVL_WARM_BUS_WRANGLE,
	AVL_WARM_BUS_FULL,
	AVL_WARM_BUS_FAULT,
	AVL_WARM_BUS_ACCIDENT,
	AVL_WARM_BUS_HELP,
	AVL_WARM_BUS_OVERCAR,
	AVL_WARM_BUS_OVERSTATION,
	AVL_WARM_FORBIT_TIME,
	AVL_WARM_TEMPERATURE,
	AVL_WARM_LONGDRIVE,
	AVL_WARM_CARTURN,
	AVL_WARM_GPSEXCEPT,
	AVL_WARM_CARTRAILER,
	AVL_WARM_HITTEST,
	AVL_WARM_IGNITION,//�������
	AVL_WARM_ENGINE,//����Ϩ��
	AVL_WARM_DRAW,//�����γ�
	AVL_WARM_WITH,//��������
	AVL_WARM_GPSLONG,//��λ����
	AVL_WARM_IDLELONG,//����ʱ�����
	AVL_WARM_FLASH,
	AVL_WARM_CAN,
	AVL_WARM_GSENSER,
	AVL_WARM_GPSFAULT,
};

typedef struct
{
	u8  v_ProtoclFlag;
	u8  v_TypeID;
	u16 v_CmdID;
	u16 v_FlowID;
	u16 v_dlen;
      u8   a_AddInfo[AVL_ADD_INFO_LEN];
      u16  v_GetLen;
	u8   a_dbuf[AVL_MSG_BUF_MAX ];
}t_avltra_Parase;

typedef struct
{
	bool LogOKFlag;
	u16	LogSendDelay;
	u16	LogSendTimes;

	u16	HeartSetDelay;
	u16  HeartSendDelay;
	u16  HeartSendTimes;

	u16  PosSetDelay;
	u16  PosSendDelay;
	u16  PosSendTimes;
}t_Gprs_Server_Manage;

typedef struct
{
    u16     DeadPosSpeed;
    u16     DeadPosCourse;
    u8      DeadPosLati[4];
    u8      DeadPosLongi[4];
    u8      DeadPosTime[6];
    bool    DeadPosWarmStatus[30];
}t_Gprs_DeadPos_Pack;

typedef struct
{
    u8  ReadTimeInterval;       //��ʱ����
    u8  WriteTimeInterval;      //дʱ����
    u16 DeadPosNum;
    u16 DeadPosPread;
    u16 DeadPosPwrite;
    u32 DeadPosSize;
}t_Gprs_DeadPos_Manage;

extern void YS_GprsDeadPosIni(void);


extern void YS_GprsDeadPosResetRefresh(void);

extern void YS_GprsDeadPosFormat(void);

extern void YS_GprsDeadPosGetPackInfo(t_Gprs_DeadPos_Pack *t_DeadPosPackInfo);

extern void YS_GprsDeadPosGetInfo(t_Gprs_DeadPos_Manage *t_DeadPosInfo);

extern u16 YS_GprsDeadPosRunInfo(u8 *dbuf);

extern bool YS_GprsServerSendInterface(u8 CmdID, u8 *dbuf, u16 dlen);

extern void YS_GprsGpsServerDataInput(u8 *dbuf, u16 dlen);
#endif

