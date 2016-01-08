#ifndef _YS_AUX_UART_H
#define _YS_AUX_UART_H
#include "data_type.h"

typedef struct
{
	u16 	DataLen;
	u8   AddEnable;
	u8	Reserve1;
	u8	DataBuf[1024];
}t_AuxUart_Parsae_Buf;

//use to parase obd fault info
typedef  struct
{
		u8	FaultNum; //故障码个数 *
		u8	Fault[5][5];
}t_Obd_Fault_Info;


//请求驾驶习惯数据流
typedef  struct
{
        u8   Vmp;//电压 V
		u16	 Rpm;//转速 /min
		u8   Spd; //速度 KM/h
		u8   Toh;//节气门开度 %
		u8	 Load;//发动机负荷 %
		u8   Ect; //冷却液温度 C
		u8   Fli;//剩余油量 %
        u8   XM; //百公里油耗
		u8   XH;//怠速油耗
        u8   Ga;//急加速次数
        u8   Gb;//急刹车次数

        u8	 FaultNum; //故障码个数
		u32	 Ms;//上电累计里程 Km
		u32  Fs;//累计耗油量
		u8   Ts;//点火时间H
        u8   Ss;//点火次数
        u16  ObdStrlen;
        u8   ObdStr[200]; //字符数据
}t_Obd_Main_Info;

extern void YS_AuxBufAddData(u8 *dbuf, u16 dlen);

extern void YS_AuxUartBufInit(void);

extern void YS_OBDGetFaultInfo(t_Obd_Fault_Info *pt_GetObdFault);

extern void YS_OBDGetMainInfo(t_Obd_Main_Info *pt_GetObdMain);

extern void YS_ObdGetCarData(t_Obd_Main_Info *pt_ObdMainInfo);

extern u16  YS_ObdCarPackage(u8 *PackBuf);
#endif
