#ifndef _YS_SYSFACE_H
#define _YS_SYSFACE_H
#include "Data_type.h"

enum
{
	YS_RST_FLAG_TIMING,
	YS_RST_FLAG_USER,
	YS_RST_FLAG_REG,
	YS_RST_FLAG_SOCKET,
	YS_RST_FLAG_PPP,
	YS_RST_FLAG_ACTIVE,
	YS_RST_FLAG_FORMAT,
	YS_RST_FLAG_SOCKET_FULLTIMES,
	YS_RST_FLAG_SER_LOG_FAILLTIMES,
	YS_RST_FLAG_DNS_ERR_FULLTIMES,
	YS_RST_FLAG_LOSE_NET,
	YS_RST_FLAG_ATUO_REG,
	YS_RST_FLAG_DOWNLOAD,
	YS_RST_FLAG_NUM
};

typedef struct
{
	u8	RstFlag;
	u8	RstDelay;
	u8	WatchDogDelay;
	u8   FactoryParam;
	u8	CaliDataFlag;
}t_Ys_Rst_Manage;

/*cell info structure*/
typedef struct{
    u16 	arfcn;           /*ARFCN*/
    u8 	bsic;              /*BSIC*/
    u8 	rxlev;            /*Received signal level*/
    u16 	mcc;            /*MCC*/
    u16 	mnc;            /*MNC*/
    u16 	lac;              /*LAC*/
    u16 	ci;                /*CI*/
    u8      ta;
    u8      NrbCellNum;
}t_Cell_Info_Manage;


extern void YS_SysRsqSystemRST(u8 RstFlag);

extern void YS_SysRstManageCtrl(void);

extern void YS_SysRstManageInit(void);

extern void YS_SysGetBarCodeInterface(void);

extern void YS_SysGetBarCodeCtrl(void);

extern void YS_SysSyncServerDatetime(char *DateStr);

extern void YS_SysSyncSystemDT(void);

extern void YS_SysGetCaliBartionDataInterface(void);

extern u16 YS_SysPackNetInfo(u8 *PackBuf);

extern u16 YS_SysGetSystemTimeToString(char *PackBuf);

extern void YS_SysGprsSyncSystemDT(u8 *buf);
#endif

