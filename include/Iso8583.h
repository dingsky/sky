/*
*功能：包含8583报文的相关工具函数
*作者：丁春
*时间：2015年11月9日
*/

#ifndef _ISO8583_H_
#define _ISO8583_H_
#include "public.h"

#define FileMaxLen 999

//定义8583域的一些相关属性
typedef struct FiledDefine
{
	short nLenType;			//域类型：见枚举类型LenType
	short nLenCodeMode;     //域编码方式：见枚举类型CodeMode
	short nFieldCodeMode;   //域编码方式：见枚举类型CodeMode
	short nFieldLen;        //域长度(变长域为最大长度)
	short nActualLen;       //实际长度
	short nFieldFormat;     //域数据格式: 见美剧类型FileFormat
	char sFieldBuf[999+1];  //域内容	
}FiledDefine;

//定义8583报文结构
typedef struct
{
    char     sTpdu[10+1];           //Tpdu
    char     sMsgHeader[12+1];      //报文头
    char     sMsgType[4+1];         //消息类型  
    char     sBitMap[64+1];         //位图
    short    nTpduOffset;           //tpdu偏移量
    short    nMsgHeadOffset;        //报文头偏移量
    short    nMsgTypeOffset;        //消息类型偏移量
    short    nBitMapOffset;         //位图偏移量
    short    nMsgBodyOffset;        //报文内容偏移量
    struct FiledDefine FieldInfoBuf[64];        
}Iso8583;

//定义长度类型
enum LenType
{
    FIXED_LEN=1,            //固定长
    TWO_CHANGE_LEN,         //两位边长
    THREE_CHANGE_LEN        //三位变长        
};

//定义编码方式
enum CodeMode
{
    ASCII=1,                //ASCII码
    LEFT_BCD,               //左靠BCD码
    RIGHT_BCD               //右靠BCD码
        
};

//数据格式
enum FieldFormat
{
    DATE=1,                 //日期yyyymm
    TIME                    //时间HHMMSS
        
};


//函数声明
int GetBitMap(unsigned char *, short, char *);
int SetBitMap(unsigned char *, short);
int UnPackIso8583(unsigned char *, short, Iso8583 *);
int PackageIso8583();
void ShowIso8583(Iso8583 *);

#endif
