#ifndef _YS_UART_H
#define _YS_UART_H
#include "data_type.h"

#define INTER_CMD_FLAG_AFFIR 			0xF000		//确认信息
#define INTER_CMD_FLAG_FAIL			0xF001		//否认信息
#define INTER_CMD_FLAG_UNKNOW		0xF002		//指令终端无法识别

//-----------------------------下行指令定义-----------------------------------------
#define INTER_CMD_GET_RUNINFO			0x0100		//获取系统运行状态
#define INTER_CMD_GET_GPSSTATUS		0x0101		//获取GPS 运行状态
#define INTER_CMD_GET_NSCK			0x0102		//获取其它通道SOCKET 运行状态
#define INTER_CMD_GET_NETINFO			0x0103		//获取网络信息
#define INTER_CMD_GET_BDGPSINFO		0x0104		//获取北斗GPS 信息
#define INTER_CMD_DIS_CALC				0x0110		//测试两点距离计算工式
#define INTER_CMD_CALIBRATION_GET		0x0120		//读取已校准标志
#define INTER_CMD_CALIBAR_DATA_GET	0x0121		//读取校准数据

#define INTER_CMD_SET_PARAM			0x0201		//请求设置配置参数
#define INTER_CMD_READ_PARAM			0x0202		//请求读取配置参数

#define INTER_CMD_RST_TERI				0x0300		//复位终端
#define INTER_CMD_SET_IP				0x0301		//设置IP信息
#define INTER_CMD_READ_IP				0x0302		//读取IP信息
#define INTER_CMD_SET_PORT				0x0303		//设置端口
#define INTER_CMD_READ_PORT			0x0304		//读取端口
#define INTER_CMD_SET_APN				0x0305		//设置APN 信息
#define INTER_CMD_READ_APN			0x0306		//读取APN 信息
#define INTER_CMD_TEST_CMD			0x0307 		//测试批蛉
#define INTER_CMD_SET_DEBUG_EN		0x0308		//设置调试使能
#define INTER_CMD_READ_DEBUG_EN		0x0309		//设置调试使能
#define INTER_CMD_READ_RSTINFO		0x030A		//读取复位信息
#define INTER_CMD_READ_AREAINFO		0x030B		//读取区域报数据
#define INTER_CMD_AREA_WARTEST		0x030C		//区域报警测试
#define INTER_CMD_SMS_SIMULATION		0x030D		//短信输入模拟
#define INTER_CMD_INIT_SYSPARAM		0x030E		// 初始化运行参数
#define INTER_CMD_SHUTDOWN_SYSTEM	0x030F		//系统关机

#define INTER_CMD_SMS_SEND_TEST		0x0310		//短信发送测试
#define INTER_CMD_NVREAD_TEST			0x0311
#define INTER_CMD_NVWRITE_TEST		0x0312
#define INTER_CMD_GPS_ON_TEST			0x0313		//GPS电源测试开
#define INTER_CMD_GPS_OFF_TEST		0x0314		//GPS电源测试关
#define INTER_CMD_LED_STATUS_TEST	0x0315		//LED 指示灯控制
#define INTER_CMD_SET_GSENSOR_REG	0x0316		//设置gsensor寄存器
#define INTER_CMD_GET_GSENSOR_REG	0x0317		//读取gsensor 寄存器
#define INTER_CMD_ENABLE_SLEEP		0x0318		//允许休眠
#define INTER_CMD_FORBIT_SLEEP		0x0319		//禁止休眠
#define INTER_CMD_CALL_OUT_TEST		0x031A		//电话呼出测试
#define INTER_CMD_VIB_POW_TEST		0x031B		//gsendsor电源控制测试
#define INTER_CMD_VIB_INIT_TEST		0x031C		//gsensor 初始化处理
#define INTER_CMD_GPS_CTRL				0x031D		//GSP 芯片数据透传
#define INTER_CMD_AGPS_START			0x031E		//AGPS 数据请求测试
#define INTER_CMD_GSENSOR_STUDY		0x031F		//gsensor自学习处理

#define INTER_CMD_GET_IRSENSOR_REG	0x0320		//读取IRSENSOR寄存器
#define INTER_CMD_IRSENSOR_STUDY		0x0321		//IRSENSOR校准
#define INTER_CMD_ALM_TIME_SET		0x0325		//时间同步
#define INTER_CMD_ALM_TIME_READ		0x0326		//时间读取
#define INTER_CMD_ALM_WARN_SET		0x032A		//设置闹钟
#define INTER_CMD_ALM_WARN_READ		0x032B		//查询闹钟
#define INTER_CMD_START_RECORD		0x032C		//开始录音
#define INTER_CMD_STOP_RECORD			0x032D		//停止录音
#define INTER_CMD_START_AUDIOPLAY	0x032E		//开始播放音频文件

//#define INTER_CMD_GET_IRSENSOR_THL	0x0322		//获取IRSENSOR下限阈值

#define INTER_CMD_GET_BLIND_INFO		0x0330		//获取盲区补传相关状态信息
#define INTER_CMD_GENERALIO_INIT		0x0333		//通用IO 初始化
#define INTER_CMD_GENERALIO_WRITE	0x0334		//通用IO 写处理
#define INTER_CMD_GENERALIO_READ		0x0335		//通用IO 读处理
#define INTER_CMD_GPS_PYYH_TEST		0x0336	
#define INTER_CMD_GET_RST_WARNINFO	0x0338		//获取复位状态下的报警信息
#define INTER_CMD_FAC_CONNECT			0x0343		//工厂测试终端探测
#define INTER_CMD_OPEN_AUXUART		0x0345		//打开辅助串口
#define INTER_CMD_CLOSE_AUXUART		0x0346		//关闭辅助串口
#define INTER_CMD_SEND_AUXUART		0x0347		//辅助串口发送处理


#define INTER_CMD_SET_FLI_EN			0x0419		//设置飞行模式使能
#define INTER_CMD_READ_FLI_EN			0x0420		//读取飞行模式使能

#define INTER_CMD_SET_ENTRY_FACTORY	0x0424
#define INTER_CMD_SET_EXIT_FACTORY	0x0425


#define INTER_CMD_SENSOR_REC_BEGIN	0x0510
#define INTER_CMD_SENSOR_REC_END		0x0511
#define INTER_CMD_SENSOR_REC_INFO	0x0512
#define INTER_CMD_SENSOR_REC_GET		0x0513

//-----------------------------上行指令定义-------------------------------------------
#define INTER_ACK_CALIBRATION_GET		0x8120		//读取已校准标志应答
#define INTER_ACK_GET_RUNINFO			0x8100		//获取系统运行状态应答
#define INTER_ACK_GET_GPSSTATUS		0x8101		//获取GPS 运行状态应答
#define INTER_ACK_GET_NSCK				0x8102		//获取其它通道SOCKET 运行状态应答
#define INTER_ACK_GET_NETINFO			0x8103		//获取网络信息应答
#define INTER_ACK_GET_BDGPSINFO		0x8104		//获取北斗GPS 信息应答
#define INTER_ACK_DIS_CALC				0x8110		//测试两点距离计算工式应答
#define INTER_ACK_CALIBAR_DATA_GET	0x8121		//读取校准数据

#define INTER_PTL_UPDATE_DEBUGINFO	0x8110		//上报调试数据
#define INTER_PTL_UPDATE_SMSREC		0x8111		//上报短信接收内容
#define INTER_PTL_UPDATE_SMSSEND		0x8112		//上报短信发送内容
#define INTER_PTL_UPDATE_GPRSREC		0x8113		//上报GPRS接收内容
#define INTER_PTL_UPDATE_GPRSSEND	0x8114		//上报GPRS 发送内容

#define INTER_ACK_SET_PARAM			0x8201		//请求设置配置参数应答
#define INTER_ACK_READ_PARAM			0x8202		//请求读取配置参数应答
#define INTER_ACK_READ_IP				0x8302		//读取IP信息应答
#define INTER_ACK_READ_PORT			0x8304		//读取端口信息应答
#define INTER_ACK_READ_APN			0x8306		//读取APN 信息应答
#define INTER_ACK_TEST_CMD				0x8307 		//测试应答
#define INTER_ACK_READ_DEBUG_EN		0x8309		//设置调试使能
#define INTER_ACK_READ_RSTINFO		0x830A		//读取复位信息
#define INTER_ACK_READ_AREAINFO		0x830B		//读取区域报数据应答
#define INTER_ACK_AREA_WARTEST		0x830C		//区域报警测试应答
#define INTER_ACK_SMS_SIMULATION		0x830D		//短信输入模拟应答
#define INTER_ACK_SHUTDOWN_SYSTEM	0x830F		//系统关机应答
#define INTER_ACK_NVREAD_TEST			0x8311
#define INTER_ACK_NVWRITE_TEST		0x8312
#define INTER_ACK_SET_GSENSOR_REG	0x8316		//设置gsensor寄存器应答
#define INTER_ACK_GET_GSENSOR_REG	0x8317		//读取gsensor 寄存器应答
#define INTER_ACK_ENABLE_SLEEP		0x8318		//允许休眠应答
#define INTER_ACK_FORBIT_SLEEP			0x8319		//禁止休眠应答
#define INTER_ACK_CALL_OUT_TEST		0x831A		//电话呼出测试应答
#define INTER_ACK_GSENSOR_STUDY		0x831F		//gsensor自学习处理应答

#define INTER_ACK_GET_IRSENSOR_REG	0x8320		//读取IRSENSOR寄存器应答
#define INTER_ACK_IRSENSOR_STUDY		0x8321		//IRSENSOR校准应答
//#define INTER_ACK_GET_IRSENSOR_THL	0x8322		//获取IRSENSOR下限阈值应答
#define INTER_ACK_ALM_TIME_READ		0x8326		//时间读取应答
#define INTER_ACK_ALM_WARN_READ		0x832B		//查询闹钟应答
#define INTER_ACK_START_RECORD		0x832C		//开始录音应答
#define INTER_ACK_START_AUDIOPLAY		0x832E		//音频播入请求应答

#define INTER_ACK_GET_BLIND_INFO		0x8330		//获取盲区补传相关状态信息应答

#define INTER_ACK_GENERALIO_INIT		0x8333		//通用IO 初始化应答
#define INTER_ACK_GENERALIO_WRITE		0x8334		//通用IO 写处理应答
#define INTER_ACK_GENERALIO_READ		0x8335		//通用IO 读处理应答
#define INTER_ACK_GET_RST_WARNINFO	0x8338		//获取复位状态下的报警信息应答
#define INTER_ACK_FAC_CONNECT			0x8343		//工厂测试终端探测应答
#define INTER_ACK_OPEN_AUXUART		0x8345		//打开辅助串口应答
#define INTER_ACK_CLOSE_AUXUART		0x8346		//关闭辅助串口
#define INTER_ACK_SEND_AUXUART		0x8347		//辅助串口发送处理应答

#define INTER_PTL_TEST_I2C_GDATA		0x8374		//gSensor 数据输入

#define INTER_ACK_READ_SYNC_TIME_MODE	0x8416	//读取终端校时模式应答

#define INTER_ACK_READ_FLI_EN			0x8420

#define INTER_ACK_ENTRY_FACTORY		0x8424
#define INTER_ACK_EXIT_FACTORY		0x8425
#define INTER_ACK_SENSOR_REC_INFO		0x8512

extern void YS_DebugInputInfoDeal(char *DataPoint);

extern void YS_UartDebugInterfacel(unsigned short int CmdID, unsigned char *dbuf, unsigned short int dlen);

extern void YS_UartDebugString(char *DebugString);

extern void YS_UartDataOutInterfacel(unsigned short int CmdID, unsigned char *dbuf, unsigned short int dlen);

extern void YS_UartDebugLengError(u8 *dbuf, u16 dlen);

#endif
