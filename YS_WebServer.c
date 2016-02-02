/*-----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------
                                       YS_WebServer.c  2013-07-17
此文件用管理GPS 数据到中文地址的转换
-------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------*/
#include "YS_Public.h"

u8	Web_Server_Buf[2048];
u8  Web_Rec_Buf[2048];
t_Web_Server_Manage	t_WebManage;

t_File_Para WebFilePara;
// http://qq.dogcare.com.cn

u32 crcrlt=0xffffffff;
static u32  CRC32_Table[256] =
{

    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,

    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,

    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,

    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,

    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,

    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,

    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,

    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,

    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,

    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,

    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,

    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,

    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,

    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,

    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,

    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,

    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,

    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,

    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,

    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,

    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,

    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,

    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,

    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,

    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,

    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,

    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,

    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,

    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,

    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,

    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,

    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,

    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,

    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,

    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,

    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,

    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,

    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,

    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,

    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,

    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,

    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,

    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,

    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,

    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,

    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,

    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,

    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,

    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,

    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,

    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,

    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,

    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,

    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,

    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,

    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,

    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,

    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,

    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,

    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,

    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D

};

//CRC32位校验
u32 crc32( const unsigned char *buf, u32 size, u32 crc)
{
    u32 i;

    for (i = 0; i < size; i++)
        crc = CRC32_Table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
    return crc;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_LongToPosStr
功能说明：GPS 数据转换为度字符串格式
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_LongToPosStr(u8 *Pos, char *BackStr)
{
    char StrDat[20];
    double f1,f2;

    sprintf(StrDat,"%02d.%02d%02d",Pos[1],Pos[2],Pos[3]);
    f1=Pos[0];
    f2=atof(StrDat);
    f2/=60;
    f1+=f2;
    sprintf(BackStr,"%.6f",f1);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebServicePackageRsq
功能说明：判断是否需要升级
修改记录：
-------------------------------------------------------------------------------------------*/
u16	YS_WebServicePackageRsq(u8 *PackBuf)
{
    u16 pos,i,len;

    pos=0;
    len=strlen(WEBSTR_RSQ);  //第一行数据
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_RSQ[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;

    len=strlen(WEBSTR_1); //第二行数据
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_1[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;


    len=strlen(WEBSTR_2);
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_2[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;


    len=strlen(WEBSTR_3);
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_3[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;

    len=strlen(WEBSTR_4);
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_4[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;

    len=strlen(WEBSTR_5);
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_5[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;

    len=strlen(WEBSTR_6);
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_6[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;
    return(pos);
}


/*-----------------------------------------------------------------------------------------
函数名：YS_WebServicePackageRsq
功能说明：判断是否需要升级
修改记录：
-------------------------------------------------------------------------------------------*/
u16	YS_WebServicePackageUpdate(u8 *PackBuf)
{
    u16 pos,i,len;

    pos=0;
    len=strlen(WEBSTR_UPDATE);  //第一行数据
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_UPDATE[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;

    len=strlen(WEBSTR_1); //第二行数据
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_1[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;


    len=strlen(WEBSTR_2);
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_2[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;


    len=strlen(WEBSTR_3);
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_3[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;

    len=strlen(WEBSTR_4);
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_4[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;

    len=strlen(WEBSTR_5);
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_5[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;

    len=strlen(WEBSTR_6);
    for(i=0; i<len; i++)
    {
        PackBuf[pos]=WEBSTR_6[i];
        pos++;
    }
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;
    PackBuf[pos]=0x0D;
    pos++;
    PackBuf[pos]=0x0A;
    pos++;
    return(pos);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebDWRsqAdd
功能说明：添加一个中文位置请求
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_WebDWRsqAdd(u8 *PhoneData, u8 PhoneLen, u8 EventFlag)
{
    u8 i,p;

    if(t_WebManage.CmdNum<Web_Cmd_Buf_NUM)
    {
        p=t_WebManage.CmdWritePoint;
        t_WebManage.a_PhoneLen[p]=PhoneLen;
        t_WebManage.a_EventFlag[p]=EventFlag;

        memset(t_WebManage.a_PhoneData[p], 0x00,20);
        for(i=0; i<PhoneLen; i++)
        {
            t_WebManage.a_PhoneData[p][i]=PhoneData[i];
        }
        t_WebManage.CmdNum++;
        t_WebManage.CmdWritePoint++;
        if(t_WebManage.CmdWritePoint>=Web_Cmd_Buf_NUM)
        {
            t_WebManage.CmdWritePoint=0;
        }
        YS_GprsDebugString("Add one DW rsq ok",t_WebManage.CmdNum);
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebAddRequest
功能说明：升级请求 一次
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_WebAddRequest(void)
{
    YS_GprsDebugString("ycsj request!",0);
    t_WebManage.RsqUpdateOKFlag=0;
    t_WebManage.CmdNum = 1;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebDWRsqAdd
功能说明：从请求队列中删除一个请求
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_WebDWRsqDeleteOne(void)
{
    t_WebManage.CmdNum--;
    t_WebManage.CmdReadPoint++;
    if(t_WebManage.CmdReadPoint>=Web_Cmd_Buf_NUM)
    {
        t_WebManage.CmdReadPoint=0;
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebUpdateRsqDeal
功能说明：判断升级请求
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_WebUpdateRsqDeal(void)
{
    u16 PackLen;

    t_WebManage.RsqSendDelay++;
    if(t_WebManage.RsqSendDelay>=Web_RSQ_SEND_DELAY)
    {
        t_WebManage.RsqSendDelay=0;
        t_WebManage.RsqSendTimes++;
        if(t_WebManage.RsqSendTimes<=Web_RSQ_SEND_TIMES)
        {
            PackLen = YS_WebServicePackageRsq(Web_Server_Buf);
            YS_RunWebSendInterface(Web_Server_Buf,PackLen);
            ycsj_debug((char *)Web_Server_Buf);
        }
        else
        {
            t_WebManage.RsqDataFailFlag=1;
        }
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebUpdateRsqDataDeal
功能说明：请求升级
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_WebUpdateRsqDataDeal(void)
{
    u16 PackLen;

    t_WebManage.RsqSendDelay++;
    if(t_WebManage.RsqSendDelay>=Web_RSQ_SEND_DELAY)
    {
        t_WebManage.RsqSendDelay=0;
        t_WebManage.RsqSendTimes++;
        if(t_WebManage.RsqSendTimes<=Web_RSQ_SEND_TIMES)
        {
            PackLen = YS_WebServicePackageUpdate(Web_Server_Buf);
            YS_RunWebSendInterface(Web_Server_Buf,PackLen);
            ycsj_debug((char *)Web_Server_Buf);
        }
        else
        {
            t_WebManage.RsqDataFailFlag=1;
        }
    }
}

void YS_WebSocketClose(void)
{
    t_WebManage.RsqDataOKFlag = 1;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebSetDnsOKFlag
功能说明：设置DNS 解析成功标志
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_WebSetDnsOKFlag(void)
{
    t_WebManage.DnsOKFlag=TRUE;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebSetDnsFailFlag
功能说明：设置DNS 解析失败标志
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_WebSetDnsFailFlag(void)
{
    t_WebManage.DnsFailFlag=TRUE;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebServiceRsqFlow
功能说明：中文位置请求流程处理
修改记录：
-------------------------------------------------------------------------------------------*/
void	YS_WebServiceRsqFlow(void)
{
    int res=0;
    u8 DnsRlt;
//    ycsj_debug("WEB RUN FLOW:%d\r\n", t_WebManage.RsqFlow);
    switch(t_WebManage.RsqFlow)
    {
        case WEB_RSQ_FLOW_SELCMD:
            if(t_WebManage.CmdNum>0)
            {
                YS_GprsDebugString("WEB_RSQ_FLOW_SELCMD OK",0);
                t_WebManage.RsqFlow=WEB_RSQ_FLOW_PPP;
            }
            break;

        case WEB_RSQ_FLOW_PPP:
            YS_GprsDebugString("WEB_RSQ_FLOW_PPP OK",0);
            //YS_RunPPPInterfaceNew();
            YS_RunWebPPPInterface();
            t_WebManage.RsqFlow=WEB_RSQ_FLOW_SCOKET;
            break;
#if 0
        case WEB_RSQ_FLOW_DNS_BEGIN: //DNS解析入口
            t_WebManage.DelayCount=0;
            t_WebManage.DnsOKFlag=FALSE;
            t_WebManage.DnsFailFlag=FALSE;
            YS_RunWebDnsInterface();
            t_WebManage.RsqFlow=WEB_RSQ_FLOW_DNS_WAIT;
            break;

        case WEB_RSQ_FLOW_DNS_WAIT: //DNS解析等待
            if(t_WebManage.DnsOKFlag==TRUE)
            {
                t_WebManage.RsqFlow=WEB_RSQ_FLOW_SCOKET;  //连接SOCKET
            }
            else if(t_WebManage.DnsFailFlag==TRUE)
            {
                t_WebManage.RsqFlow=WEB_RSQ_FLOW_PPP; //DNS 失败，退出
            }
            else
            {
                t_WebManage.DelayCount++;
                if(t_WebManage.DelayCount>=60)
                {
                    YS_UartDebugString("WEB Dns parase fulltime exit!!!");
                    t_WebManage.RsqFlow=WEB_RSQ_FLOW_SELCMD;
                }
            }
            break;
#endif
        case WEB_RSQ_FLOW_SCOKET:
            YS_GprsDebugString("WEB_RSQ_FLOW_SCOKET OK",0);
            t_WebManage.DelayCount=0;
            t_WebManage.SckOKFlag=0;
            YS_RunWebSocketInterFace();
            t_WebManage.RsqFlow=WEB_RSQ_FLOW_SCOKET_WAIT;
            break;

        case WEB_RSQ_FLOW_SCOKET_WAIT:
            if(t_WebManage.SckOKFlag==1)
            {
                YS_GprsDebugString("Get socket connect msg",0);
                if (t_WebManage.RsqUpdateOKFlag==0)
                {
                    t_WebManage.RsqFlow=WEB_RSQ_FLOW_RSQ;
                }
                else
                {
                    t_WebManage.RsqFlow=WEB_RSQ_FLOW_RSQDATA;
                }
            }
            else
            {
                t_WebManage.DelayCount++;
                if(t_WebManage.DelayCount>=Web_SCK_WAIT_DELAY)
                {
                    YS_GprsDebugString("Get socket connect full time",0);
                    YS_RunWebSocketClose();
                    YS_WebDWRsqDeleteOne();
                    t_WebManage.DelayCount=0;
                    t_WebManage.RsqFlow=WEB_RSQ_FLOW_EXIT;
                }
            }
            break;

        case WEB_RSQ_FLOW_RSQ:
            t_WebManage.RsqSendDelay=Web_RSQ_SEND_DELAY;
            t_WebManage.RsqSendTimes=0;
            t_WebManage.RsqOKFlag=0;
            t_WebManage.RsqFailFlag=0;
            t_WebManage.RsqUpdateOKFlag=0;
            t_WebManage.RsqFlow=WEB_RSQ_FLOW_RSQ_WAIT;
            break;

        case WEB_RSQ_FLOW_RSQ_WAIT:
            if (t_WebManage.RsqOKFlag==1)
            {
                YS_GprsDebugString("Web rsq ok",0);
                YS_RunWebSocketClose();
                YS_WebDWRsqDeleteOne();
                t_WebManage.DelayCount=0;
                t_WebManage.RsqFlow=WEB_RSQ_FLOW_EXIT;
            }
            else if (t_WebManage.RsqUpdateOKFlag==1)//判断需要升级
            {
                YS_GprsDebugString("Web rsq updata ok",0);
                //删掉加载的BIN文件
#if 1
                res = sjfun_File_Open((kal_uint16*)UPDATE_FILE_NAME,  FS_READ_ONLY);
                if (res > 0)
                {
                    sjfun_File_Close(res);
                    res=sjfun_File_Delete(UPDATE_FILE_NAME);
                    YS_GprsDebugString("File_Delete rlt",res);
                }
#endif
                YS_RunWebSocketClose();
                YS_RunSetFlowRst();
                t_WebManage.RsqFlow=WEB_RSQ_FLOW_SELCMD;
            }
            else if(t_WebManage.RsqDataFailFlag==1)
            {
                YS_GprsDebugString("Web rsq data fail",0);
                YS_RunWebSocketClose();
                YS_WebDWRsqDeleteOne();
                t_WebManage.DelayCount=0;
                t_WebManage.RsqFlow=WEB_RSQ_FLOW_EXIT;
            }
            else
            {
                YS_WebUpdateRsqDeal();
            }
            break;

        case WEB_RSQ_FLOW_RSQDATA:
            YS_GprsDebugString("WEB_RSQ_FLOW_RSQDATA",0);
            WebFilePara.filehandle = sjfun_File_Open((kal_uint16*)UPDATE_FILE_NAME,  FS_READ_WRITE | FS_CREATE_ALWAYS);
            YS_GprsDebugString("WEB_RSQ_FLOW_RSQDATA handle",WebFilePara.filehandle);
            WebFilePara.writepos = 0;
            WebFilePara.packnum = 0;
            t_WebManage.RsqSendDelay=Web_RSQ_SEND_DELAY;
            t_WebManage.RsqSendTimes=0;
            t_WebManage.RsqDataOKFlag=0;
            t_WebManage.RsqDataFailFlag=0;
            t_WebManage.RsqFlow=WEB_RSQ_FLOW_RSQDATA_WAIT;
            break;

        case WEB_RSQ_FLOW_RSQDATA_WAIT:
            if(t_WebManage.RsqDataOKFlag==1)
            {
                YS_GprsDebugString("Web rsq data ok",0);
                YS_RunWebSocketClose();
                sjfun_File_Close(WebFilePara.filehandle);
#if 1
                if (t_WebManage.CrcCode != crcrlt)
                {
                    res = sjfun_File_Open((kal_uint16*)UPDATE_FILE_NAME,  FS_READ_ONLY);
                    if (res > 0)
                    {
                        sjfun_File_Close(res);
                        res=sjfun_File_Delete(UPDATE_FILE_NAME);
                        YS_GprsDebugString("File_Delete rlt",res);
                    }
                }
#endif
                YS_WebDWRsqDeleteOne();
                t_WebManage.DelayCount=0;
                t_WebManage.RsqFlow=WEB_RSQ_FLOW_EXIT;
            }
            else if(t_WebManage.RsqDataFailFlag==1)
            {
                YS_GprsDebugString("Web rsq data fail",0);
                YS_RunWebSocketClose();
                YS_WebDWRsqDeleteOne();
#if 1
                sjfun_File_Close(WebFilePara.filehandle);
                res = sjfun_File_Open((kal_uint16*)UPDATE_FILE_NAME,  FS_READ_ONLY);
                if (res > 0)
                {
                    sjfun_File_Close(res);
                    res=sjfun_File_Delete(UPDATE_FILE_NAME);
                    YS_GprsDebugString("File_Delete rlt",res);
                }
#endif
                t_WebManage.DelayCount=0;
                t_WebManage.RsqFlow=WEB_RSQ_FLOW_EXIT;
            }
            else
            {
                YS_WebUpdateRsqDataDeal();
            }
            break;

        case WEB_RSQ_FLOW_EXIT:
            t_WebManage.DelayCount++;
            if(t_WebManage.DelayCount>=Web_Exit_Wait_Delay)
            {
                YS_GprsDebugString("WEB_RSQ_FLOW_EXIT OK",0);
                t_WebManage.RsqFlow=WEB_RSQ_FLOW_SELCMD;
                if ((t_WebManage.RsqDataOKFlag==1)||(t_WebManage.RsqDataFailFlag==1))
                {
                    YS_SysRsqSystemRST(YS_RST_FLAG_USER);
                }
            }
            break;

        default:
            break;
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebServiceSckConnectMsg
功能说明：WEB SOCKET 连接成功消息
修改记录：
-------------------------------------------------------------------------------------------*/
void	YS_WebServiceSckConnectMsg(void)
{
    t_WebManage.SckOKFlag=1;
}

bool YS_WebJudgeUpdate(u8 *buf, u16 len)
{
    bool rlt= FALSE;
    u16 verlen=0;
    u16 i=0;

    verlen = strlen(EBIKE_SOFT_VER);
    if(verlen != len)
    {
        return FALSE;
    }
    for ( i = 0; i<len; i++ )
    {
        if (buf[i] != EBIKE_SOFT_VER[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebGetGMTtime
功能说明：获取GMT时间
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_WebGetGMTtime(u8 *buf, u16 len)
{
    u16 GetLen;
    u8 value,StrDat[30];
    u8 DataBuf[6];
    char  *p1=NULL,*p2=NULL;
    char *str1="Date: ";
    char *str2="GMT";
    p1 = strstr((char *)buf,str1);
    p2 = strstr((char *)buf,str2);

    if(p2)
    {
        *p2 = 0;
        GetLen=YS_CodeGetItemInBuf((u8 *)p1,50,StrDat,4,' ',20);
        StrDat[GetLen]=0;
        DataBuf[0] = YS_CodeHexCharConver(StrDat[GetLen-2])*10+YS_CodeHexCharConver(StrDat[GetLen-1]);

        GetLen=YS_CodeGetItemInBuf((u8 *)p1,30,StrDat,3,' ',20);
        StrDat[GetLen]=0;
        DataBuf[1] = YS_CodeMouthHexConver((char *)StrDat);
        if (DataBuf[1] == 0xff)
        {
            return;
        }

        GetLen=YS_CodeGetItemInBuf((u8 *)p1,30,StrDat,2,' ',20);
        StrDat[GetLen]=0;
        DataBuf[2] = YS_CodeHexCharConver(StrDat[GetLen-2])*10+YS_CodeHexCharConver(StrDat[GetLen-1]);

        GetLen=YS_CodeGetItemInBuf((u8 *)p1,30,StrDat,1,':',20);
        StrDat[GetLen]=0;
        DataBuf[3] = YS_CodeHexCharConver(StrDat[GetLen-2])*10+YS_CodeHexCharConver(StrDat[GetLen-1]);

        GetLen=YS_CodeGetItemInBuf((u8 *)p1,30,StrDat,2,':',20);
        StrDat[GetLen]=0;
        DataBuf[4] = YS_CodeHexCharConver(StrDat[GetLen-2])*10+YS_CodeHexCharConver(StrDat[GetLen-1]);

        GetLen=YS_CodeGetItemInBuf((u8 *)p1,30,StrDat,5,' ',20);
        StrDat[GetLen]=0;
        DataBuf[5] = YS_CodeHexCharConver(StrDat[GetLen-2])*10+YS_CodeHexCharConver(StrDat[GetLen-1]);

        if(YS_RunGetDTSyncStatus()==FALSE)
        {
            ycsj_debug("the web time20%d-%d-%d %d:%d:%d",DataBuf[0],DataBuf[1],DataBuf[2],DataBuf[3],DataBuf[4],DataBuf[5]);
            YS_CodeCretaeBJDT(DataBuf);
            sjfun_Set_System_DateTime(DataBuf);
            YS_RunSetDTSyncFlag();
        }
        else
        {
            ycsj_debug("sync by WEB TIME already ok!\r\n");
        }
    }
    else
    {
        ycsj_debug("sync by WEB TIME failled!\r\n");
    }
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebFirstPackParase
功能说明：升级第一个包判断
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_WebFirstPackRsqParase(u8 *buf, u16 len)
{
    u16 i,j,pos;
    u8 GetLen;
    u8 StrDat[50];
    Avl_ULong ldat;

    YS_WebGetGMTtime(buf, len);
    for  ( i = 256; i<len; i++ )
    {
        if ((buf[i] == 0x0D)&&(buf[i+1] == 0x0A)&&(buf[i+2] == 0x0D)&&(buf[i+3] == 0x0A))
        {
            pos = i+4;
            break;
        }
    }
    YS_GprsDebugString((char *)&buf[pos],0);

    GetLen=YS_CodeGetItemInBuf(&buf[pos],len-pos,StrDat,0,' ',20);
    StrDat[GetLen]=0;
    if (GetLen <=3)
    {
        t_WebManage.RsqOKFlag=1;
        return;
    }
    ycsj_debug("THE VER IS:");
    ycsj_debug((char *)StrDat);
    if (YS_WebJudgeUpdate(StrDat, GetLen) == TRUE)
    {
        t_WebManage.RsqOKFlag=1;
    }
    else
    {
        t_WebManage.RsqUpdateOKFlag=1;
//        ldat.b[AVL_LSTOR_FIR] = buf[pos+GetLen+1];
//        ldat.b[AVL_LSTOR_SEC] = buf[pos+GetLen+2];
//        ldat.b[AVL_LSTOR_THI] = buf[pos+GetLen+3];
//        ldat.b[AVL_LSTOR_FOR] = buf[pos+GetLen+4];

        ldat.b[AVL_LSTOR_FIR] = buf[pos+GetLen+4];
        ldat.b[AVL_LSTOR_SEC] = buf[pos+GetLen+3];
        ldat.b[AVL_LSTOR_THI] = buf[pos+GetLen+2];
        ldat.b[AVL_LSTOR_FOR] = buf[pos+GetLen+1];
        t_WebManage.CrcCode = ldat.l;
        ycsj_debug("\r\n-1-THE crc code is:%lx\r\n",ldat.l);

        ldat.b[AVL_LSTOR_FIR] = buf[pos+GetLen+9];
        ldat.b[AVL_LSTOR_SEC] = buf[pos+GetLen+8];
        ldat.b[AVL_LSTOR_THI] = buf[pos+GetLen+7];
        ldat.b[AVL_LSTOR_FOR] = buf[pos+GetLen+6];

        WebFilePara.filelen = ldat.l;
        ycsj_debug("the file len%lx\r\n",WebFilePara.filelen);
    }
}


/*-----------------------------------------------------------------------------------------
函数名：YS_WebFirstPackParase
功能说明：升级第一个包判断
修改记录：
-------------------------------------------------------------------------------------------*/
u32 YS_WebFirstPackDataParase(u8 *buf, u16 len)
{
    u32 writed=0;
    int res;
    u16 i,j,pos;
    Avl_ULong ldat;

    for  ( i = 256; i<len; i++ )
    {
        if ((buf[i] == 0x0D)&&(buf[i+1] == 0x0A)&&(buf[i+2] == 0x0D)&&(buf[i+3] == 0x0A))
        {
            pos = i+4;
            break;
        }
    }
    crcrlt = crc32(&buf[pos], len-pos, crcrlt);

    res = sjfun_File_Write(WebFilePara.filehandle, &buf[pos], len-pos, &writed);
    YS_GprsDebugString("sjfun_File_Write rlt",res);
    return writed;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebServiceDataParase
功能说明：Web  平台应答数据解析处理
修改记录：
-------------------------------------------------------------------------------------------*/
void  YS_WebServiceDataParase(u8 *buf, u16 len)
{
    u32 writed=0;
    int res;
    if (t_WebManage.RsqFlow==WEB_RSQ_FLOW_RSQ_WAIT)
    {
        YS_WebFirstPackRsqParase(buf, len);
    }
    else if (t_WebManage.RsqFlow==WEB_RSQ_FLOW_RSQDATA_WAIT)
    {
        if (WebFilePara.packnum == 0)
        {
            writed = YS_WebFirstPackDataParase(buf, len);
            WebFilePara.writepos += writed;
        }
        else
        {
            res=sjfun_File_Seek(WebFilePara.filehandle, WebFilePara.writepos, 0);
            crcrlt = crc32(buf, len, crcrlt);
            res = sjfun_File_Write(WebFilePara.filehandle, buf, len, &writed);
            WebFilePara.writepos += writed;
            if (WebFilePara.writepos == WebFilePara.filelen)
            {
                crcrlt = crcrlt^0xffffffff;
                if(t_WebManage.CrcCode == crcrlt)
                {
                    ycsj_debug("the CrcCode is right!\r\n");
                }
                else //校验码 错误 删除
                {
                    ycsj_debug("the CrcCode is wrong!\r\n");
                }
                ycsj_debug("the crc rlt is:0x%lx", crcrlt);
                ycsj_debug("the update file len is:0x%lx", WebFilePara.writepos);
                YS_WebSocketClose();
                return;
            }
        }

        WebFilePara.packnum ++;
        if (len <1340)
        {
            crcrlt = crcrlt^0xffffffff;
            if(t_WebManage.CrcCode == crcrlt)
            {
                ycsj_debug("the CrcCode is right!\r\n");
            }
            else //校验码 错误 删除
            {
                ycsj_debug("the CrcCode is wrong!\r\n");
            }
            ycsj_debug("the crc rlt is:0x%lx", crcrlt);
            ycsj_debug("the update file len is:0x%lx", WebFilePara.writepos);
            YS_WebSocketClose();
        }
    }
    else
    {
        YS_GprsDebugString("Rec parase wrong",0);
    }

}

/*-----------------------------------------------------------------------------------------
函数名：YS_WebServiceInitParam
功能说明：WEB 处理初始化运行参数
修改记录：
-------------------------------------------------------------------------------------------*/
void	YS_WebServiceInitParam(void)
{
    //ycsj_debug("YS_WebServiceInitParam!!!\n");
    t_WebManage.RsqFlow=WEB_RSQ_FLOW_SELCMD;
    t_WebManage.CmdNum=0;
    t_WebManage.CmdReadPoint=0;
    t_WebManage.CmdWritePoint=0;
}
