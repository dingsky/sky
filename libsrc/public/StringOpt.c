/*
��    �ܣ���Ҫ����һЩ�ַ�����صĴ�����
��    �ߣ�����
����ʱ�䣺2015��10��30��    
*/

#include "public.h"


/*
*��������TransUpperToLower
*���ܣ�  ���ַ����еĴ�д��ĸȫ��ת����Сд��ĸ,�����ַ����ֲ���
*��Σ�  sString->�����ַ���
*���Σ�  sString->����ַ���
*˵��:
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
*��������TransLowerToUpper
*���ܣ�  ���ַ����е�Сд��ĸȫ��ת���ɴ�д��ĸ,�����ַ����ֲ���
*��Σ�  sString->�����ַ���
*���Σ�  sString->����ַ���
*˵��:
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
*��������ReverseString
*���ܣ�  ���ַ�����ͷ
*��Σ�  sString->�����ַ���
*���Σ�  sString->����ַ���
*˵��:	 ��"abc"ת��Ϊ"cba"
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
*��������TurnRoundString
*���ܣ�  �������ַ��д�д�ַ�ת��ΪСд, Сд�ַ�ת��Ϊ��д, �����ַ�����
*��Σ�  sString->�����ַ���
*���Σ�  sString->����ַ���
*˵��:	 ��"A123bZk"ת��Ϊ"a123BzK"
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
*��������IsUpperString
*���ܣ�  �ж�һ���ַ����Ƿ���ɴ�д�ַ���� 
*��Σ�  sString->�����ַ���
*����ֵ: Ntrue -> ��, NFalse -> ��
*˵��:	
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
*��������IsLowerString
*���ܣ�  �ж�һ���ַ����Ƿ����Сд�ַ���� 
*��Σ�  sString->�����ַ���
*����ֵ: Ntrue -> ��, NFalse -> ��
*˵��:	 
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
*��������IsCharString
*���ܣ�  �ж�һ���ַ����Ƿ����Ӣ���ַ���� 
*��Σ�  sString->�����ַ���
*����ֵ: Ntrue -> ��, NFalse -> ��
*˵��:	 
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
*��������IsDigitString
*���ܣ�  �ж�һ���ַ����Ƿ�����������
*��Σ�  sString->�����ַ���
*����ֵ: Ntrue -> ��, NFalse -> ��
*˵��:	 
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
*��������TrimString
*���ܣ�  ȥ��һ���ַ��������еĿո�, �����ַ������λ�ñ��ֲ���
*��Σ�  sString->�����ַ���
*���Σ�  sString->����ַ���
*˵��:	 
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
*��������TrimString
*���ܣ�  ȥ��һ���ַ��������׿ո�
*��Σ�  sString->�����ַ���
*���Σ�  sString->����ַ���
*˵��:	 
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
*��������TrimString
*���ܣ�  ȥ��һ���ַ�������β�ո�
*��Σ�  sString->�����ַ���
*���Σ�  sString->����ַ���
*˵��:	 
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
*��������TrimNewLine
*���ܣ�  ȥ��һ���ַ�������β���з�
*��Σ�  sString->�����ַ���
*���Σ�  sString->����ַ���
*˵��:	 
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
*��������FillString
*���ܣ�  ʹ��ָ�����ַ���䵽�ַ�����
*��Σ�  sString->�����ַ��� ch->����ַ� snFillFlag:RIGTH_FILL->����� LEFT_FILL->����� inFillLen->��䳤��
*���Σ�  sString->����ַ���
*˵��:	 ��������û�а취�жϵ������Ƿ���Խ������ڴ�����, ��Ҫ�����������ʱ�����㹻���ڴ�
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
*��������LTrimZero
*���ܣ�  ȥ��һ���ַ�����������
*��Σ�  sString->�����ַ���
*���Σ�  sString->����ַ���
*˵��:	 
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
*��������RemoveComments
*���ܣ�  ��sSymbolΪע�ͷ���,ȥ��sString�е�ע������
*��Σ�  sString->�����ַ��� sSymbol->ע�ͷ���
*���Σ�  sString->����ַ���
*˵��:	 RemoveComments("abc#123", "#") -> "abc"
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
*��������AppendString
*���ܣ�  ��sStringAdd׷�ӵ�sString����ʼλ��
*��Σ�  sString->�����ַ��� sStringAdd->׷���ַ���
*���Σ�  sString->����ַ���
*˵��:	 AppendStr("123", "456");	-> "456123"
*				 ���������޷��жϵ������Ƿ����Խ������ڴ�����, ��������ҪΪsString�����㹻���ڴ�ռ�			 
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
*��������TransCharToDigit
*���ܣ�  ���ַ�ת��Ϊ����ASCII��
*��Σ�  ch->�����ַ�
*���أ�  ��������ASCII��
*˵��:	 
*/
int TransCharToDigit(char ch)
{
    if(isdigit(ch)) return (ch - '0');
    else if(isupper(ch)) return (ch - 'A');
    else if(islower(ch)) return (ch - 'a');
    else return -1;    
}

/*
*��������TransAscToBcd
*���ܣ�  ��ASC�ַ���ת��ΪBCD��
*��Σ�  sStringInput->�����ַ���
*���Σ�  sStringOutput->����ַ���
*˵��:	 TransAscToBcd("30313233", sStringOutput); sStringOutput-> "0123"	 
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
*��������TransBcdToAsc
*���ܣ�  ��BCD�ַ���ת��ΪASC��
*��Σ�  sStringInput->�����ַ��� inLen->���봮����
*���Σ�  sStringOutput->����ַ���
*˵��:	 TransAscToBcd("30313233", sStringOutput); sStringOutput-> "0123"	 
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


