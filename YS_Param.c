/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Param.c  2013-06-22
���ļ��ù����������Ķ�д����
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

t_Param_Manage t_ParamManage;

/*-----------------------------------------------------------------------------------------
��������YS_SmsCompString
����˵������֤�ַ�������Ϊ�������ַ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_SmsCompString(u8 *PackBuf, u16 dlen, u16 CompLen)
{
    u16 i;
    if(dlen!=CompLen)
    {
        return(FALSE);
    }

    for(i=0; i<dlen; i++)
    {
        if((PackBuf[i]<0x30)||(PackBuf[i]>0x39))
        {
            return(FALSE);
        }
    }
    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmReadOneItem
����˵�����ֽ⴦��GPRMC���ݣ���������������ݽṹ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PrmReadOneItem(u16 PrmID, u16 dlen, u8 *FillBuf )
{
    sjfun_param_read(PrmID,dlen,FillBuf);
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmWriteOneItem
����˵����д��һ��������ϵͳ��������������NV����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PrmWriteOneItem(u16 PrmID, u16 dlen, u8 *dbuf )
{
    sjfun_param_write(PrmID,dlen,dbuf);
}

/*-----------------------------------------------------------------------------------------
��������YS_PamFormatRunParam
����˵�����ͻ����ͻ�㹳�������ָ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PamFormatRunParam(void)
{
    char StrDat[40];
    u8	fbuf[40];
    u16 i,len;

    //fbuf[0]=FJYD_TERI_TYPE_EBIKE;				//�ն�����
    //YS_PrmWriteOneItem(FLH_PRM_TERI_TYPE,FLH_PRM_TERI_TYPE_LEN,fbuf);

    fbuf[0]=FJYD_TERI_STATUS_INIT;			//��ʼ��ϵͳΪδ����״̬
    YS_PrmWriteOneItem(FLH_PRM_ACTIVE_FLAG,FLH_PRM_ACTIVE_FLAG_LEN,fbuf);

    sprintf(StrDat,"cmnet");					//APN��Ϣ
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,fbuf);

    /*fbuf[0]=0x01;								//Խ�籨��500��
    fbuf[1]=0xF4;
    YS_PrmWriteOneItem(FLH_PRM_MOVE_WARN,FLH_PRM_MOVE_WARN_LEN,fbuf);
    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_OUT_SMS,FLH_PRM_OUT_SMS_LEN,fbuf);		//OUT
    YS_PrmWriteOneItem(FLH_PRM_OUT_GPRS,FLH_PRM_OUT_GPRS_LEN,fbuf);	//OUTS*/

    fbuf[0]=1;								//λ���ϱ�ʹ��
    YS_PrmWriteOneItem(FLH_PRM_TRACE_ENABLE,FLH_PRM_TRACE_ENABLE_LEN,fbuf);

    //fbuf[0]=0x40;	//�񶯴�����������ֵ
    //YS_PrmWriteOneItem(FLH_PRM_VIB_BASEVALUE,FLH_PRM_VIB_BASEVALUE_LEN,fbuf);

    fbuf[0]=0x00;								//��λ������15��
    fbuf[1]=0x1e;
    YS_PrmWriteOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);

    fbuf[0]=0x00;								//����������������
    fbuf[1]=0x78;
    YS_PrmWriteOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_VIBWARN_SMS,FLH_PRM_VIBWARN_SMS_LEN,fbuf); //VIB
//	fbuf[0]=1;
//	YS_PrmWriteOneItem(FLH_PRM_VIBWARN_GPRS,FLH_PRM_VIBWARN_GPRS_LEN,fbuf); //VIBS
    fbuf[0]=4;
    YS_PrmWriteOneItem(FLH_PRM_VIB_LEVEL,FLH_PRM_VIB_LEVEL_LEN,fbuf); ////VIBL
    fbuf[0]=10;  //10:4
    fbuf[1]=4;
    YS_PrmWriteOneItem(FLH_PRM_VIB_CHK,FLH_PRM_VIB_CHK_LEN,fbuf); ////VIBL

    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_SMS,FLH_PRM_POWOFFWARN_SMS_LEN,fbuf); //POF
    YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_GPRS,FLH_PRM_POWOFFWARN_GPRS_LEN,fbuf); //POFS
    /*fbuf[0]=0x00; //POFT 120
    fbuf[1]=0x78;
    YS_PrmWriteOneItem(FLH_PRM_POWOFF_TIME,FLH_PRM_POWOFF_TIME_LEN,fbuf); //POFT*/

    /*fbuf[0]=0x00; //POFSLEEPT 120
    fbuf[1]=0x78;
    YS_PrmWriteOneItem(FLH_PRM_POWOFFSLEEP_TIME,FLH_PRM_POWOFFSLEEP_TIME_LEN,fbuf); //POFSLEEPT*/

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_ACC_LOCK,FLH_PRM_ACC_LOCK_LEN,fbuf); //ACCLOCK
    /*fbuf[0]=0;	//ACCLT	180	second
    fbuf[1]=0xB4;
    YS_PrmWriteOneItem(FLH_PRM_ACC_TIME,FLH_PRM_ACC_TIME_LEN,fbuf); //ACCLT*/

    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf); //SLEEP ENABLE
    fbuf[0]=0x00;
    fbuf[1]=0x3c;
    YS_PrmWriteOneItem(FLH_PRM_SLEEP_TIME,FLH_PRM_SLEEP_TIME_LEN,fbuf);	//SLEEP TIME  180 second

    fbuf[0]=0x0E;
    fbuf[1]=0x74;
    YS_PrmWriteOneItem(FLH_PRM_BATT_LOW_LIMIT,FLH_PRM_BATT_LOW_LIMIT_LEN,fbuf);	//BATT LOW LIMIT


    //fbuf[0]=1;//�̵���Ĭ�Ϻ���
    //YS_PrmWriteOneItem(FLH_PRM_RELAY_STOR,FLH_PRM_RELAY_STOR_LEN,fbuf);

    memset(fbuf,0,40);
    YS_PrmWriteOneItem(FLH_PRM_AREA_STOR,FLH_PRM_AREA_STOR_LEN,fbuf);	//AREA ��������
    YS_PrmWriteOneItem(FLH_PRM_RST_RECORD,FLH_PRM_RST_RECORD_LEN,fbuf);//��λ��¼����
    YS_PrmWriteOneItem(FLH_PRM_DIS_VALUE,FLH_PRM_DIS_VALUE_LEN,fbuf);//�������

    fbuf[0]=AGPS_FUNC_UBLOX;
    YS_PrmWriteOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);	//AGPS ����

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_NV_GPS_PYYH,FLH_PRM_NV_GPS_PYYH_LEN,fbuf);	//��GPS�Ż�

    fbuf[0]=0;  //���Կ���
    YS_PrmWriteOneItem(FLH_PRM_DEBUG_ENABLE,FLH_PRM_DEBUG_ENABLE_LEN,fbuf);

    fbuf[0]=0x00; //�յ㲹������
    YS_PrmWriteOneItem(FLH_PRM_ANGLE_SEND_FLAG,FLH_PRM_ANGLE_SEND_FLAG_LEN,fbuf);

    fbuf[0]=1;              //��ʼ���͵�����������
    YS_PrmWriteOneItem(FLH_PRM_NV_POWLOW_FLAG,FLH_PRM_NV_POWLOW_FLAG_LEN,fbuf);

    /*fbuf[0]=0x02;			//��ʼ��״̬�ϱ�ʱ��
    fbuf[1]=0x58;
    YS_PrmWriteOneItem(FLH_PRM_NV_REPORT_TER_STA_TIME,FLH_PRM_NV_REPORT_TER_STA_TIME_LEN,fbuf);*/



    //fbuf[0]=0x01;
    //YS_PrmWriteOneItem(FLH_PRM_NV_ANYNUM_FLAG, FLH_PRM_NV_ANYNUM_FLAG_LEN,fbuf);

    fbuf[0]=0x00;
    YS_PrmWriteOneItem(FLH_PRM_FLI_ENABLE, FLH_PRM_FLI_ENABLE_LEN,fbuf);

    /*fbuf[0]=0x00;        //�񶯼��ʱ��Ĭ��50��
    fbuf[1]=0x32;
    YS_PrmWriteOneItem(FLH_PRM_RF_CHECK_TEME,FLH_PRM_RF_CHECK_TEME_LEN,fbuf);*/


    fbuf[0]=0x00; //��λ����
    fbuf[1]=0x00;
    YS_PrmWriteOneItem(FLH_PRM_RST_NUM, FLH_PRM_RST_NUM_LEN, fbuf);

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_GPS_DEBUG_EN, FLH_PRM_GPS_DEBUG_EN_LEN, fbuf);

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_OBD_DEBUG_EN, FLH_PRM_OBD_DEBUG_EN_LEN, fbuf);

}

void YS_PrmNewNVTest(u8 *dbuf, u16 dlen)
{
#if 0
    u16 i;
    for(i=0; i<dlen; i++)
    {
        YS_Param_Buf[i]=dbuf[i];
    }
    t_ParamManage.WriteFlag=1;
    t_ParamManage.WriteDelay=0;
#endif
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmQuickWriteNV
����˵����NV�Ŀ���д�뺯��������ϵͳ��λ2�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PrmQuickWriteNV(void)
{
    sjfun_param_Quickwr();
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmJudgeParamNeedUpdate
����˵������֤��ǰ����汾�Ƿ���Ҫ����Ĭ�Ϲ�������
                           �����Ҫ���£������°汾��־��������TRUE
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_PrmJudgeParamNeedUpdate(void)
{
    u16 i,len1,len2;
    u8 fbuf[40];
    //bool CompRlt;

    len1=strlen(EBIKE_SOFT_VER);
    YS_PrmReadOneItem(FLH_PRM_SOFT_VER_INFO,FLH_PRM_SOFT_VER_INFO_LEN,fbuf); //����Ƿ��ʼ�����ı�־
    len2=YS_CodeBufRealLen(fbuf,FLH_PRM_SOFT_VER_INFO_LEN);

    if(len2==0)
    {
        for(i=0; i<len1; i++)
        {
            fbuf[i]=EBIKE_SOFT_VER[i];
        }
        fbuf[len1]=0;
        YS_PrmWriteOneItem(FLH_PRM_SOFT_VER_INFO,FLH_PRM_SOFT_VER_INFO_LEN,fbuf);
        return(TRUE);
    }
    else
    {
        for(i=0; i<len1; i++)
        {
            fbuf[i]=EBIKE_SOFT_VER[i];
        }
        fbuf[len1]=0;
        YS_PrmWriteOneItem(FLH_PRM_SOFT_VER_INFO,FLH_PRM_SOFT_VER_INFO_LEN,fbuf);
        return(FALSE);
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmGetTotalSoftVer
����˵������ȡ��ǰϵͳ��ȫ�汾��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PrmGetTotalSoftVer(u8 *fbuf)
{
    u16 i,len,pos;
    char StrDat[40];

    pos = 0;
    YS_GetModuleVer(StrDat);
    len = strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[pos] = StrDat[i];
        pos++;
    }
    fbuf[pos]='_';
    pos++;
    len=strlen(EBIKE_SOFT_VER);
    for(i=0; i<len; i++)
    {
        fbuf[pos] = EBIKE_SOFT_VER[i];
        pos++;
    }
    fbuf[pos]=0;
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmSetFactroyWorkParam
����˵�������س����ĳ�ʼ����������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PrmSetFactroyWorkParam(void)
{
    char StrDat[40];
    u8	fbuf[50];
    u16 i,len;

    if(YS_PrmJudgeParamNeedUpdate()==FALSE)
    {
        return;
    }

    fbuf[0]=0;  //��ֹ���Թ���
    YS_PrmWriteOneItem(FLH_PRM_DEBUG_ENABLE,FLH_PRM_DEBUG_ENABLE_LEN,fbuf);

    sprintf(StrDat,"356823030000000");		//�ն�IMEI
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_TERI_IMEI,FLH_PRM_TERI_IMEI_LEN,fbuf);

//    memset(fbuf,0,FLH_PRM_SIM_CODE_LEN);
    sprintf(StrDat,"1353789600");		//�ն�IMEI
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf); 			//��ʼ��SIM����Ϊ��
    YS_PrmWriteOneItem(FLH_PRM_OWNER_CODE,FLH_PRM_OWNER_CODE_LEN,fbuf);		//��ʼ����������Ϊ��

    sprintf(StrDat,"123456");					//��������
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,fbuf);

//    sprintf(StrDat,"dev.comobot.com");		//������IP
    sprintf(StrDat,"219.234.95.56");		//������IP

    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
    fbuf[0]=0x17;								//�������˿�6000
    fbuf[1]=0x70;
    YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);

    sprintf(StrDat,"cmnet");					//APN��Ϣ
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,fbuf);

    /*fbuf[0]=0x01;								//Խ�籨��500��
    fbuf[1]=0xF4;
    YS_PrmWriteOneItem(FLH_PRM_MOVE_WARN,FLH_PRM_MOVE_WARN_LEN,fbuf);
    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_OUT_SMS, FLH_PRM_OUT_SMS_LEN,fbuf);		//OUT
    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_OUT_GPRS, FLH_PRM_OUT_GPRS_LEN,fbuf);	//OUTS*/

    fbuf[0]=1;								//λ���ϱ�ʹ��
    YS_PrmWriteOneItem(FLH_PRM_TRACE_ENABLE,FLH_PRM_TRACE_ENABLE_LEN,fbuf);

    fbuf[0]=0x00;								//��λ������30��
    fbuf[1]=0x1E;
    YS_PrmWriteOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);

    fbuf[0]=0x00;								//����������������
    fbuf[1]=0x78;
    YS_PrmWriteOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_VIBWARN_SMS,FLH_PRM_VIBWARN_SMS_LEN,fbuf); //VIB
//	fbuf[0]=1;
//	YS_PrmWriteOneItem(FLH_PRM_VIBWARN_GPRS,FLH_PRM_VIBWARN_GPRS_LEN,fbuf); //VIBS

    fbuf[0]=4;
    YS_PrmWriteOneItem(FLH_PRM_VIB_LEVEL,FLH_PRM_VIB_LEVEL_LEN,fbuf); ////VIBL
    fbuf[0]=10;  //10:4
    fbuf[1]=4;
    YS_PrmWriteOneItem(FLH_PRM_VIB_CHK,FLH_PRM_VIB_CHK_LEN,fbuf); ////VIBCHK

    //fbuf[0]=5;  //5��
    //YS_PrmWriteOneItem(FLH_PRM_VIBWARN_DELAY,FLH_PRM_VIBWARN_DELAY_LEN,fbuf); ////VIBT

    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_SMS,FLH_PRM_POWOFFWARN_SMS_LEN,fbuf); //POF
    YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_GPRS,FLH_PRM_POWOFFWARN_GPRS_LEN,fbuf); //POFS

    /*fbuf[0]=0x00; //POFT 120
    fbuf[1]=0x78;
    YS_PrmWriteOneItem(FLH_PRM_POWOFF_TIME,FLH_PRM_POWOFF_TIME_LEN,fbuf); //POFT*/

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_ACC_LOCK,FLH_PRM_ACC_LOCK_LEN,fbuf); //ACCLOCK
    /*fbuf[0]=0x00;	//ACCLT	180��
    fbuf[1]=0xB4;
    YS_PrmWriteOneItem(FLH_PRM_ACC_TIME,FLH_PRM_ACC_TIME_LEN,fbuf); //ACCLT*/

    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf); //SLEEP ENABLE
    fbuf[0]=0x00;
    fbuf[1]=0x3c;
    YS_PrmWriteOneItem(FLH_PRM_SLEEP_TIME,FLH_PRM_SLEEP_TIME_LEN,fbuf);	//SLEEP TIME

    fbuf[0]=0x0E;
    fbuf[1]=0x74;
    YS_PrmWriteOneItem(FLH_PRM_BATT_LOW_LIMIT,FLH_PRM_BATT_LOW_LIMIT_LEN,fbuf);	//BATT LOW LIMIT




    //fbuf[0]=0; //Ĭ�Ͻ�ֹ�δ�λ����
    //YS_PrmWriteOneItem(FLH_PRM_CALL_FUNC,FLH_PRM_CALL_FUNC_LEN,fbuf);	//CALL FUNC

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_CALL_DISP,FLH_PRM_CALL_DISP_LEN,fbuf);	//CALL DISP

    //fbuf[0]=0;
    //YS_PrmWriteOneItem(FLH_PRM_WAKEUP_EN,FLH_PRM_WAKEUP_EN_LEN,fbuf);	//WAKEUP ENABLE
    //fbuf[0]=0x54;	//6Сʱ
    //fbuf[1]=0x60;
    //YS_PrmWriteOneItem(FLH_PRM_WAKEUP_TIME,FLH_PRM_WAKEUP_TIME_LEN,fbuf);	//WAKEUP TIME



    memset(fbuf,0,50);
    //YS_PrmWriteOneItem(FLH_PRM_AREA_STOR,FLH_PRM_AREA_STOR_LEN,fbuf);	//AREA ��������
    YS_PrmWriteOneItem(FLH_PRM_RST_RECORD,FLH_PRM_RST_RECORD_LEN,fbuf);//��λ��¼����


    fbuf[0]=0x40;	//�񶯴�����������ֵ
    YS_PrmWriteOneItem(FLH_PRM_VIB_BASEVALUE,FLH_PRM_VIB_BASEVALUE_LEN,fbuf);

    //fbuf[0]=1;	//Ĭ�ϳ���Ϊͣ��״̬
    //YS_PrmWriteOneItem(FLH_PRM_CAR_RUNSTATUS,FLH_PRM_CAR_RUNSTATUS_LEN,fbuf);

    //fbuf[0]=0x10;	//�������ʱ���趨
    //fbuf[1]=0xE0;
    //YS_PrmWriteOneItem(FLH_PRM_FLIT_SET,FLH_PRM_FLIT_SET_LEN,fbuf);

    //fbuf[0]=0x00;	//�������ʱ�����
    //fbuf[1]=0x00;
    //YS_PrmWriteOneItem(FLH_PRM_FLIT_COUNT,FLH_PRM_FLIT_COUNT_LEN,fbuf);



    fbuf[0]=0x00; //�յ㲹������
    YS_PrmWriteOneItem(FLH_PRM_ANGLE_SEND_FLAG,FLH_PRM_ANGLE_SEND_FLAG_LEN,fbuf);

    //fbuf[0]=10; //������������
    //YS_PrmWriteOneItem(FLH_PRM_WARN_TIMES_MAX, FLH_PRM_WARN_TIMES_MAX_LEN, fbuf);

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_NV_GPS_PYYH,FLH_PRM_NV_GPS_PYYH_LEN,fbuf);	//��GPS�Ż�

    fbuf[0]=113;	//��ʼ��Ĭ�ϵľ�γ����Ϣ
    fbuf[1]=52;
    fbuf[2]=79;
    fbuf[3]=66;
    fbuf[4]=22;
    fbuf[5]=34;
    fbuf[6]=27;
    fbuf[7]=05;
    YS_PrmWriteOneItem(FLH_PRM_LAST_GPSPOINT,FLH_PRM_LAST_GPSPOINT_LEN,fbuf);

    /*fbuf[0]=0x00;        //�񶯼��ʱ��Ĭ��50��
    fbuf[1]=0x32;
    YS_PrmWriteOneItem(FLH_PRM_RF_CHECK_TEME,FLH_PRM_RF_CHECK_TEME_LEN,fbuf);*/



    //fbuf[0]=1;				//RF�ر�ʹ��,Ĭ������ر�
    //YS_PrmWriteOneItem(FLH_PRM_RF_CLOSE_ENABLE,FLH_PRM_RF_CLOSE_ENABLE_LEN,fbuf);

    fbuf[0]=2;  //2002-01-01 12:30:0
    fbuf[1]=1;
    fbuf[2]=1;
    fbuf[3]=12;
    fbuf[4]=30;
    fbuf[5]=0;
    YS_PrmWriteOneItem(FLH_PRM_NV_LAST_POINT_DT,FLH_PRM_NV_LAST_POINT_DT_LEN,fbuf);

    //fbuf[0]=0;
    //YS_PrmWriteOneItem(FLH_PRM_NV_LED_CTRL,FLH_PRM_NV_LED_CTRL_LEN,fbuf);	//LEDָʾ�ƿ���

    //fbuf[0]=0;
    //	YS_PrmWriteOneItem(FLH_PRM_NV_GPRS_CTRL,FLH_PRM_NV_GPRS_CTRL_LEN,fbuf);	//GPRS ���ӿ���

    //fbuf[0]=1;
    //YS_PrmWriteOneItem(FLH_PRM_RF_CHECK_ENABLE,FLH_PRM_RF_CHECK_ENABLE_LEN,fbuf);

    //fbuf[0]=0x01;
    //YS_PrmWriteOneItem(FLH_PRM_NV_ANYNUM_FLAG, FLH_PRM_NV_ANYNUM_FLAG_LEN,fbuf);

    fbuf[0]=AGPS_FUNC_UBLOX;
    YS_PrmWriteOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);	//AGPS ����

    fbuf[0]=1;
    YS_PrmWriteOneItem(FLH_PRM_NV_POWLOW_FLAG,FLH_PRM_NV_POWLOW_FLAG_LEN,fbuf);



    //fbuf[0]=0x00;
    //fbuf[1]=0xB4;
    //YS_PrmWriteOneItem(FLH_PRM_SF_TIME, FLH_PRM_SF_TIME_LEN, fbuf);

    //fbuf[0]=0x00;
    //fbuf[1]=0x78;
    //YS_PrmWriteOneItem(FLH_PRM_TC_TIME,FLH_PRM_TC_TIME_LEN,fbuf);

//	fbuf[0]=0;					//Ĭ�ϰ���״̬
//	YS_PrmWriteOneItem(FLH_PRM_AF_OR_JK_FLAG, FLH_PRM_AF_OR_JK_FLAG_LEN, fbuf);

    //fbuf[0]=1;			//Ĭ��������Ƶ
    //YS_PrmWriteOneItem(FLH_PRM_RF_AUTO_FREQ_EN, FLH_PRM_RF_AUTO_FREQ_EN_LEN,fbuf);

    /*memset(fbuf,0,40);
    YS_PrmWriteOneItem(FLH_PRM_NV_KEYID1,FLH_PRM_NV_KEYID1_LEN,fbuf);
    YS_PrmWriteOneItem(FLH_PRM_NV_KEYID2,FLH_PRM_NV_KEYID2_LEN,fbuf);
    YS_PrmWriteOneItem(FLH_PRM_NV_KEYNUM,FLH_PRM_NV_KEYNUM_LEN,fbuf);	*/

    fbuf[0]=0; //Զ��������־
    YS_PrmWriteOneItem(FLH_PRM_NV_DWFLAG,FLH_PRM_NV_DWFLAG_LEN,fbuf);

    fbuf[0]=0x00; //Զ�������˿�80
    fbuf[1]=0x50;
    YS_PrmWriteOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);

    sprintf(StrDat,"120.24.255.230");			//Զ������������IP 120.24.255.230:80
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);

    sprintf(StrDat,"qq.dogcare.com.cn/zhf/A20");			//HTTP�����ļ�·��
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        fbuf[i]=StrDat[i];
    }
    fbuf[len]=0;
    YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_ADDR, FLH_PRM_NV_AUTOPLAT_ADDR_LEN,fbuf);

    fbuf[0]=1; //�Զ�����ʹ��
    YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_ENABLE,FLH_PRM_NV_AUTOPLAT_ENABLE_LEN,fbuf);

    fbuf[0]=0x00;
    YS_PrmWriteOneItem(FLH_PRM_FLI_ENABLE, FLH_PRM_FLI_ENABLE_LEN,fbuf);

    fbuf[0]=0x00;	//Ĭ��Ϊ���������汾   0Ϊ���� 1Ϊ������
    YS_PrmWriteOneItem(FLH_PRM_SKYLINE_FLAG, FLH_PRM_SKYLINE_FLAG_LEN,fbuf);

    fbuf[0]=0x00; //��λ����
    fbuf[1]=0x00;
    YS_PrmWriteOneItem(FLH_PRM_RST_NUM, FLH_PRM_RST_NUM_LEN, fbuf);

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_GPS_DEBUG_EN, FLH_PRM_GPS_DEBUG_EN_LEN, fbuf);

    fbuf[0]=0;
    YS_PrmWriteOneItem(FLH_PRM_OBD_DEBUG_EN, FLH_PRM_OBD_DEBUG_EN_LEN, fbuf);

    fbuf[0]=1;								//CAN�ϱ�ʹ��
    YS_PrmWriteOneItem(FLH_PRM_CAN_ENABLE,FLH_PRM_CAN_ENABLE_LEN,fbuf);

    fbuf[0]=0x00;								//CAN������30��
    fbuf[1]=0x0a;
    YS_PrmWriteOneItem(FLH_PRM_CAN_TIME,FLH_PRM_CAN_TIME_LEN,fbuf);

    fbuf[0] = 0xbe;
    fbuf[1] = 0xa9;
    fbuf[2] = 0x46;
    fbuf[3] = 0x31;
    fbuf[4] = 0x32;
    fbuf[5] = 0x33;
    fbuf[6] = 0x34;
    fbuf[7] = 0x35;
    fbuf[8] = 0;
    YS_PrmWriteOneItem(FLH_JTB_PLATE_STRING,FLH_JTB_PLATE_STRING_LEN,fbuf);

}

/*-----------------------------------------------------------------------------------------
��������YS_PrmFindStringInbuf
����˵�������һ����������ʶ���ַ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_PrmFindStringInbuf(u8 *dbuf, u16 dlen, char *IndexStr, char *BackStr)
{
    u16 i,j,pos,len,count;
    bool result;
    u8	tdat;

    len=strlen(IndexStr);
    result=FALSE;
    pos=0;
    for(i=0; i<dlen; i++)
    {
        result=TRUE;
        for(j=0; j<len; j++)
        {
            tdat=toupper(dbuf[i+j]);  //�������ĸ�Ĵ�Сд
            if(tdat!=IndexStr[j])
            {
                result=FALSE;
                break;
            }
        }

        if(result==TRUE)
        {
            pos=i+len;
            break;
        }
    }

    if(result==TRUE)
    {
        count=0;
        for(i=pos; i<dlen; i++)
        {
            if(dbuf[i]==']')
            {
                break;
            }
            BackStr[i-pos]=dbuf[i];
            count++;
        }
        BackStr[count]=0;
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmPDNumString
����˵�����ж�������ַ����Ƿ�Ϊ��Ч�����ֽڷ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_PrmPDNumString(char *InputStr, bool EnLenLimit, u16 AskLen, u32 MinValue, u32 MaxValue)
{
    u16 Len;
    u32 RealValue;

    if(YS_CodePDNumString(InputStr)==FALSE)
    {
        return(FALSE);
    }

    Len=strlen(InputStr);

    if(EnLenLimit==TRUE)
    {
        if(AskLen!=Len)
        {
            return(FALSE);
        }
    }

    RealValue=atoi(InputStr);
    if(RealValue<MinValue)
    {
        return(FALSE);
    }

    if(RealValue>MaxValue)
    {
        return(FALSE);
    }

    return(TRUE);
}


/*-----------------------------------------------------------------------------------------
��������YS_PrmCompTwoString
����˵�������Դ�Сд��ʽ�Ƚ������ַ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_PrmCompTwoString(char *Str1, char *Str2)
{
    u16 i,len1,len2;
    bool result;
    u8 dat1,dat2;

    len1=strlen(Str1);
    len2=strlen(Str2);
    if(len1!=len2)
    {
        return(FALSE);
    }
    result=TRUE;
    for(i=0; i<len1; i++)
    {
        dat1=toupper(Str1[i]);
        dat2=toupper(Str2[i]);
        if(dat1!=dat2)
        {
            result=FALSE;
            break;
        }
    }
    return(result);
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmCreateGpsInfo
����˵��������GPS ״̬����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PrmCreateIMEI(char *Imeistr)
{
    u8 fbuf[30];
    u16 i,len;

    YS_PrmReadOneItem(FLH_PRM_TERI_IMEI,FLH_PRM_TERI_IMEI_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_TERI_IMEI_LEN);
    for(i=0; i<len; i++)
    {
        Imeistr[i]=fbuf[i];
    }
    Imeistr[len]=0;
}



/*-----------------------------------------------------------------------------------------
��������YS_PrmCreateGpsInfo
����˵��������GPS ״̬����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PrmCreateGpsInfo(char *GpsInfo)
{
    u16 i,j,loop1,loop2,len;
    u8 dat1,dat2;
    u16 pos;
    char StrDat[20];

    t_Gps_Star_Info t_StarInfo;
    YS_GpsGetStarInfo(&t_StarInfo);
    if(t_StarInfo.StarNum>2)
    {
        loop1=t_StarInfo.StarNum;
        loop2=t_StarInfo.StarNum-1;
        for(i=0; i<loop1; i++)
        {
            for(j=0; j<loop2; j++)
            {
                if(t_StarInfo.StarSign[j+1]>t_StarInfo.StarSign[j])
                {
                    dat1=t_StarInfo.StarSign[j];
                    dat2=t_StarInfo.StarCode[j];
                    t_StarInfo.StarSign[j]=t_StarInfo.StarSign[j+1];
                    t_StarInfo.StarCode[j]=t_StarInfo.StarCode[j+1];
                    t_StarInfo.StarSign[j+1]=dat1;
                    t_StarInfo.StarCode[j+1]=dat2;
                }
            }
        }
    }

    pos=0;
    for(i=0; i<12; i++)
    {
        sprintf(StrDat,"%02d%02d ",t_StarInfo.StarCode[i],t_StarInfo.StarSign[i]);
        len=strlen(StrDat);
        for(j=0; j<len; j++)
        {
            GpsInfo[pos]=StrDat[j];
            pos++;
        }
    }
    pos--;
    GpsInfo[pos]=0;
}


/*
	RdBuf[0]=node->Hour;
	RdBuf[1]=node->Min;
	RdBuf[2]=node->State;
	RdBuf[3]=node->Freq;
	RdBuf[4]=node->Days;
	RdBuf[5]=node->Snooze;*/
/*-----------------------------------------------------------------------------------------
��������YS_PrmGetSysAlarmClock
����˵������ȡϵͳʱ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_PrmGetSysAlarmClock(char *PackBuf)
{
    char StrDat[20];
    u8	ClockBuf[40];
    u8	IDBuf[8];
    u16  i,k,len,pos;

    for(i=0; i<4; i++)
    {
        IDBuf[i]=i;
    }

    sjfun_Read_Alm_Warn(IDBuf,4,ClockBuf);
    pos=0;
    for(i=0; i<4; i++)
    {
        sprintf(StrDat,"%d:%d:%d:%d|",i,ClockBuf[i*6+2],ClockBuf[i*6],ClockBuf[i*6+1]);
        len=strlen(StrDat);
        YS_GprsDebugString(StrDat,0);
        for(k=0; k<len; k++)
        {
            PackBuf[pos]=StrDat[k];
            pos++;
        }
    }
    pos--;
    return(pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_WorkPrmSetDeal
����˵��������������ַ��������в���д������Ĵ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_WorkPrmReadDeal(u8 *dbuf, u16 dlen, u8 *PackBuf, u16 *PackLen)
{
    char	StrDat[100];
    u8 PowStatue;
    u16 i,len,Value,dat,pos;
    u8	fbuf[50];
    //u32 Time;
    //u8 	Chnanel;
    dbuf[dlen]=0;

    pos=0;
    if(YS_PrmCompTwoString(HBYD_PARAM_READ_SOFTVER,(char *)dbuf)) //��ȡ����汾��
    {
        len=strlen(HBYD_PARAM_READ_SOFTVER);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_SOFTVER[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;

        memset(fbuf,0x00,50);
        YS_PrmGetTotalSoftVer(fbuf);
        len = YS_CodeBufRealLen(fbuf,50);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_GSM,(char *)dbuf)) //��ȡGSM �ź�ǿ��
    {
        len=strlen(HBYD_PARAM_READ_GSM);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_GSM[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        Value=YS_RunGetCsqValue();
        Value/=3;
        dat=80+Value;
        if(dat>90)
        {
            dat=100;
        }
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_GPS,(char *)dbuf)) //��ȡGPS ��������
    {
        len=strlen(HBYD_PARAM_READ_GPS);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_GPS[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        YS_PrmCreateGpsInfo(StrDat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VBAT,(char *)dbuf)) //��ȡ��ǰ������ѹ
    {
        len=strlen(HBYD_PARAM_READ_VBAT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_VBAT[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        dat=YS_RunGetBattValue();
        sprintf(StrDat,"%04d000:5000000:000000",dat);	//�ڶ���Ϊ�ⲿ��Դֵ
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_LOGIN,(char *)dbuf)) //��ȡ��¼״̬???
    {
        len=strlen(HBYD_PARAM_READ_LOGIN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_LOGIN[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        PackBuf[pos]='1';
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_IMSI,(char *)dbuf)) //��ȡSIM ��IMSI
    {
        len=strlen(HBYD_PARAM_READ_IMSI);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_IMSI[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        YS_RunGetIMSI(StrDat); 	//IMSI ��
        for(i=0; i<15; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_IMEI,(char *)dbuf)) //��ȡ�豸IMEI
    {
        len=strlen(HBYD_PARAM_READ_IMEI);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_IMEI[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        YS_PrmReadOneItem(FLH_PRM_TERI_IMEI,FLH_PRM_TERI_IMEI_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_TERI_IMEI_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_ACC,(char *)dbuf)) //��ȡACC ��ǰ״̬
    {
        len=strlen(HBYD_PARAM_READ_ACC);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_ACC[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        if(YS_RunGetACCStatus()==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_UPDELAY,(char *)dbuf)) //��ȡ�����ϱ�Ƶ��
    {
        len=strlen(HBYD_PARAM_NAME_UPDELAY);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_UPDELAY[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_CIRCLEAREA,(char *)dbuf)) //��ȡԽ�籨������
    {
    	len=strlen(HBYD_PARAM_NAME_CIRCLEAREA);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_CIRCLEAREA[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_MOVE_WARN,FLH_PRM_MOVE_WARN_LEN,fbuf);
    	dat=fbuf[0]*256+fbuf[1];
    	sprintf(StrDat,"%d",dat);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_DOMAIN,(char *)dbuf)) //��ȡ����������
    {
        len=strlen(HBYD_PARAM_READ_DOMAIN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_DOMAIN[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
        PackBuf[pos]=':';
        pos++;
        YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_PWM,(char *)dbuf)) //��ȡ��������
    {
        len=strlen(HBYD_PARAM_NAME_PWM);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_PWM[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_PASSWORD_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_TRACE,(char *)dbuf))
    {
        len=strlen(HBYD_PARAM_NAME_TRACE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_TRACE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_TRACE_ENABLE,FLH_PRM_TRACE_ENABLE_LEN,fbuf);
        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_PULSE,(char *)dbuf)) //��ȡ����������
    {
        len=strlen(HBYD_PARAM_NAME_PULSE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_PULSE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_PHONE,(char *)dbuf)) //��ȡSIM ����
    {
        len=strlen(HBYD_PARAM_NAME_PHONE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_PHONE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_SIM_CODE_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_USER,(char *)dbuf)) //��ȡ�û�����
    {
        len=strlen(HBYD_PARAM_NAME_USER);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_USER[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_OWNER_CODE,FLH_PRM_OWNER_CODE_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_OWNER_CODE_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VIB,(char *)dbuf)) //�񶯱�������ʹ��
    {
    	len=strlen(HBYD_PARAM_NAME_VIB);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_VIB[i];
    		pos++;
    	}
    	//YS_PrmReadOneItem(FLH_PRM_VIBWARN_SMS,FLH_PRM_VIBWARN_SMS_LEN,fbuf);
    	if(fbuf[0]==1)
    	{
    		PackBuf[pos]='1';
    	}
    	else
    	{
    		PackBuf[pos]='0';
    	}
    	pos++;
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VIBS,(char *)dbuf)) //�񶯱���������ʹ��
    {
    	len=strlen(HBYD_PARAM_NAME_VIBS);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_VIBS[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_VIBWARN_GPRS,FLH_PRM_VIBWARN_GPRS_LEN,fbuf);
    	if(fbuf[0]==1)
    	{
    		PackBuf[pos]='1';
    	}
    	else
    	{
    		PackBuf[pos]='0';
    	}
    	pos++;
    }*/
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VIBL,(char *)dbuf)) //��ȡ�񶯼���
    {
        len=strlen(HBYD_PARAM_NAME_VIBL);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_VIBL[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_VIB_LEVEL,FLH_PRM_VIB_LEVEL_LEN,fbuf);
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_POF,(char *)dbuf)) //��ȡ�ϵ籨������ʹ��
    {
        len=strlen(HBYD_PARAM_NAME_POF);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_POF[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_POWOFFWARN_SMS,FLH_PRM_POWOFFWARN_SMS_LEN,fbuf);
        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_POFS,(char *)dbuf)) //��ȡ�ϵ籨����������ʹ��
    {
        len=strlen(HBYD_PARAM_NAME_POFS);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_POFS[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_POWOFFWARN_GPRS,FLH_PRM_POWOFFWARN_GPRS_LEN,fbuf);
        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_LBV,(char *)dbuf)) //��ȡ��ص͵�ѹ��������
    {
        len=strlen(HBYD_PARAM_NAME_LBV);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_LBV[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_BATT_LOW_LIMIT,FLH_PRM_BATT_LOW_LIMIT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d000",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SLEEP,(char *)dbuf)) //��ȡ���߿���
    {
        len=strlen(HBYD_PARAM_NAME_SLEEP);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SLEEP[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf);
        Value=fbuf[0];
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }

    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_CALLLOCK,(char *)dbuf)) //��ȡ���繦��
    {
    	len=strlen(HBYD_PARAM_NAME_CALLLOCK);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_CALLLOCK[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_CALL_FUNC,FLH_PRM_CALL_FUNC_LEN,fbuf);
    	Value=fbuf[0];
    	sprintf(StrDat,"%d",fbuf[0]);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_CALLDISP,(char *)dbuf)) //��ȡ���Լ�⹦��
    {
    	len=strlen(HBYD_PARAM_NAME_CALLDISP);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_CALLDISP[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_CALL_DISP,FLH_PRM_CALL_DISP_LEN,fbuf);
    	Value=fbuf[0];
    	sprintf(StrDat,"%d",fbuf[0]);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/


    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VIBCHK,(char *)dbuf))  //�񶯱���������
    {
        len=strlen(HBYD_PARAM_NAME_VIBCHK);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_VIBCHK[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_VIB_CHK,FLH_PRM_VIB_CHK_LEN,fbuf);
        sprintf(StrDat,"%d:%d",fbuf[0],fbuf[1]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_POFT,(char *)dbuf)) //�ϵ籨�����ʱ��
    {
    	len=strlen(HBYD_PARAM_NAME_POFT);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_POFT[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_POWOFF_TIME,FLH_PRM_POWOFF_TIME_LEN,fbuf);
    	Value=fbuf[0]*256+fbuf[1];
    	sprintf(StrDat,"%d",Value);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SLEEPT,(char *)dbuf)) //���߼��ʱ��
    {
        len=strlen(HBYD_PARAM_NAME_SLEEPT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SLEEPT[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SLEEP_TIME,FLH_PRM_SLEEP_TIME_LEN,fbuf);
        Value=fbuf[0]*256+fbuf[1];
        Value/=60;	//ת��Ϊ����
        sprintf(StrDat,"%d",Value);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_ACCLT,(char *)dbuf)) //ACC ���ʱ��
    {
    	len=strlen(HBYD_PARAM_NAME_ACCLT);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_ACCLT[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_ACC_TIME,FLH_PRM_ACC_TIME_LEN,fbuf);
    	Value=fbuf[0]*256+fbuf[1];
    	sprintf(StrDat,"%d",Value);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_ACCLOCK,(char *)dbuf)) //ACC ���ʹ��
    {
    	len=strlen(HBYD_PARAM_NAME_ACCLOCK);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_ACCLOCK[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_ACC_LOCK,FLH_PRM_ACC_LOCK_LEN,fbuf);
    	Value=fbuf[0];
    	sprintf(StrDat,"%d",fbuf[0]);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_OUT,(char *)dbuf)) //��ȡԽ�籨�����Ų���
    {
    	len=strlen(HBYD_PARAM_NAME_OUT);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_OUT[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_OUT_SMS,FLH_PRM_OUT_SMS_LEN,fbuf);

    	if(fbuf[0]==1)
    	{
    		PackBuf[pos]='1';
    	}
    	else
    	{
    		PackBuf[pos]='0';
    	}
    	pos++;
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_OUTS,(char *)dbuf)) //��ȡԽ�������ʹ��
    {
    	len=strlen(HBYD_PARAM_NAME_OUTS);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_OUTS[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_OUT_GPRS,FLH_PRM_OUT_GPRS_LEN,fbuf);

    	if(fbuf[0]==1)
    	{
    		PackBuf[pos]='1';
    	}
    	else
    	{
    		PackBuf[pos]='0';
    	}
    	pos++;
    }*/
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_WAKEUP,(char *)dbuf)) //��ȡ�Զ�����ʹ��
    {
    	len=strlen(HBYD_PARAM_NAME_WAKEUP);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_WAKEUP[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_WAKEUP_EN,FLH_PRM_WAKEUP_EN_LEN,fbuf);
    	dat=fbuf[0];
    	sprintf(StrDat,"%d",dat);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_WAKEUPT,(char *)dbuf)) //��ȡ�Զ���������
    {
    	len=strlen(HBYD_PARAM_NAME_WAKEUPT);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_WAKEUPT[i];
    		pos++;
    	}
    	YS_PrmReadOneItem(FLH_PRM_WAKEUP_TIME,FLH_PRM_WAKEUP_TIME_LEN,fbuf);
    	dat=fbuf[0]*256+fbuf[1];
    	dat/=60;	//ת��Ϊ����
    	sprintf(StrDat,"%d",dat);
    	len=strlen(StrDat);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=StrDat[i];
    		pos++;
    	}
    }*/

    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_APN,(char *)dbuf)) //��ȡAPN ��Ϣ
    {
        len=strlen(HBYD_PARAM_NAME_APN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_APN[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_PPP_APN_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SERVIP,(char *)dbuf)) //��ȡ������IP
    {
        len=strlen(HBYD_PARAM_NAME_SERVIP);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SERVIP[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SERVPORT,(char *)dbuf)) //��ȡ�������˿�
    {
        len=strlen(HBYD_PARAM_NAME_SERVPORT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SERVPORT[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_EQUTYPE,(char *)dbuf)) //��ȡ�ն�����
    {
        len=strlen(HBYD_PARAM_NAME_EQUTYPE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_EQUTYPE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_TERI_TYPE,FLH_PRM_TERI_TYPE_LEN,fbuf);
        dat=fbuf[0];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_AGPS_EN,(char *)dbuf)) //AGPS ���ܲ�������
    {
        len=strlen(HBYD_PARAM_NAME_AGPS_EN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_AGPS_EN[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_GPS_FINTER_EN,(char *)dbuf)) //�Ƿ���Ư���Ż�
    {
        len=strlen(HBYD_PARAM_NAME_GPS_FINTER_EN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_GPS_FINTER_EN[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_GPS_PYYH,FLH_PRM_NV_GPS_PYYH_LEN,fbuf);
        dat=fbuf[0];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_VIB_BASE,(char *)dbuf)) //��ȡ�񶯱�������ֵ
    {
        len=strlen(HBYD_PARAM_NAME_VIB_BASE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_VIB_BASE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_VIB_BASEVALUE,FLH_PRM_VIB_BASEVALUE_LEN,fbuf);
        dat=fbuf[0];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_ANGLE_SEND,(char *)dbuf)) //�յ㲹��ʹ��
    {
        len=strlen(HBYD_PARAM_NAME_ANGLE_SEND);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_ANGLE_SEND[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_ANGLE_SEND_FLAG,FLH_PRM_ANGLE_SEND_FLAG_LEN,fbuf);

        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }

    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_CHARGE_STATUS,(char *)dbuf)) //��ȡ���״̬
    {
        len=strlen(HBYD_PARAM_READ_CHARGE_STATUS);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_READ_CHARGE_STATUS[i];
            pos++;
        }
        PackBuf[pos]= '=';
        pos++;
        if(YS_GetChargeStatue()==1)
        {
            PackBuf[pos]='1';
        }
        else if(YS_GetChargeStatue()==0)
        {
            YS_RunGetStatusInfo(FJYD_STATUS_ID_POWER,&PowStatue);
            if(PowStatue==1)
            {
                PackBuf[pos]='2';
            }
            else
            {
                PackBuf[pos]='0';
            }
        }
        pos++;
        //YS_GprsDebugString(char * DebugStr, kal_uint32 Value)
    }
    /*else if(YS_PrmCompTwoString(HBYD_PARAM_READ_FACMODE,(char *)dbuf)) //��ȡ����ģʽ״̬
    {
    	len=strlen(HBYD_PARAM_NAME_FACMODE);
    	for(i=0; i<len; i++)
    	{
    		PackBuf[pos]=HBYD_PARAM_NAME_FACMODE[i];
    		pos++;
    	}
    	if(YS_RunGetFactoryMode()==TRUE)
    	{
    		PackBuf[pos]='1';
    	}
    	else
    	{
    		PackBuf[pos]='0';
    	}
    	pos++;
    }*/
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_DW_ADDR,(char *)dbuf)) //��ȡ������IP
    {
        len=strlen(HBYD_PARAM_NAME_DW_ADDR);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_DW_ADDR[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_NV_DWADDR_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SJIP,(char *)dbuf)) //��ȡ������IP
    {
        len=strlen(HBYD_PARAM_NAME_SJIP);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SJIP[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);
        len=YS_CodeBufRealLen(fbuf,FLH_PRM_NV_DWADDR_LEN);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=fbuf[i];
            pos++;
        }
    }

    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_DW_PORT,(char *)dbuf)) //��ȡ�������˿�
    {
        len=strlen(HBYD_PARAM_NAME_DW_PORT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_DW_PORT[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_DLPORT,(char *)dbuf)) //��ȡ�������˿�
    {
        len=strlen(HBYD_PARAM_NAME_DLPORT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_DLPORT[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);
        dat=fbuf[0]*256+fbuf[1];
        sprintf(StrDat,"%d",dat);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_DW_FLAG,(char *)dbuf)) //GPRS ���ܿ���
    {
        len=strlen(HBYD_PARAM_NAME_DW_FLAG);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_DW_FLAG[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_DWFLAG,FLH_PRM_NV_DWFLAG_LEN,fbuf);
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_pARAM_READ_AUTO_ENABLE,(char *)dbuf)) //�Զ���������ʹ��
    {
        len=strlen(HBYD_pARAM_NAME_AUTO_ENABLE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_pARAM_NAME_AUTO_ENABLE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_NV_AUTOPLAT_ENABLE,FLH_PRM_NV_AUTOPLAT_ENABLE_LEN,fbuf);
        sprintf(StrDat,"%d",fbuf[0]);
        len=strlen(StrDat);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=StrDat[i];
            pos++;
        }
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_UPDATE_MODE,(char *)dbuf)) //��ȡ����ģʽ
    {
        len=strlen(HBYD_PARAM_NAME_UPDATE_MODE);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_UPDATE_MODE[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_UPDATE_MODE,FLH_PRM_UPDATE_MODE_LEN,fbuf);
        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SYNC_DT,(char *)dbuf)) //��ȡϵͳʱ��
    {
        len=strlen(HBYD_PARAM_NAME_SYNC_DT);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SYNC_DT[i];
            pos++;
        }
        len=YS_SysGetSystemTimeToString((char *)&PackBuf[pos]);
        pos+=len;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_ALARM_CLOCK,(char *)dbuf)) //��ȡ��������
    {
        len=strlen(HBYD_PARAM_NAME_ALARM_CLOCK);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_ALARM_CLOCK[i];
            pos++;
        }
        len=YS_PrmGetSysAlarmClock((char *)&PackBuf[pos]);
        pos+=len;
    }
    else if(YS_PrmCompTwoString(HBYD_PARAM_READ_SKYLINE_FLAG,(char *)dbuf)) //��ȡ�Ƿ�Ϊ���ߵ���ģʽ
    {
        len=strlen(HBYD_PARAM_NAME_SKYLINE_FLAG);
        for(i=0; i<len; i++)
        {
            PackBuf[pos]=HBYD_PARAM_NAME_SKYLINE_FLAG[i];
            pos++;
        }
        YS_PrmReadOneItem(FLH_PRM_SKYLINE_FLAG,FLH_PRM_SKYLINE_FLAG_LEN,fbuf);
        if(fbuf[0]==1)
        {
            PackBuf[pos]='1';
        }
        else
        {
            PackBuf[pos]='0';
        }
        pos++;
    }
    else
    {
        return(FALSE);
    }
    *PackLen=pos;
    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmSetVibChk
����˵��������������ַ����������񶯱�������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PrmSetVibChk(char *DataString)
{
    u16 i,len,pos,count;
    char StrDat[10];
    u8 fbuf[10];

    pos=0;
    len=strlen(DataString);
    count=0;
    for(i=0; i<len; i++)
    {
        if(DataString[i]==':')
        {
            pos=i+1;
            break;
        }
        count++;
        StrDat[i]=DataString[i];
    }
    StrDat[count]=0;
    fbuf[0]=atoi(StrDat);
    count=0;
    for(i=pos; i<len; i++)
    {
        StrDat[i-pos]=DataString[i];
        count++;
    }
    StrDat[count]=0;
    fbuf[1]=atoi(StrDat);
    YS_PrmWriteOneItem(FLH_PRM_VIB_CHK,FLH_PRM_VIB_CHK_LEN,fbuf);
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmSetServerParam
����˵��������������ַ�����������SOCKET ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_PrmSetServerParam(char *StrInfo)
{
    u16 i,pos,len,count,dat;
    u8 fbuf[50];
    char StrDat[10];
    bool FindRlt;

    len=strlen(StrInfo);
    FindRlt=FALSE;
    pos=0;
    for(i=0; i<len; i++)
    {
        if(StrInfo[i]==':')
        {
            pos=i;
            FindRlt=TRUE;
            break;
        }
    }

    if(FindRlt==FALSE)
    {
        return(FALSE);
    }

    for(i=0; i<pos; i++)
    {
        fbuf[i]=StrInfo[i];
    }
    fbuf[pos]=0;
    YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);

    pos++;
    count=0;
    for(i=pos; i<len; i++)
    {
        if(StrDat[i]==']')
        {
            break;
        }

        if(count<6)
        {
            StrDat[i-pos]=StrInfo[i];
            count++;
        }
    }
    StrDat[count]=0;
    dat=atoi(StrDat);
    fbuf[0]=dat/256;
    fbuf[1]=dat%256;
    YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
    YS_SysRsqSystemRST(YS_RST_FLAG_ACTIVE);//add zhf 0823
    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmCreateIMEIVerify
����˵���������ն�IMEI ��У����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_PrmCreateIMEIVerify(char *IMEIStr)
{
    u16 i,Value;
    u8 dat;

    Value=0;
    for(i=0; i<7; i++) //�ۼ�����λ֮��
    {
        Value+=(IMEIStr[i*2]-0x30);
    }

    for(i=0; i<7; i++) //����ż��λ����
    {
        dat=IMEIStr[i*2+1]-0x30;
        dat*=2;
        Value+=(dat/10);
        Value+=(dat%10);
    }
    dat=Value%10;
    if(dat>0)
    {
        dat=10-dat;
    }
    dat+=0x30;
    return(dat);
}


/*-----------------------------------------------------------------------------------------
��������YS_PrmCreateIMEIInfo
����˵�������������ն�ID ���Զ�����IMEI��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PrmCreateIMEIInfo()
{

}

void YS_PrmSetOneAlarmByStr(char *ClockDat, u8 *PackBuf)
{
    char StrDat[10];
    u16 len;

    len=strlen(ClockDat);
    YS_CodeGetItemInBuf((u8 *)ClockDat,len,(u8 *)StrDat,0,':',4);	//����
    PackBuf[0]=atoi(StrDat);
    YS_CodeGetItemInBuf((u8 *)ClockDat,len,(u8 *)StrDat,1,':',4);	//ʹ��
    PackBuf[1]=atoi(StrDat);
    YS_CodeGetItemInBuf((u8 *)ClockDat,len,(u8 *)StrDat,2,':',4);	//ʱ
    PackBuf[2]=atoi(StrDat);
    YS_CodeGetItemInBuf((u8 *)ClockDat,len,(u8 *)StrDat,3,':',4);	//��
    PackBuf[3]=atoi(StrDat);
}

void YS_PrmSetAlarmClockDebug(u8 Flow)
{
    char DispBuf[100];

    sprintf(DispBuf,"Set clock Flow:%d",Flow);
    YS_UartDataOutInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8 *)DispBuf,strlen(DispBuf));
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmSetAlarmClock
����˵��������ϵͳ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_PrmSetAlarmClock(char *InputStr)
{
    u16 i,count,len,pos,ItemLen;
    char ClockStr[20];
    u8	ClockBuf[20];

    len=strlen(InputStr);
    count=0;
    for(i=0; i<len; i++)
    {
        if(InputStr[i]=='|')
        {
            count++;
        }
    }

    if(count>4)
    {
        YS_PrmSetAlarmClockDebug(1);
        return(FALSE);
    }

    pos=0;
    for(i=0; i<4; i++)
    {
        ItemLen=YS_CodeGetItemInBuf((u8 *)InputStr,len,(u8 *)ClockStr,i,'|',10);
        ClockStr[ItemLen]=0;
        YS_PrmSetOneAlarmByStr(ClockStr,&ClockBuf[pos]);
        pos+=4;
    }
    sjfun_Set_Alm_Warn(ClockBuf,pos);
    YS_PrmSetAlarmClockDebug(2);
    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
��������YS_WorkPrmSetDeal
����˵��������������ַ��������в���д������Ĵ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_WorkPrmSetDeal(u8 *dbuf, u16 dlen)
{
    char StrDat[50];
    u16 i,Value,len;
    u8	fbuf[50];
    Avl_ULong ldat;

    if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_UPDELAY,StrDat))  //�����ϱ�����
    {
        if(YS_PrmPDNumString(StrDat,FALSE,0,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_DOMAIN,StrDat)) //�л���������ַ
    {
        YS_PrmSetServerParam(StrDat);
        return(TRUE);
    }
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_CIRCLEAREA,StrDat)) //����Խ�籨������
     {
    	if(YS_PrmPDNumString(StrDat,FALSE,0,0,0xFFFF)==FALSE)
    	{
    		return(FALSE);
    	}
    	else
    	{
    		Value=atoi(StrDat);
    		fbuf[0]=Value/256;
    		fbuf[1]=Value%256;
    		YS_PrmWriteOneItem(FLH_PRM_MOVE_WARN,FLH_PRM_MOVE_WARN_LEN,fbuf);
    		return(TRUE);
    	}
     }*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_TRACE,StrDat)) //���ù켣ʹ��
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')
            {
                fbuf[0]=1;
            }
            else
            {
                fbuf[0]=0;
            }
            YS_PrmWriteOneItem(FLH_PRM_TRACE_ENABLE,FLH_PRM_TRACE_ENABLE_LEN,fbuf);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_PULSE,StrDat)) //����������
    {
        if(YS_PrmPDNumString(StrDat,FALSE,0,10,300)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_HEART_TIME,FLH_PRM_HEART_TIME_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_TERIID,StrDat)) //�����ն�ID��
    {
        if(YS_SmsCompString((u8 *)StrDat,strlen(StrDat),11)==FALSE)
        {
            return(FALSE);
        }
        else
        {
//		    YS_CodeBCDCharConver(StrDat,,fbuf);
//			YS_PrmWriteOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf);
//			YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_PHONE,StrDat)) //����SIM����
    {
        if(YS_SmsCompString((u8 *)StrDat,strlen(StrDat),11)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<11; i++)
            {
                fbuf[i]=StrDat[i];
            }
            fbuf[11]=0;
            YS_PrmWriteOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_USER,StrDat)) //���ó�������
    {
        if(YS_SmsCompString((u8 *)StrDat,strlen(StrDat),11)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<11; i++)
            {
                fbuf[i]=StrDat[i];
            }
            fbuf[11]=0;
            YS_PrmWriteOneItem(FLH_PRM_OWNER_CODE,FLH_PRM_OWNER_CODE_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIB,StrDat)) //�����񶯶��ſ���
    {
      if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
      {
    	return(FALSE);
      }
      else
      {
    	 if(StrDat[0]=='1')
    	 {
    		 fbuf[0]=1;
    	 }
    	 else
    	 {
    		 fbuf[0]=0;
    	 }
    	// YS_PrmWriteOneItem(FLH_PRM_VIBWARN_GPRS,FLH_PRM_VIBWARN_GPRS_LEN,fbuf); //�޸�ΪGPRS ����
    	// YS_PrmWriteOneItem(FLH_PRM_VIBWARN_SMS,FLH_PRM_VIBWARN_SMS_LEN,fbuf);
    	 return(TRUE);
      }
    }*/
    /*  else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIBS,StrDat)) //�����񶯷���������
      {
    	  if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	  {
    		return(FALSE);
    	  }
    	  else
    	  {
    		 if(StrDat[0]=='1')
    		 {
    			 fbuf[0]=1;
    		 }
    		 else
    		 {
    			 fbuf[0]=0;
    		 }
    		 YS_PrmWriteOneItem(FLH_PRM_VIBWARN_GPRS,FLH_PRM_VIBWARN_GPRS_LEN,fbuf);
    		 return(TRUE);
    	  }
      }	  */
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIBL,StrDat)) //������������
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,15)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_VIB_LEVEL,FLH_PRM_VIB_LEVEL_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_POF,StrDat)) //���öϵ���ſ���
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')
            {
                fbuf[0]=1;
            }
            else
            {
                fbuf[0]=0;
            }
            YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_SMS,FLH_PRM_POWOFFWARN_SMS_LEN,fbuf);
            //YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_GPRS,FLH_PRM_POWOFFWARN_GPRS_LEN,fbuf); //�޸�ΪGPRS ����
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_POFS,StrDat)) //���öϵ����������
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')
            {
                fbuf[0]=1;
            }
            else
            {
                fbuf[0]=0;
            }
            YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_GPRS,FLH_PRM_POWOFFWARN_GPRS_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_LBV,StrDat))  //���õ�ص͵�ѹ����
    {
        if(YS_PrmPDNumString(StrDat,TRUE,7,3000000,4200000)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            ldat.l=atoi(StrDat);
            Value=ldat.l/1000;
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_BATT_LOW_LIMIT,FLH_PRM_BATT_LOW_LIMIT_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SLEEP,StrDat)) //����ʹ�ܿ���
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_SLEEP_CTRL,FLH_PRM_SLEEP_CTRL_LEN,fbuf);
            return(TRUE);
        }
    }

    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_CALLLOCK,StrDat)) //���빦������
    {
     if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
     {
    	return(FALSE);
     }
     else
     {
    	 Value=atoi(StrDat);
    	 fbuf[0]=Value%256;
    	 YS_PrmWriteOneItem(FLH_PRM_CALL_FUNC,FLH_PRM_CALL_FUNC_LEN,fbuf);
    	 return(TRUE);
     }
    }*/
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_CALLDISP,StrDat)) //����������֤
     {
    	 if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	else
    	{
    		Value=atoi(StrDat);
    		fbuf[0]=Value%256;
    		YS_PrmWriteOneItem(FLH_PRM_CALL_DISP,FLH_PRM_CALL_DISP_LEN,fbuf);
    		return(TRUE);
    	}
     }*/


    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIBCHK,StrDat)) //�񶯱�������
    {
        YS_PrmSetVibChk(StrDat);
        return(TRUE);
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_POFT,StrDat)) //�ϵ籨����ʱ
    {
      if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
      {
    	return(FALSE);
      }
      else
      {
    	  Value=atoi(StrDat);
    	  fbuf[0]=Value/256;
    	  fbuf[1]=Value%256;
    	  YS_PrmWriteOneItem(FLH_PRM_POWOFF_TIME,FLH_PRM_POWOFF_TIME_LEN,fbuf);
    	  return(TRUE);
     }
    }*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SLEEPT,StrDat)) //����ʱ��
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            Value*=60;
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_SLEEP_TIME,FLH_PRM_SLEEP_TIME_LEN,fbuf);
            return(TRUE);
        }
    }
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_ACCLT,StrDat))	//����ACC ������ʱ��
     {
    	 if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {
    		Value=atoi(StrDat);
    		fbuf[0]=Value/256;
    		fbuf[1]=Value%256;
    		YS_PrmWriteOneItem(FLH_PRM_ACC_TIME,FLH_PRM_ACC_TIME_LEN,fbuf);
    		return(TRUE);
    	 }
     }*/
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_ACCLOCK,StrDat)) //����ACC ���ʹ��
     {
    	 if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {
    		if(StrDat[0]=='1')
    		{
    			fbuf[0]=1;
    		}
    		else
    		{
    			fbuf[0]=0;
    		}
    		YS_PrmWriteOneItem(FLH_PRM_ACC_LOCK,FLH_PRM_ACC_LOCK_LEN,fbuf);
    		return(TRUE);
    	 }
     }*/
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_OUT,StrDat)) //Խ�籨������ʹ��
     {
    	 if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {

    		 if(StrDat[0]=='1')
    		 {
    			 fbuf[0]=1;
    		 }
    		 else
    		 {
    			fbuf[0]=0;
    		 }
    //			 YS_PrmWriteOneItem(FLH_PRM_OUT_SMS,FLH_PRM_OUT_SMS_LEN,fbuf);
    		YS_PrmWriteOneItem(FLH_PRM_OUT_GPRS,FLH_PRM_OUT_GPRS_LEN,fbuf); //�޸�ΪGPRS ����
    		 return(TRUE);
    	 }
     }*/
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_OUTS,StrDat))//Խ�籨��������ʹ��
     {
    	 if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {
    		 if(StrDat[0]=='1')
    		 {
    			 fbuf[0]=1;
    		 }
    		 else
    		 {
    			 fbuf[0]=0;
    		 }
    	 	YS_PrmWriteOneItem(FLH_PRM_OUT_GPRS,FLH_PRM_OUT_GPRS_LEN,fbuf);
    	 	return(TRUE);
    	 }
     }*/
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_WAKEUP,StrDat)) //�Զ�����ʹ��
     {
    	 if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {
    		Value=atoi(StrDat);
    		fbuf[0]=Value%256;
    		 if(StrDat[0]=='1')
    		 {
    			 fbuf[0]=1;
    		 }
    		 else
    		 {
    			 fbuf[0]=0;
    		 }
    	 	YS_PrmWriteOneItem(FLH_PRM_WAKEUP_EN,FLH_PRM_WAKEUP_EN_LEN,fbuf);
    		return(TRUE);
    	 }
     }
     else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_WAKEUPT,StrDat))//�Զ�����ʱ������
     {
    	 if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
    	 {
    		return(FALSE);
    	 }
    	 else
    	 {
    		 Value=atoi(StrDat);
    		 Value*=60;  //ת��Ϊ��
    		 fbuf[0]=Value/256;
    		 fbuf[1]=Value%256;
    		 YS_PrmWriteOneItem(FLH_PRM_WAKEUP_TIME,FLH_PRM_WAKEUP_TIME_LEN,fbuf);
    		 return(TRUE);
    	 }
     }*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_APN,StrDat))//����APN ��Ϣ
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_PPP_APN_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_PPP_APN_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SERVIP,StrDat))//���÷�����IP
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_SERV_ADDR_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_SERV_ADDR_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SERVPORT,StrDat))//���÷������˿�
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_EQUTYPE,StrDat))//���÷������˿�
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_TERI_TYPE,FLH_PRM_TERI_TYPE_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_IMEI,StrDat))//����IMEI
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_TERI_IMEI_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_TERI_IMEI_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_TERI_IMEI,FLH_PRM_TERI_IMEI_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    /* else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SPEED,StrDat))//���ó��ٱ�������
    {
    	 Value=atoi(StrDat);
    	 fbuf[0]=Value/256;
    	 fbuf[1]=Value%256;
    	 //YS_PrmWriteOneItem(FLH_PRM_SPEED_LIMIT,FLH_PRM_SPEED_LIMIT_LEN,fbuf);
    	 return(TRUE);
    } */
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIB_BASE,StrDat))//�����񶯱�������ֵ
    {
        Value=atoi(StrDat);
        fbuf[0]=Value;
        YS_PrmWriteOneItem(FLH_PRM_VIB_BASEVALUE,FLH_PRM_VIB_BASEVALUE_LEN,fbuf);
        return(TRUE);
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_FLIT,StrDat))//�����������ʱ��
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_FLIT_SET,FLH_PRM_FLIT_SET_LEN,fbuf);
            return(TRUE);
        }
    }	 */
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIBT,StrDat))//�����񶯱�����ʱ
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,180)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value;
            YS_PrmWriteOneItem(FLH_PRM_VIBWARN_DELAY,FLH_PRM_VIBWARN_DELAY_LEN,fbuf);
            return(TRUE);
        }
    }		*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_ANGLE_SEND,StrDat))//���ùյ㲹��ʹ��
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')
            {
                fbuf[0]=1;
            }
            else
            {
                fbuf[0]=0;
            }
            YS_PrmWriteOneItem(FLH_PRM_ANGLE_SEND_FLAG,FLH_PRM_ANGLE_SEND_FLAG_LEN,fbuf);
            return(TRUE);
        }
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_WARN_TIMES,StrDat))//���ùյ㲹��ʹ��
    {
    	if(YS_PrmPDNumString(StrDat,FALSE,1,0,100)==FALSE)
    	{
    	    	return(FALSE);
    	}
    	else
    	{
    	    	fbuf[0]=atoi(StrDat);
    	   	YS_PrmWriteOneItem(FLH_PRM_WARN_TIMES_MAX,FLH_PRM_WARN_TIMES_MAX_LEN,fbuf);
    	    	return(TRUE);
    	}
    }	*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_GPS_FINTER_EN,StrDat)) //GPSƯ���Ż�ʹ��
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')//ʹ��
            {
                fbuf[0]=1;
                YS_PrmWriteOneItem(FLH_PRM_NV_GPS_PYYH,FLH_PRM_NV_GPS_PYYH_LEN,fbuf);
            }
            else              //ʧ��
            {
                fbuf[0]=0;
                YS_PrmWriteOneItem(FLH_PRM_NV_GPS_PYYH,FLH_PRM_NV_GPS_PYYH_LEN,fbuf);
            }
            return(TRUE);
        }
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_VIBSMS_EN,StrDat)) //�񶯶��ű���ʹ������
    {
    	if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	{
    		return(FALSE);
    	}
    	else
    	{
    		if(StrDat[0]=='1')
    		{
    			fbuf[0]=1;
    		}
    		else
    		{
    			fbuf[0]=0;
    		}
    		//YS_PrmWriteOneItem(FLH_PRM_VIBWARN_SMS,FLH_PRM_VIBWARN_SMS_LEN,fbuf);
    		return(TRUE);
    	}
    }	*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_POFSMS_EN,StrDat)) //�ϵ�/��ѹ��������ʹ������
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            if(StrDat[0]=='1')
            {
                fbuf[0]=1;
            }
            else
            {
                fbuf[0]=0;
            }
            YS_PrmWriteOneItem(FLH_PRM_POWOFFWARN_SMS,FLH_PRM_POWOFFWARN_SMS_LEN,fbuf);
            return(TRUE);
        }
    }
    /*else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_OUTSMS_EN,StrDat)) //Խ�籨��SMSʹ������
    {
    	if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
    	{
    		return(FALSE);
    	}
    	else
    	{
    		if(StrDat[0]=='1')
    		{
    			fbuf[0]=1;
    		}
    		else
    		{
    			fbuf[0]=0;
    		}
    		YS_PrmWriteOneItem(FLH_PRM_OUT_SMS,FLH_PRM_OUT_SMS_LEN,fbuf);
    		return(TRUE);
    	}
    }	*/
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_AGPS_EN,StrDat)) //GPSƯ���Ż�ʹ��
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,2)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            fbuf[0]=atoi(StrDat);
            YS_PrmWriteOneItem(FLH_PRM_NV_GPS_AGPS,FLH_PRM_NV_GPS_AGPS_LEN,fbuf);
            return(TRUE);
        }
    }

    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_DW_ADDR,StrDat))//���÷�����IP
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_NV_DWADDR_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_NV_DWADDR_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SJIP,StrDat))//���÷�����IP
    {
        len=strlen(StrDat);
        if((len==0)||(len>FLH_PRM_NV_DWADDR_LEN))
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fbuf[i]=StrDat[i];
            }

            if(len<FLH_PRM_NV_DWADDR_LEN) fbuf[len]=0;
            YS_PrmWriteOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_DW_PORT,StrDat))//���÷������˿�
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_DLPORT,StrDat))//���÷������˿�
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value/256;
            fbuf[1]=Value%256;
            YS_PrmWriteOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_DW_FLAG,StrDat)) //Զ����������
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            fbuf[0]=atoi(StrDat);
            YS_PrmWriteOneItem(FLH_PRM_NV_DWFLAG,FLH_PRM_NV_DWFLAG_LEN,fbuf);
            YS_PrmQuickWriteNV();
            if(fbuf[0]==1)  //���������ʼ����
            {
                YS_SysRsqSystemRST(YS_RST_FLAG_USER);	 //��λϵͳ
            }
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_PWM,StrDat)) 	//���ò�������
    {
        if(YS_SmsCompString((u8 *)StrDat,strlen(StrDat),6)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            for(i=0; i<6; i++)
            {
                fbuf[i]=StrDat[i];
            }
            YS_PrmWriteOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,fbuf);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_pARAM_NAME_AUTO_ENABLE,StrDat)) //�����Զ�����ʹ��
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            fbuf[0]=atoi(StrDat);
            YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_ENABLE,FLH_PRM_NV_AUTOPLAT_ENABLE_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_UPDATE_MODE,StrDat)) //���ù���ģʽ
    {
        if(YS_PrmPDNumString(StrDat,TRUE,1,0,1)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            fbuf[0]=atoi(StrDat);
            YS_PrmWriteOneItem(FLH_PRM_UPDATE_MODE,FLH_PRM_UPDATE_MODE_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SYNC_DT,StrDat)) //����ϵͳʱ��
    {
        if(strlen(StrDat)!=19)
        {
            return(FALSE);
        }
        else
        {
            YS_SysSyncServerDatetime(StrDat);
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_ALARM_CLOCK,StrDat))	//����ϵͳ����
    {
        if(YS_PrmSetAlarmClock(StrDat)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            return(TRUE);
        }
    }
    else if(YS_PrmFindStringInbuf(dbuf,dlen,HBYD_PARAM_NAME_SKYLINE_FLAG,StrDat))//����ϵͳ�Ƿ�Ϊ���ߵ���ģʽ
    {
        if(YS_PrmPDNumString(StrDat,FALSE,1,0,0xFFFF)==FALSE)
        {
            return(FALSE);
        }
        else
        {
            Value=atoi(StrDat);
            fbuf[0]=Value;
            YS_PrmWriteOneItem(FLH_PRM_SKYLINE_FLAG,FLH_PRM_SKYLINE_FLAG_LEN,fbuf);
            YS_PrmQuickWriteNV();
            return(TRUE);
        }
    }
    else
    {
        return(FALSE);
    }
    return(FALSE);
}

/*-----------------------------------------------------------------------------------------
��������YS_PamWriteNVDeal
����˵���������������п���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_PrmWriteNVDeal(void)
{
    //S16 error;

    if(t_ParamManage.WriteFlag==1)
    {
        t_ParamManage.WriteDelay++;
        YS_GprsDebugString("NV Write count",t_ParamManage.WriteDelay);
        if(t_ParamManage.WriteDelay>=10)
        {
            t_ParamManage.WriteFlag=0;
            YS_GprsDebugString("NV Write Opt Flag",0);
            sjfun_WriteFilesToNV();
//   			WriteRecord(NVRAM_EF_GPSTERM1_LID,1,YS_Param_Buf,512,&error);
//			WriteRecord(NVRAM_EF_YSPRM_SETTING_LID,1,YS_Param_Buf,16,&error);
        }
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_SetPowLowFlag
����˵�������õ͵�ѹ����ʹ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_SetPowLowFlag(void)
{
    u8 fbuf[4];

    YS_PrmReadOneItem(FLH_PRM_NV_POWLOW_FLAG,FLH_PRM_NV_POWLOW_FLAG_LEN,fbuf);
    if(fbuf[0]==0)
    {
        YS_UartDebugString("set powlow flag");
        fbuf[0]=1;
        YS_PrmWriteOneItem(FLH_PRM_NV_POWLOW_FLAG, FLH_PRM_NV_POWLOW_FLAG_LEN, fbuf);
    }

}

/*-----------------------------------------------------------------------------------------
��������YS_CleanPowLowFlag
����˵��������͵�ѹ����ʹ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_CleanPowLowFlag(void)
{
    u8 fbuf[4];

    YS_PrmReadOneItem(FLH_PRM_NV_POWLOW_FLAG,FLH_PRM_NV_POWLOW_FLAG_LEN,fbuf);
    if(fbuf[0]==1)
    {
        YS_UartDebugString("clean powlow flag");
        fbuf[0]=0;
        YS_PrmWriteOneItem(FLH_PRM_NV_POWLOW_FLAG, FLH_PRM_NV_POWLOW_FLAG_LEN, fbuf);
    }
}

/*-----------------------------------------------------------------------------------------
��������YS_PrmFSFilesISExist
����˵�����ж�һ���ļ��Ƿ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
U8 YS_PrmFSFilesISExist(const kal_uint16* file_name)
{
	int res = sjfun_File_Open(file_name, FS_READ_ONLY);
	sjfun_File_Close(res);
	if (res < 0)
	{
		return 0;
	}
	return 1;
}

