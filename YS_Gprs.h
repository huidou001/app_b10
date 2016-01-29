#ifndef _YS_GPRS_H
#define _YS_GPRS_H
#include "Data_type.h"

#define HBYD_SERV_CMD_REGACK            		0x01    //平台下行注册应答
#define HBYD_SERV_CMD_LOGACK				0x02	//平台下行登录应答
#define HBYD_SERV_CMD_HEARTACK			0x03	//平台下行心跳包应答
#define HBYD_SERV_CMD_POSACK				0x04	//平台下行定位包应答
#define HBYD_SERV_CMD_GETPOS				0x05	//平台下行点名
#define HBYD_SERV_CMD_PARAMREAD			0x06	//平台下行参数读取
#define HBYD_SERV_CMD_PARAMSET			0x07	//平台下行参数设置
#define HBYD_SERV_CMD_RSTTERI				0x08	//平台下行复位终端
#define HBYD_SERV_CMD_BEGINTHIEF			0x09	//平台下行设防
#define HBYD_SERV_CMD_STOPTHIEF			0x0A	//平台下行撤防
#define HBYD_SERV_CMD_AREASET				0x0B	//平台下行区域设置
#define HBYD_SERV_CMD_AREACANLE			0x0C	//平台下行取消区域


#define SERV_UP_CMD_REG					0x01 	//注册包
#define SERV_UP_CMD_LOG					0x02 	//登录包
#define SERV_UP_CMD_HEART					0x03	//心跳包
#define SERV_UP_CMD_POS				 	0x04	//定位数据包
#define SERV_UP_CMD_ONEPOS				0x05	//定位数据包
#define SERV_UP_CMD_WARN					0x06	//报警数据包
#define SERV_UP_CMD_ONEGPSACK		    	0x07	//点名应答
#define SERV_UP_CMD_THIEFACK				0x08	//设防应答
#define SERV_UP_CMD_UNTHIEFACK		    	0x09	//撤防应答
#define SERV_UP_CMD_SETPRMACK				0x0A	//设置参数应答
#define SERV_UP_CMD_READPRMACK		    	0x0B	//读取参数应答
#define SERV_UP_CMD_SETAREAACK		    	0x0C	//区域设置应答
#define SERV_UP_CMD_CANLEAREAACK			0x0D	//区域取消应答
#define SERV_UP_CMD_ACK				    	0x0E 	//终端通用应答
#define SERV_UP_CMD_MODEACK             		0x0f    //模式切换应答
#define SERV_UP_CMD_POSFRE				0x10	//位置报文频率应答
#define SERV_UP_CMD_POSRSQ			    	0x11	//位置信息请求
#define SERV_UP_CMD_DEADPOS			    	0x12	//位置信息请求
#define SERV_UP_CMD_POS_QUEUE			0x13	//定位数据包插入队列
#define SERV_UP_CMD_CARCTL              		0x14        //车辆控制
#define SERV_UP_CMD_CAN                 0x15        //静态CAN数据包
#define SERV_UP_CMD_CANOFF                 0x16        //熄火
#define SERV_UP_CMD_SIMINFO                 0x17    //SIM卡信息包
#define SERV_UP_CMD_OBDINFO                 0x18    //读取指定OBD数据

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

#define YS_GPRS_DEADPOS_LEN                 		16      //盲点 报文长度
#define YS_GPRS_DEADPOS_TIME                		30      //盲点 记录的时间间隔
#define YS_GPRS_DEADPOS_NUM                 		500     //盲点 最大个数
#define YS_GPRS_DEADPOSBUF_LEN              	8016    //盲点 缓存长度

enum
{
    YS_CAR_PROTCOL_808, //808协议
    YS_CAR_PROTCOL_TQ,  //天琴协议

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
	AVL_WARM_IGNITION,//汽车点火
	AVL_WARM_ENGINE,//汽车熄火
	AVL_WARM_DRAW,//车机拔出
	AVL_WARM_WITH,//车机接上
	AVL_WARM_GPSLONG,//定位过长
	AVL_WARM_IDLELONG,//怠速时间过长
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
    u8  ReadTimeInterval;       //读时间间隔
    u8  WriteTimeInterval;      //写时间间隔
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

