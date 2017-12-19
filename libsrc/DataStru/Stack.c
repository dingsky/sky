/*实现栈的相关功能*/
/***********************************************************************************
*修改履历:
*20150112 增加IsFull函数，防止栈的溢出
					增加InitStack函数初始化栈
***********************************************************************************/

#include "DataStru.h"

/*返回栈顶元素*/
int Top(Stack *StackBuf)
{
	return StackBuf->nData[StackBuf->nSize];
	
}

/*弹出栈顶元素*/
int Pop(Stack *StackBuf)
{
	if(IsEmpty(StackBuf)) return -1;
	StackBuf->nSize--;
	return 0;
	//return StackBuf->nData[StackBuf->nSize--];
}

/*入栈*/
int Push(Stack *StackBuf, int nNum)
{
	if(IsFull(StackBuf)) return -1;
	StackBuf->nData[++StackBuf->nSize] = nNum;
	return 0;
}

/*判断栈是否为空*/
int IsEmpty(Stack *StackBuf)
{	
	if(StackBuf->nSize>0) return 0;
	else return 1;
}

/*初始化栈*/
int InitStack(Stack *StackBuf, int nMaxSize)
{
	if(nMaxSize <= 0) return -1;
	StackBuf->nMaxSize = nMaxSize;
	StackBuf->nData = malloc(nMaxSize*sizeof(int));		
	memset(StackBuf->nData, 0, nMaxSize*sizeof(int));
	StackBuf->nSize = 0;
	return 0;
}

/*判断栈是否已满*/
int IsFull(Stack *StackBuf)
{
	if(StackBuf->nSize < StackBuf->nMaxSize) return 0;
	return 1;
}

/*释放栈*/
void FreeStack(Stack *StackBuf)
{
	free(StackBuf->nData);
	StackBuf->nSize = 0;
	StackBuf->nMaxSize = 0;
}
