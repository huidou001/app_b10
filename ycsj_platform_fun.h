#ifndef __YCSJ_PLATFORM_FUN_H__
#define __YCSJ_PLATFORM_FUN_H__

#include "data_type.h"
#define RUN_MODE_FLIGHT			1
#define RUN_MODE_NORMAL			0

extern  void ycsj_debug(const char* format, ...);

extern void sjfun_debug(char *DebugStr, int Value);

extern void sjfun_timer(U16 timerid, U32 delay, FuncPtr funcPtr);

extern void sjfun_stoptimer(u16 timerid);

extern void sjfun_Debug_Uart_send(U8 *dbuf, U16 dlen);

extern void sjfun_param_read(u16 PrmID, u16 dlen, u8 *FillBuf);

extern void sjfun_param_write(u16 PrmID, u16 dlen, u8 *dbuf );

extern void sjfun_param_Quickwr(void);

extern void sjfun_Set_GpsLed(u8 Status);

extern void sjfun_Set_GsmLed(u8 Status);

extern int get_module_ver(unsigned char *p_buf,int len_buf);

extern void *sjfun_Get_SysVar(char * p_name_variable);

extern U32 sjfun_PPP_Interface(char *apn, char *ApnUser, char *ApnPassWord);

extern bool sjfun_Socket_Create(U32 AcctID, U8 *SocketID, U8 *IPBuf, U16 Port, U8 SocketType);

extern U32 sjfun_Socket_Send(U8 SocketID, U8 *dbuf, U16 dlen);

extern void sjfun_Socket_Close(U8 SocketID);

extern void sjfun_System_Assert(void);

extern U8 sjfun_DW_Rsq_FileBuf(U32 FileSize);

extern U8 sjfun_DW_Write_PackData(U32 FilePos, U8 *dbuf, U16 dlen);

extern U8 sjfun_DW_Read_PackData(U32 FilePos, U8 *dbuf, U16 dlen);

extern U8 sjfun_DW_Calc_FileVerify(U32 FileSize);

extern U8 sjfun_DW_Update_File(U32 FileSize);

extern void sjfun_Sys_Sleep_Enable(void);

extern void sjfun_Sys_Sleep_Disable(void);

extern u8 sjfun_Flight_Mode_Cut(u8 ID);

extern u8 sjfun_Sms_Send_Interface(u8 * pNumber, u8 pNumberLen, u8* pContent, u16 pContentLen,bool FormChangeFlag);

extern void sjfun_Gpio_Set_Mode(u16 GpioID, u8 Value);

extern void sjfun_Gpio_Set_Direction(u16 GpioID, u8 Value);

extern void sjfun_Gpio_Set_PullEN(u16 GpioID, u8 Value);

extern void sjfun_Gpio_Set_PullValue(u16 GpioID, u8 Value);

extern void sjfun_Gpio_Write_Value(u16 GpioID, u8 Value);

extern u8 sjfun_Gpio_Read_Value(u16 GpioID);

//extern void sjfun_Eint_reg(u8 eintno, bool Dbounce_En, bool ACT_Polarity, void(reg_hisr)(void), bool auto_umask);

extern void sjfun_Eint_DebounceModify(u8 eintno,u8 debounce_time);

extern void sjfun_Get_System_DateTime(u8 *DateTime);

extern void sjfun_SysGetBarCodeValue(u8 *GetBuf);

extern u32 sjfun_SysStrlen(u8 *InputBuf);

extern void sjfun_WriteFilesToNV(void);

extern void sjfun_Set_ChargeLed(u8 Status);

extern u8 sjfun_Dns_ParaseInterface(char *DnsUrl, u32 AcctID, u8 *IpBuf,u8 DnsType);

extern void sjfun_Begin_RsqNetInfo(void);

extern void sjfun_Set_System_DateTime(u8 *DateTime);

extern void sjfun_Gpio_EINT_SetPolarity(u8 EIntNO, u8 Status);

extern void sjfun_Gpio_EINT_SetDoubleTime(u8 EIntNO, u8 Value);

extern void sjfun_Gpio_EINT_RegInt(u8 EIntNO, u8 Status, FuncPtr funcPtr);

extern void sjfun_Gps_Stop(bool GpsPow);

extern void sjfun_Gps_Start(bool GpsPow);

extern void sjfun_Gps_WriteAgps(u8 *AgpsBuf,u16 Len);

extern void sjfun_request_reset(void);

extern void sjfun_req_mlocate_ncinfo(void);

extern void sjfun_Get_NV_IMEI(u8 *dbuf, u16 dlen);

extern void sjfun_GPS_Entry_PSM(void);

extern u16 sjfun_GetCaliBartionData(u8 *OutBuf);

extern void sjfun_SetKPLED(u8 Status);

extern void sjfun_sys_reset(void);

extern void sjfun_TurnOn_bt(void);

extern bool sjfun_Get_Power_Statue(void);

extern bool sjfun_Get_Chr_Statue(U8 Flag);

extern void sjfun_Set_Alm_Warn(u8 *dbuf, u16 dlen);

extern u16 sjfun_Read_Alm_Warn(u8 *dbuf, u16 dlen, u8 *OutBuf);

extern u8 sjfun_Start_Record(u8 RecTime);

extern void sjfun_Stop_Record(void);

extern u8 sjfun_Start_Audio_Play(void);

extern void sjfun_Gps_Com_send(U8 *dbuf, U16 dlen);

extern void sjfun_Shut_Down_send(void);

extern void sjfun_Open_AuxUart(void);

extern void sjfun_Close_AuxUart(void);

extern void sjfun_Aux_Com_send(U8 *dbuf, U16 dlen);

extern u8 sjfun_SkyLine_Mode(void);

extern void sjfun_Debug_Agps_Write(U8 *dbuf, U16 dlen);

extern void sjfun_VmcSignControl(u8 id);

extern u8 sjfun_VmcGetOCStatus(void);

extern void sjfun_ucs2totext_str(u8 *dst, u32 size, u8 *src, u8 num);
#endif
