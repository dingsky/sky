#ifndef _MSG_H_ 
#define _MSG_H_ 
#include "public.h"

#define MAX_REGION_SIZE  500
#define OUT_OF_REGION -2

/*转换字符串格式*/
void TranStringFmt( char *, char *, char *);

/*获得字符串中域的个数by str*/
int GetRegionNumByStr( char *, char *);

/*获得第nCount域的内容by str*/
int GetCountRegionByStr( char *, char *, int, char *);

/*解析各个域，存到数组中`*/
char **GetRegionIntoArrayByStr(char *sStringInput, char *sStr);

/*释放动态缓存*/
void FreeArray(char **sArray);

#endif
