/*************************************************************************************
*功能：实现日志打印函数
*作者：丁春
*时间：2014-06-30
*版本：v1.0
*************************************************************************************/
#include	"public.h"
#include    <stdarg.h>



/*打印日志*/
void PrintLog(char *sSourceFile, int nLine, int nType, char *sFmt, ...)
{
	FILE *fp = NULL;
	char sLogBuf[LOG_BUF_MAX_LEN+1], sLogFilePath[200+1], sNowDate[20], sNowTime[20], sFullLogFileName[60+1];
	va_list args;
	memset(sLogBuf, 0, sizeof(sLogBuf));
	memset(sLogFilePath, 0, sizeof(sLogFilePath));
	memset(sNowDate, 0, sizeof(sNowDate));
	memset(sNowTime, 0, sizeof(sNowTime));
	memset(sFullLogFileName, 0, sizeof(sFullLogFileName));

	/*根据不同的日志类型给日志文件添加后缀*/
	GetToday(sNowDate);
	switch(nType)
	{
		case LOG_ERR:
				sprintf(sFullLogFileName, "%s_%s_err.log", sLogFileName, sNowDate);
				break;
		case LOG_TRACE:
				sprintf(sFullLogFileName, "%s_%s_trace.log", sLogFileName, sNowDate);
				break;
		case LOG_WARN:
				sprintf(sFullLogFileName, "%s_%s_warn.log", sLogFileName, sNowDate);
				break;
		default:
				break;
	}
		
	/*初始化args,使得args指向第一个参数*/
	va_start(args, sFmt);
	
	/*将当前时间、原文件名、源文件所在行等信息追加到日志内容中*/
	GetNowTimeFormat2(sNowTime);
	sprintf(sLogBuf, "%s %s(%d)", sNowTime, sSourceFile, nLine);	
	
	/*获取需要打印的内容*/
	vsprintf(sLogBuf+strlen(sLogBuf), sFmt, args);
	
	/*追加换行符*/
	strcat(sLogBuf, "\n");
	
	/*打开日志文件*/
	sprintf(sLogFilePath, "%s/log/%s", getenv("DCDIR"), sFullLogFileName);
	fp = fopen(sLogFilePath, "a+");
	if(fp == NULL) return;
		
	/*将内容写到日志文件中*/
	fwrite(sLogBuf, strlen(sLogBuf),1, fp);
	
	/*关闭日志文件*/ 
	fclose(fp);
	
	/*关闭args*/
	va_end(args);

	return;
}

/*
*功能：打印16进制报文
*入参：sString->需要打印的16进制串
       nLen -> 串长度
*作者：丁春
*时间：2015年11月17日
*/
void PrintHexDebug(unsigned char *sString, short nLen)
{
    short nLix;
    char sNowTime[30], sLogFilePath[200+1], sNowDate[20];
    FILE *fp = NULL;
    
    if(sString == NULL || nLen <= 0) return;
        
    //获取当前时间和日期
    GetNowDateTimeFormat2(sNowTime);
    GetToday(sNowDate);
    
    //打开文件
    sprintf(sLogFilePath, "%s/log/%s_%s_debug.log", getenv("DCDIR"), sLogFileName, sNowDate);
    fp = fopen(sLogFilePath, "a+");
    if(fp == NULL)
    {
        printf("open file[%s] err[%d] errmsg[%s]\n", sLogFilePath, errno, strerror(errno));    
        return;
    }
    
    //写交易时间和报文长度
    fprintf(fp, "time[%s] msglen[%d]\n", sNowTime, nLen); 
    
    //写16进制报文
    for(nLix=0; nLix<nLen; nLix++)
    {
        fprintf(fp, "[%02x]", sString[nLix]);
        if((nLix+1)%20 == 0 || nLix == nLen-1)  //每打印20个字节或者打印完毕追加一个换行符
        {
            fprintf(fp, "\n");    
        }    
        else
        {
            fprintf(fp, " ");    
        }
    }
    
    fclose(fp);
    return;     
}
