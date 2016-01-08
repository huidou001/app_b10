#ifndef _YS_GPRS_OBD_H
#define _YS_GPRS_OBD_H
#include "YS_TypeDef.h"


extern u16 YS_GprsPackRsqServerPackage(u8 *PackBuf);

extern u16 YS_GprsPackLogPackage_obd(u8 *PackBuf);

extern u16 YS_GprsPackHeartPackage_obd(u8 *PackBuf);

extern u16 YS_GprsPackGpsPackage(u8 *PackBuf);

extern u16 YS_GprsPackCarObdPackage(u8 *PackBuf);

extern u16 YS_GprsPackWarnPackage(u8 *PackBuf);

extern u16 YS_GprsPackObdDstPackage(u8 *PackBuf);

extern bool YS_GprsSckPtlUpConver(void);
#endif
