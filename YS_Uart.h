#ifndef _YS_UART_H
#define _YS_UART_H
#include "data_type.h"

#define INTER_CMD_FLAG_AFFIR 			0xF000		//ȷ����Ϣ
#define INTER_CMD_FLAG_FAIL			0xF001		//������Ϣ
#define INTER_CMD_FLAG_UNKNOW		0xF002		//ָ���ն��޷�ʶ��

//-----------------------------����ָ���-----------------------------------------
#define INTER_CMD_GET_RUNINFO			0x0100		//��ȡϵͳ����״̬
#define INTER_CMD_GET_GPSSTATUS		0x0101		//��ȡGPS ����״̬
#define INTER_CMD_GET_NSCK			0x0102		//��ȡ����ͨ��SOCKET ����״̬
#define INTER_CMD_GET_NETINFO			0x0103		//��ȡ������Ϣ
#define INTER_CMD_GET_BDGPSINFO		0x0104		//��ȡ����GPS ��Ϣ
#define INTER_CMD_DIS_CALC				0x0110		//�������������㹤ʽ
#define INTER_CMD_CALIBRATION_GET		0x0120		//��ȡ��У׼��־
#define INTER_CMD_CALIBAR_DATA_GET	0x0121		//��ȡУ׼����

#define INTER_CMD_SET_PARAM			0x0201		//�����������ò���
#define INTER_CMD_READ_PARAM			0x0202		//�����ȡ���ò���

#define INTER_CMD_RST_TERI				0x0300		//��λ�ն�
#define INTER_CMD_SET_IP				0x0301		//����IP��Ϣ
#define INTER_CMD_READ_IP				0x0302		//��ȡIP��Ϣ
#define INTER_CMD_SET_PORT				0x0303		//���ö˿�
#define INTER_CMD_READ_PORT			0x0304		//��ȡ�˿�
#define INTER_CMD_SET_APN				0x0305		//����APN ��Ϣ
#define INTER_CMD_READ_APN			0x0306		//��ȡAPN ��Ϣ
#define INTER_CMD_TEST_CMD			0x0307 		//��������
#define INTER_CMD_SET_DEBUG_EN		0x0308		//���õ���ʹ��
#define INTER_CMD_READ_DEBUG_EN		0x0309		//���õ���ʹ��
#define INTER_CMD_READ_RSTINFO		0x030A		//��ȡ��λ��Ϣ
#define INTER_CMD_READ_AREAINFO		0x030B		//��ȡ��������
#define INTER_CMD_AREA_WARTEST		0x030C		//���򱨾�����
#define INTER_CMD_SMS_SIMULATION		0x030D		//��������ģ��
#define INTER_CMD_INIT_SYSPARAM		0x030E		// ��ʼ�����в���
#define INTER_CMD_SHUTDOWN_SYSTEM	0x030F		//ϵͳ�ػ�

#define INTER_CMD_SMS_SEND_TEST		0x0310		//���ŷ��Ͳ���
#define INTER_CMD_NVREAD_TEST			0x0311
#define INTER_CMD_NVWRITE_TEST		0x0312
#define INTER_CMD_GPS_ON_TEST			0x0313		//GPS��Դ���Կ�
#define INTER_CMD_GPS_OFF_TEST		0x0314		//GPS��Դ���Թ�
#define INTER_CMD_LED_STATUS_TEST	0x0315		//LED ָʾ�ƿ���
#define INTER_CMD_SET_GSENSOR_REG	0x0316		//����gsensor�Ĵ���
#define INTER_CMD_GET_GSENSOR_REG	0x0317		//��ȡgsensor �Ĵ���
#define INTER_CMD_ENABLE_SLEEP		0x0318		//��������
#define INTER_CMD_FORBIT_SLEEP		0x0319		//��ֹ����
#define INTER_CMD_CALL_OUT_TEST		0x031A		//�绰��������
#define INTER_CMD_VIB_POW_TEST		0x031B		//gsendsor��Դ���Ʋ���
#define INTER_CMD_VIB_INIT_TEST		0x031C		//gsensor ��ʼ������
#define INTER_CMD_GPS_CTRL				0x031D		//GSP оƬ����͸��
#define INTER_CMD_AGPS_START			0x031E		//AGPS �����������
#define INTER_CMD_GSENSOR_STUDY		0x031F		//gsensor��ѧϰ����

#define INTER_CMD_GET_IRSENSOR_REG	0x0320		//��ȡIRSENSOR�Ĵ���
#define INTER_CMD_IRSENSOR_STUDY		0x0321		//IRSENSORУ׼
#define INTER_CMD_ALM_TIME_SET		0x0325		//ʱ��ͬ��
#define INTER_CMD_ALM_TIME_READ		0x0326		//ʱ���ȡ
#define INTER_CMD_ALM_WARN_SET		0x032A		//��������
#define INTER_CMD_ALM_WARN_READ		0x032B		//��ѯ����
#define INTER_CMD_START_RECORD		0x032C		//��ʼ¼��
#define INTER_CMD_STOP_RECORD			0x032D		//ֹͣ¼��
#define INTER_CMD_START_AUDIOPLAY	0x032E		//��ʼ������Ƶ�ļ�

//#define INTER_CMD_GET_IRSENSOR_THL	0x0322		//��ȡIRSENSOR������ֵ

#define INTER_CMD_GET_BLIND_INFO		0x0330		//��ȡä���������״̬��Ϣ
#define INTER_CMD_GENERALIO_INIT		0x0333		//ͨ��IO ��ʼ��
#define INTER_CMD_GENERALIO_WRITE	0x0334		//ͨ��IO д����
#define INTER_CMD_GENERALIO_READ		0x0335		//ͨ��IO ������
#define INTER_CMD_GPS_PYYH_TEST		0x0336	
#define INTER_CMD_GET_RST_WARNINFO	0x0338		//��ȡ��λ״̬�µı�����Ϣ
#define INTER_CMD_FAC_CONNECT			0x0343		//���������ն�̽��
#define INTER_CMD_OPEN_AUXUART		0x0345		//�򿪸�������
#define INTER_CMD_CLOSE_AUXUART		0x0346		//�رո�������
#define INTER_CMD_SEND_AUXUART		0x0347		//�������ڷ��ʹ���


#define INTER_CMD_SET_FLI_EN			0x0419		//���÷���ģʽʹ��
#define INTER_CMD_READ_FLI_EN			0x0420		//��ȡ����ģʽʹ��

#define INTER_CMD_SET_ENTRY_FACTORY	0x0424
#define INTER_CMD_SET_EXIT_FACTORY	0x0425


#define INTER_CMD_SENSOR_REC_BEGIN	0x0510
#define INTER_CMD_SENSOR_REC_END		0x0511
#define INTER_CMD_SENSOR_REC_INFO	0x0512
#define INTER_CMD_SENSOR_REC_GET		0x0513

//-----------------------------����ָ���-------------------------------------------
#define INTER_ACK_CALIBRATION_GET		0x8120		//��ȡ��У׼��־Ӧ��
#define INTER_ACK_GET_RUNINFO			0x8100		//��ȡϵͳ����״̬Ӧ��
#define INTER_ACK_GET_GPSSTATUS		0x8101		//��ȡGPS ����״̬Ӧ��
#define INTER_ACK_GET_NSCK				0x8102		//��ȡ����ͨ��SOCKET ����״̬Ӧ��
#define INTER_ACK_GET_NETINFO			0x8103		//��ȡ������ϢӦ��
#define INTER_ACK_GET_BDGPSINFO		0x8104		//��ȡ����GPS ��ϢӦ��
#define INTER_ACK_DIS_CALC				0x8110		//�������������㹤ʽӦ��
#define INTER_ACK_CALIBAR_DATA_GET	0x8121		//��ȡУ׼����

#define INTER_PTL_UPDATE_DEBUGINFO	0x8110		//�ϱ���������
#define INTER_PTL_UPDATE_SMSREC		0x8111		//�ϱ����Ž�������
#define INTER_PTL_UPDATE_SMSSEND		0x8112		//�ϱ����ŷ�������
#define INTER_PTL_UPDATE_GPRSREC		0x8113		//�ϱ�GPRS��������
#define INTER_PTL_UPDATE_GPRSSEND	0x8114		//�ϱ�GPRS ��������

#define INTER_ACK_SET_PARAM			0x8201		//�����������ò���Ӧ��
#define INTER_ACK_READ_PARAM			0x8202		//�����ȡ���ò���Ӧ��
#define INTER_ACK_READ_IP				0x8302		//��ȡIP��ϢӦ��
#define INTER_ACK_READ_PORT			0x8304		//��ȡ�˿���ϢӦ��
#define INTER_ACK_READ_APN			0x8306		//��ȡAPN ��ϢӦ��
#define INTER_ACK_TEST_CMD				0x8307 		//����Ӧ��
#define INTER_ACK_READ_DEBUG_EN		0x8309		//���õ���ʹ��
#define INTER_ACK_READ_RSTINFO		0x830A		//��ȡ��λ��Ϣ
#define INTER_ACK_READ_AREAINFO		0x830B		//��ȡ��������Ӧ��
#define INTER_ACK_AREA_WARTEST		0x830C		//���򱨾�����Ӧ��
#define INTER_ACK_SMS_SIMULATION		0x830D		//��������ģ��Ӧ��
#define INTER_ACK_SHUTDOWN_SYSTEM	0x830F		//ϵͳ�ػ�Ӧ��
#define INTER_ACK_NVREAD_TEST			0x8311
#define INTER_ACK_NVWRITE_TEST		0x8312
#define INTER_ACK_SET_GSENSOR_REG	0x8316		//����gsensor�Ĵ���Ӧ��
#define INTER_ACK_GET_GSENSOR_REG	0x8317		//��ȡgsensor �Ĵ���Ӧ��
#define INTER_ACK_ENABLE_SLEEP		0x8318		//��������Ӧ��
#define INTER_ACK_FORBIT_SLEEP			0x8319		//��ֹ����Ӧ��
#define INTER_ACK_CALL_OUT_TEST		0x831A		//�绰��������Ӧ��
#define INTER_ACK_GSENSOR_STUDY		0x831F		//gsensor��ѧϰ����Ӧ��

#define INTER_ACK_GET_IRSENSOR_REG	0x8320		//��ȡIRSENSOR�Ĵ���Ӧ��
#define INTER_ACK_IRSENSOR_STUDY		0x8321		//IRSENSORУ׼Ӧ��
//#define INTER_ACK_GET_IRSENSOR_THL	0x8322		//��ȡIRSENSOR������ֵӦ��
#define INTER_ACK_ALM_TIME_READ		0x8326		//ʱ���ȡӦ��
#define INTER_ACK_ALM_WARN_READ		0x832B		//��ѯ����Ӧ��
#define INTER_ACK_START_RECORD		0x832C		//��ʼ¼��Ӧ��
#define INTER_ACK_START_AUDIOPLAY		0x832E		//��Ƶ��������Ӧ��

#define INTER_ACK_GET_BLIND_INFO		0x8330		//��ȡä���������״̬��ϢӦ��

#define INTER_ACK_GENERALIO_INIT		0x8333		//ͨ��IO ��ʼ��Ӧ��
#define INTER_ACK_GENERALIO_WRITE		0x8334		//ͨ��IO д����Ӧ��
#define INTER_ACK_GENERALIO_READ		0x8335		//ͨ��IO ������Ӧ��
#define INTER_ACK_GET_RST_WARNINFO	0x8338		//��ȡ��λ״̬�µı�����ϢӦ��
#define INTER_ACK_FAC_CONNECT			0x8343		//���������ն�̽��Ӧ��
#define INTER_ACK_OPEN_AUXUART		0x8345		//�򿪸�������Ӧ��
#define INTER_ACK_CLOSE_AUXUART		0x8346		//�رո�������
#define INTER_ACK_SEND_AUXUART		0x8347		//�������ڷ��ʹ���Ӧ��

#define INTER_PTL_TEST_I2C_GDATA		0x8374		//gSensor ��������

#define INTER_ACK_READ_SYNC_TIME_MODE	0x8416	//��ȡ�ն�УʱģʽӦ��

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
