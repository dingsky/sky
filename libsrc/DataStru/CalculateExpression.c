#include "public.h"
#include "DataStru.h"
/*�����д���һ���Ż�*/              
/***********************************************************************************
*�޸�����: 
*20150112:
					1.�޸����ú����ļ���Ϊ����ָ��Ĵ���ʽ���������ظ����룬�����Ժ����չ  
					2.��ȫ�ֱ�����DataStru.h�ƶ������ļ�����Ϊ�����ظ��������ǲ������ظ�����
						�����������Ա�����Դ�ļ�����ͬһ��ͷ�ļ������ظ�����Ĵ���
					3.����һ����ƽ��ֵ�����ú���avg
					4.������ջ�ĳ�ʼ�����ͷš��������
*20150113��
					1.�������ƣ����ú����������ŵ��ұ�ֻ���������Ż��������
***********************************************************************************/

#define DEBUG 0
#define STACK_SIZE 1024

/*����ȫ�ֱ�������ʼ��*/
CalFunList CalFunListBuf[]
=
{
	{"min", DealMin},
	{"max", DealMax},
	{"length", DealLength},
	{"avg", DealAvg},
	{"", NULL}
};
/*˵���������Ҫ������ú�����ֻ��Ҫ��DataStru.h��Ӻ�������, ��CalFunListBuf�����Ӷ�Ӧ��ϵ����ʵ�ָøú�������*/

/*����һ���������ʽ�ַ���������ñ��ʽ�Ľ������,֧��min,max,length���������ú���*/
int CalculateExpression(char *sExpression, int * nResult)
{
	char *sTmpExpression = NULL;
	
	/*�������ı��ʽΪNULL���߳���Ϊ0�򷵻ش���*/
	if(sExpression == NULL || strlen(sExpression) == 0)
	{
		PrintLog(ERROR, "�������ʽ����Ϊ�ջ�NULL");	
		return -1;
	}
	
	PrintLog(TRACE, "����ı��ʽΪ[%s]", sExpression);
	
	/*���䶯̬�ڴ�ռ�*/
	sTmpExpression = malloc(strlen(sExpression)+100);
	memset(sTmpExpression, 0, strlen(sExpression)+100);
	
	/*�滻���ú���*/	
	if(ReplaceFunction(sExpression, sTmpExpression)) 
	{
		PrintLog(ERROR, "�滻���ú�������");
		free(sTmpExpression);
		return -1;	
	}
	
	PrintLog(TRACE, "�滻���ú�����,���ʽΪ[%s]", sTmpExpression);
	
	/*�����ʽ�ĺϷ���*/
	if(CheckExpression(sTmpExpression))
	{
		PrintLog(ERROR, "���ʽ�Ƿ�");
		free(sTmpExpression);
		return -1;	
	}
	
	/*����׺���ʽ��Ϊ��׺���ʽ*/
	if(TranInfixToSufixx(sTmpExpression))
	{
		PrintLog(ERROR, "����׺���ʽת��Ϊ��׺���ʽ����");
		free(sTmpExpression);
		return -1;	
	}
	
	PrintLog(TRACE, "���б��ʽ��Ϊ��׺���ʽ��Ϊ[%s]", sTmpExpression);
	/*�����׺���ʽ��ֵ*/
	if(CalculateSufixx(sTmpExpression, nResult))
	{
			PrintLog(ERROR, "�����׺���ʽ��ֵ����");
			free(sTmpExpression);
			return -1;
	}
	
	PrintLog(TRACE, "������ʽ�Ľ��Ϊ[%d]", *nResult);
	
	/*�ͷŶ�̬�ڴ�ռ�*/
	free(sTmpExpression);
	
	return 0;
}


/*�滻���ú���max(1,2,...), min(1,2,...), length("...")��*/
static int ReplaceFunction(char *sExpression, char *sTmpExpression)
{
	int nTmp, nNum, nLen, i=0;
	char *sStart =NULL, *sEnd = NULL, *sTmp = NULL, *sTmpStr = NULL, sFmtStr[10], sContentBuf[1024], sTmpBuf[20];
	if(sExpression == NULL || sTmpExpression == NULL) return -1;
	strcpy(sTmpExpression, sExpression);
	nLen = strlen(sTmpExpression)+100;
	sTmpStr = malloc(nLen);                 
	
	/*�滻���ú���*/
	while(CalFunListBuf[i].func != NULL && CalFunListBuf[i].sFunDesc[0] != 0)
	{
		memset(sFmtStr, 0, sizeof(sFmtStr));
		strcpy(sFmtStr, CalFunListBuf[i].sFunDesc);
		while(1)
		{
			memset(sContentBuf, 0, sizeof(sContentBuf));
			memset(sTmpBuf, 0, sizeof(sTmpBuf));
			memset(sTmpStr, 0, nLen);	
			sStart = strstr(sTmpExpression, sFmtStr);
			if( sStart == NULL ) break;
			sStart += strlen(sFmtStr);
			if(*sStart++ != '(') 
			{
				PrintLog(ERROR, "%s�����÷�����,ȱʧ������", sFmtStr);
				free(sTmpStr); 			
				return -1;
			}
			sEnd = strchr(sStart, ')');
			if(sEnd == NULL) 
			{
				PrintLog(ERROR, "%s�����÷�����,ȱʧ������", sFmtStr);
				free(sTmpStr); 			
				return -1;
			}
			
			/*���ú��������ŵ��ұ�ֻ���������Ż�������� add by dc 20150113*/
			if(*(sEnd+1) != ')' && IsMathSymbol(*(sEnd+1)) != 0 && sEnd != sTmpExpression+strlen(sTmpExpression)-1)
			{
				PrintLog(ERROR, "%s�����÷�����,�����ŵ��ұ�ֻ���������ţ����������", sFmtStr);
				free(sTmpStr); 			
				return -1;				
			}
			
			memcpy(sContentBuf, sStart, sEnd-sStart);
			if(CalFunListBuf[i].func(sContentBuf, &nNum)) 
			{
				PrintLog(ERROR, "�������ú���[%s]��ֵ����", CalFunListBuf[i].sFunDesc);
				free(sTmpStr); 			
				return -1;
			}
			sprintf(sTmpBuf, "%d", nNum);
			sStart -= strlen(sFmtStr)+1;
			nTmp = sStart-sTmpExpression;
			memcpy(sTmpStr, sTmpExpression, nTmp);
			memcpy(sTmpStr+nTmp, sTmpBuf, strlen(sTmpBuf));
			strcpy(sTmpStr+nTmp+strlen(sTmpBuf), ++sEnd);
			memset(sTmpExpression, 0, strlen(sTmpExpression));
			strcpy(sTmpExpression, sTmpStr);		
		}	
		i++;
	}
	
//	/*�滻length����, length�������������滻, �������ַ������������min max���Ӵ��ᱨ��*/
//	while(1)
//	{
//		memset(sFmtStr, 0, sizeof(sFmtStr));
//		memset(sContentBuf, 0, sizeof(sContentBuf));
//		memset(sTmpBuf, 0, sizeof(sTmpBuf));
//		memset(sTmpStr, 0, nLen);	
//		strcpy(sFmtStr, "length");
//		sStart = strstr(sTmpExpression, sFmtStr);
//		if( sStart == NULL ) break;
//		sStart += strlen(sFmtStr);
//		if(*sStart++ != '(') 
//		{
//			PrintLog(ERROR, "%s�����÷�����,ȱʧ������", sFmtStr);
//			free(sTmpStr); 			
//			return -1;
//		}
//		sEnd = strchr(sStart, ')');
//		if(sEnd == NULL) 
//		{
//			PrintLog(ERROR, "%s�����÷�����,ȱʧ������", sFmtStr);
//			free(sTmpStr); 			
//			return -1;
//		}
//		memcpy(sContentBuf, sStart, sEnd-sStart);
//		if(DealLength(sContentBuf, &nNum)) 
//		{
//			free(sTmpStr); 			
//			return -1;
//		}
//		sprintf(sTmpBuf, "%d", nNum);
//		sStart -= strlen(sFmtStr)+1;
//		nTmp = sStart-sTmpExpression;
////		memcpy(sTmpExpression+nTmp, sTmpBuf, strlen(sTmpBuf));
////		strcpy(sTmpExpression+nTmp+strlen(sTmpBuf), ++sEnd);
//		memcpy(sTmpStr, sTmpExpression, nTmp);
//		memcpy(sTmpStr+nTmp, sTmpBuf, strlen(sTmpBuf));
//		strcpy(sTmpStr+nTmp+strlen(sTmpBuf), ++sEnd);
//		memset(sTmpExpression, 0, strlen(sTmpExpression));
//		strcpy(sTmpExpression, sTmpStr);
//	}
	
//	PrintLog(TRACE, "�滻length��[%s]", sTmpExpression);		
	
//	/*�滻min����*/
//	while(1)
//	{
//		memset(sFmtStr, 0, sizeof(sFmtStr));
//		memset(sContentBuf, 0, sizeof(sContentBuf));
//		memset(sTmpBuf, 0, sizeof(sTmpBuf));
//		memset(sTmpStr, 0, nLen);		
//		strcpy(sFmtStr, "min");
//		sStart = strstr(sTmpExpression, sFmtStr);
//		if( sStart == NULL ) break;
//		sStart += strlen(sFmtStr);
//		if(*sStart++ != '(')
//		{
//			PrintLog(ERROR, "%s�����÷�����,ȱʧ������", sFmtStr);
//			free(sTmpStr); 
//			return -1;
//		}
//		sEnd = strchr(sStart, ')');
//		if(sEnd == NULL) 
//		{
//			PrintLog(ERROR, "%s�����÷�����,ȱʧ������", sFmtStr);
//			free(sTmpStr); 
//			return -1;
//		}	
//		memcpy(sContentBuf, sStart, sEnd-sStart);
//		if(DealMin(sContentBuf, &nNum)) 
//		{
//			free(sTmpStr); 
//			return -1;
//		}
//		sprintf(sTmpBuf, "%d", nNum);
//		sStart -= strlen(sFmtStr)+1;
//		nTmp = sStart-sTmpExpression;
//		//PrintLog(TRACE, "111sTmpExpression[%s]\n sStart[%s]\n sEnd[%s] \n sContentBuf[%s]", sTmpExpression, sStart, sEnd, sContentBuf);
//		memcpy(sTmpStr, sTmpExpression, nTmp);
//		memcpy(sTmpStr+nTmp, sTmpBuf, strlen(sTmpBuf));
//		strcpy(sTmpStr+nTmp+strlen(sTmpBuf), ++sEnd);
//		//memcpy(sTmpExpression+nTmp, sTmpBuf, strlen(sTmpBuf));
//		//strcpy(sTmpExpression+nTmp+strlen(sTmpBuf), ++sEnd);
//		//PrintLog(TRACE, "222sTmpExpression[%s]\n sStart[%s]\n sEnd[%s] \n sContentBuf[%s]", sTmpExpression, sStart, sEnd, sContentBuf);
//		memset(sTmpExpression, 0, strlen(sTmpExpression));
//		strcpy(sTmpExpression, sTmpStr);
//		//PrintLog(TRACE, "333sTmpExpression[%s]\n sStart[%s]\n sEnd[%s] \n sContentBuf[%s]", sTmpExpression, sStart, sEnd, sContentBuf);
//	}
//	
////	PrintLog(TRACE, "�滻min��[%s]", sTmpExpression);	
//	
//	/*�滻max����*/
//	while(1)
//	{
//		memset(sFmtStr, 0, sizeof(sFmtStr));
//		memset(sContentBuf, 0, sizeof(sContentBuf));
//		memset(sTmpBuf, 0, sizeof(sTmpBuf));
//		memset(sTmpStr, 0, nLen);	
//		strcpy(sFmtStr, "max");
//		sStart = strstr(sTmpExpression, sFmtStr);
//		if( sStart == NULL ) break;
//		sStart += strlen(sFmtStr);
//		if(*sStart++ != '(')
//		{
//			free(sTmpStr); 
//			 return -1;
//		}
//		sEnd = strchr(sStart, ')');
//		if(sEnd == NULL) 
//		{
//			free(sTmpStr); 			
//			return -1;
//		}
//		memcpy(sContentBuf, sStart, sEnd-sStart);
//		if(DealMax(sContentBuf, &nNum)) 
//		{
//			free(sTmpStr); 			
//			return -1;
//		}
//		sprintf(sTmpBuf, "%d", nNum);
//		sStart -= strlen(sFmtStr)+1;
//		nTmp = sStart-sTmpExpression;
//		//memcpy(sTmpExpression+nTmp, sTmpBuf, strlen(sTmpBuf));
//		//strcpy(sTmpExpression+nTmp+strlen(sTmpBuf), ++sEnd);
//		memcpy(sTmpStr, sTmpExpression, nTmp);
//		memcpy(sTmpStr+nTmp, sTmpBuf, strlen(sTmpBuf));
//		strcpy(sTmpStr+nTmp+strlen(sTmpBuf), ++sEnd);
//		memset(sTmpExpression, 0, strlen(sTmpExpression));
//		strcpy(sTmpExpression, sTmpStr);
//	}
//	
//	//PrintLog(TRACE, "�滻max��[%s]", sTmpExpression);
	
	/*�ͷŶ�̬�ռ�*/
	free(sTmpStr);
	
	return 0;
}
static int CheckExpression(char *sTmpExpression)
{
	int nLeftBraNum = 0, nRightBraNum = 0, i;
	/*�ַ��ĺϷ���*/
	for(i=0; i<strlen(sTmpExpression); i++)
	{
			if(!(isdigit(sTmpExpression[i])
				||IsMathSymbol(sTmpExpression[i]) == 0
				||sTmpExpression[i]=='(' ||sTmpExpression[i]==')'))
			return -1;
			if(sTmpExpression[i] == '(' ) 
			{
				if(i>0)
				{
					/*�����ŵ����ֻ�����������������*/
					if(IsMathSymbol(sTmpExpression[i-1]) != 0 && sTmpExpression[i-1] != '(') 
					{
						PrintLog(ERROR, "�����ŵ����ֻ�����������������,����Ϊ[%c]", sTmpExpression[i-1]);
						return -1;
					}
					/*�����ŵ��ұ�ֻ�������ֻ���������*/
					if((!isdigit(sTmpExpression[i+1])) && sTmpExpression[i+1] !='(') 
					{
						PrintLog(ERROR, "�����ŵ��ұ�ֻ�������ֻ���������,����Ϊ[%c]", sTmpExpression[i+1]);
						return -1;
					}
				}
				nLeftBraNum++;
			}
			else if(sTmpExpression[i] == ')') 
			{
				if(i<strlen(sTmpExpression)-1)
				{
					/*�����ŵ��ұ�ֻ�����������������*/
					if(IsMathSymbol(sTmpExpression[i+1]) != 0 && sTmpExpression[i+1] != ')') 
					{
						PrintLog(ERROR, "�����ŵ��ұ�ֻ�����������������,����Ϊ[%c]", sTmpExpression[i+1]);	
						return -1;
					}
					/*�����ŵ����ֻ�������ֻ���������*/
					if((!isdigit(sTmpExpression[i-1])) && sTmpExpression[i-1] !=')') 
					{
						PrintLog(ERROR, "�����ŵ����ֻ�������ֻ���������,����Ϊ[%c]", sTmpExpression[i-1]);
						return -1;
					}
				}
				nRightBraNum++;
			}
	}
	
	/*�������ű���Գ�*/
	if(nLeftBraNum != nRightBraNum) 
	{
		PrintLog(ERROR, "�������Ų��Գ�");
		return -1;
	}
	return 0;
}


/*������׺���ʽ,��������֣���ֱ�������ջ�������С������ֱ�������ջ������������������ջ�����з��ų�ջ
,��ջ�ķ��������ջ,ֱ������������(������Ҳ��ջ�����ǲ������ջ)
#���������������������ջ��Ϊ�������ȼ���ջ����������ȼ���,��ջ�����ų�ջ�����ջ
�ظ�#����ֱ���÷������ȼ���ջ���������ȼ��ͣ���ջΪ�գ���÷��������ջ
���ַ���������֮�󣬽����з���ջ�еķ��ŵ������ջ*/
static int TranInfixToSufixx(char *sTmpExpression)
{
	/*�������*/
	Stack StackBuf, StackBufOut;
	int i;
	char sTmpBuf[1024], sTmpBuf1[2];
	memset(sTmpBuf, 0, sizeof(sTmpBuf));
//	memset(&StackBuf, 0, sizeof(StackBuf));
//	memset(&StackBufOut, 0, sizeof(StackBufOut));
	
	/*��ʼ��ջ*/
	InitStack(&StackBuf, STACK_SIZE);
	InitStack(&StackBufOut, STACK_SIZE);
	
	for(i=0; i<strlen(sTmpExpression); i++)
	{
		switch(sTmpExpression[i])
		{
			case '(': 
				if(Push(&StackBuf, sTmpExpression[i]))
				{
					FreeStack(&StackBuf);
					FreeStack(&StackBufOut);
					PrintLog(ERROR, "push StackBuf error");
					return -1;	
				}
				break;
			case ')':
				while(1)
				{
					if(Top(&StackBuf) == '(')
					{
						
						if(Pop(&StackBuf))
						{				
							FreeStack(&StackBuf);
							FreeStack(&StackBufOut);
							PrintLog(ERROR, "pop StackBuf error");
							return -1;	
											
						}
						break;	
					}
					else
					{
						if(Push(&StackBufOut, Top(&StackBuf)))
						{
							FreeStack(&StackBuf);
							FreeStack(&StackBufOut);
							PrintLog(ERROR, "push StackBufOut error");
							return -1;		
						}
						
						if(Pop(&StackBuf))
						{
							FreeStack(&StackBuf);
							FreeStack(&StackBufOut);
							PrintLog(ERROR, "pop StackBuf error");
							return -1;		
						}						
					}
				}
				break;
			case '+': case '-' : case '*' : case '/':
				while(1)
				{
					if(IsEmpty(&StackBuf) || 
							(sTmpExpression[i]=='*' || sTmpExpression[i] == '/') && (Top(&StackBuf)=='+' || Top(&StackBuf)=='-')
							|| Top(&StackBuf) == '(')
					{
						if(Push(&StackBuf, sTmpExpression[i]))
						{
							FreeStack(&StackBuf);
							FreeStack(&StackBufOut);
							PrintLog(ERROR, "push StackBuf error");
							return -1;									
						}
						break;
					}
					else
					{
						if(Push(&StackBufOut, Top(&StackBuf)))
						{
							FreeStack(&StackBuf);
							FreeStack(&StackBufOut);
							PrintLog(ERROR, "push StackBufOut error");
							return -1;									
						}
						
						if(Pop(&StackBuf))
						{
							FreeStack(&StackBuf);
							FreeStack(&StackBufOut);
							PrintLog(ERROR, "pop StackBuf error");
							return -1;								
						}
					}
				}
				break;	
				default :
						while(sTmpExpression[i] >='0' && sTmpExpression[i]<='9')
						{
							//printf("i[%d] ch[%c]\n", i, sTmpExpression[i]);
							if(Push(&StackBufOut, sTmpExpression[i]))
							{
								FreeStack(&StackBuf);
								FreeStack(&StackBufOut);
								PrintLog(ERROR, "push StackBufOut error");
								return -1;									
							}
							i++;
						}
						i--;
						Push(&StackBufOut, ' ');
				break;
		}
	}
	
	/*��������ջ�е�Ԫ��ȫ�����������ջ*/
	while(!IsEmpty(&StackBuf))
	{
		if(Push(&StackBufOut, Top(&StackBuf)))
		{
			FreeStack(&StackBuf);
			FreeStack(&StackBufOut);
			PrintLog(ERROR, "push StackBufOut error");
			return -1;					
		}
		if(Pop(&StackBuf))
		{
			FreeStack(&StackBuf);
			FreeStack(&StackBufOut);
			PrintLog(ERROR, "pop StackBuf error");
			return -1;					
		}
	}
	
	//printf("=============================");
	/*�����ջ������ȫ������*/
	while(!(IsEmpty(&StackBufOut)))
	{
		memset(sTmpBuf1, 0, sizeof(sTmpBuf1));
		sprintf(sTmpBuf1, "%c", Top(&StackBufOut));
		strcat(sTmpBuf, sTmpBuf1);
		Pop(&StackBufOut);
	}
	TurnStringOpposite(sTmpBuf);
	
	memset(sTmpExpression, 0, strlen(sTmpExpression));
	strcpy(sTmpExpression, sTmpBuf);
//	PrintLog(TRACE,"sTmpBuf[%s]\n", sTmpBuf);

	/*�ͷ�ջ*/
	FreeStack(&StackBuf);
	FreeStack(&StackBufOut);
		
	return 0;
}

/*������׺���ʽ,���������,����ջ,
�������������, ��ȡջ����������,��������,���������ջ,�ظ����ϲ���
���ȡ��ջ��Ԫ����Ϊ���*/
static int CalculateSufixx( char *sTmpExpression, int *nResult)
{
	int i, nLen, nNum1, nNum2, nTmpResult;
	Stack StackBuf;
//	memset(&StackBuf, 0, sizeof(StackBuf));
	char sTmpBuf[100], sTmpBuf1[2];
	InitStack(&StackBuf, STACK_SIZE);
	
	nLen = strlen(sTmpExpression);
	for(i=0; i<nLen;i++)
	{
		switch(sTmpExpression[i])
		{
			case ' ':
				break;
			case '+': case '-': case '*':  case '/':
				nNum1 = Top(&StackBuf);
				Pop(&StackBuf);
				nNum2 = Top(&StackBuf);		
				Pop(&StackBuf);	
				if(sTmpExpression[i] == '+') nTmpResult=nNum2+nNum1;
				else if(sTmpExpression[i] == '-') nTmpResult=nNum2-nNum1;	
				else if(sTmpExpression[i] == '*') nTmpResult=nNum2*nNum1;
				else
				{
					if(nNum1 == 0) 
					{
						PrintLog(ERROR, "���ʽ�Ƿ�, ��������Ϊ��");
						return -1;
					} 	
					nTmpResult=nNum2/nNum1;
				}
				if(Push(&StackBuf, nTmpResult))
				{
					FreeStack(&StackBuf);
					PrintLog(ERROR, "push StackBuf error");
					return -1;					
				}
				break;
			default:
				memset(sTmpBuf, 0, sizeof(sTmpBuf));
				while(sTmpExpression[i]>='0' && sTmpExpression[i] <='9')
				{
					memset(sTmpBuf1, 0, sizeof(sTmpBuf1));
					sprintf(sTmpBuf1, "%c", sTmpExpression[i]);
					strcat(sTmpBuf, sTmpBuf1);
					i++;
				}
				if(Push(&StackBuf, atoi(sTmpBuf)))
				{
					FreeStack(&StackBuf);
					PrintLog(ERROR, "push StackBuf error");
					return -1;					
				}
				break;
		}
	}
	*nResult = Top(&StackBuf);
	
	/*�ͷ�ջ*/
	FreeStack(&StackBuf);
	
	return 0;
	
}


/*������Сֵ*/
int DealMin(char *sContent, int *nResult)
{
	int nNum = 0, nTmpNum = 0, i=0;
	char sTmpBuf[1024], sNumStr[20],*sStart = NULL, *sEnd = NULL, *sPos = NULL;
	memset(sTmpBuf, 0, sizeof(sTmpBuf));
	if(sContent[0] != ',') sprintf(sTmpBuf, "%c%s", ',', sContent);
	else strcpy(sTmpBuf, sContent);
	if(sTmpBuf[strlen(sTmpBuf)-1] != ',') strcat(sTmpBuf, ",");
		
	sPos = sTmpBuf;
	while(1)
	{
			memset(sNumStr, 0, sizeof(sNumStr));
			if((sStart = strchr(sPos, ',')) ==NULL )
			break;
		  sStart++;			
		  if((sEnd = strchr(sStart, ',')) ==NULL )
		  break;
		  memcpy(sNumStr, sStart, sEnd-sStart);
		  if(sNumStr[0] == '\0')
		  {
		  	PrintLog(ERROR, "min����ʹ�÷�������,���ż�ȱʧ����"); 
		  	return -1;
		  }
		  if(IsAllDigit(sNumStr)!=0) 
		  {
		  	PrintLog(ERROR, "min����ʹ�÷�������,���ż䲻�ܺ��з������ַ�");
		  	return -1;
		  }
		  nTmpNum = atoi(sNumStr);
		  if( i==0 ) nNum = nTmpNum;
		  else
		  {
		  	if(nTmpNum < nNum) nNum = nTmpNum;
		  }	
		  sPos = sEnd;
		  i++;
		  
	}
	if(i == 0 )
	{
		PrintLog(ERROR, "min����ʹ�÷�������, �������ż�ȱʧ����"); 
		return -1;
	}
	*nResult = nNum;
	return 0;	
}


/*�������ֵ*/
int DealMax(char *sContent, int *nResult)
{
	int nNum = 0, nTmpNum = 0, i=0;
	char sTmpBuf[1024], sNumStr[20],*sStart = NULL, *sEnd = NULL, *sPos = NULL;
	memset(sTmpBuf, 0, sizeof(sTmpBuf));
	if(sContent[0] != ',') sprintf(sTmpBuf, "%c%s", ',', sContent);
	else strcpy(sTmpBuf, sContent);
	if(sTmpBuf[strlen(sTmpBuf)-1] != ',') strcat(sTmpBuf, ",");
		
	sPos = sTmpBuf;
	while(1)
	{
			memset(sNumStr, 0, sizeof(sNumStr));
			if((sStart = strchr(sPos, ',')) ==NULL )
			break;
		  sStart++;			
		  if((sEnd = strchr(sStart, ',')) ==NULL )
		  break;
		  memcpy(sNumStr, sStart, sEnd-sStart);
		  if(sNumStr[0] == '\0') 
		  {
		  	PrintLog(ERROR, "max����ʹ�÷�������,���ż�ȱʧ����"); 
		  	return -1;
		  }
		  if(IsAllDigit(sNumStr)!=0) 
		  {
		  	PrintLog(ERROR, "max����ʹ�÷�������,���ż䲻�ܺ��з������ַ�");
		  	return -1;
		  }
		  nTmpNum = atoi(sNumStr);
		  if( i==0 ) nNum = nTmpNum;
		  else
		  {
		  	if(nTmpNum > nNum) nNum = nTmpNum;
		  }	
		  sPos = sEnd;
		  i++;
		  
	}
	if(i == 0 ) 
	{
		PrintLog(ERROR, "max����ʹ�÷�������, �������ż�ȱʧ����"); 
		return -1;
	}
	*nResult = nNum;
	return 0;
}

/*���㳤��*/
int DealLength(char *sContent, int *nResult)
{
	int nNum = 0, nTmpNum = 0, i=0;
	char sTmpBuf[1024];
	memset(sTmpBuf, 0, sizeof(sTmpBuf));

	if(*sContent != '\"') 
	{
		PrintLog(ERROR, "length����ʹ�÷�������, ȱʧ��˫����");
		return -1;
	}
	if(*(sContent+strlen(sContent)-1) != '\"') 
	{
		PrintLog(ERROR, "length����ʹ�÷�������, ȱʧ��˫����");
		return -1;
	}
	memcpy(sTmpBuf, sContent+1, strlen(sContent)-2);
	
	if(IsAllChar(sTmpBuf) != 0 ) 
	{
		PrintLog(ERROR, "length����ʹ�÷�������, ˫���ż京�зǰ���ַ�");
		return -1;
	}	
	*nResult = strlen(sTmpBuf);
	
	return 0;	
}


/*����ƽ��ֵ*/
int DealAvg(char *sContent, int *nResult)
{
	int nNum = 0, nTotalNum = 0, nCount=0;
	char sTmpBuf[1024], sNumStr[20],*sStart = NULL, *sEnd = NULL, *sPos = NULL;
	memset(sTmpBuf, 0, sizeof(sTmpBuf));
	if(sContent[0] != ',') sprintf(sTmpBuf, "%c%s", ',', sContent);
	else strcpy(sTmpBuf, sContent);
	if(sTmpBuf[strlen(sTmpBuf)-1] != ',') strcat(sTmpBuf, ",");
		
	sPos = sTmpBuf;
	while(1)
	{
			memset(sNumStr, 0, sizeof(sNumStr));
			if((sStart = strchr(sPos, ',')) ==NULL )
			break;
		  sStart++;			
		  if((sEnd = strchr(sStart, ',')) ==NULL )
		  break;
		  memcpy(sNumStr, sStart, sEnd-sStart);
		  if(sNumStr[0] == '\0') 
		  {
		  	PrintLog(ERROR, "avg����ʹ�÷�������,���ż�ȱʧ����"); 
		  	return -1;
		  }
		  if(IsAllDigit(sNumStr)!=0) 
		  {
		  	PrintLog(ERROR, "avg����ʹ�÷�������,���ż䲻�ܺ��з������ַ�");
		  	return -1;
		  }
		  nTotalNum += atoi(sNumStr);
		  nCount++;
		  sPos = sEnd;		  		  
	}
	if(nCount == 0 ) 
	{
		PrintLog(ERROR, "avg����ʹ�÷�������, �������ż�ȱʧ����"); 
		return -1;
	}
	
	nNum = nTotalNum/nCount;	
	*nResult = nNum;
	return 0;
}


