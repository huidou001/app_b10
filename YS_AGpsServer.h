#ifndef _YS_AGPSSERVER_H
#define _YS_AGPSSERVER_H

#include "Data_type.h"
#define UBLOX_SERVER_DNS_STR "agps.u-blox.com"
//#define UBLOX_SERVER_DNS_STR "98-po2up4ou"

#define AGPS_SOCKET_DELAY_DEF		30
#define AGPS_DATA_SEND_DELAY		15
#define AGPS_DATA_SEND_TIMES		1

enum
{
	AGPS_FUNC_NONE=0,
	AGPS_FUNC_UBLOX,
	AGPS_FUNC_XINDOU
};

typedef struct
{
	u8	RunFlow;
	u8 	AGpsFlag;
	u8	AGpsSendTimes;
	u8	AGpsSendDelay;
	u8	DelayCount;
	u8	SckOKFlag;
	u8	DealOKFlag;
	u8	DealEnable;
	u8	DnsOKFlag;
	u8	DnsFailFlag;
	u8	SckCreateFlag;
	u16	RecDataLen;
	u16 SendStarPos;
}t_AGps_Manage;

enum
{
	AGPS_RSQ_FLOW_INIT,
	AGPS_RSQ_FLOW_BEGIN,
	AGPS_RSQ_FLOW_PPP,
	AGPS_REQ_FLOW_DNS,
	AGPS_REQ_FLOW_DNS_WAIT,
	AGPS_RSQ_FLOW_SCOKET,
	AGPS_RSQ_FLOW_SCOKET_WAIT,
	AGPS_RSQ_FLOW_RSQDATA,
	AGPS_RSQ_FLOW_RSQDATA_WAIT,
	AGPS_RSQ_FLOW_SYNC_CHIP,
	AGPS_RSQ_FLOW_SYNC_CHIP_WAIT,
	AGPS_RSQ_FLOW_EXIT
};

extern void YS_AGpsDealInitDeal(void);

extern void YS_AGpsDealInterFace(void);

extern void YS_AGpsGetRunFlow(void);

extern void YS_AGpsServerConnectMsgDeal(void);

extern void YS_AGpsServerDataParase(u8 *dbuf, u16 dlen);

extern void YS_AGpsSocketClose(void);

extern void YS_RunAGpsSendInterface(u8 *dbuf, u16 dlen);

extern void YS_AGpsSetDnsOKFlag(void);

extern void YS_AGpsSetDnsFailFlag(void);

extern void YS_AGpsFunctionExit(void);



#endif
