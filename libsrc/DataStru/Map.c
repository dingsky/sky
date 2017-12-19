/***************************************************************************************
*���ܣ�ʵ��MAP�๤�ߺ���
*���ߣ�sky
*ʱ�䣺2016-08-04
***************************************************************************************/

#include "DataStru.h"

//����map
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

//�ͷ�Map
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
    pMap->nSize = 0;    //Ԫ�ظ�����Ϊ0
    free(pMap);
    
    return;    
}   

//��ѯһ��Map�ڵ�         
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

//����һ���ڵ�
short InsertMap(Map *pMap, void *sKey, short nKeyLength, void *sVal, short nValLength)
{
    MapNode *tmpMapNode = NULL, *MapNodeNew =NULL;
    
    if(pMap == NULL || sKey == NULL || sVal == NULL || nKeyLength <= 0 || nValLength <=0 ) 
    {
        PrintLog(ERROR, "invalid param");
        return -1;
    }
    
    //����һ���µĽڵ�
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
    
    //MapΪ��, ���µĽڵ�ŵ�ͷ��
    if(pMap->pMapNode == NULL)
    {
        pMap->pMapNode = MapNodeNew;    
    }
    else    //Map��������, ���µĽڵ�ŵ�β��
    {
        tmpMapNode = (MapNode *)pMap->pMapNode;   
        while(tmpMapNode != NULL && tmpMapNode->pNext != NULL)
        {        
            tmpMapNode = (MapNode *)tmpMapNode->pNext;    
        } 
        tmpMapNode->pNext = (struct MapNode *)MapNodeNew;          
    } 
    
    pMap->nSize += 1;       //MapԪ�ظ�����1 
       
    return 0;        
}

//ɾ��Map�ڵ�
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
            if(pMapNodeLast == NULL)//ͷ����һ��Ԫ��
            {
                pMap->pMapNode = (MapNode *)tmpMapNode->pNext;                     
            }
            else
            {
                pMapNodeLast->pNext =  tmpMapNode->pNext;
            }
            free(tmpMapNode);     
            tmpMapNode = NULL;    
            
            pMap->nSize -= 1;   //MapԪ�ظ�����1
                     
            return 0;    
        }
        pMapNodeLast = tmpMapNode;
        tmpMapNode = (MapNode *)tmpMapNode->pNext;
    } 
    
    return ERR_NOT_FOUND;       
}               

//����Map
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



//�޸�һ��Map�ڵ�   
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
            if(tmpMapNode->nValLength < nValLength) //���ݱ䳤, ���·����ڴ�
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

