/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Gprs_808.c  2013-06-25
���ļ��ù�����GPS ������ͨѸЭ��Ĵ�������808Э��
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"
extern t_avltra_Parase		t_AvlSckParase;
extern t_avltra_Parase         t_AvlSckConver;

void YS_GprsConverSimCode(u8 *OutBuf, char *SimCode)
{
    char StrDat[20];
    char StrSim[20];
    u8 i,dat,pos;
    u8 len;
    pos=0;

    len=strlen(SimCode);
    for(i=0; i<12-len; i++)
    {
        StrSim[pos]='0';
        pos++;
    }
    for(i=0; i<len; i++)
    {
        StrSim[pos]=SimCode[i];
        pos++;
    }
    for(i=0; i<6; i++)
    {
        StrDat[0]=StrSim[i*2];
        StrDat[1]=StrSim[i*2+1];
        StrDat[2]=0;
        dat=atoi(StrDat);
        OutBuf[i]=YS_CodeOneByteToBCD(dat);
    }
}

u32  YS_GprsPosToMillionDU(u8 *PosBuf)
{
    double  f;
    char StrDat[20];
    u32 result;

    sprintf(StrDat,"%d.%02d%02d",PosBuf[1],PosBuf[2],PosBuf[3]);
    f=atof(StrDat);
    f=f*100; //ת���ɶ�С��
    f=f/6000;
    f=f+PosBuf[0];
    f*=1000000;
    result=(u32)f;
    return(result);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsAddOneByte
����˵����ת���ַ�����  0x7e<---->0x7d 0x02    0x7d<---->0x7d 0x01
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_GprsAddOneByte(u8 dat, u8 *buf, u8 *Verify)
{

    u8 tVer;

    tVer=*Verify;
    *Verify=tVer^dat;
    if(dat==0x7E)
    {
        buf[0]=0x7D;
        buf[1]=0x02;
        return(2);
    }
    else if(dat==0x7D)
    {
        buf[0]=0x7D;
        buf[1]=0x01;
        return(2);
    }
    else
    {
        buf[0]=dat;
        return(1);
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsDealJTBPtlSend
����˵������ͨ��808Э���װһ�����ݰ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsDealJTBPtlSend(u16 CmdID, u8 *dbuf, u16 dlen, u8 *PackBuf, u16 TotalNum, u16 CurNO)
{
    u8 Verify,SimCode[6],tdat;
    u16 i,pos,td16;
    static u16 SendFlow=0;
    u8 fbuf[20];

    pos=0;
    PackBuf[pos]=0x7E; //��Ӱ�ͷ
    pos++;
    Verify=0x00;
    pos+=YS_GprsAddOneByte(CmdID/256,&PackBuf[pos],&Verify);
    pos+=YS_GprsAddOneByte(CmdID%256,&PackBuf[pos],&Verify);

    td16=dlen;
    if(TotalNum>1) //������÷ְ���ʽ
    {
        td16|=0x2000;
    }
    pos+=YS_GprsAddOneByte(td16/256,&PackBuf[pos],&Verify);
    pos+=YS_GprsAddOneByte(td16%256,&PackBuf[pos],&Verify);

//    YS_PrmReadOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf);
    YS_RunAddIDInfo(fbuf);
    YS_GprsConverSimCode(SimCode,(char *)fbuf);
    for(i=0; i<6; i++)  //���SIM ����
    {
        pos+=YS_GprsAddOneByte(SimCode[i],&PackBuf[pos],&Verify);
    }

    pos+=YS_GprsAddOneByte(SendFlow/256,&PackBuf[pos],&Verify); //�����ˮ��
    pos+=YS_GprsAddOneByte(SendFlow%256,&PackBuf[pos],&Verify);
    SendFlow++;

    if(TotalNum>1) //������÷ְ�����
    {
        pos+=YS_GprsAddOneByte(TotalNum/256,&PackBuf[pos],&Verify);  //�ܰ���
        pos+=YS_GprsAddOneByte(TotalNum%256,&PackBuf[pos],&Verify);
        pos+=YS_GprsAddOneByte(CurNO/256,&PackBuf[pos],&Verify);  //��ǰ����
        pos+=YS_GprsAddOneByte(CurNO%256,&PackBuf[pos],&Verify);
    }

    if(dlen>0)  //�����Ϣ�岿��
    {
        for(i=0; i<dlen; i++)
        {
            pos+=YS_GprsAddOneByte(dbuf[i],&PackBuf[pos],&Verify);
        }
    }

    /*
    	PackBuf[pos]=Verify;  //���Ч��
    	pos++;*/
//	tdat

    pos+=YS_GprsAddOneByte(Verify,&PackBuf[pos],&tdat);   //����У�����ݵ�ת�崦��

    PackBuf[pos]=0x7E;   //��Ӱ�β��Ϣ
    pos++;

    return(pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsSendAckPackage
����˵�����ն�ͨ��Ӧ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsSendAckPackage(u8 *PackBuf, u8 *AddInfo, u8 rlt)
{
    u8 GpsBuf[300];
    u16 GpsLen,PackLen;

    GpsBuf[0]=AddInfo[10];//Ӧ����ˮ��
    GpsBuf[1]=AddInfo[11];
    GpsBuf[2]=AddInfo[0];//Ӧ��ID
    GpsBuf[3]=AddInfo[1];
    GpsBuf[4]=rlt;//���
    GpsLen=5;
//	GpsLen=YS_GprsPackGpsInfo(&GpsBuf[2]);
//	GpsLen+=2;
    PackLen=YS_GprsDealJTBPtlSend(0x0001,GpsBuf,GpsLen,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackRegPackage
����˵������װһ��ע�����ݰ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackRegPackage(u8 *PackBuf)
{
    u8 tempbuf[200],fbuf[20],len;
    u16 i,pos,PackLen;

    pos=0;
    YS_PrmReadOneItem(FLH_JTB_PROVINCE_NAME,FLH_JTB_PROVINCE_NAME_LEN,fbuf);  //ʡ��ID
    tempbuf[pos]=fbuf[0];
    pos++;
    tempbuf[pos]=fbuf[1];
    pos++;

    YS_PrmReadOneItem(FLH_JTB_CITY_NAME,FLH_JTB_CITY_NAME_LEN,fbuf);  //����ID
    tempbuf[pos]=fbuf[0];
    pos++;
    tempbuf[pos]=fbuf[1];
    pos++;

    YS_PrmReadOneItem(FLH_JTB_PRODUCER_NAME,FLH_JTB_PRODUCER_NAME_LEN,fbuf);  //������ID
    for(i=0; i<5; i++)
    {
        tempbuf[pos]=fbuf[i];
        pos++;
    }

    YS_PrmReadOneItem(FLH_JTB_TERMINAL_TYPE,FLH_JTB_TERMINAL_TYPE_LEN,fbuf);  //�ն��ͺ�
    for(i=0; i<8; i++)
    {
        tempbuf[pos]=fbuf[i];
        pos++;
    }
    for(i=0; i<12; i++)
    {
        tempbuf[pos]=0;
        pos++;
    }

    YS_PrmReadOneItem(FLH_JTB_TERMINAM_ID,FLH_JTB_TERMINAM_ID_LEN,fbuf);  //�ն��ͺ�
    for(i=0; i<7; i++)
    {
        tempbuf[pos]=fbuf[i];
        pos++;
    }

//    YS_PrmReadOneItem(FLH_JTB_PLATE_COLOR,FLH_JTB_PLATE_COLOR_LEN,fbuf);  //������ɫ
//    tempbuf[pos]=fbuf[0];
    tempbuf[pos]=0x01;
    pos++;

    YS_PrmReadOneItem(FLH_JTB_PLATE_STRING,FLH_JTB_PLATE_STRING_LEN,fbuf);  //���ƺ���
//    for(i=0; i<7; i++)
//    {
//        tempbuf[pos]=fbuf[i];
//        pos++;
//    }
    len=YS_CodeBufRealLen(fbuf,FLH_JTB_PLATE_STRING_LEN);
    for(i=0; i<len; i++)
    {
        tempbuf[pos]=fbuf[i];
        pos++;
    }
    PackLen=YS_GprsDealJTBPtlSend(0x0100,tempbuf,pos,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackLogPackage
����˵������װһ����¼���ݰ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackLogPackage(u8 *PackBuf)
{
    u8 fbuf[40];
    u16 len,PackLen;

    YS_PrmReadOneItem(FLH_JTB_SERV_DOWN_SN,FLH_JTB_SERV_DOWN_SN_LEN,fbuf);  //��Ȩ��
    len=YS_CodeBufRealLen(fbuf,FLH_JTB_SERV_DOWN_SN_LEN);
    PackLen=YS_GprsDealJTBPtlSend(0x0102,fbuf,len,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackHeartPackage
����˵������װһ���������ݰ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackHeartPackage(u8 *PackBuf)
{
    u16 PackLen;
    PackLen=YS_GprsDealJTBPtlSend(0x002,NULL,0,PackBuf,1,1);
    return(PackLen);
}

u16 YS_GprsAddOnePrmID(u16 CmdID,u8 *PackBuf)
{
    PackBuf[0]=0;
    PackBuf[1]=0;
    PackBuf[2]=CmdID/256;
    PackBuf[3]=CmdID%256;
    return(4);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsWriteJTBPrm
����˵������ȡ�ն� ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsReadJTBPrmServ(u8 *PackBuf)
{
    u16 i,pos,len;
    u8 fbuf[60];
    Avl_ULong ldat;
    t_Sys_Run_Status t_GetStatus;

    YS_RunGetSystemStatus(&t_GetStatus);
    pos=0;
    pos+=YS_GprsAddOnePrmID(0x0001,&PackBuf[pos]);  //�ն�������
    PackBuf[pos]=4;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=fbuf[0];
    pos++;
    PackBuf[pos]=fbuf[1];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0013,&PackBuf[pos]); //����������ַ
    YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
    PackBuf[pos]=len;
    pos++;
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=fbuf[i];
        pos++;
    }

    pos+=YS_GprsAddOnePrmID(0x0018,&PackBuf[pos]); //�������˿�
    PackBuf[pos]=4;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=fbuf[0];
    pos++;
    PackBuf[pos]=fbuf[1];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0029,&PackBuf[pos]); //ACC���ϱ�ʱ����
    PackBuf[pos]=4;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=fbuf[0];
    pos++;
    PackBuf[pos]=fbuf[1];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0055,&PackBuf[pos]); //���ٱ������ʱ��
    PackBuf[pos]=4;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_SPEED_VALUE,FLH_PRM_SPEED_VALUE_LEN,fbuf);
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=fbuf[0];
    pos++;
    PackBuf[pos]=fbuf[1];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0056,&PackBuf[pos]); //���ٳ���ʱ��
    PackBuf[pos]=4;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_SPEED_TIME,FLH_PRM_SPEED_TIME_LEN,fbuf);
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=0;
    pos++;
    PackBuf[pos]=fbuf[0];
    pos++;
    PackBuf[pos]=fbuf[1];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0080,&PackBuf[pos]); //��ȡ���
    PackBuf[pos]=4;
    pos++;
    ldat.l = (t_GetStatus.Distance/100);
    PackBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    PackBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    PackBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    PackBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    pos+=YS_GprsAddOnePrmID(0x0083,&PackBuf[pos]); //��ȡ���ƺ�
    YS_PrmReadOneItem(FLH_JTB_PLATE_STRING,FLH_JTB_PLATE_STRING_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_JTB_PLATE_STRING_LEN);
    PackBuf[pos]=len;
    pos++;
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=fbuf[i];
        pos++;
    }

    return(pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackPrmAckPackage
����˵������ѯ����Ӧ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackPrmAckPackage(u8 *PackBuf, u8 *AddInfo)
{
    u16 len;
    u8 fbuf[400];
    u8 PrmBuf[300];
    u16 i,pos,PackLen;

    pos=0;
    PrmBuf[pos]=AddInfo[10];
    pos++;
    PrmBuf[pos]=AddInfo[11];
    pos++;
    PrmBuf[pos]=0x08;
    pos++;
    len = YS_GprsReadJTBPrmServ(fbuf);
    for(i=0; i<len; i++)
    {
        PrmBuf[pos]=fbuf[i];
        pos++;
    }
    PackLen=YS_GprsDealJTBPtlSend(0x0104,PrmBuf,pos,PackBuf,1,1);
    return(PackLen);

}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackGpsInfo
����˵������װGps��Ϣ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackObdInfo(u8 *GpsBuf)
{
    u16 pos,i,len,count;

    pos = 0;
    //������Ϣ�������ݰ�
    GpsBuf[pos]=0xe1;
    pos++;


    return(pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackGpsInfo
����˵������װGps��Ϣ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackGpsInfo(u8 *GpsBuf)
{
    t_Sys_Run_Status	t_GetStatus;
    t_Gps_Data_Info	t_GetGps;
    Avl_ULong  ldat;
    u32 ValueTmp=0;
    u16 pos,i,len;
    u8 fbuf[10];
    u16 SpeedSet;

    pos=0;
    YS_RunGetSystemStatus(&t_GetStatus);
    YS_GpsGetPosData(&t_GetGps);

    /*------------------------����״̬�ַ�װ------------------------*/
    ldat.l=0;
    if(t_GetStatus.WarnStatus[AVL_WARM_EXIGEN]==TRUE)  //��������
    {
        ldat.l=ldat.l|0x00000001;
    }

    if(t_GetGps.Speed>=30)
    {
        if(t_GetStatus.WarnStatus[AVL_WARM_SPEED]==TRUE) //���ٱ���
        {
            ldat.l=ldat.l|0x00000002;
        }
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_LONGDRIVE]==TRUE) //ƣ�ͼ�ʻ����
    {
        ldat.l=ldat.l|0x00000004;
    }


    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_STATION]==TRUE) //GPS ģ����
    {
        ldat.l=ldat.l|0x00000010;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_GPSEXCEPT]==TRUE) //GPS ���߱���
    {
        ldat.l=ldat.l|0x00000020;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_WRANGLE]==TRUE) //GPS ���߶�·
    {
        ldat.l=ldat.l|0x00000040;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_LOWVOL]==TRUE) //�͵�ѹ����
    {
        ldat.l=ldat.l|0x00000080;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_OFFVOL]==TRUE) //�ϵ籨��
    {
        ldat.l=ldat.l|0x00000100;
    }
    if(t_GetStatus.WarnStatus[AVL_WARM_MOVE]==TRUE) //λ�Ʊ���
    {
        ldat.l=ldat.l|0x10000000;
    }
    if(t_GetStatus.WarnStatus[AVL_WARM_INAREA]==TRUE) //�����ڱ���
    {
//        ldat.l=ldat.l|0x00000100;
    }
    if(t_GetStatus.WarnStatus[AVL_WARM_OUTAREA]==TRUE) //�����ⱨ��
    {
//        ldat.l=ldat.l|0x00000100;
    }

    //LCD ����
    //TTS ����
    //����ͷ
    //����
    //�����ۼƼ�ʻ��ʱ

    if(t_GetStatus.WarnStatus[AVL_WARM_STOP]==TRUE) //ͣ����ʱ
    {
        ldat.l=ldat.l|0x00080000;
    }

    if((t_GetStatus.WarnStatus[AVL_WARM_INAREA])||(t_GetStatus.WarnStatus[AVL_WARM_OUTAREA])) //��������
    {
        ldat.l=ldat.l|0x00100000;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_FULL]==TRUE) //����·�߱���
    {
        ldat.l=ldat.l|0x00200000;
    }

    //·����ʻʱ�䲻��/����
    if(t_GetStatus.WarnStatus[AVL_WARM_COURSE]==TRUE) //ƫ������
    {
        ldat.l=ldat.l|0x00800000;
    }

    //����VSS ����(�ٶȴ�����)
    //�����쳣
    if(t_GetStatus.WarnStatus[AVL_WARM_THIEF]==TRUE) //��������
    {
        ldat.l=ldat.l|0x04000000;
    }
    //�Ƿ����
    //�Ƿ���λ
    //��ײ�෭����
    //����30-31
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    /*--------------------------------ϵͳ״̬�ַ�װ--------------------*/
    ldat.l=0;
    if(t_GetStatus.AccStatus) //����ACC ״̬
    {
        ldat.l|=0x00000001;
    }

//    if(t_GetGps.Effective) //����GPS ��λ��־
    {
        ldat.l|=0x00000002;
    }

    if(t_GetGps.SouthORNorth==FALSE) //�ϱ�γ��־
    {
        ldat.l|=0x00000004;
    }

    if(t_GetGps.EastORWest==FALSE) //��������־
    {
        ldat.l|=0x00000008;
    }

    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    /*------------------------���GPS ����------------------------*/
    ldat.l=YS_GprsPosToMillionDU(t_GetGps.Latitude); //γ��
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    ldat.l=YS_GprsPosToMillionDU(t_GetGps.Longitude); //����
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    GpsBuf[pos]=0;  //���θ߶�
    pos++;
    GpsBuf[pos]=0;
    pos++;

    if(t_GetStatus.WarnStatus[AVL_WARM_SPEED]==TRUE)
    {
        YS_PrmReadOneItem(FLH_PRM_SPEED_VALUE, FLH_PRM_SPEED_VALUE_LEN, fbuf);
        SpeedSet=fbuf[0]*256+fbuf[1];
        if(t_GetGps.Speed<=SpeedSet)
        {
            t_GetGps.Speed=SpeedSet*10;
        }
        else
        {
            t_GetGps.Speed=t_GetGps.Speed*10;
        }
    }
    else
    {
        t_GetGps.Speed=t_GetGps.Speed*10;   //ת��Ϊ0.1ǧ��
    }

    GpsBuf[pos]=t_GetGps.Speed/256;  //�ٶ�
    pos++;
    GpsBuf[pos]=t_GetGps.Speed%256;
    pos++;

    GpsBuf[pos]=t_GetGps.Course/256; //����
    pos++;
    GpsBuf[pos]=t_GetGps.Course%256;
    pos++;

    if(YS_RunGsmColckisOK()==TRUE) //���ϵͳʱ����ͬ��
    {
        sjfun_Get_System_DateTime(t_GetGps.DataTime);  //ȡϵͳʱ��
    }
    else
    {
        YS_CodeCretaeBJDT(t_GetGps.DataTime);	//ȡGPS ʱ�䣬��ת��Ϊ������ʽ
    }

    for(i=0; i<6; i++)
    {
        GpsBuf[pos]=YS_CodeOneByteToBCD(t_GetGps.DataTime[i]);
        pos++;
    }

    GpsBuf[pos]=0x01;     //������Ϣ1 ���
    pos++;
    GpsBuf[pos]=0x04;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_DIS_RATIO, FLH_PRM_DIS_RATIO_LEN, fbuf);//��ȡ��̱���ϵ��

    ValueTmp=t_GetStatus.Distance*fbuf[0]/100000;

    ldat.l=(t_GetStatus.Distance/100)+ValueTmp;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    //������Ϣ������
    GpsBuf[pos]=0x02;
    pos++;
    GpsBuf[pos]=0x02;
    pos++;
    GpsBuf[pos]=0x01;
    pos++;
    GpsBuf[pos]=0x01;
    pos++;

    //������Ϣ����¼���ٶ�
    GpsBuf[pos]=0x03;
    pos++;
    GpsBuf[pos]=0x02;
    pos++;
//	GpsBuf[pos]=(t_GetStatus.McuSpeed*10)/256;
    GpsBuf[pos]=0;
    pos++;
//	GpsBuf[pos]=(t_GetStatus.McuSpeed*10)%256;
    GpsBuf[pos]=0;
    pos++;

    //���ٱ���������Ϣ
    if(t_GetStatus.WarnStatus[AVL_WARM_SPEED]==TRUE) //���ٱ���
    {
        GpsBuf[pos]=0x11;
        pos++;
        GpsBuf[pos]=0x01;
        pos++;
        GpsBuf[pos]=0;
        pos++;
    }

    //����·������������Ϣ
    if ((t_GetStatus.WarnStatus[AVL_WARM_INAREA]==TRUE)||(t_GetStatus.WarnStatus[AVL_WARM_OUTAREA]==TRUE))
    {

        YS_PrmReadOneItem(FLH_PRM_AREA_STOR, FLH_PRM_AREA_STOR_LEN, fbuf);
        GpsBuf[pos]=0x12;
        pos++;
        GpsBuf[pos]=0x06;
        pos++;
        GpsBuf[pos]=fbuf[2];//��������
        pos++;
        for ( i = 0; i <4 ; i++ )//����ID
        {
            GpsBuf[pos]=fbuf[3+i];
            pos++;
        }
        GpsBuf[pos]=fbuf[1];//����������
        pos++;
    }
#if ENABLE_X1_MODLE == 4
    //���վ��λ������Ϣ
    len = YS_SysPackNetInfo(&GpsBuf[pos]);
    pos+=len;
#endif
    return(pos);

}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackCarCtlAckPackage
����˵������������Ӧ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackCarCtlAckPackage(u8 *PackBuf, u8 *AddInfo)
{
    u8 GpsBuf[300],PrmBuf[300];

    u16 i,pos,PackLen;
    u16 GpsLen;

    pos=0;
    PrmBuf[pos]=AddInfo[10];
    pos++;
    PrmBuf[pos]=AddInfo[11];
    pos++;

    GpsLen=YS_GprsPackGpsInfo(GpsBuf);
    for ( i = 0; i < GpsLen; i++ )
    {
        PrmBuf[pos]=GpsBuf[i];
        pos++;
    }

    PackLen=YS_GprsDealJTBPtlSend(0x0500,PrmBuf,pos,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackPosPackage
����˵������װһ����λ�����ϴ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackPosPackage(u8 *PackBuf)
{
    u8 GpsBuf[300];
    u16 GpsLen,PackLen;

    GpsLen=YS_GprsPackGpsInfo(GpsBuf);
    PackLen=YS_GprsDealJTBPtlSend(0x0200,GpsBuf,GpsLen,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackDeadGpsInfo
����˵������װGps��Ϣ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackDeadGpsInfo(u8 *GpsBuf)
{
    t_Sys_Run_Status	t_GetStatus;
    t_Gps_Data_Info	t_GetGps;
    t_Gprs_DeadPos_Pack t_GetDeadPosPack;
    Avl_ULong  ldat;
    u16 pos,i;
    u8 fbuf[10];
    u16 SpeedSet;

    pos=0;
    YS_RunGetSystemStatus(&t_GetStatus);
    YS_GpsGetPosData(&t_GetGps);
    YS_GprsDeadPosGetPackInfo(&t_GetDeadPosPack);

    /*------------------------����״̬�ַ�װ------------------------*/
    ldat.l=0;
    if(t_GetStatus.WarnStatus[AVL_WARM_EXIGEN]==TRUE)  //��������
    {
        ldat.l=ldat.l|0x00000001;
    }

    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_SPEED]==TRUE) //���ٱ���
    {
        ldat.l=ldat.l|0x00000002;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_LONGDRIVE]==TRUE) //ƣ�ͼ�ʻ����
    {
        ldat.l=ldat.l|0x00000004;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_STATION]==TRUE) //GPS ģ����
    {
        ldat.l=ldat.l|0x00000010;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_GPSEXCEPT]==TRUE) //GPS ���߱���
    {
        ldat.l=ldat.l|0x00000020;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_WRANGLE]==TRUE) //GPS ���߶�·
    {
        ldat.l=ldat.l|0x00000040;
    }

    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_LOWVOL]==TRUE) //�͵�ѹ����
    {
        ldat.l=ldat.l|0x00000080;
    }

    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_OFFVOL]==TRUE) //�ϵ籨��
    {
        ldat.l=ldat.l|0x00000100;
    }
    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_MOVE]==TRUE) //λ�Ʊ���
    {
        ldat.l=ldat.l|0x10000000;
    }
    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_INAREA]==TRUE) //�����ڱ���
    {
//        ldat.l=ldat.l|0x00000100;
    }
    if(t_GetDeadPosPack.DeadPosWarmStatus[AVL_WARM_OUTAREA]==TRUE) //�����ⱨ��
    {
//        ldat.l=ldat.l|0x00000100;
    }
    //LCD ����
    //TTS ����
    //����ͷ
    //����
    //�����ۼƼ�ʻ��ʱ

    if(t_GetStatus.WarnStatus[AVL_WARM_STOP]==TRUE) //ͣ����ʱ
    {
        ldat.l=ldat.l|0x00080000;
    }

    if((t_GetStatus.WarnStatus[AVL_WARM_INAREA])||(t_GetStatus.WarnStatus[AVL_WARM_OUTAREA])) //��������
    {
        ldat.l=ldat.l|0x00100000;
    }

    if(t_GetStatus.WarnStatus[AVL_WARM_BUS_FULL]==TRUE) //����·�߱���
    {
        ldat.l=ldat.l|0x00200000;
    }

    //·����ʻʱ�䲻��/����
    if(t_GetStatus.WarnStatus[AVL_WARM_COURSE]==TRUE) //ƫ������
    {
        ldat.l=ldat.l|0x00800000;
    }

    //����VSS ����(�ٶȴ�����)
    //�����쳣
    if(t_GetStatus.WarnStatus[AVL_WARM_THIEF]==TRUE) //��������
    {
        ldat.l=ldat.l|0x04000000;
    }
    //�Ƿ����
    //�Ƿ���λ
    //��ײ�෭����
    //����30-31
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    /*--------------------------------ϵͳ״̬�ַ�װ--------------------*/
    ldat.l=0;
//    if(t_GetStatus.AccStatus) //����ACC ״̬
//    {
    ldat.l|=0x00000001;
//    }

//    if(t_GetGps.Effective) //����GPS ��λ��־
//    {
    ldat.l|=0x00000002;
//    }

    if(t_GetGps.SouthORNorth==FALSE) //�ϱ�γ��־
    {
        ldat.l|=0x00000004;
    }

    if(t_GetGps.EastORWest==FALSE) //��������־
    {
        ldat.l|=0x00000008;
    }

    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    /*------------------------���GPS ����------------------------*/
    ldat.l=YS_GprsPosToMillionDU(t_GetDeadPosPack.DeadPosLati); //γ��
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    ldat.l=YS_GprsPosToMillionDU(t_GetDeadPosPack.DeadPosLongi); //����
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    GpsBuf[pos]=0;  //���θ߶�
    pos++;
    GpsBuf[pos]=0;
    pos++;

    if(t_GetStatus.WarnStatus[AVL_WARM_SPEED]==TRUE)
    {
        YS_PrmReadOneItem(FLH_PRM_SPEED_VALUE, FLH_PRM_SPEED_VALUE_LEN, fbuf);
        SpeedSet=fbuf[0]*256+fbuf[1];
        if(t_GetGps.Speed<=SpeedSet)
        {
            t_GetGps.Speed=SpeedSet*10;
        }
    }
    else
    {
        t_GetGps.Speed=t_GetGps.Speed*10;   //ת��Ϊ0.1ǧ��
    }

    GpsBuf[pos]=t_GetDeadPosPack.DeadPosSpeed/256;  //�ٶ�
    pos++;
    GpsBuf[pos]=t_GetDeadPosPack.DeadPosSpeed%256;
    pos++;

    GpsBuf[pos]=t_GetDeadPosPack.DeadPosCourse/256; //����
    pos++;
    GpsBuf[pos]=t_GetDeadPosPack.DeadPosCourse%256;
    pos++;

    //��ת��Ϊ����ʱ��
    for(i=0; i<6; i++)
    {
        GpsBuf[pos]=YS_CodeOneByteToBCD(t_GetDeadPosPack.DeadPosTime[i]);
        pos++;
    }

    GpsBuf[pos]=0x01;     //������Ϣ1 ���
    pos++;
    GpsBuf[pos]=0x04;
    pos++;
    YS_PrmReadOneItem(FLH_PRM_DIS_RATIO, FLH_PRM_DIS_RATIO_LEN, fbuf);//��ȡ��̱���ϵ��

    ldat.l=(t_GetStatus.Distance*(fbuf[0]+1000)/100000);//
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    //������Ϣ������
    GpsBuf[pos]=0x02;
    pos++;
    GpsBuf[pos]=0x02;
    pos++;
    GpsBuf[pos]=0x01;
    pos++;
    GpsBuf[pos]=0x01;
    pos++;

    //������Ϣ����¼���ٶ�
    GpsBuf[pos]=0x03;
    pos++;
    GpsBuf[pos]=0x02;
    pos++;
//	GpsBuf[pos]=(t_GetStatus.McuSpeed*10)/256;
    GpsBuf[pos]=0;
    pos++;
//	GpsBuf[pos]=(t_GetStatus.McuSpeed*10)%256;
    GpsBuf[pos]=0;
    pos++;

    //���ٱ���������Ϣ
    if(t_GetStatus.WarnStatus[AVL_WARM_SPEED]==TRUE) //���ٱ���
    {
        GpsBuf[pos]=0x11;
        pos++;
        GpsBuf[pos]=0x01;
        pos++;
        GpsBuf[pos]=0;
        pos++;
    }

    //����·������������Ϣ
    if ((t_GetStatus.WarnStatus[AVL_WARM_INAREA]==TRUE)||(t_GetStatus.WarnStatus[AVL_WARM_OUTAREA]==TRUE))
    {

        YS_PrmReadOneItem(FLH_PRM_AREA_STOR, FLH_PRM_AREA_STOR_LEN, fbuf);
        GpsBuf[pos]=0x12;
        pos++;
        GpsBuf[pos]=0x06;
        pos++;
        GpsBuf[pos]=fbuf[2];//��������
        pos++;
        for ( i = 0; i <4 ; i++ )//����ID
        {
            GpsBuf[pos]=fbuf[3+i];
            pos++;
        }
        GpsBuf[pos]=fbuf[1];//����������
        pos++;
    }
    //·����ʻ�������������Ϣ

    return(pos);

}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackDeadPosPackage
����˵������װһ����λ�����ϴ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackDeadPosPackage(u8 *PackBuf)
{
    u8 GpsBuf[300];
    u16 GpsLen,PackLen;

    GpsLen=YS_GprsPackDeadGpsInfo(GpsBuf);
    PackLen=YS_GprsDealJTBPtlSend(0x0200,GpsBuf,GpsLen,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsSendPointPackage
����˵��������Ӧ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsSendPointPackage(u8 *PackBuf, u8 *AddInfo)
{
    u8 GpsBuf[300];
    u16 GpsLen,PackLen;

    GpsBuf[0]=AddInfo[10];
    GpsBuf[1]=AddInfo[11];
    GpsLen=YS_GprsPackGpsInfo(&GpsBuf[2]);
    GpsLen+=2;
    PackLen=YS_GprsDealJTBPtlSend(0x0201,GpsBuf,GpsLen,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsPackGpsRsqInfo
����˵������װGps��Ϣ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsPackGpsRsqInfo(u8 *GpsBuf)
{
    t_Sys_Run_Status	t_GetStatus;
    t_Gps_Data_Info	t_GetGps;
    Avl_UWord  wdat;
    Avl_ULong  ldat;
    u16 pos,i;

    pos=0;
    YS_RunGetSystemStatus(&t_GetStatus);
    YS_GpsGetPosData(&t_GetGps);

    if(YS_RunGsmColckisOK()==TRUE) //���ϵͳʱ����ͬ��
    {
        sjfun_Get_System_DateTime(t_GetGps.DataTime);  //ȡϵͳʱ��
    }
    else
    {
        YS_CodeCretaeBJDT(t_GetGps.DataTime);	//ȡGPS ʱ�䣬��ת��Ϊ������ʽ
    }

    for(i=0; i<6; i++)
    {
        GpsBuf[pos]=YS_CodeOneByteToBCD(t_GetGps.DataTime[i]);
        pos++;
    }

    GpsBuf[pos]=0;
    if(t_GetGps.Effective) //����GPS ��λ��־
    {
        GpsBuf[pos]|=0x01;
    }

    if(t_GetGps.SouthORNorth==FALSE) //�ϱ�γ��־
    {
        GpsBuf[pos]|=0x02;
    }

    if(t_GetGps.EastORWest==FALSE) //��������־
    {
        GpsBuf[pos]|=0x04;
    }
    pos++;


    /*------------------------���GPS ����------------------------*/
    ldat.l=YS_GprsPosToMillionDU(t_GetGps.Latitude); //γ��
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    ldat.l=YS_GprsPosToMillionDU(t_GetGps.Longitude); //����
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    ldat.l=t_GetStatus.CellID;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    wdat.w=t_GetStatus.AreaID;
    GpsBuf[pos]=wdat.b[AVL_WSTOR_FIR];
    pos++;
    GpsBuf[pos]=wdat.b[AVL_WSTOR_SEC];
    pos++;

    wdat.w=t_GetStatus.CountryName;
    GpsBuf[pos]=wdat.b[AVL_WSTOR_FIR];
    pos++;
    GpsBuf[pos]=wdat.b[AVL_WSTOR_SEC];
    pos++;

    ldat.l=t_GetStatus.NetName;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FIR];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_SEC];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_THI];
    pos++;
    GpsBuf[pos]=ldat.b[AVL_LSTOR_FOR];
    pos++;

    return(pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsCreatePosRsqInfo
����˵����808����λ�û�ȡ
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_GprsCreatePosRsqInfo(u8 *PackBuf)
{
    u8 GpsBuf[300];
    u16 GpsLen,PackLen;

    GpsLen=YS_GprsPackGpsRsqInfo(GpsBuf);
    PackLen=YS_GprsDealJTBPtlSend(0xB000,GpsBuf,GpsLen,PackBuf,1,1);
    return(PackLen);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsTraceParase
����˵����ƽ̨�·���TRACE ָ���������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GprsTraceParase(u8 *dbuf, u16 dlen)
{
    u16 TraceTimes,TraceDelay;
    u32 TraceEdtime;

    TraceDelay = dbuf[0]*256+dbuf[1];
    TraceEdtime=dbuf[2]*256*256*256+dbuf[3]*256*256+dbuf[4]*256+dbuf[5];
    TraceTimes = (u16)(TraceEdtime/TraceDelay);
    YS_RunTraceInterFace(TraceTimes,TraceDelay);
}

void YS_GprsJTBRegAckDeal(u8 *dbuf, u16 dlen)
{
    u8  fbuf[40];
    u16 i,len;

    switch(dbuf[2])
    {
        case 0: //ע��ɹ�
            len=dlen-3;
            if(len>=40) len=39;
            for(i=0; i<len; i++)
            {
                fbuf[i]=dbuf[3+i];
            }
            fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_JTB_SERV_DOWN_SN,FLH_JTB_SERV_DOWN_SN_LEN,fbuf); //������Ȩ��
            break;

        case 1://�����ѱ�ע��
            YS_GprsDebugString("\nCar already reg",0);
            break;

        case 2://���ݿ����޸ó���
            YS_GprsDebugString("\nno car in lib",0);
            break;

        case 3://�ն����ѱ�ע��
            YS_GprsDebugString("\nTeri already reg",0);
            break;

        case 4://���ݿ����޸��ն�
            YS_GprsDebugString("\nno Teri in lib",0);
            break;

        default:
            YS_GprsDebugString("\nrec unknow reg ack",0);
            break;

    }
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsGetStrChePosUnit
����˵����λ������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_GprsGetStrChePosUnit(u8 *dbuf, u16 dlen, u8 StrID, char *StrRlt)
{
    u16 i,IDCount,pos,DataCount;
    bool Result1;

    IDCount=0;
    Result1=FALSE;
    pos=0;
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]==':')
        {
            IDCount++;
            if(IDCount>=StrID)
            {
                pos=i+1;
                Result1=TRUE;
                break;
            }
        }
    }

    DataCount=0;
//    Result2=FALSE;
    for(i=pos; i<dlen; i++)
    {
        StrRlt[i-pos]=dbuf[i];
        DataCount++;
    }

    if(Result1==TRUE)
    {
        StrRlt[DataCount]=0;
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsParaseHeadTail
����˵������Ϣ���ͷ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_GprsParaseHeadTail(u8 *dbuf, u16 dlen)
{
    bool result;
    u16 i,pos,newlen;
    u8	verify;

//	AVL_TRACE_BIN(dbuf,dlen);
//	YS_GprsDebugString("\nJTB parase3");
    pos=0;  //����ת�廹ԭ
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]==0x7D)
        {
            if(dbuf[i+1]==0x01)
            {
                dbuf[pos]=0x7D;
                i++;
            }
            else if(dbuf[i+1]==0x02)
            {
                dbuf[pos]=0x7E;
                i++;
            }
            else
            {
                dbuf[pos]=dbuf[i];
            }
        }
        else
        {
            dbuf[pos]=dbuf[i];
        }
        pos++;
    }

    newlen=pos;
    verify=0;
    for(i=1; i<newlen-2; i++)
    {
        verify^=dbuf[i];
    }


    if(verify==dbuf[newlen-2])
    {
        for(i=1; i<16; i++)  //������Ϣͷ����
        {
            t_AvlSckParase.a_AddInfo[i-1]=dbuf[i];
        }

        if((dbuf[3]&0x20)==0x20)
        {
            pos=17;
        }
        else
        {
            pos=13;
        }

        t_AvlSckParase.v_CmdID=dbuf[1]*256+dbuf[2];
        t_AvlSckParase.v_dlen=newlen-(pos+2);
        if(t_AvlSckParase.v_dlen>0)
        {
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckParase.a_dbuf[i]=dbuf[pos+i];
            }
        }
        result=TRUE;
    }
    else
    {
        result=FALSE;
    }
    return(result);
}

void YS_GprsLongToPos(u8 *Lonbuf, u8 *aimbuf)
{
    Avl_ULong ldat;
    double f1;
    char strdat[20];
    char strbuf[20];
    u16 i;

    for(i=0; i<4; i++)
    {
        ldat.b[3-i] =Lonbuf[i];
    }

    aimbuf[0]=(u8)(ldat.l/1000000);
    f1 = ldat.l-aimbuf[0]*1000000;
    f1=f1/1000000;
    f1*=60;
    f1/=100;
    sprintf(strbuf,"%.6f",f1);
    for(i=0; i<3; i++)
    {
        strdat[0]=strbuf[2*(i+1)];
        strdat[1]=strbuf[2*(i+1)+1];
        strdat[2]=0;
        aimbuf[i+1]=atoi(strdat);
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsAreaSet_808
����˵����ƽ̨�����·�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GprsAreaSet_808(u8 area, u8 *dbuf, u16 dlen)
{
    u8 pos=0,i;
    u8 AreaBuf[40];
    memset(AreaBuf, 0x00, 40);

    switch (area)
    {
        case 1 ://Բ������
            AreaBuf[pos]=1;//������Ч
            pos++;
            if(((dbuf[7]&0x04)==0x04)||((dbuf[7]&0x08)==0x08))//������
            {
                AreaBuf[pos]=0;//������
                pos++;
            }
            else if(((dbuf[7]&0x10)==0x10)||((dbuf[7]&0x20)==0x20))//������
            {
                AreaBuf[pos]=1;//������
                pos++;
            }
            else
            {
                AreaBuf[pos]=3;
                pos++;
            }
            AreaBuf[pos]=1;//Բ������
            pos++;

            for(i=0; i<4; i++) //����ID
            {
                AreaBuf[pos]=dbuf[2+i];
                pos++;
            }
            YS_GprsLongToPos(&dbuf[12],&AreaBuf[pos]);//����
            pos+=4;

            YS_GprsLongToPos(&dbuf[8],&AreaBuf[pos]);//����
            pos+=4;

            for(i=0; i<4; i++) //�뾶
            {
                AreaBuf[pos]=dbuf[16+i];
                pos++;
            }
            AreaBuf[FLH_PRM_AREA_STOR_LEN-1]=0;//���»�ñ�������
            break;

        case 2 ://��������
            AreaBuf[pos]=1;//������Ч
            pos++;
            if(((dbuf[7]&0x04)==0x04)||((dbuf[7]&0x08)==0x08))//������
            {
                AreaBuf[pos]=0;//������
                pos++;
            }
            else if(((dbuf[7]&0x10)==0x10)||((dbuf[7]&0x20)==0x20))//������
            {
                AreaBuf[pos]=1;//������
                pos++;
            }
            else
            {
                AreaBuf[pos]=3;
                pos++;
            }
            AreaBuf[pos]=2;//��������
            pos++;

            for(i=0; i<4; i++) //����ID
            {
                AreaBuf[pos]=dbuf[2+i];
                pos++;
            }
            YS_GprsLongToPos(&dbuf[12],&AreaBuf[pos]);//���ϽǾ���
            pos+=4;

            YS_GprsLongToPos(&dbuf[8],&AreaBuf[pos]);//���Ͻ�γ��
            pos+=4;

            YS_GprsLongToPos(&dbuf[20],&AreaBuf[pos]);//���½Ǿ���
            pos+=4;

            YS_GprsLongToPos(&dbuf[16],&AreaBuf[pos]);//���½�γ��
            pos+=4;
            AreaBuf[FLH_PRM_AREA_STOR_LEN-1]=0;//���»�ñ�������
            break;

        case 3 ://���������

            break;
        case 0 ://ɾ������
            AreaBuf[pos]=0;//���� ��Ч
        default:
            break;
    }
    YS_GpsInitAreaDeal();
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, AreaBuf, 40);
    YS_PrmWriteOneItem(FLH_PRM_AREA_STOR, FLH_PRM_AREA_STOR_LEN, AreaBuf);
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsAreaDel_808
����˵����ƽ̨�����·�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_GprsAreaDel_808(u8 area, u8 *dbuf, u16 dlen)
{
    u8 AreaBuf[40];
    memset(AreaBuf, 0, 40);
    YS_PrmWriteOneItem(FLH_PRM_AREA_STOR, FLH_PRM_AREA_STOR_LEN, AreaBuf);
    return TRUE;
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsWriteJTBPrm
����˵����ƽ̨�����·�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GprsWriteJTBPrm(u8 *dbuf, u16 dlen)
{
    u16 pos,PrmID;
    u16 PrmLen;
    u8  fbuf[60];
    Avl_ULong ldat;
    bool ResetFlag = FALSE;

    pos=0;
    while(pos<dlen)
    {
        pos+=2;
        PrmID=dbuf[pos]*256+dbuf[pos+1];
        pos+=2;
        PrmLen=dbuf[pos];
        pos++;
        memset(fbuf,0,60);
        memcpy(fbuf,&dbuf[pos],PrmLen);
        switch(PrmID)
        {
            case 0x0001:  //����������
                YS_PrmWriteOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,&dbuf[pos+2]);
                pos+=PrmLen;
                break;

            case 0x0013://IP��ַ
                YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,&dbuf[pos]);
                ResetFlag = TRUE;
                pos+=PrmLen;
                break;

            case 0x0018://�˿�
                YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,&dbuf[pos+2]);
                ResetFlag = TRUE;
                pos+=PrmLen;
                break;

            case 0x0029://GPS�ϴ�Ƶ��
                YS_PrmWriteOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,&dbuf[pos+2]);
                pos+=PrmLen;
                break;

            case 0x0055: //��������
                YS_PrmWriteOneItem(FLH_PRM_SPEED_VALUE,FLH_PRM_SPEED_VALUE_LEN,&dbuf[pos+2]);
                pos+=PrmLen;
                break;

            case 0x0056: //����ʱ��
                YS_PrmWriteOneItem(FLH_PRM_SPEED_TIME,FLH_PRM_SPEED_TIME_LEN,&dbuf[pos+2]);
                pos+=PrmLen;
                break;

            case 0x0080://�������
                ldat.l=dbuf[pos]*256*256*256+dbuf[pos+1]*256*256+dbuf[pos+2]*256+dbuf[pos+3];
                ldat.l *= 100;
                fbuf[0] = ldat.b[AVL_LSTOR_FIR];
                fbuf[1] = ldat.b[AVL_LSTOR_SEC];
                fbuf[2] = ldat.b[AVL_LSTOR_THI];
                fbuf[3] = ldat.b[AVL_LSTOR_FOR];

                YS_PrmWriteOneItem(FLH_PRM_DIS_VALUE,FLH_PRM_DIS_VALUE_LEN,fbuf);
                YS_GpsInitDistance();
                pos+=PrmLen;
                break;

            case 0x0083://���ó��ƺ�
                YS_PrmWriteOneItem(FLH_JTB_PLATE_STRING,FLH_JTB_PLATE_STRING_LEN,&dbuf[pos]);
                pos+=PrmLen;
                break;

            default:
                break;

        }

    }
    if(ResetFlag == TRUE)
    {
        YS_SysRsqSystemRST(YS_RST_FLAG_USER);
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_GprsDealJTBServRecData
����˵���������ݻ���������ȡ���ݰ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_GprsDealJTBServRecData(u8 *buf, u16 len)
{
    bool result;
    u16 i,pos,count;

    if((buf[0]==0x7E)&&(buf[len-1]==0x7E))  //�����ͷ��β��ȷ
    {
        pos=0;
        count=0;
        for(i=0; i<len; i++)
        {
            pos++;
            if(buf[i]==0x7E)
            {
                count++;
                if(count>=2)
                {
                    break;
                }
            }
        }

        if(YS_GprsParaseHeadTail(buf,pos)==TRUE)
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
bool YS_GprsJBTSckPtlUpConver(void)
{
    bool result,ack;
    u16 i,AckCmd;
    u8 fbuf[5];

    result=TRUE;
    memcpy(t_AvlSckConver.a_AddInfo,t_AvlSckParase.a_AddInfo,AVL_ADD_INFO_LEN);

    switch(t_AvlSckParase.v_CmdID)
    {
        case 0x8001: //ƽ̨ͨ��Ӧ��
            AckCmd=t_AvlSckParase.a_dbuf[2]*256+t_AvlSckParase.a_dbuf[3];
            t_AvlSckConver.a_dbuf[0]=t_AvlSckParase.a_dbuf[4];
            t_AvlSckConver.v_dlen=1;
            switch(AckCmd)
            {
                case 0x0002: //����������Ӧ��
                    t_AvlSckConver.v_dlen=0;
                    t_AvlSckConver.v_CmdID=INTER_CMD_GPS_HEART_ACK;
                    YS_RunIdleHeartAck();
                    break;

                case 0x0003: //������ע��Ӧ��
                    t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_UNREG;
                    break;
                case 0x0102: //��������¼Ӧ��
                    YS_GprsDebugString("\nServ log ack cmd",0);
                    t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_LOGACK;
                    if (t_AvlSckConver.a_dbuf[0]==0)
                    {
                        YS_RunLoginServerAck();
                    }
                    else
                    {
                        fbuf[0]=FJYD_TERI_STATUS_INIT;
                        YS_PrmWriteOneItem(FLH_PRM_ACTIVE_FLAG, FLH_PRM_ACTIVE_FLAG_LEN, fbuf);
                    }
                    break;

                case 0x0200: //λ�ü�������ϢӦ��
                    YS_GprsDebugString("\nServ pos ack",0);
                    if(4==t_AvlSckConver.a_dbuf[0])
                    {
                        YS_RunCleanWarmFlag();
                    }
                    else if(0==t_AvlSckConver.a_dbuf[0])
                    {
                        YS_RunCleanWarmACK();
                    }
                    t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_POSACK;
                    break;

                case 0x0800: //ͼƬ�ϴ�Ӧ��
                    t_AvlSckConver.v_dlen=0;
                    t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_MULTINFOACK;
                    break;

                case 0x0801: //ͼƬ����Ӧ��
                    t_AvlSckConver.v_dlen=0;
                    t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_MULSENDACK;
                    break;
                default:
                    result=FALSE;
                    break;
            }
            break;

        case 0x8100: //�ն�ע��Ӧ��
            YS_GprsDebugString("\nServ reg ack cmd",0);
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_REGACK;

            if((2==t_AvlSckConver.a_dbuf[2])||(t_AvlSckConver.a_dbuf[2]>=4))//ע��ʧ��
            {
                break;
            }
            YS_GprsJTBRegAckDeal(t_AvlSckConver.a_dbuf,t_AvlSckParase.v_dlen);
            YS_RunRegServerAck();
            break;

        case 0x8103: //�����ն˲���
            for(i=0; i<t_AvlSckParase.v_dlen-1; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i+1];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen-1;
            t_AvlSckConver.v_CmdID=INTER_CMD_SET_JTB_PRM;
            YS_GprsWriteJTBPrm(t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckParase.a_AddInfo, 17);
            break;

        case 0x8104: //��ѯ�ն˲���
            t_AvlSckConver.v_dlen=0;
            t_AvlSckConver.v_CmdID=INTER_CMD_READ_JTB_PRM_SERV;
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_READPRMACK, t_AvlSckParase.a_AddInfo, 17);
            break;

        case 0x8105: //�ն˿���
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            if(t_AvlSckConver.v_dlen>0)
            {
                for(i=0; i<t_AvlSckConver.v_dlen; i++)
                {
                    t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
                }
                if(0x64==t_AvlSckConver.a_dbuf[0])//���͵�
                {
                    fbuf[0]=0;
                    YS_PrmWriteOneItem(FLH_PRM_RELAY_STOR, FLH_PRM_RELAY_STOR_LEN, fbuf);
#if ENABLE_X1_MODLE == 3
                    YS_MotorIOCtrl(1);
#else
                    YS_RunMotorCtrlInterface();
#endif
                }
                else if(0x65==t_AvlSckConver.a_dbuf[0])//ͨ�͵�
                {
                    fbuf[0]=1;
                    YS_PrmWriteOneItem(FLH_PRM_RELAY_STOR, FLH_PRM_RELAY_STOR_LEN, fbuf);
                    YS_MotorIOCtrl(0);
                    YS_IOACCDydReCount();
                }
            }
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckParase.a_AddInfo, 17);
            t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_CTR;
            break;

        case 0x8201: //�ն�λ�ò�ѯ
            t_AvlSckConver.v_CmdID=INTER_CMD_GET_ONE_GPS;
            YS_RunEntryWakeupMode();
            t_AvlSckParase.a_AddInfo[16]=1;
            YS_GprsServerSendInterface(SERV_UP_CMD_ONEPOS, t_AvlSckParase.a_AddInfo, 17);
            break;

        case 0x8202: //λ�ø���
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_GPRS_GETPOS_BYTIME;
            YS_RunEntryWakeupMode();
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckParase.a_AddInfo, 17);
            YS_GprsTraceParase(t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            break;

        case 0x8400: //�绰�ذ�
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckParase.a_AddInfo, 17);
//         YS_CallOutInterface((char *)(&t_AvlSckConver.a_dbuf[1]));
            break;

        case 0x8500://��������
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            if(0x03&t_AvlSckConver.a_dbuf[0])//���͵�
            {
                fbuf[0]=0;
                YS_PrmWriteOneItem(FLH_PRM_RELAY_STOR, FLH_PRM_RELAY_STOR_LEN, fbuf);
#if ENABLE_X1_MODLE == 3
                    YS_MotorIOCtrl(1);
#else
                    YS_RunMotorCtrlInterface();
#endif
            }
            else
            {
                fbuf[0]=1;
                YS_PrmWriteOneItem(FLH_PRM_RELAY_STOR, FLH_PRM_RELAY_STOR_LEN, fbuf);
                YS_MotorIOCtrl(0);
                YS_IOACCDydReCount();
            }
            YS_GprsServerSendInterface(SERV_UP_CMD_CARCTL, t_AvlSckParase.a_AddInfo, 17);
            break;

        case 0x8600: //����Բ������
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            YS_GprsAreaSet_808(1,t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckConver.a_AddInfo, 17);
            break;

        case 0x8601: //ɾ��Բ������
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            ack=YS_GprsAreaDel_808(1,t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            if(ack==TRUE)
            {
                t_AvlSckParase.a_AddInfo[16]=0;
                YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckConver.a_AddInfo, 17);
            }

            break;

        case 0x8602: //���þ�������
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            YS_GprsAreaSet_808(2,t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            t_AvlSckParase.a_AddInfo[16]=0;
            YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckConver.a_AddInfo, 17);
            break;

        case 0x8603: //ɾ����������
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            ack=YS_GprsAreaDel_808(2,t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            if(ack==TRUE)
            {
                t_AvlSckParase.a_AddInfo[16]=0;
                YS_GprsServerSendInterface(SERV_UP_CMD_ACK, t_AvlSckConver.a_AddInfo, 17);
            }
            break;

        case 0x8604: //���ö��������
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            break;

        case 0x8605: //ɾ�����������
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            break;

        case 0xA800://����λ������
            if(TRUE==YS_GprsGetStrChePosUnit(t_AvlSckParase.a_dbuf,t_AvlSckParase.v_dlen,1,(char *)t_AvlSckConver.a_dbuf))
            {
                t_AvlSckConver.v_dlen=strlen((char *)t_AvlSckConver.a_dbuf);
//                YS_SmsSendRsqPosMsg(t_AvlSckConver.a_dbuf, t_AvlSckConver.v_dlen);
 //               YS_SmsSendUTF8Interface(t_AvlSckConver.a_dbuf, t_AvlSckConver.v_dlen);
                YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, t_AvlSckConver.a_dbuf,t_AvlSckConver.v_dlen);
            }
            break;
#if 0
        case 0x8300: //�ı���Ϣ�·�
            YS_GprsDebugString("\nGet center text down",0);
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen-1;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i+1];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_STOLCD_MSG;
            break;

        case 0x8301: //�¼�����
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_EVENT_SET;
            break;

        case 0x8302: //�����·�
            /*
            		for(i=0; i<t_AvlSckParase.v_dlen; i++)
            		{
            			t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            		}
            		t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;*/
            YS_GprsDebugString("\nGet center rsq down");
            t_AvlSckConver.v_dlen=avltra69_41ParaseRsqDown(t_AvlSckParase.a_dbuf,t_AvlSckParase.v_dlen,t_AvlSckConver.a_dbuf);
            t_AvlSckConver.v_CmdID=INTER_CMD_STOLCD_MSG;
            break;

        case 0x8303: //��Ϣ�㲥�˵�����
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_INFO_MENU;
            break;

        case 0x8304: //��Ϣ����
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen-3;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i+3];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_STOLCD_MSG;
            break;

        case 0x8400: //�绰�ذ�
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_CALL_DEAL;
            break;

        case 0x8401: //���õ绰��
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_PHONE_SET;
            break;

        case 0x8500: //��������
            t_AvlSckConver.a_dbuf[0]=t_AvlSckParase.a_dbuf[0];
            t_AvlSckConver.a_dbuf[1]=t_AvlSckConver.a_AddInfo[10];
            t_AvlSckConver.a_dbuf[2]=t_AvlSckConver.a_AddInfo[11];
            t_AvlSckConver.v_dlen=3;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_CAR_CTRL;
            break;

        case 0x8606: //������·
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTBSET_ROUTE;
            break;

        case 0x8607: //ɾ����·
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTBDEL_ROUTE;
            break;

        case 0x8700: //��¼�ǲɼ�����
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_BOXGET;
            break;

        case 0x8701: //��¼�ǲ�����������
            for(i=0; i<t_AvlSckParase.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_BOXPRM;
            break;

        case 0x8800: //��ý�������ϴ�Ӧ��
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_SERV_JTB_MULDATAACK;
            break;

        case 0x8801: //����ͷ����������
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_SNOPPIC;
            break;

        case 0x8802: //������ý�����ݼ���
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_MULRSQ;
            break;

        case 0x8803: //������ý�������ϴ�
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_MULGET;
            break;

        case 0x8804: //¼����ʼ����
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_VOICEBEIGN;
            break;

        case 0x8900: //��������͸��
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_CLRDOWN;
            break;

        case 0x8A00: //ƽ̨��Կ
            t_AvlSckConver.v_dlen=t_AvlSckParase.v_dlen;
            for(i=0; i<t_AvlSckConver.v_dlen; i++)
            {
                t_AvlSckConver.a_dbuf[i]=t_AvlSckParase.a_dbuf[i];
            }
            t_AvlSckConver.v_CmdID=INTER_CMD_JTB_RSAINFO;
            break;
#endif


        default:
            result=FALSE;
            break;
    }

    return(result);
}

