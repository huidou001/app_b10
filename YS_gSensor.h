#ifndef _YS_GSENSOR_H
#define _YS_GSENSOR_H

#include "YS_TypeDef.h"

//#include "YS_TypeDef.h"
//映射high-g 终端到INT1
#define INT1_MAP 		0X19
#define INT1_MAP_VALUE 	0X02

typedef struct
{
	u8	ActDelay;
	u8	SignEffecFlag;
	u16	VibSignDelay;
}t_gSensor_Manage;


typedef struct	//振动芯片，自校准管理
{
	u8	FuncEnable;
	u8	StudyValue;
	u8	StudyFlow;
	u8	VibSignFlag;
	u8	CheckDelay;
	u8   CheckCount;
}t_gSensor_Chip_Study;

enum
{
	GSTUDY_FLOW_INIT,
	GSTUDY_FLOW_CHIP_CHECK,
	GSTUDY_FLOW_VIB_CHECK,
	GSTUDY_FLOW_VIB_CHECK_WAIT,
	GSTUDY_FLOW_EXIT
};

//开启high-g中断
#define INTERRUPT_ENGINES0 0x16
#define INTERRUTP_ENGINES0_VALUE 0X00
#define INTERRUPT_ENGINES1 0x17
#define INTERRUTP_ENGINES1_VALUE 0X07
#define INTERRUPT_ENGINES2 0x18
#define INTERRUTP_ENGINES2_VALUE 0X00

#define SHAKE_THRESHOLD 0X26        //设置震荡阈值寄存器地址0x26

//设置中断模式
#define INTERRUPT_MODE 0X21
#define	INTERRUPT_MODE_VALUE 0x0E
//选择震荡精度
#define G_RANG_SEL 		0X0F
#define G_RANG_SEL_VAL1 0X03              	// //2g  3.91mg
#define G_RANG_SEL_VAL2 0x05             	////4g  7.81mg
#define G_RANG_SEL_VAL3 0x08 		 	//8g  15.62mg
#define G_RANG_SEL_VAL4 0x0c            		//16g 31.25mg
//high_g中断保持时间
#define HIGH_G_DELAY 	   		0X25
#define HIGH_G_DELAY_VALUE 	0x10      //中断保持 (0x10+1)*2ms

#define KXSD9_WRITE			0x30  //0011 0000
#define KXSD9_READ			0x31  //0011 0001

extern bool YS_GSensorTest(void);

extern void YS_GSensorInitDeal(void);

extern void YS_GsenserAutoGetVibSign(void);

extern void YS_GSensorWorkStatusInit(void);

extern void YS_GSensorSetRegExtern(u8 RegName, u8 Value);

extern u8 YS_GSensorReadRegExtern(u8 RegName);

extern void YS_GSensorInitChip(void);

extern void YS_GSensorRunMoni(void);

extern void YS_GSensorStudyInterface(void);

extern void YS_GSensorRunTotal(void);

extern void YS_GSensorRunSource(void);

extern void YS_GSensorIntIRQ(void);

extern void YS_GSensorInitInterface(void);

#endif

