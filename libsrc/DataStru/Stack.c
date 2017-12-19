/*ʵ��ջ����ع���*/
/***********************************************************************************
*�޸�����:
*20150112 ����IsFull��������ֹջ�����
					����InitStack������ʼ��ջ
***********************************************************************************/

#include "DataStru.h"

/*����ջ��Ԫ��*/
int Top(Stack *StackBuf)
{
	return StackBuf->nData[StackBuf->nSize];
	
}

/*����ջ��Ԫ��*/
int Pop(Stack *StackBuf)
{
	if(IsEmpty(StackBuf)) return -1;
	StackBuf->nSize--;
	return 0;
	//return StackBuf->nData[StackBuf->nSize--];
}

/*��ջ*/
int Push(Stack *StackBuf, int nNum)
{
	if(IsFull(StackBuf)) return -1;
	StackBuf->nData[++StackBuf->nSize] = nNum;
	return 0;
}

/*�ж�ջ�Ƿ�Ϊ��*/
int IsEmpty(Stack *StackBuf)
{	
	if(StackBuf->nSize>0) return 0;
	else return 1;
}

/*��ʼ��ջ*/
int InitStack(Stack *StackBuf, int nMaxSize)
{
	if(nMaxSize <= 0) return -1;
	StackBuf->nMaxSize = nMaxSize;
	StackBuf->nData = malloc(nMaxSize*sizeof(int));		
	memset(StackBuf->nData, 0, nMaxSize*sizeof(int));
	StackBuf->nSize = 0;
	return 0;
}

/*�ж�ջ�Ƿ�����*/
int IsFull(Stack *StackBuf)
{
	if(StackBuf->nSize < StackBuf->nMaxSize) return 0;
	return 1;
}

/*�ͷ�ջ*/
void FreeStack(Stack *StackBuf)
{
	free(StackBuf->nData);
	StackBuf->nSize = 0;
	StackBuf->nMaxSize = 0;
}
