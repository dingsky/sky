/*ʵ�ֳ����������㷨*/
#include "DataStru.h"

/*��������*/
void PrintArray(int nArray[], int nCount)
{
	int i;	
	for(i=0; i<nCount; i++)
	{
		printf("%d ", nArray[i]);	
	}
	printf("\n");
}

/*��������*/
void MySwap(int *nNum1, int *nNum2)
{
	int nTmp = *nNum1;
	*nNum1 = *nNum2;
	*nNum2 = nTmp;	
}

/*ð������*/
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
		if(!nSwapFlag) break;	/*ǰһ������û�з�����������˵���Ѿ��������*/
	}
}

/*ѡ������*/
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


/*��������*/            
