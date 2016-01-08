/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_AutoUpdate.c  2014-10-13
���ļ������ն��Ƿ���Ҫ�������Զ��ж�
*/
#include "YS_Public.h"

extern t_Sys_Run_Status  	t_SysRunStatus;
extern t_Flow_Info			t_FlowInfo;

t_Auto_Update_Manage		t_AutoUpdateManage;
/*-----------------------------------------------------------------------------------------
��������YS_AutoUpdateSocketBeing
����˵���������Զ�����ƽ̨SOCKET ���� 
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_AutoUpdateSocketBeing(void)
{
	u8 fbuf[40],IPBuf[4];
	u16 i,len,count,Port,rlt;
	char StrDat[10];
		
	YS_PrmReadOneItem(FLH_PRM_NV_AUTOPLAT_ADDR,FLH_PRM_NV_AUTOPLAT_ADDR_LEN,fbuf);
	len=YS_CodeBufRealLen(fbuf,FLH_PRM_NV_AUTOPLAT_ADDR_LEN);
	count=0;

	for(i=0; i<4; i++)
	{
	    count=YS_CodeGetItemInBuf(fbuf,len,(u8 *)StrDat,i,'.',4);
	    StrDat[count]=0;
	    IPBuf[i]=atoi(StrDat);
	}	
	
	YS_PrmReadOneItem(FLH_PRM_NV_AUTOPLAT_PORT,FLH_PRM_NV_AUTOPLAT_PORT_LEN,fbuf);
	Port=fbuf[0]*256+fbuf[1];
	rlt=sjfun_Socket_Create(t_FlowInfo.AcctID, &t_FlowInfo.AutoUpdateSckID,IPBuf,Port,1);
	return(rlt);
}

/*-----------------------------------------------------------------------------------------
��������YS_AutoUpdateSocketClose
����˵�����ر��Զ�����ƽ̨SOCKET
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void	YS_AutoUpdateSocketClose(void)
{
	sjfun_Socket_Close(t_FlowInfo.AutoUpdateSckID);	
}

/*-----------------------------------------------------------------------------------------
��������YS_AutoUpdateSocketAck
����˵�����Զ�����ƽ̨SOCKET �����ɹ�Ӧ��ص�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AutoUpdateSocketAck(void)
{
	t_AutoUpdateManage.SocketOKFlag=1;
}

/*-----------------------------------------------------------------------------------------
��������YS_AutoInfoRsqAckDeal
����˵�����Զ�����ƽ̨Ӧ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AutoInfoRsqAckDeal(u8 *dbuf, u16 dlen)
{
	u8 fbuf[40];
	u16 i,AddrLen;

	if(dbuf[0]==0) //�������Ҫ������������
	{
		t_AutoUpdateManage.RsqInfoOKFlag=1;
		t_AutoUpdateManage.RsqInfoUpdateFlag=0;
		YS_SysRsqSystemRST(YS_RST_FLAG_USER);	 //��λϵͳ
	}
	else
	{
		fbuf[0]=1; 
		YS_PrmWriteOneItem(FLH_PRM_NV_DWFLAG,FLH_PRM_NV_DWFLAG_LEN,fbuf);
		fbuf[0]=dbuf[1];
		fbuf[1]=dbuf[2];
		YS_PrmWriteOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);
		AddrLen=dbuf[3];
		for(i=0; i<AddrLen; i++)
		{
			fbuf[i]=dbuf[4+i];
		}
		fbuf[AddrLen]=0;
		YS_PrmWriteOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);
		t_AutoUpdateManage.RsqInfoOKFlag=1;
		t_AutoUpdateManage.RsqInfoUpdateFlag=1;
		YS_PrmQuickWriteNV(); 
		YS_SysRsqSystemRST(YS_RST_FLAG_DOWNLOAD);	 //��λϵͳ
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_AutoUpdateSendData
����˵�����Զ�����ƽ̨��������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AutoUpdateSendData(u8 *dbuf, u16 dlen)
{
	u32 SendRlt;
	
	YS_RunSocketDataInc(DATA_FLAG_SEND_ADD);
	YS_UartDebugInterfacel(INTER_PTL_UPDATE_GPRSSEND,dbuf,dlen);
	SendRlt=sjfun_Socket_Send(t_FlowInfo.AutoUpdateSckID, dbuf, dlen);
}

/*-----------------------------------------------------------------------------------------
��������YS_AutoUpdateSendInterface
����˵�����Զ�����ƽ̨���ʹ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AutoUpdateSendInterface(u8 CmdID, u8 *dbuf, u16 dlen)
{
	u8 Gprs_Send_Buf[200];
	u16 i,pos,len;
	u8 verify;
	
	pos=0;
	Gprs_Send_Buf[pos]='Y';
	pos++;
	Gprs_Send_Buf[pos]='S';
	pos++;
	len=YS_DWServerPackAddID(&Gprs_Send_Buf[pos]);
	pos+=len;
	len=YS_DWServerPackAddPrjName(&Gprs_Send_Buf[pos]);
	pos+=len;
	Gprs_Send_Buf[pos]=CmdID;
	pos++;
	Gprs_Send_Buf[pos]=dlen/256;
	pos++;
	Gprs_Send_Buf[pos]=dlen%256;
	pos++;
	if(dlen>0)
	{
		for(i=0; i<dlen; i++)
		{
			Gprs_Send_Buf[pos]=dbuf[i];
			pos++;
		}
	}
	verify=0;
	for(i=0; i<pos; i++)
	{
		verify+=Gprs_Send_Buf[pos];
	}
	Gprs_Send_Buf[pos]=verify;
	pos++;
	Gprs_Send_Buf[pos]=0x0D;
	pos++;
	Gprs_Send_Buf[pos]=0x0A;
	pos++;
	YS_AutoUpdateSendData(Gprs_Send_Buf,pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_AutoInfoSendRsqPackage
����˵�����Զ�����ƽ̨�����������ݰ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AutoInfoSendRsqPackage(void)
{
	u8	VerBuf[20];
	u16 	i,len;
	
	len=strlen(PROJECT_APP_VER_DEF);
	for(i=0; i<len; i++)
	{
		VerBuf[i]=PROJECT_APP_VER_DEF[i];
	}
	YS_AutoUpdateSendInterface(DW_UP_CMD_AUTO_RSQ,VerBuf,len);
}

/*-----------------------------------------------------------------------------------------
��������YS_AutoInfoRsqCtrl
����˵�����Զ�����ƽ̨��Ϣ�������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AutoInfoRsqCtrl(void)
{
	t_AutoUpdateManage.RsqInfoDelay++;
	if(t_AutoUpdateManage.RsqInfoDelay>=AUTO_RSQ_LOGIN_DELAY)
	{
		t_AutoUpdateManage.RsqInfoDelay=0;
		t_AutoUpdateManage.RsqInfoTimes++;
		if(t_AutoUpdateManage.RsqInfoTimes<=AUTO_RSQ_LOGIN_TIMES)
		{
			YS_AutoInfoSendRsqPackage();
		}
		else
		{
			t_AutoUpdateManage.RsqInfoFailFlag=1;
		}
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_AutoUpdateFlowCtrl
����˵�����������̿���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AutoUpdateFlowCtrl(void)
{
	switch(t_AutoUpdateManage.WorkFlow)
	{
	case YS_RUN_FLOW_INIT://��ʼ��
		if(t_AutoUpdateManage.WorkEnable==1)
		{
			ycsj_debug("Auto Begin Flow");
			t_AutoUpdateManage.WorkEnable=0;
			t_AutoUpdateManage.WorkFlow=YS_RUN_FLOW_SOCKET_BEGIN;
		}
		break;

	case YS_RUN_FLOW_SOCKET_BEGIN: //��ʼ����SOCKET
		t_AutoUpdateManage.SocketOKFlag=0;
		t_AutoUpdateManage.OptDelay=0;
		YS_AutoUpdateSocketBeing();
		t_AutoUpdateManage.WorkFlow=YS_RUN_FLOW_WAIT_SOCKET;
		break;

	case YS_RUN_FLOW_WAIT_SOCKET: //�ȴ�SOCKET ����
		if(t_AutoUpdateManage.SocketOKFlag==1)
		{
			ycsj_debug("Auto Socket OK");
			t_AutoUpdateManage.WorkFlow=YS_RUN_FLOW_SEVLOG_BEGIN;
		}
		else
		{
			t_AutoUpdateManage.OptDelay++;
			if(t_AutoUpdateManage.OptDelay>=AUTO_RSQ_SOCKET_DELAY)
			{
				ycsj_debug("Auto Socket Fail");
				YS_AutoUpdateSocketClose();
				t_AutoUpdateManage.WorkFlow=YS_RUN_FLOW_INIT;
			}
		}
		break;

	case YS_RUN_FLOW_SEVLOG_BEGIN: //��ʼ���������ж�
		t_AutoUpdateManage.RsqInfoOKFlag=0;
		t_AutoUpdateManage.RsqInfoFailFlag=0;
		t_AutoUpdateManage.RsqInfoTimes=0;
		t_AutoUpdateManage.RsqInfoDelay=AUTO_RSQ_LOGIN_DELAY;
		t_AutoUpdateManage.RsqInfoUpdateFlag=0;
		t_AutoUpdateManage.WorkFlow=YS_RUN_FLOW_WAIT_SERVLOG;
		break;

	case YS_RUN_FLOW_WAIT_SERVLOG: //���������ж��ȴ�
		if(t_AutoUpdateManage.RsqInfoOKFlag==1) //��Ϣ����ɹ���־
		{
			ycsj_debug("Auto Rsq Info ok");
			YS_AutoUpdateSocketClose();
			t_AutoUpdateManage.WorkFlow=YS_RUN_FLOW_INIT;
		}
		else if(t_AutoUpdateManage.RsqInfoFailFlag==1) //��Ϣ����ʧ�ܱ�־
		{
			ycsj_debug("Auto Rsq Info Fail");
			YS_AutoUpdateSocketClose();
			t_AutoUpdateManage.WorkFlow=YS_RUN_FLOW_INIT;
			YS_SysRsqSystemRST(YS_RST_FLAG_USER);	 //��λϵͳ
		}
		else //��Ϣ������
		{
			ycsj_debug("Auto Rsq Info Doing");
			YS_AutoInfoRsqCtrl();
		}
		break;
		
	default:
		break;
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_AutoUpdateRunInterface
����˵�����Զ�����ƽ̨������ں���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AutoUpdateRunInterface(void)
{
	u8 fbuf[4];

    	YS_PrmReadOneItem(FLH_PRM_NV_AUTOPLAT_ENABLE,FLH_PRM_NV_AUTOPLAT_ENABLE_LEN,fbuf);	
	if(fbuf[0]==0)
	{
		return;
	}

	YS_PrmReadOneItem(FLH_PRM_NV_AUTOPLAT_FLAG,FLH_PRM_NV_AUTOPLAT_FLAG_LEN,fbuf);	
	if(fbuf[0]==1)
	{
		t_AutoUpdateManage.WorkEnable=1; //��ʼ��������
		fbuf[0]=0; //����������ӱ�־
		YS_PrmWriteOneItem(FLH_PRM_NV_AUTOPLAT_FLAG,FLH_PRM_NV_AUTOPLAT_FLAG_LEN,fbuf);	
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_AutoUpdateInitParam
����˵�����Զ�����ƽ̨��ʼ�����в���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_AutoUpdateInitParam(void)
{
	t_AutoUpdateManage.WorkEnable=0;
	t_AutoUpdateManage.WorkFlow=YS_RUN_FLOW_INIT;
}



