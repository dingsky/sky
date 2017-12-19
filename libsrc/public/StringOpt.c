/*
功    能：主要包含一些字符串相关的处理函数
作    者：丁春
创建时间：2015年10月30日    
*/

#include "public.h"


/*
*函数名：TransUpperToLower
*功能：  将字符串中的大写字母全部转换成小写字母,其他字符保持不变
*入参：  sString->输入字符串
*出参：  sString->输出字符串
*说明:
*/
void TransUpperToLower(char *sString)
{
    int inLen = 0, inLix;
    
    if(sString == NULL) return;
        
    inLen = strlen(sString);
    for(inLix=0; inLix<inLen; inLix++)
    {
        if(isupper(sString[inLix]))
        {
            sString[inLix] += 32;
        }    
    }
    
    return;    
}



/*
*函数名：TransLowerToUpper
*功能：  将字符串中的小写字母全部转换成大写字母,其他字符保持不变
*入参：  sString->输入字符串
*出参：  sString->输出字符串
*说明:
*/
void TransLowerToUpper(char *sString)
{
    int inLen = 0, inLix;
    
    if(sString == NULL) return;
        
    inLen = strlen(sString);
    for(inLix=0; inLix<inLen; inLix++)
    {
        if(islower(sString[inLix]))
        {
            sString[inLix] -= 32;
        }    
    }
    
    return;    
}

/*
*函数名：ReverseString
*功能：  将字符串掉头
*入参：  sString->输入字符串
*出参：  sString->输出字符串
*说明:	 将"abc"转换为"cba"
*/
void ReverseString(char *sString)
{
	char *sTempPoint = NULL, *sPointStart = NULL;
	int inLen, inLix;

	inLen = strlen(sString);	
	if(sString == NULL || inLen <= 0) return;
	
	sTempPoint = malloc(inLen);
	if(sTempPoint == NULL) return;
	strcpy(sTempPoint, sString);	
	sPointStart = sTempPoint;
	sTempPoint += inLen-1;
	for(inLix=0; inLix<inLen; inLix++)
	{
		*sString++ = *sTempPoint--;	
  }

	free(sPointStart);
	return;	
}

/*
*函数名：TurnRoundString
*功能：  将输入字符中大写字符转换为小写, 小写字符转换为大写, 其他字符不变
*入参：  sString->输入字符串
*出参：  sString->输出字符串
*说明:	 将"A123bZk"转换为"a123BzK"
*/
void TurnRoundString(char *sString)
{
	int inLen, inLix;	
	
	if(sString == NULL) return;
	
	inLen=strlen(sString);
	for(inLix=0; inLix<inLen; inLix++)
	{
  	if(islower(sString[inLix]))
    {
        sString[inLix] -= 32;
    }   
    else if(isupper(sString[inLix]))
    {
        sString[inLix] += 32;
    }
	}
	
	return;
}


/*
*函数名：IsUpperString
*功能：  判断一个字符串是否仅由大写字符组成 
*入参：  sString->输入字符串
*返回值: Ntrue -> 真, NFalse -> 假
*说明:	
*/
short IsUpperString(char *sString)
{
	int inLen, inLix;	
	if(sString == NULL) return NFalse;
		
	inLen = strlen(sString);
	for(inLix=0; inLix<inLen; inLix++)
	{
		if(!isupper(sString[inLix]))	return NFalse;
	}
	
	return NTrue;
}

/*
*函数名：IsLowerString
*功能：  判断一个字符串是否仅由小写字符组成 
*入参：  sString->输入字符串
*返回值: Ntrue -> 真, NFalse -> 假
*说明:	 
*/
short IsLowerString(char *sString)
{
	int inLen, inLix;	
	if(sString == NULL) return NFalse;
		
	inLen = strlen(sString);
	for(inLix=0; inLix<inLen; inLix++)
	{
		if(!islower(sString[inLix]))	return NFalse;
	}
	
	return NTrue;
}

/*
*函数名：IsCharString
*功能：  判断一个字符串是否仅由英文字符组成 
*入参：  sString->输入字符串
*返回值: Ntrue -> 真, NFalse -> 假
*说明:	 
*/
short IsCharString(char *sString)
{
	int inLen, inLix;	
	if(sString == NULL) return NFalse;
		
	inLen = strlen(sString);
	for(inLix=0; inLix<inLen; inLix++)
	{
		if(!islower(sString[inLix]) && !isupper(sString[inLix]))	return NFalse;
	}
	
	return NTrue;
}

/*
*函数名：IsDigitString
*功能：  判断一个字符串是否仅由数字组成
*入参：  sString->输入字符串
*返回值: Ntrue -> 真, NFalse -> 假
*说明:	 
*/

short IsDigitString(char *sString)
{
	int inLen, inLix;	
	if(sString == NULL) return NFalse;
		
	inLen = strlen(sString);
	for(inLix=0; inLix<inLen; inLix++)
	{
		if(!isdigit(sString[inLix]))	return NFalse;
	}
	
	return NTrue;		
}

/*
*函数名：TrimString
*功能：  去除一个字符串中所有的空格, 其他字符的相对位置保持不变
*入参：  sString->输入字符串
*出参：  sString->输出字符串
*说明:	 
*/
void TrimString(char *sString)
{
	char *sTempPoint = NULL, *sPointStart = NULL;
	int inLen;
	

	if(sString == NULL) return;
	inLen = strlen(sString);	
	if(inLen <= 0) return;
		
	sTempPoint = malloc(inLen);	
	strcpy(sTempPoint, sString);
	sPointStart = sTempPoint;
	while(*sTempPoint != 0)
	{
		if(*sTempPoint != ' ')
		{
			*sString++ = *sTempPoint++;	
		}		
		else sTempPoint++;
	}		
	
	*sString = 0;
	
	free(sPointStart);
	
	return;
}

/*
*函数名：TrimString
*功能：  去除一个字符串中行首空格
*入参：  sString->输入字符串
*出参：  sString->输出字符串
*说明:	 
*/
void LTrimString(char *sString)
{
	char *sTempPoint = NULL, *sPointStart = NULL;
	int inLen;
	

	if(sString == NULL) return;
	inLen = strlen(sString);
	if(inLen <= 0) return;
		
	sTempPoint = malloc(inLen);	
	strcpy(sTempPoint, sString);
	sPointStart = sTempPoint;
	while(*sTempPoint++ == ' ');
	sTempPoint--;
	strcpy(sString, sTempPoint);
	
	free(sPointStart);
	
	return;
}

/*
*函数名：TrimString
*功能：  去除一个字符串中行尾空格
*入参：  sString->输入字符串
*出参：  sString->输出字符串
*说明:	 
*/
void RTrimString(char *sString)
{
	char *sTempPoint = NULL, *sPointStart = NULL;
	int inLen;
	
	if(sString == NULL) return;
	inLen = strlen(sString);
	if(inLen <=0) return;
		
	sTempPoint = malloc(inLen);	
	strcpy(sTempPoint, sString);
	sPointStart = sTempPoint;
	sTempPoint += inLen-1;
	while(*sTempPoint == ' ') sTempPoint--;
	*++sTempPoint = 0;
	strcpy(sString, sPointStart);
	
	free(sPointStart);
	
	return;
}

/*
*函数名：TrimNewLine
*功能：  去除一个字符串的行尾换行符
*入参：  sString->输入字符串
*出参：  sString->输出字符串
*说明:	 
*/
void TrimNewLine(char *sString)
{
	int inLen;
	
	if(sString == NULL) return;
	
	inLen = strlen(sString);	
	if(sString[inLen-1] == '\n') sString[inLen-1] = 0;
	return;
}


/*
*函数名：FillString
*功能：  使用指定的字符填充到字符串中
*入参：  sString->输入字符串 ch->填充字符 snFillFlag:RIGTH_FILL->右填充 LEFT_FILL->左填充 inFillLen->填充长度
*出参：  sString->输出字符串
*说明:	 函数本身并没有办法判断调用者是否有越界访问内存的情况, 需要调用者在填充时申请足够的内存
*/
void FillString(char *sString, char ch, short snFillFlag, int inFillLen)
{
	int inLen, inLix;
	char *sTempPoint = NULL, *sStartPoint = NULL;
	
	if(sString == NULL || inFillLen <= 0) return;
		
	inLen = strlen(sString);
	sTempPoint = malloc(inLen+inFillLen);
	if(sTempPoint == NULL) return;				
	strcpy(sTempPoint, sString);
	sStartPoint = sTempPoint;
	
	switch(snFillFlag)
	{
		case RIGHT_FILL:
			sTempPoint += inLen;
			for(inLix=0; inLix<inFillLen; inLix++)
			{
				*sTempPoint++ = ch;			
			}
			*sTempPoint = 0;
			strcpy(sString, sStartPoint);
			break;
		case LEFT_FILL:
			for(inLix=0; inLix<inFillLen; inLix++)
			{
				*sTempPoint++ = ch;
			}
			*sTempPoint = 0;
			strcat(sTempPoint, sString);
			strcpy(sString, sStartPoint);
			break;
		default:
			break;
	}
	
	free(sStartPoint);	
	return;		
}


/*
*函数名：LTrimZero
*功能：  去除一个字符串中行首零
*入参：  sString->输入字符串
*出参：  sString->输出字符串
*说明:	 
*/
void LTrimZero(char *sString)
{
	char *sTempPoint = NULL, *sPointStart = NULL;
	int inLen;
	

	if(sString == NULL) return;
	inLen = strlen(sString);
	if(inLen <= 0) return;
		
	sTempPoint = malloc(inLen);	
	strcpy(sTempPoint, sString);
	sPointStart = sTempPoint;
	while(*sTempPoint++ == '0');
	sTempPoint--;
	strcpy(sString, sTempPoint);
	
	free(sPointStart);
	
	return;
}

/*
*函数名：RemoveComments
*功能：  以sSymbol为注释符号,去除sString中的注释内容
*入参：  sString->输入字符串 sSymbol->注释符号
*出参：  sString->输出字符串
*说明:	 RemoveComments("abc#123", "#") -> "abc"
*/
void RemoveComments(char *sString, char *sSymbol)
{
	char *sStart = NULL;
	
	if(sString == NULL || sSymbol == NULL || sSymbol[0] == 0) return;		
	if((sStart=strstr(sString, sSymbol)) == NULL)	return;
	*sStart = 0;
	
	return;
}

/*
*函数名：AppendString
*功能：  将sStringAdd追加到sString的起始位置
*入参：  sString->输入字符串 sStringAdd->追加字符串
*出参：  sString->输出字符串
*说明:	 AppendStr("123", "456");	-> "456123"
*				 函数本身无法判断调用者是否存在越界访问内存的情况, 调用者需要为sString申请足够的内存空间			 
*/
void AppendString(char *sString, char *sStringAdd)
{
	char *sTempPoint = NULL;
	
	if(sString == NULL || sStringAdd == NULL || sStringAdd[0] == 0) return;
	
	sTempPoint = (char *)malloc(strlen(sString)+strlen(sStringAdd));
	sprintf(sTempPoint, "%s%s", sStringAdd, sString);
	strcpy(sString, sTempPoint);
	
	free(sTempPoint);
	return;	
}

/*
*函数名：TransCharToDigit
*功能：  将字符转换为整型ASCII码
*入参：  ch->输入字符
*返回：  返回整型ASCII码
*说明:	 
*/
int TransCharToDigit(char ch)
{
    if(isdigit(ch)) return (ch - '0');
    else if(isupper(ch)) return (ch - 'A');
    else if(islower(ch)) return (ch - 'a');
    else return -1;    
}

/*
*函数名：TransAscToBcd
*功能：  将ASC字符串转换为BCD串
*入参：  sStringInput->输入字符串
*出参：  sStringOutput->输出字符串
*说明:	 TransAscToBcd("30313233", sStringOutput); sStringOutput-> "0123"	 
*/
void TransAscToBcd(char *sStringInput, unsigned char *sStringOutput)
{
    int inLen = 0, inLix, inLiy;
    
    if(sStringInput == NULL || sStringOutput == NULL || sStringInput[0] == 0) return;
        
    inLen = strlen(sStringInput);    			
    for(inLix=0, inLiy=0; inLiy<inLen; inLix++, inLiy += 2)
    {
        sStringOutput[inLix] = 16*(TransCharToDigit(sStringInput[inLiy]));
        sStringOutput[inLix] += TransCharToDigit(sStringInput[inLiy+1]);
    }
    
    return;
} 

/*
*函数名：TransBcdToAsc
*功能：  将BCD字符串转换为ASC串
*入参：  sStringInput->输入字符串 inLen->输入串长度
*出参：  sStringOutput->输出字符串
*说明:	 TransAscToBcd("30313233", sStringOutput); sStringOutput-> "0123"	 
*/
void TransBcdToAsc(unsigned char *sStringInput, int inLen, char *sStringOutput)
{
    int inLix, nOffset;
    
    if(sStringInput == NULL || sStringOutput == NULL) return;
        		
    nOffset = 0;    		
    for(inLix=0; inLix<inLen; inLix++)
    {
        sprintf(sStringOutput+nOffset, "%02x", sStringInput[inLix]);    
        nOffset += 2;
    }
} 


