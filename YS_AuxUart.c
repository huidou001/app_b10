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
                    pos +=i;;
                    return;
                }
            }

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

//            ycsj_debug((char *)t_ObdMainInfo.ObdStr);
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
��������
����˵����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_OBDDataSourceInput(u8 *dbuf, u16 dlen)
{
    u16 i,len,tpos,count;
    bool GetTailOK;

    len=dlen;
    tpos=0;
    while(len!=0)
    {
        if((dbuf[tpos]=='B')&&(dbuf[tpos+1]=='D')&&(dbuf[tpos+2]=='$'))
        {
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
                YS_OBDOneLinesDeal(&dbuf[tpos], count); //����һ������
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

/*-----------------------------------------------------------------------------------------
��������YS_AuxUartDealInit
����˵������ʼ�����ݽ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AuxUartBufInit(void)
{
    t_AuxParaseBuf.AddEnable=1;
    t_AuxParaseBuf.DataLen=0;

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
}

