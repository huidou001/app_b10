#include "YS_Public.h"

#define AVLGPS_NUM_STR_DATA 	"0123456789."
#define GOOGL_LINK_STRING		"http://maps.google.com/maps?q="
#define GODE_LINK_STRING		"http://mo.amap.com/?q="
#define GODE_LINK_END_STRING 	"&dev=1"

#define DIS_CALC_EARTH_R	6378144
#define DIS_CALC_PI			3.1415926535

//define month's days
u8  Month_Days_tab[]={
							31,	//一月
							28,	//二月
							31,	//三月
							30,	//四月
							31,	//五月
							30,	//六月
							31,	//七月
							31,	//八月
							30,	//九月
							31,	//十月
							30,	//十一月
							31	//十二月
 };



u8  Leap_Year_Month_Days_tab[]={
							31,	//一月
							29,	//二月
							31,	//三月
							30,	//四月
							31,	//五月
							30,	//六月
							31,	//七月
							31,	//八月
							30,	//九月
							31,	//十月
							30,	//十一月
							31	//十二月
 };
#define UNLEAP_YEAR_SEC	0x1E13380
#define LEAP_YEAR_SEC		0x1E28500
#define ONE_DAY_SEC_NUM	86400

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeHexCharConver
功能说明：将数据转换为16进制字符
修改记录：
-------------------------------------------------------------------------------------------*/
u8 YS_CodeMouthHexConver(char *Str)
{
	u8 result;

	if(strcmp("Jan", Str)==0)
	{
		return 1;
	}
	else if(strcmp("Feb", Str)==0)
	{
		return 2;
	}
	else if(strcmp("Mar", Str)==0)
	{
		return 3;
	}
	else if(strcmp("Apr", Str)==0)
	{
		return 4;
	}
	else if(strcmp("May", Str)==0)
	{
		return 5;
	}
	else if(strcmp("Jun", Str)==0)
	{
		return 6;
	}
	else if(strcmp("Jul", Str)==0)
	{
		return 7;
	}
	else if(strcmp("Aug", Str)==0)
	{
		return 8;
	}
	else if(strcmp("Sep", Str)==0)
	{
		return 9;
	}
	else if(strcmp("Oct", Str)==0)
	{
		return 10;
	}
	else if(strcmp("Nov", Str)==0)
	{
		return 11;
	}
	else if(strcmp("Dec", Str)==0)
	{
		return 12;
	}
	else
	{
		return 0xff;
	}

	return(result);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeHexCharConver
功能说明：将数据转换为16进制字符
修改记录：
-------------------------------------------------------------------------------------------*/
u8 YS_CodeHexCharConver(u8 Dat)
{
	u8 result;

	switch(Dat)
	{
	case '0':
		result=0;
		break;
	case '1':
		result=1;
		break;
	case '2':
		 result=2;
		 break;
	case '3':
		 result=3;
		 break;
	case '4':
		 result=4;
		 break;
	case '5':
		 result=5;
		 break;
	case '6':
		 result=6;
		 break;
	case '7':
		 result=7;
		 break;
	case '8':
		 result=8;
		 break;
	case '9':
		 result=9;
		 break;
	case 'a':
	case 'A':
		 result=10;
		 break;
	case 'b':
	case 'B':
		 result=11;
		 break;
	case 'c':
	case 'C':
		 result=12;
		 break;
	case 'd':
	case 'D':
		 result=13;
		 break;
	case 'e':
	case 'E':
		 result=14;
		 break;
	case 'f':
	case 'F':
		 result=15;
		 break;
	default:
		 result=0;
		 break;
	}
	return(result);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeHextoString
功能说明：将16进制转换为字符串
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_CodeHextoString(u8 *buf,u16 len, u8 *p)
{
	u16 i,pos;
	if(len == 0)
	{
		return;
	}
	if(p == NULL)
	{
		return;
	}
	pos = 0;
	for(i=0; i<len; i++)
	{
		if(buf[i]/16>=10)
		{
			p[pos] = 0x37 + (buf[i]/16);
			pos++;
		}
		else
		{
			p[pos] = 0x30 + (buf[i]/16);
			pos++;
		}

		if(buf[i]%16>=10)
		{
			p[pos] = 0x37 + (buf[i]%16);
			pos++;
		}
		else
		{
			p[pos] = 0x30 + (buf[i]%16);
			pos++;
		}

		p[pos] = ' ';
		pos++;

	}
	p[pos] = '\r';
	pos++;
	p[pos] = '\n';
	pos++;
	p[pos] = 0;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeaCharHexToInt
功能说明：将16进制字符串转换为u32 类型数据
修改记录：
-------------------------------------------------------------------------------------------*/
u32 YS_CodeaCharHexToInt(char *str)
{
	u8	len,byte[4];
	u32 Result;

	Result=0;
	len=strlen(str);
	if((len==0)||(len>8))
	return (Result);

	switch(len)
	{
	case 1:	byte[0]=YS_CodeHexCharConver(str[0]);
			Result=byte[0];
			break;
	case 2:
			byte[0]=YS_CodeHexCharConver(str[0])*16+YS_CodeHexCharConver(str[1]);
			Result=byte[0];
			break;

	case 3:
			byte[0]=YS_CodeHexCharConver(str[0]);
			byte[1]=YS_CodeHexCharConver(str[1])*16+YS_CodeHexCharConver(str[2]);
			Result=byte[0]*0x00000100+byte[1];
			break;
	case 4:
			byte[0]=YS_CodeHexCharConver(str[0])*16+YS_CodeHexCharConver(str[1]);
			byte[1]=YS_CodeHexCharConver(str[2])*16+YS_CodeHexCharConver(str[3]);
			Result=byte[0]*0x00000100+byte[1];
			break;
	case 5:
			byte[0]=YS_CodeHexCharConver(str[0]);
			byte[1]=YS_CodeHexCharConver(str[1])*16+YS_CodeHexCharConver(str[2]);
			byte[2]=YS_CodeHexCharConver(str[3])*16+YS_CodeHexCharConver(str[4]);
			Result=byte[0]*0x00010000+byte[1]*0x00000100+byte[2];
			break;
	case 6:
			byte[0]=YS_CodeHexCharConver(str[0])*16+YS_CodeHexCharConver(str[1]);
			byte[1]=YS_CodeHexCharConver(str[2])*16+YS_CodeHexCharConver(str[3]);
			byte[2]=YS_CodeHexCharConver(str[4])*16+YS_CodeHexCharConver(str[5]);
			Result=byte[0]*0x00010000+byte[1]*0x00000100+byte[2];
			break;
	case 7:
			byte[0]=YS_CodeHexCharConver(str[0]);
			byte[1]=YS_CodeHexCharConver(str[1])*16+YS_CodeHexCharConver(str[2]);
			byte[2]=YS_CodeHexCharConver(str[3])*16+YS_CodeHexCharConver(str[4]);
			byte[3]=YS_CodeHexCharConver(str[5])*16+YS_CodeHexCharConver(str[6]);
			Result=byte[0]*0x01000000+byte[1]*0x00010000+byte[2]*0x00000100+byte[3];
			break;
	case 8:
			byte[0]=YS_CodeHexCharConver(str[0])*16+YS_CodeHexCharConver(str[1]);
			byte[1]=YS_CodeHexCharConver(str[2])*16+YS_CodeHexCharConver(str[3]);
			byte[2]=YS_CodeHexCharConver(str[4])*16+YS_CodeHexCharConver(str[5]);
			byte[3]=YS_CodeHexCharConver(str[6])*16+YS_CodeHexCharConver(str[7]);
			Result=byte[0]*0x01000000+byte[1]*0x00010000+byte[2]*0x00000100+byte[3];
			break;

	default:
			break;

	}
	return (Result);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeHexCharConver
功能说明：将BCD码转数据
修改记录：
-------------------------------------------------------------------------------------------*/
u8  YS_CodeBCDCharConver(u8 *Dat, u8 len, u8 *OutBuf)
{
    u8 i;
    u8 Conlen=0;
    for(i=0; i<len; i++)
    {
        OutBuf[Conlen] = (Dat[i]&0x0f)+0x30;
        Conlen++;
        OutBuf[Conlen] = ((Dat[i]>>4)&0x0f)+0x30;
        Conlen++;
    }
    return (2*len);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodePDNumString
功能说明：判断字符串是否为数字字符串
修改记录：
-------------------------------------------------------------------------------------------*/
bool  YS_CodePDNumString(char *str)
{
	u8		i,j,len,count;
	bool		IsNum;

	len=strlen(str);
	if(len==0) return (FALSE);

	for(i=0; i<len; i++)
	{
		IsNum=FALSE;
		for(j=0; j<11; j++)
		{
			if(str[i]==AVLGPS_NUM_STR_DATA[j]) IsNum=TRUE;
		}
		if(IsNum==FALSE)   return (FALSE);  //该字符为非法信息
	}
	count=0;		//如果'.'个数大于1则认为是非字数据串
	for(i=0; i<len; i++)
	{
		if(str[i]=='.')
		count++;
	}

	if(count>1)
	{
		return (FALSE);
	}
	else
	{
		return (TRUE);
	}
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeBufRealLen
功能说明：求一个二进行缓冲区的有效长度
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_CodeBufRealLen(u8 *buf, u16 len)
{
	u16 i,count;

	count=0;
	for(i=0; i<len; i++)
	{
		if(buf[i]==0)
		{
			break;
		}
		count++;
	}
	return(count);
}

u8 YS_CodeItoa(u8 dat)
{
	u8 result=0;

	switch(dat)
	{

	case 0:
		result='0';
		break;
	case 1:
		result='1';
		break;
	case 2:
		result='2';
		break;
	case 3:
		result='3';
		break;
	case 4:
		result='4';
		break;
	case 5:
		result='5';
		break;
	case 6:
		result='6';
		break;
	case 7:
		result='7';
		break;
	case 8:
		result='8';
		break;
	case 9:
		result='9';
		break;
	case 10:
		result='A';
		break;
	case 11:
		result='B';
		break;
	case 12:
		result='C';
		break;
	case 13:
		result='D';
		break;
	case 14:
		result='E';
		break;
	case 15:
		result='F';
		break;
	default:
		break;
	}
	return(result);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeCretaeBJDT
功能说明：格林威治时间转换为北京时间
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_CodeCretaeBJDT(u8 *buf)
{
	u8 year,month,day,hour,Mdays;
	bool result,leap;

	result=TRUE;
	if(1)
	{
		year=buf[0];
		month=buf[1];
		day=buf[2];
		hour=buf[3];
		if((month>0)&&(month<13)&&(day>0)&&(day<32))
		{
			if(hour<16)//if not over one day
			{
				buf[3]+=8;
			}
			else//over one day
			{
				Mdays=Month_Days_tab[month-1]; //get cur month's days
				leap=FALSE; //leap year deal
				if((year%4)==0)  //2011-01-25
				{
					leap=TRUE;
				}
				if((leap==TRUE)&&(month==2))
				{
					Mdays++;
				}
				day++;
				if(day>Mdays)
				{
					day=1;
					month++;
					if(month>12)
					{
						month=1;
						year++;
					}
				}
				buf[3]+=8;
				buf[3]%=24;
				buf[2]=day;
				buf[1]=month;
				buf[0]=year;
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
函数名：YS_CompTwoString
功能说明：对两字符串做全匹配比较
修改记录：
-------------------------------------------------------------------------------------------*/
bool YS_CompTwoString(char *InputStr, char *CmdStr)
{
	u16 i,len1,len2;
	bool result;

	len1=strlen(InputStr);
	len2=strlen(CmdStr);
	if(len1!=len2)
	{
		result=FALSE;
	}
	else
	{
		result=TRUE;
		for(i=0; i<len1; i++)
		{
			if(InputStr[i]!=CmdStr[i])
			{
				result=FALSE;
				break;
			}
		}
	}
	return(result);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeLongToPos
功能说明：将LONG 转换为GPS 数据结构
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_CodeLongToPos(u32 LongPos, u8 *AimBuf)
{
	Avl_ULong ldat;
	double f1;
	char strpos[20];
	char strdat[10];
	u16 i;
	u32 tdat1,tdat2;
	ldat.l=LongPos;
	tdat1=ldat.l/30000;  //转换度数据
	AimBuf[0]=tdat1/60;
	tdat2=AimBuf[0]*30000;
	tdat2*=60;
	tdat1=ldat.l-tdat2;
	f1=((double)tdat1)/30000;
	f1/=100;
	sprintf(strpos,"%.6f",f1);
	for(i=0; i<3; i++)
	{
		strdat[0]=strpos[2*i+2];
		strdat[1]=strpos[2*i+3];
		strdat[2]=0;
		AimBuf[i+1]=atoi(strdat);
	}
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeGetItemInBuf
功能说明：获取由指定分隔符分隔的字符串条目
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_CodeGetItemInBuf(u8 *dbuf, u16 dlen, u8 *PackBuf,u8 ID,u8 KeyChar, u16 MaxLen)
{
	u16 i,pos,count;

	pos=0;
	count=0;
	if(ID>0)
	{
		for(i=0; i<dlen; i++)
		{
			if(dbuf[i]==KeyChar)
			{
				count++;
				if(count>=ID)
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


/*u16 YS_CodeCreateGoogleLink(u8 *PackBuf, u8 *Longitude, u8 *Latitude)
{
	u16 i,len,pos;
	char LongiStr[20];
	char LatiStr[20];

	pos=0;
	len=strlen(GOOGL_LINK_STRING);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=GOOGL_LINK_STRING[i];
		pos++;
	}

	YS_LongToPosStr(Longitude,LongiStr);
	YS_LongToPosStr(Latitude,LatiStr);

	len=strlen(LatiStr);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=LatiStr[i];
		pos++;
	}

	PackBuf[pos]=' ';
	pos++;

	len=strlen(LongiStr);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=LongiStr[i];
		pos++;
	}
	return(pos);
}*/

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeCalcTowPointTest
功能说明：距离计算测试入口
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_CodeCalcTowPointTest(u8 *dbuf, u16 dlen, u8 *PackBuf)
{
	u8 Longi1[4],Longi2[4],Lati1[4],Lati2[4];
	u8 i;
	Avl_ULong ldat;

	for(i=0; i<4; i++)
	{
		Longi1[i]=dbuf[i];
		Lati1[i]=dbuf[i+4];
		Longi2[i]=dbuf[i+8];
		Lati2[i]=dbuf[i+12];
	}

	ldat.l=YS_CodeCalcTwoPointDisFine(Longi1,Lati1,Longi2,Lati2);
	dbuf[0]=ldat.b[AVL_LSTOR_FIR];
	dbuf[1]=ldat.b[AVL_LSTOR_SEC];
	dbuf[2]=ldat.b[AVL_LSTOR_THI];
	dbuf[3]=ldat.b[AVL_LSTOR_FOR];
	return(4);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeUTF8ToUCS2Conver
功能说明：UTF8数据转换为UCS2数据
修改记录：
-------------------------------------------------------------------------------------------*/
u8	YS_CodeUTF8ToUCS2Conver(u8 *InputBuf, u16 InputLen, u8 *OutBuf, u16 *OutBufLen)
{
	u16 pos,GetLen,OutPos;
	Avl_UWord wdat;
	u8 dat;

	pos=0;
	OutPos=0;
	*OutBufLen=0;
	while(InputLen>0)
	{
		if((InputBuf[pos]&0x80)==0x00) //占用一字节
		{
			GetLen=1;
		}
		else if((InputBuf[pos]&0xE0)==0xC0) //占用两字节
		{
			GetLen=2;
		}
		else if((InputBuf[pos]&0xF0)==0xE0) //占用3 字节
		{
			GetLen=3;
		}
		else
		{
			return(1);  //非法字符
		}

		if(InputLen<GetLen) //输入字节 不足
		{
			return(2);
		}

		wdat.w=0;
		switch(GetLen)
		{
		case 1:
			wdat.w|=InputBuf[pos];
			break;

		case 2:
			dat=InputBuf[pos];
			dat&=0x1F;
			wdat.w|=dat;
			wdat.w<<=6;
			dat=InputBuf[pos+1];
			dat&=0x3F;
			wdat.w|=dat;
			break;

		case 3:
			dat=InputBuf[pos];
			dat&=0x0F;
			wdat.w|=dat;
			wdat.w<<=6;
			dat=InputBuf[pos+1];
			dat&=0x3F;
			wdat.w|=dat;
			wdat.w<<=6;
			dat=InputBuf[pos+2];
			dat&=0x3F;
			wdat.w|=dat;
			break;

		default:
			break;
		}

		OutBuf[OutPos]=wdat.b[AVL_WSTOR_SEC];	//交换字序
		OutPos++;
		OutBuf[OutPos]=wdat.b[AVL_WSTOR_FIR];
		OutPos++;
		pos+=GetLen;
		InputLen-=GetLen;
	}

	*OutBufLen=OutPos;
	return(0);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeCreateGpsFloat
功能说明：转换位置数据格式
修改记录：
-------------------------------------------------------------------------------------------*/
void YS_CodeCreateGpsFloat(char *strdat, u8 *dbuf)
{
	char tStr[10];

	sprintf(tStr,"%d",dbuf[1]);
	if(strlen(tStr)==1)
	{
		strdat[0]='0';
		strdat[1]=tStr[0];
	}
	else
	{
		strdat[0]=tStr[0];
		strdat[1]=tStr[1];
	}

	strdat[2]='.';
	sprintf(tStr,"%d",dbuf[2]);
	if(strlen(tStr)==1)
	{
		strdat[3]='0';
		strdat[4]=tStr[0];
	}
	else
	{
		strdat[3]=tStr[0];
		strdat[4]=tStr[1];
	}

	sprintf(tStr,"%d",dbuf[3]);
	if(strlen(tStr)==1)
	{
		strdat[5]='0';
		strdat[6]=tStr[0];
	}
	else
	{
		strdat[5]=tStr[0];
		strdat[6]=tStr[1];
	}
	strdat[7]=0;

}


/*-----------------------------------------------------------------------------------------
函数名：YS_CodeTestIPString
功能说明：距离计算测试入口
修改记录：
-------------------------------------------------------------------------------------------*/
bool  YS_CodeTestIPString(char *str)
{
	u8		i,j,len,count;
	bool		IsNum;

	len=strlen(str);
	if(len==0) return (FALSE);

	for(i=0; i<len; i++)
	{
		IsNum=FALSE;
		for(j=0; j<11; j++)
		{
			if(str[i]==AVLGPS_NUM_STR_DATA[j]) IsNum=TRUE;
		}
		if(IsNum==FALSE)   return (FALSE);  //该字符为非法信息
	}
	count=0;
	for(i=0; i<len; i++)
	{
		if(str[i]=='.')
		count++;
	}

	if(count==3)
	{
		return (TRUE);
	}
	else
	{
		return (FALSE);
	}
}

u8 YS_CodeOneByteToBCD(u8 Dat)
{
    u8 dHigh,dLow;
    u8 result;

    dHigh=Dat/10;
    dLow=Dat%10;
    result=dHigh*16+dLow;
    return(result);
}

u32 YS_CodeCalcDiffSec(u16 StartYear, u8 *CurTime)
{
    u32 Result,TodaySec;
    u16 i,tYear,DiffYear;
    bool Leap;
    u8 DayNum;

    Result=0;

    tYear=2000+CurTime[0];	//历史年份秒处理
    DiffYear=tYear-StartYear;
    for(i=0; i<DiffYear; i++)
    {
        tYear=StartYear+i;
        if((tYear%4==0&&tYear%100!=0)||(tYear%400==0))
        {
            Result+=LEAP_YEAR_SEC;
        }
        else
        {
            Result+=UNLEAP_YEAR_SEC;
        }
    }

//	avlmain4_110Debug1(1,Result);
    Leap=FALSE; //当前年份秒处理

    tYear=2000+CurTime[0];
    if((((tYear%4)==0)&&((tYear%100)!=0))||(tYear%400==0))
    {
        Leap=TRUE;
    }

    if(CurTime[1]>1) //处理历史月份的秒数
    {
        for(i=0; i<(CurTime[1]-1); i++)
        {
            DayNum=Month_Days_tab[i];
            if((i==1)&&(Leap==TRUE)) //如果是2 月，且为润月
            {
                DayNum++;
            }
            Result=Result+DayNum*ONE_DAY_SEC_NUM;
        }
    }

//	avlmain4_110Debug1(2,Result);

    DayNum=CurTime[2]-1;  //处理当前月分的秒数
    Result=Result+DayNum*ONE_DAY_SEC_NUM;

//	avlmain4_110Debug1(3,Result);
    TodaySec=CurTime[3]*3600+CurTime[4]*60+CurTime[5]; //处理当天的秒数
    Result+=TodaySec;
   // Result+=101;
//	avlmain4_110Debug1(4,Result);
    return(Result);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeCrcCaculate
功能说明：crc校验
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_CodeCrcCaculate(u8 *msg, u16 msglen)
{
	//	short crc = (short) 0xFFFF;
	u16 crc;
	int i, j;
	bool c15, bit;

	crc=0xFFFF;
	for (i = 0; i < msglen; i++)
	{
		for (j = 0; j < 8; j++)
		{
			c15 = ((crc >> 15 & 1) == 1);
			bit = ((msg[i] >> (7 - j) & 1) == 1);
			crc <<= 1;
			if (c15 ^ bit)
				crc ^= 0x1021;
		}
	}
	return crc;
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeCreateGodeLink
功能说明：生成高德 link
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_CodeCreateGodeLink(u8 *PackBuf, u8 *Longitude, u8 *Latitude,u8 OnlyLink)
{
	u16 i,len,pos;
	char LongiStr[20];
	char LatiStr[20];
	//u8 DbgBuf[40]={0};

	char China_refer[] = {0xCD, 0xF8, 0xC2, 0xE7, 0xB7, 0xC3, 0xCE, 0xCA, 0xB3, 0xAC, 0xCA, 0xB1, 0xA3, 0xAC,
		0xC7, 0xEB, 0xB2, 0xCE, 0xBF, 0xBC, 0xB5, 0xD8, 0xCD, 0xBC, 0xC1, 0xB4, 0xBD, 0xD3, 0xA3, 0xBA};


	//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, China_refer, strlen(China_refer));
	pos=0;
	if(OnlyLink==0)
	{
		len = sizeof(China_refer);
		for(i=0; i<len; i++)
		{
			PackBuf[pos]=China_refer[i];
			pos++;
		}

		PackBuf[pos]=' ';
		pos++;
	}

	//sprintf((char*)DbgBuf,"the string len is %d",len);
	//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, DbgBuf, strlen((char*)DbgBuf));
	//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, PackBuf, pos);
	len=strlen(GODE_LINK_STRING);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=GODE_LINK_STRING[i];
		pos++;
	}
//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, PackBuf, strlen(PackBuf));

	YS_LongToPosStr(Longitude,LongiStr);
	YS_LongToPosStr(Latitude,LatiStr);

	len=strlen(LatiStr);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=LatiStr[i];
		pos++;
	}
//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, PackBuf, strlen(PackBuf));

	PackBuf[pos]=',';
	pos++;

	len=strlen(LongiStr);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=LongiStr[i];
		pos++;
	}

	len=strlen(GODE_LINK_END_STRING);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=GODE_LINK_END_STRING[i];
		pos++;
	}
	PackBuf[pos] = ' ';
	pos++;
	//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, PackBuf, strlen(PackBuf));

	return(pos);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeCreateGoogleLink
功能说明：生成google link
修改记录：
-------------------------------------------------------------------------------------------*/
u16 YS_CodeCreateGoogleLink(u8 *PackBuf, u8 *Longitude, u8 *Latitude,u8 OnlyLink)
{
	u16 i,len,pos;
	char LongiStr[20];
	char LatiStr[20];
	u8 China_refer[] = {0xCD, 0xF8, 0xC2, 0xE7, 0xB3, 0xAC, 0xCA, 0xB1, 0x2C, 0xC7, 0xEB, 0xB2, 0xCE, 0xBF, 0xBC};

	//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, China_refer, strlen(China_refer));
	pos=0;
	if(OnlyLink==0)
	{
		len = strlen((char*)China_refer);
		for(i=0; i<len; i++)
		{
			PackBuf[pos]=China_refer[i];
			pos++;
		}
		//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, PackBuf, strlen(PackBuf));
		PackBuf[pos]=' ';
		pos++;
	}
	len=strlen(GOOGL_LINK_STRING);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=GOOGL_LINK_STRING[i];
		pos++;
	}
//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, PackBuf, strlen(PackBuf));

	YS_LongToPosStr(Longitude,LongiStr);
	YS_LongToPosStr(Latitude,LatiStr);

	len=strlen(LatiStr);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=LatiStr[i];
		pos++;
	}
//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, PackBuf, strlen(PackBuf));

	PackBuf[pos]=',';
	pos++;

	len=strlen(LongiStr);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=LongiStr[i];
		pos++;
	}

	len=strlen(GODE_LINK_END_STRING);
	for(i=0; i<len; i++)
	{
		PackBuf[pos]=GODE_LINK_END_STRING[i];
		pos++;
	}
	PackBuf[pos] = ' ';
	pos++;
	//YS_UartDebugInterfacel(INTER_PTL_UPDATE_DEBUGINFO, PackBuf, strlen(PackBuf));

	return(pos);
}


u8  YS_Change(u8 *Dat, u8 len, u8 *OutBuf)
{
    u8 i;
    u8 Conlen=0;
    for(i=0; i<len; i++)
    {
        OutBuf[Conlen] = (Dat[i]/10)+0x30;
        Conlen++;
        OutBuf[Conlen] = (Dat[i]%10)+0x30;
        Conlen++;
    }
	OutBuf[Conlen]=0;
    return (2*len);
}


void YS_UtcTimeToTime(u32 UtcTemp,u8 *PackBuf)
{
	bool Leap=FALSE;
	u32 CurYearSec=0,CurDaySec=0,CurHourSec=0;
	u32 CurYearDay;
	u32 DayCount,DayCount1;
	u8 i,j;
	u8 dbuf[20];

	DayCount=DayCount1=0;
	//deal with year
	if((UtcTemp>=0x1A561700)&&(UtcTemp<0x1C374A80)) //2014
	{
		PackBuf[0]=14;
		Leap=FALSE;
		CurYearSec=UtcTemp-0x1A561700;
	}
	else if((UtcTemp>=0x1C374A80)&&(UtcTemp<0x1E187E00))   //2015
	{
		PackBuf[0]=15;
		Leap=FALSE;
		CurYearSec=UtcTemp-0x1C374A80;
	}
	else if((UtcTemp>=0x1E187E00)&&(UtcTemp<0x1FFB0300))  //2016
	{
		PackBuf[0]=16;
		Leap=TRUE;
		CurYearSec=UtcTemp-0x1E187E00;
	}
	else if((UtcTemp>=0x1FFB0300)&&(UtcTemp<0x21DC3680))  //2017
	{
		PackBuf[0]=17;
		Leap=FALSE;
		CurYearSec=UtcTemp-0x1FFB0300;
	}

	//deal with month
	CurYearDay=CurYearSec/ONE_DAY_SEC_NUM;
	for(i=0;i<12;i++)
	{
		if(Leap==FALSE)   //非闰年
		{
			for(j=0;j<i;j++)
			{
				DayCount+=Month_Days_tab[j];
			}
			DayCount1=DayCount+Month_Days_tab[i];
			if((CurYearDay>=DayCount)&&(CurYearDay<DayCount1))
			{
				PackBuf[1]=i+1;
				break;
			}
		}
		else
		{
			for(j=0;j<i;j++)
			{
				DayCount+=Leap_Year_Month_Days_tab[j];
			}
			DayCount1=DayCount+Leap_Year_Month_Days_tab[i];
			if((CurYearDay>=DayCount)&&(CurYearDay<DayCount1))
			{
				PackBuf[1]=i+1;
				break;
			}
		}
		DayCount=0;
		DayCount1=0;
	}
	//deal with day
	PackBuf[2]=CurYearDay-DayCount+1;

	//deal with hour
	CurDaySec=CurYearSec%ONE_DAY_SEC_NUM;
	PackBuf[3]=CurDaySec/3600;
	//deal with minute
	CurHourSec=CurDaySec%3600;
	PackBuf[4]=CurHourSec/60;
	//deal with sec
	PackBuf[5]=CurHourSec%60;

	PackBuf[6]=0;

	YS_Change(PackBuf,6,dbuf);
	//YS_UartDebugString((char*)dbuf);
}


/*-----------------------------------------------------------------------------------------
函数名：YS_CodeCalcTwoPointDisFine
功能说明：将角度转换为弧度
修改记录：
-------------------------------------------------------------------------------------------*/
double YS_CodeConverPosToRadian(u8 *dbuf)
{
	char strdat[10];
	double f1,f2;

	YS_CodeCreateGpsFloat(strdat, dbuf);
	f2=atof(strdat);
	f1=dbuf[0]*60+f2;
	f1/=60;
	f1=(DIS_CALC_PI*f1)/180;
	return(f1);
}

/*-----------------------------------------------------------------------------------------
函数名：YS_CodeCalcTwoPointDisFine
功能说明：精确计算两点间的距离
修改记录：
-------------------------------------------------------------------------------------------*/
u32 YS_CodeCalcTwoPointDisFine(u8 *Longi1, u8 *Lati1, u8 *Longi2, u8 *Lati2)
{
	u32 result;
	double f,flong1,flong2,flati1,flati2,fresult;
	double f1,f2,f3;
	flong1=YS_CodeConverPosToRadian(Longi1);
	flong2=YS_CodeConverPosToRadian(Longi2);
	flati1=YS_CodeConverPosToRadian(Lati1);
	flati2=YS_CodeConverPosToRadian(Lati2);
	f=sin(flati1)*sin(flati2)+cos(flati1)*cos(flati2)*cos(flong1-flong2);
	f1=sin(flati1)*sin(flati2);
	f2=cos(flati1)*cos(flati2);
	f3=cos(flong1-flong2)*f2;
	f=f1+f3;
	fresult=DIS_CALC_EARTH_R*acos(f);
	result=(u32)fresult;
	return (result);
}

bool  YS_CodePDNumString2(char *str)
{
	u8		i,j,len;
	bool		IsNum;

	len=strlen(str);
	if(len==0) return (FALSE);

	for(i=0; i<len; i++)
	{
		IsNum=FALSE;
		for(j=0; j<10; j++)
		{
			if(str[i]==AVLGPS_NUM_STR_DATA[j]) IsNum=TRUE;
		}
		if(IsNum==FALSE)
			return (FALSE);  //该字符为非法信息
	}
	return(TRUE);
}


