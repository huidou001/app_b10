#ifndef _YS_PUBLIC_H
#define _YS_PUBLIC_H
//�ڲ����̽ӿ��ļ�
//#include "Nvram_data_items.h"
#include "Data_type.h"
#include "YS_CodeFun.h"
#include "YS_RunManage.h"
#include "YS_Uart.h"
#include "YS_Param.h"
#include "YS_IODeal.h"
#include "YS_Gps.h"
#include "YS_Sms.h"
#include "YS_Gprs.h"
#include "YS_SysFace.h"
//#include "YS_DownLoad.h"
#include "Ycsj_platform_fun.h"
#include "ycsj_module_fun.h"
#include "YS_AGpsServer.h"
#include "YS_gSensor.h"
#include "YS_WebServer.h"
#include "YS_AutoUpdate.h"
#include "YS_Gprs_808.h"
#include "YS_AuxUart.h"
#include "YS_Gprs_obd.h"
#include "YS_File.h"

//ϵͳͷ�ļ��ӿ�
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
//�ն�����汾����
#define 	IMEI_FIRM_STR_INFO		"35682303"

#define ENABLE_X1_MODLE 		0   	//(ע��YS_IODeal.h�ļ���IO�Ķ��壬�ú겻��������H�ļ�ʵ���﷨)0����X1_9   1����X1_8 2����X1_16
#define LIMITPHONEEN       		0	//��̻�IP ID��ء�0�����̻���1����̻�

#define	IP_CHANGE_EN     		1

#define 	EBIKE_SOFT_VER  			"B10_V1.6t"

#define   PROJECT_NAME_DEF			"OBD"

#define   PROJECT_APP_VER_DEF		"V1.01"

#define RUN_SOCKET_DELAY_DEF		60
#define RUN_LOGIN_DELAY_DEF		15
#define RUN_LOGIN_TIMES_DEF		3
#define DW_RSQ_FILE_DELAY			15
#define DW_RSQ_FILE_TIMES			3
#define DW_RSQ_GROUP_DELAY		20
#define DW_RSQ_GROUP_TIMES		3

#define RUN_HEART_TIMES_DEF		2
#define RUN_RECON_DELAY_DEF		10
#define POW_SLEEP_TIME              		300
#define RUN_DNS_DELAY_DEF			30

enum
{
    FJYD_TERI_STATUS_INIT,			//��ʼ��״̬
    FJYD_TERI_STATUS_ACTIVE,			//���󼤻�״̬
    FJYD_TERI_STATUS_ANOTHER,		//�л��û����󼤻�״̬
    FJYD_TERI_STATUS_OK				//�ѳɹ�����״̬
};

enum
{
    FJYD_WARN_ID_ALL,
    FJYD_WARN_ID_POSOUT,				//Խ�籨��
    FJYD_WARN_ID_BATTLOW,			//��ص͵�ѹ����
    FJYD_WARN_ID_DISMATLE			//�������
};

enum
{
    DW_RLT_UPDATE_SUCCESS,			//�����ɹ�
    DW_RLT_REG_NET_FAIL,				//����ע��ʧ��
    DW_RLT_NO_POS_FOR_TERI,			//����ƽ̨�����ն��������ܾ����ε�¼����������Ժ�����
    DW_RLT_NO_UPDATE_FILE,			//�޶�Ӧ�������ļ�
    DW_RLT_DOWNFILE_FAIL				//�����ļ�ʧ��
};

enum
{
    FJYD_TERI_TYPE_EBIKE,				//��ƿ��ģʽ
    FJYD_TERI_TYPE_MOTO				//ģ�г�ģʽ
};

enum
{
    CUR_WORK_MODE_ALARM_CLOCK,		//��ǰ����ģʽ,ʱ�ӻ���
    CUR_WORK_MODE_UPDATE_TIMER		//��ǰ����ģʽ,��λ���
};

typedef enum
{
    SOC_READ    = 0x01,  /* Notify for read */
    SOC_WRITE   = 0x02,  /* Notify for write */
    SOC_ACCEPT  = 0x04,  /* Notify for accept */
    SOC_CONNECT = 0x08,  /* Notify for connect */
    SOC_CLOSE   = 0x10   /* Notify for close */
} soc_event_enum;

#endif
