/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Gprs.c  2013-06-25
���ļ��ù���Ӧ�ò������ײ����Ľ����ӿڡ�
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"
extern t_Sys_Run_Status  	t_SysRunStatus;
extern t_Flow_Info			t_FlowInfo;
extern t_avltra_Parase		t_AvlSckParase;

t_Update_Manage			t_UpdateManage;
t_Down_One_Group		t_DownOneGroup;


/*-----------------------------------------------------------------------------------------
��������YS_DWServerPackAddID
����˵��������ն�ID��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_DWServerPackAddID(u8 *PackBuf)
{
	sjfun_Get_NV_IMEI(PackBuf,8);
	return(8);
}

/*-----------------------------------------------------------------------------------------
��������YS_DWServerPackAddID
����˵��������ն�ID��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u16 YS_DWServerPackAddPrjName(u8 *PackBuf)
{
	u8 i,len;

	for(i=0; i<20; i++)
	{
		PackBuf[i]=0;
	}
	len=strlen(PROJECT_NAME_DEF);

	for(i=0; i<len; i++)
	{
		PackBuf[i]=PROJECT_NAME_DEF[i];
	}
	return(20);
}

/*-----------------------------------------------------------------------------------------
��������YS_DWSocketSendData
����˵����SOCKET ���ݷ��ͽӿ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWSocketSendData(u8 *dbuf, u16 dlen)
{
	u32 SendRlt;

	YS_RunSocketDataInc(DATA_FLAG_SEND_ADD);
	YS_UartDebugInterfacel(INTER_PTL_UPDATE_GPRSSEND,dbuf,dlen);
	SendRlt=sjfun_Socket_Send(t_FlowInfo.DWSckID, dbuf, dlen);
}

/*-----------------------------------------------------------------------------------------
��������YS_DWServerSendInterface
����˵��������ƽ̨���ݷ��ʹ������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWServerSendInterface(u8 CmdID, u8 *dbuf, u16 dlen)
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
	YS_DWSocketSendData(Gprs_Send_Buf,pos);
}


/*-----------------------------------------------------------------------------------------
��������YS_DWLoginServerCtrl
����˵������¼����ƽ̨����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWLoginServerCtrl(void)
{
	t_FlowInfo.LogDelay++;
	if(t_FlowInfo.LogDelay>=RUN_LOGIN_DELAY_DEF)
	{
		t_FlowInfo.LogDelay=0;
		t_FlowInfo.LogTimes++;
		if(t_FlowInfo.LogTimes<=RUN_LOGIN_TIMES_DEF)
		{
			YS_DWServerSendInterface(DW_UP_CMD_LOGIN, NULL,0);
		}
		else
		{
			t_FlowInfo.LogFailFlag=TRUE;
		}
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWLoginAckDeal
����˵������¼����ƽ̨����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWLoginAckDeal(u8 *dbuf, u16 dlen)
{
	if(dbuf[0]==1)
	{
		t_FlowInfo.LogOKFlag=TRUE;
	}
	else
	{
		t_FlowInfo.LogFullFlag=TRUE;
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWRsqFileInfoCtrl
����˵�������������ļ���Ϣ����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWRsqFileInfoCtrl(void)
{
	t_FlowInfo.RsqFileDelay++;
	if(t_FlowInfo.RsqFileDelay>=DW_RSQ_FILE_DELAY)
	{
		t_FlowInfo.RsqFileDelay=0;
		t_FlowInfo.RsqFileTimes++;
		if(t_FlowInfo.RsqFileTimes<=DW_RSQ_FILE_TIMES)
		{
			YS_DWServerSendInterface(DW_UP_CMD_RSQ_INFO, NULL,0);
		}
		else
		{
			t_FlowInfo.RsqFileFailFlag=1;
		}
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWLoginAckDeal
����˵������¼����ƽ̨����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWFileInfoAckDeal(u8 *dbuf, u16 dlen)
{
	Avl_ULong ldat;
	if(dbuf[0]==1)
	{
		t_FlowInfo.RsqFileOKFlag=1;
	}
	else
	{
		t_FlowInfo.RsqFileUnknowFlag=1;
	}

	if(t_UpdateManage.FirstRsqFlag==1)  //��һ�������ʱ��ų�ʼ������״̬
	{
		ldat.b[AVL_LSTOR_FIR]=dbuf[1];
		ldat.b[AVL_LSTOR_SEC]=dbuf[2];
		ldat.b[AVL_LSTOR_THI]=dbuf[3];
		ldat.b[AVL_LSTOR_FOR]=dbuf[4];
		t_UpdateManage.FileLen=ldat.l;
		t_UpdateManage.TotalPackNum=t_UpdateManage.FileLen/DW_ONE_FILE_PACK_LEN;
		if((t_UpdateManage.FileLen%DW_ONE_FILE_PACK_LEN)>0)
		{
			t_UpdateManage.TotalPackNum++;
		}
		t_UpdateManage.CurPackNO=0;
		t_UpdateManage.CurPackNum=0;
		t_UpdateManage.TotalGroupNum=t_UpdateManage.TotalPackNum/DW_ONE_FILE_GROUP_NUM;
		if((t_UpdateManage.TotalPackNum%DW_ONE_FILE_GROUP_NUM)>0)
		{
			t_UpdateManage.TotalGroupNum++;
		}
		t_UpdateManage.DownFileVerify=dbuf[5];
		t_UpdateManage.CalcFileVerify=0;
		t_UpdateManage.FileWritePos=0;
		ycsj_debug("FileInfo ack flen:%d,verify:%d,TotalGP:%d,TotalPK:%d",t_UpdateManage.FileLen,t_UpdateManage.DownFileVerify,t_UpdateManage.TotalGroupNum,t_UpdateManage.TotalPackNum);
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWGroupPackNOISOK
����˵�����ж���ǰ���ݰ��Ƿ���Ч
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_DWGroupPackNOISOK(u16 PackNO)
{
	u16 i,MaxPackNO;

	if(t_DownOneGroup.GroupBufPackNum>=DW_ONE_FILE_GROUP_NUM)
	{
		return(FALSE);
	}

	if((t_UpdateManage.CurPackNO+DW_ONE_FILE_GROUP_NUM)>t_UpdateManage.TotalPackNum)
	{ //��������һ�飬��������
		MaxPackNO=t_UpdateManage.CurPackNO+(t_UpdateManage.TotalPackNum%DW_ONE_FILE_GROUP_NUM);
	}
	else
	{//����������һ�飬��������
		MaxPackNO=t_UpdateManage.CurPackNO+DW_ONE_FILE_GROUP_NUM;
	}

	if((PackNO<t_UpdateManage.CurPackNO)||(PackNO>MaxPackNO))
	{
		return(FALSE);
	}

	if(t_DownOneGroup.GroupBufPackNum==0)
	{
		return(TRUE);
	}
	else
	{
		for(i=0; i<t_DownOneGroup.GroupBufPackNum; i++)
		{
			if(PackNO==t_DownOneGroup.t_DownOnePack[i].PackNO)
			{
				return(FALSE);
			}
		}
		return(TRUE);
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWGroupGetPosINGroup
����˵������ȡ��ǰ�ļ������е�λ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8  YS_DWGroupGetPosINGroup(u16 PackNO)
{
	u8 i,rlt;

	rlt=DW_PACK_STOR_UNKNOW;
	if(t_DownOneGroup.GroupBufPackNum==0)
	{
		return(rlt);
	}

	for(i=0; i<t_DownOneGroup.GroupBufPackNum; i++)
	{
		if(PackNO==t_DownOneGroup.t_DownOnePack[i].PackNO)
		{
			rlt=i;
			break;
		}
	}
	return(rlt);
}


/*-----------------------------------------------------------------------------------------
��������YS_SendCurGroupRsq
����˵�������͵�ǰ������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void	YS_DWSendGroupRsq(void)
{
	u16 i,pos,NeedPackNum;
	u8  CurNum,RsqBuf[50];

	if((t_UpdateManage.CurPackNO+DW_ONE_FILE_GROUP_NUM)>t_UpdateManage.TotalPackNum)
	{
		NeedPackNum=t_UpdateManage.TotalPackNum%DW_ONE_FILE_GROUP_NUM;
	}
	else
	{
		NeedPackNum=DW_ONE_FILE_GROUP_NUM;
	}

	pos=1;
	CurNum=0;
	for(i=0; i<NeedPackNum; i++)
	{
		if(YS_DWGroupGetPosINGroup(t_UpdateManage.CurPackNO+i)==DW_PACK_STOR_UNKNOW)
		{
			CurNum++;
			RsqBuf[pos]=(t_UpdateManage.CurPackNO+i)/256;
			pos++;
			RsqBuf[pos]=(t_UpdateManage.CurPackNO+i)%256;
			pos++;
		}
	}
	RsqBuf[0]=CurNum;
	YS_DWServerSendInterface(DW_UP_CMD_RSQ_FILE,RsqBuf,pos);
}

/*-----------------------------------------------------------------------------------------
��������YS_SendGroupCtrl
����˵�������͵�ǰ������ʱ���ƺ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_SendGroupCtrl(void)
{
	t_UpdateManage.RsqGroupDelay++;
	if(t_UpdateManage.RsqGroupDelay>=DW_RSQ_GROUP_DELAY)
	{
		t_UpdateManage.RsqGroupDelay=0;
		t_UpdateManage.RsqGroupTimes++;
		if(t_UpdateManage.RsqGroupTimes<=DW_RSQ_GROUP_TIMES)
		{
			YS_DWSendGroupRsq();
		}
		else
		{
			t_UpdateManage.RsqGroupFailFlag=1;
		}
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWSysFileBufWrite
����˵�����������ص������ݰ���д��ϵͳ�ļ�������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWSysFileBufWrite(void)
{
	u16 i,j,NeedPackNum;
	u16 PackPos,PackNO;
	u32 FileWritePos;

	if((t_UpdateManage.CurPackNO+DW_ONE_FILE_GROUP_NUM)>t_UpdateManage.TotalPackNum)
	{
		NeedPackNum=t_UpdateManage.TotalPackNum%DW_ONE_FILE_GROUP_NUM;
	}
	else
	{
		NeedPackNum=DW_ONE_FILE_GROUP_NUM;
	}

	for(i=0; i<NeedPackNum; i++)  //���������ݰ�д�뻺������������У��ֵ
	{
		PackNO=t_UpdateManage.CurPackNO+i;
		PackPos=YS_DWGroupGetPosINGroup(PackNO);
		for(j=0; j<t_DownOneGroup.t_DownOnePack[PackPos].PackLen; j++)
		{
			t_UpdateManage.CalcFileVerify+=t_DownOneGroup.t_DownOnePack[PackPos].PackBuf[j];
		}
		FileWritePos=PackNO*DW_ONE_FILE_PACK_LEN; //������д���ļ�������
//		ycsj_debug("DW APP FileWR,pos:%d,PackLen:%d..\n",FileWritePos,t_DownOneGroup.t_DownOnePack[PackPos].PackLen);
		sjfun_DW_Write_PackData(FileWritePos,t_DownOneGroup.t_DownOnePack[PackPos].PackBuf,t_DownOneGroup.t_DownOnePack[PackPos].PackLen);
//		ycsj_debug("PKNO:%d,PKPos:%d,dlen:%d,Verify:%d..",t_UpdateManage.CurPackNO+i,PackPos,t_DownOneGroup.t_DownOnePack[PackPos].PackLen,t_UpdateManage.CalcFileVerify);
	}
}

/*------------------------------------------------------------------------------------------
��������YS_DWGroupOKJudgeDeal
����˵�������黺���������ж����Ƿ�ǰ�����������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWGroupOKJudgeDeal(void)
{
	u16 i,NeedPackNum;
	bool RecOK;

	if((t_UpdateManage.CurPackNO+DW_ONE_FILE_GROUP_NUM)>t_UpdateManage.TotalPackNum)
	{
		NeedPackNum=t_UpdateManage.TotalPackNum%DW_ONE_FILE_GROUP_NUM;
	}
	else
	{
		NeedPackNum=DW_ONE_FILE_GROUP_NUM;
	}

	RecOK=TRUE;
	for(i=0; i<NeedPackNum; i++)
	{
		if(YS_DWGroupGetPosINGroup(t_UpdateManage.CurPackNO+i)==DW_PACK_STOR_UNKNOW)
		{ //������ݰ�������
			RecOK=FALSE;
			break;
		}
	}

	if(RecOK==TRUE) //������е����ݰ��������
	{
		YS_DWSysFileBufWrite();							//��������д���ļ�������
		t_UpdateManage.CurPackNO+=NeedPackNum;		//����ָ����һ������
		t_UpdateManage.CurPackNum+=NeedPackNum;
		t_UpdateManage.CurGroupNum++;
		t_UpdateManage.CurGroupNO++;
		t_UpdateManage.RsqGroupOKFlag=1;
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWFileDataInputDeal
����˵����ƽ̨�·����������ݰ�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWFileDataInputDeal(u8 *dbuf, u16 dlen)
{
	u16 i,PackNO;
	u8 p;

	if((dlen<3)||(dlen>(DW_ONE_FILE_PACK_LEN+2)))
	{
		return;
	}

	PackNO=dbuf[0]*256+dbuf[1];
	ycsj_debug("DW Get One Pack NO:%d..",PackNO);

	if(t_UpdateManage.RsqGroupOKFlag==1) //�����ǰ���ѽ�����
	{
		return;
	}

	if(YS_DWGroupPackNOISOK(PackNO)==TRUE)
	{
		p=t_DownOneGroup.GroupBufPackWP;
		t_DownOneGroup.t_DownOnePack[p].PackNO=PackNO;
		t_DownOneGroup.t_DownOnePack[p].PackLen=dlen-2;
		for(i=0; i<(dlen-2); i++)
		{
			t_DownOneGroup.t_DownOnePack[p].PackBuf[i]=dbuf[i+2];
		}
		t_DownOneGroup.GroupBufPackWP++;
		t_DownOneGroup.GroupBufPackNum++;
		YS_DWGroupOKJudgeDeal();
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWAppSocketBeing
����˵������������ƽ̨SOCKET
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
u8 YS_DWAppSocketBeing(void)
{
	u8 fbuf[40],IPBuf[4];
	u16 i,len,count,Port,rlt;
	char StrDat[10];

	YS_PrmReadOneItem(FLH_PRM_NV_DWADDR,FLH_PRM_NV_DWADDR_LEN,fbuf);
	len=YS_CodeBufRealLen(fbuf,FLH_PRM_NV_DWADDR_LEN);
	count=0;

	for(i=0; i<4; i++)
	{
	    count=YS_CodeGetItemInBuf(fbuf,len,(u8 *)StrDat,i,'.',4);
	    StrDat[count]=0;
	    IPBuf[i]=atoi(StrDat);
	}

	YS_PrmReadOneItem(FLH_PRM_NV_DWPORT,FLH_PRM_NV_DWPORT_LEN,fbuf);
	Port=fbuf[0]*256+fbuf[1];
	rlt=sjfun_Socket_Create(t_FlowInfo.AcctID, &t_FlowInfo.DWSckID,IPBuf,Port,1);
	return(rlt);
}

/*-----------------------------------------------------------------------------------------
��������YS_DWAppSocketAck
����˵������������ƽ̨SOCKET Ӧ��
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWAppSocketAck(void)
{
	t_UpdateManage.SocketOKFlag=1;
}

/*-----------------------------------------------------------------------------------------
��������YS_DWServerPtlParase
����˵�����ӻ�������ȡһ������֡�������н���ת������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_DWServerPtlParase(u8 *dbuf, u16 dlen, u16 *GetLen)
{
	u16 i,pos,getlen;
	u8 verify;
	pos=0;
	getlen=dlen;

	while(getlen>=16) //��СЭ�鳤��
	{
		if((dbuf[pos]=='Y')&&(dbuf[pos+1]=='S'))
		{
			t_AvlSckParase.v_CmdID=dbuf[pos+10];
			t_AvlSckParase.v_dlen=dbuf[pos+11]*256+dbuf[pos+12];
			if(t_AvlSckParase.v_dlen>=600)
			{
				pos++;
				getlen--;
				continue;
			}

			if(getlen<(t_AvlSckParase.v_dlen+16))
			{
				pos++;
				getlen--;
				continue;
			}

			if(t_AvlSckParase.v_dlen>0)
			{
				for(i=0; i<t_AvlSckParase.v_dlen; i++)
				{
					t_AvlSckParase.a_dbuf[i]=dbuf[pos+i+13];
				}
			}

			verify=0;
			for(i=pos; i<(t_AvlSckParase.v_dlen+13+pos); i++)
			{
				verify+=dbuf[i];
			}

			ycsj_debug("Verify!pos:%d,v_len:%d,vd:%d",pos,t_AvlSckParase.v_dlen,dbuf[pos+13+t_AvlSckParase.v_dlen]);

			if(verify!=dbuf[pos+13+t_AvlSckParase.v_dlen])
			{
				pos++;
				getlen--;
				ycsj_debug("DW Parsae verify err,v1:%d,v2:%d..",verify,dbuf[pos+13+t_AvlSckParase.v_dlen]);
				continue;
			}

			if((0x0D!=dbuf[pos+14+t_AvlSckParase.v_dlen])||(0x0A!=dbuf[pos+15+t_AvlSckParase.v_dlen]))
			{
				pos++;
				getlen--;
				ycsj_debug("DW Parase tail err..");
				continue;
			}

			*GetLen=16+t_AvlSckParase.v_dlen+pos;
			ycsj_debug("DW Parsae serv cmd ok");
			return(TRUE);
		}
		else
		{
			pos++;
			getlen--;
		}
	}

	ycsj_debug("DW Parsae serv cmd fail");
	return(FALSE);
}

/*-----------------------------------------------------------------------------------------
��������YS_DWServerCmdDeal
����˵����ƽ̨�·����ݴ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWServerCmdDeal(void)
{
	switch(t_AvlSckParase.v_CmdID)
	{
	case DW_UP_CMD_LOGIN_ACK:
		ycsj_debug("DW Get login ack");
		YS_DWLoginAckDeal(t_AvlSckParase.a_dbuf,t_AvlSckParase.v_dlen);
		break;

	case DW_UP_CMD_RSQ_INFO_ACK:
		ycsj_debug("DW Get FileInfo ack");
		YS_DWFileInfoAckDeal(t_AvlSckParase.a_dbuf,t_AvlSckParase.v_dlen);
		break;

	case DW_UP_CMD_RSQ_FILE_ACK:
		YS_DWFileDataInputDeal(t_AvlSckParase.a_dbuf,t_AvlSckParase.v_dlen);
		break;

	case DW_UP_CMD_EXIT_LOG_ACK:

		break;

	case DW_UP_CMD_AUTO_RSQ_ACK:
		YS_AutoInfoRsqAckDeal(t_AvlSckParase.a_dbuf,t_AvlSckParase.v_dlen);
		break;

	default:
		break;
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWAppServerDataInput
����˵����Զ�����أ�ƽ̨���ݰ�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWAppServerDataInput(u8 *dbuf, u16 dlen)
{
	u16 pos,GetLen;

	YS_UartDebugLengError(dbuf,dlen);
	YS_UartDebugInterfacel(INTER_PTL_UPDATE_GPRSREC,dbuf,dlen);
	pos=0;
	while(dlen>0)
	{
		if(YS_DWServerPtlParase(&dbuf[pos],dlen,&GetLen)==TRUE)
		{
			YS_DWServerCmdDeal();
			dlen-=GetLen;
			pos+=GetLen;
		}
		else
		{
			return;
		}
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWFlowCtrlDeal
����˵����Զ�����ؽ������,�������ն���־���ɼ����Ŵ�����������ظ�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWUpdateResultDeal(u8 Rlt)
{
	u8 fbuf[50],SmsBuf[100];
	u8 len,SmsBufLen,i;

	u8 SmsConSucceBuf[]={0xC8, 0xED, 0xBC, 0xFE, 0xC9, 0xFD, 0xBC, 0xB6, 0xB3, 0xC9,
		0xB9, 0xA6, 0xA3, 0xA1, 0xB5, 0xB1, 0xC7, 0xB0, 0xC8, 0xED, 0xBC, 0xFE, 0xB0, 0xE6, 0xB1, 0xBE,
		0xCE, 0xAA, 0xA3, 0xBA};//��������ɹ�����ǰ����汾Ϊ��
	u8 SmsConNoPosBuf[]={0xC8, 0xED, 0xBC, 0xFE, 0xB0, 0xE6, 0xB1, 0xBE, 0xC9, 0xFD, 0xBC, 0xB6, 0xB3,
		0xC9, 0xB9, 0xA6, 0xA3, 0xA1, 0xB5, 0xB1, 0xC7, 0xB0, 0xC8, 0xED, 0xBC, 0xFE, 0xB0, 0xE6, 0xB1,
		0xBE, 0xCE, 0xAA, 0xA3, 0xBA};//��ǰ�����������ɣ����Ժ���������
	u8 SmsConNoUpdateFileBuf[]={0xB7, 0xFE, 0xCE, 0xF1, 0xC6, 0xF7, 0xCE, 0xDE, 0xCF, 0xE0, 0xD3, 0xA6,
		0xB5, 0xC4, 0xC9, 0xFD, 0xBC, 0xB6, 0xCE, 0xC4, 0xBC, 0xFE, 0x21};//����������Ӧ�������ļ�!
	u8 SmsConFailBuf[]={0xC8, 0xED, 0xBC, 0xFE, 0xC9, 0xFD, 0xBC, 0xB6, 0xCA, 0xA7, 0xB0, 0xDC, 0x21};
	//�������ʧ��!

	SmsBufLen=0;
	switch(Rlt)
	{
		case DW_RLT_UPDATE_SUCCESS:
			len=sizeof(SmsConSucceBuf);
			for(i=0;i<len;i++)
			{
				SmsBuf[SmsBufLen]=SmsConSucceBuf[i];
				SmsBufLen++;
			}
			memset(fbuf,0x00,50);
			YS_PrmGetTotalSoftVer(fbuf);
			len = YS_CodeBufRealLen(fbuf,50);
			for(i=0;i<len;i++)
			{
				SmsBuf[SmsBufLen]=fbuf[i];
				SmsBufLen++;
			}
			break;

		case DW_RLT_REG_NET_FAIL:

			break;

		case DW_RLT_NO_POS_FOR_TERI:
			len=sizeof(SmsConNoPosBuf);
			for(i=0;i<len;i++)
			{
				SmsBuf[SmsBufLen]=SmsConNoPosBuf[i];
				SmsBufLen++;
			}
			break;

		case DW_RLT_NO_UPDATE_FILE:
			len=sizeof(SmsConNoUpdateFileBuf);
			for(i=0;i<len;i++)
			{
				SmsBuf[SmsBufLen]=SmsConNoUpdateFileBuf[i];
				SmsBufLen++;
			}
			break;

		case DW_RLT_DOWNFILE_FAIL:
			len=sizeof(SmsConFailBuf);
			for(i=0;i<len;i++)
			{
				SmsBuf[SmsBufLen]=SmsConFailBuf[i];
				SmsBufLen++;
			}
			break;

		default:

			break;
	}
	if(SmsBufLen > 0)
	{
		YS_PrmReadOneItem(FLH_PRM_DW_PHONE,FLH_PRM_DW_PHONE_LEN,fbuf);
		len=YS_CodeBufRealLen(fbuf, FLH_PRM_DW_PHONE_LEN);
		YS_SmsSendInterface(SmsBuf,SmsBufLen, fbuf, len, TRUE);

		memset(fbuf,0x00,50);
		YS_PrmWriteOneItem(FLH_PRM_DW_PHONE,FLH_PRM_DW_PHONE_LEN,fbuf);
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWFlowCtrlDeal
����˵����Զ�����ش�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWFlowCtrlDeal(void)
{
	u8 Rlt,fbuf[20];

	switch(t_SysRunStatus.RunFlow)
	{
	case YS_RUN_FLOW_INIT:
		ycsj_debug("DW Flow begin");
		t_FlowInfo.OptDelay=0;
		t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_REG;
		break;

	case YS_RUN_FLOW_WAIT_REG:

		if(t_SysRunStatus.RegStatus==1)
		{
                	t_FlowInfo.OptDelay=0;
			ycsj_debug("DW Reg ok begin ppp");
                	t_SysRunStatus.RunFlow=YS_RUN_FLOW_PPP_BEIGN;
		}
		else
		{
			t_FlowInfo.OptDelay++;
			if(t_FlowInfo.OptDelay==1)  //��ֹ�ڸ�λǰϵͳ�Ѵ��ڷ���ģʽ
			{
				sjfun_Flight_Mode_Cut(RUN_MODE_NORMAL);
				//YS_RunFlightModeInterface(RUN_MODE_NORMAL);   //��ϵͳת����������ģʽ
			}
			if(t_FlowInfo.OptDelay>=120)  //���2������δע���������
			{
				YS_DWUpdateResultDeal(DW_RLT_REG_NET_FAIL);
				memset(fbuf,0x00,20);
				YS_PrmWriteOneItem(FLH_PRM_DW_PHONE,FLH_PRM_DW_PHONE_LEN,fbuf);
				t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_EXIT;
			}
		}
		break;

	case YS_RUN_FLOW_PPP_BEIGN:
		t_FlowInfo.OptDelay=0;
		t_FlowInfo.AcctID=YS_RunPPPInterFaceDeal();
		ycsj_debug("DW PPP count ID:%d",t_FlowInfo.AcctID);
		t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_PPP;
		break;

	case YS_RUN_FLOW_WAIT_PPP:
		t_FlowInfo.OptDelay++;
		if(t_FlowInfo.OptDelay>=8)
		{
			ycsj_debug("DW App ppp ok begin socket");
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_SOCKET_BEGIN;
		}
		break;

	case YS_RUN_FLOW_SOCKET_BEGIN:
		t_FlowInfo.OptDelay=0;
		t_UpdateManage.SocketOKFlag=0;
		Rlt=YS_DWAppSocketBeing();
		ycsj_debug("DW App Socket rlt:%d",Rlt);
		if(Rlt==0)
		{
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_SOCKET;
		}
		else
		{
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
		}
		break;

	case YS_RUN_FLOW_WAIT_SOCKET:
		 if(t_UpdateManage.SocketOKFlag==1)
		 {
		 	ycsj_debug("DW Socket is ok");
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_SEVLOG_BEGIN;
		 }
		 else
		 {
			t_FlowInfo.OptDelay++;
			if(t_FlowInfo.OptDelay>=120)
			{
				t_UpdateManage.SocketTimes++;
				t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
			}
		 }
		break;

	case YS_RUN_FLOW_SEVLOG_BEGIN:
		ycsj_debug("DW Login server begin");
		t_FlowInfo.LogOKFlag=FALSE;
		t_FlowInfo.LogFullFlag=FALSE;
		t_FlowInfo.LogFailFlag=FALSE;
		t_FlowInfo.LogTimes=0;
		t_FlowInfo.LogDelay=RUN_LOGIN_DELAY_DEF;
		t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_SERVLOG;
		break;

	case YS_RUN_FLOW_WAIT_SERVLOG:
		if(t_FlowInfo.LogOKFlag==TRUE)
		{
			ycsj_debug("DW Login ok");
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_RSQINFO_BEGIN;
		}
		else if(t_FlowInfo.LogFullFlag==TRUE) //ƽ̨��ǰ���������ն�����
		{
			ycsj_debug("DW Login Full");
			YS_DWUpdateResultDeal(DW_RLT_NO_POS_FOR_TERI);
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_EXIT;
		}
		else if(t_FlowInfo.LogFailFlag==TRUE)
		{
			ycsj_debug("DW Login Fail");
			t_UpdateManage.SocketTimes++;
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
		}
		else
		{
			YS_DWLoginServerCtrl();
		}
		break;

	case YS_RUN_FLOW_DW_RSQINFO_BEGIN:
		ycsj_debug("DW Rsq Fileinfo begin");
		t_FlowInfo.RsqFileOKFlag=0;
		t_FlowInfo.RsqFileFailFlag=0;
		t_FlowInfo.RsqFileUnknowFlag=0;
		t_FlowInfo.RsqFileDelay=DW_RSQ_FILE_DELAY;
		t_FlowInfo.RsqFileTimes=0;
		t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_RSQINFO_WAIT;
		break;

	case YS_RUN_FLOW_DW_RSQINFO_WAIT:
		if(t_FlowInfo.RsqFileOKFlag==1)
		{
			if(t_UpdateManage.FirstRsqFlag==1)  //��һ�������־
			{
				t_UpdateManage.FirstRsqFlag=0;
				Rlt=sjfun_DW_Rsq_FileBuf(t_UpdateManage.FileLen);
				ycsj_debug("DW First Info Rsq Memory Rlt:%d..\n",Rlt);
				if(Rlt==1)
				{
					t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_RSQDATA_BEGIN;
				}
				else
				{
					t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_EXIT;
				}
			}
			else
			{
				t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_RSQDATA_BEGIN;
			}
		}
		else if(t_FlowInfo.RsqFileUnknowFlag==1) //ƽ̨�����ڹ��̶�Ӧ�������ļ�
		{
			ycsj_debug("DW file info Rsq Unknowl");
			YS_DWUpdateResultDeal(DW_RLT_NO_UPDATE_FILE);
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_EXIT;
		}
		else if(t_FlowInfo.RsqFileFailFlag==1)
		{
			t_UpdateManage.SocketTimes++;
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
		}
		else
		{
			YS_DWRsqFileInfoCtrl();
		}
		break;

	case YS_RUN_FLOW_DW_RSQDATA_BEGIN:
		ycsj_debug("DW Rsq Group data CurGroup:%d",t_UpdateManage.CurGroupNO);
		t_UpdateManage.RsqGroupOKFlag=0;
		t_UpdateManage.RsqGroupFailFlag=0;
		t_UpdateManage.RsqGroupDelay=DW_RSQ_GROUP_DELAY;
		t_UpdateManage.RsqGroupTimes=0;
		t_DownOneGroup.GroupBufPackNum=0;
		t_DownOneGroup.GroupBufPackWP=0;
		t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_RSQDATA_WAIT;
		break;

	case YS_RUN_FLOW_DW_RSQDATA_WAIT:
		if(t_UpdateManage.RsqGroupOKFlag==1)
		{
			if((t_UpdateManage.TotalGroupNum==t_UpdateManage.CurGroupNum)&&(t_UpdateManage.TotalPackNum==t_UpdateManage.CurPackNum))
			{ //����������
				YS_DWServerSendInterface(DW_UP_CMD_EXIT_LOG,NULL,0);  //��������������ݰ�
				Rlt=sjfun_DW_Calc_FileVerify(t_UpdateManage.FileLen); //�����ļ���������ֵ
				ycsj_debug("DW comp verify v1:%d,v2:%d,v3:%d..",t_UpdateManage.CalcFileVerify,t_UpdateManage.DownFileVerify,Rlt);
				if((t_UpdateManage.CalcFileVerify==t_UpdateManage.DownFileVerify)&&(Rlt==t_UpdateManage.DownFileVerify))
				{
					t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_UPDATE_BEGIN;
				}
				else
				{
					ycsj_debug("DW file down verify error");
					YS_DWUpdateResultDeal(DW_RLT_DOWNFILE_FAIL);
					t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_EXIT;
				}
			}
			else //������һ������
			{
				t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_RSQDATA_BEGIN;
			}
		}
		else if(t_UpdateManage.RsqGroupFailFlag==1)
		{
			t_UpdateManage.SocketTimes++;
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_BEGIN;
		}
		else
		{
			YS_SendGroupCtrl();
		}
		break;

	case YS_RUN_FLOW_DW_UPDATE_BEGIN:
		Rlt=sjfun_DW_Update_File(t_UpdateManage.FileLen);
		ycsj_debug("Dw Update file rlt:%d",Rlt);
		t_FlowInfo.OptDelay=0;
		t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_UPDATE_WAIT;
		break;

	case YS_RUN_FLOW_DW_UPDATE_WAIT:
		t_FlowInfo.OptDelay++;
		if(t_FlowInfo.OptDelay>=3)
		{
			//YS_DWUpdateResultDeal(DW_RLT_UPDATE_SUCCESS);
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_EXIT;
		}
		break;

	case YS_RUN_FLOW_RDCON_BEGIN:
		t_UpdateManage.SocketTimes++;
		if(t_UpdateManage.SocketTimes<=DW_SOCKET_MAX_TIMES)
		{
			ycsj_debug("DW Reconnect begin beign");
			t_FlowInfo.OptDelay=0;
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_RDCON_WAIT;
		}
		else
		{
			ycsj_debug("DW Reconnect Fulltimes exit");
			YS_DWUpdateResultDeal(DW_RLT_DOWNFILE_FAIL);
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_DW_EXIT;
		}
		break;

	case YS_RUN_FLOW_RDCON_WAIT:
		t_FlowInfo.OptDelay++;
		if(t_FlowInfo.OptDelay>=30)
		{
			t_SysRunStatus.RunFlow=YS_RUN_FLOW_WAIT_REG;
			t_FlowInfo.OptDelay++;
		}
		break;

	case YS_RUN_FLOW_DW_EXIT:
		ycsj_debug("DW Exit deal begin");
		fbuf[0]=0;
		YS_PrmWriteOneItem(FLH_PRM_NV_DWFLAG,FLH_PRM_NV_DWFLAG_LEN,fbuf);
		YS_SysRsqSystemRST(YS_RST_FLAG_DOWNLOAD);
		t_FlowInfo.OptDelay=0;
		t_SysRunStatus.RunFlow=YS_RUN_FLOW_RST_WAITING;
		break;

	case YS_RUN_FLOW_RST_WAITING:
		t_FlowInfo.OptDelay++;
		break;

	default:
		break;
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_DWAppInitDeal
����˵������������ƽ̨SOCKET
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_DWAppInitDeal(void)
{
	t_UpdateManage.SocketTimes=0;
	t_UpdateManage.FirstRsqFlag=1;
	t_UpdateManage.CurPackNO=0;
	t_UpdateManage.CurGroupNO=0;
}


