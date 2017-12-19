/*
���ܣ�����ʱ����ص�ͨ�ú���
���ߣ�����
ʱ�䣺2015��11��2��
*/
#include "public.h"

/*
*��������GetDateTime   
*���ܣ�  ��ȡ���ں�ʱ��Ļ�������
*��Σ�  sSeparator1~sSeparator7�ָ��� yyyy(sSeparator1)mm(sSeparator2)dd(sSeparator3)(sSeparator4)HH(sSeparator5)MM(sSeparator6)SS(sSeparator7)
*        snFlag-> TIMEONLY����ȡʱ�� DATEONLY����ȡ���� DATETIME��ȡ���ں�ʱ��
         CHNWEEKDAY->��ȡ�������ڼ� ENGWEEKDAY->��ȡӢ�����ڼ�
*���Σ�sString->�����  
*˵��:
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
        				sprintf(sString, "%s", "����һ");		
        				break;
        			case 1:
        				sprintf(sString, "%s", "���ڶ�");		
        				break;
        			case 2:
        				sprintf(sString, "%s", "������");		
        				break;
        			case 3:
        				sprintf(sString, "%s", "������");		
        				break;
        			case 4:
        				sprintf(sString, "%s", "������");		
        				break;
        			case 5:
        				sprintf(sString, "%s", "������");		
        				break;
        			case 6:
        				sprintf(sString, "%s", "������");		
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
*��������GetToday
*���ܣ�  ��ȡ��ǰ����
*���Σ� sString->�����  
*˵��:
*/
void GetToday(char *sString)
{
    GetDateTime(sString, "", "", "", NULL, NULL, NULL, NULL, 0, DATEONLY);
    return;    
}

/*
*��������GetTomorrow
*���ܣ�  ��ȡ���������
*���Σ� sString->�����  
*˵��:
*/
void GetTomorrow(char *sString)
{
    GetDateTime(sString, "", "", "", NULL, NULL, NULL, NULL, 1, DATEONLY);
    return;    
}

/*
*��������GetYesterday
*���ܣ�  ��ȡ���������
*���Σ� sString->�����  
*˵��:
*/
void GetYesterday(char *sString)
{
    GetDateTime(sString, "", "", "", NULL, NULL, NULL, NULL, -1, DATEONLY);
    return;    
}

/*
*��������GetSomeDaysAgo
*���ܣ�  ��ȡ������ǰ������
*���Σ� sString->�����  
*˵��:
*/
void GetSomeDaysAgo(char *sString, int inNum)
{
    GetDateTime(sString, "", "", "", NULL, NULL, NULL, NULL, inNum*(-1), DATEONLY);
    return;    
}

/*
*��������GetSomeDaysLater
*���ܣ�  ��ȡ������������
*���Σ� sString->�����  
*˵��:
*/
void GetSomeDaysLater(char *sString, int inNum)
{
    GetDateTime(sString, "", "", "", NULL, NULL, NULL, NULL, inNum, DATEONLY);
    return;    
}

/*
*��������GetNowDateTime
*���ܣ�  ��ȡ��ǰ������+ʱ��
*���Σ� sString->�����  
*˵��:
*/
void GetNowDateTime(char *sString)
{
    GetDateTime(sString, "", "","", "", "", "", "", 0, DATETIME);
    return;    
}

/*
*��������GetNowTime
*���ܣ�  ��ȡ��ǰ��ʱ��
*���Σ� sString->�����  
*˵��:
*/
void GetNowTime(char *sString)
{
    GetDateTime(sString, NULL, NULL, NULL, NULL, "", "", "", 0, TIMEONLY);
    return;    
}

/*
*��������GetNowDateTimeFormat1
*���ܣ�  ��ȡ��ǰ��ʱ��, ��ʽΪ yyyy��mm��dd�� HHʱMM��SS��
*���Σ� sString->�����  
*˵��:
*/
void GetNowDateTimeFormat1(char *sString)
{
    GetDateTime(sString, "��", "��", "��", " ", "ʱ", "��", "��", 0, DATETIME);
    return;    	
}

/*
*��������GetNowDateTimeFormat2
*���ܣ�  ��ȡ��ǰ��ʱ��, ��ʽΪ yyyy-mm-dd HH:MM:SS
*���Σ� sString->�����  
*˵��:
*/
void GetNowDateTimeFormat2(char *sString)
{
    GetDateTime(sString, "-", "-", "", " ", ":", ":", "", 0, DATETIME);
    return;    	
}


/*
*��������GetNowTimeFormat1
*���ܣ�  ��ȡ��ǰ��ʱ�� ��ʽΪHHʱMM��SS��
*���Σ� sString->�����  
*˵��:
*/
void GetNowTimeFormat1(char *sString)
{
    GetDateTime(sString, NULL, NULL, NULL, NULL, "ʱ", "��", "��", 0, TIMEONLY);
    return;    
}

/*
*��������GetNowTimeFormat2
*���ܣ�  ��ȡ��ǰ��ʱ�� ��ʽΪHH:MM:SS
*���Σ� sString->�����  
*˵��:
*/
void GetNowTimeFormat2(char *sString)
{
    GetDateTime(sString, NULL, NULL, NULL, NULL, ":", ":", "", 0, TIMEONLY);
    return;    
}

/*
*��������GetChnWeekDay
*���ܣ�  ��ȡ�������ڼ�
*���Σ� sString->�����  
*˵��:
*/
void GetChnWeekDay(char *sString)
{
    GetDateTime(sString, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, CHNWEEKDAY);
    return;    
}

/*
*��������GetEngWeekDay
*���ܣ�  ��ȡӢ�����ڼ�
*���Σ� sString->�����  
*˵��:
*/
void GetEngWeekDay(char *sString)
{
    GetDateTime(sString, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, ENGWEEKDAY);
    return;    
}

