#ifndef YS_AUTO_UPDATE_H
#define YS_AUTO_UPDATE_H
#include "Data_type.h"

#define AUTO_RSQ_SOCKET_DELAY		30
#define AUTO_RSQ_LOGIN_TIMES		3
#define AUTO_RSQ_LOGIN_DELAY   	15

typedef struct
{
	u8 	WorkEnable;
	u8 	WorkFlow;
	u8 	SocketOKFlag;	
	u8	RsqInfoOKFlag;
	u8	RsqInfoFailFlag;
	u8	RsqInfoUpdateFlag;
	u8	RsqInfoTimes;
	u8	RsqInfoDelay;
	u16 	OptDelay;
}t_Auto_Update_Manage;

extern void YS_AutoUpdateSocketAck(void);

extern void YS_AutoInfoRsqAckDeal(u8 *dbuf, u16 dlen);

extern void YS_AutoUpdateFlowCtrl(void);

extern void YS_AutoUpdateInitParam(void);

extern void YS_AutoUpdateRunInterface(void);

#endif

