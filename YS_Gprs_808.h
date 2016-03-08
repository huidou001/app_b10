#ifndef _YS_GPRS_808_H
#define _YS_GPRS_808_H
#include "YS_TypeDef.h"

extern u16 YS_GprsSendAckPackage(u8 *PackBuf, u8 *AddInfo, u8 rlt);

extern u16 YS_GprsPackRegPackage(u8 *PackBuf);

extern u16 YS_GprsPackLogPackage(u8 *PackBuf);

extern u16 YS_GprsPackHeartPackage(u8 *PackBuf);

extern u16 YS_GprsPackPrmAckPackage(u8 *PackBuf, u8 *AddInfo);

extern u16 YS_GprsPackPosPackage(u8 *PackBuf);

extern u16 YS_GprsPackCarCtlAckPackage(u8 *PackBuf, u8 *AddInfo);

extern u16 YS_GprsPackDeadPosPackage(u8 *PackBuf);

extern u16 YS_GprsSendPointPackage(u8 *PackBuf, u8 *AddInfo);

extern u16 YS_GprsCreatePosRsqInfo(u8 *PackBuf);

extern bool YS_GprsDealJTBServRecData(u8 *buf, u16 len);

extern bool YS_GprsJBTSckPtlUpConver(void);

extern u16 YS_GprsPackObdInfoAckPackage(u8 *PackBuf, u8 *dbuf, u16 dlen);
#endif