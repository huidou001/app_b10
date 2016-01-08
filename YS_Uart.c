/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_Uart.c  2013-06-26
此文件用管理终端与PC 的UART 接口，用于设备的设试及生产维护
-------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------*/

#include "YS_Public.h"

#define PRM_OK_BUF_LEN		500
#define PRM_FAIL_BUF_LEN	100

#define UART_CMD_HEAD_IPPORT	    "#IP#"
#define UART_CMD_HEAD_IPPORT_CHK    "#IP#?#"
#define UART_CMD_HEAD_GPSDATA	    "#GPSDATA#"
#define UART_CMD_HEAD_GPS	        "#GPS#"
#define UART_CMD_HEAD_GPSCHK	    "#GPS#?#"
#define UART_CMD_HEAD_APN	        "#APN#"
#define UART_CMD_HEAD_APNCHK	    "#APN#?#"
#define UART_CMD_HEAD_LOGIN	        "#LOGIN#?#"
#define UART_CMD_HEAD_SMS	        "#SMS#"
#define UART_CMD_HEAD_RST           "#RST#"
#define UART_CMD_HEAD_IMEICHK       "#IMEI#?#"
#define UART_CMD_HEAD_VERCHK        "#VER#?#"
#define UART_CMD_HEAD_OBD           "#OBD#"
#define UART_CMD_HEAD_ID            "#ID#"

enum
{
    UART_ACK_IP_ERR,
    UART_ACK_IP_OK,
    UART_ACK_IP_CHK,
    UART_ACK_OBD_OK,
    UART_ACK_GPSDATA_OK,
    UART_ACK_GPS_OK,
    UART_ACK_GPS_CHK,
    UART_ACK_APN_OK,
    UART_ACK_APN_CHK,
    UART_ACK_LOGIN_CHK,
    UART_ACK_SMS_OK,
    UART_ACK_RST_OK,
    UART_ACK_IMEICHK,
    UART_ACK_VERCHK,
    UART_ACK_ID_OK,
};

t_avltra_Parase 	t_AvlComParase;
t_avltra_Parase     t_TxzComParase;

u8	UartPrmBuf[600], UartSendBuf[2048];

extern void sjfun_Debug_Uart_send(U8 *dbuf, U16 dlen);

static bool YS_UartInputCmdControl(u8 *dbuf, u8 dlen);
/*------------------------------------------------------------------------------------------
函数名：YS_UartPackYSProtocl
功能说明：封装数据成优胜串口协议
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_UartPackProtocl(u16 CmdID, u8* dbuf, u16 dlen, u8 *PackBuf)
{
    u16 i,pos;
    u8 verify;
    static u16 SendFlow=0;

    PackBuf[0]='Y'; 			//package head
    PackBuf[1]='S';

    PackBuf[2]=0;			//res data
    PackBuf[3]=0;

    for(i=0; i<5; i++)			//product ID
        PackBuf[i+4]=0;

    PackBuf[9]=SendFlow/256;	 //Flow ID
    PackBuf[10]=SendFlow%256;

    SendFlow++;

    PackBuf[11]=CmdID/256;	//CmdID
    PackBuf[12]=CmdID%256;

    PackBuf[13]=dlen/256;	//data len
    PackBuf[14]=dlen%256;

    pos=15;

    if(dlen>0)				//fill data
    {
        for(i=0; i<dlen; i++)
        {
            PackBuf[pos]=dbuf[i];
            pos++;
        }
    }

    verify=0;					//verify
    for(i=0; i<pos; i++)
    {
        verify+=PackBuf[i];
    }
    PackBuf[pos]=verify;
    pos++;

    PackBuf[pos]=0x0D;		//Package tail
    pos++;
    PackBuf[pos]=0x0A;
    pos++;

    return(pos);				//return package length
}


/*------------------------------------------------------------------------------------------
函数名：YS_UartParaseComProtocl
功能说明：解析串口接收的数据
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_UartParaseComProtocl(u8 *dbuf, u16 dlen)
{
    u16 i,pos,getlen;
    u8  verify;

    getlen=0;
    pos=0;

    while(dlen>=18)  //min len of protocol
    {
        if((dbuf[pos]=='Y')&&(dbuf[pos+1]=='S')&&(dbuf[pos+2]==0x00)&&(dbuf[pos+3]==0x00))
        {
            t_AvlComParase.v_FlowID=dbuf[pos+9]*256+dbuf[pos+10]; //save flowid pos+9

            t_AvlComParase.v_CmdID=dbuf[pos+11]*256+dbuf[pos+12];//save cmdid pos+11

            t_AvlComParase.v_dlen=dbuf[pos+13]*256+dbuf[pos+14];
            if((t_AvlComParase.v_dlen>600)||((t_AvlComParase.v_dlen+18)>dlen)) //test dlen if rigth
            {
                dlen--;
                pos++;
                getlen++;
                continue;
            }

            for(i=0; i<t_AvlComParase.v_dlen; i++)//save databuf
            {
                t_AvlComParase.a_dbuf[i]=dbuf[pos+15+i];
            }

            verify=0;  //test verify if right
            for(i=0; i<(t_AvlComParase.v_dlen+15); i++)
            {
                verify+=dbuf[pos+i];
            }

            if(verify!=dbuf[pos+t_AvlComParase.v_dlen+15])
            {
                dlen--;
                pos++;
                getlen++;
                continue;
            }

            if((dbuf[pos+t_AvlComParase.v_dlen+16]!=0x0D)||(dbuf[pos+t_AvlComParase.v_dlen+17]!=0x0A))
            {
                dlen--;
                pos++;
                getlen++;
                continue;
            }

            getlen=getlen+(t_AvlComParase.v_dlen+18);
            t_AvlComParase.v_GetLen=getlen; //set have deal len
            return(TRUE);
        }
        else
        {
            dlen--;
            pos++;
            getlen++;
        }
    }
    return(FALSE);
}

/*------------------------------------------------------------------------------------------
函数名：YS_UartMoniStatusPack
功能说明：封装系统运行状态数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_UartMoniStatusPack(u8 *PackBuf)
{
    t_Sys_Run_Status	t_GetStatus;
    u16 len;
    u8 *p;

    YS_RunGetSystemStatus(&t_GetStatus);
    p=(u8 *)(&t_GetStatus);
    len=sizeof(t_Sys_Run_Status);
    memcpy(PackBuf,p,len);
    return(len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsGetItemInBuf
功能说明：在缓冲区中查找指定的数据成员，可设置分隔符，数据缓冲区的最大值
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_SmsGetItemInBuf(u8 *dbuf, u16 dlen, u8 *PackBuf,u8 ID,u8 KeyChar, u16 MaxLen)
{
    u16 i,pos,count;

    pos=0;
    count=0;
    if(ID==0)
    {
        pos++;
    }
    else
    {
        for(i=0; i<dlen; i++)
        {
            if(dbuf[i]==KeyChar)
            {
                count++;
                if(count>=ID+1)
                {
                    pos=i+1;
                    break;
                }
            }
        }
    }

    count=0;
    for(i=pos; i<dlen; i++)
    {
        if(dbuf[i]==KeyChar)
        {
            break;
        }

        if(count<=MaxLen)
        {
            PackBuf[i-pos]=dbuf[i];
            count++;
        }
    }

    return(count);
}

/*------------------------------------------------------------------------------------------
函数名：YS_UartSetPrmDeal
功能说明：参数设置处理
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_UartSetPrmDeal(u8 *dbuf, u16 dlen, u8 *PackBuf)
{

    u16 i,k,loop,pos,len,OKPos,FailPos;
    u8	SelBuf[60];
    u8	SetOKBuf[PRM_OK_BUF_LEN];
    u8	SetFailBuf[PRM_FAIL_BUF_LEN];


    loop=0;
    pos=0;
    OKPos=0;
    FailPos=0;
    for(i=0; i<dlen; i++)		//获取输入参数的个数
    {
        if(dbuf[i]=='#')
        {
            loop++;
        }
    }

    if(loop>0)
    {
        for(k=0; k<loop; k++)	//遂一处理每个参数的设置工作
        {
            len=YS_SmsGetItemInBuf(dbuf,dlen,SelBuf,k,'#',60);
            if(YS_WorkPrmSetDeal(SelBuf,len))	//处理参数设置工作
            {
                if((OKPos+len+1)<PRM_OK_BUF_LEN)
                {
                    for(i=0; i<len; i++)		//添加成功项
                    {
                        SetOKBuf[OKPos+i]=SelBuf[i];
                    }
                    OKPos+=len;
                    SetOKBuf[OKPos]=',';
                    OKPos++;
                }
            }
            else
            {
                if((FailPos+len+1)<PRM_FAIL_BUF_LEN)
                {
                    for(i=0; i<len; i++)		//添加失败项
                    {
                        SetFailBuf[FailPos+i]=SelBuf[i];
                    }
                    FailPos+=len;
                    SetFailBuf[FailPos]=',';
                    FailPos++;
                }
            }
        }


        if(OKPos>0)
        {
            OKPos--;
        }
        PackBuf[pos]=OKPos/256;				//返回结果添加设置成功数据
        pos++;
        PackBuf[pos]=OKPos%256;
        pos++;
        if(OKPos>0)
        {
            for(i=0; i<OKPos; i++)
            {
                PackBuf[pos]=SetOKBuf[i];
                pos++;
            }
        }


        if(FailPos>0)
        {
            FailPos--;
        }
        PackBuf[pos]=FailPos/256;				//返回结果添加设置成败数据
        pos++;
        PackBuf[pos]=FailPos%256;
        pos++;
        if(FailPos>0)
        {
            FailPos--;
            for(i=0; i<FailPos; i++)
            {
                PackBuf[pos]=SetFailBuf[i];
                pos++;
            }
        }
    }
    return(pos);
}

/*------------------------------------------------------------------------------------------
函数名：YS_UartReadPrmDeal
功能说明：参数读取处理
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_UartReadPrmDeal(u8 *dbuf, u16 dlen, u8 *PackBuf)
{
    u8	SelBuf[50];
    u16 i,k,len,loop,pos,FailPos,AddLen,OkPos;
    u8 SetOKBuf[PRM_OK_BUF_LEN];
    u8 SetFailBuf[PRM_FAIL_BUF_LEN];

    loop=0;
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]=='#')
        {
            loop++;
        }
    }

    if(loop<1) //未输入有效设置项目
    {
        return(0);
    }

    FailPos=0;
    OkPos=0;
    for(k=0; k<loop; k++)
    {
        len=YS_SmsGetItemInBuf(dbuf,dlen,SelBuf,k,'#',50);
        if(YS_WorkPrmReadDeal(SelBuf,len,&SetOKBuf[OkPos],&AddLen))
        {
            if((OkPos+AddLen+1)<=PRM_OK_BUF_LEN)
            {
                OkPos+=AddLen;
                SetOKBuf[OkPos]=',';
                OkPos++;
            }
        }
        else
        {
            if((FailPos+len+1)<=PRM_FAIL_BUF_LEN)
            {
                for(i=0; i<len; i++)
                {
                    SetFailBuf[FailPos+i]=SelBuf[i];
                }
                FailPos+=len;
                SetFailBuf[FailPos]=',';
                FailPos++;
            }
        }
    }

    pos=0;
    if(OkPos>0)
    {
        OkPos--;
    }
    PackBuf[pos]=OkPos/256;				//添加读取成功结果
    pos++;
    PackBuf[pos]=OkPos%256;
    pos++;
    if(OkPos>0)
    {
        for(i=0; i<OkPos; i++)
        {
            PackBuf[pos]=SetOKBuf[i];
            pos++;
        }
    }

    if(FailPos>0)
    {
        FailPos--;
    }
    PackBuf[pos]=FailPos/256;				//添加读取失败结果
    pos++;
    PackBuf[pos]=FailPos%256;
    pos++;
    if(FailPos>0)
    {
        for(i=0; i<FailPos; i++)
        {
            PackBuf[pos]=SetFailBuf[i];
            pos++;
        }
    }
    return(pos);
}



/*------------------------------------------------------------------------------------------
函数名：YS_UartMoniGpsPack
功能说明：封装GPS 监控数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_UartMoniGpsPack(u8 *buf)
{
    u16 i,GpsLen,pos,CollectTimes;
    u8  StarNum;
    u8 *p;
    t_Gps_Star_Info 	t_GetStar;
    t_Gps_Data_Info	t_GetGps;
    YS_GpsGetStarInfo(&t_GetStar);
    YS_GpsGetPosData(&t_GetGps);
    CollectTimes=YS_GpsGetCollectTimes();
    GpsLen=sizeof(t_Gps_Data_Info);
    StarNum=t_GetStar.StarNum;
    p=(u8 *)(&t_GetGps);
    pos=0; //set gps data
    for(i=0; i<GpsLen; i++)
    {
        buf[pos]=p[i];
        pos++;
    }
    buf[pos]=StarNum; //set star data
    pos++;
    for(i=0; i<StarNum; i++)
    {
        buf[pos]=t_GetStar.StarCode[i];
        pos++;
        buf[pos]=t_GetStar.StarSign[i];
        pos++;
    }
    buf[pos]=CollectTimes/256; //添加GPS 数据采样次数
    pos++;
    buf[pos]=CollectTimes%256;
    pos++;
    return(pos);
}

/*------------------------------------------------------------------------------------------
函数名：YS_UartMoniBDGpsInfo
功能说明：封装北斗GPS 监控数据
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_UartMoniBDGpsInfo(u8 *buf)
{
    u16 i,GpsLen,pos,CollectTimes;
    u8  StarNum;
    u8 *p;
    t_Gps_Star_Info 	t_GetStar;
    t_Gps_Star_Info	t_GetBD;
    t_Gps_Data_Info	t_GetGps;

    YS_GpsGetStarInfo(&t_GetStar);
    YS_GpsGetPosData(&t_GetGps);
    YS_GpsGetBDInfo(&t_GetBD);

    CollectTimes=YS_GpsGetCollectTimes();
    GpsLen=sizeof(t_Gps_Data_Info);
    StarNum=t_GetStar.StarNum;
    p=(u8 *)(&t_GetGps);
    pos=0; //set gps data
    for(i=0; i<GpsLen; i++)
    {
        buf[pos]=p[i];
        pos++;
    }
    buf[pos]=StarNum; //set star data
    pos++;
    for(i=0; i<StarNum; i++)
    {
        buf[pos]=t_GetStar.StarCode[i];
        pos++;
        buf[pos]=t_GetStar.StarSign[i];
        pos++;
    }
    buf[pos]=CollectTimes/256; //添加GPS 数据采样次数
    pos++;
    buf[pos]=CollectTimes%256;
    pos++;

    buf[pos]=t_GetBD.StarNum;//添加北斗星历数据
    pos++;
    for(i=0; i<t_GetBD.StarNum; i++)
    {
        buf[pos]=t_GetBD.StarCode[i];
        pos++;
        buf[pos]=t_GetBD.StarSign[i];
        pos++;
    }
    return(pos);
}

/*------------------------------------------------------------------------------------------
函数名：YS_UartInputCmdDeal
功能说明：串口数据解析后对应指令的处理。
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_UartInputCmdDeal(u16 CmdID, u8 *dbuf, u16 dlen)
{
    u16 PackLen,SendLen;
    u16 AckCmd;
    bool SendBack;

    SendBack=TRUE;
    PackLen=0;
    switch(CmdID)
    {
        case INTER_CMD_GET_RUNINFO: //获取系统运行状态
            PackLen=YS_UartMoniStatusPack(UartPrmBuf);
            AckCmd=INTER_ACK_GET_RUNINFO;
            break;

        case INTER_CMD_GET_GPSSTATUS://获取GPS 运行状态
            PackLen=YS_UartMoniGpsPack(UartPrmBuf);
            AckCmd=INTER_ACK_GET_GPSSTATUS;
            break;

        case INTER_CMD_GET_BDGPSINFO: //获取北斗定位数据
            PackLen=YS_UartMoniBDGpsInfo(UartPrmBuf);
            AckCmd=INTER_ACK_GET_BDGPSINFO;
            break;

        case INTER_CMD_SET_PARAM://请求设置配置参数
            PackLen=YS_UartSetPrmDeal(dbuf,dlen,UartPrmBuf);
            AckCmd=INTER_ACK_SET_PARAM;
            break;

        case INTER_CMD_DIS_CALC: //测试两距离计算公式
            PackLen=YS_CodeCalcTowPointTest(dbuf,dlen,UartPrmBuf);
            AckCmd=INTER_ACK_DIS_CALC;
            break;

        case INTER_CMD_READ_PARAM://请求读取配置参数
            PackLen=YS_UartReadPrmDeal(dbuf,dlen,UartPrmBuf);
            AckCmd=INTER_ACK_READ_PARAM;
            break;

        case INTER_CMD_SET_IP: //设置地址
            if(dlen<=FLH_PRM_SERV_ADDR_LEN)
            {
                dbuf[dlen]=0;
                YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,dbuf);
                YS_PrmQuickWriteNV();
                AckCmd=INTER_CMD_FLAG_AFFIR;
            }
            else
            {
                AckCmd=INTER_CMD_FLAG_FAIL;
            }
            break;

        case INTER_CMD_READ_IP: //读取地址
            YS_PrmReadOneItem(FLH_PRM_SERV_ADDR, FLH_PRM_SERV_ADDR_LEN, UartPrmBuf);
            PackLen=YS_CodeBufRealLen(UartPrmBuf,FLH_PRM_SERV_ADDR_LEN);
            AckCmd=INTER_ACK_READ_IP;
            break;

        case INTER_CMD_SET_PORT: //设置端口
            if(dlen==2)
            {
                YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,dbuf);
                YS_PrmQuickWriteNV();
                AckCmd=INTER_CMD_FLAG_AFFIR;
            }
            else
            {
                AckCmd=INTER_CMD_FLAG_FAIL;
            }
            break;

        case INTER_CMD_READ_PORT: //读取端口
            YS_PrmReadOneItem(FLH_PRM_SERV_PORT, FLH_PRM_SERV_PORT_LEN, UartPrmBuf);
            PackLen=2;
            AckCmd=INTER_ACK_READ_PORT;
            break;

        case INTER_CMD_SET_APN: //设置APN
            if(dlen<=FLH_PRM_PPP_APN_LEN)
            {
                dbuf[dlen]=0;
                YS_PrmWriteOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,dbuf);
                AckCmd=INTER_CMD_FLAG_AFFIR;
            }
            else
            {
                AckCmd=INTER_CMD_FLAG_FAIL;
            }
            break;

        case INTER_CMD_READ_APN: //读取APN
            YS_PrmReadOneItem(FLH_PRM_PPP_APN, FLH_PRM_PPP_APN_LEN, UartPrmBuf);
            PackLen=YS_CodeBufRealLen(UartPrmBuf,FLH_PRM_PPP_APN_LEN);
            AckCmd=INTER_ACK_READ_APN;
            break;

        case INTER_CMD_SET_DEBUG_EN: //设置调试使能
            if(dlen==FLH_PRM_DEBUG_ENABLE_LEN)
            {
                YS_PrmWriteOneItem(FLH_PRM_DEBUG_ENABLE,FLH_PRM_DEBUG_ENABLE_LEN,dbuf);
                AckCmd=INTER_CMD_FLAG_AFFIR;
            }
            else
            {
                AckCmd=INTER_CMD_FLAG_FAIL;
            }
            break;

        case INTER_CMD_READ_DEBUG_EN: //查询调试使能
            YS_PrmReadOneItem(FLH_PRM_DEBUG_ENABLE, FLH_PRM_DEBUG_ENABLE_LEN, UartPrmBuf);
            PackLen=FLH_PRM_DEBUG_ENABLE_LEN;
            AckCmd=INTER_ACK_READ_DEBUG_EN;
            break;

        case INTER_CMD_SMS_SEND_TEST:
            YS_SmsSendTestInterface(dbuf,dlen);
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_RST_TERI:  //软件复位终羰
            AckCmd=INTER_CMD_FLAG_AFFIR;
            YS_SysRsqSystemRST(YS_RST_FLAG_USER);
            break;

        case INTER_CMD_SHUTDOWN_SYSTEM: //系统关机
            sjfun_Shut_Down_send();
            AckCmd=INTER_ACK_SHUTDOWN_SYSTEM;
            break;

        case INTER_CMD_READ_RSTINFO: //读取复位信息
            YS_PrmReadOneItem(FLH_PRM_RST_RECORD, FLH_PRM_RST_RECORD_LEN, UartPrmBuf);
            PackLen=FLH_PRM_RST_RECORD_LEN;
            AckCmd=INTER_ACK_READ_RSTINFO;
            break;

        case INTER_CMD_SMS_SIMULATION://短信输入模拟
            YS_SmsComInputSimuLation(dbuf,dlen);
            PackLen=0;
            AckCmd=INTER_ACK_SMS_SIMULATION;
            break;

        case INTER_CMD_GPS_ON_TEST: //GPS 电源开测试
            sjfun_Gps_Start(TRUE);
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_GPS_OFF_TEST: //GPS电源关测试
            sjfun_Gps_Stop(TRUE);
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_LED_STATUS_TEST: //LED 指示灯控制
            if(dbuf[0]==0)
            {
                sjfun_Set_GsmLed(dbuf[1]);
            }
            else
            {
                sjfun_Set_GsmLed(dbuf[1]);
            }
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_SET_GSENSOR_REG:
            YS_GSensorSetRegExtern(dbuf[0],dbuf[1]);
            PackLen=0;
            AckCmd=INTER_ACK_SET_GSENSOR_REG;
            break;

        case INTER_CMD_GET_GSENSOR_REG:
            UartPrmBuf[0]=YS_GSensorReadRegExtern(dbuf[0]);
            PackLen=1;
            AckCmd=INTER_ACK_GET_GSENSOR_REG;
            break;

        case INTER_CMD_ENABLE_SLEEP: //允许MTK 休眠
            sjfun_Sys_Sleep_Enable();
            PackLen=0;
            AckCmd=INTER_ACK_ENABLE_SLEEP;
            break;

        case INTER_CMD_FORBIT_SLEEP: //禁止MTK 休眠
            sjfun_Sys_Sleep_Disable();
            PackLen=0;
            AckCmd=INTER_ACK_FORBIT_SLEEP;
            break;

        case INTER_CMD_CALL_OUT_TEST: //电话呼出测试
            dbuf[dlen]=0;
            PackLen=0;
            AckCmd=INTER_ACK_CALL_OUT_TEST;
            break;

        case INTER_CMD_VIB_POW_TEST: //gsensor电源控制测试
            if(dbuf[0]==0)
            {
                sjfun_Gpio_Write_Value(YS_PIN_NO_GPOW,0);	//电源开启
            }
            else
            {
                sjfun_Gpio_Write_Value(YS_PIN_NO_GPOW,1);	//电源关闭
            }
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_GPS_CTRL: //GPS  芯片数据指令发送处理
            sjfun_Gps_WriteAgps(dbuf,dlen);
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_AGPS_START:
            YS_AGpsDealInterFace();
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;
        case INTER_CMD_CALIBRATION_GET:
            YS_SysGetBarCodeInterface();
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_FAC_CONNECT: //工厂测试探测指令
            PackLen=0;
            AckCmd=INTER_ACK_FAC_CONNECT;
            break;

        case INTER_CMD_GENERALIO_INIT:
            YS_IOGeneralInit(dbuf);
            AckCmd=INTER_ACK_GENERALIO_INIT;
            break;

        case INTER_CMD_GENERALIO_WRITE:
            YS_IOGeneralWrite(dbuf);
            PackLen=0;
            AckCmd=INTER_ACK_GENERALIO_WRITE;
            break;

        case INTER_CMD_GENERALIO_READ:
            UartPrmBuf[0]=YS_IOGeneralRead(dbuf);
            PackLen=1;
            AckCmd=INTER_ACK_GENERALIO_READ;
            break;

        case INTER_CMD_SET_FLI_EN:
            if((dbuf[0]==0)||(dbuf[0]==1))
            {
                YS_PrmWriteOneItem(FLH_PRM_FLI_ENABLE,FLH_PRM_FLI_ENABLE_LEN,dbuf);
                AckCmd=INTER_CMD_FLAG_AFFIR;
            }
            else
            {
                AckCmd=INTER_CMD_FLAG_FAIL;
            }
            break;

        case INTER_CMD_READ_FLI_EN:
            YS_PrmReadOneItem(FLH_PRM_FLI_ENABLE, FLH_PRM_FLI_ENABLE_LEN, UartPrmBuf);
            PackLen=FLH_PRM_FLI_ENABLE_LEN;
            AckCmd=INTER_ACK_READ_FLI_EN;
            break;

        case INTER_CMD_SET_ENTRY_FACTORY:
            if(YS_RunGetSysInfoInit()==TRUE)
            {
                YS_RunSetFactoryMode(TRUE);
                PackLen=0;
                AckCmd=INTER_ACK_ENTRY_FACTORY;
            }
            else
            {
                AckCmd=INTER_CMD_FLAG_FAIL;
            }

            break;

        case INTER_CMD_GET_NETINFO: //获取网络信息
            PackLen=YS_SysGetNetInfo(UartPrmBuf);
            AckCmd=INTER_ACK_GET_NETINFO;
            break;

        case INTER_CMD_SET_EXIT_FACTORY:
            YS_RunSetFactoryMode(FALSE);
            PackLen=0;
            AckCmd=INTER_ACK_EXIT_FACTORY;
            break;

        case INTER_CMD_CALIBAR_DATA_GET:
            YS_SysGetCaliBartionDataInterface();
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_ALM_TIME_SET: //设置系统时间
            sjfun_Set_System_DateTime(dbuf);
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_ALM_TIME_READ://查询系统时间
            sjfun_Get_System_DateTime(UartPrmBuf);
            PackLen=6;
            AckCmd=INTER_ACK_ALM_TIME_READ;
            break;

        case INTER_CMD_ALM_WARN_SET: //设置闹铃
            sjfun_Set_Alm_Warn(dbuf, dlen);
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_ALM_WARN_READ: //查询闹铃
            PackLen=sjfun_Read_Alm_Warn(dbuf,dlen,UartPrmBuf);
            AckCmd=INTER_ACK_ALM_WARN_READ;
            break;

        case INTER_CMD_START_RECORD: //开始录音
            UartPrmBuf[0]=sjfun_Start_Record(dbuf[0]);
            PackLen=1;
            AckCmd=INTER_ACK_START_RECORD;
            break;

        case INTER_CMD_STOP_RECORD: //停止录音
            sjfun_Stop_Record();
            AckCmd=INTER_CMD_FLAG_AFFIR;
            break;

        case INTER_CMD_START_AUDIOPLAY://开始播放音频
            UartPrmBuf[0]=sjfun_Start_Audio_Play();
            PackLen=1;
            AckCmd=INTER_ACK_START_AUDIOPLAY;
            break;

        case INTER_CMD_OPEN_AUXUART://打开辅助串口
            sjfun_Open_AuxUart();
            AckCmd=INTER_ACK_OPEN_AUXUART;
            break;

        case INTER_CMD_CLOSE_AUXUART://关闭辅助串口
            sjfun_Close_AuxUart();
            AckCmd=INTER_ACK_CLOSE_AUXUART;
            break;

        case INTER_CMD_SEND_AUXUART:
            sjfun_Aux_Com_send(dbuf,dlen);
            AckCmd=INTER_ACK_SEND_AUXUART;
            break;

        case INTER_CMD_GET_NSCK://获取其它通道SOCKET 运行状态
        default:	//无法识别的指令
            AckCmd=INTER_CMD_FLAG_UNKNOW;
            break;
    }

    if((AckCmd==INTER_CMD_FLAG_AFFIR)||(AckCmd==INTER_CMD_FLAG_FAIL)||(AckCmd==INTER_CMD_FLAG_UNKNOW)) //如果返回确认信息
    {
        UartPrmBuf[0]=CmdID/256;
        UartPrmBuf[1]=CmdID%256;
        PackLen=2;
    }

    if(SendBack==TRUE) //如果参数区有数据则发送数据包到串口
    {
        SendLen=YS_UartPackProtocl(AckCmd,UartPrmBuf,PackLen,UartSendBuf);
        sjfun_Debug_Uart_send(UartSendBuf,SendLen);
//		sjfun_Gps_Com_send(UartSendBuf,SendLen);
    }
}

/*------------------------------------------------------------------------------------------
函数名：YS_UartInputDataDeal
功能说明：串口数据输入处理
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_DebugInputInfoDeal(char *DataPoint)
{
    u16 i,len,pos,loop;
    u8 Data[512];
    static u8 n=0;


    len=strlen(DataPoint);
    if(len<7) //18*2+5
    {
        return;
    }

    if((DataPoint[0]=='A')&&(DataPoint[1]=='T')&&(DataPoint[2]=='Y')&&(DataPoint[3]=='S')&&(DataPoint[len-1]==0x0D)) //比较外围封装
    {

        loop=(len-5)/2;
        if(len>512*2) //数据长度异常
        {
            return;
        }
        pos=0; //转换为二进制缓冲区数据
        for(i=0; i<loop; i++) //
        {
            Data[pos]=YS_CodeHexCharConver(DataPoint[i*2+4])*16+YS_CodeHexCharConver(DataPoint[i*2+5]);
            pos++;
        }

        if(YS_UartParaseComProtocl(Data, pos)==TRUE)
        {
            YS_UartInputCmdDeal(t_AvlComParase.v_CmdID,t_AvlComParase.a_dbuf,t_AvlComParase.v_dlen);
        }
    }
    if ((DataPoint[0]=='A')&&(DataPoint[1]=='T')&&(DataPoint[2]=='*')&&(DataPoint[3]=='*')&&(DataPoint[len-1]==0x0d))
    {
        if (DataPoint[2]=='*')//透传包
        {
            for ( i = 0; i<len-5; i++)
            {
                t_TxzComParase.a_dbuf[i] = DataPoint[4+i];
            }
            YS_UartInputCmdControl(t_TxzComParase.a_dbuf, len-5);
        }
    }
}


/*------------------------------------------------------------------------------------------
函数名：YS_UartDebugInterfacel
功能说明：通过串口向外输出调试数据
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_UartDebugInterfacel(unsigned short int CmdID, unsigned char *dbuf, unsigned short int dlen)
{
    u8 fbuf[4];
    u16 PackLen;

    YS_PrmReadOneItem(FLH_PRM_DEBUG_ENABLE,FLH_PRM_DEBUG_ENABLE_LEN,fbuf);
    if(fbuf[0]==1)
    {
        if(dlen<=600)
        {
            PackLen=YS_UartPackProtocl(CmdID, dbuf, dlen, UartSendBuf);
            sjfun_Debug_Uart_send(UartSendBuf,PackLen);  //发送数据
        }
        else
        {
            YS_UartDebugLengError(dbuf,dlen);
        }
    }
}

void YS_UartDebugString(char *DebugString)
{
    u8 DebugBuf[100];
    u16 i,len;

    len=strlen(DebugString);
    for(i=0; i<len; i++)
    {
        DebugBuf[i]=DebugString[i];
    }
    sjfun_Debug_Uart_send((U8*)DebugBuf, len);
//	YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO,(u8*)DebugBuf,len);
}

/*------------------------------------------------------------------------------------------
函数名：YS_UartDataOutInterfacel
功能说明：通过串口非控制运行数据
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_UartDataOutInterfacel(unsigned short int CmdID, unsigned char *dbuf, unsigned short int dlen)
{
    u16 PackLen;

    PackLen=YS_UartPackProtocl(CmdID, dbuf, dlen, UartSendBuf);
    sjfun_Debug_Uart_send(UartSendBuf,PackLen);  //发送数据*/
}


/*------------------------------------------------------------------------------------------
函数名：YS_UartDebugLengError
功能说明：通过串口向外输出调试数据
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_UartDebugLengError(u8 *dbuf, u16 dlen)
{
    char DispBuf[100];
    u16 PackLen;

    sprintf(DispBuf,"Debug data Length:%d",dlen);
    PackLen=YS_UartPackProtocl(INTER_PTL_UPDATE_DEBUGINFO, (u8 *)DispBuf,strlen(DispBuf), UartSendBuf);
    sjfun_Debug_Uart_send(UartSendBuf,PackLen);  //发送数据
}


/*-----------------------------------------------------------------------------------------
函数名：YS_UartCmdAckDeal
功能说明：短消息的发送接口
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_UartCmdAckDeal(u8 RltID)
{
    u8 uart_buf[100],fbuf[40],uart_str[30];
    u8 i,len,pos;
    u8  iperr[] = "#IP#FAIL#";
    u8  ipok[] = "#IP#OK#";
    u8  ip[] = "#IP#";
    u8  gpsdata[] = "#GPSDATA#OK#";
    u8  obddata[] = "#OBD#OK#";
    u8  gpsok[] = "#GPS#OK#";
    u8  gps_1[] = "#GPS#1#";
    u8  gps_0[] = "#GPS#0#";
    u8  apnok[] = "#APN#OK#";
    u8  apn[] =   "#APN#";
    u8  login[] = "#LOGIN#";
    u8  smsok[]=  "#SMS#OK#";
    u8  rstok[]=  "#RST#OK#";
    u8  imei[] = "#IMEI#";
    u8  ver[] = "#VER#";

    t_Sys_Run_Status GetSysStatus;

    memset(fbuf,0,40);
    YS_RunGetSystemStatus(&GetSysStatus);
    pos=0;
    uart_buf[pos]='*';
    pos++;
    uart_buf[pos]='*';
    pos++;
    switch (RltID)
    {
        case UART_ACK_IP_ERR :
            len = strlen((char *)iperr);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=iperr[i];
                pos++;
            }
            break;

        case UART_ACK_IP_OK :
            len = strlen((char *)ipok);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=ipok[i];
                pos++;
            }
            break;

        case UART_ACK_IP_CHK:
            len = strlen((char *)ip);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=ip[i];
                pos++;
            }
            YS_PrmReadOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,fbuf);//ip地址端口
            len=YS_CodeBufRealLen(fbuf,FLH_PRM_SERV_ADDR_LEN);
            for(i=0; i<len; i++)
            {
                uart_buf[pos]=fbuf[i];
                pos ++;
            }
            uart_buf[pos]='#';
            pos ++;
            YS_PrmReadOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
            sprintf((char *)uart_str,"%d",fbuf[0]*256+fbuf[1]);
            len=strlen((char *)uart_str);
            for(i=0; i<len; i++)
            {
                uart_buf[pos]=uart_str[i];
                pos++;
            }
            uart_buf[pos]='#';
            pos ++;
            break;

        case UART_ACK_OBD_OK:
            len = strlen((char *)obddata);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=obddata[i];
                pos++;
            }
            break;

        case UART_ACK_GPSDATA_OK:
            len = strlen((char *)gpsdata);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=gpsdata[i];
                pos++;
            }
            break;

        case UART_ACK_GPS_OK:
            len = strlen((char *)gpsok);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=gpsok[i];
                pos++;
            }
            break;

        case UART_ACK_GPS_CHK:
            if (YS_RunGetGpsPower()==TRUE)
            {
                len = strlen((char *)gps_1);
                for ( i = 0; i<len; i++ )
                {
                    uart_buf[pos]=gps_1[i];
                    pos++;
                }
            }
            else
            {
                len = strlen((char *)gps_0);
                for ( i = 0; i<len; i++ )
                {
                    uart_buf[pos]=gps_0[i];
                    pos++;
                }
            }

            break;

        case UART_ACK_APN_OK:
            len = strlen((char *)apnok);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=apnok[i];
                pos++;
            }
            break;

        case UART_ACK_APN_CHK:
            len = strlen((char *)apn);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=apn[i];
                pos++;
            }
            YS_PrmReadOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,fbuf);
            len = strlen((char *)fbuf);
            for(i=0; i<len; i++)
            {
                uart_buf[pos]=fbuf[i];
                pos++;
            }
            uart_buf[pos]='#';
            pos ++;
            break;

        case UART_ACK_LOGIN_CHK:
            len = strlen((char *)login);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=login[i];
                pos++;
            }
            if (GetSysStatus.RunFlow == YS_RUN_FLOW_IDLE_DEAL)
            {
                uart_buf[pos]='1';
                pos++;
            }
            else
            {
                uart_buf[pos]='0';
                pos++;
            }
            uart_buf[pos]='#';
            pos ++;
            break;

        case UART_ACK_SMS_OK:
            len = strlen((char *)smsok);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=smsok[i];
                pos++;
            }
            break;

        case UART_ACK_RST_OK:
            len = strlen((char *)rstok);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=rstok[i];
                pos++;
            }
            break;

        case UART_ACK_IMEICHK:
            len = strlen((char *)imei);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=imei[i];
                pos++;
            }
            YS_RunGetIMEI((char *)fbuf);
            len = strlen((char *)fbuf);
            for(i=0; i<len; i++)
            {
                uart_buf[pos]=fbuf[i];
                pos++;
            }
            uart_buf[pos]='#';
            pos ++;
            break;

        case UART_ACK_VERCHK:
            len = strlen((char *)ver);
            for ( i = 0; i<len; i++ )
            {
                uart_buf[pos]=ver[i];
                pos++;
            }
            len=strlen(EBIKE_SOFT_VER);
            for(i=0; i<len; i++)
            {
                uart_buf[pos]=EBIKE_SOFT_VER[i];
                pos++;
            }
            uart_buf[pos]='#';
            pos ++;
            break;

        case UART_ACK_ID_OK:
            uart_buf[pos]='#';
            pos ++;
            uart_buf[pos]='I';
            pos ++;
            uart_buf[pos]='D';
            pos ++;
            uart_buf[pos]='#';
            pos ++;
            YS_RunAddIDInfo((u8 *)fbuf);
            len=YS_CodeBufRealLen((u8 *)fbuf,FLH_PRM_SIM_CODE_LEN);
            for(i=0; i<len; i++)
            {
                uart_buf[pos]=fbuf[i];
                pos++;
            }
            uart_buf[pos]='#';
            pos ++;
            break;

        default:
            break;
    }

    uart_buf[pos]='\r';
    pos++;
    uart_buf[pos]='\n';
    pos++;

    sjfun_Debug_Uart_send(uart_buf,pos);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsCompHeadDeal
功能说明：比较数据头
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_UartCompHeadDeal(char *HeadStr, u8 *dbuf,u16 dlen)
{
    u16 i,len;
    u8 CompBuf[40];
    bool CompRlt;

    len=strlen(HeadStr);
    if(dlen<len)
    {
        return(FALSE);
    }

    len=strlen(HeadStr);
    for(i=0; i<len; i++)
    {
        CompBuf[i]=toupper(dbuf[i]);
    }

    CompRlt=TRUE;
    for(i=0; i<len; i++)
    {
        if(CompBuf[i]!=HeadStr[i])
        {
            CompRlt=FALSE;
            break;
        }
    }
    return(CompRlt);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_SmsSetServerAddress
功能说明：串口查询配置IP端口
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_UartSetServerAddress(u8 *dbuf,u16 dlen)
{
    char StrDat[20];
    u8	fbuf[20];
    u16 i,pos,count,Value;

    if (YS_UartCompHeadDeal(UART_CMD_HEAD_IPPORT_CHK,dbuf,dlen)==TRUE)
    {
        YS_UartCmdAckDeal(UART_ACK_IP_CHK);
        return TRUE;
    }
    else if(YS_UartCompHeadDeal(UART_CMD_HEAD_IPPORT,dbuf,dlen)==FALSE)
    {
        return FALSE;
    }
    count=0;
    for(i=0; i<dlen; i++)
    {
        if(dbuf[i]=='#')
        {
            count++;
        }
    }

    if(count!=4)  //字段数目不足
    {
        YS_UartCmdAckDeal(UART_ACK_IP_ERR);
        return TRUE;
    }
    else
    {
        count=0;
        pos=0;
        for(i=0; i<dlen; i++)
        {
            if(dbuf[i]=='#')
            {
                count++;
                if(count==2)
                {
                    pos=i+1;
                }
            }
        }

        count=0;
        for(i=pos; i<dlen; i++)
        {
            if(dbuf[i]=='#')
            {
                break;
            }
            if(count<20)
            {
                count++;
                StrDat[i-pos]=dbuf[i];
            }
        }

        if(count<10)
        {
            YS_UartCmdAckDeal(UART_ACK_IP_ERR);
            return(TRUE);
        }
        StrDat[count]=0;
        YS_PrmWriteOneItem(FLH_PRM_SERV_ADDR,FLH_PRM_SERV_ADDR_LEN,(u8 *)StrDat);
        count=0; //端口处理
        for(i=0; i<dlen; i++)
        {
            if(dbuf[i]=='#')
            {
                count++;
                if(count==3)
                {
                    pos=i+1;
                }
            }
        }
        count=0;
        for(i=pos; i<dlen; i++)
        {
            if(dbuf[i]=='#')
            {
                break;
            }
            if(count<20)
            {
                count++;
                StrDat[i-pos]=dbuf[i];
            }
        }

        if(count==0)
        {
            YS_UartCmdAckDeal(UART_ACK_IP_ERR);
        }
        else
        {
            StrDat[count]=0;
            if(YS_CodePDNumString((char *)StrDat)==FALSE) //非法字符
            {
                YS_UartCmdAckDeal(UART_ACK_IP_ERR);
            }
            else
            {
                Value=atoi((char *)StrDat);
                fbuf[0]=Value/256;
                fbuf[1]=Value%256;
                YS_PrmWriteOneItem(FLH_PRM_SERV_PORT,FLH_PRM_SERV_PORT_LEN,fbuf);
                YS_UartCmdAckDeal(UART_ACK_IP_OK);
//                YS_RunSetSysStatus(YS_RUN_FLOW_RDCON_BEGIN);
            }
        }
    }
    return(TRUE);

}


/*------------------------------------------------------------------------------------------
函数名：YS_UartInputCmdControl
功能说明：串口数据对应指令的处理。
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_UartInputCmdControl(u8 *dbuf, u8 dlen)
{
    u16 i,count,GetLen,GetLen1,Value,pos;
    u8 StrDat[20],fbuf[20];
    u8 smsphonenum[15];
    u8 smsdata[100];

    if (YS_UartSetServerAddress(dbuf,dlen)==TRUE)
    {
        return TRUE;
    }
    else if(YS_UartCompHeadDeal(UART_CMD_HEAD_OBD,dbuf,dlen)==TRUE)
    {
        GetLen=YS_CodeGetItemInBuf(dbuf,dlen,StrDat,2,'#',20);
        StrDat[GetLen]=0;
        if ('0'==StrDat[0])
        {
            fbuf[0]=0;
        }
        else
        {
            fbuf[0]=1;
        }
        YS_PrmWriteOneItem(FLH_PRM_OBD_DEBUG_EN,FLH_PRM_OBD_DEBUG_EN_LEN,fbuf);
        YS_UartCmdAckDeal(UART_ACK_OBD_OK);
    }
    else if(YS_UartCompHeadDeal(UART_CMD_HEAD_GPSDATA,dbuf,dlen)==TRUE)
    {
        GetLen=YS_CodeGetItemInBuf(dbuf,dlen,StrDat,2,'#',20);
        StrDat[GetLen]=0;
        if ('0'==StrDat[0])
        {
            fbuf[0]=0;
        }
        else
        {
            fbuf[0]=1;
        }
        YS_PrmWriteOneItem(FLH_PRM_GPS_DEBUG_EN,FLH_PRM_GPS_DEBUG_EN_LEN,fbuf);
        YS_UartCmdAckDeal(UART_ACK_GPSDATA_OK);
    }
    else if(YS_UartCompHeadDeal(UART_CMD_HEAD_GPS,dbuf,dlen)==TRUE)
    {
        GetLen=YS_CodeGetItemInBuf(dbuf,dlen,StrDat,2,'#',20);
        StrDat[GetLen]=0;
        if ('?'==StrDat[0])
        {
            YS_UartCmdAckDeal(UART_ACK_GPS_CHK);
        }
        else if ('0'==StrDat[0])
        {
            //关闭GPS 电源
            sjfun_Gpio_Write_Value(YS_PIN_NO_GPS_PWR,0);
            YS_RunSetGpsPower(0);
            YS_UartCmdAckDeal(UART_ACK_GPS_OK);
        }
        else
        {
            //打开GPS 电源
            sjfun_Gpio_Write_Value(YS_PIN_NO_GPS_PWR,1);
            YS_RunSetGpsPower(1);
            YS_UartCmdAckDeal(UART_ACK_GPS_OK);
        }

    }
    else if(YS_UartCompHeadDeal(UART_CMD_HEAD_APN,dbuf,dlen)==TRUE)
    {
        GetLen=YS_CodeGetItemInBuf(dbuf,dlen,StrDat,2,'#',20);
        StrDat[GetLen]=0;
        if ('?'==StrDat[0])
        {
            YS_UartCmdAckDeal(UART_ACK_APN_CHK);
        }
        else
        {
            YS_PrmWriteOneItem(FLH_PRM_PPP_APN,FLH_PRM_PPP_APN_LEN,StrDat);
            YS_UartCmdAckDeal(UART_ACK_APN_OK);
            YS_SysRsqSystemRST(YS_RST_FLAG_USER);
        }

    }
    else if(YS_UartCompHeadDeal(UART_CMD_HEAD_LOGIN,dbuf,dlen)==TRUE)
    {
        YS_UartCmdAckDeal(UART_ACK_LOGIN_CHK);
    }
    else if(YS_UartCompHeadDeal(UART_CMD_HEAD_SMS,dbuf,dlen)==TRUE)
    {
        GetLen=YS_CodeGetItemInBuf(dbuf,dlen,smsphonenum,2,'#',20);
        smsphonenum[GetLen]=0;
        GetLen1=YS_CodeGetItemInBuf(dbuf,dlen,smsdata,3,'#',100);
        smsdata[GetLen1]=0;
        YS_SmsSendInterface(smsdata,GetLen1,smsphonenum,GetLen,TRUE);
        YS_UartCmdAckDeal(UART_ACK_SMS_OK);
    }
    else if (YS_UartCompHeadDeal(UART_CMD_HEAD_RST,dbuf,dlen)==TRUE)
    {
        YS_SysRsqSystemRST(YS_RST_FLAG_USER);
        YS_UartCmdAckDeal(UART_ACK_RST_OK);
    }
    else if (YS_UartCompHeadDeal(UART_CMD_HEAD_IMEICHK,dbuf,dlen)==TRUE)
    {
        YS_UartCmdAckDeal(UART_ACK_IMEICHK);
    }
    else if (YS_UartCompHeadDeal(UART_CMD_HEAD_VERCHK,dbuf,dlen)==TRUE)
    {
        YS_UartCmdAckDeal(UART_ACK_VERCHK);
    }
    else if (YS_UartCompHeadDeal(UART_CMD_HEAD_ID,dbuf,dlen)==TRUE)
    {
        GetLen=YS_CodeGetItemInBuf(dbuf,dlen,StrDat,2,'#',20);
        StrDat[GetLen]=0;
        if ('?'==StrDat[0])
        {
            YS_UartCmdAckDeal(UART_ACK_ID_OK);
        }
        else
        {
            YS_PrmWriteOneItem(FLH_PRM_SIM_CODE,FLH_PRM_SIM_CODE_LEN, StrDat);
            YS_UartCmdAckDeal(UART_ACK_ID_OK);
        }
    }
    return FALSE;
}

