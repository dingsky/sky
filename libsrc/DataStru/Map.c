/***************************************************************************************
*功能：实现MAP类工具函数
*作者：sky
*时间：2016-08-04
***************************************************************************************/

#include "DataStru.h"

//创建map
Map *CreateMap()
{
    Map *pMap = NULL;
    
    pMap = malloc(sizeof(Map));
    if(pMap == NULL)
    {
        PrintLog(ERROR, "malloc error!");
        return NULL;    
    }
    
    pMap->nSize = 0;
    pMap->pMapNode = NULL;

    return pMap;            
}

//释放Map
void DropMap(Map *pMap)
{
    MapNode *tmpMapNode = NULL;
    MapNode *tmpMapNodeNext = NULL;
    
    if(pMap == NULL || pMap->pMapNode == NULL) return;
    tmpMapNode = (MapNode *)pMap->pMapNode;
    
    while(tmpMapNode != NULL)
    {
        tmpMapNodeNext = (MapNode *)tmpMapNode->pNext;              
        if(tmpMapNode->sKey != NULL) free(tmpMapNode->sKey);
        if(tmpMapNode->sVal != NULL) free(tmpMapNode->sVal);  
        free(tmpMapNode);        
        tmpMapNode = tmpMapNodeNext;
    }    
    
    pMap->pMapNode = NULL;
    pMap->nSize = 0;    //元素个数置为0
    free(pMap);
    
    return;    
}   

//查询一个Map节点         
short SelectMapNode(Map *pMap, void *sKey, short nKeyLength, void *sVal, short *pValLength)
{
    MapNode *tmpMapNode = NULL;
    
    if(pMap == NULL || sKey == NULL || sVal == NULL || nKeyLength <= 0 || pMap->pMapNode == NULL) return -1;
    tmpMapNode = (MapNode *)pMap->pMapNode;

    while(tmpMapNode != NULL)
    {        
        if(memcmp(tmpMapNode->sKey, sKey, nKeyLength) == 0 &&
           tmpMapNode->nKeyLength == nKeyLength)
        {
            memcpy(sVal, tmpMapNode->sVal, tmpMapNode->nValLength);
            *pValLength = tmpMapNode->nValLength;
            return 0;    
        }
        tmpMapNode = (MapNode *)tmpMapNode->pNext;
    }    
              
    return ERR_NOT_FOUND;        
}

//插入一个节点
short InsertMap(Map *pMap, void *sKey, short nKeyLength, void *sVal, short nValLength)
{
    MapNode *tmpMapNode = NULL, *MapNodeNew =NULL;
    
    if(pMap == NULL || sKey == NULL || sVal == NULL || nKeyLength <= 0 || nValLength <=0 ) 
    {
        PrintLog(ERROR, "invalid param");
        return -1;
    }
    
    //创建一个新的节点
    MapNodeNew = malloc(sizeof(MapNode));
    if(MapNodeNew == NULL)
    {
        PrintLog(ERROR, "malloc error!");
        return -1;    
    } 
    MapNodeNew->sKey = calloc(1, nKeyLength+1);
    if(MapNodeNew->sKey == NULL)
    {
        free(pMap->pMapNode);
        pMap->pMapNode = NULL;
        PrintLog(ERROR, "calloc error!");
        return -1;                
    }
    MapNodeNew->sVal = calloc(1, nValLength+1);
    if(MapNodeNew->sVal == NULL)
    {
        free(pMap->pMapNode->sKey);
        pMap->pMapNode->sKey=NULL;
        free(pMap->pMapNode);
        pMap->pMapNode = NULL;            
        PrintLog(ERROR, "calloc error!");
        return -1;                
    }        
    memcpy(MapNodeNew->sKey, sKey, nKeyLength);   
    MapNodeNew->nKeyLength = nKeyLength;     
    memcpy(MapNodeNew->sVal, sVal, nValLength);    
    MapNodeNew->nValLength = nValLength;  
    
    //Map为空, 则将新的节点放到头部
    if(pMap->pMapNode == NULL)
    {
        pMap->pMapNode = MapNodeNew;    
    }
    else    //Map已有数据, 将新的节点放到尾部
    {
        tmpMapNode = (MapNode *)pMap->pMapNode;   
        while(tmpMapNode != NULL && tmpMapNode->pNext != NULL)
        {        
            tmpMapNode = (MapNode *)tmpMapNode->pNext;    
        } 
        tmpMapNode->pNext = (struct MapNode *)MapNodeNew;          
    } 
    
    pMap->nSize += 1;       //Map元素个数加1 
       
    return 0;        
}

//删除Map节点
short DeleteMapNode(Map *pMap, void *sKey, short nKeyLength)
{
    if(pMap == NULL || sKey == NULL|| nKeyLength <= 0 || pMap->pMapNode == NULL) return -1;
    MapNode *tmpMapNode = pMap->pMapNode;
    MapNode *pMapNodeLast = NULL;   
    
    while(tmpMapNode != NULL)
    {        
        if(memcmp(tmpMapNode->sKey, sKey, nKeyLength) == 0 &&
           tmpMapNode->nKeyLength == nKeyLength)
        {
            if(pMapNodeLast == NULL)//头部第一个元素
            {
                pMap->pMapNode = (MapNode *)tmpMapNode->pNext;                     
            }
            else
            {
                pMapNodeLast->pNext =  tmpMapNode->pNext;
            }
            free(tmpMapNode);     
            tmpMapNode = NULL;    
            
            pMap->nSize -= 1;   //Map元素个数减1
                     
            return 0;    
        }
        pMapNodeLast = tmpMapNode;
        tmpMapNode = (MapNode *)tmpMapNode->pNext;
    } 
    
    return ERR_NOT_FOUND;       
}               

//遍历Map
void ShowMap(Map *pMap, Func func)
{
    MapNode *tmpMapNode = NULL;
    
    if(pMap == NULL || pMap->pMapNode == NULL || func == NULL) return;
    tmpMapNode = (MapNode *)pMap->pMapNode;

    while(tmpMapNode != NULL)
    {        
        func(tmpMapNode->sKey, tmpMapNode->nKeyLength, tmpMapNode->sVal, tmpMapNode->nValLength);
        tmpMapNode = (MapNode *)tmpMapNode->pNext;
    }  
    
    return;       
}



//修改一个Map节点   
short UpdateMapNode(Map *pMap, void *sKey, short nKeyLength, void *sVal, short nValLength)
{
    MapNode *tmpMapNode = NULL;
    
    if(pMap == NULL || sKey == NULL || sVal == NULL || nKeyLength <= 0 || pMap->pMapNode == NULL) return -1;
    tmpMapNode = (MapNode *)pMap->pMapNode;

    while(tmpMapNode != NULL)
    {        
        if(memcmp(tmpMapNode->sKey, sKey, nKeyLength) == 0 &&
           tmpMapNode->nKeyLength == nKeyLength)
        {
            if(tmpMapNode->nValLength < nValLength) //数据变长, 重新分配内存
            {
                free(tmpMapNode->sVal);
                tmpMapNode->sVal = calloc(1, nValLength+1);
                if(tmpMapNode->sVal == NULL)
                {
                    PrintLog(ERROR, "calloc error!");
                    return -1; 
                }
            }
            memcpy(tmpMapNode->sVal, sVal, nValLength);
            tmpMapNode->nValLength = nValLength;
            return 0;    
        }
        tmpMapNode = (MapNode *)tmpMapNode->pNext;
    }    
              
    return ERR_NOT_FOUND;        
}


short GetMapSize(Map *pMap)
{
    return pMap->nSize;    
}

