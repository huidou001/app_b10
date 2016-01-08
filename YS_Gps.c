/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Gps.c  2013-06-13
此文件用于对输入的GPS数据进行解析，封装成标装数据用
存放在数据结构中供其它程序调用。
修改计录：
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

enum
{
    AREA_COMP_BIG,
    AREA_COMP_SMALL,
    AREA_COMP_EQUAL
};

enum
{
    AREA_STATUS_OUT,
    AREA_STATUS_IN,
    AREA_STATUS_BUF
};

t_Gps_Parsae_Buf			t_GpsParaseBuf;
t_Gps_Data_Info			t_GpsDataInfo;
t_Gps_Gprmc_Info			t_TempGprmcInfo;
t_Gps_Gprmc_Info			t_GpsGprmcInfo;

t_Avl_Gps_Manage			t_AvlGpsManage;

//GPS 星历解析
t_Gps_Star_Info			t_GpsStarInfo;
t_Gps_Star_Parse_t		t_GpsStarParse;
t_Gps_Star_Info			t_TempStarInfo;

//北斗 星历解析
t_Gps_Star_Info			t_BDStarInfo;
t_Gps_Star_Parse_t		t_BDStarParase;
t_Gps_Star_Info			t_TempBDStar;

t_Gps_Drift_Info        		t_GpsDriftInfo;
t_fj_Area_Deal			t_fjAreaDeal;
t_Dis_Calc_Manage		t_DistanceCalc;

kal_uint8 	Gps_Data_Rec_Buf[200];
kal_uint16 	Gps_Data_rRec_Len;

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsBufInit
功能说明：GPS 接收缓冲区处理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsBufInit(void)
{
    t_GpsParaseBuf.AddEnable=TRUE;
    t_GpsParaseBuf.DataLen=0;
}


/*-----------------------------------------------------------------------------------------
函数名：YS_GpsDriftInfoInit
功能说明：GPS漂移相关参数初始化
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsDriftInfoInit(void)
{
    u8 gpsbuf[8],i;

    t_GpsDriftInfo.Dr_Effective = FALSE;
    t_GpsDriftInfo.Dr_StablePointCount = 0;
    t_GpsDriftInfo.Dr_DriftPointCount = 0;

    YS_PrmReadOneItem(FLH_PRM_LAST_GPSPOINT, FLH_PRM_LAST_GPSPOINT_LEN, gpsbuf);
    for(i=0; i<4; i++)
    {
        t_GpsDriftInfo.Dr_Longitude[i]=gpsbuf[i];
        t_GpsDriftInfo.Dr_Latitude[i]=gpsbuf[i+4];
        t_GpsDriftInfo.Dr_TmpLongitude[i] =gpsbuf[i];
        t_GpsDriftInfo.Dr_TmpLatitude[i] = gpsbuf[i+4];
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsGetStrFloatLen
功能说明：获取浮点字符串小数部份的长度
修改记录：
-------------------------------------------------------------------------------------------*/
u8 YS_GpsGetStrFloatLen(char *str)
{
    u8	len,count,i;
    u8	result;

    len=strlen(str);
    count=0;
    for(i=0; i<len; i++)
    {
        if(str[i]=='.')
        {
            break;
        }
        count++;
    }
    result=len-count-1;
    return(result);
}


/*-----------------------------------------------------------------------------------------
函数名：
功能说明：
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GpsOneLineVerify(u8 *dbuf, u16 dlen)
{
    u8 verify1,verify2;
    u16 i;

    if(dlen<10)
    {
        return(FALSE);
    }

    verify1=0;
    for(i=1; i<dlen-5; i++)
    {
        verify1=verify1^dbuf[i];
    }
    verify2=YS_CodeHexCharConver(dbuf[dlen-4])*16+YS_CodeHexCharConver(dbuf[dlen-3]);
    if(verify1==verify2)
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}



/*-----------------------------------------------------------------------------------------
函数名：YS_GpsDriftFilterDeal
功能说明：GPS漂移过滤处理(GPS已定位)
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsDriftFilterDeal(void)
{
    u32 TwoPointDis,i;//SecDis
    u32 CompareDis=0;
    u8 fbuf[4];
    static bool FirstPoint=TRUE;

    YS_PrmReadOneItem(FLH_PRM_NV_GPS_PYYH, FLH_PRM_NV_GPS_PYYH_LEN, fbuf);
    if(fbuf[0] == 0)//未使能GPS漂移功能
    {
        return;
    }

    if(FirstPoint==TRUE) //开机第一个定位点不进行滤波处理
    {
        FirstPoint=FALSE;
//		YS_GpsdriftFilterDebug("First point deal",0);
        for(i=0; i<4; i++)
        {
            t_GpsDriftInfo.Dr_Longitude[i]=t_GpsDataInfo.Longitude[i];
            t_GpsDriftInfo.Dr_Latitude[i]=t_GpsDataInfo.Latitude[i];
            t_GpsDriftInfo.Dr_TmpLongitude[i] =t_GpsDataInfo.Longitude[i];
            t_GpsDriftInfo.Dr_TmpLatitude[i] = t_GpsDataInfo.Latitude[i];
        }
        return;
    }

    if(t_GpsDriftInfo.Dr_Effective == FALSE)//首次定位需要取稳定点
    {
        TwoPointDis = YS_CodeCalcTwoPointDisFine(t_GpsDataInfo.Longitude, \
                      t_GpsDataInfo.Latitude, t_GpsDriftInfo.Dr_TmpLongitude, t_GpsDriftInfo.Dr_TmpLatitude);
//		YS_GpsdriftFilterDebug("Get firm point dis",TwoPointDis);
        for ( i = 0; i<4; i++ )
        {
            t_GpsDriftInfo.Dr_TmpLongitude[i] =t_GpsDataInfo.Longitude[i];
            t_GpsDriftInfo.Dr_TmpLatitude[i] = t_GpsDataInfo.Latitude[i];
        }

        /*if(TRUE == YS_GpsEffectiveForStar())//GPS信号满足
        {
        	t_GpsDriftInfo.Dr_Effective=TRUE;
        	t_GpsDriftInfo.Dr_DriftPointCount=0;
        	YS_GpsdriftFilterDebug("Get firm point OK by star",0);
        	YS_GpsRefreshLastPoint();//刷新备份点
        }
        else */
        if(TwoPointDis<350)//距离小于350M
        {
            t_GpsDriftInfo.Dr_StablePointCount++;
            if(t_GpsDriftInfo.Dr_StablePointCount>=15)//取到稳定点
            {
                t_GpsDriftInfo.Dr_Effective=TRUE;
                t_GpsDriftInfo.Dr_DriftPointCount=0;
//				YS_GpsdriftFilterDebug("Get firm point OK by dis",0);
	    			YS_GpsRefreshLastPoint();//刷新备份点
                for ( i = 0; i<4; i++ )//取备份点
                {
                    t_GpsDriftInfo.Dr_Longitude[i] =t_GpsDataInfo.Longitude[i];
                    t_GpsDriftInfo.Dr_Latitude[i] = t_GpsDataInfo.Latitude[i];
                }
            }
        }
        else
        {
            t_GpsDriftInfo.Dr_StablePointCount=0;
        }

        for ( i = 0; i<4; i++ )//设置当前数据为备份点数据
        {
            t_GpsDataInfo.Longitude[i] = t_GpsDriftInfo.Dr_Longitude[i];
            t_GpsDataInfo.Latitude[i] = t_GpsDriftInfo.Dr_Latitude[i];
        }
    }
    else//取到稳定点通过计算距离过滤漂移点
    {
        TwoPointDis = YS_CodeCalcTwoPointDisFine(t_GpsDataInfo.Longitude, \
                      t_GpsDataInfo.Latitude, t_GpsDriftInfo.Dr_Longitude, t_GpsDriftInfo.Dr_Latitude);

//		YS_GpsdriftFilterDebug("Get change point dis calc",TwoPointDis);
        CompareDis = t_GpsDriftInfo.Dr_DriftPointCount*350+350;//电动车每秒距离
//		YS_GpsdriftFilterDebug("Get change point allow",CompareDis);
        if(TwoPointDis<=CompareDis)//距离小于等于100M
        {
            t_GpsDriftInfo.Dr_DriftPointCount=0;
            for ( i = 0; i<4; i++ )//更新备份点(取当前点)
            {
                t_GpsDriftInfo.Dr_Longitude[i] =t_GpsDataInfo.Longitude[i];
                t_GpsDriftInfo.Dr_Latitude[i] = t_GpsDataInfo.Latitude[i];
            }
        }
        else//大于100M
        {
            t_GpsDriftInfo.Dr_DriftPointCount++;
//			YS_GpsdriftFilterDebug("dis bigger allow count",t_GpsDriftInfo.Dr_DriftPointCount);
            if(t_GpsDriftInfo.Dr_DriftPointCount>=18)//连续漂移超18个点
            {
                t_GpsDriftInfo.Dr_Effective = FALSE;
                t_GpsDriftInfo.Dr_StablePointCount=0;
                for ( i = 0; i<4; i++ )//取备份点
                {
                    t_GpsDriftInfo.Dr_TmpLongitude[i] = t_GpsDriftInfo.Dr_Longitude[i];
                    t_GpsDriftInfo.Dr_TmpLatitude[i] = t_GpsDriftInfo.Dr_Latitude[i];
                }
            }
            for ( i = 0; i<4; i++ )//取备份点
            {
                t_GpsDataInfo.Longitude[i] = t_GpsDriftInfo.Dr_Longitude[i];
                t_GpsDataInfo.Latitude[i] = t_GpsDriftInfo.Dr_Latitude[i];
            }
        }
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsParaseGNRMCInfo
功能说明：分解处理GPRMC数据，并存贮到相关数据结构中
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GpsParaseGNRMCInfo(u8 *buf, u16 len)
{
    u16 i,pos,loop,count,tpos;

    pos=0;
    loop=len;
    while(loop>6)
    {
        if((buf[pos]=='$')&&(buf[pos+1]=='G')&&(buf[pos+2]=='N')&&(buf[pos+3]=='R')&&(buf[pos+4]=='M')&&(buf[pos+5]=='C'))
        {
            pos+=6;
            count=0;
            for(i=pos; i<len; i++)	//检测','的个数，是否为一条完整的$GPRMC信息
            {
                if(buf[i]==',')
                {
                    count++;
                }
                if(count>=12)
                {
                    break;
                }
            }
            if(count<10)//信息不完整,删除当前数据
            {
                loop--;
                pos++;
                continue;
            }

            count=0;//检测是否有'A'或'V'信息
            tpos=pos;
            for(i=pos; i<len; i++)
            {
                if(buf[i]==',')
                {
                    count++;
                }
                if(count==2)
                {
                    tpos=i+1;
                    break;
                }
            }

            if((buf[tpos]!='A')&&(buf[tpos]!='V'))
            {
                loop--;
                pos++;
                continue;
            }

            tpos=pos;	//定位到第一个','
            for(i=tpos; i<len; i++)
            {
                if(buf[i]==',')
                {
                    break;
                }
                pos++;
            }
            pos++;

            tpos=pos;//开始取时间信息
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<16)
                {
                    t_TempGprmcInfo.va_Time[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            t_TempGprmcInfo.va_Time[count]=0;

            pos++;//定位到第二个','之后开始取定位标志
            t_TempGprmcInfo.v_Effective=buf[pos];
            tpos=pos;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                pos++;
            }

            pos++;	//定位到第三个','之后开始取纬度信息
            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<16)
                {
                    t_TempGprmcInfo.va_Latitude[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            t_TempGprmcInfo.va_Latitude[count]=0;

            pos++;	//定位到第四个','之后开始取南北纬标志
            if(buf[pos]!=',') //maybe some gps not have falg when unpos
            {
                t_TempGprmcInfo.v_SouthORNorth=buf[pos];
            }

            tpos=pos;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                pos++;
            }

            pos++;//定位到第五个','之后开始取经度信息
            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<16)
                {
                    t_TempGprmcInfo.va_Longitude[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            t_TempGprmcInfo.va_Longitude[count]=0;

            pos++;//定位到第六个','之后开始取东西经标志
            if(buf[pos]!=',') //maybe some gps not have falg when unpos
            {
                t_TempGprmcInfo.v_EastORWest=buf[pos];
            }
            tpos=pos;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                pos++;
            }

            pos++;//定位到第七个','开始取速度信息
            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<9)
                {
                    t_TempGprmcInfo.va_Speed[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            t_TempGprmcInfo.va_Speed[count]=0;

            pos++;//定位到第八个','之后开始取航向信息
            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<9)
                {
                    t_TempGprmcInfo.va_Course[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            t_TempGprmcInfo.va_Course[count]=0;

            pos++;//定位到第九个','之后开始取日期信息
            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<9)
                {
                    t_TempGprmcInfo.va_Date[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            t_TempGprmcInfo.va_Date[count]=0;
            return(TRUE);
        }
        else
        {
            loop--;
            pos++;
        }
    }
    return(FALSE);
}



/*-----------------------------------------------------------------------------------------
函数名：YS_GpsParaseGPRMCInfo
功能说明：分解处理GPRMC数据，并存贮到相关数据结构中
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsWriteSysGpsData(void)
{
    u16 len,tdat;
    char strdat[10];
    //u8 fbuf[10];
    float f;
    u8	i,LenInt,LenFloat,Count,year;
    bool BankEffec;//FirPos;
    u16  CurSpeed;
    static bool FirstGpsPoint=FALSE;

    t_AvlGpsManage.CollectTimes++;	//累加GPS 已采样的次数

    //deal time info
    if(YS_CodePDNumString(t_TempGprmcInfo.va_Time)&&(strlen(t_TempGprmcInfo.va_Time)>=6))
    {
        strdat[0]=t_TempGprmcInfo.va_Time[0]; //hour
        strdat[1]=t_TempGprmcInfo.va_Time[1];
        strdat[2]=0;
        t_GpsDataInfo.DataTime[3]=atoi(strdat);
        strdat[0]=t_TempGprmcInfo.va_Time[2];	//minute
        strdat[1]=t_TempGprmcInfo.va_Time[3];
        strdat[2]=0;
        t_GpsDataInfo.DataTime[4]=atoi(strdat);
        strdat[0]=t_TempGprmcInfo.va_Time[4];	//second
        strdat[1]=t_TempGprmcInfo.va_Time[5];
        strdat[2]=0;
        t_GpsDataInfo.DataTime[5]=atoi(strdat);
        strcpy(t_GpsGprmcInfo.va_Time,t_TempGprmcInfo.va_Time);
    }

    BankEffec=t_GpsDataInfo.Effective;		//保存上一次是否定位的标志
    if(t_TempGprmcInfo.v_Effective=='A')	//设置当前的定位数据的标识
    {
        t_GpsDataInfo.Effective=TRUE;
        t_GpsGprmcInfo.v_Effective='A';
    }
    else
    {
        t_GpsDataInfo.Effective=FALSE;
        t_GpsGprmcInfo.v_Effective='V';
    }

    //deal date info
    if((YS_CodePDNumString(t_TempGprmcInfo.va_Date))&&(strlen(t_TempGprmcInfo.va_Date)>=6))
    {
        strdat[0]=t_TempGprmcInfo.va_Date[4];  //year
        strdat[1]=t_TempGprmcInfo.va_Date[5];
        strdat[2]=0;
        year=atoi(strdat);
        if((year<80)||(t_GpsDataInfo.Effective))
        {
            t_GpsDataInfo.DataTime[0]=year;
            strdat[0]=t_TempGprmcInfo.va_Date[2];  //month
            strdat[1]=t_TempGprmcInfo.va_Date[3];
            strdat[2]=0;
            t_GpsDataInfo.DataTime[1]=atoi(strdat);
            strdat[0]=t_TempGprmcInfo.va_Date[0];  //day
            strdat[1]=t_TempGprmcInfo.va_Date[1];
            strdat[2]=0;
            t_GpsDataInfo.DataTime[2]=atoi(strdat);
            strcpy(t_GpsGprmcInfo.va_Date,t_TempGprmcInfo.va_Date);
        }
    }

    //deal speed info
    CurSpeed=0;
    if(t_GpsDataInfo.Effective==TRUE)
    {
        if(YS_CodePDNumString(t_TempGprmcInfo.va_Speed)==TRUE)
        {
            f=atof(t_TempGprmcInfo.va_Speed);
            f*=1.852;
            tdat=(u16)f;
            if(tdat<=5) //5公里以下，速度数据无效
            {
                t_GpsDataInfo.Speed=0;
                strcpy(t_GpsGprmcInfo.va_Speed,"00.000");
            }
            else
            {
                t_GpsDataInfo.Speed=tdat;
                strcpy(t_GpsGprmcInfo.va_Speed,t_TempGprmcInfo.va_Speed);
            }
        }
    }
    else //如果GPS 未定位，将速度值清零
    {
        t_GpsDataInfo.Speed=0;
        strcpy(t_GpsGprmcInfo.va_Speed,"00.000");
    }

    //deal course info
    if(YS_CodePDNumString(t_TempGprmcInfo.va_Course))
    {
        if(t_GpsDataInfo.Speed>0)
        {
            f=atof(t_TempGprmcInfo.va_Course);
            if(f<360)
            {
                tdat=(u16)f;
                t_GpsDataInfo.Course=tdat;
                strcpy(t_GpsGprmcInfo.va_Course,t_TempGprmcInfo.va_Course);
            }
        }
    }

    //deal eastwest  south north flag
    if(t_GpsDataInfo.Effective==TRUE)
    {
        if(t_TempGprmcInfo.v_EastORWest=='W')
        {
            t_GpsDataInfo.EastORWest=FALSE;
            t_GpsGprmcInfo.v_EastORWest='W';
        }
        else
        {
            t_GpsDataInfo.EastORWest=TRUE;
            t_GpsGprmcInfo.v_EastORWest='E';
        }

        if(t_TempGprmcInfo.v_SouthORNorth=='S')
        {
            t_GpsDataInfo.SouthORNorth=FALSE;
            t_GpsGprmcInfo.v_SouthORNorth='S';
        }
        else
        {
            t_GpsDataInfo.SouthORNorth=TRUE;
            t_GpsGprmcInfo.v_SouthORNorth='N';
        }
    }
    else
    {
        t_GpsDataInfo.EastORWest=TRUE;		//default is east north
        t_GpsDataInfo.SouthORNorth=TRUE;
        t_GpsGprmcInfo.v_EastORWest='E';
        t_GpsGprmcInfo.v_SouthORNorth='N';
    }

    if((YS_CodePDNumString(t_TempGprmcInfo.va_Longitude))&&(strlen(t_TempGprmcInfo.va_Longitude)>=7)&&(t_GpsDataInfo.Effective))
    {
        len=strlen(t_TempGprmcInfo.va_Longitude);
        LenFloat=YS_GpsGetStrFloatLen(t_TempGprmcInfo.va_Longitude);
        LenInt=len-LenFloat-1;
        if((LenInt>=3)&&(LenFloat>=3)&&(LenInt<6))
        {
            Count=0;
            for(i=0; i<(LenInt-2); i++)								//Trad
            {
                strdat[i]=t_TempGprmcInfo.va_Longitude[i];
                Count++;
            }
            strdat[Count]=0;
            t_GpsDataInfo.Longitude[0]=atoi(strdat);
            strdat[0]=t_TempGprmcInfo.va_Longitude[Count];			//Minute
            strdat[1]=t_TempGprmcInfo.va_Longitude[Count+1];
            strdat[2]=0;
            t_GpsDataInfo.Longitude[1]=atoi(strdat);
            strdat[0]=t_TempGprmcInfo.va_Longitude[LenInt+1];     	//Minutef1
            strdat[1]=t_TempGprmcInfo.va_Longitude[LenInt+2];
            strdat[2]=0;
            t_GpsDataInfo.Longitude[2]=atoi(strdat);				//Minutef2
            if(LenFloat==3)
            {
                strdat[0]=t_TempGprmcInfo.va_Longitude[LenInt+3];
                strdat[1]='0';
                strdat[2]=0;
            }
            else
            {
                strdat[0]=t_TempGprmcInfo.va_Longitude[LenInt+3];
                strdat[1]=t_TempGprmcInfo.va_Longitude[LenInt+4];
                strdat[2]=0;
            }
            t_GpsDataInfo.Longitude[3]=atoi(strdat);
            strcpy(t_GpsGprmcInfo.va_Longitude,t_TempGprmcInfo.va_Longitude);
        }
    }

    if((YS_CodePDNumString(t_TempGprmcInfo.va_Latitude))&&(strlen(t_TempGprmcInfo.va_Latitude)>=7)&&(t_GpsDataInfo.Effective))
    {
        len=strlen(t_TempGprmcInfo.va_Latitude);
        LenFloat=YS_GpsGetStrFloatLen(t_TempGprmcInfo.va_Latitude);
        LenInt=len-LenFloat-1;
        if((LenInt>=3)&&(LenFloat>=3)&&(LenInt<6))
        {
            Count=0;							//Trad
            for(i=0; i<(LenInt-2); i++)
            {
                strdat[i]=t_TempGprmcInfo.va_Latitude[i];
                Count++;
            }
            strdat[Count]=0;
            t_GpsDataInfo.Latitude[0]=atoi(strdat);			//Minute
            strdat[0]=t_TempGprmcInfo.va_Latitude[Count];
            strdat[1]=t_TempGprmcInfo.va_Latitude[Count+1];
            strdat[2]=0;
            t_GpsDataInfo.Latitude[1]=atoi(strdat);			//Minutef1
            strdat[0]=t_TempGprmcInfo.va_Latitude[LenInt+1];
            strdat[1]=t_TempGprmcInfo.va_Latitude[LenInt+2];
            strdat[2]=0;
            t_GpsDataInfo.Latitude[2]=atoi(strdat);			//Minutef2
            if(LenFloat==3)
            {
                strdat[0]=t_TempGprmcInfo.va_Latitude[LenInt+3];
                strdat[1]='0';
                strdat[2]=0;
            }
            else
            {
                strdat[0]=t_TempGprmcInfo.va_Latitude[LenInt+3];
                strdat[1]=t_TempGprmcInfo.va_Latitude[LenInt+4];
                strdat[2]=0;
            }
            t_GpsDataInfo.Latitude[3]=atoi(strdat);
            strcpy(t_GpsGprmcInfo.va_Latitude,t_TempGprmcInfo.va_Latitude);
        }
    }

    if(t_GpsDataInfo.Effective==TRUE)	//同步系统时间
    {
        if (FirstGpsPoint == FALSE)//第一次定位更新GPS备份点
        {
            FirstGpsPoint = TRUE;
            YS_GpsRefreshLastPoint();
        }

        YS_SysSyncSystemDT();
        YS_GpsDriftFilterDeal();
    }
}


/*-----------------------------------------------------------------------------------------
函数名：YS_GpsParaseBDGSVInfo
功能说明：分解处理BDGSV数据，并存贮到相关数据结构中
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GpsParaseBDGSVInfo(u8 *buf, u16 len)
{
    u16 i,pos,loop,count,tpos;
    u8	j,LinesCount,StarNum,LinesNO,starloop;
    char strdat[10];
    u8	p,starNO,starSign;

    pos=0;
    loop=len;
    while(loop>6)
    {
        if((buf[pos]=='$')&&(buf[pos+1]=='B')&&(buf[pos+2]=='D')&&(buf[pos+3]=='G')&&(buf[pos+4]=='S')&&(buf[pos+5]=='V'))
        {
            pos+=6;//定位到第一个','
            count=0;
            for(i=pos; i<len; i++)
            {
                if(buf[i]==',')
                {
                    count++;
                }
            }

            if(count<3) //数据不完整
            {
                loop--;
                pos++;
                continue;
            }

            for(i=pos; i<len; i++)//定位到第一个','之后
            {
                if(buf[pos]==',')
                {
                    break;
                }
                pos++;
            }
            pos++;

            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<2)
                {
                    strdat[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            strdat[count]=0;
            if(count==0)	//数据有误
            {
                loop--;
                pos++;
                continue;
            }
            LinesCount=atoi(strdat);  //信息的行数

            pos++;//指向第二个','号之后
            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<2)
                {
                    strdat[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            strdat[count]=0;
            if(count==0)//数据有误
            {
                loop--;
                pos++;
                continue;
            }
            LinesNO=atoi(strdat);//信息的行编号

            pos++;	//指向第3个','之后
            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<2)
                {
                    strdat[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            strdat[count]=0;
            if(count==0)
            {
                loop--;
                pos++;
                continue;
            }
            StarNum=atoi(strdat);
            if(LinesNO==1)  //如果是第一条信息，设置接收的数据结构
            {
                t_BDStarParase.TestStar=TRUE;
                t_BDStarParase.RecNum=0;
                t_BDStarParase.RecWP=0;
                t_BDStarParase.LineCount=LinesCount;
                t_BDStarParase.StarCount=StarNum;
            }

            if(t_BDStarParase.TestStar==TRUE) //如果已接收到了第一行信息
            {
                count=0;
                for(i=pos; i<len; i++)
                {
                    if(buf[i]=='*')
                    {
                        break;
                    }
                    if(buf[i]==',')
                    {
                        count++;
                    }
                }
                count++;	//最后一颗星只有在个','
                starloop=count/4;
                if((starloop==0)||(starloop>4))
                {
                    loop--;
                    pos++;
                    continue;
                }

                for(j=0; j<starloop; j++)	//循环读取一条GPGSV中的所有星的数据
                {
                    pos++;
                    tpos=pos;
                    count=0;
                    for(i=tpos; i<len; i++)
                    {
                        if(buf[pos]==',')
                        {
                            break;
                        }
                        if(count<=3)
                        {
                            strdat[i-tpos]=buf[pos];
                            count++;
                        }
                        pos++;
                    }
                    strdat[count]=0;
                    starNO=atoi(strdat);
                    pos++; //跳过第一个','
                    count=0;
                    tpos=pos;
                    for(i=tpos; i<len; i++)
                    {
                        if(buf[pos]==',')
                        {
                            count++;
                            if(count==2)
                            {
                                break;
                            }
                        }
                        pos++;
                    }
                    pos++;	//指向一颗星的信号值部份
                    tpos=pos;
                    count=0;
                    for(i=tpos; i<len; i++)
                    {
                        if((buf[pos]==',')||(buf[pos]=='*'))
                        {
                            break;
                        }
                        if(count<=3)
                        {
                            strdat[i-tpos]=buf[pos];
                            count++;
                        }
                        pos++;
                    }
                    strdat[count]=0;
                    if(count==0)
                    {
                        starSign=0;
                    }
                    else
                    {
                        starSign=atoi(strdat);
                    }

                    p=t_BDStarParase.RecWP;  //存贮当前的星信息
                    if(p<24)
                    {
                        t_TempBDStar.StarCode[p]=starNO;
                        t_TempBDStar.StarSign[p]=starSign;

                        t_BDStarParase.RecWP++;
                        t_BDStarParase.RecNum++;
                    }
                }//for j

                if((LinesNO==t_BDStarParase.LineCount)&&(t_BDStarParase.RecNum==t_BDStarParase.StarCount))
                {
                    t_TempBDStar.StarNum=t_BDStarParase.StarCount;
                    t_BDStarParase.TestOK=TRUE;

                }
                else
                {
                    t_BDStarParase.TestOK=FALSE;
                }
                return(TRUE);
            }
            else//接收未初始化
            {
                loop--;
                pos++;
                continue;
            }
        }
        else
        {
            loop--;
            pos++;
        }
    }
    return(FALSE);
}


/*-----------------------------------------------------------------------------------------
函数名：YS_GpsParaseGPGSVInfo
功能说明：分解处理GPGSV数据，并存贮到相关数据结构中
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GpsParaseGPGSVInfo(u8 *buf, u16 len)
{
    u16 i,pos,loop,count,tpos;
    u8	j,LinesCount,StarNum,LinesNO,starloop;
    char strdat[10];
    u8	p,starNO,starSign;

    pos=0;
    loop=len;
    while(loop>6)
    {
        if((buf[pos]=='$')&&(buf[pos+1]=='G')&&(buf[pos+2]=='P')&&(buf[pos+3]=='G')&&(buf[pos+4]=='S')&&(buf[pos+5]=='V'))
        {
            pos+=6;//定位到第一个','
            count=0;
            for(i=pos; i<len; i++)
            {
                if(buf[i]==',')
                {
                    count++;
                }
            }

            if(count<3) //数据不完整
            {
                loop--;
                pos++;
                continue;
            }

            for(i=pos; i<len; i++)//定位到第一个','之后
            {
                if(buf[pos]==',')
                {
                    break;
                }
                pos++;
            }
            pos++;

            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<2)
                {
                    strdat[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            strdat[count]=0;
            if(count==0)	//数据有误
            {
                loop--;
                pos++;
                continue;
            }
            LinesCount=atoi(strdat);  //信息的行数

            pos++;//指向第二个','号之后
            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<2)
                {
                    strdat[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            strdat[count]=0;
            if(count==0)//数据有误
            {
                loop--;
                pos++;
                continue;
            }
            LinesNO=atoi(strdat);//信息的行编号

            pos++;	//指向第3个','之后
            tpos=pos;
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==',')
                {
                    break;
                }
                if(count<2)
                {
                    strdat[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            strdat[count]=0;
            if(count==0)
            {
                loop--;
                pos++;
                continue;
            }
            StarNum=atoi(strdat);
            if(LinesNO==1)  //如果是第一条信息，设置接收的数据结构
            {
                t_GpsStarParse.TestStar=TRUE;
                t_GpsStarParse.RecNum=0;
                t_GpsStarParse.RecWP=0;
                t_GpsStarParse.LineCount=LinesCount;
                t_GpsStarParse.StarCount=StarNum;
            }

            if(t_GpsStarParse.TestStar==TRUE) //如果已接收到了第一行信息
            {
                count=0;
                for(i=pos; i<len; i++)
                {
                    if(buf[i]=='*')
                    {
                        break;
                    }
                    if(buf[i]==',')
                    {
                        count++;
                    }
                }
                count++;	//最后一颗星只有在个','
                starloop=count/4;
                if((starloop==0)||(starloop>4))
                {
                    loop--;
                    pos++;
                    continue;
                }

                for(j=0; j<starloop; j++)	//循环读取一条GPGSV中的所有星的数据
                {
                    pos++;
                    tpos=pos;
                    count=0;
                    for(i=tpos; i<len; i++)
                    {
                        if(buf[pos]==',')
                        {
                            break;
                        }
                        if(count<=3)
                        {
                            strdat[i-tpos]=buf[pos];
                            count++;
                        }
                        pos++;
                    }
                    strdat[count]=0;
                    starNO=atoi(strdat);
                    pos++; //跳过第一个','
                    count=0;
                    tpos=pos;
                    for(i=tpos; i<len; i++)
                    {
                        if(buf[pos]==',')
                        {
                            count++;
                            if(count==2)
                            {
                                break;
                            }
                        }
                        pos++;
                    }
                    pos++;	//指向一颗星的信号值部份
                    tpos=pos;
                    count=0;
                    for(i=tpos; i<len; i++)
                    {
                        if((buf[pos]==',')||(buf[pos]=='*'))
                        {
                            break;
                        }
                        if(count<=3)
                        {
                            strdat[i-tpos]=buf[pos];
                            count++;
                        }
                        pos++;
                    }
                    strdat[count]=0;
                    if(count==0)
                    {
                        starSign=0;
                    }
                    else
                    {
                        starSign=atoi(strdat);
                    }

                    p=t_GpsStarParse.RecWP;  //存贮当前的星信息
                    if(p<24)
                    {
                        t_TempStarInfo.StarCode[p]=starNO;
                        t_TempStarInfo.StarSign[p]=starSign;

                        t_GpsStarParse.RecWP++;
                        t_GpsStarParse.RecNum++;
                    }
                }//for j

                if((LinesNO==t_GpsStarParse.LineCount)&&(t_GpsStarParse.RecNum==t_GpsStarParse.StarCount))
                {
                    t_TempStarInfo.StarNum=t_GpsStarParse.StarCount;
                    t_GpsStarParse.TestOK=TRUE;

                }
                else
                {
                    t_GpsStarParse.TestOK=FALSE;
                }
                return(TRUE);
            }
            else//接收未初始化
            {
                loop--;
                pos++;
                continue;
            }
        }
        else
        {
            loop--;
            pos++;
        }
    }
    return(FALSE);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsWriteSystemStarInfo
功能说明：写GPS 星历数据到系统结构体中
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsWriteSystemStarInfo(void)
{
    u8 *pSourc, *pAim;
    u16 len;

    len=sizeof(t_Gps_Star_Info);
    pSourc=(u8 *)(&t_TempStarInfo);
    pAim=(u8 *)(&t_GpsStarInfo);
    memcpy(pAim,pSourc,len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsWriteBDStarInfo
功能说明：写北斗 星历数据到系统结构体中
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsWriteBDStarInfo(void)
{
    u8 *pSourc, *pAim;
    u16 len;

    len=sizeof(t_Gps_Star_Info);
    pSourc=(u8 *)(&t_TempBDStar);
    pAim=(u8 *)(&t_BDStarInfo);
    memcpy(pAim,pSourc,len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsGetStarInfo
功能说明：返回GPS 的采样次数用于测试GPS 芯片是否在工作
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_GpsGetCollectTimes(void)
{
    return(t_AvlGpsManage.CollectTimes);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsParaseGPGGAFormat
功能说明：分解处理GPGGA语句，获取HDOP值
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_GpsParaseGPGGAFormat(u8 *buf, u16 len)
{
    u16 loop,pos,i,count,tpos;
    bool result;
    char DiffStr[20];
    float f;

    pos=0;
    loop=len;
    result=FALSE;
    while(loop>6)
    {
        if((buf[pos]=='$')&&(buf[pos+1]=='G')&&(buf[pos+2]=='P')&&(buf[pos+3]=='G')&&(buf[pos+4]=='G')&&(buf[pos+5]=='A'))
        {
            count=0;
            for(i=pos; i<len; i++)	//检测','的个数，是否为一条完整的$GPGGA信息
            {
                if(buf[i]==',')
                {
                    count++;
                }
                if(count>=14)
                {
                    break;
                }
            }

            if(count<14)	//信息不全，查找下一个有效字符串
            {
                loop--;
                pos++;
                continue;
            }
            count=0;
            tpos=pos;
            for(i=tpos; i<len; i++)
            {

                if(buf[i]==',')
                {
                    count++;
                    if(count>=8)
                    {
                        pos++;
                        break;
                    }
                }
                pos++;
            }
            count=0;
            tpos=pos;
            for(i=tpos; i<len; i++)
            {
                if(buf[i]==',')
                {
                    break;
                }
                if(count<20)
                {
                    DiffStr[count]=buf[i];
                    count++;
                }
            }

            if((count>0)&&(count<6))	//找到一个有效的GPGGA 格式数据时，退出当前数据的检索
            {
                DiffStr[count]=0;
                result=TRUE;
            }
            break;
        }
        else
        {
            loop--;
            pos++;
        }
    }

    if(result==TRUE)
    {
        if((YS_CodePDNumString(DiffStr))&&(strlen(DiffStr)>=3))
        {
            f=atof(DiffStr);
            if(f>20)
            {
                t_GpsDataInfo.Precision=0xFF;		//及不可靠，或未定位
            }
            else
            {
                f*=10;
                t_GpsDataInfo.Precision=(u8)f; 		//定位，且数据在检测小范围之内
            }
        }
        else
        {
            result=FALSE;
        }
    }
    return(result);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsOneLinesDeal
功能说明：
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsOneLinesDeal(u8 *dbuf, u16 dlen)
{
    if(YS_GpsParaseGNRMCInfo(dbuf,dlen))  //解析GNRMC语句
    {
        YS_GpsWriteSysGpsData();
    }
    else if(YS_GpsParaseGPGSVInfo(dbuf, dlen)==TRUE) //解析GPGSV语句，获取星历数据
    {
        if(t_GpsStarParse.TestOK==TRUE)
        {
            YS_GpsWriteSystemStarInfo();
        }
    }
    else if(YS_GpsParaseBDGSVInfo(dbuf, dlen)==TRUE) //解析GPGSV语句，获取星历数据
    {
        if(t_BDStarParase.TestOK==TRUE)
        {
            YS_GpsWriteBDStarInfo();
        }
    }
    else if(YS_GpsParaseGPGGAFormat(dbuf,dlen)==TRUE)//解析GPGGA语句，获取HDOP值
    {
        ;
    }
}

/*-----------------------------------------------------------------------------------------
函数名：
功能说明：
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsDataSourceInput(u8 *dbuf, u16 dlen)
{
    u16 i,len,tpos,count;
    bool GetTailOK;
    bool GprmcFlag;

    len=dlen;
    GprmcFlag=FALSE;
    tpos=0;
    while(len!=0)
    {
        if(dbuf[tpos]=='$')
        {
            GetTailOK=FALSE;
            count=0;
            for(i=0; i<len; i++) //from '$' begin test
            {
                count++;
                if(dbuf[tpos+i]==0x0A)  //行结束字符检测
                {
                    GetTailOK=TRUE;
                    break;
                }
            }

            if(GetTailOK==TRUE) //如果存在行结束字符
            {
                if(YS_GpsOneLineVerify(&dbuf[tpos], count)==TRUE)
                {
                    YS_GpsOneLinesDeal(&dbuf[tpos], count); //处理一行数据
                    tpos+=count; //删除当前检索行
                    len-=count;
                }
                else
                {
                    len--;
                    tpos++;
                }
            }
            else
            {
                return; //无结束字符，退出检测
            }
        }
        else
        {
            len--;
            tpos++;
        }
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsSysInitDeal
功能说明：系统初始化时，初始化GPS相关运行数据
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsSysInitDeal(void)
{
    u8 i;
    u8 fbuf[20];

    t_AvlGpsManage.CollectTimes=0;
    t_GpsDataInfo.Effective=FALSE;                //init GpsDataInfo
    t_GpsDataInfo.Speed=0;
    t_GpsDataInfo.Course=0;
    t_GpsDataInfo.SouthORNorth=TRUE;
    t_GpsDataInfo.EastORWest=TRUE;

    YS_PrmReadOneItem(FLH_PRM_LAST_GPSPOINT,FLH_PRM_LAST_GPSPOINT_LEN,fbuf);
    for(i=0; i<4; i++)
    {
        t_GpsDataInfo.Longitude[i]=fbuf[i];
        t_GpsDataInfo.Latitude[i]=fbuf[i+4];
    }

    t_GpsDataInfo.DataTime[0]=0x02;
    t_GpsDataInfo.DataTime[1]=0x01;
    t_GpsDataInfo.DataTime[2]=0x01;
    t_GpsDataInfo.DataTime[3]=0x18;
    t_GpsDataInfo.DataTime[4]=0x01;
    t_GpsDataInfo.DataTime[5]=0x02;
    t_GpsDataInfo.Precision=0xFF;

    t_GpsGprmcInfo.v_Effective='V';		 //init GpsGprmcInfo
    t_GpsGprmcInfo.v_EastORWest='E';
    t_GpsGprmcInfo.v_SouthORNorth='N';
    strcpy(t_GpsGprmcInfo.va_Course,"0.00");
    strcpy(t_GpsGprmcInfo.va_Speed,"0.00");
    strcpy(t_GpsGprmcInfo.va_Time,"180102");
    strcpy(t_GpsGprmcInfo.va_Date,"010102");
    strcpy(t_GpsGprmcInfo.va_Longitude,"00000.0000");
    strcpy(t_GpsGprmcInfo.va_Latitude,"0000.0000");

    t_GpsStarInfo.StarNum=0;				//init gpsstarinfo
    t_GpsStarParse.TestStar=FALSE;		//init gpsparse control
    t_GpsStarParse.TestOK=FALSE;

    t_BDStarInfo.StarNum=0;
    t_BDStarParase.TestStar=FALSE;
    t_BDStarParase.TestOK=FALSE;

    YS_GpsBufInit();
    sjfun_Gps_Start(TRUE);
}


/*-----------------------------------------------------------------------------------------
函数名：YS_GpsBufParaseDeal
功能说明：对接收缓冲区进行解析处理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsBufParaseDeal(void)
{
    u8 fbuf[5];
    YS_PrmReadOneItem(FLH_PRM_GPS_DEBUG_EN,FLH_PRM_GPS_DEBUG_EN_LEN,fbuf);
    if (fbuf[0] == 1)
    {
        t_GpsParaseBuf.DataBuf[t_GpsParaseBuf.DataLen]=0;
        ycsj_debug((char *)(t_GpsParaseBuf.DataBuf));
    }
    t_GpsParaseBuf.AddEnable=FALSE;
    YS_GpsDataSourceInput(t_GpsParaseBuf.DataBuf,t_GpsParaseBuf.DataLen);
    YS_GpsBufInit();	//重新开始接收
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsBufAddData
功能说明：向接收缓冲区中添加数据，并处理执行定时器
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsBufAddData(u8 *dbuf, u16 dlen)
{
    u16 i;

//    ycsj_debug((char *)(dbuf));
    if(t_GpsParaseBuf.AddEnable==FALSE)  //如果正在解析数据
    {
        return;
    }

    if((t_GpsParaseBuf.DataLen+dlen)<=YS_GPS_BUF_LEN) //如果缓冲区可存贮
    {
        for(i=0; i<dlen; i++)
        {
            t_GpsParaseBuf.DataBuf[t_GpsParaseBuf.DataLen]=dbuf[i];
            t_GpsParaseBuf.DataLen++;
        }
        sjfun_stoptimer(GIS_TIMER_ID_5);						//开始解析定时器
        sjfun_timer(GIS_TIMER_ID_5,100,YS_GpsBufParaseDeal);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsGetPosData
功能说明：获取一个GPS 数据结构的拷贝
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsGetPosData(t_Gps_Data_Info *pt_GetGpsData)
{
    u8 *pSour, *pAim;
    u8 len;

    pSour=(u8 *)(&t_GpsDataInfo);
    pAim=(u8 *)(pt_GetGpsData);
    len=sizeof(t_Gps_Data_Info);
    memcpy(pAim,pSour,len);
}

void YS_GpsSetPosStatusNot(void)
{
    if(t_GpsDataInfo.Effective==TRUE)
    {
        t_GpsDataInfo.Effective=FALSE;
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsGetPosData
功能说明：获取一个GPRMC数据结构的拷贝
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsGetGprmcData(t_Gps_Gprmc_Info *pt_GetGpsGprmc)
{
    u8 *pSour, *pAim;
    u8 len;

    pSour=(u8 *)(&t_GpsGprmcInfo);
    pAim=(u8 *)(pt_GetGpsGprmc);
    len=sizeof(t_Gps_Gprmc_Info);
    memcpy(pAim,pSour,len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsGetStarInfo
功能说明：获取一个星历数据结构的拷贝
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsGetStarInfo(t_Gps_Star_Info *pt_GetGpsStar)
{
    u8 *pSour, *pAim;
    u8 len;

    pSour=(u8 *)(&t_GpsStarInfo);
    pAim=(u8 *)(pt_GetGpsStar);
    len=sizeof(t_Gps_Star_Info);
    memcpy(pAim,pSour,len);
}


/*-----------------------------------------------------------------------------------------
函数名：YS_GpsGetBDInfo
功能说明：获取一个北斗星历数据结构的拷贝
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsGetBDInfo(t_Gps_Star_Info *pt_GetGpsStar)
{
    u8 *pSour, *pAim;
    u8 len;

    pSour=(u8 *)(&t_BDStarInfo);
    pAim=(u8 *)(pt_GetGpsStar);
    len=sizeof(t_Gps_Star_Info);
    memcpy(pAim,pSour,len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsRefreshLastPoint
功能说明：更新最新位置点
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsRefreshLastPoint(void)
{
    u8 fbuf[8],i;
    u8 Longitude[4];
    u8 Latitude[4];
    u32 CalcDis;
    static bool PosEffFlag=0;

    if(PosEffFlag==0)  //系统本次复位后有过定位的标志
    {
        if(t_GpsDataInfo.Effective==1)
        {
            PosEffFlag=1;
        }
    }

    YS_PrmReadOneItem(FLH_PRM_LAST_GPSPOINT,FLH_PRM_LAST_GPSPOINT_LEN,fbuf);
    for(i=0; i<4; i++)
    {
        Longitude[i]=fbuf[i];
        Latitude[i]=fbuf[i+4];
    }
    CalcDis=YS_CodeCalcTwoPointDisFine(t_GpsDataInfo.Longitude, t_GpsDataInfo.Latitude,Longitude,Latitude);

    if((CalcDis>=50)&&(PosEffFlag==1))  //距离大于50米,并且有过定位记录
    {
        for(i=0; i<4; i++)
        {
            fbuf[i]=t_GpsDataInfo.Longitude[i];
            fbuf[i+4]=t_GpsDataInfo.Latitude[i];
        }

        for ( i = 0; i<4; i++ )//取备份点
        {
            t_GpsDriftInfo.Dr_Longitude[i] =t_GpsDataInfo.Longitude[i];
            t_GpsDriftInfo.Dr_Latitude[i] = t_GpsDataInfo.Latitude[i];
        }

        YS_PrmWriteOneItem(FLH_PRM_LAST_GPSPOINT,FLH_PRM_LAST_GPSPOINT_LEN,fbuf);

        sjfun_Get_System_DateTime(fbuf);  //保存时间
        YS_PrmWriteOneItem(FLH_PRM_NV_LAST_POINT_DT,FLH_PRM_NV_LAST_POINT_DT_LEN,fbuf);
    }
}


/*-----------------------------------------------------------------------------------------
函数名：YS_GpsInitAreaDeal
功能说明：区域报警初始化处理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsInitAreaDeal(void)
{
    t_fjAreaDeal.wmEnable=TRUE;
    t_fjAreaDeal.FirTestFlag=TRUE;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsAreaWarnCalc
功能说明：区域报警计算入口
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsAreaWarnCalc(t_Gps_Data_Info *pt_GetGps)
{

}

/*-----------------------------------------------------------------------------------------
函数名：YS_GpsInitDistance
功能说明：外部调用里程存贮函数
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_GpsInitDistance(void)
{
    u8 fbuf[4];
    Avl_ULong ldat;

    YS_PrmReadOneItem(FLH_PRM_DIS_VALUE,FLH_PRM_DIS_VALUE_LEN,fbuf);
    ldat.b[AVL_LSTOR_FIR]=fbuf[0];
    ldat.b[AVL_LSTOR_SEC]=fbuf[1];
    ldat.b[AVL_LSTOR_THI]=fbuf[2];
    ldat.b[AVL_LSTOR_FOR]=fbuf[3];
    t_DistanceCalc.DisAddSave=0;
    t_DistanceCalc.SaveTimeCount=0;
    t_DistanceCalc.DistanceValue=ldat.l;
    t_DistanceCalc.FirstCalc=TRUE;
    t_DistanceCalc.BankSpeedCount=0;
}
