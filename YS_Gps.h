#ifndef YS_GPS_H
#define YS_GPS_H
#include "Data_type.h"

#define YS_GPS_BUF_LEN				1024

//system data for star info
typedef struct
{
	u8 StarNum;			//总星数
	u8 StarCode[24]; 		//星编号
	u8 StarSign[24];  		//信号值
}t_Gps_Star_Info;

//system data for position info
typedef struct
{
	bool   	Effective;		// 定位标志
	bool	 	EastORWest;		//东西经标志
	bool	  	SouthORNorth;	//南北纬标志
	u8		Precision;		//HDOP
	u16		Speed;			//速度
	u16     	Course;			//航向
	u8 		Longitude[4];	//经度
	u8 		Latitude[4];		//纬度
	u8	    	DataTime[6];		//时间日期
 }t_Gps_Data_Info;

typedef struct
{
    bool    Dr_Effective;
    u8      Dr_StablePointCount;
    u8      Dr_DriftPointCount;
    u8      Dr_TmpLongitude[4];
    u8      Dr_TmpLatitude[4];
    u8      Dr_Longitude[4];
    u8      Dr_Latitude[4];
}t_Gps_Drift_Info;//GPS漂移参数

typedef struct
{
	u16 		CollectTimes;
}t_Avl_Gps_Manage;

//system data for position string
typedef struct
{
 	char    	v_Effective;
	char    	v_EastORWest;
	char	   	v_SouthORNorth;
	char       v_Reserv1;

      char   	va_Longitude[16];
	char		va_Latitude[16];
	char		va_Date[12];
	char		va_Time[16];
	char		va_Speed[8];
	char		va_Course[8];
}t_Gps_Gprmc_Info;

//use to parase star info
typedef  struct
{
		bool			TestStar;
		bool			TestOK;
		u8			LineCount;
		u8			StarCount;
		u8			RecNum;
		u8			RecWP;
}t_Gps_Star_Parse_t;

typedef struct
{
	bool wmEnable;
	bool BankStatus;
	bool FirTestFlag;
	u8   StatusCount;
}t_fj_Area_Deal;

typedef struct
{
	u16 DataLen;
	u8   AddEnable;
	u8	Reserve1;
	u8	DataBuf[YS_GPS_BUF_LEN];
}t_Gps_Parsae_Buf;

typedef struct
{
    bool		FirstCalc;
    u8		Reserve1;
    u16        	DisUnit;

    u8     		ResetLongi[4];
    u8     		ResetLati[4];
    u8     		BankLongi[4];
    u8     		BankLati[4];

    u16 		SaveTimeCount;
    double  	BankSpeed;
    u32		BankTime;
    u32     	mode;
    u32        	DistanceValue;
    u32        	DisAddSave;
    bool    	BankEffective;
    bool    	FlagEffective;
    u16      	BankSpeedCount;
    u16     	BankSpeedBuf[10];
    bool    	FlagCarStop;
    u16     	BankCourseCount;
    u16     	BankCourseBuf[10];
    bool    	FlagCarTurn;
    u8      		DisLongi[4];
    u8      		DisLati[4];
} t_Dis_Calc_Manage;

extern void YS_GpsGetPosData(t_Gps_Data_Info *pt_GetGpsData);

extern void YS_GpsGetGprmcData(t_Gps_Gprmc_Info *pt_GetGpsGprmc);

extern void YS_GpsGetStarInfo(t_Gps_Star_Info *pt_GetGpsStar);

extern void YS_GpsGetBDInfo(t_Gps_Star_Info *pt_GetGpsStar);

extern void YS_GpsSysInitDeal(void);

extern void YS_GpsSetPosStatusNot(void);

extern void YS_GpsRefreshLastPoint(void);

extern void YS_GpsBufAddData(u8 *dbuf, u16 dlen);

extern void YS_GpsDriftFilterDeal(void);

extern u16 YS_GpsGetCollectTimes(void);

extern void YS_GpsInitAreaDeal(void);

extern void YS_GpsInitDistance(void);


#endif

