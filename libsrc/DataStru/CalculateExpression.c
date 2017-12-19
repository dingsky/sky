#include "public.h"
#include "DataStru.h"
/*程序有待下一步优化*/              
/***********************************************************************************
*修改履历: 
*20150112:
					1.修改内置函数的计算为函数指针的处理方式，消除了重复代码，便于以后的扩展  
					2.将全局变量由DataStru.h移动到此文件，因为可以重复声明但是不可以重复定义
						这样做，可以避免多个源文件包含同一个头文件导致重复定义的错误
					3.增加一个求平均值的内置函数avg
					4.新增对栈的初始化、释放、错误处理等
*20150113：
					1.增加限制：内置函数的右括号的右边只能是右括号或者运算符
***********************************************************************************/

#define DEBUG 0
#define STACK_SIZE 1024

/*定义全局变量并初始化*/
CalFunList CalFunListBuf[]
=
{
	{"min", DealMin},
	{"max", DealMax},
	{"length", DealLength},
	{"avg", DealAvg},
	{"", NULL}
};
/*说明：今后需要添加内置函数，只需要在DataStru.h添加函数声明, 在CalFunListBuf中增加对应关系，并实现该该函数即可*/

/*输入一个算数表达式字符串，计算该表达式的结果返回,支持min,max,length这三个内置函数*/
int CalculateExpression(char *sExpression, int * nResult)
{
	char *sTmpExpression = NULL;
	
	/*如果输入的表达式为NULL或者长度为0则返回错误*/
	if(sExpression == NULL || strlen(sExpression) == 0)
	{
		PrintLog(ERROR, "算数表达式不能为空或NULL");	
		return -1;
	}
	
	PrintLog(TRACE, "输入的表达式为[%s]", sExpression);
	
	/*分配动态内存空间*/
	sTmpExpression = malloc(strlen(sExpression)+100);
	memset(sTmpExpression, 0, strlen(sExpression)+100);
	
	/*替换内置函数*/	
	if(ReplaceFunction(sExpression, sTmpExpression)) 
	{
		PrintLog(ERROR, "替换内置函数出错");
		free(sTmpExpression);
		return -1;	
	}
	
	PrintLog(TRACE, "替换内置函数后,表达式为[%s]", sTmpExpression);
	
	/*检查表达式的合法性*/
	if(CheckExpression(sTmpExpression))
	{
		PrintLog(ERROR, "表达式非法");
		free(sTmpExpression);
		return -1;	
	}
	
	/*将中缀表达式变为后缀表达式*/
	if(TranInfixToSufixx(sTmpExpression))
	{
		PrintLog(ERROR, "将中缀表达式转换为后缀表达式错误");
		free(sTmpExpression);
		return -1;	
	}
	
	PrintLog(TRACE, "将中表达式变为后缀表达式后为[%s]", sTmpExpression);
	/*计算后缀表达式的值*/
	if(CalculateSufixx(sTmpExpression, nResult))
	{
			PrintLog(ERROR, "计算后缀表达式的值错误");
			free(sTmpExpression);
			return -1;
	}
	
	PrintLog(TRACE, "计算表达式的结果为[%d]", *nResult);
	
	/*释放动态内存空间*/
	free(sTmpExpression);
	
	return 0;
}


/*替换内置函数max(1,2,...), min(1,2,...), length("...")等*/
static int ReplaceFunction(char *sExpression, char *sTmpExpression)
{
	int nTmp, nNum, nLen, i=0;
	char *sStart =NULL, *sEnd = NULL, *sTmp = NULL, *sTmpStr = NULL, sFmtStr[10], sContentBuf[1024], sTmpBuf[20];
	if(sExpression == NULL || sTmpExpression == NULL) return -1;
	strcpy(sTmpExpression, sExpression);
	nLen = strlen(sTmpExpression)+100;
	sTmpStr = malloc(nLen);                 
	
	/*替换内置函数*/
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
				PrintLog(ERROR, "%s函数用法错误,缺失左括号", sFmtStr);
				free(sTmpStr); 			
				return -1;
			}
			sEnd = strchr(sStart, ')');
			if(sEnd == NULL) 
			{
				PrintLog(ERROR, "%s函数用法错误,缺失右括号", sFmtStr);
				free(sTmpStr); 			
				return -1;
			}
			
			/*内置函数右括号的右边只能是右括号或者运算符 add by dc 20150113*/
			if(*(sEnd+1) != ')' && IsMathSymbol(*(sEnd+1)) != 0 && sEnd != sTmpExpression+strlen(sTmpExpression)-1)
			{
				PrintLog(ERROR, "%s函数用法错误,右括号的右边只能是右括号，或者运算符", sFmtStr);
				free(sTmpStr); 			
				return -1;				
			}
			
			memcpy(sContentBuf, sStart, sEnd-sStart);
			if(CalFunListBuf[i].func(sContentBuf, &nNum)) 
			{
				PrintLog(ERROR, "计算内置函数[%s]的值出错", CalFunListBuf[i].sFunDesc);
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
	
//	/*替换length函数, length函数必须最先替换, 否则其字符串中如果包含min max等子串会报错*/
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
//			PrintLog(ERROR, "%s函数用法错误,缺失左括号", sFmtStr);
//			free(sTmpStr); 			
//			return -1;
//		}
//		sEnd = strchr(sStart, ')');
//		if(sEnd == NULL) 
//		{
//			PrintLog(ERROR, "%s函数用法错误,缺失右括号", sFmtStr);
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
	
//	PrintLog(TRACE, "替换length后[%s]", sTmpExpression);		
	
//	/*替换min函数*/
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
//			PrintLog(ERROR, "%s函数用法错误,缺失左括号", sFmtStr);
//			free(sTmpStr); 
//			return -1;
//		}
//		sEnd = strchr(sStart, ')');
//		if(sEnd == NULL) 
//		{
//			PrintLog(ERROR, "%s函数用法错误,缺失右括号", sFmtStr);
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
////	PrintLog(TRACE, "替换min后[%s]", sTmpExpression);	
//	
//	/*替换max函数*/
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
//	//PrintLog(TRACE, "替换max后[%s]", sTmpExpression);
	
	/*释放动态空间*/
	free(sTmpStr);
	
	return 0;
}
static int CheckExpression(char *sTmpExpression)
{
	int nLeftBraNum = 0, nRightBraNum = 0, i;
	/*字符的合法性*/
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
					/*左括号的左边只能是运算符或左括号*/
					if(IsMathSymbol(sTmpExpression[i-1]) != 0 && sTmpExpression[i-1] != '(') 
					{
						PrintLog(ERROR, "左括号的左边只能是运算符或左括号,不能为[%c]", sTmpExpression[i-1]);
						return -1;
					}
					/*左括号的右边只能是数字或者左括号*/
					if((!isdigit(sTmpExpression[i+1])) && sTmpExpression[i+1] !='(') 
					{
						PrintLog(ERROR, "左括号的右边只能是数字或者左括号,不能为[%c]", sTmpExpression[i+1]);
						return -1;
					}
				}
				nLeftBraNum++;
			}
			else if(sTmpExpression[i] == ')') 
			{
				if(i<strlen(sTmpExpression)-1)
				{
					/*右括号的右边只能是运算符或右括号*/
					if(IsMathSymbol(sTmpExpression[i+1]) != 0 && sTmpExpression[i+1] != ')') 
					{
						PrintLog(ERROR, "右括号的右边只能是运算符或右括号,不能为[%c]", sTmpExpression[i+1]);	
						return -1;
					}
					/*右括号的左边只能是数字或者右括号*/
					if((!isdigit(sTmpExpression[i-1])) && sTmpExpression[i-1] !=')') 
					{
						PrintLog(ERROR, "右括号的左边只能是数字或者右括号,不能为[%c]", sTmpExpression[i-1]);
						return -1;
					}
				}
				nRightBraNum++;
			}
	}
	
	/*左右括号必须对称*/
	if(nLeftBraNum != nRightBraNum) 
	{
		PrintLog(ERROR, "左右括号不对称");
		return -1;
	}
	return 0;
}


/*遍历中缀表达式,如果是数字，则直接入输出栈，如果是小括号则直接入符号栈，如果是右括号则符号栈中所有符号出栈
,出栈的符号入输出栈,直到遇到左括号(左括号也出栈，但是不入输出栈)
#如果是运算符，则如果符号栈不为空且优先级比栈顶运算符优先级高,则栈顶符号出栈入输出栈
重复#操作直到该符号优先级比栈顶符号优先级低，或栈为空，则该符号入符号栈
当字符串遍历完之后，将所有符号栈中的符号弹入输出栈*/
static int TranInfixToSufixx(char *sTmpExpression)
{
	/*定义变量*/
	Stack StackBuf, StackBufOut;
	int i;
	char sTmpBuf[1024], sTmpBuf1[2];
	memset(sTmpBuf, 0, sizeof(sTmpBuf));
//	memset(&StackBuf, 0, sizeof(StackBuf));
//	memset(&StackBufOut, 0, sizeof(StackBufOut));
	
	/*初始化栈*/
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
	
	/*算数符号栈中的元素全部弹出到输出栈*/
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
	/*将输出栈的数据全部弹出*/
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

	/*释放栈*/
	FreeStack(&StackBuf);
	FreeStack(&StackBufOut);
		
	return 0;
}

/*遍历后缀表达式,如果是数字,则入栈,
如果是算数符号, 则取栈顶两个数字,进行运算,并将结果入栈,重复以上操作
最后取出栈顶元素则为结果*/
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
						PrintLog(ERROR, "表达式非法, 除数不能为零");
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
	
	/*释放栈*/
	FreeStack(&StackBuf);
	
	return 0;
	
}


/*计算最小值*/
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
		  	PrintLog(ERROR, "min函数使用方法错误,逗号间缺失内容"); 
		  	return -1;
		  }
		  if(IsAllDigit(sNumStr)!=0) 
		  {
		  	PrintLog(ERROR, "min函数使用方法错误,逗号间不能含有非数字字符");
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
		PrintLog(ERROR, "min函数使用方法错误, 左右括号间缺失内容"); 
		return -1;
	}
	*nResult = nNum;
	return 0;	
}


/*计算最大值*/
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
		  	PrintLog(ERROR, "max函数使用方法错误,逗号间缺失内容"); 
		  	return -1;
		  }
		  if(IsAllDigit(sNumStr)!=0) 
		  {
		  	PrintLog(ERROR, "max函数使用方法错误,逗号间不能含有非数字字符");
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
		PrintLog(ERROR, "max函数使用方法错误, 左右括号间缺失内容"); 
		return -1;
	}
	*nResult = nNum;
	return 0;
}

/*计算长度*/
int DealLength(char *sContent, int *nResult)
{
	int nNum = 0, nTmpNum = 0, i=0;
	char sTmpBuf[1024];
	memset(sTmpBuf, 0, sizeof(sTmpBuf));

	if(*sContent != '\"') 
	{
		PrintLog(ERROR, "length函数使用方法错误, 缺失左双引号");
		return -1;
	}
	if(*(sContent+strlen(sContent)-1) != '\"') 
	{
		PrintLog(ERROR, "length函数使用方法错误, 缺失右双引号");
		return -1;
	}
	memcpy(sTmpBuf, sContent+1, strlen(sContent)-2);
	
	if(IsAllChar(sTmpBuf) != 0 ) 
	{
		PrintLog(ERROR, "length函数使用方法错误, 双引号间含有非半角字符");
		return -1;
	}	
	*nResult = strlen(sTmpBuf);
	
	return 0;	
}


/*计算平均值*/
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
		  	PrintLog(ERROR, "avg函数使用方法错误,逗号间缺失内容"); 
		  	return -1;
		  }
		  if(IsAllDigit(sNumStr)!=0) 
		  {
		  	PrintLog(ERROR, "avg函数使用方法错误,逗号间不能含有非数字字符");
		  	return -1;
		  }
		  nTotalNum += atoi(sNumStr);
		  nCount++;
		  sPos = sEnd;		  		  
	}
	if(nCount == 0 ) 
	{
		PrintLog(ERROR, "avg函数使用方法错误, 左右括号间缺失内容"); 
		return -1;
	}
	
	nNum = nTotalNum/nCount;	
	*nResult = nNum;
	return 0;
}


