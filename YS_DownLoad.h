#ifndef _YS_DOWN_LOAD_H
#define _YS_DOWN_LOAD_H
#include "Data_type.h"

#define DW_ONE_FILE_PACK_LEN			512
#define DW_ONE_FILE_GROUP_NUM		10
#define DW_PACK_STOR_UNKNOW			0xFF
#define DW_SOCKET_MAX_TIMES			5


#define DW_PTL_VER_DEF					0x01

#define DW_UP_CMD_LOGIN				0x01
#define DW_UP_CMD_RSQ_INFO			0x02
#define DW_UP_CMD_RSQ_FILE			0x03
#define DW_UP_CMD_EXIT_LOG			0x04
#define DW_UP_CMD_AUTO_RSQ			0x10

#define DW_UP_CMD_LOGIN_ACK			0x81
#define DW_UP_CMD_RSQ_INFO_ACK		0x82
#define DW_UP_CMD_RSQ_FILE_ACK		0x83
#define DW_UP_CMD_EXIT_LOG_ACK		0x84
#define DW_UP_CMD_AUTO_RSQ_ACK		0x90

typedef struct
{
	u16	PackNO;
	u16  PackLen;
	u8	PackBuf[DW_ONE_FILE_PACK_LEN];
}t_Down_One_Package;

typedef struct
{
	u8	GroupBufPackNum;
	u8	GroupBufPackWP;	
	t_Down_One_Package t_DownOnePack[DW_ONE_FILE_GROUP_NUM];
}t_Down_One_Group;

typedef struct
{
	u8	FirstRsqFlag;
	u8	SocketTimes;
	u8	SocketOKFlag;
	u32  FileLen;
	u32  FileWritePos;
	u16	TotalGroupNum;	
	u16	CurGroupNum;
	u16	CurGroupNO;
	u16	TotalPackNum;
	u16	CurPackNum;
	u16	CurPackNO;
	u8	DownFileVerify;
	u8	CalcFileVerify;
	u8	RsqGroupDelay;
	u8	RsqGroupTimes;
	u8	RsqGroupOKFlag;
	u8	RsqGroupFailFlag;	
}t_Update_Manage;

extern void YS_DWAppInitDeal(void);

extern void YS_DWFlowCtrlDeal(void);

extern void YS_DWAppSocketAck(void);

extern void YS_DWAppServerDataInput(u8 *dbuf, u16 dlen);

extern void YS_DWUpdateResultDeal(u8 Rlt);

extern u16 YS_DWServerPackAddID(u8 *PackBuf);

extern u16 YS_DWServerPackAddPrjName(u8 *PackBuf);
#endif


