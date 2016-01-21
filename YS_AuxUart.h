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
		u8	FaultNum; //��������� *
		u8	Fault[5][5];
}t_Obd_Fault_Info;


//�����ʻϰ��������
typedef  struct
{
        u8   Vmp;//��ѹ V
		u16	 Rpm;//ת�� /min
		u8   Spd; //�ٶ� KM/h
		u8   Toh;//�����ſ��� %
		u8	 Load;//���������� %
		u8   Ect; //��ȴҺ�¶� C
		u8   Fli;//ʣ������ %
        u8   XM; //�ٹ����ͺ�
		u8   XH;//�����ͺ�
        u8   Ga;//�����ٴ���
        u8   Gb;//��ɲ������

        u8	 FaultNum; //���������
		u32	 Ms;//�ϵ��ۼ���� Km
		u32  Fs;//�ۼƺ�����
		u8   Ts;//���ʱ��H
        u8   Ss;//������
        u16  ObdStrlen;
        u8   ObdStr[200]; //�ַ�����
}t_Obd_Main_Info;

extern void YS_AuxBufAddData(u8 *dbuf, u16 dlen);

extern void YS_AuxUartBufInit(void);

extern void YS_OBDGetFaultInfo(t_Obd_Fault_Info *pt_GetObdFault);

extern void YS_OBDGetMainInfo(t_Obd_Main_Info *pt_GetObdMain);

extern void YS_ObdGetCarData(t_Obd_Main_Info *pt_ObdMainInfo);

extern u16  YS_ObdCarPackage(u8 *PackBuf);

extern void YS_OBDRstInit(bool flag);

extern void YS_OBDRstDeal(void);
#endif
