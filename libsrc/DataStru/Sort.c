/*实现常见的排序算法*/
#include "DataStru.h"

/*遍历数组*/
void PrintArray(int nArray[], int nCount)
{
	int i;	
	for(i=0; i<nCount; i++)
	{
		printf("%d ", nArray[i]);	
	}
	printf("\n");
}

/*交换函数*/
void MySwap(int *nNum1, int *nNum2)
{
	int nTmp = *nNum1;
	*nNum1 = *nNum2;
	*nNum2 = nTmp;	
}

/*冒泡排序*/
void BubbleSort(int nArray[], int nCount)
{
	int i,j, nSwapFlag;
	for(i=0; i<nCount; i++)
	{
		nSwapFlag = 0;
		for(j=nCount-1;j>i;j--)
		{
			if(nArray[j]<nArray[j-1]) 
			{
				MySwap(&nArray[j], &nArray[j-1]);	
				nSwapFlag = 1;
			}
		}
		if(!nSwapFlag) break;	/*前一次排序没有发生过交换，说明已经排序完成*/
	}
}

/*选择排序*/
void ChooseSort(int nArray[], int nCount)
{
	int i,j, k;
	for(i=0; i<nCount; i++)
	{
		k = i;
		for(j=i+1; j<nCount; j++)
		{
			if(nArray[j] < nArray[k])
			{ 
				k=j;
			}	
		}	
		MySwap(&nArray[i], &nArray[k]);
	}
}


/*插入排序*/            
