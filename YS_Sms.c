/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Sms.c  2014-09-15
此文件用于管理与短信相关的接口
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

t_Sms_Pos_Rsq			t_SmsPosRsq;
t_Sms_Active_Rsq			t_SmsActiveRsq;
t_Sms_Mul_Send_Manage	t_SmsMulManage;
t_Sms_Com_Moni			t_SmsComMoni;

//#define MAX_UNICODETABLE_LEN  sizeof(YS_UnicodeTable)

#define SMS_CMD_HEAD_CHECK		"CHECK"
#define SMS_CMD_HEAD_PW			"PW"
#define SMS_CMD_HEAD_CENTER		"CENTER"
#define SMS_CMD_HEAD_IP			"IP"
#define SMS_CMD_HEAD_PORT			"PORT"
#define SMS_CMD_HEAD_SURL			"SURL"
#define SMS_CMD_HEAD_FACTORY		"FACTORY"
#define SMS_CMD_HEAD_RESET		"RESET"
#define SMS_CMD_HEAD_UPLOAD		"UPLOAD"
#define SMS_CMD_HEAD_SLEEP		"SLEEP"
#define SMS_CMD_HEAD_URL			"URL"
#define SMS_CMD_HEAD_URL1			"URL1"
#define SMS_CMD_HEAD_SMS			"SMS"
#define SMS_CMD_HEAD_ANY			"ANY"
#define SMS_CMD_HEAD_FLIGHT		"FLIGHT"
#define SMS_CMD_HEAD_APN			"APN"
#define SMS_CMD_HEAD_JCSJ			"JCSJ"
#define SMS_CMD_HEAD_DTMODE		"DTMODE"
#define SMS_CMD_HEAD_DKEY			"DKEY"
#define SMS_CMD_HEAD_SKEY			"SKEY"
#define SMS_CMD_HEAD_MOTION		"MOTION"
#define SMS_CMD_HEAD_DOME		"DOME"
#define SMS_CMD_HEAD_AF			"AF"
#define SMS_CMD_HEAD_JK			"JK"
#define SMS_CMD_HEAD_DW			"DW"
#define SMS_CMD_HEAD_CC			"CC"

#define SMS_CMD_HEAD_USER			"USER"
#define SMS_CMD_HEAD_PD			"PD"
#define SMS_CMD_HEAD_PLMN			"PLMN"

#define SMS_CMD_HEAD_CURTIME		"CURTIME"
#define SMS_CMD_HEAD_UTC			"UTC"

#define SMS_CMD_HEAD_IPPORT		"#654321#IP#"
#define SMS_CMD_HEAD_ID 	    "#654321#ID#"
#define SMS_CMD_HEAD_PLATE      "#PTE#"
enum
{
    SMS_ACK_FLAG_ERRCMD,
    SMS_ACK_FLAG_CQ,
    SMS_ACK_FLAG_INIT,
    SMS_ACK_FLAG_OWN1,
    SMS_ACK_FLAG_OWN2,
    SMS_ACK_FLAG_SIM,
    SMS_ACK_FLAG_SIM_OWN,
    SMS_ACK_FLAG_PWOFF1,
    SMS_ACK_FLAG_PWOFF2,
    SMS_ACK_FLAG_MMSET,
    SMS_ACK_FLAG_VIBHIT1,
    SMS_ACK_FLAG_VIBHIT2,
    SMS_ACK_FLAG_VIBHIT3,
    SMS_ACK_FLAG_CALLHIT1,
    SMS_ACK_FLAG_CALLHIT2,
    SMS_ACK_FLAG_RADIOHIT1,
    SMS_ACK_FLAG_RADIOHIT2,
    SMS_ACK_FLAG_GPSOFF,
    SMS_ACK_FLAG_GPSON1,
    SMS_ACK_GOOGLE_LINK,
    SMS_ACK_GODE_LINK,
    SMS_ACK_FLAG_UPDATE_TIME,
    SMS_ACK_FLAG_HEART_TIME,
    SMS_ACK_FLAG_SERVADDR,
    SMS_ACK_FLAG_SERVSURL,
    SMS_ACK_FLAG_VIBCALL1,
    SMS_ACK_FLAG_VIBCALL0,
    SMS_ACK_FLAG_POWOFFCALL1,
    SMS_ACK_FLAG_POWOFFCALL0,
    SMS_ACK_FLAG_OUTCALL1,
    SMS_ACK_FLAG_OUTCALL0,
    SMS_ACK_FLAG_DDSET,
    SMS_ACK_FLAG_HFYDSET,
    SMS_ACK_FLAG_SLEEPTIME_SET,
    SMS_ACK_FLAG_SFTIME_SET,
    SMS_ACK_FLAG_TCTIME_SET,
    SMS_ACK_FLAG_RFKEYCHECK,
    SMS_ACK_FLAG_SMSWARM1,
    SMS_ACK_FLAG_SMSWARM2,
    SMS_ACK_FLAG_VIBCHECKTIME,
    SMS_ACK_FLAG_ANYMUNSET1,
    SMS_ACK_FLAG_ANYMUNSET0,
    SMS_ACK_FLAG_FLISET0,
    SMS_ACK_FLAG_FLISET1,
    SMS_ACK_FLAG_SYNCSET1,
    SMS_ACK_FLAG_SYNCSET0,
    SMS_ACK_FLAG_JCSJSET,
    SMS_ACK_FLAG_JK,
    SMS_ACK_FLAG_AF,
    SMS_ACK_FLAG_DEMO_AF,
    SMS_ACK_FLAG_DEMO_JK,
    SMS_ACK_FLAG_DELKEY,
    SMS_ACK_FLAG_SETKEY1,
    SMS_ACK_FLAG_SETKEY2,
    SMS_ACK_APN_SET,
    SMS_ACK_FLAG_PLATE,
    SMS_ACK_FLAG_DW
};


u8 	Sms_Con_cq_Ack[]= {0xD6, 0xD8, 0xC6, 0xF4, 0xD6, 0xD5, 0xB6, 0xCB, 0x21};
//重启终端!

u8 SmsCon_Def_SetPrm_OK[]= {0xC9,0xE8,0xD6,0xC3,0xB3,0xC9,0xB9,0xA6,0x21};

u8 SmsCon_Def_SetPrm_FAIL[]= {0xC9,0xE8,0xD6,0xC3,0xCA,0xA7,0xB0,0xDC,0x21};

u8 SmsCon_Def_ReadPrm_OK[]= {0xB2,0xE9,0xD1,0xAF,0xB3,0xC9,0xB9,0xA6,0xA3,0xBA}; 	 	//查询成功：

u8 SmsCon_Def_ReadPrm_FAIL[]= {0xB2,0xE9,0xD1,0xAF,0xCA,0xA7,0xB0,0xDC,0xA3,0xBA}; 	//查询失败

u8	Sms_Con_Init_Ack[]= {0xBB, 0xD6, 0xB8, 0xB4, 0xB3, 0xF6, 0xB3, 0xA7, 0xC9, 0xE8, 0xD6, 0xC3,
                         0xB3, 0xC9, 0xB9, 0xA6, 0xA3, 0xAC, 0xD2, 0xBB, 0xB7, 0xD6, 0xD6, 0xD3, 0xBA, 0xF3, 0xD6, 0xD8,
                         0xC6, 0xF4, 0xD6, 0xD5, 0xB6, 0xCB, 0xA3, 0xA1
                       }; //恢复出厂设置成功，一分钟后重启终端！

u8	Sms_Con_Ower_Clear[]= {0xD6, 0xD0, 0xD0, 0xC4, 0xBA, 0xC5, 0xC2, 0xEB, 0xD2, 0xD1, 0xC9, 0xBE,
                           0xB3, 0xFD, 0x21
                         }; //中心号码已删除!

u8 	Sms_Con_SmsWarn1[]= {0xB6, 0xCC, 0xD0, 0xC5, 0xB1, 0xA8, 0xBE, 0xAF, 0xB9, 0xD8, 0xB1,
                         0xD5, 0x21
                        };//短信报警关闭!

u8 	Sms_Con_SmsWarn2[]= {0xB6, 0xCC, 0xD0, 0xC5, 0xB1, 0xA8, 0xBE, 0xAF, 0xB4, 0xF2, 0xBF,
                         0xAA, 0x21
                        };//短信报警打开!

u8	Sms_Con_Ower_Set[]= {0xD6, 0xD0, 0xD0, 0xC4, 0xBA, 0xC5, 0xC2, 0xEB, 0xC9, 0xE8, 0xD6, 0xC3,
                         0xB3, 0xC9, 0xB9, 0xA6, 0x21
                       }; // 中心号码设置成功!

u8	Sms_Con_mm_Set[]= {0xC3, 0xDC, 0xC2, 0xEB, 0xC9, 0xE8, 0xD6, 0xC3, 0xB3, 0xC9, 0xB9, 0xA6,
                       0x21, 0xC3, 0xDC, 0xC2, 0xEB, 0xCE, 0xAA, 0x3A
                     };//密码设置成功!密码为:

u8 	Sms_Con_SleepTime_Set[]= {0xD0, 0xDD, 0xC3, 0xDF, 0xCA, 0xB1, 0xBC, 0xE4, 0xC9, 0xE8, 0xD6,
                              0xC3, 0xB3, 0xC9, 0xB9, 0xA6, 0xA3, 0xA1, 0xCA, 0xB1, 0xBC, 0xE4, 0xCE, 0xAA, 0xA3, 0xBA
                             };
//休眠时间设置成功！时间为：

u8 	Sms_Con_SYS_Set0[]= {0x47, 0x50, 0x53, 0xB6, 0xD4, 0xCA, 0xB1, 0xC9, 0xE8, 0xD6, 0xC3, 0xB3,
                         0xC9, 0xB9, 0xA6, 0x21
                        };
//GPS对时设置成功!

u8 	Sms_Con_SYS_Set1[]= {0xC6, 0xBD, 0xCC, 0xA8, 0xB6, 0xD4, 0xCA, 0xB1, 0xC9, 0xE8, 0xD6, 0xC3,
                         0xB3, 0xC9, 0xB9, 0xA6, 0x21
                        };
//平台对时设置成功!

u8 	Sms_Con_VibTime_Set[]= {0xD5, 0xF0, 0xB6, 0xAF, 0xCA, 0xB1, 0xBC, 0xE4, 0xC9, 0xE8, 0xD6,
                            0xC3, 0xB3, 0xC9, 0xB9, 0xA6, 0x21, 0xCA, 0xB1, 0xBC, 0xE4, 0xCE, 0xAA, 0x3A
                           };
//震动时间设置成功!时间为:

u8 	Sms_Con_FLI_Set1[]= {0xD4, 0xCA, 0xD0, 0xED, 0xD6, 0xD5, 0xB6, 0xCB, 0xB9, 0xD8, 0xB1, 0xD5,
                         0x47, 0x53, 0x4D, 0xC4, 0xA3, 0xBF, 0xE9, 0xA1, 0xA3
                        };
//允许终端关闭GSM模块。

u8 	Sms_Con_FLI_Set0[]= {0xBD, 0xFB, 0xD6, 0xB9, 0xD6, 0xD5, 0xB6, 0xCB, 0xB9, 0xD8, 0xB1, 0xD5,
                         0x47, 0x53, 0x4D, 0xC4, 0xA3, 0xBF, 0xE9, 0xA1, 0xA3
                        };
//禁止终端关闭GSM模块。

u8	Sms_Con_Err_Cmd[]= {0xB6, 0xCC, 0xD0, 0xC5, 0xD6, 0xB8, 0xC1, 0xEE, 0xD3, 0xD0, 0xCE, 0xF3,
                        0xA3, 0xAC, 0xC7, 0xEB, 0xD6, 0xD8, 0xD0, 0xC2, 0xB7, 0xA2, 0xCB, 0xCD, 0xA3,
                        0xA1
                      }; //短信指令有误，请重新发送！

u8 SmsCon_Def_Reset[]= {0xD6,0xD8,0xC6,0xF4,0xB3,0xC9,0xB9,0xA6,0xA1,0xA3};

u8 SmsCon_Def_SetPrm_PWErr[]= {0xC4,0xFA,0xCA,0xE4,0xC8,0xEB,0xB5,0xC4,0xC3,0xDC,0xC2,0xEB,0xD3,
                               0xD0,0xCE,0xF3,0xA3,0xA1
                              }; //您输入的密码有误！

u8	Sms_Con_Minute[]= {0xB7, 0xD6, 0xD6, 0xD3}; //分钟

u8	Sms_Con_Second[]= {0xC3, 0xEB}; //秒

u8    Sms_Con_Update_Time[]= {0xC9, 0xCF, 0xB4, 0xAB, 0xBC, 0xE4, 0xB8, 0xF4, 0xC9, 0xE8, 0xD6,
                              0xC3, 0xB3, 0xC9, 0xB9, 0xA6, 0xA3, 0xA1, 0xBC, 0xE4, 0xB8, 0xF4, 0xCA, 0xB1, 0xBC, 0xE4, 0xCE,
                              0xAA, 0xA3, 0xBA
                             }; //上传间隔设置成功！间隔时间为：

u8	Sms_Con_Serv_Addr[]= {0x49, 0x50, 0xBA, 0xCD, 0xB6, 0xCB, 0xBF, 0xDA, 0xC9, 0xE8, 0xD6, 0xC3,
                          0xB3, 0xC9, 0xB9, 0xA6, 0x21
                        }; //IP和端口设置成功!

u8 Sms_Con_Sim_Own[]= {0x49, 0x44, 0xC9, 0xE8, 0xD6, 0xC3, 0xB3, 0xC9, 0xB9, 0xA6, 0x21}; //ID设置成功!

//您的终端【
u8 SmsCon_Warn_Batt_Low[]= {0xC4, 0xFA, 0xB5, 0xC4, 0xD6, 0xD5, 0xB6, 0xCB, 0xA1, 0xBE};

//】电量不足，请及时充电!剩余电量:
u8 SmsCon_Warn_Batt_Low1[]= {0xA1, 0xBF, 0xB5, 0xE7, 0xC1, 0xBF, 0xB2, 0xBB, 0xD7, 0xE3, 0xA3, 0xAC, 0xC7,
                             0xEB, 0xBC, 0xB0, 0xCA, 0xB1, 0xB3, 0xE4, 0xB5, 0xE7, 0x21, 0xCA, 0xA3, 0xD3, 0xE0, 0xB5, 0xE7, 0xC1, 0xBF, 0x3A
                            };

 //车牌号设置成功,
u8 SmsCon_Plate[]= {0xB3, 0xB5, 0xC5, 0xC6, 0xBA, 0xC5, 0xC9, 0xE8, 0xD6, 0xC3, 0xB3, 0xC9, 0xB9, 0xA6, 0x2C};
/*-----------------------------------------------------------------------------------------
函数名：YS_SmsMulSendAddOneRec
功能说明：添加一条缓冲信息
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsMulSendAddOneRec(u8 *pNumber, u8 pNumberLen, u8* pContent, u16 pContentLen,bool FormChangeFlag)
{
    u16 i;
    u8 p;

    if(t_SmsMulManage.RecNum<SMS_MUL_RECORD_NUM)
    {
        p=t_SmsMulManage.WritePoint;
        t_SmsMulManage.SmsRecBuf[p].PhoneLen=pNumberLen;
        t_SmsMulManage.SmsRecBuf[p].FormChangeFlag=FormChangeFlag;
        for(i=0; i<pNumberLen; i++)
        {
            t_SmsMulManage.SmsRecBuf[p].PhoneBuf[i]=pNumber[i];
        }
        t_SmsMulManage.SmsRecBuf[p].SmsConLen=pContentLen;
        //YS_UartDebugInterfacel(0x8110,pContent, pContentLen);
        for(i=0; i<pContentLen; i++)
        {
            t_SmsMulManage.SmsRecBuf[p].SmsConBuf[i]=pContent[i];
        }
        //YS_UartDebugInterfacel(0x8110,t_SmsMulManage.SmsRecBuf[p].SmsConBuf, pContentLen);
        t_SmsMulManage.WritePoint++;
        if(t_SmsMulManage.WritePoint>=SMS_MUL_RECORD_NUM)
        {
            t_SmsMulManage.WritePoint=0;
        }
        t_SmsMulManage.RecNum++;
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsSendOneSetFlag
功能说明：发送一条短信后，设置相关标志
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsSendOneSetFlag(void)
{
    t_SmsMulManage.Enable=0;
    t_SmsMulManage.AckFlag=0;
    t_SmsMulManage.SendDelay=0;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsSendEnableQuery
功能说明：查询当前是否允许发送信息
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsSendEnableQuery(void)
{
    if(t_SmsMulManage.Enable==1)
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsMulSendInit
功能说明：多信息发送初始化
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsMulSendInit(void)
{
    t_SmsMulManage.RecNum=0;
    t_SmsMulManage.ReadPoint=0;
    t_SmsMulManage.WritePoint=0;
    t_SmsMulManage.Enable=0;
    t_SmsMulManage.AckFlag=0;
    t_SmsMulManage.SendDelay=0;
    t_SmsMulManage.RstFlag = FALSE;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsMulSendManage
功能说明：多条信息发送管理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsMulSendManage(void)
{
    t_Sys_Run_Status	t_GetStatus;
    u8 p;

    if(t_SmsMulManage.Enable==0)  //如果当前禁止发送新短信
    {
        YS_RunGetSystemStatus(&t_GetStatus);
        t_SmsMulManage.SendDelay++;
        if(t_SmsMulManage.SendDelay>SMS_DEFAULT_SEND_DELAY)
        {
            t_SmsMulManage.SendDelay=SMS_DEFAULT_SEND_DELAY;
        }

        if((t_GetStatus.RegStatus==1)&&((t_SmsMulManage.SendDelay>=SMS_DEFAULT_SEND_DELAY)||(t_SmsMulManage.AckFlag==1)))
        {
            //网络已注册，且发送间隔时间到或收到发送回执
            t_SmsMulManage.Enable=1;
        }
    }
    else
    {
        if(t_SmsMulManage.RecNum>0)  //从缓冲区中读取一条记录进行发送
        {
            p=t_SmsMulManage.ReadPoint;
            YS_RunSmsDataInc(DATA_FLAG_SEND_ADD);
            YS_UartDebugInterfacel(INTER_PTL_UPDATE_SMSSEND,t_SmsMulManage.SmsRecBuf[p].PhoneBuf,t_SmsMulManage.SmsRecBuf[p].PhoneLen);
            YS_UartDebugInterfacel(INTER_PTL_UPDATE_SMSSEND,t_SmsMulManage.SmsRecBuf[p].SmsConBuf,t_SmsMulManage.SmsRecBuf[p].SmsConLen);
            sjfun_Sms_Send_Interface(t_SmsMulManage.SmsRecBuf[p].PhoneBuf,t_SmsMulManage.SmsRecBuf[p].PhoneLen,t_SmsMulManage.SmsRecBuf[p].SmsConBuf,t_SmsMulManage.SmsRecBuf[p].SmsConLen,t_SmsMulManage.SmsRecBuf[p].FormChangeFlag);
            t_SmsMulManage.ReadPoint++;
            if(t_SmsMulManage.ReadPoint>=SMS_MUL_RECORD_NUM)
            {
                t_SmsMulManage.ReadPoint=0;
            }
            t_SmsMulManage.RecNum--;
        }
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsSendRltAck
功能说明：短信发送结果回调函数
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsSendRltAck(u8 ID)
{
    if(ID==1)
    {
        YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)"Sms Send OK!",strlen("Sms Send OK!"));
    }
    else
    {
        YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)"Sms Send Fail!",strlen("Sms Send Fail!"));
    }
    t_SmsMulManage.AckFlag=1;
    if(t_SmsMulManage.RstFlag == TRUE)
    {
        t_SmsMulManage.RstFlag = FALSE;
        YS_SysRsqSystemRST(YS_RST_FLAG_ACTIVE);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsSendInterface
功能说明：短信发送接口
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsSendInterface(u8* pContent, u16 pContentLen, u8 * pNumber, u8 pNumberLen,bool FormChangeFlag)
{
    YS_SmsMulSendAddOneRec(pNumber,pNumberLen,pContent,pContentLen,FormChangeFlag);
}

/*-------------------------------------------------------------------------------------------
函数名：YS_SmsSendTestInterface
功能说明：短信发送测试接口
修改记录：
--------------------------------------------------------------------------------------------*/
void YS_SmsSendTestInterface(u8 *dbuf, u16 dlen)
{
    u8 i,PhoneLen,ConLen;
    u8 ContBuf[100],NumBuf[20];

    PhoneLen=dbuf[0];
    ConLen=dbuf[PhoneLen+1];
    for(i=0; i<ConLen; i++)
    {
        ContBuf[i]=dbuf[PhoneLen+2+i];
    }

    ContBuf[ConLen]=0;
    for(i=0; i<PhoneLen; i++)
    {
        NumBuf[i]=dbuf[i+1];
    }
    NumBuf[PhoneLen]=0;
    YS_SmsSendInterface(ContBuf,ConLen,NumBuf,PhoneLen,TRUE);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsZCRltDeal
功能说明：注册短信处理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsAckSmsDeal(u8 RltID, u8 *PhoneCode, u8 PhoneLen)
{
    u8 	PackBuf[200];
    u16 	PackLen,i,pos,len,Value;
    u8 	fbuf[40];
    char StrDat[10];
    t_Gps_Data_Info	t_GetGps;

    PackLen=0;
    switch(RltID)
    {
        case SETPRM_SMS_ACK_PWERR:
            PackLen=sizeof(SmsCon_Def_SetPrm_PWErr);
            memcpy(&PackBuf,SmsCon_Def_SetPrm_PWErr,PackLen);
            break;
        case RST_SMS_ACK: //复位终端
            PackLen=sizeof(SmsCon_Def_Reset);
            memcpy(&PackBuf,SmsCon_Def_Reset,PackLen);
            break;
        case SMS_ACK_FLAG_ERRCMD:
            PackLen=sizeof(Sms_Con_Err_Cmd);
            memcpy(PackBuf,Sms_Con_Err_Cmd,PackLen);
            break;

        case SMS_ACK_FLAG_CQ:
            PackLen=sizeof(Sms_Con_cq_Ack);
            memcpy(PackBuf,Sms_Con_cq_Ack,PackLen);
            break;

        case SMS_ACK_FLAG_INIT:
            PackLen=sizeof(Sms_Con_Init_Ack);
            memcpy(PackBuf,Sms_Con_Init_Ack,PackLen);
            t_SmsMulManage.RstFlag=TRUE;
            break;

        case SMS_ACK_FLAG_OWN1:
            PackLen=sizeof(Sms_Con_Ower_Clear);
            memcpy(PackBuf,Sms_Con_Ower_Clear,PackLen);
            break;

        case SMS_ACK_FLAG_SMSWARM1:    //报警短信关闭
            PackLen=sizeof(Sms_Con_SmsWarn1);
            memcpy(PackBuf,Sms_Con_SmsWarn1,PackLen);
            break;

        case SMS_ACK_FLAG_SMSWARM2:
            PackLen=sizeof(Sms_Con_SmsWarn2);
            memcpy(PackBuf,Sms_Con_SmsWarn2,PackLen);
            break;

        case SMS_ACK_FLAG_OWN2:
            PackLen=sizeof(Sms_Con_Ower_Set);
            memcpy(PackBuf,Sms_Con_Ower_Set,PackLen);
            break;

        case SMS_ACK_FLAG_VIBCHECKTIME:
            PackLen=sizeof(Sms_Con_VibTime_Set);
            memcpy(PackBuf,Sms_Con_VibTime_Set,PackLen);

            YS_PrmReadOneItem(FLH_PRM_VIB_CHK,FLH_PRM_VIB_CHK_LEN,fbuf);
            sprintf(StrDat,"%d",fbuf[0]);
            len=strlen(StrDat);
            for(i=0; i<len; i++)
            {
                PackBuf[PackLen]=StrDat[i];
                PackLen++;
            }

            PackBuf[PackLen]=0xC3;
            PackLen++;
            PackBuf[PackLen]=0xEB;
            PackLen++;
            break;

        case SMS_ACK_FLAG_FLISET0:
            PackLen=sizeof(Sms_Con_FLI_Set0);
            memcpy(PackBuf,Sms_Con_FLI_Set0,PackLen);
            break;

        case SMS_ACK_FLAG_FLISET1:
            PackLen=sizeof(Sms_Con_FLI_Set1);
            memcpy(PackBuf,Sms_Con_FLI_Set1,PackLen);
            break;

        case SMS_ACK_FLAG_MMSET: //密码置成功，新密码为
            PackLen=sizeof(Sms_Con_mm_Set);
            memcpy(PackBuf,Sms_Con_mm_Set,PackLen);
            YS_PrmReadOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,fbuf);
            len=YS_CodeBufRealLen(fbuf,FLH_PRM_PASSWORD_LEN);
            for(i=0; i<len; i++)
            {
                PackBuf[PackLen]=fbuf[i];
                PackLen++;
            }
            PackBuf[PackLen]=0xA3;
            PackLen++;
            PackBuf[PackLen]=0xA1;
            PackLen++;
            break;

        case SMS_ACK_FLAG_SLEEPTIME_SET:
            PackLen=sizeof(Sms_Con_SleepTime_Set);
            memcpy(PackBuf,Sms_Con_SleepTime_Set,PackLen);

            YS_PrmReadOneItem(FLH_PRM_SLEEP_TIME, FLH_PRM_SLEEP_TIME_LEN, fbuf);
            Value=fbuf[0]*256+fbuf[1];
            Value/=60;	//转换为分钟
            sprintf(StrDat,"%d",Value);
            len=strlen(StrDat);
            for(i=0; i<len; i++)
            {
                PackBuf[PackLen]=StrDat[i];
                PackLen++;
            }
            len=sizeof(Sms_Con_Minute);
            memcpy(&PackBuf[PackLen],Sms_Con_Minute,len);
            PackLen+=len;
            break;

        case SMS_ACK_FLAG_SYNCSET1:
            PackLen=sizeof(Sms_Con_SYS_Set1);
            memcpy(PackBuf,Sms_Con_SYS_Set1,PackLen);
            break;
        case SMS_ACK_FLAG_SYNCSET0:
            PackLen=sizeof(Sms_Con_SYS_Set0);
            memcpy(PackBuf,Sms_Con_SYS_Set0,PackLen);
            break;

        case SMS_ACK_GOOGLE_LINK: //回复google 链接
            YS_GpsGetPosData(&t_GetGps);
            PackLen=YS_CodeCreateGoogleLink(PackBuf,t_GetGps.Longitude,t_GetGps.Latitude,1);
            //PackLen=YS_CodeCreateGoogleLink(PackBuf,t_GetGps.Longitude,t_GetGps.Latitude);
            break;
        case SMS_ACK_GODE_LINK: //回复google 链接
            YS_GpsGetPosData(&t_GetGps);
            PackLen=YS_CodeCreateGodeLink(PackBuf,t_GetGps.Longitude,t_GetGps.Latitude,1);
            //PackLen=YS_CodeCreateGoogleLink(PackBuf,t_GetGps.Longitude,t_GetGps.Latitude);
            break;

        case SMS_ACK_FLAG_UPDATE_TIME: // 设置上报时间间隔应答
            PackLen=sizeof(Sms_Con_Update_Time);
            memcpy(PackBuf,Sms_Con_Update_Time,PackLen);

            YS_PrmReadOneItem(FLH_PRM_UPDATE_TIME,FLH_PRM_UPDATE_TIME_LEN,fbuf);
            sprintf(StrDat,"%d",fbuf[0]*256+fbuf[1]);
            len=strlen(StrDat);
            for(i=0; i<len; i++)
            {
                PackBuf[PackLen]=StrDat[i];
                PackLen++;
            }
            len=sizeof(Sms_Con_Second);
            for(i=0; i<len; i++)
            {
                PackBuf[PackLen]=Sms_Con_Second[i];
                PackLen++;
            }
            break;

        case SMS_ACK_FLAG_SERVADDR: //设置平台地址应答
            YS_UartDebugString("pack ip ack");
            memset(PackBuf, 0, 200);
            PackLen=sizeof(Sms_Con_Serv_Addr);
            memcpy(PackBuf,Sms_Con_Serv_Addr,PackLen);

            PackBuf[PackLen]='I';
            PackLen++;
            PackBuf[PackLen]='P';
            PackLen++;
            PackBuf[PackLen]=':';
            PackLen++;

            memset(fbuf, 0, 40);
            YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);//ip地址端口
            len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
            for(i=0; i<len; i++)
            {
                PackBuf[PackLen]=fbuf[i];
                PackLen++;
            }
            YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)PackBuf,PackLen);
            PackBuf[PackLen]=' ';
            PackLen++;
            YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)PackBuf,PackLen);

            PackBuf[PackLen]='P';
            PackLen++;
            PackBuf[PackLen]='O';
            PackLen++;
            PackBuf[PackLen]='R';
            PackLen++;
            PackBuf[PackLen]='T';
            PackLen++;
            PackBuf[PackLen]=':';
            PackLen++;

            YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
            sprintf(StrDat,"%d",fbuf[0]*256+fbuf[1]);
            len=strlen(StrDat);
            for(i=0; i<len; i++)
            {
                PackBuf[PackLen]=StrDat[i];
                PackLen++;
            }
            t_SmsMulManage.RstFlag = TRUE;
            //YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)PackBuf,PackLen);
            break;

        case SMS_ACK_FLAG_SIM:
            PackLen=sizeof(Sms_Con_Sim_Own);
            memcpy(PackBuf,Sms_Con_Sim_Own,PackLen);
            PackBuf[PackLen]='I';
            PackLen++;
            PackBuf[PackLen]='D';
            PackLen++;
            PackBuf[PackLen]=':';
            PackLen++;
            YS_RunAddIDInfo(fbuf);
            len=YS_CodeBufRealLen(fbuf,FLH_PRM_SIM_CODE_LEN);
            for(i=0; i<len; i++)
            {
                PackBuf[PackLen]=fbuf[i];
                PackLen++;
            }
            t_SmsMulManage.RstFlag = TRUE;
            break;

        case SMS_ACK_FLAG_PLATE:
            PackLen=sizeof(SmsCon_Plate);
            memcpy(PackBuf,SmsCon_Plate,PackLen);
            YS_PrmReadOneItem(FLH_JTB_PLATE_STRING,FLH_JTB_PLATE_STRING_LEN,fbuf);
            len=YS_CodeBufRealLen(fbuf, FLH_JTB_PLATE_STRING_LEN);
            for ( i = 0; i<len; i++ )
            {
                PackBuf[PackLen]=fbuf[i];
                PackLen++;
            }
            break;

        default:
            break;
    }

    if(PackLen>0)
    {
        pos=PackLen;
        //sjfun_Sms_Send_Interface(PackBuf, pos, PhoneCode, PhoneLen);
        YS_SmsSendInterface(PackBuf, pos, PhoneCode, PhoneLen,TRUE);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsCompHeadDeal
功能说明：比较数据头
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsCompHeadDeal(char *HeadStr, u8 *dbuf,u16 dlen)
{
    u16 i,len;
    u8 CompBuf[40];
    bool CompRlt;

    len=strlen((char*)HeadStr);
    if(dlen<len)
    {
        return(FALSE);
    }

    len=strlen(HeadStr);
    for(i=0; i<len; i++)
    {
        CompBuf[i]=toupper(dbuf[i]);
    }

    CompRlt=TRUE;
    for(i=0; i<len; i++)
    {
        if(CompBuf[i]!=HeadStr[i])
        {
            CompRlt=FALSE;
            break;
        }
    }
    return(CompRlt);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsWeiHuSmsDeal
功能说明：维护短信检测处理
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsWeiHuSmsDeal(u8 *dbuf, u16 dlen, u8 *PhoneData, u8 PhoneLen)
{
    char  SmsConBuf[400];
    u8  fbuf[400];
    char StrDat[30];
//    char*p = NULL;
    t_Sys_Run_Status	t_GetStatus;
    t_Gps_Data_Info	t_GetGps;
    bool CompRlt;
    u8 SleepStatus;
    u16 i,len,pos,dat16;
    u8 VibSms;
    u8 China_Pow1[]= {0xC9,0xCF,0xB5,0xE7};		//上电
    u8 China_Pow2[]= {0xB6,0xCF,0xB5,0xE7};		//断电
    u8 China_Acc1[]= {0x41,0x43,0x43,0xBF,0xAA};			//ACC开
    u8 China_Acc2[]= {0x41,0x43,0x43,0xB9,0xD8};			//ACC关

    CompRlt=FALSE;

    if((dlen==3)&&((dbuf[0]=='N')||(dbuf[0]=='n'))&&((dbuf[1]=='w')||(dbuf[1]=='W'))&&((dbuf[2]=='h')||(dbuf[2]=='H')))  //维护
    {
        CompRlt=TRUE;
        YS_GprsDebugString("rec sms for weihu English",0);
    }
    if(CompRlt==FALSE) //比较失败
    {
        return(FALSE);
    }
    YS_GpsGetPosData(&t_GetGps);
    YS_RunGetSystemStatus(&t_GetStatus);
    pos=0;

    len=strlen(EBIKE_SOFT_VER);             //软件版本
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=EBIKE_SOFT_VER[i];
        pos++;
    }
    SmsConBuf[pos]=',';
    pos++;
    YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);//ip地址端口
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=fbuf[i];
        pos++;
    }
    SmsConBuf[pos]=':';
    pos++;
    YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
    dat16=fbuf[0]*256+fbuf[1];
    sprintf(StrDat,"%d",dat16);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=StrDat[i];
        pos++;
    }
    SmsConBuf[pos]=',';
    pos++;
    YS_RunAddIDInfo(fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_SIM_CODE_LEN);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=fbuf[i];
        pos++;
    }
    SmsConBuf[pos]=',';
    pos++;

    SmsConBuf[pos]='G';		//平台连接状态
    pos++;
    SmsConBuf[pos]='P';
    pos++;
    SmsConBuf[pos]='R';
    pos++;
    SmsConBuf[pos]='S';
    pos++;
    SmsConBuf[pos]='=';
    pos++;
    if(t_GetStatus.RunFlow==YS_RUN_FLOW_IDLE_DEAL) //GPRS在线
    {
        SmsConBuf[pos]=0x31;
        pos++;
    }
    else //GPRS 不在线
    {
        SmsConBuf[pos]=0x30;
        pos++;
    }
    SmsConBuf[pos]=',';
    pos++;

    sprintf(StrDat,"CSQ:%d",t_GetStatus.CsqValue);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=StrDat[i];
        pos++;
    }
    SmsConBuf[pos]=',';
    pos++;

    if(t_GetStatus.AccStatus==1) //ACC 状态
    {
        len=sizeof(China_Acc1);
        for(i=0; i<len; i++)
        {
            SmsConBuf[pos]=China_Acc1[i];
            pos++;
        }
    }
    else
    {
        len=sizeof(China_Acc2);
        for(i=0; i<len; i++)
        {
            SmsConBuf[pos]=China_Acc2[i];
            pos++;
        }
    }
    SmsConBuf[pos]=',';
    pos++;

    if(t_GetStatus.PowerStatus==1) //外电状态
    {
        len=sizeof(China_Pow1);
        for(i=0; i<len; i++)
        {
            SmsConBuf[pos]=China_Pow1[i];
            pos++;
        }
    }
    else
    {
        len=sizeof(China_Pow2);
        for(i=0; i<len; i++)
        {
            SmsConBuf[pos]=China_Pow2[i];
            pos++;
        }
    }
    SmsConBuf[pos]=',';
    pos++;

    sprintf(StrDat,",RunTime:%d",t_GetStatus.TotalRunTime);  //当前总运行时间
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=StrDat[i];
        pos++;
    }
    SmsConBuf[pos]='S';
    pos++;
//    YS_GprsDebugString("wei hu sms cont Length",pos);
    YS_SmsSendInterface((u8 *)SmsConBuf, pos, PhoneData, PhoneLen,TRUE);
    return(TRUE);
}

#if 0
/*-----------------------------------------------------------------------------------------
函数名：YS_SmsWeiHuSmsDeal
功能说明：维护短信检测处理
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsWeiHuSmsDeal_ASCII(u8 *dbuf, u16 dlen, u8 *PhoneData, u8 PhoneLen)
{
    char  SmsConBuf[400],smsdbg[100];
    u8  fbuf[400];
    char StrDat[30];
    t_Sys_Run_Status	t_GetStatus;
    t_Gps_Data_Info	t_GetGps;
    bool CompRlt;
    u16 i,len,pos,dat16;
    u8 VibSms;

    CompRlt=FALSE;
    if((dlen==2)&&((dbuf[0]=='w')||(dbuf[0]=='W'))&&((dbuf[1]=='h')||(dbuf[1]=='H')))  //维护
    {
        CompRlt=TRUE;
        YS_GprsDebugString("rec sms for weihu English",0);
    }
    else if((dlen==2)&&(dbuf[0]==0xF4)&&(dbuf[1]==0xA4))
    {
        CompRlt=TRUE;
        YS_GprsDebugString("rec sms for weihu Chinese",0);
    }

    if(CompRlt==FALSE) //比较失败
    {
        return(FALSE);
    }
    YS_GpsGetPosData(&t_GetGps);
    YS_RunGetSystemStatus(&t_GetStatus);
    pos=0;

    len=strlen(EBIKE_SOFT_VER);             //软件版本
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=EBIKE_SOFT_VER[i];
        pos++;
    }
    SmsConBuf[pos]='\n';
    pos++;
    YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);//ip地址端口
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=fbuf[i];
        pos++;
    }
    SmsConBuf[pos]=':';
    pos++;
    YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
    dat16=fbuf[0]*256+fbuf[1];
    sprintf(StrDat,"%d",dat16);
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=StrDat[i];
        pos++;
    }
    SmsConBuf[pos]='\n';
    pos++;

    YS_RunAddIDInfo(fbuf);//id号
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_SIM_CODE_LEN);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=fbuf[i];
        pos++;
    }
    SmsConBuf[pos]='\n';
    pos++;

    sprintf(StrDat,"GM=%d",(t_GetStatus.CsqValue*100/31));//信号值
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=StrDat[i];
        pos++;
    }
    SmsConBuf[pos]='%';
    pos++;
    SmsConBuf[pos]='\n';
    pos++;

    SmsConBuf[pos]='G';		//平台连接状态
    pos++;
    SmsConBuf[pos]='R';
    pos++;
    SmsConBuf[pos]='=';
    pos++;
    if(t_GetStatus.RunFlow==YS_RUN_FLOW_IDLE_DEAL) //GPRS在线
    {
        SmsConBuf[pos]=0x31;
        pos++;
    }
    else //GPRS 不在线
    {
        SmsConBuf[pos]=0x30;
        pos++;
    }
    SmsConBuf[pos]='\n';
    pos++;

    SmsConBuf[pos]='G';		//GPS
    pos++;
    SmsConBuf[pos]='P';
    pos++;
    SmsConBuf[pos]='S';
    pos++;
    SmsConBuf[pos]='=';
    pos++;
    if(t_GetGps.Effective==TRUE) //定位状态
    {
        SmsConBuf[pos]=0x31;
        pos++;
    }
    else
    {
        SmsConBuf[pos]=0x30;
        pos++;
    }
    SmsConBuf[pos]=',';
    pos++;
    SmsConBuf[pos]=0x30+YS_SmsGetGpsInfo();
    pos++;

    SmsConBuf[pos]='\n';
    pos++;

    SmsConBuf[pos]='A';		//ACC
    pos++;
    SmsConBuf[pos]='C';
    pos++;
    SmsConBuf[pos]='C';
    pos++;
    SmsConBuf[pos]='=';
    pos++;
    if(t_GetStatus.AccStatus==1) //ACC 状态
    {
        SmsConBuf[pos]=0x31;
        pos++;
    }
    else
    {
        SmsConBuf[pos]=0x30;
        pos++;
    }
    SmsConBuf[pos]='\n';
    pos++;

    SmsConBuf[pos]='P';
    pos++;
    SmsConBuf[pos]='=';
    pos++;
    if(t_GetStatus.PowerStatus==1) //外电状态
    {
        SmsConBuf[pos]=0x31;
        pos++;
    }
    else
    {
        SmsConBuf[pos]=0x30;
        pos++;
    }
    SmsConBuf[pos]='\n';
    pos++;

    SmsConBuf[pos]='Z';//车主号
    pos++;
    SmsConBuf[pos]='H';
    pos++;
    SmsConBuf[pos]='=';
    pos++;
    YS_PrmReadOneItem(FLH_PRM_OWNER_CODE,FLH_PRM_OWNER_CODE_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_OWNER_CODE_LEN);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=fbuf[i];
        pos++;
    }
    SmsConBuf[pos]='\n';
    pos++;

    SmsConBuf[pos]='M';//密码
    pos++;
    SmsConBuf[pos]='=';
    pos++;
    YS_PrmReadOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=fbuf[i];
        pos++;
    }
    SmsConBuf[pos]='\n';
    pos++;

    sprintf(StrDat,"RT=%d",t_GetStatus.TotalRunTime);  //当前总运行时间
    len=strlen(StrDat);
    for(i=0; i<len; i++)
    {
        SmsConBuf[pos]=StrDat[i];
        pos++;
    }
    SmsConBuf[pos]='\n';
    pos++;

    sprintf(smsdbg,"wei hu sms cont Length:%d",pos);
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, smsdbg,strlen(smsdbg));
    YS_SmsSendMessage_7BIT(PhoneData, PhoneLen, (u8 *)SmsConBuf, pos);
    return(TRUE);
}
#endif
/*-----------------------------------------------------------------------------------------
函数名：YS_CompOwnerCode
功能说明：比较号码是否与车主号码一致
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_CompOwnerCode(u8 *PhoneData, u8 PhoneLen)
{
    u8 fbuf[20];
    u16 i,len;

    YS_PrmReadOneItem( FLH_PRM_OWNER_CODE,FLH_PRM_OWNER_CODE_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_OWNER_CODE_LEN);
    if(len==0) //如果车主号码未设置
    {
        return TRUE;
    }
    for (i=len-1; i>0; i--) //匹配后11位
    {
        if (fbuf[i]!=PhoneData[PhoneLen-1])
        {
            return FALSE;
        }
        PhoneLen --;
    }
    return TRUE;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsSetParamDeal
功能说明：请求参数设置
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsSetParamDeal(u8 *dbuf, u16 dlen, u8 *PhoneData, u8 PhoneLen)
{
    u8	SelBuf[50];
    u16 i,k,len,loop,pos,count,OKPos,FailPos;
    u8 OutBuf[150];
    u8 SetOKBuf[150];
    u8 SetFailBuf[150];
    u8 SmsConBuf[150];

    loop=0;		//求本次共输入参数的个数
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]=='#')
        {
            loop++;
        }
    }

    if(loop<1) //未输入有效设置项目
    {
        YS_SmsAckSmsDeal(SETPRM_SMS_ACK_CONERR,PhoneData, PhoneLen);
        return;
    }

    if(PhoneLen>13)
    {
        for(k=0; k<loop; k++)
        {
            pos=0;
            len=YS_CodeGetItemInBuf(dbuf,dlen,SelBuf,k+1,'#',50);
            if(YS_WorkPrmSetDeal(SelBuf,len))	//处理参数设置工作
            {
                OutBuf[pos]=1;
            }
            else
            {
                OutBuf[pos]=0;
            }
            pos++;

            count=0;
            for(i=0; i<len; i++)
            {
                if(SelBuf[i]=='=')
                {
                    break;
                }
                count++;
            }
            for(i=0; i<count; i++)
            {
                OutBuf[pos]=SelBuf[i];
                pos++;
            }
            YS_GprsServerSendInterface(SERV_UP_CMD_SETPRMACK,OutBuf,pos);
        }
    }
    else
    {
        OKPos=0;
        FailPos=0;
        for(k=0; k<loop; k++)
        {
            len=YS_CodeGetItemInBuf(dbuf,dlen,SelBuf,k+1,'#',50);
            if(YS_WorkPrmSetDeal(SelBuf,len))	//处理参数设置工作
            {
                for(i=0; i<len; i++)		//添加成功项
                {
                    SetOKBuf[OKPos+i]=SelBuf[i];
                }
                OKPos+=len;
                SetOKBuf[OKPos]=',';
                OKPos++;
            }
            else
            {
                for(i=0; i<len; i++)		//添加失败项
                {
                    SetFailBuf[FailPos+i]=SelBuf[i];
                }
                FailPos+=len;
                SetFailBuf[FailPos]=',';
                FailPos++;
            }
        }

        pos=0;	//生成回复短信内容
        if(OKPos>0)
        {
            OKPos--;
            for(i=0; i<OKPos; i++)
            {
                SmsConBuf[pos]=SetOKBuf[i];
                pos++;
            }
            len=sizeof(SmsCon_Def_SetPrm_OK);
            for(i=0; i<len; i++)
            {
                SmsConBuf[pos]=SmsCon_Def_SetPrm_OK[i];
                pos++;
            }
        }

        if(FailPos>0)
        {
            FailPos--;
            for(i=0; i<FailPos; i++)
            {
                SmsConBuf[pos]=SetFailBuf[i];
                pos++;
            }

            len=sizeof(SmsCon_Def_SetPrm_FAIL);
            for(i=0; i<len; i++)
            {
                SmsConBuf[pos]=SmsCon_Def_SetPrm_FAIL[i];
                pos++;
            }
        }
        YS_SmsSendInterface(SmsConBuf, pos, PhoneData, PhoneLen,TRUE);
    }

}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsReadParamDeal
功能说明：请求查询参数处理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsReadParamDeal(u8 *dbuf, u16 dlen, u8 *PhoneData, u8 PhoneLen)
{
    u8	SelBuf[50];
    u16 i,k,len,loop,FailPos,AddLen,OkPos,PackPos;
    u8 SmsConBuf[150];
    u8 SetOKBuf[150];
    u8 SetFailBuf[150];

    loop=0;
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]=='#')
        {
            loop++;
        }
    }

    if(loop<1) //未输入有效设置项目
    {
        YS_SmsAckSmsDeal(READPRM_SMS_ACK_CONERR,PhoneData, PhoneLen);
        return;
    }

    if(PhoneLen>13)
    {
        for(k=0; k<loop; k++)
        {
            len=YS_CodeGetItemInBuf(dbuf,dlen,SelBuf,k+1,'#',50);
            if(YS_WorkPrmReadDeal(SelBuf,len,SetOKBuf,&AddLen))
            {
                YS_GprsServerSendInterface(SERV_UP_CMD_READPRMACK,SetOKBuf,AddLen);
            }
        }
    }
    else
    {
        FailPos=0;
        OkPos=0;
        for(k=0; k<loop; k++)
        {
            len=YS_CodeGetItemInBuf(dbuf,dlen,SelBuf,k+1,'#',50);
            if(YS_WorkPrmReadDeal(SelBuf,len,&SetOKBuf[OkPos],&AddLen))
            {
                OkPos+=AddLen;
                SetOKBuf[OkPos]=',';
                OkPos++;
            }
            else
            {
                for(i=0; i<len; i++)
                {
                    SetFailBuf[FailPos+i]=SelBuf[i];
                }
                FailPos+=len;
                SetFailBuf[FailPos]=',';
                FailPos++;
            }
        }

        PackPos=0;
        if(OkPos>0)
        {
            OkPos--;
            len=sizeof(SmsCon_Def_ReadPrm_OK);
            for(i=0; i<len; i++)
            {
                SmsConBuf[PackPos]=SmsCon_Def_ReadPrm_OK[i];
                PackPos++;
            }
            for(i=0; i<OkPos; i++)
            {
                SmsConBuf[PackPos]=SetOKBuf[i];
                PackPos++;
            }
        }

        if(FailPos>0)
        {
            FailPos--;
            len=sizeof(SmsCon_Def_ReadPrm_FAIL);
            for(i=0; i<len; i++)
            {
                SmsConBuf[PackPos]=SmsCon_Def_ReadPrm_FAIL[i];
                PackPos++;
            }
            for(i=0; i<FailPos; i++)
            {
                SmsConBuf[PackPos]=SetFailBuf[i];
                PackPos++;
            }
        }
        YS_SmsSendInterface(SmsConBuf, PackPos, PhoneData, PhoneLen,TRUE);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsModifyPassword
功能说明：处理密码修改请求
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsModifyPassword(u8 *dbuf, u16 dlen, u8 *PhoneData, u8 PhoneLen)
{
    u16 i,count,ItemLen;
    u8 fbuf[100],t_buf[20];
    bool CodeComp;

    count=0;
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]=='#')
        {
            count++;
        }
    }

    if(count!=2)
    {
        return;
    }

    ItemLen=YS_CodeGetItemInBuf(dbuf,dlen,fbuf,1,'#',100);	//终端密码设置
    YS_PrmReadOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,t_buf);
    CodeComp=TRUE;
    for(i=0; i<6; i++)
    {
        if(fbuf[i]!=t_buf[i])
        {
            CodeComp=FALSE;
            break;
        }
    }
    if(CodeComp==FALSE)
    {
        YS_SmsAckSmsDeal(SZMM_SMS_ACK_ERR1,PhoneData, PhoneLen);
        return;
    }


    ItemLen=YS_CodeGetItemInBuf(dbuf,dlen,fbuf,2,'#',100);	//终端密码设置
    if(YS_SmsCompString(fbuf,ItemLen,6)==FALSE)
    {
        YS_SmsAckSmsDeal(SZMM_SMS_ACK_ERR2,PhoneData, PhoneLen);
        return;
    }

    fbuf[ItemLen]=0;
    YS_PrmWriteOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,fbuf);
    YS_SmsAckSmsDeal(SZMM_SMS_ACK_SUCC,PhoneData, PhoneLen);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsParaseRstCmd
功能说明：解析复位指令
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsParaseRstCmd(u8 *dbuf, u16 dlen)
{
    if((dlen==4)&&(dbuf[0]==0xD6)&&(dbuf[1]==0xD8)&&(dbuf[2]==0xC6)&&(dbuf[3]==0xF4)) //中文重启D6 D8 C6 F4
    {
        return(TRUE);
    }
    else if((dlen==2)&&((dbuf[0]=='c')||(dbuf[0]=='C'))&&((dbuf[1]=='q')||(dbuf[1]=='Q')))
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsParaseUpdateCmd
功能说明：解析升级指令
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsParaseUpdateCmd(u8 *dbuf, u16 dlen)
{
    if((dlen==6)&&(dbuf[0]=='U')&&(dbuf[1]=='P')&&(dbuf[2]=='D')&&(dbuf[3]=='A')&&(dbuf[4]=='T')&&(dbuf[5]=='E')) //远程升级
    {
        YS_WebAddRequest();
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}
/*-----------------------------------------------------------------------------------------
函数名：YS_SmsCompHeadDeal
功能说明：比较操作密码
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsCompPassWordDeal(u8 *dbuf,u16 dlen)
{
    u8 	StrDat[20];
    u8	fbuf[20];
    u16 i,GetLen,len;
    bool CompRlt;


    GetLen=YS_CodeGetItemInBuf(dbuf,dlen,StrDat,1,',',20);
    if(GetLen==0)
    {
        return(FALSE);
    }

    if(StrDat[GetLen-1]=='#')  //如果取到结束字符
    {
        GetLen--;
    }

    YS_PrmReadOneItem(FLH_PRM_PASSWORD,FLH_PRM_PASSWORD_LEN,fbuf);
    len=YS_CodeBufRealLen(fbuf,FLH_PRM_PASSWORD_LEN);
    if(GetLen!=len)
    {
        return(FALSE);
    }
    CompRlt=TRUE;
    for(i=0; i<len; i++)
    {
        if(fbuf[i]!=StrDat[i])
        {
            CompRlt=FALSE;
            break;
        }
    }
    return(CompRlt);
}


/*-----------------------------------------------------------------------------------------
函数名：YS_SmsPosRsqSendInterface
功能说明：平台位置查询处理入口
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_SmsPosRsqSendInterface(u8 *dbuf, u16 dlen, u8 *PhoneCode, u8 PhoneLen, u8 SmsEvent,bool FormChangeFlag)
{
    return;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsSetServerAddress
功能说明：采用短信配置服务器地址
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsSetServerAddress(u8 *dbuf,u16 dlen, u8 *PhoneData, u8 PhoneLen)
{
    char StrDat[20];
    u8	fbuf[20];
    u16 i,pos,count,Value;

    if(YS_SmsCompHeadDeal(SMS_CMD_HEAD_IPPORT,dbuf,dlen)==FALSE) //设置服务地址
    {
        return FALSE;
    }

    count=0;
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]=='#')
        {
            count++;
        }
    }

    if(count!=5)  //字段数目不足
    {
        YS_SmsAckSmsDeal(SMS_ACK_FLAG_ERRCMD,PhoneData,PhoneLen);
    }
    else
    {
        count=0;
        pos=0;
        for(i=0; i<dlen; i++)
        {
            if(dbuf[i]=='#')
            {
                count++;
                if(count==3)
                {
                    pos=i+1;
                }
            }
        }

        count=0;
        for(i=pos; i<dlen; i++)
        {
            if(dbuf[i]=='#')
            {
                break;
            }
            if(count<20)
            {
                count++;
                StrDat[i-pos]=dbuf[i];
            }
        }

        if(count<10)
        {
            YS_SmsAckSmsDeal(SMS_ACK_FLAG_ERRCMD,PhoneData,PhoneLen);
            return(TRUE);
        }
        StrDat[count]=0;
        YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,(u8 *)StrDat);
        count=0; //端口处理
        for(i=0; i<dlen; i++)
        {
            if(dbuf[i]=='#')
            {
                count++;
                if(count==4)
                {
                    pos=i+1;
                }
            }
        }
        count=0;
        for(i=pos; i<dlen; i++)
        {
            if(dbuf[i]=='#')
            {
                break;
            }
            if(count<20)
            {
                count++;
                StrDat[i-pos]=dbuf[i];
            }
        }

        if(count==0)
        {
//            YS_SmsAckSmsDeal(SMS_ACK_FLAG_ERRCMD,PhoneData,PhoneLen);
        }
        else
        {
            StrDat[count]=0;
            if(YS_CodePDNumString((char *)StrDat)==FALSE) //非法字符
            {
                YS_SmsAckSmsDeal(SMS_ACK_FLAG_ERRCMD,PhoneData,PhoneLen);
            }
            else
            {
                Value=atoi((char *)StrDat);
                fbuf[0]=Value/256;
                fbuf[1]=Value%256;
                YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
                YS_SmsAckSmsDeal(SMS_ACK_FLAG_SERVADDR,PhoneData,PhoneLen);
            }
        }
    }
    return(TRUE);

}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsSetID
功能说明：采用短信设置ID
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsSetID(u8 *dbuf,u16 dlen, u8 *PhoneData, u8 PhoneLen)
{
    char StrDat[20];
    u8	fbuf[5];
    u16 i,pos=0,count;

    if(YS_SmsCompHeadDeal(SMS_CMD_HEAD_ID,dbuf,dlen)==FALSE)
    {
        return FALSE;
    }

    count=0;
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]=='#')
        {
            count++;
        }
    }

    if(count!=4)  //字段数目不足
    {
        YS_SmsAckSmsDeal(SMS_ACK_FLAG_ERRCMD,PhoneData,PhoneLen);
    }
    else
    {
        count=0;
        for(i=0; i<dlen; i++)
        {
            if(dbuf[i]=='#')
            {
                count++;
                if(count==3)
                {
                    pos=i+1;
                    break;
                }
            }
        }
        count=0;
        for(i=pos; i<dlen; i++)
        {
            if(dbuf[i]=='#')
            {
                break;
            }
            if(count<20)
            {
                count++;
                StrDat[i-pos]=dbuf[i];
            }
        }
        if(count==0)
        {
            YS_SmsAckSmsDeal(SMS_ACK_FLAG_ERRCMD,PhoneData,PhoneLen);
        }
        else
        {
            StrDat[count]=0;
            if(YS_CodePDNumString((char *)StrDat)==FALSE) //非法字符
            {
                YS_SmsAckSmsDeal(SMS_ACK_FLAG_ERRCMD,PhoneData,PhoneLen);
            }
            else
            {
                YS_PrmWriteOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN,(u8 *)StrDat);
                fbuf[0]=FJYD_TERI_STATUS_INIT;
                YS_PrmWriteOneItem(FLH_PRM_ACTIVE_FLAG,FLH_PRM_ACTIVE_FLAG_LEN,fbuf);
                YS_SmsAckSmsDeal(SMS_ACK_FLAG_SIM,PhoneData,PhoneLen);
            }
        }
    }
    return(TRUE);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsComInfoParase
功能说明：新短信内容处理
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_SmsComInfoParase(u8 *dbuf, u16 dlen, u8 *PhoneData, u8 PhoneLen)
{
    u16 i,GetLen;
    u16 pos,ParmPos,ParmLen;
    u8 ParmValue;
    u8 StrDat[20],fbuf[20];

    if(dlen==0)
    {
        return(FALSE);
    }
    for(i=0; i<dlen-1; i++)
    {
        if((dbuf[i]==0xA3)&&(dbuf[i+1]==0xAC))      //全角逗号
        {
            dbuf[i]=',';
            dlen-=1;
            for(pos=i+1; pos<dlen; pos++)
            {
                dbuf[pos] = dbuf[pos+1];
            }
        }
        else if((dbuf[i]==0xA3)&&(dbuf[i+1]==0xA3))  //全角井号
        {
            dbuf[i]='#';
            dlen-=1;
            for(pos=i+1; pos<dlen; pos++)
            {
                dbuf[pos] = dbuf[pos+1];
            }
        }
    }
    if(YS_SmsSetServerAddress(dbuf,dlen,PhoneData,PhoneLen)==TRUE) //服务器地址配置
    {
        YS_UartDebugInterfacel(INTER_PTL_UPDATE_SMSREC,(u8 *)"ip ok",5);
        return(TRUE);
    }
    else if(YS_SmsSetID(dbuf,dlen,PhoneData,PhoneLen)==TRUE) //配置终端ID号(SIM卡号)
    {
        YS_UartDebugInterfacel(INTER_PTL_UPDATE_SMSREC,(u8 *)"id ok",5);
//        YS_SysRsqSystemRST(YS_RST_FLAG_USER);
        return(TRUE);
    }
    else if(YS_SmsParaseUpdateCmd(dbuf,dlen)==TRUE)  //重启
    {
//        YS_SmsAckSmsDeal(SMS_ACK_FLAG_CQ,PhoneData,PhoneLen);
//        YS_SysRsqSystemRST(YS_RST_FLAG_USER);
        return(TRUE);
    }
    else if(YS_SmsParaseRstCmd(dbuf,dlen)==TRUE)  //重启
    {
        YS_SmsAckSmsDeal(SMS_ACK_FLAG_CQ,PhoneData,PhoneLen);
        YS_SysRsqSystemRST(YS_RST_FLAG_USER);
        return(TRUE);
    }
    else if(YS_SmsCompHeadDeal(SMS_CMD_HEAD_PLATE,dbuf,dlen)==TRUE)//设置车牌号
    {
        u8 buf[30]={0};
        u8 DbgBuf[30];
//        sjfun_ucs2totext_str(buf,30,dbuf,2);
//        dlen=strlen((char *)buf);
        GetLen=YS_CodeGetItemInBuf(dbuf,dlen,StrDat,2,'#',FLH_JTB_PLATE_STRING_LEN);
        StrDat[GetLen]=0;
//        sjfun_ucs2totext_str(buf,GetLen*2+2,StrDat,2);
        YS_CodeHextoString(dbuf,GetLen*2+2,DbgBuf);
        ycsj_debug((char *)DbgBuf);
//        StrDat[0] = buf[0];
//        StrDat[1] = buf[1];
        YS_PrmWriteOneItem(FLH_JTB_PLATE_STRING,FLH_JTB_PLATE_STRING_LEN,StrDat);
        YS_SmsAckSmsDeal(SMS_ACK_FLAG_PLATE,PhoneData,PhoneLen);
    }
    else if(YS_SmsWeiHuSmsDeal(dbuf,dlen,PhoneData,PhoneLen)==TRUE) //维护
    {
        return(TRUE);
    }
    else if(((dbuf[0]=='S')||(dbuf[0]=='s'))&&((dbuf[1]=='Z')||(dbuf[1]=='z'))&&	//参数设置
            ((dbuf[2]=='C')||(dbuf[2]=='c'))&&((dbuf[3]=='S')||(dbuf[3]=='s')))
    {
        YS_SmsSetParamDeal(dbuf,dlen,PhoneData,PhoneLen);
    }
    else if(((dbuf[0]=='C')||(dbuf[0]=='c'))&&((dbuf[1]=='X')||(dbuf[1]=='x'))&&	//查询参数
            ((dbuf[2]=='C')||(dbuf[2]=='c'))&&((dbuf[3]=='S')||(dbuf[3]=='s')))
    {
        YS_SmsReadParamDeal(dbuf,dlen,PhoneData,PhoneLen);
    }

    else if(((dbuf[0]=='Y')||(dbuf[0]=='y'))&&((dbuf[1]=='C')||(dbuf[1]=='c'))&&((dbuf[2]=='S')||(dbuf[2]=='s'))&&
            ((dbuf[3]=='J')||(dbuf[3]=='j')))
    {
        YS_WebAddRequest();
//        YS_PrmWriteOneItem(FLH_PRM_DW_PHONE,FLH_PRM_DW_PHONE_LEN,PhoneData);
//        fbuf[0]=1;
//        YS_PrmWriteOneItem(FLH_PRM_NV_DWFLAG, FLH_PRM_NV_DWFLAG_LEN,fbuf);
//        YS_SysRsqSystemRST(YS_RST_FLAG_DOWNLOAD);
    }
    else if(((dbuf[0]=='D')||(dbuf[0]=='d'))&&((dbuf[1]=='D')||(dbuf[1]=='d'))&&((dbuf[2]=='B')||(dbuf[2]=='b'))&&
            ((dbuf[3]=='W')||(dbuf[3]=='w')))
    {
        YS_RunWarnSignInput(FJYD_WARN_ID_BATTLOW);
        //YS_GprsServerSendInterface(SERV_UP_WARN_BATTLOW_ID,NULL,0);
    }
    else if(((dbuf[0]=='C')||(dbuf[0]=='c'))&&((dbuf[1]=='C')||(dbuf[1]=='c'))&&((dbuf[2]=='B')||(dbuf[2]=='b'))&&
            ((dbuf[3]=='W')||(dbuf[3]=='w')))
    {
        YS_RunWarnSignInput(FJYD_WARN_ID_DISMATLE);
    }
    else if((dbuf[0]=='S')&&(dbuf[1]=='E')&&(dbuf[2]=='T')&&(dbuf[3]=='P'))
    {
        GetLen = YS_CodeGetItemInBuf(dbuf,dlen,(u8 *)StrDat,1,',',50);
        ParmPos = atoi((char*)StrDat);
        YS_UartDebugInterfacel(0X8110, StrDat, GetLen);
        GetLen = YS_CodeGetItemInBuf(dbuf,dlen,(u8 *)StrDat,2,',',50);
        ParmLen= atoi((char*)StrDat);
        YS_UartDebugInterfacel(0X8110, StrDat, GetLen);
        GetLen = YS_CodeGetItemInBuf(dbuf,dlen,(u8 *)StrDat,3,',',50);
        ParmValue = atoi((char*)StrDat);
        YS_UartDebugInterfacel(0X8110, StrDat, GetLen);
        YS_PrmWriteOneItem(ParmPos, ParmLen, &ParmValue);
    }
    else if((dbuf[0]=='S')&&(dbuf[1]=='H')&&(dbuf[2]=='U')&&(dbuf[3]=='T'))
    {
        YS_SetShutDownFlag();
    }
    else if((dbuf[0]=='I')&&(dbuf[1]=='R')&&(dbuf[2]=='J')&&(dbuf[3]=='Z'))
    {
        ;
    }
    else
    {
        YS_SmsAckSmsDeal(SMS_ACK_FLAG_ERRCMD,PhoneData,PhoneLen);
    }
    return FALSE;
}

/*-------------------------------------------------------------------------------------------
函数名：YS_SmsComeInterface
功能说明：系统短信，接收新短信接口
修改记录：
--------------------------------------------------------------------------------------------*/
void  YS_SmsComeInterface(char* sFromNumber,char* sContent)
{
    u8 sFromNumberLen;

    sFromNumberLen = strlen(sFromNumber);
    YS_RunSmsDataInc(DATA_FLAG_REC_ADD);
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_SMSREC,(u8*)sFromNumber,strlen(sFromNumber));
    YS_UartDebugInterfacel(INTER_PTL_UPDATE_SMSREC,(u8*)sContent,strlen(sContent));

    if(strcmp(sFromNumber,"10086")==0)
    {
        return;
    }
    YS_SmsComInfoParase((u8 *)sContent, strlen(sContent), (u8*)sFromNumber, sFromNumberLen);
}

/*-------------------------------------------------------------------------------------------
函数名：YS_SmsManageInit
功能说明：短信处理初始化
修改记录：
--------------------------------------------------------------------------------------------*/
void YS_SmsManageInit(void)
{
    t_SmsActiveRsq.ActFlag=FALSE;
    t_SmsComMoni.NewFlag=0;
    YS_SmsMulSendInit();
}

/*-------------------------------------------------------------------------------------------
函数名：YS_SmsComInputSimuLation
功能说明：串口短信模拟输入
修改记录：
--------------------------------------------------------------------------------------------*/
void YS_SmsComInputSimuLation(u8 *dbuf, u16 dlen)
{
    u16 i,pLen,sLen;
    char ContInfo[200];
    char PhoneInfo[20];

    pLen=dbuf[0];
    for(i=0; i<pLen; i++)
    {
        PhoneInfo[i]=dbuf[i+1];
    }
    PhoneInfo[pLen]=0;

    sLen=dbuf[pLen+1];
    for(i=0; i<sLen; i++)
    {
        ContInfo[i]=dbuf[pLen+2+i];
    }
    ContInfo[sLen]=0;

    t_SmsComMoni.NewFlag=1;
    strcpy(t_SmsComMoni.PhoneBuf,PhoneInfo);
    strcpy(t_SmsComMoni.ConBuf,ContInfo);
}

/*-------------------------------------------------------------------------------------------
函数名：YS_SmsSendComMoniCtrl
功能说明：串口短信定时器转发
修改记录：
--------------------------------------------------------------------------------------------*/
void YS_SmsSendComMoniCtrl(void)
{
    if(t_SmsComMoni.NewFlag==1)
    {
        t_SmsComMoni.NewFlag=0;
        YS_SmsComeInterface(t_SmsComMoni.PhoneBuf,t_SmsComMoni.ConBuf);
    }
}

/*-------------------------------------------------------------------------------------------
函数名：YS_SmsDealTimerInput
功能说明：短信处理，定时器输入
修改记录：
--------------------------------------------------------------------------------------------*/
void YS_SmsDealTimerInput(void)
{
    YS_SmsSendComMoniCtrl();

    YS_SmsMulSendManage();
}
