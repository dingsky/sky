/*****************************************************************************
*功能：实现分隔符报文的解析，其中分隔符报文可以是字符fmt,也可以是字符串
*作者：丁春
*时间：2013-09-16
*说明：a|b|c |a|b|c |a|b|c| a###b###c ###a###b####c ###a###b###c###  这样的组合报文都是可以处理的
*****************************************************************************/
#include "public.h"
#include "Msg.h"
#define DEBUG 1

/*
2015-11-09 GetRegionIntoArrayByStr改为返回指针数组
*/


/*转换字符串的格式，保持收尾都有分隔符*/
void TranStringFmt( char *sStringInput, char *sStringOutput, char *sStr )
{	
	if(memcmp(sStringInput, sStr, strlen(sStr)) == 0)	strcpy(sStringOutput, sStringInput);
	else	sprintf(sStringOutput,"%s%s", sStr, sStringInput);
		
	if(memcmp(sStringInput+strlen(sStringInput)-strlen(sStr), sStr, strlen(sStr)) != 0) strcat(sStringOutput, sStr);
}


/*获得字符串中域的个数,以sStr为分隔符*/
/*当分隔符为单字节的时候, strchr和strstr的效率很接近，顾统一采用strstr*/
int GetRegionNumByStr( char *sString, char *sStr )
{
	char *sPosition = sString; char *sStrPos   = NULL;
	int nNum = 0;

	/*过滤行尾换行*/	
	if( sString[strlen(sString)-1] == '\n' )  sString[strlen(sString)-1] = '\0';
		
	/*循环计数*/
	while( (sStrPos = strstr(sPosition, sStr)) != NULL )
	{
		nNum++;
		sPosition = sStrPos + strlen(sStr);
	}		
	return --nNum;
}

/*获得第nCount域的内容,以sStr为分隔符，将获得的内容填入sStringOutput中*/
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
	
	/*转换字符串格式*/
	TranStringFmt(sStringInput, sContentBuf, sStr);
	sPosition = sContentBuf;
	
	nTotalRegion = GetRegionNumByStr( sContentBuf, sStr );
	if( (nCount < 1) || (nCount > nTotalRegion) ) 
	{
		free(sContentBuf);		/*add by dc 20150109*/
		return OUT_OF_REGION;
	}

/*获得第nCount-1个分隔符的位置,开头和结尾的忽略*/
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
	
	/*释放资源*/
	free(sContentBuf);
	
	return 0;
	
}  

/*解析报文存入指定的字符串数组, 以sStr为分隔符*/
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
	
	/*转换字符串格式*/
	TranStringFmt(sStringInput, sContentBuf, sStr);
	sPosition = sContentBuf;
	
	/*获得域总数，并判断数组容量是否足够*/
	nTotalRegion = GetRegionNumByStr( sContentBuf, sStr );
    sArray = (char **)malloc(sizeof(char *)*nTotalRegion);
    if(sArray == NULL)
    {
        PrintLog(ERROR, "malloc error[%d] errmsg[%s]", errno, strerror(errno));
        free(sContentBuf);
        return NULL;    
    }
	
	#if DEBUG
		/*printf("域总数为[%d]\n", nTotalRegion);*//*delete by dc 20150109*/
		PrintLog(TRACE, "域总数为[%d]\n", nTotalRegion);/*add by dc 20150109*/
	#endif

	/*循环获得各个域*/
	for(i=0; i<nTotalRegion; i++)	
	{
	
		/*获得第i域的起始位置和结束位置*/
		sStrPos = strstr(sPosition, sStr);
		sStart = sStrPos + strlen(sStr);
		sPosition = sStart;
		sEnd = strstr(sPosition, sStr);
		sPosition = sEnd;	/*前一个的域的结尾作为后一个域的起始*/
		
		/*从起始位置开始到结束位置截取第i域的value*/
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
	
	/*释放资源*/
	free(sContentBuf);
	
	return sArray;
}

/*释放动态内存*/
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

/*显示各个域的内容*/
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

