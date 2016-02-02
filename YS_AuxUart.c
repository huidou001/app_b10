/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_AuxUart.c  2015-06-01
���ļ��ù����ն���PC ��UART �ӿڣ������豸�����Լ�����ά��
-------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------*/

#include "YS_Public.h"

#define  OBD_UARTCMD_ATS        "ATS\r\n"       //��ѯ�汾��

t_AuxUart_Parsae_Buf	t_AuxParaseBuf;
t_Obd_Fault_Info        t_ObdFaultInfo;
t_Obd_Main_Info         t_ObdMainInfo;

u8 OBDRstFlag=FALSE;
/*-----------------------------------------------------------------------------------------
��������YS_GpsGetPosData
����˵������ȡһ��GPS ���ݽṹ�Ŀ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_OBDGetFaultInfo(t_Obd_Fault_Info *pt_GetObdFault)
{
    u8 *pSour, *pAim;
    u8 len;

    pSour=(u8 *)(&t_ObdFaultInfo);
    pAim=(u8 *)(pt_GetObdFault);
    len=sizeof(t_ObdFaultInfo);
    memcpy(pAim,pSour,len);
}

/*-----------------------------------------------------------------------------------------
��������YS_OBDGetMainInfo
����˵������ȡһ��OBD ���ݽṹ�Ŀ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_OBDGetMainInfo(t_Obd_Main_Info *pt_GetObdMain)
{
    u8 *pSour, *pAim;
    u8 len;

    pSour=(u8 *)(&t_ObdMainInfo);
    pAim=(u8 *)(pt_GetObdMain);
    len=sizeof(t_ObdMainInfo);
    memcpy(pAim,pSour,len);
}

/*-----------------------------------------------------------------------------------------
��������YS_OBDParaseCarRun
����˵������������ʻ���� (���)
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_OBDParaseCarRun(u8 *buf, u16 len)
{
    u16 i,j,pos,loop,count,num,tpos;
    u8 fbuf[20];
    double value;

    pos=0;
    loop=len;
    while(loop>12)
    {
        if((buf[pos]=='B')&&(buf[pos+1]=='D')&&(buf[pos+2]=='$'))
        {
            pos+=3;
            count=0;
            for(i=pos; i<len; i++)	//���';'�ĸ���
            {
                if(buf[i]==';')
                {
                    count++;
                }
                if(count>=19)
                {
                    loop-=i;
                    pos +=i;
                    return FALSE;
                }
            }
#if 0
            tpos=pos;	//��λ����һ��','
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            value = atof((char *)fbuf)*10;
            t_ObdMainInfo.Vmp= value;
//            ycsj_debug("t_ObdMainInfo.Vmp=%d\r\n", t_ObdMainInfo.Vmp);

            pos++;
            tpos=pos;//��ʼȡ������ת��
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Rpm= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Rpm=%d\r\n",t_ObdMainInfo.Rpm);

            pos++;
            tpos=pos;//��ʼȡ����
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Spd= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Spd=%d\r\n", t_ObdMainInfo.Spd);

            pos++;
            tpos=pos;//�����ſ���
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            value = atoi((char *)fbuf);
            t_ObdMainInfo.Toh= value;
//            ycsj_debug("t_ObdMainInfo.Toh=%d\r\n", t_ObdMainInfo.Toh);

            pos++;
            tpos=pos;//��ʼȡ����������
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            value = atoi((char *)fbuf);
            t_ObdMainInfo.Load= value;
//            ycsj_debug("t_ObdMainInfo.Load=%d\r\n", t_ObdMainInfo.Load);

            pos++;
            tpos=pos;//��ʼȡ��ȴҺ�¶�
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            value = atoi((char *)fbuf);
            t_ObdMainInfo.Ect = value;
//            ycsj_debug("t_ObdMainInfo.Ect=%d\r\n", t_ObdMainInfo.Ect);

            pos++;
            tpos=pos;//��ʼȡʣ������
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            value = atof((char *)fbuf)*10;
            t_ObdMainInfo.Fli = value;
//            ycsj_debug("t_ObdMainInfo.Fli=%d\r\n", t_ObdMainInfo.Fli);

            pos++;
            tpos=pos;//�ٹ����ͺ�
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.XM= atof((char *)fbuf)*10;
//            ycsj_debug("t_ObdMainInfo.Xm=%d\r\n", t_ObdMainInfo.XM);

            pos++;
            tpos=pos;//��ʻ���
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Ms= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Ms=%d\r\n", t_ObdMainInfo.Ms);

            pos++;
            tpos=pos;//��ʼȡ�ۼƺ�����
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Fs = atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Fs=%d\r\n", t_ObdMainInfo.Fs);

            pos++;
            tpos=pos;//�ۼƵ��ʱ��
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Ts= atof((char *)fbuf)*1000;
//            ycsj_debug("t_ObdMainInfo.Ts=%d\r\n", t_ObdMainInfo.Ts);

            pos++;
            tpos=pos;//�����ٴ���
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Ga= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Ga=%d\r\n", t_ObdMainInfo.Ga);

            pos++;
            tpos=pos;//��ɲ������
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Gb= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Gb=%d\r\n", t_ObdMainInfo.Gb);

            pos++;
            tpos=pos;//���������
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.FaultNum= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.FaultNum=%d\r\n", t_ObdMainInfo.FaultNum);

            pos++;
            tpos=pos;//G X
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Gx= atoi((char *)fbuf);

            pos++;
            tpos=pos;//G Y
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Gy= atoi((char *)fbuf);

            pos++;
            tpos=pos;//G Z
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Gz= atoi((char *)fbuf);
#else
            num = count;
            t_ObdMainInfo.ObdStrlen = 0;
            for ( j = 0; j<num; j++ )
            {
                tpos=pos;//��ѹ
                count=0;
                for(i=tpos; i<len; i++)
                {
                    if(buf[pos]==';')
                    {
                        break;
                    }
                    if(count<11)
                    {
                        fbuf[i-tpos]=buf[pos];
                        t_ObdMainInfo.ObdStr[t_ObdMainInfo.ObdStrlen] = buf[pos];
                        t_ObdMainInfo.ObdStrlen ++;
                        count++;
                    }
                    pos++;
                }
                fbuf[count]=0;
                t_ObdMainInfo.ObdStr[t_ObdMainInfo.ObdStrlen] = '|';
                t_ObdMainInfo.ObdStrlen ++;
                pos++;
            }

            t_ObdMainInfo.ObdStrlen = t_ObdMainInfo.ObdStrlen - 1;//ȥ�����һ��'|'
            t_ObdMainInfo.ObdStr[t_ObdMainInfo.ObdStrlen] = 0;
#endif
            return TRUE;
        }
        else
        {
            loop--;
            pos++;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------------------
��������YS_OBDParaseCarRun
����˵������������ʻ���� (Ϩ��)
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_OBDParaseCarStop(u8 *buf, u16 len)
{
    u16 i,j,pos,loop,count,num,tpos,bpos;
    u8 fbuf[20];
    u16 value;
    bool ParBegin=FALSE;

    pos=0;
    loop=len;
    while(loop>12)
    {
        if((buf[pos]=='F')&&(buf[pos+1]=='F')&&(buf[pos+2]=='$'))
        {
            pos+=3;
            count=0;
            for(i=pos; i<len; i++)	//���';'�ĸ���
            {
                if(buf[i]==';')
                {
                    count++;
                }
                if(count>=15)
                {
                    loop-=i;
                    pos +=i;
                    return FALSE;
                }
            }

            num = count;
            t_ObdMainInfo.ObdOffStrlen = 0;
            for ( j = 0; j<num; j++ )
            {
                tpos=pos;	//��λ����һ��','
                count=0;
                ParBegin = FALSE;
                for(i=tpos; i<len; i++)
                {
                    if(buf[pos]==';')
                    {
                        break;
                    }
                    if (ParBegin == TRUE)
                    {
                        if(count<10)
                        {
                            fbuf[count]=buf[pos];
                            count++;
                        }
                    }
                    if (buf[pos] == ':')
                    {
                        bpos = i;
                        ParBegin = TRUE;
                    }
                    pos++;
                }
                pos++;
                fbuf[count]=0;
//                value = atoi((char *)fbuf);
//                ycsj_debug("%s,j=%d, value = %d\r\n",fbuf,j,value);
                switch (j)
                {
                    case 0 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'R';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'I';
                        t_ObdMainInfo.ObdOffStrlen ++;

                        break;
                    case 1 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'R';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'A';
                        t_ObdMainInfo.ObdOffStrlen ++;

                        break;
                    case 2 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'S';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'A';
                        t_ObdMainInfo.ObdOffStrlen ++;

                        break;
                    case 3 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'S';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'V';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    case 4 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'A';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'A';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    case 5 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'M';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'T';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    case 6 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'F';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'T';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    case 7 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'B';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'S';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    case 8 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'R';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'S';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    case 9 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'A';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'F';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    case 10 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'F';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'M';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    case 11 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'F';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'F';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    case 12 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'T';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'S';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    case 13 :
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'S';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  'S';
                        t_ObdMainInfo.ObdOffStrlen ++;
                        break;
                    default:
                        break;
                }

                for(i=0; i<count; i++)
                {
                    t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  fbuf[i];
                    t_ObdMainInfo.ObdOffStrlen ++;
                }
                t_ObdMainInfo.ObdOffStr[t_ObdMainInfo.ObdOffStrlen] =  '|';
                t_ObdMainInfo.ObdOffStrlen ++;

            }

            ycsj_debug("\r %s \r", t_ObdMainInfo.ObdOffStr);
            YS_GprsServerSendInterface(SERV_UP_CMD_CANOFF, NULL,0);
            return(TRUE);
        }
        else
        {
            loop--;
            pos++;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------------------
��������YS_OBDOneLinesDeal
����˵����
�޸ļ�¼��

-------------------------------------------------------------------------------------------*/
void YS_OBDOneLinesDeal(u8 *dbuf, u16 dlen)
{
    if (YS_OBDParaseCarRun(dbuf, dlen) == TRUE)
    {
        ycsj_debug("car run Parase ok!");
    }
    else
    {
        ycsj_debug("car Parase fail!");
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_ObdGetCarData
����˵������ȡһ��Car���ݽṹ�Ŀ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_ObdGetCarData(t_Obd_Main_Info *pt_ObdMainInfo)
{
    u8 *pSour, *pAim;
    u8 len;

    pSour=(u8 *)(&t_ObdMainInfo);
    pAim=(u8 *)(pt_ObdMainInfo);
    len=sizeof(t_ObdMainInfo);
    memcpy(pAim,pSour,len);
}

/*-----------------------------------------------------------------------------------------
��������YS_ObdCarPackage
����˵������ȡһ��Car����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16  YS_ObdCarPackage(u8 *PackBuf)
{
    u8 i;
    for ( i = 0; i < t_ObdMainInfo.ObdStrlen; i++ )
    {
        PackBuf[i] = t_ObdMainInfo.ObdStr[i];
    }

    return t_ObdMainInfo.ObdStrlen;
}

/*-----------------------------------------------------------------------------------------
��������YS_ObdCarOffPackage
����˵������ȡһ��Car����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16  YS_ObdCarOffPackage(u8 *PackBuf)
{
    u8 i;
    for ( i = 0; i < t_ObdMainInfo.ObdOffStrlen; i++ )
    {
        PackBuf[i] = t_ObdMainInfo.ObdOffStr[i];
    }

    return t_ObdMainInfo.ObdOffStrlen;
}

/*-----------------------------------------------------------------------------------------
��������
����˵����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_OBDDataSourceInput(u8 *dbuf, u16 dlen)
{

    u16 i,len,tpos,count;
    bool GetTailOK;
    t_Sys_Run_Status t_GetStatus;

    YS_RunGetSystemStatus(&t_GetStatus);
    len=dlen;
    tpos=0;
    while(len!=0)
    {
        if((dbuf[tpos]=='B')&&(dbuf[tpos+1]=='D')&&(dbuf[tpos+2]=='$'))
        {
            if (t_GetStatus.AccStatus == 0)
            {
//                YS_RunAccStatusBrush(1);
            }
            GetTailOK=FALSE;
            count=0;
            for(i=0; i<len; i++) //from '$' begin test
            {
                count++;
                if(dbuf[tpos+i]==0x0A)  //�н����ַ����
                {
                    GetTailOK=TRUE;
                    break;
                }
            }

            if(GetTailOK==TRUE) //��������н����ַ�
            {
                YS_OBDParaseCarRun(&dbuf[tpos], count);//��������������
                tpos+=count; //ɾ����ǰ������
                len-=count;
            }
            else
            {
                return; //�޽����ַ����˳����
            }
        }
        // AT POWERON  &&  CONNECTED 1ECU  �������
        else if (((dbuf[tpos]=='E')&&(dbuf[tpos+1]=='R')&&(dbuf[tpos+2]=='O')&&(dbuf[tpos+3]=='N'))||((dbuf[tpos] > '0')&&(dbuf[tpos+1]=='E')&&(dbuf[tpos+2]=='C')&&(dbuf[tpos+3]=='U')))
        {
//            YS_RunAccStatusBrush(1);
            len -= 4;
            tpos += 4;
        }
        //ATPOWEROFF$MAXRPM.... Ϩ������ͳ��
        else if (((dbuf[tpos]=='O')&&(dbuf[tpos+1]=='F')&&(dbuf[tpos+2]=='F')&&(dbuf[tpos+3]=='$'))||((dbuf[tpos]=='0')&&(dbuf[tpos+1]=='E')&&(dbuf[tpos+2]=='C')&&(dbuf[tpos+3]=='U')))
        {
//            YS_RunAccStatusBrush(0);
            GetTailOK=FALSE;
            count=0;
            for(i=0; i<len; i++) //from '$' begin test
            {
                count++;
                if(dbuf[tpos+i]==0x0A)  //�н����ַ����
                {
                    GetTailOK=TRUE;
                    break;
                }
            }

            if(GetTailOK==TRUE) //��������н����ַ�
            {
                YS_OBDParaseCarStop(&dbuf[tpos], count);//��������������
                tpos+=count; //ɾ����ǰ������
                len-=count;
            }
            else
            {
                return; //�޽����ַ����˳����
            }
        }
        else
        {
            len--;
            tpos++;
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_AuxUartSendCmd
����˵�������ڷ���ָ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AuxUartSendCmd(void)
{

}

void YS_OBDRstInit(bool flag)
{
    OBDRstFlag = flag;
}

void YS_OBDRstDeal(void)
{
    static u8 num = 0;
    if (OBDRstFlag == TRUE)
    {
        sjfun_Gpio_Write_Value(YS_PIN_NO_OBD_RST,0);
        num ++;
        if (num >=2)
        {
            OBDRstFlag = FALSE;
            sjfun_Gpio_Write_Value(YS_PIN_NO_OBD_RST,1);
        }
    }
}
/*-----------------------------------------------------------------------------------------
��������YS_AuxUartDealInit
����˵������ʼ�����ݽ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AuxUartBufInit(void)
{
    t_AuxParaseBuf.AddEnable=1;
    t_AuxParaseBuf.DataLen=0;
    YS_OBDRstInit(TRUE);
    sjfun_Open_AuxUart();
}

/*-----------------------------------------------------------------------------------------
��������YS_AuxBufParaseDeal
����˵�����Խ��ջ��������н�������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AuxBufParaseDeal(void)
{
    u8 fbuf[5];
    YS_PrmReadOneItem(FLH_PRM_OBD_DEBUG_EN,FLH_PRM_OBD_DEBUG_EN_LEN,fbuf);
    if (fbuf[0] == 1)
    {
        t_AuxParaseBuf.DataBuf[t_AuxParaseBuf.DataLen] = 0;
        ycsj_debug((char *)t_AuxParaseBuf.DataBuf);
    }
    YS_OBDDataSourceInput(t_AuxParaseBuf.DataBuf, t_AuxParaseBuf.DataLen);
    t_AuxParaseBuf.AddEnable=1;
    t_AuxParaseBuf.DataLen=0;
}

/*-----------------------------------------------------------------------------------------
��������YS_AuxBufAddData
����˵��������ջ�������������ݣ�������ִ�ж�ʱ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AuxBufAddData(u8 *dbuf, u16 dlen)
{
#if 1
    u16 i;
    if((t_AuxParaseBuf.DataLen+dlen)<=YS_GPS_BUF_LEN) //����������ɴ���
    {
        for(i=0; i<dlen; i++)
        {
            t_AuxParaseBuf.DataBuf[t_AuxParaseBuf.DataLen]=dbuf[i];
            t_AuxParaseBuf.DataLen++;
        }
        sjfun_stoptimer(GIS_TIMER_ID_2);						//��ʼ������ʱ��
        sjfun_timer(GIS_TIMER_ID_2,100,YS_AuxBufParaseDeal);
    }
#else
    t_Sys_Run_Status	t_GetStatus;
    YS_RunGetSystemStatus(&t_GetStatus);
    if (t_GetStatus.RunFlow == YS_RUN_FLOW_IDLE_DEAL)
    {
        if(YS_GprsSendDataToServer(dbuf, dlen) == FALSE)
        {
            YS_SysRsqSystemRST(YS_RST_FLAG_LOSE_NET);
//            t_GetStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
        }
    }
#endif
}

