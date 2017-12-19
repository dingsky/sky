/***************************************************************************************
*���ܣ�ʵ�ֵ�����
*���ߣ�����
*ʱ�䣺2014-12-31
***************************************************************************************/
#include "DataStru.h"

/*��������*/
List *CreatList(int nData)
{
	List *ListBuf = malloc(sizeof(List));
	ListBuf->nData = nData;
	ListBuf->next = NULL;
	return ListBuf;	
}

/*������ͷ����һ��Ԫ��*/
int InsertFromHead(List *ListBuf, int nData)
{
		List *Node =NULL;
		
		/*����Ϊ�յ������,���ش���*/
		if(ListIsEmpty(ListBuf))
		{
			PrintLog(ERROR, "���ܲ����ͷΪNULL������");
			return -1;
		}
		else	/*����Ϊ�յ��������ͷ������һ���µĽڵ�*/
		{
			Node = malloc(sizeof(List));
			if(Node == NULL)
			{
				PrintLog(ERROR, "malloc Node error");
				return -1;	
			}
			Node->nData=nData;			
			if(ListBuf->next == NULL) 
			{
				ListBuf->next = Node;
				Node->next = NULL;
			}
			else
			{
				Node->next = ListBuf->next;
				ListBuf->next = Node;
			}
		}
		return 0;
}

/*������ͷɾ��һ��Ԫ��*/
int DeleteFromHead(List *ListBuf)
{
	List *Node = NULL;
	if(ListIsEmpty(ListBuf)) return -1;	
	if(ListBuf->next == NULL) return -1;
	if(ListBuf->next->next == NULL)
	{
		free(ListBuf->next);
		ListBuf->next = NULL;	
	}
	else
	{
		Node = ListBuf->next;
		ListBuf->next = ListBuf->next->next;	
		free(Node);
	}
	return 0;
}

/*������ӡ����*/
void PrintList(List *ListBuf)
{
	List *Node = ListBuf;
	while(Node != NULL)
	{
		printf("%d ", Node->nData);
		Node = Node->next;
	}	
	printf("\n");
}


/*�ͷ��������нڵ�*/
int FreeList(List *ListBuf)
{
	List *Node = ListBuf, *TmpNode = NULL;
	if(ListIsEmpty(ListBuf)) return -1;
	while(Node != NULL)
	{
		TmpNode = Node->next;
		free(Node);
		Node = TmpNode;
	}
	ListBuf = NULL;
}

/*�ж������Ƿ�Ϊ��*/
int ListIsEmpty(List *ListBuf)
{
	if(ListBuf == NULL) return 1;
	else return 0;	
}