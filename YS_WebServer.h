#ifndef _YS_WEBSERVER_H
#define _YS_WEBSERVER_H

#include "Data_type.h"

#define Web_Cmd_Buf_NUM	5
#define Web_SCK_WAIT_DELAY	30
#define Web_RSQ_SEND_DELAY	120
#define Web_RSQ_SEND_TIMES	1
#define Web_Exit_Wait_Delay		5

#if 1   //dog care
#define WEBSTR_RSQ    "POST http://qq.dogcare.com.cn/zhf/A20/A20.bin HTTP/1.1"
#define WEBSTR_UPDATE "POST http://qq.dogcare.com.cn/zhf/A20/jt_ads.bin HTTP/1.1"

#define WEBSTR_1 "Host:http://www.dogcare.com.cn"
#define WEBSTR_2 "User-Agent:SQH_D480B_01/LB19504/WAP2.0Profile"
#define WEBSTR_3 "Accept:image/gif, image/jpeg, imagepeg, imagepeg, application/x-shockwave-flash, applicationnd.ms-excel, applicationnd.ms-powerpoint, application/msword, */*"
#define WEBSTR_4 "Accept-Charset:utf-8,utf-16,iso-8859-1,iso-10646-ucs-2,GB2312,windows-1252,us-ascii"
#define WEBSTR_5 "Accept-Language:zh-tw,zh-cn,en"
#define WEBSTR_6 "Connection:Keep-Alive"
#endif

#if 0
#define WEB_SERVER_DNS_STR "afusece.duapp.com"

#define WEBSTR_RSQ    "POST http://afusece.duapp.com/1548/a10/rsq.bin HTTP/1.1"
#define WEBSTR_UPDATE "POST http://afusece.duapp.com/1548/a10/jt_ads.bin HTTP/1.1"

#define WEBSTR_1 "Host:http://afusece.duapp.com"
#define WEBSTR_2 "User-Agent:SQH_D480B_01/LB19504/WAP2.0Profile"
#define WEBSTR_3 "Accept:image/gif, image/jpeg, imagepeg, imagepeg, application/x-shockwave-flash, applicationnd.ms-excel, applicationnd.ms-powerpoint, application/msword, */*"
#define WEBSTR_4 "Accept-Charset:utf-8,utf-16,iso-8859-1,iso-10646-ucs-2,GB2312,windows-1252,us-ascii"
#define WEBSTR_5 "Accept-Language:zh-tw,zh-cn,en"
#define WEBSTR_6 "Connection:Keep-Alive"
#endif

enum
{
	WEB_RSQ_FLOW_SELCMD,
	WEB_RSQ_FLOW_BEGIN,
	WEB_RSQ_FLOW_PPP,
	WEB_RSQ_FLOW_DNS_BEGIN,
	WEB_RSQ_FLOW_DNS_WAIT,
	WEB_RSQ_FLOW_SCOKET,
	WEB_RSQ_FLOW_SCOKET_WAIT,
	WEB_RSQ_FLOW_RSQ,
	WEB_RSQ_FLOW_RSQ_WAIT,
	WEB_RSQ_FLOW_RSQDATA,
	WEB_RSQ_FLOW_RSQDATA_WAIT,
	WEB_RSQ_FLOW_EXIT
};

typedef struct
{
	u8 	    a_PhoneData[Web_Cmd_Buf_NUM][20];
	u8 	    a_PhoneLen[Web_Cmd_Buf_NUM];
	u8	    a_EventFlag[Web_Cmd_Buf_NUM];
	u8	    CmdReadPoint;
	u8	    CmdWritePoint;
	u8	    CmdNum;
	u8	    RsqFlow;
	bool	DnsAddrFlag;
	u8		DnsIPInfo[4];
	bool	DnsOKFlag;
	u8      DnsFailFlag;
	u8      DnsErrTimes;
	u8	    RsqSendTimes;
	u8	    RsqSendDelay;
	u8	    RsqOKFlag;
	u8	    RsqUpdateOKFlag;
	u8	    RsqFailFlag;
	u8	    RsqDataOKFlag;
	u8	    RsqDataFailFlag;
	u8	    DelayCount;
	u8	    SckOKFlag;
	u8 	    EventFlag;
	u32     CrcCode;
}t_Web_Server_Manage;

extern t_Web_Server_Manage	t_WebManage;

extern void YS_WebServiceRsqFlow(void);

extern void YS_WebDWRsqAdd(u8 *PhoneData, u8 PhoneLen, u8 EventFlag);

extern void YS_WebServiceInitParam(void);

extern void YS_WebServiceSckConnectMsg(void);

extern void YS_LongToPosStr(u8 *Pos, char *BackStr);

extern void	YS_WebServiceDataParase(u8 *buf, u16 len);

extern void YS_WebAddRequest(void);

extern void YS_WebSocketClose(void);

extern void YS_WebUpdateRsq(void);
#endif
