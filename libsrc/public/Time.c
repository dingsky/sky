/*
功能：包含时间相关的通用函数
作者：丁春
时间：2015年11月2日
*/
#include "public.h"

/*
*函数名：GetDateTime   
*功能：  获取日期和时间的基础函数
*入参：  sSeparator1~sSeparator7分隔符 yyyy(sSeparator1)mm(sSeparator2)dd(sSeparator3)(sSeparator4)HH(sSeparator5)MM(sSeparator6)SS(sSeparator7)
*        snFlag-> TIMEONLY仅获取时间 DATEONLY仅获取日期 DATETIME获取日期和时间
         CHNWEEKDAY->获取中文星期几 ENGWEEKDAY->获取英文星期几
*出参：sString->输出串  
*说明:
*/
void GetDateTime(char *sString, char *sSeparator1, char *sSeparator2, char *sSeparator3, char *sSeparator4, char *sSeparator5, char *sSeparator6, char *sSeparator7, int inNum, short snFlag)
{
    struct tm *TmBuf = NULL;
    time_t inTime;
    
    if(sString == NULL) return;
    
    inTime = time(NULL);
    inTime += inNum*24*60*60;
    TmBuf = localtime(&inTime);
    if(TmBuf == NULL) return;
        
    switch(snFlag)
    {
        case TIMEONLY:
        		if(sSeparator5 == NULL || sSeparator6 == NULL  || sSeparator7 == NULL) return;
            sprintf(sString, "%02d%s%02d%s%02d%s", TmBuf->tm_hour, sSeparator5, TmBuf->tm_min, sSeparator6, TmBuf->tm_sec, sSeparator7);
            break;
        case DATEONLY:
        		if(sSeparator1 == NULL || sSeparator2 == NULL || sSeparator3 == NULL) return;
            sprintf(sString, "%04d%s%02d%s%02d%s", TmBuf->tm_year+1900, sSeparator1, TmBuf->tm_mon+1, sSeparator2, TmBuf->tm_mday, sSeparator3);            
            break;
        case DATETIME:
        		if(sSeparator1 == NULL || sSeparator2 == NULL || sSeparator3 == NULL 
        			 || sSeparator4 == NULL || sSeparator5 == NULL || sSeparator6 == NULL || sSeparator7 == NULL) return;
            sprintf(sString, "%04d%s%02d%s%02d%s%s%02d%s%02d%s%02d%s", TmBuf->tm_year+1900, sSeparator1, TmBuf->tm_mon+1, sSeparator2, TmBuf->tm_mday,sSeparator3, sSeparator4, \
            TmBuf->tm_hour, sSeparator5, TmBuf->tm_min, sSeparator6, TmBuf->tm_sec, sSeparator7);                        
            break;
        case ENGWEEKDAY:
        		switch(TmBuf->tm_wday)
        		{
        			case 0:
        				sprintf(sString, "%s", "Monday");		
        				break;
        			case 1:
        				sprintf(sString, "%s", "Tuesday");		
        				break;
        			case 2:
        				sprintf(sString, "%s", "Wednesday");		
        				break;
        			case 3:
        				sprintf(sString, "%s", "Thurday");		
        				break;
        			case 4:
        				sprintf(sString, "%s", "Friday");		
        				break;
        			case 5:
        				sprintf(sString, "%s", "Saturday");		
        				break;
        			case 6:
        				sprintf(sString, "%s", "Sunday");		
        				break;	
        		  default:
        		  	break;
        		}
       			break;
         case CHNWEEKDAY:
        		switch(TmBuf->tm_wday)
        		{
        			case 0:
        				sprintf(sString, "%s", "星期一");		
        				break;
        			case 1:
        				sprintf(sString, "%s", "星期二");		
        				break;
        			case 2:
        				sprintf(sString, "%s", "星期三");		
        				break;
        			case 3:
        				sprintf(sString, "%s", "星期四");		
        				break;
        			case 4:
        				sprintf(sString, "%s", "星期五");		
        				break;
        			case 5:
        				sprintf(sString, "%s", "星期六");		
        				break;
        			case 6:
        				sprintf(sString, "%s", "星期日");		
        				break;	
        			default:
        				break;
        		}
       			break;
        default:
            break;    
    }    
   
    return;            
}

/*
*函数名：GetToday
*功能：  获取当前日期
*出参： sString->输出串  
*说明:
*/
void GetToday(char *sString)
{
    GetDateTime(sString, "", "", "", NULL, NULL, NULL, NULL, 0, DATEONLY);
    return;    
}

/*
*函数名：GetTomorrow
*功能：  获取明天的日期
*出参： sString->输出串  
*说明:
*/
void GetTomorrow(char *sString)
{
    GetDateTime(sString, "", "", "", NULL, NULL, NULL, NULL, 1, DATEONLY);
    return;    
}

/*
*函数名：GetYesterday
*功能：  获取昨天的日期
*出参： sString->输出串  
*说明:
*/
void GetYesterday(char *sString)
{
    GetDateTime(sString, "", "", "", NULL, NULL, NULL, NULL, -1, DATEONLY);
    return;    
}

/*
*函数名：GetSomeDaysAgo
*功能：  获取若干天前的日期
*出参： sString->输出串  
*说明:
*/
void GetSomeDaysAgo(char *sString, int inNum)
{
    GetDateTime(sString, "", "", "", NULL, NULL, NULL, NULL, inNum*(-1), DATEONLY);
    return;    
}

/*
*函数名：GetSomeDaysLater
*功能：  获取若干天后的日期
*出参： sString->输出串  
*说明:
*/
void GetSomeDaysLater(char *sString, int inNum)
{
    GetDateTime(sString, "", "", "", NULL, NULL, NULL, NULL, inNum, DATEONLY);
    return;    
}

/*
*函数名：GetNowDateTime
*功能：  获取当前的日期+时间
*出参： sString->输出串  
*说明:
*/
void GetNowDateTime(char *sString)
{
    GetDateTime(sString, "", "","", "", "", "", "", 0, DATETIME);
    return;    
}

/*
*函数名：GetNowTime
*功能：  获取当前的时间
*出参： sString->输出串  
*说明:
*/
void GetNowTime(char *sString)
{
    GetDateTime(sString, NULL, NULL, NULL, NULL, "", "", "", 0, TIMEONLY);
    return;    
}

/*
*函数名：GetNowDateTimeFormat1
*功能：  获取当前的时间, 格式为 yyyy年mm月dd日 HH时MM分SS秒
*出参： sString->输出串  
*说明:
*/
void GetNowDateTimeFormat1(char *sString)
{
    GetDateTime(sString, "年", "月", "日", " ", "时", "分", "秒", 0, DATETIME);
    return;    	
}

/*
*函数名：GetNowDateTimeFormat2
*功能：  获取当前的时间, 格式为 yyyy-mm-dd HH:MM:SS
*出参： sString->输出串  
*说明:
*/
void GetNowDateTimeFormat2(char *sString)
{
    GetDateTime(sString, "-", "-", "", " ", ":", ":", "", 0, DATETIME);
    return;    	
}


/*
*函数名：GetNowTimeFormat1
*功能：  获取当前的时间 格式为HH时MM分SS秒
*出参： sString->输出串  
*说明:
*/
void GetNowTimeFormat1(char *sString)
{
    GetDateTime(sString, NULL, NULL, NULL, NULL, "时", "分", "秒", 0, TIMEONLY);
    return;    
}

/*
*函数名：GetNowTimeFormat2
*功能：  获取当前的时间 格式为HH:MM:SS
*出参： sString->输出串  
*说明:
*/
void GetNowTimeFormat2(char *sString)
{
    GetDateTime(sString, NULL, NULL, NULL, NULL, ":", ":", "", 0, TIMEONLY);
    return;    
}

/*
*函数名：GetChnWeekDay
*功能：  获取中文星期几
*出参： sString->输出串  
*说明:
*/
void GetChnWeekDay(char *sString)
{
    GetDateTime(sString, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, CHNWEEKDAY);
    return;    
}

/*
*函数名：GetEngWeekDay
*功能：  获取英文星期几
*出参： sString->输出串  
*说明:
*/
void GetEngWeekDay(char *sString)
{
    GetDateTime(sString, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, ENGWEEKDAY);
    return;    
}

