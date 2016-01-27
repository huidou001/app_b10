/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_gSensor.c  2013-06-27
���ļ����ڹ���gSensor �Ŀ���
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"
#define STUDY_TIMER_DELAY		200
#define SEND_INIT_TIMER_DELAY  200
#define STUDY_BEIGN_VALUE		0x32
#define STUDY_ERROR_VALUE		0x64

u8 gsenser_on=1;
const u8 gsenser_EINTnum = 5;

t_gSensor_Manage			t_gSensorManage;
t_gSensor_Chip_Study		t_gChipStudy;


void Set_SCL_Dir(u8 Value)
{
	if(Value==0)
	{
		sjfun_Gpio_Set_Direction(YS_PIN_NO_I2CSCL,0);
	}
	else
	{
		sjfun_Gpio_Set_Direction(YS_PIN_NO_I2CSCL,1);
	}
}

void Set_SDA_Dir(u8 Value)
{
	if(Value==0)
	{
		sjfun_Gpio_Set_Direction(YS_PIN_NO_I2CDAT,0);
	}
	else
	{
		sjfun_Gpio_Set_Direction(YS_PIN_NO_I2CDAT,1);
	}
}

void Set_SCL_Data(u8 Value)
{
	sjfun_Gpio_Write_Value(YS_PIN_NO_I2CSCL,Value);
}

void Set_SDA_Data(u8 Value)
{
	sjfun_Gpio_Write_Value(YS_PIN_NO_I2CDAT,Value);
}

u8 Get_SDA_Data(void)
{
	u8 rlt;
	rlt=sjfun_Gpio_Read_Value(YS_PIN_NO_I2CDAT);
	return(rlt);
}
void Start_I2C(void)
{
	Set_SCL_Dir(0);
	Set_SDA_Dir(0);
	Set_SDA_Data(1);
	Set_SCL_Data(1);
	Set_SDA_Data(0);
	Set_SCL_Data(0);
}

void Stop_I2C(void)
{
	Set_SCL_Dir(0);
	Set_SDA_Dir(0);
	Set_SDA_Data(0);
	Set_SCL_Data(1);
	Set_SDA_Data(1);
}
unsigned char Read()
{
	u8 data_in=0,i;

	Set_SDA_Dir(1);
	Set_SCL_Data(0);

	for(i=0;i<8;i++)  //ѭ������
	{
		Set_SCL_Data(1);
		data_in <<= 1;
		if(Get_SDA_Data() == 1)
		{
			data_in |= 0x01;
		}
		else
		{
			data_in &= 0xFE;
		}
		Set_SCL_Data(0);
	}
	Set_SDA_Dir(0);
	return data_in;
}

unsigned char Write( unsigned char data_out )
{
	u8 i;
	u8 AckStatus;

	Set_SDA_Dir(0);
	for(i=0;i<8;i++)          //ѭ������
	{
		if(data_out & 0x80)
		{
			Set_SDA_Data(1);
		}
		else
		{
			Set_SDA_Data(0);
		}
		Set_SCL_Data(1);
		Set_SCL_Data(0);
		data_out <<= 1;
	}

	Set_SDA_Dir(1);
	Set_SCL_Data(1);

	if(Get_SDA_Data() == 1)
	{
		AckStatus = 0;
	}
	else
	{
		AckStatus = 1;    //�յ�Ӧ���ź�
	}
	Set_SCL_Data(0);
	return AckStatus;
}

/*-----------------------------------------------------------------------------------------
��������Simu_WriteI2C
����˵����������ָ����ַд��һ���ֽ�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
unsigned char Simu_WriteI2C(unsigned char address ,unsigned char data)
{
	Start_I2C();

	if(Write(KXSD9_WRITE) == 0)
	{
		return 4;
	}

	if(Write(address) == 0)
	{
		return 5;
	}

	if(Write(data) == 0)
	{
		return 6;
	}
	Stop_I2C();

	return 0;
}

/*-----------------------------------------------------------------------------------------
��������Simu_ReadI2C
����˵������ȡ�����ϵ�һ��ָ����ַ������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
unsigned int Simu_ReadI2C(unsigned char address)
{
	unsigned int RecvData = 0;
	Start_I2C();

	if(Write(KXSD9_WRITE) == 0)
	{
		return 1;
	}

	if(Write(address) == 0)
	{
		return 2;
	}
	Start_I2C();

	if(0==Write(KXSD9_READ))
	{
		return 4;
	}
	RecvData = Read();
	Stop_I2C();
	return RecvData;
}


/*-----------------------------------------------------------------------------------------
��������YS_GSensorWorkStatusInit
����˵��������оƬ�񶯼���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GSensorWorkStatusInit(void)
{
	u8 rlt,BaseValue;
	u8 fbuf[2];

	YS_PrmReadOneItem(FLH_PRM_VIB_BASEVALUE,FLH_PRM_VIB_BASEVALUE_LEN,fbuf);
	BaseValue=fbuf[0];
	YS_PrmReadOneItem(FLH_PRM_VIB_LEVEL,FLH_PRM_VIB_LEVEL_LEN,fbuf);
	if(fbuf[0]> 0x0f)
	{
		fbuf[0] = 0x0f;
	}
	Simu_WriteI2C(SHAKE_THRESHOLD,BaseValue+fbuf[0]*2);		//����ֵ����70+VibLevel*2
	rlt=Simu_ReadI2C(SHAKE_THRESHOLD);
}

/*----------------------------------------------------------------------------------------
��������YS_GSensorInitChip
����˵������ʼ��оƬ��������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GSensorInitChip(void)
{
	Simu_WriteI2C(INTERRUPT_MODE,INTERRUPT_MODE_VALUE);   		//ѡ���ж�ģʽ
	Simu_WriteI2C(INTERRUPT_MODE,INTERRUPT_MODE_VALUE);   		//ѡ���ж�ģʽ
	Simu_WriteI2C(INTERRUPT_MODE,INTERRUPT_MODE_VALUE);   		//ѡ���ж�ģʽ
	Simu_WriteI2C(INT1_MAP,INT1_MAP_VALUE);				 		//ӳ��ܽ�
	Simu_WriteI2C(G_RANG_SEL,G_RANG_SEL_VAL2);					//���÷�Χ
	Simu_WriteI2C(INTERRUPT_ENGINES0,INTERRUTP_ENGINES0_VALUE);//�ж�ʹ��
	Simu_WriteI2C(INTERRUPT_ENGINES1,INTERRUTP_ENGINES1_VALUE);
	Simu_WriteI2C(INTERRUPT_ENGINES2,INTERRUTP_ENGINES2_VALUE);
	Simu_WriteI2C(HIGH_G_DELAY,HIGH_G_DELAY_VALUE);			//�����ж�ά��ʱ�䣺32ms
	YS_GSensorWorkStatusInit();
}

/*-----------------------------------------------------------------------------------------
��������YS_GSensorTest
����˵��������250�Ƿ���������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
bool YS_GSensorTest(void)
{
	u8 Rlt;

	Rlt=Simu_ReadI2C(0x00);
	if(Rlt == 0XF9)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_OpenGsenserEint
����˵��������BM250�жϼ���ź�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GsenserInitHandler(void)
{
	u8 Rlt;
//	EINT_Mask(gsenser_EINTnum);
    YS_GprsDebugString("have a interrupt!", gsenser_on);
	if(gsenser_on==1)  //�ߵ�ƽ�л����͵�ƽ
	{
		Rlt=Simu_ReadI2C(0x09);
		if((Rlt&0x02)==0x02)  //���Ϊ�񶯴����ж�
		{
			YS_IODealVibSwitchInput(1);
			t_gSensorManage.VibSignDelay=0;		//��������ۼ�ʱ��
			t_gChipStudy.VibSignFlag=1;
		}
		gsenser_on = !gsenser_on;
		sjfun_Gpio_EINT_SetPolarity(gsenser_EINTnum,gsenser_on);
		sjfun_Gpio_EINT_SetDoubleTime(gsenser_EINTnum,1);
	}
	else //�͵�ƽ�л����ߵ�ƽ
	{
		Rlt=Simu_ReadI2C(0x09);
		if((Rlt&0x02)==0x02)
		{
			YS_IODealVibSwitchInput(0);
			t_gSensorManage.VibSignDelay=0;		//��������ۼ�ʱ��
			t_gChipStudy.VibSignFlag=1;
		}
		gsenser_on = !gsenser_on;
		sjfun_Gpio_EINT_SetPolarity(gsenser_EINTnum,gsenser_on);
		sjfun_Gpio_EINT_SetDoubleTime(gsenser_EINTnum,1);//5
	}
//	EINT_UnMask(gsenser_EINTnum);
}

/*-----------------------------------------------------------------------------------------
��������YS_GsenserAutoGetVibSign
����˵������ʱ��ȡоƬ���жϼĴ������ж��Ƿ�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GsenserAutoGetVibSign(void)
{
	u8 Rlt;
	u8 reg0f,reg10,reg11,reg12,reg26;

	Rlt=Simu_ReadI2C(0x09);
	reg0f=Simu_ReadI2C(0x0F);
	reg10=Simu_ReadI2C(0x10);
	reg11=Simu_ReadI2C(0x11);
	reg12=Simu_ReadI2C(0x12);
	reg26=Simu_ReadI2C(0x26);

	if((Rlt&0x02)==0x02)
	{
		YS_IODealVibSwitchInput(1);
		YS_GSensorWorkStatusInit();
	}
}

/*-----------------------------------------------------------------------------------------
��������YS_OpenGsenserEint
����˵����250�����жϼ���ź�
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_OpenGsenserEint(void)
{
   	    sjfun_Gpio_Set_Mode(YS_PIN_NO_GINT, 1);
    	sjfun_Gpio_Set_Direction(YS_PIN_NO_GINT,1); //input
    	sjfun_Gpio_Set_PullEN(YS_PIN_NO_GINT,0);
    	sjfun_Gpio_Set_PullValue(YS_PIN_NO_GINT,0);
    	sjfun_Gpio_EINT_RegInt(gsenser_EINTnum,gsenser_on,YS_GsenserInitHandler);
}


void YS_GSensorSetRegExtern(u8 RegName, u8 Value)
{
	Simu_WriteI2C(RegName,Value);
}

u8 YS_GSensorReadRegExtern(u8 RegName)
{
	u8 rlt;

	rlt=Simu_ReadI2C(RegName);
	return(rlt);
}

/*-----------------------------------------------------------------------------------------
��������
����˵�����񶯴�������ʼ�����̿���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GSensorInitFlow(void)
{
	switch(t_gSensorManage.ActDelay)
	{
	case 0: //�ض�оƬ��Դ
		sjfun_Gpio_Write_Value(YS_PIN_NO_GPOW,0);
		sjfun_Gpio_Write_Value(YS_PIN_NO_I2CSCL,0);
		sjfun_Gpio_Write_Value(YS_PIN_NO_I2CDAT,0);
		break;

	case 2: //����оƬ��Դ
		sjfun_Gpio_Write_Value(YS_PIN_NO_I2CSCL,1);
		sjfun_Gpio_Write_Value(YS_PIN_NO_I2CDAT,1);
		sjfun_Gpio_Write_Value(YS_PIN_NO_GPOW,1);
		break;

	case 4: //��ʼ��оƬ
		YS_GSensorInitChip();
		break;

	case 10: //��ʼ�����״̬
		t_gSensorManage.SignEffecFlag=1;
		break;

	default:
		break;
	}

	t_gSensorManage.ActDelay++;
	if(t_gSensorManage.ActDelay<=10)
	{
		sjfun_timer(GIS_TIMER_ID_7,STUDY_TIMER_DELAY,YS_GSensorInitFlow);
	}
}

/*-----------------------------------------------------------------------------------------
��������
����˵�����񶯴�����оƬ��ʼ�����
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GSensorInitInterface(void)
{
	sjfun_stoptimer(GIS_TIMER_ID_7);
	sjfun_timer(GIS_TIMER_ID_7,STUDY_TIMER_DELAY,YS_GSensorInitFlow);
	t_gSensorManage.VibSignDelay=0;
	t_gSensorManage.ActDelay=0;
	t_gSensorManage.SignEffecFlag=0;
}

/*-----------------------------------------------------------------------------------------
��������
����˵������ʱ���250�Ƿ����������� �������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GSensorRunMoni(void)
{
	t_gSensorManage.VibSignDelay++;			//ÿ3 ������������źţ����³�ʼ���񶯴�����
	if(t_gSensorManage.VibSignDelay>=180)
	{
		t_gSensorManage.VibSignDelay=0;
		YS_GSensorInitInterface();
	}
}

/*-----------------------------------------------------------------------------------------
��������
����˵��������250�Ƿ���������
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GSensorInitDeal(void)
{
	t_gSensorManage.ActDelay=0;
	t_gSensorManage.VibSignDelay=0;
	t_gSensorManage.SignEffecFlag=0;

	sjfun_Gpio_Set_Mode(YS_PIN_NO_GPOW,0);  		//�򿪵�Դ����
	sjfun_Gpio_Set_Direction(YS_PIN_NO_GPOW,0);
	sjfun_Gpio_Write_Value(YS_PIN_NO_GPOW,1);

	sjfun_Gpio_Set_Mode(YS_PIN_NO_I2CSCL, 0);		//����I2CSCL ��Ϊ��ͨIO
	sjfun_Gpio_Set_Mode(YS_PIN_NO_I2CDAT, 0);		//����I2CSCL ��Ϊ��ͨIO

	YS_GSensorInitChip();
	YS_OpenGsenserEint();
}

void YS_GSensorRunTotal(void)
{
	YS_OpenGsenserEint();
	YS_GSensorInitInterface();
}

void YS_GSensorRunSource(void)
{
	YS_GSensorInitInterface();
}

void YS_GSensorIntIRQ(void)
{
	YS_OpenGsenserEint();
}

/*-----------------------------------------------------------------------------------------
��������
����˵�����񶯴���������ѧϰ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GSensorChipStatusDeal(void)
{
	u8 	ReadValue;
	u8	OutBuf[12];
	u8	fbuf[2];

	switch(t_gChipStudy.StudyFlow)
	{
	case GSTUDY_FLOW_INIT:  //����ʼ��
		t_gChipStudy.StudyValue=STUDY_BEIGN_VALUE;
		t_gChipStudy.StudyFlow=GSTUDY_FLOW_CHIP_CHECK;
		break;

	case GSTUDY_FLOW_CHIP_CHECK: //оƬ���
		Simu_WriteI2C(G_RANG_SEL,G_RANG_SEL_VAL2);				//���÷�Χ
		ReadValue=Simu_ReadI2C(G_RANG_SEL);
		if(ReadValue!=G_RANG_SEL_VAL2) //оƬͨ���쳣
		{
			OutBuf[0]=1;
			OutBuf[1]=0;
			YS_UartDataOutInterfacel(INTER_ACK_GSENSOR_STUDY,OutBuf,2);
			return;
		}
		else //оƬͨ��������������һ��⻷��
		{
			OutBuf[0]=1;
			OutBuf[1]=1;
			YS_UartDataOutInterfacel(INTER_ACK_GSENSOR_STUDY,OutBuf,2);
			t_gChipStudy.StudyFlow=GSTUDY_FLOW_VIB_CHECK;
		}
		break;

	case GSTUDY_FLOW_VIB_CHECK:
		if(t_gChipStudy.StudyValue<STUDY_ERROR_VALUE) //С���趨���ޣ���������
		{
			Simu_WriteI2C(SHAKE_THRESHOLD,t_gChipStudy.StudyValue);
			t_gChipStudy.VibSignFlag=0;
			t_gChipStudy.CheckCount=0;
			t_gChipStudy.CheckDelay=0;
			t_gChipStudy.StudyFlow=GSTUDY_FLOW_VIB_CHECK_WAIT;
		}
		else //�������ֵ�����趨���ޣ����س�����Ϣ
		{
			OutBuf[0]=2;
			OutBuf[1]=0;
			OutBuf[2]=t_gChipStudy.StudyValue;
			YS_UartDataOutInterfacel(INTER_ACK_GSENSOR_STUDY,OutBuf,3);
			return;
		}
		break;

	case GSTUDY_FLOW_VIB_CHECK_WAIT:
		t_gChipStudy.CheckDelay++;
		if(t_gChipStudy.CheckDelay>=4)
		{
			OutBuf[0]=3;
			OutBuf[1]=t_gChipStudy.StudyValue;
			OutBuf[2]=t_gChipStudy.CheckCount;
			YS_UartDataOutInterfacel(INTER_ACK_GSENSOR_STUDY,OutBuf,3);
			if(t_gChipStudy.CheckCount>=1)  //�����⵽���ź�,�ۼ��񶯷�ֵ���������
			{
				t_gChipStudy.StudyValue+=2;
				t_gChipStudy.StudyFlow=GSTUDY_FLOW_VIB_CHECK;
			}
			else
			{
				fbuf[0]=t_gChipStudy.StudyValue; 	//���������񶯻���
				YS_PrmWriteOneItem(FLH_PRM_VIB_BASEVALUE,FLH_PRM_VIB_BASEVALUE_LEN,fbuf);
				YS_PrmQuickWriteNV();
				YS_GSensorWorkStatusInit();		//���õ�ǰ���񶯼���
				OutBuf[0]=2;
				OutBuf[1]=1;
				OutBuf[2]=t_gChipStudy.StudyValue;
				YS_UartDataOutInterfacel(INTER_ACK_GSENSOR_STUDY,OutBuf,3);
				return;
			}
		}
		else
		{
			if(t_gChipStudy.VibSignFlag==1)
			{
				t_gChipStudy.CheckCount++;
				t_gChipStudy.VibSignFlag=0;
			}
		}
		break;

	case GSTUDY_FLOW_EXIT:
	default:
		return;
		break;
	}
	sjfun_timer(GIS_TIMER_ID_6,STUDY_TIMER_DELAY,YS_GSensorChipStatusDeal);
}


/*-----------------------------------------------------------------------------------------
��������
����˵�����񶯴���������ѧϰ���
�޸ļ�¼��
-------------------------------------------------------------------------------------------*/
void YS_GSensorStudyInterface(void)
{
	t_gChipStudy.StudyFlow=GSTUDY_FLOW_INIT;
	sjfun_stoptimer(GIS_TIMER_ID_6);
	sjfun_timer(GIS_TIMER_ID_6,STUDY_TIMER_DELAY,YS_GSensorChipStatusDeal);
}


