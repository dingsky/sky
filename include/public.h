/*
    ͨ��ͷ�ļ�,��Ҫ����һЩ���õ�ͷ�ļ����á�ͨ�ú����Ķ��塢ͨ��ȫ�ֱ����Ķ���
    ������2015��10��30��
*/
#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

//һЩͨ�õĺ궨��
#define NTrue   1
#define NFalse  0

//Time.c
#define TIMEONLY 0
#define DATEONLY 1
#define DATETIME 2
#define ENGWEEKDAY 3
#define CHNWEEKDAY 4
void GetDateTime(char *, char *, char *, char *, char *, char *, char *, char *, int, short);
void GetToday(char *);
void GetTomorrow(char *);
void GetYesterday(char *);
void GetSomeDaysAgo(char *, int);
void GetSomeDaysLater(char *, int);
void GetNowDateTime(char *);
void GetNowTime(char *);
void GetNowDateTimeFormat1(char *);
void GetNowDateTimeFormat2(char *);
void GetNowTimeFormat1(char *);
void GetNowTimeFormat2(char *);

//StringOpt.c
#define RIGHT_FILL 0
#define LEFT_FILL  1
void TransUpperToLower(char *);          
void TransLowerToUpper(char *);
void ReverseString(char *);
void TurnRoundString(char *);
short IsUpperString(char *);
short IsLowerString(char *);
short IsCharString(char *);
short IsDigitString(char *);
void TrimString(char *);
void LTrimString(char *);
void RTrimString(char *);
void TrimNewLine(char *);
void FillString(char *, char , short, int);
void LTrimZero(char *);
void RemoveComments(char *, char *);
void TransAmtToUpper(char *);
void AppendString(char *, char *); //��һ���ַ���׷�ӵ���һ���ַ����Ŀ�ͷ
int TransCharToDigit(char);
void TransAscToBcd(char *, unsigned char *);
void TransBcdToAsc(unsigned char *, int, char *);

//PrintLog.c
char sLogFileName[50+1];		
void PrintLog(char *, int, int, char *, ...);
#define LOG_BUF_MAX_LEN 3000
#define LOG_ERR   0
#define LOG_TRACE 1
#define LOG_WARN  2
#define ERROR __FILE__, __LINE__, LOG_ERR 
#define TRACE __FILE__, __LINE__, LOG_TRACE 
#define WARN  __FILE__, __LINE__, LOG_WARN 
void PrintHexDebug(unsigned char *, short);



#endif
