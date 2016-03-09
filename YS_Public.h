#ifndef _YS_PUBLIC_H
#define _YS_PUBLIC_H
//内部工程接口文件
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

//系统头文件接口
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
//终端软件版本定义
#define 	IMEI_FIRM_STR_INFO		"35682303"

#define ENABLE_X1_MODLE 		0   	//(注意YS_IODeal.h文件中IO的定义，该宏不能在另外H文件实现语法)0代表X1_9   1代表X1_8 2代表X1_16
#define LIMITPHONEEN       		0	//与固化IP ID相关。0代表不固化，1代表固化

#define	IP_CHANGE_EN     		1

#define 	EBIKE_SOFT_VER  			"A20_V1.0t3"

#define   PROJECT_NAME_DEF			"MOTOR"

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
    FJYD_TERI_STATUS_INIT,			//初始化状态
    FJYD_TERI_STATUS_ACTIVE,			//请求激活状态
    FJYD_TERI_STATUS_ANOTHER,		//切换用户请求激活状态
    FJYD_TERI_STATUS_OK				//已成功激话状态
};

enum
{
    FJYD_WARN_ID_ALL,
    FJYD_WARN_ID_POSOUT,				//越界报警
    FJYD_WARN_ID_BATTLOW,			//电池低电压报警
    FJYD_WARN_ID_DISMATLE			//拆除报警
};

enum
{
    DW_RLT_UPDATE_SUCCESS,			//升级成功
    DW_RLT_REG_NET_FAIL,				//网络注册失败
    DW_RLT_NO_POS_FOR_TERI,			//由于平台在线终端已满，拒绝本次登录，如果继续稍后再试
    DW_RLT_NO_UPDATE_FILE,			//无对应的升级文件
    DW_RLT_DOWNFILE_FAIL				//下载文件失败
};

enum
{
    FJYD_TERI_TYPE_EBIKE,				//电瓶车模式
    FJYD_TERI_TYPE_MOTO				//模托车模式
};

enum
{
    CUR_WORK_MODE_ALARM_CLOCK,		//当前工作模式,时钟唤醒
    CUR_WORK_MODE_UPDATE_TIMER		//当前工作模式,定位监控
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
