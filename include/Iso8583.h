/*
*���ܣ�����8583���ĵ���ع��ߺ���
*���ߣ�����
*ʱ�䣺2015��11��9��
*/

#ifndef _ISO8583_H_
#define _ISO8583_H_
#include "public.h"

#define FileMaxLen 999

//����8583���һЩ�������
typedef struct FiledDefine
{
	short nLenType;			//�����ͣ���ö������LenType
	short nLenCodeMode;     //����뷽ʽ����ö������CodeMode
	short nFieldCodeMode;   //����뷽ʽ����ö������CodeMode
	short nFieldLen;        //�򳤶�(�䳤��Ϊ��󳤶�)
	short nActualLen;       //ʵ�ʳ���
	short nFieldFormat;     //�����ݸ�ʽ: ����������FileFormat
	char sFieldBuf[999+1];  //������	
}FiledDefine;

//����8583���Ľṹ
typedef struct
{
    char     sTpdu[10+1];           //Tpdu
    char     sMsgHeader[12+1];      //����ͷ
    char     sMsgType[4+1];         //��Ϣ����  
    char     sBitMap[64+1];         //λͼ
    short    nTpduOffset;           //tpduƫ����
    short    nMsgHeadOffset;        //����ͷƫ����
    short    nMsgTypeOffset;        //��Ϣ����ƫ����
    short    nBitMapOffset;         //λͼƫ����
    short    nMsgBodyOffset;        //��������ƫ����
    struct FiledDefine FieldInfoBuf[64];        
}Iso8583;

//���峤������
enum LenType
{
    FIXED_LEN=1,            //�̶���
    TWO_CHANGE_LEN,         //��λ�߳�
    THREE_CHANGE_LEN        //��λ�䳤        
};

//������뷽ʽ
enum CodeMode
{
    ASCII=1,                //ASCII��
    LEFT_BCD,               //��BCD��
    RIGHT_BCD               //�ҿ�BCD��
        
};

//���ݸ�ʽ
enum FieldFormat
{
    DATE=1,                 //����yyyymm
    TIME                    //ʱ��HHMMSS
        
};


//��������
int GetBitMap(unsigned char *, short, char *);
int SetBitMap(unsigned char *, short);
int UnPackIso8583(unsigned char *, short, Iso8583 *);
int PackageIso8583();
void ShowIso8583(Iso8583 *);

#endif
