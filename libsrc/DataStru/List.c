/***************************************************************************************
*功能：实现单链表
*作者：丁春
*时间：2014-12-31
***************************************************************************************/
#include "DataStru.h"

/*创建链表*/
List *CreatList(int nData)
{
	List *ListBuf = malloc(sizeof(List));
	ListBuf->nData = nData;
	ListBuf->next = NULL;
	return ListBuf;	
}

/*从链表头插入一个元素*/
int InsertFromHead(List *ListBuf, int nData)
{
		List *Node =NULL;
		
		/*链表为空的情况下,返回错误*/
		if(ListIsEmpty(ListBuf))
		{
			PrintLog(ERROR, "不能插入表头为NULL的链表");
			return -1;
		}
		else	/*链表不为空的情况下在头部插入一个新的节点*/
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

/*从链表头删除一个元素*/
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

/*遍历打印链表*/
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


/*释放链表所有节点*/
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

/*判断链表是否为空*/
int ListIsEmpty(List *ListBuf)
{
	if(ListBuf == NULL) return 1;
	else return 0;	
}