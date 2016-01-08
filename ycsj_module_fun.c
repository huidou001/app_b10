#include "YS_Public.h"

jt_get_sym_entry_t jt_get_sym_entry = NULL;

int return_val_nll=0;
int *return_null=&return_val_nll;

int Tdat1=0;

int platform_call_demo(void);

/*��ƽ̨����ģ���ڵĺ���˳���о�*/
void* out_Methods[] =
{
	(void*)&platform_get_variable,
	(void*)&platform_call_demo,
	(void*)&YS_DebugInputInfoDeal,
	(void*)&YS_RunYSAppEntry,
	(void*)&YS_RunSocketEventHandler,
	(void*)&YS_SmsComeInterface,
	(void*)&YS_SmsSendRltAck,
	(void*)&YS_RunDnsEventHandler,
	(void*)&YS_GpsBufAddData,
	(void*)&YS_RunUbloxDnsEventHandler,
	(void*)&YS_RunWebDnsEventHandler,
	(void*)&YS_SetNetInfo,
	(void*)&YS_SetShutDownFlag,
	(void*)&YS_SetPowoffKeyMsg,
	(void*)&YS_AuxBufAddData
};

#define OUT_METHOD_MAX (sizeof(out_Methods)/sizeof(void*))

char module_ver_buf[32];


void YS_GetModuleVer(char *InputBuf)
{
	memcpy(InputBuf,module_ver_buf,sizeof(module_ver_buf));
}
/**********************************************************
Fuction:		jt_entry
Description:	ģ����ں���
Input:		get_sym_entry
			methods_count
			methods
Return:		void
Others:
**********************************************************/
void jt_entry(IN void* (*get_sym_entry)(const char* sym), IN U16 methods_count, OUT void** methods[])
{
 	U16 i=0;
	U16 n=0;

	jt_get_sym_entry = get_sym_entry;

	setlocale(LC_ALL, "C");

	if(methods_count<OUT_METHOD_MAX)
	{
		n=methods_count;
	}
	else
	{
		n=OUT_METHOD_MAX;
	}

	for(i=0; i<n;i++)
	{
		*methods[i] = out_Methods[i];
	}

	memset(module_ver_buf,0x00,sizeof(module_ver_buf));
	get_module_ver((unsigned char *)module_ver_buf,sizeof(module_ver_buf));
}

/**********************************************************
Fuction:		platform_call_demo
Description:	ƽ̨�����ն˺�������
**********************************************************/
int platform_call_demo(void)
{
	static int RunTimeCount=0;

	RunTimeCount++;
//	sjfun_debug("sjfun_debug out value:%d",RunTimeCount);
	return(RunTimeCount);
}

/**********************************************************
Fuction:		platform_get_variable
Description:	ƽ̨��ȡ����
Input:		p_name_variable
Return:		void *
Others:         ��ƽ̨����ģ���ڵı���˳���о�
**********************************************************/
void *platform_get_variable(char * p_name_variable)
{
	return (void *)return_null;
}

