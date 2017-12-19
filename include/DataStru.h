#ifndef _DATA_STRU_
#define _DATA_STRU_
#include "public.h"

/*List.c*/
struct List
{
        int nData;
        struct List *next;
};

typedef struct List List;
List *CreatList(int nData);
int InsertFromHead(List *, int);
int DeleteFromHead(List *);
void PrintList(List *);
int FreeList(List *);
int ListIsEmpty(List *);


/*Stack.c*/
/*定义栈*/
typedef struct 
{
//	int nData[1000];
	int *nData;
	int nSize;	
	int nMaxSize;
}Stack;
int Push(Stack *, int);	
int Pop(Stack *);
int Top(Stack *);
int IsEmpty(Stack *);
int IsFull(Stack *);
int InitStack(Stack *, int);
void FreeStack(Stack *);

/*CalculateExpression.c*/
/*函数声明*/
static int ReplaceFunction(char *, char *);
static int CheckExpression(char *);
static int TranInfixToSufixx(char *);
static int CalculateSufixx( char *, int *);
int CalculateExpression(char *, int *);
int DealMin(char *, int *);
int DealMax(char *, int *);
int DealLength(char *, int *);
int DealAvg(char *, int *);
int DealExp(char *, int *);
typedef struct 
{
	char sFunDesc[50+1];
	int (* func)(char *, int *);
}CalFunList;



/*Quenue.c*/
typedef struct
{
//	int nData[1000];
	int *nData;
	int nSize;	
	int nMaxSize;
}Quence;
int QueIsEmpty(Quence *);
int QueIsFull(Quence *);
int InQuence(Quence *, int);
int OutQuence(Quence *);
int QueLastElement(Quence *);
int PrintQuence(Quence *);
int InitQuence(Quence *, int);
void FreeQuence(Quence *);

/*Sort.c*/
void PrintArray(int [], int );
void MySwap(int *, int *);
void BubbleSort(int [], int );

//Map.c
typedef struct
{
    void *sKey;
    short nKeyLength;
    void *sVal;
    short nValLength;
    struct MapNode *pNext;
        
}MapNode;

typedef struct
{
    short nSize;            //Map当前元素个数
    MapNode *pMapNode;      //Map保存信息的链表    
}Map;
Map *CreateMap();           //创建Map
void DropMap(Map *);        //释放Map
short SelectMapNode(Map *, void *, short, void *, short *);       //查询一个Map节点
short InsertMap(Map *, void *, short, void *, short);             //插入一个节点
short DeleteMapNode(Map *, void *, short);                        //删除Map节点
short UpdateMapNode(Map *, void *, short, void *, short);         //更新一个节点
short GetMapSize(Map *);                                          //返回Map元素个数
typedef (*Func)(void *, short, void *, short);
#define ERR_NOT_FOUND       100



#endif
