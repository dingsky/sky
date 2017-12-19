#ifndef _MSG_H_ 
#define _MSG_H_ 
#include "public.h"

#define MAX_REGION_SIZE  500
#define OUT_OF_REGION -2

/*ת���ַ�����ʽ*/
void TranStringFmt( char *, char *, char *);

/*����ַ�������ĸ���by str*/
int GetRegionNumByStr( char *, char *);

/*��õ�nCount�������by str*/
int GetCountRegionByStr( char *, char *, int, char *);

/*���������򣬴浽������`*/
char **GetRegionIntoArrayByStr(char *sStringInput, char *sStr);

/*�ͷŶ�̬����*/
void FreeArray(char **sArray);

#endif
