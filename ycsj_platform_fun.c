#include "YS_Public.h"
//��ʽ����ӡ����
 typedef void (*_ycsj_debug_t)(const char* format, ...);
 _ycsj_debug_t _ycsj_debug = NULL;
 void ycsj_debug(const char* format, ...)
{
	char tempstr[200];
	va_list args;

	memset(tempstr, 0, sizeof(tempstr));
	va_start(args, format);
	_vsnprintf(tempstr, sizeof(tempstr), format, args);
	va_end(args);

	if(NULL == _ycsj_debug)
		_ycsj_debug = (_ycsj_debug_t)jt_get_sym_entry("ycsj_debug");

	if(NULL != _ycsj_debug)
		_ycsj_debug(tempstr);
}

//��ֵ��ӡ����
typedef void(* p_sjfun_debug_t)(char *DebugStr, int Value);
p_sjfun_debug_t p_sjfun_debug=NULL;
void sjfun_debug(char *DebugStr, int Value)
{
	if(NULL == p_sjfun_debug)
	{
		p_sjfun_debug = (p_sjfun_debug_t)jt_get_sym_entry("sjfun_debug");
	}

	if(NULL!=p_sjfun_debug)
	{
		p_sjfun_debug(DebugStr,Value);
	}
}


//��ʱ��������ں���
typedef void(* p_sjfun_timer_t)(U16 timerid, U32 delay, FuncPtr funcPtr);
p_sjfun_timer_t p_sjfun_timer=NULL;
void sjfun_timer(U16 timerid, U32 delay, FuncPtr funcPtr)
{
	if(NULL==p_sjfun_timer)
	{
		p_sjfun_timer = (p_sjfun_timer_t)jt_get_sym_entry("sjfun_timer");
	}

	if(NULL!=p_sjfun_timer)
	{
		p_sjfun_timer(timerid,delay,funcPtr);
	}
}

//���Դ������ݷ��ͽӿ�
typedef void(* p_sjfun_Debug_Uart_send_t)(U8 *dbuf, U16 dlen);
p_sjfun_Debug_Uart_send_t p_sjfun_Debug_Uart_send=NULL;
void sjfun_Debug_Uart_send(U8 *dbuf, U16 dlen)
{
	if(NULL==p_sjfun_Debug_Uart_send)
	{
		p_sjfun_Debug_Uart_send = (p_sjfun_Debug_Uart_send_t)jt_get_sym_entry("Debug_Uart_send");
	}

	if(NULL!=p_sjfun_Debug_Uart_send)
	{
		p_sjfun_Debug_Uart_send(dbuf,dlen);
	}
}

//������ȡ�ӿ�

typedef void(* p_sjfun_param_read_t)(u16 PrmID, u16 dlen, u8 *FillBuf );
p_sjfun_param_read_t p_sjfun_param_read=NULL;
void sjfun_param_read(u16 PrmID, u16 dlen, u8 *FillBuf)
{
	if(NULL==p_sjfun_param_read)
	{
		p_sjfun_param_read = (p_sjfun_param_read_t)jt_get_sym_entry("param_read");
	}

	if(NULL!=p_sjfun_param_read)
	{
		p_sjfun_param_read(PrmID,dlen,FillBuf);
	}
}

//�������ýӿ�

typedef void(* p_sjfun_param_write_t)(u16 PrmID, u16 dlen, u8 *dbuf );
p_sjfun_param_write_t p_sjfun_param_write=NULL;
void sjfun_param_write(u16 PrmID, u16 dlen, u8 *dbuf )
{
	if(NULL==p_sjfun_param_write)
	{
		p_sjfun_param_write = (p_sjfun_param_write_t)jt_get_sym_entry("param_write");
	}

	if(NULL!=p_sjfun_param_write)
	{
		p_sjfun_param_write(PrmID,dlen,dbuf);
	}
}

//��������д��ӿ�
typedef void(* p_sjfun_param_Quickwr_t)(void );
p_sjfun_param_Quickwr_t p_sjfun_param_Quickwr=NULL;
void sjfun_param_Quickwr(void)
{
	if(NULL==p_sjfun_param_Quickwr)
	{
		p_sjfun_param_Quickwr = (p_sjfun_param_Quickwr_t)jt_get_sym_entry("param_Quickwr");
	}

	if(NULL!=p_sjfun_param_Quickwr)
	{
		p_sjfun_param_Quickwr();
	}
}

//����GPS LED ��ʾ
 typedef void (*p_sjfun_Set_GpsLed_t)(u8 Status);
p_sjfun_Set_GpsLed_t  p_sjfun_Set_GpsLed=NULL;
void sjfun_Set_GpsLed(u8 Status)
{
	if(NULL==p_sjfun_Set_GpsLed)
	{
		p_sjfun_Set_GpsLed = (p_sjfun_Set_GpsLed_t)jt_get_sym_entry("led_gps");
	}

	if(NULL!=p_sjfun_Set_GpsLed)
	{
		p_sjfun_Set_GpsLed(Status);
	}
}

//����GSM LED ��ʾ
 typedef void (*p_sjfun_Set_GsmLed_t)(u8 Status);
p_sjfun_Set_GsmLed_t  p_sjfun_Set_GsmLed=NULL;
void sjfun_Set_GsmLed(u8 Status)
{

	if(NULL==p_sjfun_Set_GsmLed)
	{
		p_sjfun_Set_GsmLed = (p_sjfun_Set_GsmLed_t)jt_get_sym_entry("led_gms");
	}

	if(NULL!=p_sjfun_Set_GsmLed)
	{
		p_sjfun_Set_GsmLed(Status);
	}
}

//��ȡϵͳ�汾��
 typedef int (*_get_module_ver_t)(unsigned char *p_buf,int len_buf);
_get_module_ver_t _get_module_ver = NULL;
int get_module_ver(unsigned char *p_buf,int len_buf)
{
	if(NULL == _get_module_ver)
		_get_module_ver = (_get_module_ver_t)jt_get_sym_entry("get_module_ver");

	if(NULL != _get_module_ver)
	   return _get_module_ver(p_buf,len_buf);
	return 0;
}

//��ȡϵͳ״̬����
 typedef void * (*p_sjfun_Get_SysVar_t)(char * p_name_variable);
p_sjfun_Get_SysVar_t  p_sjfun_Get_SysVar = NULL;
void *sjfun_Get_SysVar(char * p_name_variable)
{
	if(NULL == p_sjfun_Get_SysVar)
		p_sjfun_Get_SysVar = (p_sjfun_Get_SysVar_t)jt_get_sym_entry("get_sys_var");

	if(NULL != p_sjfun_Get_SysVar)
	   return p_sjfun_Get_SysVar(p_name_variable);
	return 0;
}


//��ȡϵͳ״̬����
 typedef U32 (*p_sjfun_PPP_Interface_t)(char *apn, char *ApnUser, char *ApnPassWord);
p_sjfun_PPP_Interface_t  p_sjfun_PPP_Interface = NULL;
U32 sjfun_PPP_Interface(char *apn, char *ApnUser, char *ApnPassWord)
{
	if(NULL == p_sjfun_PPP_Interface)
		p_sjfun_PPP_Interface = (p_sjfun_PPP_Interface_t)jt_get_sym_entry("PPP_Interface");

	if(NULL != p_sjfun_PPP_Interface)
	   return p_sjfun_PPP_Interface(apn, ApnUser,ApnPassWord);
	return 0;
}

//����SOCKET
 typedef u8 (*p_sjfun_Socket_Create_t)(U32 AcctID, U8 *SocketID, U8 *IPBuf, U16 Port, U8 SocketType);
p_sjfun_Socket_Create_t p_sjfun_Socket_Create=NULL;
u8 sjfun_Socket_Create(U32 AcctID, U8 *SocketID, U8 *IPBuf, U16 Port, U8 SocketType)
{
	if(NULL == p_sjfun_Socket_Create)
		p_sjfun_Socket_Create = (p_sjfun_Socket_Create_t)jt_get_sym_entry("Socket_Create");

	if(NULL != p_sjfun_Socket_Create)
	   return p_sjfun_Socket_Create(AcctID, SocketID, IPBuf, Port, SocketType);
	return 0;
}

//SOCKET ��������
 typedef U32 (*p_sjfun_Socket_Send_t)(U8 SocketID, U8 *dbuf, U16 dlen);
p_sjfun_Socket_Send_t p_sjfun_Socket_Send=NULL;
U32 sjfun_Socket_Send(U8 SocketID, U8 *dbuf, U16 dlen)
{
	if(NULL == p_sjfun_Socket_Send)
		p_sjfun_Socket_Send = (p_sjfun_Socket_Send_t)jt_get_sym_entry("Socket_Send");

	if(NULL != p_sjfun_Socket_Send)
	   return p_sjfun_Socket_Send(SocketID, dbuf,dlen);
	return 0;
}

//SOCKET �ر�
 typedef void (*p_sjfun_Socket_Close_t)(U8 SocketID);
p_sjfun_Socket_Close_t p_sjfun_Socket_Close=NULL;
void sjfun_Socket_Close(U8 SocketID)
{
	if(NULL == p_sjfun_Socket_Close)
		p_sjfun_Socket_Close = (p_sjfun_Socket_Close_t)jt_get_sym_entry("Socket_Close");

	if(NULL != p_sjfun_Socket_Close)
		p_sjfun_Socket_Close(SocketID);
}


//ϵͳ��λ����
 typedef void (*p_sjfun_System_Assert_t)(void);
p_sjfun_System_Assert_t p_sjfun_System_Assert=NULL;
void sjfun_System_Assert(void)
{
	if(NULL == p_sjfun_System_Assert)
		p_sjfun_System_Assert = (p_sjfun_System_Assert_t)jt_get_sym_entry("System_Assert_Rst");

	if(NULL != p_sjfun_System_Assert)
	{
		ycsj_debug("App Call System rstfun");
		p_sjfun_System_Assert();
	}
}

//�������������ļ�������
 typedef U8 (*p_sjfun_DW_Rsq_FileBuf_t)(U32 FileSize);
p_sjfun_DW_Rsq_FileBuf_t p_sjfun_DW_Rsq_FileBuf=NULL;
U8 sjfun_DW_Rsq_FileBuf(U32 FileSize)
{
	if(NULL == p_sjfun_DW_Rsq_FileBuf)
		p_sjfun_DW_Rsq_FileBuf = (p_sjfun_DW_Rsq_FileBuf_t)jt_get_sym_entry("DW_Rsq_Memory");

	if(NULL != p_sjfun_DW_Rsq_FileBuf)
	   return p_sjfun_DW_Rsq_FileBuf(FileSize);
	return 0xFF;
}

//��������д�ļ����ݰ�
 typedef U8 (*p_sjfun_DW_Write_PackData_t)(U32 FilePos, U8 *dbuf, U16 dlen);
p_sjfun_DW_Write_PackData_t p_sjfun_DW_Write_PackData=NULL;
U8 sjfun_DW_Write_PackData(U32 FilePos, U8 *dbuf, U16 dlen)
{
	if(NULL == p_sjfun_DW_Write_PackData)
	p_sjfun_DW_Write_PackData = (p_sjfun_DW_Write_PackData_t)jt_get_sym_entry("DW_Write_Pack");

	if(NULL != p_sjfun_DW_Write_PackData)
	   return p_sjfun_DW_Write_PackData(FilePos,dbuf,dlen);
	return 0xFF;
}


//�����������ļ����ݰ�
 typedef U8 (*p_sjfun_DW_Read_PackData_t)(U32 FilePos, U8 *dbuf, U16 dlen);
p_sjfun_DW_Read_PackData_t p_sjfun_DW_Read_PackData=NULL;
U8 sjfun_DW_Read_PackData(U32 FilePos, U8 *dbuf, U16 dlen)
{
	if(NULL == p_sjfun_DW_Read_PackData)
	p_sjfun_DW_Read_PackData = (p_sjfun_DW_Read_PackData_t)jt_get_sym_entry("DW_Read_Pack");

	if(NULL != p_sjfun_DW_Read_PackData)
	   return p_sjfun_DW_Read_PackData(FilePos,dbuf,dlen);
	return 0xFF;
}

//�������������ļ�������У��
 typedef U8 (*p_sjfun_DW_Calc_FileVerify_t)(U32 FileSize);
p_sjfun_DW_Calc_FileVerify_t p_sjfun_DW_Calc_FileVerify=NULL;
U8 sjfun_DW_Calc_FileVerify(U32 FileSize)
{
	if(NULL == p_sjfun_DW_Calc_FileVerify)
	p_sjfun_DW_Calc_FileVerify = (p_sjfun_DW_Calc_FileVerify_t)jt_get_sym_entry("DW_Calc_Verify");

	if(NULL != p_sjfun_DW_Calc_FileVerify)
	   return p_sjfun_DW_Calc_FileVerify(FileSize);
	return 0xFF;
}

//������������Ӧ�ó���
 typedef U8 (*p_sjfun_DW_Update_File_t)(U32 FileSize);
p_sjfun_DW_Update_File_t p_sjfun_DW_Update_File=NULL;
U8 sjfun_DW_Update_File(U32 FileSize)
{
	if(NULL == p_sjfun_DW_Update_File)
	p_sjfun_DW_Update_File = (p_sjfun_DW_Update_File_t)jt_get_sym_entry("DW_Update_File");

	if(NULL != p_sjfun_DW_Update_File)
	   return p_sjfun_DW_Update_File(FileSize);
	return 0xFF;
}

//����ϵͳ���ߺ���
 typedef void (*p_sjfun_Sys_Sleep_Enable_t)(void);
p_sjfun_Sys_Sleep_Enable_t p_sjfun_Sys_Sleep_Enable=NULL;
void sjfun_Sys_Sleep_Enable(void)
{
	if(NULL == p_sjfun_Sys_Sleep_Enable)
	p_sjfun_Sys_Sleep_Enable = (p_sjfun_Sys_Sleep_Enable_t)jt_get_sym_entry("Sys_Sleep_Enable");

	if(NULL != p_sjfun_Sys_Sleep_Enable)
	  p_sjfun_Sys_Sleep_Enable();
}

//��ֹϵͳ���ߺ���
 typedef void (*p_sjfun_Sys_Sleep_Disable_t)(void);
p_sjfun_Sys_Sleep_Disable_t p_sjfun_Sys_Sleep_Disable=NULL;
void sjfun_Sys_Sleep_Disable(void)
{
	if(NULL == p_sjfun_Sys_Sleep_Disable)
	p_sjfun_Sys_Sleep_Disable = (p_sjfun_Sys_Sleep_Disable_t)jt_get_sym_entry("Sys_Sleep_Disable");

	if(NULL != p_sjfun_Sys_Sleep_Disable)
	   p_sjfun_Sys_Sleep_Disable();
}

//����ģʽ�л�����
 typedef U8 (*p_sjfun_Flight_Mode_Cut_t)(u8 ID);
p_sjfun_Flight_Mode_Cut_t p_sjfun_Flight_Mode_Cut=NULL;
u8 sjfun_Flight_Mode_Cut(u8 ID)
{
	if(NULL == p_sjfun_Flight_Mode_Cut)
	p_sjfun_Flight_Mode_Cut = (p_sjfun_Flight_Mode_Cut_t)jt_get_sym_entry("Sys_Flight_Cut");

	if(NULL != p_sjfun_Flight_Mode_Cut)
	 return  p_sjfun_Flight_Mode_Cut(ID);
	return 0;
}

//���ŷ��ͺ���
 typedef U8 (*p_sjfun_Sms_Send_Interface_t)(u8 * pNumber, u8 pNumberLen, u8* pContent, u16 pContentLen,bool FormChangeFlag);
p_sjfun_Sms_Send_Interface_t p_sjfun_Sms_Send_Interface=NULL;
u8 sjfun_Sms_Send_Interface(u8 * pNumber, u8 pNumberLen, u8* pContent, u16 pContentLen,bool FormChangeFlag)
{
	if(NULL == p_sjfun_Sms_Send_Interface)
	p_sjfun_Sms_Send_Interface = (p_sjfun_Sms_Send_Interface_t)jt_get_sym_entry("Sms_Send_Interface");

	if(NULL != p_sjfun_Sms_Send_Interface)
	 return  p_sjfun_Sms_Send_Interface(pNumber,pNumberLen,pContent,pContentLen,FormChangeFlag);
	 return 0;
}

//���ñ���״̬
 typedef void (*p_sjfun_Set_Isink_Status_t)(u8 ID, u8 Status);
p_sjfun_Set_Isink_Status_t p_sjfun_Set_Isink_Status=NULL;
void sjfun_Set_Isink_Status(u8 ID, u8 Status)
{
	if(NULL == p_sjfun_Set_Isink_Status)
	p_sjfun_Set_Isink_Status = (p_sjfun_Set_Isink_Status_t)jt_get_sym_entry("Isink_SetStatus");

	if(NULL != p_sjfun_Set_Isink_Status)
	   p_sjfun_Set_Isink_Status(ID,Status);
}

//GPIO����ģʽ
 typedef void (*p_sjfun_Gpio_Set_Mode_t)(u16 GpioID, u8 Value);
p_sjfun_Gpio_Set_Mode_t p_sjfun_Gpio_Set_Mode=NULL;
void sjfun_Gpio_Set_Mode(u16 GpioID, u8 Value)
{
	if(NULL == p_sjfun_Gpio_Set_Mode)
	p_sjfun_Gpio_Set_Mode = (p_sjfun_Gpio_Set_Mode_t)jt_get_sym_entry("Gpio_SetMode");

	if(NULL != p_sjfun_Gpio_Set_Mode)
	   p_sjfun_Gpio_Set_Mode(GpioID,Value);
}

//GPIO���÷���
 typedef void (*p_sjfun_Gpio_Set_Direction_t)(u16 GpioID, u8 Value);
p_sjfun_Gpio_Set_Direction_t p_sjfun_Gpio_Set_Direction=NULL;
void sjfun_Gpio_Set_Direction(u16 GpioID, u8 Value)
{
	if(NULL == p_sjfun_Gpio_Set_Direction)
	p_sjfun_Gpio_Set_Direction = (p_sjfun_Gpio_Set_Direction_t)jt_get_sym_entry("Gpio_SetDirection");

	if(NULL != p_sjfun_Gpio_Set_Direction)
	   p_sjfun_Gpio_Set_Direction(GpioID,Value);
}

//GPIO��������ʹ��
 typedef void (*p_sjfun_Gpio_Set_PullEN_t)(u16 GpioID, u8 Value);
p_sjfun_Gpio_Set_PullEN_t p_sjfun_Gpio_Set_PullEN=NULL;
void sjfun_Gpio_Set_PullEN(u16 GpioID, u8 Value)
{
	if(NULL == p_sjfun_Gpio_Set_PullEN)
	p_sjfun_Gpio_Set_PullEN = (p_sjfun_Gpio_Set_PullEN_t)jt_get_sym_entry("Gpio_SetPullEN");

	if(NULL != p_sjfun_Gpio_Set_PullEN)
	   p_sjfun_Gpio_Set_PullEN(GpioID,Value);
}

//GPIO��������״̬
 typedef void (*p_sjfun_Gpio_Set_PullValue_t)(u16 GpioID, u8 Value);
p_sjfun_Gpio_Set_PullValue_t p_sjfun_Gpio_Set_PullValue=NULL;
void sjfun_Gpio_Set_PullValue(u16 GpioID, u8 Value)
{
	if(NULL == p_sjfun_Gpio_Set_PullValue)
	p_sjfun_Gpio_Set_PullValue = (p_sjfun_Gpio_Set_PullValue_t)jt_get_sym_entry("Gpio_SetPullValue");

	if(NULL != p_sjfun_Gpio_Set_PullValue)
	   p_sjfun_Gpio_Set_PullValue(GpioID,Value);
}

//GPIOд�˿�״̬
 typedef void (*p_sjfun_Gpio_Write_Value_t)(u16 GpioID, u8 Value);
p_sjfun_Gpio_Write_Value_t p_sjfun_Gpio_Write_Value=NULL;
void sjfun_Gpio_Write_Value(u16 GpioID, u8 Value)
{
	if(NULL == p_sjfun_Gpio_Write_Value)
	p_sjfun_Gpio_Write_Value = (p_sjfun_Gpio_Write_Value_t)jt_get_sym_entry("Gpio_WriteValue");

	if(NULL != p_sjfun_Gpio_Write_Value)
	   p_sjfun_Gpio_Write_Value(GpioID,Value);
}

//GPIO���˿�״̬
 typedef U8 (*p_sjfun_Gpio_Read_Value_t)(u16 GpioID);
p_sjfun_Gpio_Read_Value_t p_sjfun_Gpio_Read_Value=NULL;
u8 sjfun_Gpio_Read_Value(u16 GpioID)
{
	if(NULL == p_sjfun_Gpio_Read_Value)
	p_sjfun_Gpio_Read_Value = (p_sjfun_Gpio_Read_Value_t)jt_get_sym_entry("Gpio_ReadValue");

	if(NULL != p_sjfun_Gpio_Read_Value)
	 return  p_sjfun_Gpio_Read_Value(GpioID);
	 return 0;
}

//ע��IO �ж�
 typedef void (*p_sjfun_Gpio_EINT_RegInt_t)(u8 EIntNO, u8 Status, FuncPtr funcPtr);
p_sjfun_Gpio_EINT_RegInt_t p_sjfun_Gpio_EINT_RegInt=NULL;
void sjfun_Gpio_EINT_RegInt(u8 EIntNO, u8 Status, FuncPtr funcPtr)
{
	if(NULL == p_sjfun_Gpio_EINT_RegInt)
	p_sjfun_Gpio_EINT_RegInt = (p_sjfun_Gpio_EINT_RegInt_t)jt_get_sym_entry("Gpio_EIntRegEvent");

	if(NULL != p_sjfun_Gpio_EINT_RegInt)
	   p_sjfun_Gpio_EINT_RegInt(EIntNO,Status,funcPtr);
}

//�ж�����
 typedef void (*p_sjfun_Gpio_EINT_SetPolarity_t)(u8 EIntNO, u8 Status);
p_sjfun_Gpio_EINT_SetPolarity_t p_sjfun_Gpio_EINT_SetPolarity=NULL;
void sjfun_Gpio_EINT_SetPolarity(u8 EIntNO, u8 Status)
{
	if(NULL == p_sjfun_Gpio_EINT_SetPolarity)
	p_sjfun_Gpio_EINT_SetPolarity = (p_sjfun_Gpio_EINT_SetPolarity_t)jt_get_sym_entry("Gpio_EIntSetPolarity");

	if(NULL != p_sjfun_Gpio_EINT_SetPolarity)
	   p_sjfun_Gpio_EINT_SetPolarity(EIntNO,Status);
}

//�ж���Ӧʱ������
 typedef void (*p_sjfun_Gpio_EINT_SetDoubleTime_t)(u8 EIntNO, u8 Value);
p_sjfun_Gpio_EINT_SetDoubleTime_t p_sjfun_Gpio_EINT_SetDoubleTime=NULL;
void sjfun_Gpio_EINT_SetDoubleTime(u8 EIntNO, u8 Value)
{
	if(NULL == p_sjfun_Gpio_EINT_SetDoubleTime)
	p_sjfun_Gpio_EINT_SetDoubleTime = (p_sjfun_Gpio_EINT_SetDoubleTime_t)jt_get_sym_entry("Gpio_EIntSetDebounce");

	if(NULL != p_sjfun_Gpio_EINT_SetDoubleTime)
	   p_sjfun_Gpio_EINT_SetDoubleTime(EIntNO,Value);
}

//����������ں���
 typedef U8 (*p_sjfun_Dns_ParaseInterface_t)(char *DnsUrl, u32 AcctID, u8 *IpBuf,u8 DnsType);
p_sjfun_Dns_ParaseInterface_t p_sjfun_Dns_ParaseInterface=NULL;
u8 sjfun_Dns_ParaseInterface(char *DnsUrl, u32 AcctID, u8 *IpBuf,u8 DnsType)
{
	if(NULL == p_sjfun_Dns_ParaseInterface)
	p_sjfun_Dns_ParaseInterface = (p_sjfun_Dns_ParaseInterface_t)jt_get_sym_entry("Dns_Parase_Interface");

	if(NULL != p_sjfun_Dns_ParaseInterface)
	 return  p_sjfun_Dns_ParaseInterface(DnsUrl, AcctID, IpBuf,DnsType);
	 return 0;
}

//��ʼ����������Ϣ,�����ȡ��վС����Ϣ������ע��ɹ�������ô˺���
 typedef void (*p_sjfun_Begin_RsqNetInfo_t)(void);
p_sjfun_Begin_RsqNetInfo_t p_sjfun_Begin_RsqNetInfo=NULL;
void sjfun_Begin_RsqNetInfo(void)
{
	if(NULL == p_sjfun_Begin_RsqNetInfo)
	p_sjfun_Begin_RsqNetInfo = (p_sjfun_Begin_RsqNetInfo_t)jt_get_sym_entry("Begin_NetInfo_Rsq");

	if(NULL != p_sjfun_Begin_RsqNetInfo)
	   p_sjfun_Begin_RsqNetInfo();
}

//����ϵͳʱ��
 typedef void (*p_sjfun_Set_System_DateTime_t)(u8 *DateTime);
p_sjfun_Set_System_DateTime_t p_sjfun_Set_System_DateTime=NULL;
void sjfun_Set_System_DateTime(u8 *DateTime)
{
	if(NULL == p_sjfun_Set_System_DateTime)
	p_sjfun_Set_System_DateTime = (p_sjfun_Set_System_DateTime_t)jt_get_sym_entry("Set_Sys_DateTime");

	if(NULL != p_sjfun_Set_System_DateTime)
	   p_sjfun_Set_System_DateTime(DateTime);
}


//��ȡϵͳʱ��
 typedef void (*p_sjfun_Get_System_DateTime_t)(u8 *DateTime);
p_sjfun_Get_System_DateTime_t p_sjfun_Get_System_DateTime=NULL;
void sjfun_Get_System_DateTime(u8 *DateTime)
{
	if(NULL == p_sjfun_Get_System_DateTime)
	p_sjfun_Get_System_DateTime = (p_sjfun_Get_System_DateTime_t)jt_get_sym_entry("Get_Sys_DateTime");

	if(NULL != p_sjfun_Get_System_DateTime)
	   p_sjfun_Get_System_DateTime(DateTime);
}


//��ȡϵͳIMEI ��Ϣ
 typedef void (*p_sjfun_Get_NV_IMEI_t)(u8 *dbuf, u16 dlen);
p_sjfun_Get_NV_IMEI_t p_sjfun_Get_NV_IMEI;
void sjfun_Get_NV_IMEI(u8 *dbuf, u16 dlen)
{
	if(NULL == p_sjfun_Get_NV_IMEI)
	p_sjfun_Get_NV_IMEI = (p_sjfun_Get_NV_IMEI_t)jt_get_sym_entry("Get_NV_IMEI");

	if(NULL != p_sjfun_Get_NV_IMEI)
	   p_sjfun_Get_NV_IMEI(dbuf,dlen);
}

//��ȡ���״̬
typedef bool(*p_sjfun_Get_Power_Statue_t)(void);
p_sjfun_Get_Power_Statue_t p_sjfun_Get_Power_Statue;
bool sjfun_Get_Power_Statue(void)
{
	if(NULL == p_sjfun_Get_Power_Statue)
		p_sjfun_Get_Power_Statue = (p_sjfun_Get_Power_Statue_t)jt_get_sym_entry("Get_Power_Statue");

	if(NULL != p_sjfun_Get_Power_Statue)
		return p_sjfun_Get_Power_Statue();
}

//��ȡ���״̬
typedef bool(*p_sjfun_Get_Chr_Statue_t)(U8 Flag);
p_sjfun_Get_Chr_Statue_t p_sjfun_Get_Chr_Statue;
bool sjfun_Get_Chr_Statue(U8 Flag)
{
	if(NULL == p_sjfun_Get_Chr_Statue)
		p_sjfun_Get_Chr_Statue = (p_sjfun_Get_Chr_Statue_t)jt_get_sym_entry("Get_Chr_Statue");

	if(NULL != p_sjfun_Get_Chr_Statue)
		return p_sjfun_Get_Chr_Statue(Flag);
}

//--------------------------blow the line is modify by zcj

//��ȡϵͳ��׼����
typedef void(*p_sjfun_SysGetBarCodeValue_t)(u8 *GetBuf);
p_sjfun_SysGetBarCodeValue_t p_sjfun_SysGetBarCodeValue=NULL;
void sjfun_SysGetBarCodeValue(u8 *GetBuf)
{
	if(NULL == p_sjfun_SysGetBarCodeValue)
		p_sjfun_SysGetBarCodeValue = (p_sjfun_SysGetBarCodeValue_t)jt_get_sym_entry("Get_SysBarCode");
	if(NULL != p_sjfun_SysGetBarCodeValue)
		p_sjfun_SysGetBarCodeValue(GetBuf);
}

//д�����ļ�
typedef void(*p_sjfun_WriteFilesToNV_t)(void);
p_sjfun_WriteFilesToNV_t p_sjfun_WriteFilesToNV=NULL;
void sjfun_WriteFilesToNV(void)
{
	if(NULL==p_sjfun_WriteFilesToNV)
		p_sjfun_WriteFilesToNV = (p_sjfun_WriteFilesToNV_t)jt_get_sym_entry("Write_FilesToNV");
	if(NULL != p_sjfun_WriteFilesToNV)
		p_sjfun_WriteFilesToNV();
}

//ֹͣ��ʱ��
typedef void(*p_sjfun_stoptimer_t)(u16 timerid);
p_sjfun_stoptimer_t p_sjfun_stoptimer=NULL;
void sjfun_stoptimer(u16 timerid)
{
	if(NULL == p_sjfun_stoptimer)
		p_sjfun_stoptimer = (p_sjfun_stoptimer_t)jt_get_sym_entry("sjfun_stoptimer");

	if(NULL != p_sjfun_stoptimer)
		p_sjfun_stoptimer(timerid);
}

//����charge LED ��ʾ
 typedef void (*p_sjfun_Set_ChargeLed_t)(u8 Status);
p_sjfun_Set_ChargeLed_t  p_sjfun_Set_ChargeLed=NULL;
void sjfun_Set_ChargeLed(u8 Status)
{
	if(NULL==p_sjfun_Set_ChargeLed)
	{
		p_sjfun_Set_ChargeLed = (p_sjfun_Set_ChargeLed_t)jt_get_sym_entry("led_charge");
	}

	if(NULL!=p_sjfun_Set_ChargeLed)
	{
		p_sjfun_Set_ChargeLed(Status);
	}
}

//GPS�ر�
typedef void(*p_sjfun_Gps_Stop_t)(bool GpsPow);
p_sjfun_Gps_Stop_t p_sjfun_Gps_Stop;
void sjfun_Gps_Stop(bool GpsPow)
{
	if(NULL == p_sjfun_Gps_Stop)
		p_sjfun_Gps_Stop = (p_sjfun_Gps_Stop_t)jt_get_sym_entry("Gps_Stop");
	if(NULL != p_sjfun_Gps_Stop)
		p_sjfun_Gps_Stop(GpsPow);
}

//GPS����
typedef void(*p_sjfun_Gps_Start_t)(bool GpsPow);
p_sjfun_Gps_Start_t p_sjfun_Gps_Start;
void sjfun_Gps_Start(bool GpsPow)
{
	if(NULL == p_sjfun_Gps_Start)
		p_sjfun_Gps_Start = (p_sjfun_Gps_Start_t)jt_get_sym_entry("Gps_Start");
	if(NULL != p_sjfun_Gps_Start)
		p_sjfun_Gps_Start(GpsPow);
}

//AGPS ����д��UBLOX
typedef void(*p_sjfun_Gps_WriteAgps_t)(u8 *AgpsBuf,u16 Len);
p_sjfun_Gps_WriteAgps_t p_sjfun_Gps_WriteAgps;
void sjfun_Gps_WriteAgps(u8 *AgpsBuf,u16 Len)
{
	if(NULL == p_sjfun_Gps_WriteAgps)
		p_sjfun_Gps_WriteAgps = (p_sjfun_Gps_WriteAgps_t)jt_get_sym_entry("Gps_WriteAgps");
	if(NULL != p_sjfun_Gps_WriteAgps)
		p_sjfun_Gps_WriteAgps(AgpsBuf,Len);
}

//��ȡ���վ��Ϣ
typedef void(*p_sjfun_req_mlocate_ncinfo_t)(void);
p_sjfun_req_mlocate_ncinfo_t p_sjfun_req_mlocate_ncinfo=NULL;
void sjfun_req_mlocate_ncinfo(void)
{
	if(NULL==p_sjfun_req_mlocate_ncinfo)
		p_sjfun_req_mlocate_ncinfo = (p_sjfun_req_mlocate_ncinfo_t)jt_get_sym_entry("GET_ncinfo");
	if(NULL != p_sjfun_req_mlocate_ncinfo)
		p_sjfun_req_mlocate_ncinfo();
}

//�ն˸�λ
typedef void(*p_sjfun_request_reset_t)(void);
p_sjfun_request_reset_t p_sjfun_request_reset;
void sjfun_request_reset(void)
{
	if(NULL == p_sjfun_request_reset)
		p_sjfun_request_reset = (p_sjfun_request_reset_t)jt_get_sym_entry("Request_Reset");
	if(NULL != p_sjfun_request_reset)
		p_sjfun_request_reset();
}

//�ն˸�λ(����W1-1��Ŀ����Ӳ��ƥ��)
typedef void(*p_sjfun_sys_reset_t)(void);
p_sjfun_sys_reset_t p_sjfun_sys_reset;
void sjfun_sys_reset(void)
{
	if(NULL == p_sjfun_sys_reset)
		p_sjfun_sys_reset = (p_sjfun_sys_reset_t)jt_get_sym_entry("SYS_Reset");
	if(NULL != p_sjfun_sys_reset)
		p_sjfun_sys_reset();
}


//GPS����͹���
typedef void(*p_sjfun_GPS_Entry_PSM_t)(void);
p_sjfun_GPS_Entry_PSM_t p_sjfun_GPS_Entry_PSM;
void sjfun_GPS_Entry_PSM(void)
{
	if(NULL == p_sjfun_GPS_Entry_PSM)
		p_sjfun_GPS_Entry_PSM = (p_sjfun_GPS_Entry_PSM_t)jt_get_sym_entry("Entry_PSM");
	if(NULL != p_sjfun_GPS_Entry_PSM)
		p_sjfun_GPS_Entry_PSM();
}

//��ȡNV����
typedef u16(*p_sjfun_GetCaliBartionData_t)(u8 *OutBuf);
p_sjfun_GetCaliBartionData_t p_sjfun_GetCaliBartionData;
u16 sjfun_GetCaliBartionData(u8 *OutBuf)
{
	if(NULL == p_sjfun_GetCaliBartionData)
		p_sjfun_GetCaliBartionData = (p_sjfun_GetCaliBartionData_t)jt_get_sym_entry("Get_CaliBartion_Data");
	if(NULL != p_sjfun_GetCaliBartionData)
		return p_sjfun_GetCaliBartionData(OutBuf);
}

//���Ƽ���LED����
typedef void(*p_sjfun_SetKPLED_t)(u8 Status);
p_sjfun_SetKPLED_t p_sjfun_SetKPLED;
void sjfun_SetKPLED(u8 Status)
{
	if(NULL == p_sjfun_SetKPLED)
		p_sjfun_SetKPLED = (p_sjfun_SetKPLED_t)jt_get_sym_entry("Set_Kpled");
	if(NULL != p_sjfun_SetKPLED)
		p_sjfun_SetKPLED(Status);
}

//������
typedef void(*p_sjfun_TurnOn_bt_t)(void);
p_sjfun_TurnOn_bt_t p_sjfun_TurnOn_bt;
void sjfun_TurnOn_bt(void)
{
	if(NULL == p_sjfun_TurnOn_bt)
		p_sjfun_TurnOn_bt = (p_sjfun_TurnOn_bt_t)jt_get_sym_entry("Turnon_bt");
	if(NULL != p_sjfun_TurnOn_bt)
		p_sjfun_TurnOn_bt();
}


//��������
 typedef void (*p_sjfun_Set_Alm_Warn_t)(u8 *dbuf, u16 dlen);
p_sjfun_Set_Alm_Warn_t  p_sjfun_Set_Alm_Warn=NULL;
void sjfun_Set_Alm_Warn(u8 *dbuf, u16 dlen)
{
	if(NULL==p_sjfun_Set_Alm_Warn)
	{
		p_sjfun_Set_Alm_Warn = (p_sjfun_Set_Alm_Warn_t)jt_get_sym_entry("Set_Alm_Warn");
	}

	if(NULL!=p_sjfun_Set_Alm_Warn)
	{
		p_sjfun_Set_Alm_Warn(dbuf, dlen);
	}
}

//��ѯ����
 typedef u16 (*p_sjfun_Read_Alm_Warn_t)(u8 *dbuf, u16 dlen, u8 *OutBuf);
p_sjfun_Read_Alm_Warn_t p_sjfun_Read_Alm_Warn=NULL;
u16 sjfun_Read_Alm_Warn(u8 *dbuf, u16 dlen, u8 *OutBuf)
{
	if(NULL==p_sjfun_Read_Alm_Warn)
	{
		p_sjfun_Read_Alm_Warn = (p_sjfun_Read_Alm_Warn_t)jt_get_sym_entry("Read_Alm_Warn");
	}

	if(NULL!=p_sjfun_Read_Alm_Warn)
	{
		return p_sjfun_Read_Alm_Warn(dbuf, dlen, OutBuf);
	}
}

//��ʼ¼��
 typedef u8 (*p_sjfun_Start_Record_t)(u8 RecTime);
p_sjfun_Start_Record_t p_sjfun_Start_Record=NULL;
u8 sjfun_Start_Record(u8 RecTime)
{
	if(NULL==p_sjfun_Start_Record)
	{
		p_sjfun_Start_Record=(p_sjfun_Start_Record_t)jt_get_sym_entry("Start_Record");
	}

	if(NULL!=p_sjfun_Start_Record)
	{
		return p_sjfun_Start_Record(RecTime);
	}
}

//����ֹͣ¼��
 typedef void (*p_sjfun_Stop_Record_t)(void);
p_sjfun_Stop_Record_t p_sjfun_Stop_Record=NULL;
void sjfun_Stop_Record(void)
{
	if(NULL==p_sjfun_Stop_Record)
	{
		p_sjfun_Stop_Record=(p_sjfun_Stop_Record_t)jt_get_sym_entry("Stop_Record");
	}

	if(NULL!=p_sjfun_Stop_Record)
	{
		p_sjfun_Stop_Record();
	}
}

//��Ƶ�ļ���������
 typedef u8 (*p_sjfun_Start_Audio_Play_t)(void);
p_sjfun_Start_Audio_Play_t p_sjfun_Start_Audio_Play=NULL;
u8 sjfun_Start_Audio_Play(void)
{
	if(NULL==p_sjfun_Start_Audio_Play)
	{
		p_sjfun_Start_Audio_Play=(p_sjfun_Start_Audio_Play_t)jt_get_sym_entry("Start_AudioPlay");
	}

	if(NULL!=p_sjfun_Start_Audio_Play)
	{
		return p_sjfun_Start_Audio_Play();
	}
}

//����0���ݷ��ͽӿ�
typedef void(* p_sjfun_Gps_Com_send_t)(U8 *dbuf, U16 dlen);
p_sjfun_Gps_Com_send_t p_sjfun_Gps_Com_send=NULL;
void sjfun_Gps_Com_send(U8 *dbuf, U16 dlen)
{
	if(NULL==p_sjfun_Gps_Com_send)
	{
		p_sjfun_Gps_Com_send = (p_sjfun_Gps_Com_send_t)jt_get_sym_entry("Send_to_Gps");
	}

	if(NULL!=p_sjfun_Gps_Com_send)
	{
		p_sjfun_Gps_Com_send(dbuf,dlen);
	}
}

//ϵͳ�ػ�
typedef void(* p_sjfun_Shut_Down_send_t)(void);
p_sjfun_Shut_Down_send_t p_sjfun_Shut_Down_send=NULL;
void sjfun_Shut_Down_send(void)
{
	if(NULL==p_sjfun_Shut_Down_send)
	{
		p_sjfun_Shut_Down_send = (p_sjfun_Shut_Down_send_t)jt_get_sym_entry("Shut_Down_Sys");
	}

	if(NULL!=p_sjfun_Shut_Down_send)
	{
		p_sjfun_Shut_Down_send();
	}
}

//�򿪸�������
typedef void(*p_sjfun_Open_AuxUart_t)(void);
p_sjfun_Open_AuxUart_t p_sjfun_Open_AuxUart;
void sjfun_Open_AuxUart(void)
{
	if(NULL == p_sjfun_Open_AuxUart)
		p_sjfun_Open_AuxUart = (p_sjfun_Open_AuxUart_t)jt_get_sym_entry("Aux_Uart_Open");
	if(NULL != p_sjfun_Open_AuxUart)
		p_sjfun_Open_AuxUart();
}

//�رո�������
typedef void(*p_sjfun_Close_AuxUart_t)(void);
p_sjfun_Close_AuxUart_t p_sjfun_Close_AuxUart;
void sjfun_Close_AuxUart(void)
{
	if(NULL == p_sjfun_Close_AuxUart)
		p_sjfun_Close_AuxUart = (p_sjfun_Close_AuxUart_t)jt_get_sym_entry("Aux_Uart_Close");
	if(NULL != p_sjfun_Close_AuxUart)
		p_sjfun_Close_AuxUart();
}

//�����������ݷ���
typedef void(* p_sjfun_Aux_Com_send_t)(U8 *dbuf, U16 dlen);
p_sjfun_Aux_Com_send_t p_sjfun_Aux_Com_send=NULL;
void sjfun_Aux_Com_send(U8 *dbuf, U16 dlen)
{
	if(NULL==p_sjfun_Aux_Com_send)
	{
		p_sjfun_Aux_Com_send = (p_sjfun_Aux_Com_send_t)jt_get_sym_entry("Aux_Uart_Send");
	}

	if(NULL!=p_sjfun_Aux_Com_send)
	{
		p_sjfun_Aux_Com_send(dbuf,dlen);
	}
}

//��ѯ�Ƿ�Ϊ���ߵ���ģʽ
 typedef u8 (*p_sjfun_SkyLine_Mode_t)(void);
p_sjfun_SkyLine_Mode_t  p_sjfun_SkyLine_Mode = NULL;
u8 sjfun_SkyLine_Mode(void)
{
	if(NULL==p_sjfun_SkyLine_Mode)
	{
		p_sjfun_SkyLine_Mode=(p_sjfun_SkyLine_Mode_t)jt_get_sym_entry("Sky_Line_Mode");
	}

	if(NULL!=p_sjfun_SkyLine_Mode)
	{
		return p_sjfun_SkyLine_Mode();
	}
}

//���Դ������ݷ��ͽӿ�
typedef void(* p_sjfun_Debug_Agps_Write_t)(U8 *dbuf, U16 dlen);
p_sjfun_Debug_Agps_Write_t p_sjfun_Debug_Agps_Write=NULL;
void sjfun_Debug_Agps_Write(U8 *dbuf, U16 dlen)
{
	if(NULL==p_sjfun_Debug_Agps_Write)
	{
		p_sjfun_Debug_Agps_Write = (p_sjfun_Debug_Agps_Write_t)jt_get_sym_entry("AGps_Write_File");
	}

	if(NULL!=p_sjfun_Debug_Agps_Write)
	{
		p_sjfun_Debug_Agps_Write(dbuf,dlen);
	}
}

//�ļ�ϵͳ����
//���ļ�
typedef int (*p_sjfun_File_Open_t)(u8 file_name, u32 flag);
p_sjfun_File_Open_t  p_sjfun_File_Open = NULL;
int sjfun_File_Open(u8 file_name, u32 flag)
{
	if(NULL == p_sjfun_File_Open)
		p_sjfun_File_Open = (p_sjfun_File_Open_t)jt_get_sym_entry("File_Open");

	if(NULL != p_sjfun_File_Open)
	   return p_sjfun_File_Open(file_name, flag);
	return NULL;
}

//д�ļ�
typedef int (*p_sjfun_File_Write_t)(int fileandle, u8 *buf, u32 len, u32* write);
p_sjfun_File_Write_t  p_sjfun_File_Write = NULL;
int sjfun_File_Write(int fileandle, u8 *buf, u32 len, u32* write)
{
	if(NULL == p_sjfun_File_Write)
		p_sjfun_File_Write = (p_sjfun_File_Write_t)jt_get_sym_entry("File_Write");

	if(NULL != p_sjfun_File_Write)
	   return p_sjfun_File_Write(fileandle, buf, len, write);
	return NULL;
}

//���ļ�
typedef int (*p_sjfun_File_Read_t)(int fileandle, u8 *buf, u32 len, u32* read);
p_sjfun_File_Read_t  p_sjfun_File_Read = NULL;
int sjfun_File_Read(int fileandle, u8 *buf, u32 len, u32* read)
{
	if(NULL == p_sjfun_File_Read)
		p_sjfun_File_Read = (p_sjfun_File_Read_t)jt_get_sym_entry("File_Read");

	if(NULL != p_sjfun_File_Read)
	   return p_sjfun_File_Read(fileandle, buf, len, read);
	return NULL;
}

//�ļ�ƫ��
typedef int (*p_sjfun_File_Seek_t)(int fileandle, int offset, int whence);
p_sjfun_File_Seek_t  p_sjfun_File_Seek = NULL;
int sjfun_File_Seek(int fileandle, int offset, int whence)
{
	if(NULL == p_sjfun_File_Seek)
		p_sjfun_File_Seek = (p_sjfun_File_Seek_t)jt_get_sym_entry("File_Seek");

	if(NULL != p_sjfun_File_Seek)
	   return p_sjfun_File_Seek(fileandle, offset, whence);
	return NULL;
}

//�ر��ļ�
typedef int (*p_sjfun_File_Close_t)(int fileandle);
p_sjfun_File_Close_t  p_sjfun_File_Close = NULL;
int sjfun_File_Close(int fileandle)
{
	if(NULL == p_sjfun_File_Close)
		p_sjfun_File_Close = (p_sjfun_File_Close_t)jt_get_sym_entry("File_Close");

	if(NULL != p_sjfun_File_Close)
	   return p_sjfun_File_Close(fileandle);
	return NULL;
}

//ɾ���ļ�
typedef int (*p_sjfun_File_Delete_t)(u16 *filename);
p_sjfun_File_Delete_t  p_sjfun_File_Delete = NULL;
int sjfun_File_Delete(u16 *filename)
{
	if(NULL == p_sjfun_File_Delete)
		p_sjfun_File_Delete = (p_sjfun_File_Delete_t)jt_get_sym_entry("File_Delete");

	if(NULL != p_sjfun_File_Delete)
	   return p_sjfun_File_Delete(filename);
	return NULL;
}

//����·��
typedef int (*p_sjfun_File_CreateDir_t)(u16 *dir);
p_sjfun_File_CreateDir_t  p_sjfun_File_CreateDir = NULL;
int sjfun_File_CreateDir(u16 *dir)
{
	if(NULL == p_sjfun_File_CreateDir)
		p_sjfun_File_CreateDir = (p_sjfun_File_CreateDir_t)jt_get_sym_entry("File_CreateDir");

	if(NULL != p_sjfun_File_CreateDir)
	   return p_sjfun_File_CreateDir(dir);
	return NULL;
}

//ɾ��·��
typedef int (*p_sjfun_File_RemoveDir_t)(u16 *dir);
p_sjfun_File_RemoveDir_t  p_sjfun_File_RemoveDir = NULL;
int sjfun_File_RemoveDir(u16 *dir)
{
	if(NULL == p_sjfun_File_RemoveDir)
		p_sjfun_File_RemoveDir = (p_sjfun_File_RemoveDir_t)jt_get_sym_entry("File_RemoveDir");

	if(NULL != p_sjfun_File_RemoveDir)
	   return p_sjfun_File_RemoveDir(dir);
	return NULL;
}

//��ȡunixʱ���
typedef u32 (*p_sjfun_get_unix_time_t)(void);
p_sjfun_get_unix_time_t  p_sjfun_get_unix_time = NULL;
u32 sjfun_get_unix_time(void)
{
	if(NULL == p_sjfun_get_unix_time)
		p_sjfun_get_unix_time = (p_sjfun_get_unix_time_t)jt_get_sym_entry("get_unix_time");

	if(NULL != p_sjfun_get_unix_time)
	   return p_sjfun_get_unix_time();
	return NULL;
}

//����VMC
typedef void (*p_sjfun_VmcSignControl_t)(u8 id);
p_sjfun_VmcSignControl_t  p_sjfun_VmcSignControl = NULL;
void sjfun_VmcSignControl(u8 id)
{
	if(NULL == p_sjfun_VmcSignControl)
		p_sjfun_VmcSignControl = (p_sjfun_VmcSignControl_t)jt_get_sym_entry("VmcSignControl");

	if(NULL != p_sjfun_VmcSignControl)
	   p_sjfun_VmcSignControl(id);
}

//��ȡVMC״̬
typedef u8 (*p_sjfun_VmcGetOCStatus_t)(void);
p_sjfun_VmcGetOCStatus_t  p_sjfun_VmcGetOCStatus = NULL;
u8 sjfun_VmcGetOCStatus(void)
{
	if(NULL == p_sjfun_VmcGetOCStatus)
		p_sjfun_VmcGetOCStatus = (p_sjfun_VmcGetOCStatus_t)jt_get_sym_entry("VmcGetOCStatus");

	if(NULL != p_sjfun_VmcGetOCStatus)
	   return p_sjfun_VmcGetOCStatus();
	return NULL;

}

//unicode ת GB23212
typedef void (*p_sjfun_ucs2totext_str_t)(u8 *dst, u32 size, u8 *src, u8 num);
p_sjfun_ucs2totext_str_t  p_sjfun_ucs2totext_str = NULL;
void sjfun_ucs2totext_str(u8 *dst, u32 size, u8 *src, u8 num)
{
	if(NULL == p_sjfun_ucs2totext_str)
		p_sjfun_ucs2totext_str = (p_sjfun_ucs2totext_str_t)jt_get_sym_entry("ucs2totext_str");

	if(NULL != p_sjfun_ucs2totext_str)
	   p_sjfun_ucs2totext_str(dst, size, src, num);
}