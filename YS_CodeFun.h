#ifndef _YS_CODEFUN_H
#define _YS_CODEFUN_H

#include "Data_type.h"

extern u8 YS_CodeMouthHexConver(char *Str);

extern void YS_CodeHextoString(u8 *buf,u16 len, u8 *p);

extern u16 YS_CodeBufRealLen(u8 *buf, u16 len);

extern bool  YS_CodePDNumString(char *str);

extern bool YS_CodeCretaeBJDT(u8 *buf);

extern bool YS_CompTwoString(char *InputStr, char *CmdStr);

extern u16 YS_CodeGetItemInBuf(u8 *dbuf, u16 dlen, u8 *PackBuf,u8 ID,u8 KeyChar, u16 MaxLen);

extern u8 YS_CodeItoa(u8 dat);

extern u8 YS_CodeOneByteToBCD(u8 Dat);

extern u32 YS_CodeCalcDiffSec(u16 StartYear, u8 *CurTime);

extern u16 YS_CodeCrcCaculate(u8 *msg, u16 msglen);

extern u16 YS_CodeCreateGodeLink(u8 *PackBuf, u8 *Longitude, u8 *Latitude,u8 OnlyLink);

extern u16 YS_CodeCreateGoogleLink(u8 *PackBuf, u8 *Longitude, u8 *Latitude,u8 OnlyLink);

extern u8  YS_Change(u8 *Dat, u8 len, u8 *OutBuf);

extern u32 YS_CodeCalcTwoPointDisFine(u8 *Longi1, u8 *Lati1, u8 *Long2, u8 *Lati2);

extern bool  YS_CodeTestIPString(char *str);

extern u32 YS_CodeaCharHexToInt(char *str);

extern void YS_UtcTimeToTime(u32 UtcTemp,u8 *PackBuf);

extern bool  YS_CodePDNumString2(char *str);

extern u8	YS_CodeUTF8ToUCS2Conver(u8 *InputBuf, u16 InputLen, u8 *OutBuf, u16 *OutBufLen);

extern u16 YS_CodeCalcTowPointTest(u8 *dbuf, u16 dlen, u8 *PackBuf);

extern u8 YS_CodeHexCharConver(u8 Dat);

#endif
