/*****************************************************************************
*���ܣ�ʵ�ַָ������ĵĽ��������зָ������Ŀ������ַ�fmt,Ҳ�������ַ���
*���ߣ�����
*ʱ�䣺2013-09-16
*˵����a|b|c |a|b|c |a|b|c| a###b###c ###a###b####c ###a###b###c###  ��������ϱ��Ķ��ǿ��Դ����
*****************************************************************************/
#include "public.h"
#include "Msg.h"
#define DEBUG 1

/*
2015-11-09 GetRegionIntoArrayByStr��Ϊ����ָ������
*/


/*ת���ַ����ĸ�ʽ��������β���зָ���*/
void TranStringFmt( char *sStringInput, char *sStringOutput, char *sStr )
{	
	if(memcmp(sStringInput, sStr, strlen(sStr)) == 0)	strcpy(sStringOutput, sStringInput);
	else	sprintf(sStringOutput,"%s%s", sStr, sStringInput);
		
	if(memcmp(sStringInput+strlen(sStringInput)-strlen(sStr), sStr, strlen(sStr)) != 0) strcat(sStringOutput, sStr);
}


/*����ַ�������ĸ���,��sStrΪ�ָ���*/
/*���ָ���Ϊ���ֽڵ�ʱ��, strchr��strstr��Ч�ʺܽӽ�����ͳһ����strstr*/
int GetRegionNumByStr( char *sString, char *sStr )
{
	char *sPosition = sString; char *sStrPos   = NULL;
	int nNum = 0;

	/*������β����*/	
	if( sString[strlen(sString)-1] == '\n' )  sString[strlen(sString)-1] = '\0';
		
	/*ѭ������*/
	while( (sStrPos = strstr(sPosition, sStr)) != NULL )
	{
		nNum++;
		sPosition = sStrPos + strlen(sStr);
	}		
	return --nNum;
}

/*��õ�nCount�������,��sStrΪ�ָ���������õ���������sStringOutput��*/
int GetCountRegionByStr( char *sStringInput, char *sStr, int nCount, char *sStringOutput )
{
	int i , nNum   = 0, nTotalRegion = 0;
	char *sStart   = NULL;
	char *sEnd     = NULL;
	char *sStrPos  = NULL;
	char *sPosition = sStringInput;
	char *sContentBuf = malloc(strlen(sStringInput)+strlen(sStr)*2+1);
	memset(sContentBuf, 0, strlen(sStringInput)+strlen(sStr)*2+1);
	
	if( sStringInput == NULL ) 
	{
		free(sContentBuf);		/*add by dc 20150109*/
		return -1;
	}
	
	/*ת���ַ�����ʽ*/
	TranStringFmt(sStringInput, sContentBuf, sStr);
	sPosition = sContentBuf;
	
	nTotalRegion = GetRegionNumByStr( sContentBuf, sStr );
	if( (nCount < 1) || (nCount > nTotalRegion) ) 
	{
		free(sContentBuf);		/*add by dc 20150109*/
		return OUT_OF_REGION;
	}

/*��õ�nCount-1���ָ�����λ��,��ͷ�ͽ�β�ĺ���*/
	while( (sStrPos=strstr(sPosition,sStr)) != NULL )
	{
		nNum++;
		if( nNum == nCount ) break;
		sPosition = sStrPos + strlen(sStr);
	}
	sStart = sStrPos + strlen(sStr);
//	printf("count[%d] start[%p] sStrPos[%p]\n", nCount, sStart, sStrPos);

	sEnd = strstr( sStart, sStr );
//	printf("count[%d] start[%p] sStrPos[%p] end[%p]\n", nCount, sStart, sStrPos, sEnd);
	memcpy( sStringOutput, sStart, sEnd-sStart );
	
	/*�ͷ���Դ*/
	free(sContentBuf);
	
	return 0;
	
}  

/*�������Ĵ���ָ�����ַ�������, ��sStrΪ�ָ���*/
char ** GetRegionIntoArrayByStr(char *sStringInput, char *sStr)
{
	int nTotalRegion = 0, i=0;
	char *sStart = NULL, *sEnd = NULL, *sPosition = NULL, *sStrPos = NULL;
	char *sContentBuf = malloc(strlen(sStringInput)+strlen(sStr)*2+1);
	memset(sContentBuf, 0, strlen(sStringInput)+strlen(sStr)*2+1);
	char **sArray = NULL;
	
	if(sStringInput == NULL) 
	{
		free(sContentBuf);		/*add by dc 20150109*/
		return NULL;
	}
	
	/*ת���ַ�����ʽ*/
	TranStringFmt(sStringInput, sContentBuf, sStr);
	sPosition = sContentBuf;
	
	/*��������������ж����������Ƿ��㹻*/
	nTotalRegion = GetRegionNumByStr( sContentBuf, sStr );
    sArray = (char **)malloc(sizeof(char *)*nTotalRegion);
    if(sArray == NULL)
    {
        PrintLog(ERROR, "malloc error[%d] errmsg[%s]", errno, strerror(errno));
        free(sContentBuf);
        return NULL;    
    }
	
	#if DEBUG
		/*printf("������Ϊ[%d]\n", nTotalRegion);*//*delete by dc 20150109*/
		PrintLog(TRACE, "������Ϊ[%d]\n", nTotalRegion);/*add by dc 20150109*/
	#endif

	/*ѭ����ø�����*/
	for(i=0; i<nTotalRegion; i++)	
	{
	
		/*��õ�i�����ʼλ�úͽ���λ��*/
		sStrPos = strstr(sPosition, sStr);
		sStart = sStrPos + strlen(sStr);
		sPosition = sStart;
		sEnd = strstr(sPosition, sStr);
		sPosition = sEnd;	/*ǰһ������Ľ�β��Ϊ��һ�������ʼ*/
		
		/*����ʼλ�ÿ�ʼ������λ�ý�ȡ��i���value*/
		sArray[i] = malloc(sizeof(sEnd-sStart)+1);
		if(sArray[i] == NULL)
		{
            PrintLog(ERROR, "malloc sArray[%d error[%d] ] errmsg[%s]", i, errno, strerror(errno));		    
		    break;    
		}
		memcpy(sArray[i], sStart, sEnd-sStart);
		sArray[i][sEnd-sStart] = 0;
	}
	
	sArray[i] = NULL;
	
	/*�ͷ���Դ*/
	free(sContentBuf);
	
	return sArray;
}

/*�ͷŶ�̬�ڴ�*/
void FreeArray(char **sArray)
{
    int inLix = 0;
    if(sArray == NULL) return;
        
    while(sArray[inLix] != NULL)
    {
        free(sArray[inLix]);
        inLix++;    
    }  
    free(sArray);  
}

/*��ʾ�����������*/
void ShowData( char **sString )
{
	int i = 0;
	
	if(sString == NULL) return;
	    
	while(1)
	{
	    if(sString[i] == NULL) break;
		printf("the[%d]field content is %s\n",i+1, sString[i] );
		i++;
	}
}

