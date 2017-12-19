/*实现队列的相关功能*/
#include "DataStru.h"

/*判断队列是否为空*/
int QueIsEmpty(Quence *QuenceBuf)
{
	if(QuenceBuf->nSize > 0) return 0;
	else return 1;	
}

/*判断队列是否已满*/
int QueIsFull(Quence *QuenceBuf)
{
	if(QuenceBuf->nSize<1000) return 0;
	else return 1;	
}

/*队列头入队列*/
int InQuence(Quence *QuenceBuf, int nData)
{
	int i;
	if(QueIsFull(QuenceBuf) || QuenceBuf->nMaxSize <= 0) return -1;
	for(i=QuenceBuf->nSize; i>=1; i--)
	{
		QuenceBuf->nData[i+1]=QuenceBuf->nData[i];	
	}
	QuenceBuf->nData[1]=nData;
	QuenceBuf->nSize++;
	return 0;
}

/*删除队列尾第一个数据*/
int OutQuence(Quence *QuenceBuf)
{
	if(QueIsEmpty(QuenceBuf)) return -1;
	QuenceBuf->nSize--;	
	return 0;
}

/*返回队列末尾的元素*/
int QueLastElement(Quence *QuenceBuf)
{
	return QuenceBuf->nData[QuenceBuf->nSize];	
}

/*打印队列内容*/
int PrintQuence(Quence *QuenceBuf)
{
	int i;
	for(i=1; i<=QuenceBuf->nSize; i++)
	{
		printf("%d ", QuenceBuf->nData[i]);	
	}
	
	printf("\n");
	
	return 0;
}


/*初始化队列*/
int InitQuence(Quence *QuenceBuf, int nMaxSize)
{
	if(nMaxSize <= 0) return -1;
	QuenceBuf->nMaxSize = nMaxSize;
	QuenceBuf->nData = malloc(nMaxSize*sizeof(int));		
	memset(QuenceBuf->nData, 0, nMaxSize*sizeof(int));
	QuenceBuf->nSize = 0;
	return 0;		
}

/*释放队列*/
void FreeQuence(Quence *QuenceBuf)
{
	free(QuenceBuf->nData);
	QuenceBuf->nSize = 0;
	QuenceBuf->nMaxSize = 0;	
	QuenceBuf->nData = NULL;
}
