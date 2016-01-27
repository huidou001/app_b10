/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_AuxUart.c  2015-06-01
此文件用管理终端与PC 的UART 接口，用于设备的设试及生产维护
-------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------*/

#include "YS_Public.h"

#define  OBD_UARTCMD_ATS        "ATS\r\n"       //查询版本号

t_AuxUart_Parsae_Buf	t_AuxParaseBuf;
t_Obd_Fault_Info        t_ObdFaultInfo;
t_Obd_Main_Info         t_ObdMainInfo;

u8 OBDRstFlag=FALSE;
/*-----------------------------------------------------------------------------------------
函数名：YS_GpsGetPosData
功能说明：获取一个GPS 数据结构的拷贝
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_OBDGetFaultInfo(t_Obd_Fault_Info *pt_GetObdFault)
{
    u8 *pSour, *pAim;
    u8 len;

    pSour=(u8 *)(&t_ObdFaultInfo);
    pAim=(u8 *)(pt_GetObdFault);
    len=sizeof(t_ObdFaultInfo);
    memcpy(pAim,pSour,len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_OBDGetMainInfo
功能说明：获取一个OBD 数据结构的拷贝
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_OBDGetMainInfo(t_Obd_Main_Info *pt_GetObdMain)
{
    u8 *pSour, *pAim;
    u8 len;

    pSour=(u8 *)(&t_ObdMainInfo);
    pAim=(u8 *)(pt_GetObdMain);
    len=sizeof(t_ObdMainInfo);
    memcpy(pAim,pSour,len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_OBDParaseCarRun
功能说明：请求车辆行驶数据 (点火)
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_OBDParaseCarRun(u8 *buf, u16 len)
{
    u16 i,j,pos,loop,count,num,tpos;
    u8 fbuf[20];
    double value;

    pos=0;
    loop=len;
    while(loop>12)
    {
        if((buf[pos]=='B')&&(buf[pos+1]=='D')&&(buf[pos+2]=='$'))
        {
            pos+=3;
            count=0;
            for(i=pos; i<len; i++)	//检测';'的个数
            {
                if(buf[i]==';')
                {
                    count++;
                }
                if(count>=19)
                {
                    loop-=i;
                    pos +=i;
                    return FALSE;
                }
            }
#if 1
            tpos=pos;	//定位到第一个','
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            value = atof((char *)fbuf)*10;
            t_ObdMainInfo.Vmp= value;
//            ycsj_debug("t_ObdMainInfo.Vmp=%d\r\n", t_ObdMainInfo.Vmp);

            pos++;
            tpos=pos;//开始取发动机转速
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Rpm= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Rpm=%d\r\n",t_ObdMainInfo.Rpm);

            pos++;
            tpos=pos;//开始取车速
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Spd= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Spd=%d\r\n", t_ObdMainInfo.Spd);

            pos++;
            tpos=pos;//节气门开度
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            value = atoi((char *)fbuf);
            t_ObdMainInfo.Toh= value;
//            ycsj_debug("t_ObdMainInfo.Toh=%d\r\n", t_ObdMainInfo.Toh);

            pos++;
            tpos=pos;//开始取发动机负荷
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            value = atoi((char *)fbuf);
            t_ObdMainInfo.Load= value;
//            ycsj_debug("t_ObdMainInfo.Load=%d\r\n", t_ObdMainInfo.Load);

            pos++;
            tpos=pos;//开始取冷却液温度
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            value = atoi((char *)fbuf);
            t_ObdMainInfo.Ect = value;
//            ycsj_debug("t_ObdMainInfo.Ect=%d\r\n", t_ObdMainInfo.Ect);

            pos++;
            tpos=pos;//开始取剩余油量
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            value = atof((char *)fbuf)*10;
            t_ObdMainInfo.Fli = value;
//            ycsj_debug("t_ObdMainInfo.Fli=%d\r\n", t_ObdMainInfo.Fli);

            pos++;
            tpos=pos;//百公里油耗
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.XM= atof((char *)fbuf)*10;
//            ycsj_debug("t_ObdMainInfo.Xm=%d\r\n", t_ObdMainInfo.XM);

            pos++;
            tpos=pos;//行驶里程
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Ms= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Ms=%d\r\n", t_ObdMainInfo.Ms);

            pos++;
            tpos=pos;//开始取累计耗油量
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Fs = atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Fs=%d\r\n", t_ObdMainInfo.Fs);

            pos++;
            tpos=pos;//累计点火时间
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Ts= atof((char *)fbuf)*1000;
//            ycsj_debug("t_ObdMainInfo.Ts=%d\r\n", t_ObdMainInfo.Ts);

            pos++;
            tpos=pos;//急加速次数
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Ga= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Ga=%d\r\n", t_ObdMainInfo.Ga);

            pos++;
            tpos=pos;//急刹车次数
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Gb= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.Gb=%d\r\n", t_ObdMainInfo.Gb);

            pos++;
            tpos=pos;//故障码个数
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.FaultNum= atoi((char *)fbuf);
//            ycsj_debug("t_ObdMainInfo.FaultNum=%d\r\n", t_ObdMainInfo.FaultNum);

            pos++;
            tpos=pos;//G X
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Gx= atoi((char *)fbuf);

            pos++;
            tpos=pos;//G Y
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Gy= atoi((char *)fbuf);

            pos++;
            tpos=pos;//G Z
            count=0;
            for(i=tpos; i<len; i++)
            {
                if(buf[pos]==';')
                {
                    break;
                }
                if(count<10)
                {

                    fbuf[i-tpos]=buf[pos];
                    count++;
                }
                pos++;
            }
            if (fbuf[0] > 0x39)
            {
                fbuf[0] = 0x30;
            }
            if (fbuf[1] > 0x39)
            {
                fbuf[1] = 0x30;
            }
            fbuf[count]=0;
            t_ObdMainInfo.Gz= atoi((char *)fbuf);
#else
            num = count;
            t_ObdMainInfo.ObdStrlen = 0;
            for ( j = 0; j<num; j++ )
            {
                tpos=pos;//电压
                count=0;
                for(i=tpos; i<len; i++)
                {
                    if(buf[pos]==';')
                    {
                        break;
                    }
                    if(count<11)
                    {
                        fbuf[i-tpos]=buf[pos];
                        t_ObdMainInfo.ObdStr[t_ObdMainInfo.ObdStrlen] = buf[pos];
                        t_ObdMainInfo.ObdStrlen ++;
                        count++;
                    }
                    pos++;
                }
                fbuf[count]=0;
                t_ObdMainInfo.ObdStr[t_ObdMainInfo.ObdStrlen] = '|';
                t_ObdMainInfo.ObdStrlen ++;
                pos++;
            }

            t_ObdMainInfo.ObdStrlen = t_ObdMainInfo.ObdStrlen - 1;//去掉最后一个'|'
            t_ObdMainInfo.ObdStr[t_ObdMainInfo.ObdStrlen] = 0;
#endif
            return TRUE;
        }
        else
        {
            loop--;
            pos++;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_OBDParaseCarRun
功能说明：请求车辆行驶数据 (熄火)
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_OBDParaseCarStop(u8 *buf, u16 len)
{
    u16 i,j,pos,loop,count,num,tpos,bpos;
    u8 fbuf[20];
    u16 value;
    bool ParBegin=FALSE;

    pos=0;
    loop=len;
    while(loop>12)
    {
        if((buf[pos]=='F')&&(buf[pos+1]=='F')&&(buf[pos+2]=='$'))
        {
            pos+=3;
            count=0;
            for(i=pos; i<len; i++)	//检测';'的个数
            {
                if(buf[i]==';')
                {
                    count++;
                }
                if(count>=15)
                {
                    loop-=i;
                    pos +=i;
                    return FALSE;
                }
            }

            num = count;
            for ( j = 0; j<num; j++ )
            {
                tpos=pos;	//定位到第一个','
                count=0;
                ParBegin = FALSE;
                for(i=tpos; i<len; i++)
                {
                    if(buf[pos]==';')
                    {
                        break;
                    }
                    if (ParBegin == TRUE)
                    {
                        if(count<10)
                        {
                            fbuf[count]=buf[pos];
                            count++;
                        }
                    }
                    if (buf[pos] == ':')
                    {
                        bpos = i;
                        ParBegin = TRUE;
                    }
                    pos++;
                }
                pos++;
                fbuf[count]=0;
                value = atoi((char *)fbuf);
                ycsj_debug("%s,j=%d, value = %d\r\n",fbuf,j,value);
//                switch (j)
//                {
//                    case 0 :
//                        break;
//                    case 1 :

//                        break;
//                    case 2 :

//                        break;
//                    case 3 :

//                        break;
//                    case 4 :

//                        break;
//                    case 5 :

//                        break;
//                    case 6 :

//                        break;
//                    case 7 :

//                        break;
//                    case 8 :

//                        break;
//                    case 9 :

//                        break;
//                    case 10 :

//                        break;
//                    case 11 :

//                        break;
//                    case 12 :

//                        break;
//                    case 13 :

//                        break;
//                    default:
//                        break;
//                }

            }
            return(TRUE);
        }
        else
        {
            loop--;
            pos++;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_OBDOneLinesDeal
功能说明：
修改记录：

-------------------------------------------------------------------------------------------*/
void YS_OBDOneLinesDeal(u8 *dbuf, u16 dlen)
{
    if (YS_OBDParaseCarRun(dbuf, dlen) == TRUE)
    {
        ycsj_debug("car run Parase ok!");
    }
    else
    {
        ycsj_debug("car Parase fail!");
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_ObdGetCarData
功能说明：获取一个Car数据结构的拷贝
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_ObdGetCarData(t_Obd_Main_Info *pt_ObdMainInfo)
{
    u8 *pSour, *pAim;
    u8 len;

    pSour=(u8 *)(&t_ObdMainInfo);
    pAim=(u8 *)(pt_ObdMainInfo);
    len=sizeof(t_ObdMainInfo);
    memcpy(pAim,pSour,len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_ObdCarPackage
功能说明：获取一个Car数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16  YS_ObdCarPackage(u8 *PackBuf)
{
    u8 i;
    for ( i = 0; i < t_ObdMainInfo.ObdStrlen; i++ )
    {
        PackBuf[i] = t_ObdMainInfo.ObdStr[i];
    }

    return t_ObdMainInfo.ObdStrlen;
}

/*-----------------------------------------------------------------------------------------
函数名：
功能说明：
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_OBDDataSourceInput(u8 *dbuf, u16 dlen)
{

    u16 i,len,tpos,count;
    bool GetTailOK;
    t_Sys_Run_Status t_GetStatus;

    YS_RunGetSystemStatus(&t_GetStatus);
    len=dlen;
    tpos=0;
    while(len!=0)
    {
        if((dbuf[tpos]=='B')&&(dbuf[tpos+1]=='D')&&(dbuf[tpos+2]=='$'))
        {
            if (t_GetStatus.AccStatus == 0)
            {
              YS_RunAccStatusBrush(1);
            }
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
//                YS_OBDOneLinesDeal(&dbuf[tpos], count); //处理一行数据
                YS_OBDParaseCarRun(&dbuf[tpos], count);//处理车辆运行数据
                tpos+=count; //删除当前检索行
                len-=count;
            }
            else
            {
                return; //无结束字符，退出检测
            }
        }
        // AT POWERON  &&  CONNECTED 1ECU  汽车点火
        else if (((dbuf[tpos]=='E')&&(dbuf[tpos+1]=='R')&&(dbuf[tpos+2]=='O')&&(dbuf[tpos+3]=='N'))||((dbuf[tpos] > '0')&&(dbuf[tpos+1]=='E')&&(dbuf[tpos+2]=='C')&&(dbuf[tpos+3]=='U')))
        {
            YS_RunAccStatusBrush(1);
            len -= 4;
            tpos += 4;
        }
        //ATPOWEROFF$MAXRPM.... 熄火数据统计
        else if (((dbuf[tpos]=='O')&&(dbuf[tpos+1]=='F')&&(dbuf[tpos+2]=='F')&&(dbuf[tpos+3]=='$'))||((dbuf[tpos]=='0')&&(dbuf[tpos+1]=='E')&&(dbuf[tpos+2]=='C')&&(dbuf[tpos+3]=='U')))
        {
            YS_RunAccStatusBrush(0);
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
                YS_OBDParaseCarStop(&dbuf[tpos], count);//处理车辆运行数据
                tpos+=count; //删除当前检索行
                len-=count;
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
函数名：YS_AuxUartSendCmd
功能说明：串口发送指令
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_AuxUartSendCmd(void)
{

}

void YS_OBDRstInit(bool flag)
{
    OBDRstFlag = flag;
}

void YS_OBDRstDeal(void)
{
    static u8 num = 0;
    if (OBDRstFlag == TRUE)
    {
        sjfun_Gpio_Write_Value(YS_PIN_NO_OBD_RST,0);
        num ++;
        if (num >=2)
        {
            OBDRstFlag = FALSE;
            sjfun_Gpio_Write_Value(YS_PIN_NO_OBD_RST,1);
        }
    }
}
/*-----------------------------------------------------------------------------------------
函数名：YS_AuxUartDealInit
功能说明：初始化数据接收
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_AuxUartBufInit(void)
{
    t_AuxParaseBuf.AddEnable=1;
    t_AuxParaseBuf.DataLen=0;
    YS_OBDRstInit(TRUE);
    sjfun_Open_AuxUart();
}

/*-----------------------------------------------------------------------------------------
函数名：YS_AuxBufParaseDeal
功能说明：对接收缓冲区进行解析处理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_AuxBufParaseDeal(void)
{
    u8 fbuf[5];
    YS_PrmReadOneItem(FLH_PRM_OBD_DEBUG_EN,FLH_PRM_OBD_DEBUG_EN_LEN,fbuf);
    if (fbuf[0] == 1)
    {
        t_AuxParaseBuf.DataBuf[t_AuxParaseBuf.DataLen] = 0;
        ycsj_debug((char *)t_AuxParaseBuf.DataBuf);
    }
    YS_OBDDataSourceInput(t_AuxParaseBuf.DataBuf, t_AuxParaseBuf.DataLen);
    t_AuxParaseBuf.AddEnable=1;
    t_AuxParaseBuf.DataLen=0;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_AuxBufAddData
功能说明：向接收缓冲区中添加数据，并处理执行定时器
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_AuxBufAddData(u8 *dbuf, u16 dlen)
{
    u16 i;
    if((t_AuxParaseBuf.DataLen+dlen)<=YS_GPS_BUF_LEN) //如果缓冲区可存贮
    {
        for(i=0; i<dlen; i++)
        {
            t_AuxParaseBuf.DataBuf[t_AuxParaseBuf.DataLen]=dbuf[i];
            t_AuxParaseBuf.DataLen++;
        }
        sjfun_stoptimer(GIS_TIMER_ID_2);						//开始解析定时器
        sjfun_timer(GIS_TIMER_ID_2,100,YS_AuxBufParaseDeal);
    }
}

