/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Gprs_obd.c  2013-06-25
���ļ��ù�����GPS ������ͨѸЭ��Ĵ�������OBDЭ��
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"
#define BOOTVER         "MT61D_V1.0"
#define HARDVER         "A10"
#define EQUIPTYPE       "0"
#define CHANNELCODE     "XX"

extern t_avltra_Parase		t_AvlSckParase;
extern t_avltra_Parase         t_AvlSckConver;

/*-----------------------------------------------------------------------------------------
��������YS_GprsAddPosData
����˵�����򻺳�������Ӿ�γ������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsAddPosData(u8 *PosValue, u8 *PackBuf)
{
    u16 i,len;
    char StrDat[50];
    double f;

    f=PosValue[1]*10000+PosValue[2]*100+PosValue[3];
    f/=10000;
    f/=60;
    f+=PosValue[0];
    sprintf(StrDat,"%.06f",f);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        PackBuf[i]=StrDat[i];
    }
    return(len);
}

#if 0
/*-----------------------------------------------------------------------------------------
��������YS_GprsAddPosData
����˵�����򻺳�������Ӿ�γ������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsAddUnixTime(u8 *PackBuf)
{
    u16 i,len;
    time_t t;
    int j=0;
    char StrDat[50];

//    j = time(&t);
    sprintf(StrDat,"1448532363");
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        PackBuf[i]=StrDat[i];
    }
    return(len);
}
#endif

/*-----------------------------------------------------------------------------------------
��������YS_GprsAddUnixTime
����˵����Unixʱ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsAddUnixTime(u8 *PackBuf)
{
    u16 i,len;
    u32 t;
    int j=0;
    char StrDat[50];

    t = sjfun_get_unix_time();
    t = t - 28800;

    sprintf(StrDat,"%d",t);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        PackBuf[i]=StrDat[i];
    }
    return(len);
}
/*-----------------------------------------------------------------------------------------
��������YS_GprsAddPosData
����˵�����򻺳���������ٶ�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsAddSpeedInfo(u16 speed, u8 *PackBuf)
{
    u16 i,len;
    char StrDat[50];
    sprintf(StrDat,"%d.0",speed);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        PackBuf[i]=StrDat[i];
    }
    return(len);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsAddCourseInfo
����˵�����򻺳�������Ӻ�������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsAddCourseInfo(u16 course, u8 *PackBuf)
{
    u16 i,len;
    char StrDat[50];
    sprintf(StrDat,"%d.0",course);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        PackBuf[i]=StrDat[i];
    }
    return(len);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsDealPtlSend
����˵������װһ�����ݰ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsDealPtlSend(u8 CmdID, u8 *dbuf, u16 dlen, u8 *PackBuf)
{
    u16 i,pos;

    pos=0;
    PackBuf[pos]=0x7E; //��Ӱ�ͷ
    pos++;
    PackBuf[pos]=0x7D; //��Ӱ�ͷ
    pos++;
    PackBuf[pos]=0x01; //Э��汾��
    pos++;
    PackBuf[pos]=CmdID; //ָ����
    pos++;

    PackBuf[pos]=dlen/256; //��Ϣ�峤��
    pos++;
    PackBuf[pos]=dlen%256; //
    pos++;

    if(dlen>0)  //�����Ϣ�岿��
    {
        for(i=0; i<dlen; i++)
        {
            PackBuf[pos]=dbuf[i];
            pos++;
        }
    }
    return(pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackRsqServerPackage
����˵������װһ�������������ַ���ݰ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackRsqServerPackage(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];

    pos = 0;
    YS_RunAddIDInfo(fbuf);//�豸ID
    len =strlen((char*)fbuf);
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = fbuf[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(EBIKE_SOFT_VER);//����汾��
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = EBIKE_SOFT_VER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(BOOTVER);//�豸�̼��汾��
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = BOOTVER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(HARDVER);//�豸Ӳ���汾��
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = HARDVER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(EQUIPTYPE);//�豸��������
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = EQUIPTYPE[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(CHANNELCODE);//����ʶ����
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = CHANNELCODE[i];
        pos++;
    }

    PackLen=YS_GprsDealPtlSend(0xf0,tempbuf,pos,PackBuf);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackLogPackage
����˵������װһ������ƽ̨���ݰ�(��֤�豸�Ϸ���)
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackLogPackage_obd(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];

    pos = 0;
    YS_RunAddIDInfo(fbuf);//�豸ID
    len =strlen((char*)fbuf);
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = fbuf[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(EBIKE_SOFT_VER);//����汾��
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = EBIKE_SOFT_VER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(BOOTVER);//�豸�̼��汾��
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = BOOTVER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(HARDVER);//�豸Ӳ���汾��
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = HARDVER[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(EQUIPTYPE);//�豸��������
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = EQUIPTYPE[i];
        pos++;
    }
    tempbuf[pos] = '|';
    pos++;

    len = strlen(CHANNELCODE);//����ʶ����
    for ( i = 0; i<len; i++ )
    {
        tempbuf[pos] = CHANNELCODE[i];
        pos++;
    }

    PackLen=YS_GprsDealPtlSend(0x01,tempbuf,pos,PackBuf);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackHeartPackage
����˵������װһ������ƽ̨���ݰ�(��֤�豸�Ϸ���)
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackHeartPackage_obd(u8 *PackBuf)
{
    u16 PackLen;

    PackLen=YS_GprsDealPtlSend(0x7f,NULL,0,PackBuf);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackGPSPackage
����˵������װһ��ʵʱλ�����ݰ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackGpsPackage(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];
    t_Gps_Data_Info t_GpsData;

    YS_GpsGetPosData(&t_GpsData);
    pos = 0;
    len=YS_GprsAddPosData(t_GpsData.Longitude,&tempbuf[pos]);//����
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    len=YS_GprsAddPosData(t_GpsData.Latitude,&tempbuf[pos]);//γ��
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    tempbuf[pos] = '|';
    pos++;

    len=YS_GprsAddUnixTime(&tempbuf[pos]);//UNIXʱ��
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    len=YS_GprsAddSpeedInfo(t_GpsData.Speed,&tempbuf[pos]);//�ٶ�
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    len=YS_GprsAddCourseInfo(t_GpsData.Course,&tempbuf[pos]);//����
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    tempbuf[pos] = '|';//�¶�
    pos++;

    tempbuf[pos] = '|';//����
    pos++;

    tempbuf[pos] = '|';//��λ����
    pos++;

    tempbuf[pos] = '2';
    pos++;
    tempbuf[pos] = '|';//��λ��Դ
    pos++;

    tempbuf[pos] = '|';//�ź�״̬
    pos++;

    tempbuf[pos] = '|';//��������
    pos++;

    tempbuf[pos] = '|';//��·����
    pos++;
    PackLen=YS_GprsDealPtlSend(0x20,tempbuf,pos,PackBuf);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackCarObdPackage
����˵������װһ��ʵʱ��������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackCarObdPackage(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];
    t_Obd_Main_Info t_GetObdStatus;

    YS_ObdGetCarData(&t_GetObdStatus);
    pos = 0;
    len=YS_GprsAddUnixTime(&tempbuf[pos]);//UNIXʱ��
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    len = YS_ObdCarPackage(&tempbuf[pos]);
    pos += len;

    PackLen=YS_GprsDealPtlSend(0x30,tempbuf,pos,PackBuf);

    if (len<100)
    {
        return 0;
    }
    else
    {
        return(PackLen);
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackWarnPackage
����˵������װһ�������澯����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackWarnPackage(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];

    pos = 0;
    len=YS_GprsAddUnixTime(&tempbuf[pos]);//UNIXʱ��
    pos+=len;
    tempbuf[pos] = '|';
    pos++;

    PackLen=YS_GprsDealPtlSend(0x40,tempbuf,pos,PackBuf);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackObdDstPackage
����˵������װһ������OBD�г�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackObdDstPackage(u8 *PackBuf)
{
    u16 PackLen,pos,len;
    u8 i,fbuf[20], tempbuf[200];

    pos = 0;

    PackLen=YS_GprsDealPtlSend(0x60,tempbuf,pos,PackBuf);
    return(PackLen);
}


/*-----------------------------------------------------------------------------------------
��������YS_GprsParaseHeadTail
����˵������Ϣ���ͷ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_GprsParaseHeadTail_obd(u8 *dbuf, u16 dlen)
{
    bool result;
    u16 i;

    t_AvlSckParase.v_CmdID=dbuf[3];
    t_AvlSckParase.v_dlen=dbuf[4]*256+dbuf[5];
    if(t_AvlSckParase.v_dlen>0)
    {
        for(i=0; i<t_AvlSckParase.v_dlen; i++)
        {
            t_AvlSckParase.a_dbuf[i]=dbuf[6+i];
        }
    }
    result=TRUE;
    return(result);
}


/*-----------------------------------------------------------------------------------------
��������YS_GprsDealJTBServRecData
����˵���������ݻ���������ȡ���ݰ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_GprsDealServRecData(u8 *buf, u16 len)
{
    bool result;
    u16 i,pos,count;

    if((buf[0]==0x7E)&&(buf[len-2]==0x0D)&&(buf[len-1]==0x0A))  //�����ͷ��β��ȷ
    {
        pos=0;
        count=0;
        for(i=0; i<len; i++)
        {
            pos++;
            if((buf[i]==0x0D)&&((buf[i+1]==0x0A)))
            {
                pos++;
                break;
            }
        }

        if(YS_GprsParaseHeadTail_obd(buf,pos)==TRUE)
        {
            t_AvlSckParase.v_GetLen=pos;
            result=TRUE;
        }
        else
        {
            result=FALSE;
        }
    }
    else
    {
        result=FALSE;
    }
    return(result);
}



/*-----------------------------------------------------------------------------------------
��������YS_GprsJBTSckPtlUpConver
����˵����ƽ̨�·����ݰ�����ָ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_GprsSckPtlUpConver(void)
{
    bool result,ack;
    u16 i,AckCmd;
    u8 fbuf[5];

    result=TRUE;
    memcpy(t_AvlSckConver.a_AddInfo,t_AvlSckParase.a_AddInfo,AVL_ADD_INFO_LEN);

    switch(t_AvlSckParase.v_CmdID)
    {
        case 0x01: //����ƽ̨Ӧ��
            if (t_AvlSckParase.a_dbuf[1] == 0x31)
            {
                YS_RunLoginServerAck();
                ycsj_debug("log ack!\r\n");
                YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)"log ack!\r\n",10);
            }
            break;

        case 0x7f: //����Ӧ��
            YS_RunIdleHeartAck();
            ycsj_debug("heart ack!\r\n");
            YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)"log ack!\r\n",10);
            break;

        case 0x40: //����Ӧ��

            break;

        default:
            result=FALSE;
            break;
    }

    return(result);
}

