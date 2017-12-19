/*ʵ�ֶ��е���ع���*/
#include "DataStru.h"

/*�ж϶����Ƿ�Ϊ��*/
int QueIsEmpty(Quence *QuenceBuf)
{
	if(QuenceBuf->nSize > 0) return 0;
	else return 1;	
}

/*�ж϶����Ƿ�����*/
int QueIsFull(Quence *QuenceBuf)
{
	if(QuenceBuf->nSize<1000) return 0;
	else return 1;	
}

/*����ͷ�����*/
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

/*ɾ������β��һ������*/
int OutQuence(Quence *QuenceBuf)
{
	if(QueIsEmpty(QuenceBuf)) return -1;
	QuenceBuf->nSize--;	
	return 0;
}

/*���ض���ĩβ��Ԫ��*/
int QueLastElement(Quence *QuenceBuf)
{
	return QuenceBuf->nData[QuenceBuf->nSize];	
}

/*��ӡ��������*/
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


/*��ʼ������*/
int InitQuence(Quence *QuenceBuf, int nMaxSize)
{
	if(nMaxSize <= 0) return -1;
	QuenceBuf->nMaxSize = nMaxSize;
	QuenceBuf->nData = malloc(nMaxSize*sizeof(int));		
	memset(QuenceBuf->nData, 0, nMaxSize*sizeof(int));
	QuenceBuf->nSize = 0;
	return 0;		
}

/*�ͷŶ���*/
void FreeQuence(Quence *QuenceBuf)
{
	free(QuenceBuf->nData);
	QuenceBuf->nSize = 0;
	QuenceBuf->nMaxSize = 0;	
	QuenceBuf->nData = NULL;
}
