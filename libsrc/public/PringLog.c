/*************************************************************************************
*���ܣ�ʵ����־��ӡ����
*���ߣ�����
*ʱ�䣺2014-06-30
*�汾��v1.0
*************************************************************************************/
#include	"public.h"
#include    <stdarg.h>



/*��ӡ��־*/
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

	/*���ݲ�ͬ����־���͸���־�ļ���Ӻ�׺*/
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
		
	/*��ʼ��args,ʹ��argsָ���һ������*/
	va_start(args, sFmt);
	
	/*����ǰʱ�䡢ԭ�ļ�����Դ�ļ������е���Ϣ׷�ӵ���־������*/
	GetNowTimeFormat2(sNowTime);
	sprintf(sLogBuf, "%s %s(%d)", sNowTime, sSourceFile, nLine);	
	
	/*��ȡ��Ҫ��ӡ������*/
	vsprintf(sLogBuf+strlen(sLogBuf), sFmt, args);
	
	/*׷�ӻ��з�*/
	strcat(sLogBuf, "\n");
	
	/*����־�ļ�*/
	sprintf(sLogFilePath, "%s/log/%s", getenv("DCDIR"), sFullLogFileName);
	fp = fopen(sLogFilePath, "a+");
	if(fp == NULL) return;
		
	/*������д����־�ļ���*/
	fwrite(sLogBuf, strlen(sLogBuf),1, fp);
	
	/*�ر���־�ļ�*/ 
	fclose(fp);
	
	/*�ر�args*/
	va_end(args);

	return;
}

/*
*���ܣ���ӡ16���Ʊ���
*��Σ�sString->��Ҫ��ӡ��16���ƴ�
       nLen -> ������
*���ߣ�����
*ʱ�䣺2015��11��17��
*/
void PrintHexDebug(unsigned char *sString, short nLen)
{
    short nLix;
    char sNowTime[30], sLogFilePath[200+1], sNowDate[20];
    FILE *fp = NULL;
    
    if(sString == NULL || nLen <= 0) return;
        
    //��ȡ��ǰʱ�������
    GetNowDateTimeFormat2(sNowTime);
    GetToday(sNowDate);
    
    //���ļ�
    sprintf(sLogFilePath, "%s/log/%s_%s_debug.log", getenv("DCDIR"), sLogFileName, sNowDate);
    fp = fopen(sLogFilePath, "a+");
    if(fp == NULL)
    {
        printf("open file[%s] err[%d] errmsg[%s]\n", sLogFilePath, errno, strerror(errno));    
        return;
    }
    
    //д����ʱ��ͱ��ĳ���
    fprintf(fp, "time[%s] msglen[%d]\n", sNowTime, nLen); 
    
    //д16���Ʊ���
    for(nLix=0; nLix<nLen; nLix++)
    {
        fprintf(fp, "[%02x]", sString[nLix]);
        if((nLix+1)%20 == 0 || nLix == nLen-1)  //ÿ��ӡ20���ֽڻ��ߴ�ӡ���׷��һ�����з�
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
