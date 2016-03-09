#ifndef _YS_PARAM_H
#define _YS_PARAM_H
#include "Data_type.h"

#define NV_WRIE_DELEY_DEF				10
#define FLH_PRM_BUF_LEN					512

#define FLH_PRM_SIM_CODE				0		//终端内SIM卡号
#define FLH_PRM_SIM_CODE_LEN			12

#define FLH_PRM_OWNER_CODE				12 		//车主号码
#define FLH_PRM_OWNER_CODE_LEN			12

#define FLH_PRM_SKYLINE_FLAG			24 		//天线测试版本标志
#define FLH_PRM_SKYLINE_FLAG_LEN		1

#define FLH_PRM_PASSWORD				26		//密码
#define FLH_PRM_PASSWORD_LEN			10

#define FLH_PRM_UPDATE_TIME				36		//定位上传间隔
#define FLH_PRM_UPDATE_TIME_LEN			2

#define FLH_PRM_TRACE_ENABLE			38		//定位上报使能
#define FLH_PRM_TRACE_ENABLE_LEN		1

#define FLH_PRM_HEART_TIME				39		//心跳上报间隔
#define FLH_PRM_HEART_TIME_LEN			2

#define FLH_PRM_ACTIVE_FLAG				41
#define FLH_PRM_ACTIVE_FLAG_LEN			1

#define FLH_PRM_VIB_LEVEL				42
#define FLH_PRM_VIB_LEVEL_LEN			1

#define FLH_PRM_POWOFFWARN_SMS			43
#define FLH_PRM_POWOFFWARN_SMS_LEN		1

#define FLH_PRM_POWOFFWARN_GPRS			44
#define FLH_PRM_POWOFFWARN_GPRS_LEN		1

#define FLH_PRM_BATT_LOW_LIMIT			45
#define FLH_PRM_BATT_LOW_LIMIT_LEN		2

#define FLH_PRM_SLEEP_CTRL					47
#define FLH_PRM_SLEEP_CTRL_LEN			1

#define FLH_PRM_SLEEP_TIME					49			//存储休眠时间
#define FLH_PRM_SLEEP_TIME_LEN			2

#define FLH_PRM_SERV_ADDR					51
#define FLH_PRM_SERV_ADDR_LEN				30

#define FLH_PRM_SERV_PORT					81
#define FLH_PRM_SERV_PORT_LEN				2

#define FLH_PRM_PPP_APN					83
#define FLH_PRM_PPP_APN_LEN				20

#define FLH_PRM_VIB_CHK					103		//终端振动检测次数
#define FLH_PRM_VIB_CHK_LEN				2

#define FLH_PRM_DEBUG_ENABLE				105		//终端调试使能
#define FLH_PRM_DEBUG_ENABLE_LEN			1

#define FLH_PRM_TERI_IMEI					106
#define FLH_PRM_TERI_IMEI_LEN				16

#define FLH_PRM_TERI_TYPE					122
#define FLH_PRM_TERI_TYPE_LEN				1

#define FLH_PRM_RST_RECORD				123		//复位记录存贮
#define FLH_PRM_RST_RECORD_LEN			10

#define FLH_PRM_LAST_GPSPOINT				133		//用于记录最后的GPS 定位点
#define FLH_PRM_LAST_GPSPOINT_LEN			8

#define FLH_PRM_VIB_BASEVALUE				141		//振动传感器基数保龄值
#define FLH_PRM_VIB_BASEVALUE_LEN			1

#define FLH_PRM_ANGLE_SEND_FLAG			142		//拐点补传使能控制
#define FLH_PRM_ANGLE_SEND_FLAG_LEN		1

#define FLH_PRM_NV_GPS_PYYH                		143     	//GPS漂移优化
#define FLH_PRM_NV_GPS_PYYH_LEN             	1

#define FLH_PRM_NV_GPS_AGPS				144		//AGPS 功能使能参数
#define FLH_PRM_NV_GPS_AGPS_LEN			1

#define FLH_PRM_NV_LAST_POINT_DT			145		//最后一个定位点的时间
#define FLH_PRM_NV_LAST_POINT_DT_LEN		6

#define FLH_PRM_NV_DWADDR					151		//远程下载IP
#define FLH_PRM_NV_DWADDR_LEN			20

#define FLH_PRM_NV_DWPORT					171		//远程下载端口
#define FLH_PRM_NV_DWPORT_LEN			2

#define FLH_PRM_NV_DWFLAG					173		//远程下载标志
#define FLH_PRM_NV_DWFLAG_LEN			1

#define FLH_PRM_SOFT_VER_INFO				174		//存贮终端软件版本号,用于不同软件版本间的程序互刷
#define FLH_PRM_SOFT_VER_INFO_LEN			20

#define FLH_PRM_FLI_ENABLE					194		//飞行模块使能
#define FLH_PRM_FLI_ENABLE_LEN			1

#define FLH_PRM_NV_POWLOW_FLAG			195		//低电量报警标志
#define FLH_PRM_NV_POWLOW_FLAG_LEN		1

#define FLH_PRM_PLATFORM_VER				196		//存贮主模块，判断是否需要重新加载默认的应用程序
#define FLH_PRM_PLATFORM_VER_LEN			20

//#define FLH_PRM_DW_PHONE					216		//短信升级结果回复
//#define FLH_PRM_DW_PHONE_LEN				12

#define FLH_PRM_NV_AUTOPLAT_ENABLE		216	//自动升级判定平台使能
#define FLH_PRM_NV_AUTOPLAT_ENABLE_LEN	1

#define FLH_PRM_NV_AUTOPLAT_ADDR			217     //HTTP升级文件路径
#define FLH_PRM_NV_AUTOPLAT_ADDR_LEN	30

/*****************************************/

#define FLH_PRM_NV_AUTOPLAT_FLAG			251		//自动升级判定平台连接标志
#define FLH_PRM_NV_AUTOPLAT_FLAG_LEN		1

#define FLH_JTB_PROVINCE_NAME               		252
#define FLH_JTB_PROVINCE_NAME_LEN           	2

#define FLH_JTB_CITY_NAME					254
#define FLH_JTB_CITY_NAME_LEN				2

#define FLH_JTB_PRODUCER_NAME			    	256
#define FLH_JTB_PRODUCER_NAME_LEN		5

#define FLH_JTB_TERMINAL_TYPE				261
#define FLH_JTB_TERMINAL_TYPE_LEN			8

#define FLH_JTB_TERMINAM_ID				269
#define FLH_JTB_TERMINAM_ID_LEN			7

#define FLH_JTB_PLATE_COLOR				276
#define FLH_JTB_PLATE_COLOR_LEN			1

#define FLH_JTB_PLATE_STRING				277
#define FLH_JTB_PLATE_STRING_LEN			16

#define FLH_JTB_SERV_DOWN_SN                		293 		//鉴权码
#define FLH_JTB_SERV_DOWN_SN_LEN            	40

#define FLH_PRM_SPEED_VALUE				333		//速度
#define FLH_PRM_SPEED_VALUE_LEN			2

#define FLH_PRM_SPEED_TIME					335		//速度持续时间
#define FLH_PRM_SPEED_TIME_LEN			2

#define FLH_PRM_DIS_RATIO				    	337		//用于记录最后的GPS里程计算点
#define FLH_PRM_DIS_RATIO_LEN	            		1

#define FLH_PRM_AREA_STOR					338		//用于记当区域报警数据
#define FLH_PRM_AREA_STOR_LEN				30

#define FLH_PRM_DIS_VALUE					368		//用于记录系统里程
#define FLH_PRM_DIS_VALUE_LEN				4

#define FLH_PRM_RELAY_STOR                  		372     	//用于保存继电器状态
#define FLH_PRM_RELAY_STOR_LEN              	1

#define FLH_PRM_UPDATE_MODE					373		//当前工作模式
#define FLH_PRM_UPDATE_MODE_LEN			1

#define FLH_PRM_RST_NUM					374		//复位次数
#define FLH_PRM_RST_NUM_LEN			    2

#define FLH_PRM_GPS_DEBUG_EN            376     //GPS调试使能
#define FLH_PRM_GPS_DEBUG_EN_LEN        1

#define FLH_PRM_OBD_DEBUG_EN            377     //OBD调试使能
#define FLH_PRM_OBD_DEBUG_EN_LEN        1

#define FLH_PRM_CAN_TIME					378		//CAN上传间隔
#define FLH_PRM_CAN_TIME_LEN				2

#define FLH_PRM_CAN_ENABLE			    	380		//CAN上报使能
#define FLH_PRM_CAN_ENABLE_LEN		    	1

#define HBYD_PARAM_NAME_UPDELAY				"FREQ="
#define HBYD_PARAM_NAME_DOMAIN				"DOMAIN="      /////////////////
#define HBYD_PARAM_NAME_CIRCLEAREA			"RADIUS="
#define HBYD_PARAM_NAME_PWM					"PSW="
#define HBYD_PARAM_NAME_TRACE				"TRACE="
#define HBYD_PARAM_NAME_PULSE				"PULSE="
#define HBYD_PARAM_NAME_PHONE				"PHONE="
#define HBYD_PARAM_NAME_USER					"USER="
#define HBYD_PARAM_NAME_VIB					"VIB="
#define HBYD_PARAM_NAME_VIBS					"VIBS="
#define HBYD_PARAM_NAME_VIBSMS_EN			"VIBSMS_EN=" //针对荣迅及物联网软件添加的SMS 报警使用设置
#define HBYD_PARAM_NAME_VIBL					"VIBL="
#define HBYD_PARAM_NAME_VIBT					"VIBT="
#define HBYD_PARAM_NAME_VIB_BASE				"VIB_BASE="
#define HBYD_PARAM_NAME_POF					"POF="
#define HBYD_PARAM_NAME_POFS					"POFS="
#define HBYD_PARAM_NAME_POFSMS_EN			"POFSMS_EN=" //针对荣迅及物联网软件添加的SMS 报警使用设置
#define HBYD_PARAM_NAME_LBV					"LBV="
#define HBYD_PARAM_NAME_SLEEP				"SLEEP="
#define HBYD_PARAM_NAME_VIBGPS				"VIBGPS="
#define HBYD_PARAM_NAME_VIBCALL				"VIBCALL="
#define HBYD_PARAM_NAME_SMS					"SMS="
#define HBYD_PARAM_NAME_VIBCHK				"VIBCHK="
#define HBYD_PARAM_NAME_POFT					"POFT="
#define HBYD_PARAM_NAME_SLEEPT				"SLEEPT="
#define HBYD_PARAM_NAME_ACCLT				"ACCLT="
#define HBYD_PARAM_NAME_ACCLOCK				"ACCLOCK="
#define HBYD_PARAM_NAME_OUTSMS_EN			"OUTSMS_EN=" //针对荣迅及物联网软件添加的SMS 报警使用设置
#define HBYD_PARAM_NAME_TERIID				"TERIID="
#define HBYD_PARAM_NAME_APN					"APN="
#define HBYD_PARAM_NAME_SERVIP				"SERVIP="
#define HBYD_PARAM_NAME_SERVPORT				"SERVPORT="
#define HBYD_PARAM_NAME_IMEI					"IMEI="
#define HBYD_PARAM_NAME_EQUTYPE				"EQUTYPE="
#define HBYD_PARAM_NAME_SPEED				"SPEED="
#define HBYD_PARAM_NAME_STOPSERVICE			"STOP_SERVICE="
#define HBYD_PARAM_NAME_ANGLE_SEND			"ANGLE_SEND="		//拐点补偿使能
#define HBYD_PARAM_NAME_GPS_FINTER_EN  		"GPS_FINTER_EN="
#define HBYD_PARAM_NAME_AGPS_EN				"AGPS_EN="

#define HBYD_PARAM_NAME_DW_ADDR				"DW_ADDR="
#define HBYD_PARAM_NAME_DW_PORT				"DW_PORT="
#define HBYD_PARAM_NAME_DW_FLAG				"DW_FLAG="
#define HBYD_PARAM_NAME_DW_FILE				"DW_FILE="
#define HBYD_pARAM_NAME_AUTO_ENABLE			"AUTO_ENABLE="
#define HBYD_pARAM_NAME_AUTO_ADDR			"AUTO_ADDR="
#define HBYD_pARAM_NAME_AUTO_PORT			"AUTO_PORT="
#define HBYD_pARAM_NAME_AUTO_FLAG			"AUTO_FLAG="
#define HBYD_PARAM_NAME_ALARM_CLOCK			"ALARM_CLOCK="
#define HBYD_PARAM_NAME_UPDATE_MODE		"UPDATE_MODE="
#define HBYD_PARAM_NAME_SYNC_DT				"SYNC_DT="

#define HBYD_PARAM_NAME_SJIP					"SJIP="
#define HBYD_PARAM_NAME_DLPORT				"DLPORT="
#define HBYD_pARAM_NAME_AUTODLPORT			"AUTODLPORT="
#define HBYD_PARAM_NAME_SKYLINE_FLAG		"SKY_LINE_FLAG="


#define HBYD_PARAM_READ_SOFTVER				"SOFTVERSION"
#define HBYD_PARAM_READ_GSM					"GSM"
#define HBYD_PARAM_READ_GPS					"GPS"
#define HBYD_PARAM_READ_VBAT					"VBAT"
#define HBYD_PARAM_READ_LOGIN				"LOGIN"
#define HBYD_PARAM_READ_IMSI					"IMSI"
#define HBYD_PARAM_READ_IMEI					"IMEI"
#define HBYD_PARAM_READ_ACC					"ACC"

#define HBYD_PARAM_READ_UPDELAY				"FREQ"
#define HBYD_PARAM_READ_DOMAIN				"DOMAIN"
#define HBYD_PARAM_READ_PWM					"PSW"
#define HBYD_PARAM_READ_TRACE				"TRACE"
#define HBYD_PARAM_READ_PULSE				"PULSE"
#define HBYD_PARAM_READ_PHONE				"PHONE"
#define HBYD_PARAM_READ_USER					"USER"
#define HBYD_PARAM_READ_VIB					"VIB"
#define HBYD_PARAM_READ_VIBT					"VIBT"
#define HBYD_PARAM_READ_VIBS					"VIBS"
#define HBYD_PARAM_READ_VIBSMS_EN			"VIBSMS_EN" //针对荣迅及物联网软件添加的SMS 报警使用设置
#define HBYD_PARAM_READ_VIBL					"VIBL"
#define HBYD_PARAM_READ_VIB_BASE				"VIB_BASE"
#define HBYD_PARAM_READ_POF					"POF"
#define HBYD_PARAM_READ_POFS					"POFS"
#define HBYD_PARAM_READ_POFSMS_EN			"POFSMS_EN" //针对荣迅及物联网软件添加的SMS 报警使用设置
#define HBYD_PARAM_READ_LBV					"LBV"
#define HBYD_PARAM_READ_SLEEP					"SLEEP"
#define HBYD_PARAM_READ_VIBGPS				"VIBGPS"
#define HBYD_PARAM_READ_VIBCALL				"VIBCALL"
#define HBYD_PARAM_READ_SMS					"SMS"
#define HBYD_PARAM_READ_VIBCHK				"VIBCHK"
#define HBYD_PARAM_READ_POFT					"POFT"
#define HBYD_PARAM_READ_SLEEPT				"SLEEPT"
#define HBYD_PARAM_READ_ACCLT				"ACCLT"
#define HBYD_PARAM_READ_ACCLOCK				"ACCLOCK"
#define HBYD_PARAM_READ_OUTSMS_EN			"OUTSMS_EN" //针对荣迅及物联网软件添加的SMS 报警使用设置
#define HBYD_PARAM_READ_CHARGE_STATUS		"CHARGE_STATUS"
#define HBYD_PARAM_READ_FACMODE				"FACMODE"	//工程模式

#define HBYD_PARAM_READ_TERIID				"TERIID"
#define HBYD_PARAM_READ_APN					"APN"
#define HBYD_PARAM_READ_SERVIP				"SERVIP"
#define HBYD_PARAM_READ_SERVPORT				"SERVPORT"
#define HBYD_PARAM_READ_EQUTYPE				"EQUTYPE"
#define HBYD_PARAM_READ_SPEED				"SPEED"
#define HBYD_PARAM_READ_STOPSERVICE			"STOP_SERVICE"
#define HBYD_PARAM_READ_ANGLE_SEND			"ANGLE_SEND"
#define HBYD_PARAM_READ_GPS_FINTER_EN   	  	"GPS_FINTER_EN"
#define HBYD_PARAM_READ_AGPS_EN				"AGPS_EN"

#define HBYD_PARAM_READ_DW_ADDR				"DW_ADDR"
#define HBYD_PARAM_READ_DW_PORT				"DW_PORT"
#define HBYD_PARAM_READ_DW_FLAG				"DW_FLAG"
#define HBYD_PARAM_READ_DW_FILE				"DW_FILE"
#define HBYD_pARAM_READ_AUTO_ENABLE			"AUTO_ENABLE"
#define HBYD_pARAM_READ_AUTO_ADDR			"AUTO_ADDR"
#define HBYD_pARAM_READ_AUTO_PORT			"AUTO_PORT"
#define HBYD_pARAM_READ_AUTO_FLAG			"AUTO_FLAG"

#define HBYD_PARAM_READ_SJIP					"SJIP"
#define HBYD_PARAM_READ_DLPORT				"DLPORT"
#define HBYD_pARAM_READ_AUTODLPORT			"AUTODLPORT"
#define HBYD_PARAM_READ_IRSENSOR_THL		"IRSENSOR_THL"
#define HBYD_PARAM_READ_ALARM_CLOCK			"ALARM_CLOCK"
#define HBYD_PARAM_READ_UPDATE_MODE			"UPDATE_MODE"
#define HBYD_PARAM_READ_SYNC_DT				"SYNC_DT"
#define HBYD_PARAM_READ_SKYLINE_FLAG			"SKY_LINE_FLAG"


#define TEST_SAVE_DATA_FILE     	(L"c:\\testdata.png")
#define UPDATE_FILE_PATH  		L"c:\\Updata\\"
#define UPDATE_FILE_NAME  		L"c:\\Updata\\jt_ads.bin"

typedef struct
{
	u8	WriteFlag;
	u8	WriteDelay;
}t_Param_Manage;

typedef struct
{
    int filehandle;
    u8* filename;
    u8  packnum;
    u32 writepos;
    u32 readpos;
    u32 filelen;
}t_File_Para;

extern void YS_PrmReadOneItem(u16 PrmID, u16 dlen, u8 *FillBuf );

extern void YS_PrmWriteOneItem(u16 PrmID, u16 dlen, u8 *dbuf );

extern bool YS_WorkPrmSetDeal(u8 *dbuf, u16 dlen);

extern bool YS_PrmFindStringInbuf(u8 *dbuf, u16 dlen, char *IndexStr, char *BackStr);

extern bool YS_WorkPrmReadDeal(u8 *dbuf, u16 dlen, u8 *PackBuf, u16 *PackLen);

extern void YS_PrmSetFactroyWorkParam(void);

extern void YS_PrmInitDeal(void);

extern void YS_PrmWriteNVDeal(void);

extern void YS_PrmQuickWriteNV(void);

extern void YS_PrmFacetorySetting(void);

extern void YS_CleanPowLowFlag(void);

extern void YS_PamFormatRunParam(void);

extern void YS_PrmNewNVTest(u8 *dbuf, u16 dlen);

extern void YS_SetPowLowFlag(void);

extern void YS_PrmGetTotalSoftVer(u8 *fbuf);

#endif

