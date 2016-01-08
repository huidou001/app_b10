/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_AGpsServer.c  2013-08-25
���ļ����ڹ���GPS������λ���ݵĻ�ȡ
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

#define AGPS_FIR_STRING	"A R 4C54:"
#define AGPS_SEC_STRING	"356946010000001"
#define AGPS_THI_STRING	"|AGPS|"
#define AGPS_BUF_MAX_LEN	4096

#define UBLOX_RSQ_FIR		"user=1010708517@qq.com;pwd=Dztomo;cmd=aid;"

#define BDUC221_RSQ_FIR    "GET /UR370_HBase HTTP/1.0\r\n"

#define BDUC221_RSQ_MID    "POS:"

#define BDUC221_RSQ_END    "Authorization: Basic Mjg0OTQ1MjI1MEBxcS5jb206TDB2Rm1zMHNrVW9TMUpnZQ==\r\n\r\n"

u8	AGps_Server_Buf[AGPS_BUF_MAX_LEN];
t_AGps_Manage	t_AGpsManage;

void YS_AGpsServerDataDebug(u8 *dbuf, u16 dlen);

/*-----------------------------------------------------------------------------------------
��������YS_AGpsGetServerType
����˵������ȡ��������������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_AGpsGetServerType(void)
{
    u8 fbuf[4];
    YS_PrmReadOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);	//AGPS ����
    fbuf[0] = 3;
    return(fbuf[0]);
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsDealInterFace
����˵��������AGpsƽ̨�������ں���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsDealInitDeal(void)
{
    t_AGpsManage.DealEnable=0;
    t_AGpsManage.AGpsFlag=0;
    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_INIT;
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsDealInterFace
����˵��������AGpsƽ̨�������ں���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsDealInterFace(void)
{
    u8 fbuf[4];
    YS_PrmReadOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);	//AGPS ����

    if(YS_RunGetGpsPower()==FALSE)
    {
        YS_UartDebugString("gps pow is off");
//		return;
    }
    switch(fbuf[0])	//���ݷ��������ͷ��Ͷ�Ӧ���ݰ�
    {
        case AGPS_FUNC_UBLOX:
        case AGPS_FUNC_XINDOU:
        case AGPS_FUNC_BDUC221:
            t_AGpsManage.AGpsFlag=1;
            break;

        default:
            break;
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsCalcVerify
����˵��������У��ֵ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_AGpsCalcVerify(u8 *dbuf,u16 dlen)
{
    u16 i,verify;

    verify=0;
    for(i=0; i<(dlen-4); i++)
    {
        verify+=dbuf[i+4];
    }
    return(verify);
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsXinDouPackSendDeal
����˵����ƽ̨������������ʹ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsXinDouPackSendDeal(void)
{
    u8 SendBuf[200];
    u16 pos,i,len,Verify;
    char StrDat[50];
    u8 fbuf[12];
    char LongStr[20],LatiStr[20];
    t_Sys_Run_Status	t_GetStatus;

    YS_RunGetSystemStatus(&t_GetStatus);

    pos=0;
    len=strlen(AGPS_FIR_STRING);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=AGPS_FIR_STRING[i];
        pos++;
    }

    len=strlen(AGPS_SEC_STRING);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=AGPS_SEC_STRING[i];
        pos++;
    }

    len=strlen(AGPS_THI_STRING);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=AGPS_THI_STRING[i];
        pos++;
    }

    sprintf(StrDat,"%d-%d-%d-%d|",t_GetStatus.CountryName,t_GetStatus.NetName,t_GetStatus.AreaID,t_GetStatus.CellID);
    len=strlen(StrDat); //modify by hehongjun at 2014-05-15
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=StrDat[i];
        pos++;
    }

    YS_PrmReadOneItem(FLH_PRM_LAST_GPSPOINT,FLH_PRM_LAST_GPSPOINT_LEN,fbuf);
    YS_LongToPosStr(&fbuf[0],LongStr);
    YS_LongToPosStr(&fbuf[4],LatiStr);
    sprintf(StrDat,"%s|%s|",LatiStr,LongStr);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=StrDat[i];
        pos++;
    }

    YS_PrmReadOneItem(FLH_PRM_NV_LAST_POINT_DT,FLH_PRM_NV_LAST_POINT_DT_LEN,fbuf);
    sprintf(StrDat,"20%02d-%02d-%02d %02d:%02d:%02d|",fbuf[0],fbuf[1],fbuf[2],fbuf[3],fbuf[4],fbuf[5]);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=StrDat[i];
        pos++;
    }

    Verify=YS_AGpsCalcVerify(SendBuf,pos);
    sprintf(StrDat,"%x",Verify);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=StrDat[i];
        pos++;
    }
    SendBuf[pos]='\r';
    pos++;
    SendBuf[pos]='\n';
    pos++;
    YS_RunAGpsSendInterface(SendBuf,pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsUbloxSendDeal
����˵����ƽ̨������������ʹ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsUbloxSendDeal(void)
{
    u8 SendBuf[200];
    u16 i,len,pos;
    char StrDat[100],LongStr[20],LatiStr[20];
    u8 fbuf[12];

    pos=0;
    len=strlen(UBLOX_RSQ_FIR);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=UBLOX_RSQ_FIR[i];
        pos++;
    }

    YS_PrmReadOneItem(FLH_PRM_LAST_GPSPOINT,FLH_PRM_LAST_GPSPOINT_LEN,fbuf);
    YS_LongToPosStr(&fbuf[0],LongStr);
    YS_LongToPosStr(&fbuf[4],LatiStr);
    sprintf(StrDat,"lat=%s;lon=%s;pacc=%d\n",LatiStr,LongStr,30000);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=StrDat[i];
        pos++;
    }

    YS_RunAGpsSendInterface(SendBuf,pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsBDUC221SendDeal
����˵����ƽ̨������������ʹ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsBDUC221SendDeal(void)
{
    u8 SendBuf[200];
    u16 i,len,pos;
    char StrDat[100],LongStr[20],LatiStr[20];
    u8 fbuf[12];

    pos=0;
    len=strlen(BDUC221_RSQ_FIR);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=BDUC221_RSQ_FIR[i];
        pos++;
    }

    len=strlen(BDUC221_RSQ_MID);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=BDUC221_RSQ_MID[i];
        pos++;
    }

    YS_PrmReadOneItem(FLH_PRM_LAST_GPSPOINT,FLH_PRM_LAST_GPSPOINT_LEN,fbuf);
    YS_LongToPosStr(&fbuf[0],LongStr);
    YS_LongToPosStr(&fbuf[4],LatiStr);
    sprintf(StrDat,"%s %s\r\n",LatiStr,LongStr);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=StrDat[i];
        pos++;
    }

    len=strlen(BDUC221_RSQ_END);
    for(i=0; i<len; i++)
    {
        SendBuf[pos]=BDUC221_RSQ_END[i];
        pos++;
    }

    YS_RunAGpsSendInterface(SendBuf,pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsSetDnsOKFlag
����˵��������DNS �����ɹ���־
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsSetDnsOKFlag(void)
{
    t_AGpsManage.DnsOKFlag=TRUE;
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsSetDnsFailFlag
����˵��������DNS ����ʧ�ܱ�־
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsSetDnsFailFlag(void)
{
    t_AGpsManage.DnsFailFlag=TRUE;
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsFunctionExit
����˵�����˳�AGPS ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsFunctionExit(void)
{
    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_INIT;
    t_AGpsManage.AGpsFlag=0;
    if(t_AGpsManage.SckCreateFlag==1)
    {
        t_AGpsManage.SckCreateFlag=0;
        YS_AGpsSocketClose(); //�ر�SOCKET ����
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsRsqPosStatus
����˵������ѯ��ǰ��GPS ��λ״̬
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_AGpsRsqPosStatus(void)
{
    t_Gps_Data_Info t_GpsData;

    YS_GpsGetPosData(&t_GpsData);
    return(t_GpsData.Effective);
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsGetRunFlow
����˵�����˺������ڹ���AGps���ݵĴ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsGetRunFlow(void)
{
    u8 	ServerType;

//    ycsj_debug("AGPS RUN FLOW:%d\r\n", t_AGpsManage.RunFlow);
    switch(t_AGpsManage.RunFlow)
    {
        case AGPS_RSQ_FLOW_INIT:
            t_AGpsManage.SckCreateFlag=0;
            if(t_AGpsManage.AGpsFlag==1)
            {
                if(YS_AGpsRsqPosStatus()==TRUE) //����Ѷ�λ�����APGS ����
                {
                    YS_UartDebugString("Gps is effective not run agps");
                }
                else
                {
                    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_BEGIN;
                }
                t_AGpsManage.AGpsFlag=0;
            }
            break;

        case AGPS_RSQ_FLOW_BEGIN:
            YS_UartDebugString("AGPS function begin!!!");
            ServerType=YS_AGpsGetServerType();
            ycsj_debug("ServerType:%d\r\n", ServerType);
            switch(ServerType)
            {
                case AGPS_FUNC_UBLOX:
                    t_AGpsManage.RunFlow=AGPS_REQ_FLOW_DNS;
                    break;

                case AGPS_FUNC_XINDOU:
                case AGPS_FUNC_BDUC221:
                    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_SCOKET;
                    break;

                default:
                    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_INIT;
                    break;
            }
            break;

        case AGPS_REQ_FLOW_DNS: //DNS����
            if(YS_RunGetAGpsAPExistFlag()==TRUE)
            {
                YS_UartDebugString("Agps dns ok with exist IP");
                t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_SCOKET;
            }
            else
            {
                t_AGpsManage.DelayCount=0;
                t_AGpsManage.DnsOKFlag=FALSE;
                t_AGpsManage.DnsFailFlag=FALSE;
                YS_RunUbloxDnsInterface();
                t_AGpsManage.RunFlow=AGPS_REQ_FLOW_DNS_WAIT;
            }

            break;

        case AGPS_REQ_FLOW_DNS_WAIT: //DNS ����ȴ�
            if(t_AGpsManage.DnsOKFlag==TRUE)
            {
                t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_SCOKET;  //����SOCKET
            }
            else if(t_AGpsManage.DnsFailFlag==TRUE)
            {
                t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_INIT; //DNS ʧ�ܣ��˳�
            }
            else
            {
                t_AGpsManage.DelayCount++;
                if(t_AGpsManage.DelayCount>=60)
                {
                    YS_UartDebugString("Dns parase fulltime exit!!!");
                    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_INIT;
                }
            }
            break;

        case AGPS_RSQ_FLOW_SCOKET:
            t_AGpsManage.DelayCount=0;
            t_AGpsManage.SckOKFlag=0;
            t_AGpsManage.SckCreateFlag=1;
            YS_RunAGpsSocketInterface();//����SOCKET ����
            t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_SCOKET_WAIT;
            break;

        case AGPS_RSQ_FLOW_SCOKET_WAIT:
            if(t_AGpsManage.SckOKFlag==1)
            {
                YS_UartDebugString("AGPS Socket ok!!!");
                t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_RSQDATA;
            }
            else
            {
                t_AGpsManage.DelayCount++;
                if(t_AGpsManage.DelayCount>=AGPS_SOCKET_DELAY_DEF)
                {
                    YS_RunSetAGpsAPExitFlag(FALSE);
                    YS_UartDebugString("AGPS Socket fail!!!");
                    t_AGpsManage.DelayCount=0;
                    t_AGpsManage.SckCreateFlag=0;
                    YS_AGpsSocketClose(); //�ر�SOCKET ����
                    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_INIT;
//				t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_EXIT;
                }
            }
            break;

        case AGPS_RSQ_FLOW_RSQDATA:
            t_AGpsManage.AGpsSendDelay=AGPS_DATA_SEND_DELAY;
            t_AGpsManage.AGpsSendTimes=0;
            t_AGpsManage.RecDataLen=0;
            t_AGpsManage.DelayCount=0;
            t_AGpsManage.DealEnable=1;
            t_AGpsManage.DealOKFlag=0;
            ServerType=YS_AGpsGetServerType();
            switch(ServerType)	//���ݷ��������ͷ��Ͷ�Ӧ���ݰ�
            {
                case AGPS_FUNC_UBLOX:
                    YS_AGpsUbloxSendDeal();
                    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_RSQDATA_WAIT;
                    break;

                case AGPS_FUNC_XINDOU:
                    YS_AGpsXinDouPackSendDeal();
                    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_RSQDATA_WAIT;
                    break;

                case AGPS_FUNC_BDUC221:
                    YS_AGpsBDUC221SendDeal();
                    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_RSQDATA_WAIT;
                    break;

                default:
                    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_INIT;
                    break;
            }
            break;

        case AGPS_RSQ_FLOW_RSQDATA_WAIT:
            if(t_AGpsManage.DealOKFlag==1)
            {
                YS_RunSetAGpsAPExitFlag(TRUE);
                YS_UartDebugString("AGPS deal ok!!!");
                t_AGpsManage.SckCreateFlag=0;
                YS_AGpsSocketClose(); //�ر�SOCKET ����
                t_AGpsManage.DelayCount=0;
                t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_EXIT;
            }
            else
            {
                t_AGpsManage.DelayCount++;
                if(t_AGpsManage.DelayCount>=60)
                {
                    YS_RunSetAGpsAPExitFlag(FALSE);
                    YS_UartDebugString("AGPS deal fail!!!");
                    t_AGpsManage.SckCreateFlag=0;
                    YS_AGpsSocketClose(); //�ر�SOCKET ����
                    t_AGpsManage.DelayCount=0;
                    t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_EXIT;
                }
            }
            break;

        case AGPS_RSQ_FLOW_EXIT: //5���������һ�β���
            t_AGpsManage.DelayCount++;
            if(t_AGpsManage.DelayCount>=5)
            {
                t_AGpsManage.RunFlow=AGPS_RSQ_FLOW_INIT;
            }
            break;

        default:
            break;
    }
}


void YS_AGpsServerDataDebug(u8 *dbuf, u16 dlen)
{
    u16 i,loop,pos,sendlen;
    if(dlen>0)
    {
        loop=dlen/512;
        if((dlen%512)>0)
        {
            loop++;
        }
        pos=0;
        for(i=0; i<loop; i++)
        {
            sendlen=512;
            if(i==(loop-1)) //���һ�η���
            {
                sendlen=dlen-pos;
            }
            YS_UartDebugInterfacel(INTER_PTL_UPDATE_GPRSREC,(u8*)&dbuf[pos],sendlen);
            pos+=sendlen;
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsServerDataVerify
����˵�����˺�������У��AGPS �����Ƿ��������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_AGpsServerDataVerify(u16 *RealPos, u16 *RealLen)
{
    u16 i,k;
    u8 AGps_Flag_Begin[]= {0xB5,0x62,0x0B,0x01}; //��ʼ�ַ�
    u8 AGps_Flag_End[]= {0xB5,0x62,0x0B,0x02};
    bool BeginOK,EndOK,CompRlt;

    BeginOK=FALSE;
    for(i=0; i<t_AGpsManage.RecDataLen-4; i++)
    {
        CompRlt=TRUE;
        for(k=0; k<4; k++)
        {
            if(AGps_Server_Buf[i+k]!=AGps_Flag_Begin[k])
            {
                CompRlt=FALSE;
                break;
            }
        }

        if(CompRlt==TRUE)
        {
            BeginOK=TRUE;
            break;
        }
    }

    EndOK=FALSE;
    for(i=0; i<t_AGpsManage.RecDataLen-4; i++)
    {
        CompRlt=TRUE;
        for(k=0; k<4; k++)
        {
            if(AGps_Server_Buf[i+k]!=AGps_Flag_End[k])
            {
                CompRlt=FALSE;
                break;
            }
        }

        if(CompRlt==TRUE)
        {
            EndOK=TRUE;
            break;
        }
    }

    if((BeginOK==TRUE)&&(EndOK==TRUE))
    {
        for(i=0; i<t_AGpsManage.RecDataLen-4; i++)  //�ҳ���Ч���ݵ�����λ�ü���Ч���ݳ���
        {
            CompRlt=TRUE;
            for(k=0; k<4; k++)
            {
                if(AGps_Server_Buf[i+k]!=AGps_Flag_Begin[k])
                {
                    CompRlt=FALSE;
                    break;
                }
            }

            if(CompRlt==TRUE) //����Ƚϳɹ�
            {
                *RealPos=i;
                *RealLen=t_AGpsManage.RecDataLen-i;
                BeginOK=TRUE;
                break;
            }
        }
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsServerDataParase
����˵�����˺������ڴ���AGPS ƽ̨�·�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsServerDataParase(u8 *dbuf, u16 dlen)
{
    u16 i;
    u8 DebugBuf[10];
    u8 RunFlow;

    u16 RealPos,RealLen;
//    YS_UartDebugString((char *)dbuf);
//    YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, dbuf, dlen);
//    ycsj_debug("\r\n the BDdata len is:%d\r\n", dlen);
//    YS_UartDebugString("BDGps recieve data");
    if(t_AGpsManage.DealEnable==0)
    {
        return;
    }

    if((dlen+t_AGpsManage.RecDataLen)<AGPS_BUF_MAX_LEN)
    {
        for(i=0; i<dlen; i++)
        {
            AGps_Server_Buf[t_AGpsManage.RecDataLen]=dbuf[i];
            t_AGpsManage.RecDataLen++;
        }

        if(YS_AGpsServerDataVerify(&RealPos,&RealLen)==TRUE)
        {
            t_AGpsManage.DealOKFlag=1;
            t_AGpsManage.DealEnable=0; 	//��ֹ�ٴδ���
            DebugBuf[0]=t_AGpsManage.RecDataLen/256;
            DebugBuf[1]=t_AGpsManage.RecDataLen%256;
            DebugBuf[2]=RealLen/256;
            DebugBuf[3]=RealLen%256;
            YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)DebugBuf,4);
            //YS_AGpsServerDataDebug(&AGps_Server_Buf[RealPos],RealLen);
            if(YS_AGpsRsqPosStatus()==TRUE)
            {
                YS_UartDebugString("Gps is effective not send data");
            }
            else
            {
                RunFlow=YS_RunGetRunFlow();
                if((RunFlow!=YS_RUN_FLOW_FIGHT_WAIT)&&(RunFlow!=YS_RUN_FLOW_SLEEP_WAIT))
                {
                    sjfun_Gps_WriteAgps(&AGps_Server_Buf[RealPos],RealLen);
                }

            }
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_AGpsServerConnectMsgDeal
����˵�����˺������ڴ���AGPS ƽ̨���ӳɹ���Ϣ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AGpsServerConnectMsgDeal(void)
{
    t_AGpsManage.SckOKFlag=1;	//����SCK�ɹ���־
}

#if 0
static const char encodingTable [64] = {
  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
  'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
  'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
  'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
};

static int encode(char *buf, int size, const char *user, const char *pwd)
{
    unsigned char inbuf[3];
    char *out = buf;
    int i, sep = 0, fill = 0, bytes = 0;
    while(*user || *pwd)
    {
        i = 0;
        while(i < 3 && *user) inbuf[i++] = *(user++);
        if(i < 3 && !sep)
        {
            inbuf[i++] = ':';
            ++sep;
        }
        while(i < 3 && *pwd) inbuf[i++] = *(pwd++);
        while(i < 3)
        {
            inbuf[i++] = 0;
            ++fill;
        }
        if(out-buf < size-1)
            *(out++) = encodingTable[(inbuf [0] & 0xFC) >> 2];
        if(out-buf < size-1)
            *(out++) = encodingTable[((inbuf [0] & 0x03) << 4)
                                     | ((inbuf [1] & 0xF0) >> 4)];
        if(out-buf < size-1)
        {
            if(fill == 2)
                *(out++) = '=';
            else
                *(out++) = encodingTable[((inbuf [1] & 0x0F) << 2)
                                         | ((inbuf [2] & 0xC0) >> 6)];
        }
        if(out-buf < size-1)
        {
            if(fill >= 1)
                *(out++) = '=';
            else
                *(out++) = encodingTable[inbuf [2] & 0x3F];
        }
        bytes += 4;
    }
    if(out-buf < size)
        *out = 0;
    return bytes;
}
#endif