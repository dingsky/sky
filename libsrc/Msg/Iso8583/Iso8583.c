/*
*功能：ISO8583报文的解析工具函数
*作者: 丁春
*时间：2015年11月16日
*说明：最初的版本先实现5字节的TPDU+6字节的报文头+2字节的消息类型+8字节的BITMAP+报文体
       暂时先处理64位的报文
*/

#include "Iso8583.h"
#include "public.h"

int UnPackIso8583(unsigned char *sMsgBuf, short nLen, Iso8583 *Iso8583Buf)
{
    short nOffset, nLix, nResultLen;
    unsigned char sTmpBuf[999+1], sTmpBuf1[999+1];
    
    if(sMsgBuf == NULL || nLen <= 0 || Iso8583Buf == NULL)
    {
        PrintLog(ERROR, "UnPack error, Unavailable params!");
        return -1;    
    }
    
    //tpdu
    nOffset = 0;
    if(nOffset+5 > nLen)
    {
        PrintLog(ERROR, "UnPack error, buffer overflow in tpdu!");
        return -1;                
    }
    memset(sTmpBuf, 0, sizeof(sTmpBuf));
    memcpy(sTmpBuf, sMsgBuf+nOffset, 5);
    TransBcdToAsc(sTmpBuf, 5, Iso8583Buf->sTpdu);
    nOffset += 5;
    PrintLog(TRACE, "sTpdu[%s]", Iso8583Buf->sTpdu);
    
    //msgheader
    if(nOffset+6 > nLen)
    {
        PrintLog(ERROR, "UnPack error, buffer overflow in msgheader!");
        return -1;                
    }        
    memset(sTmpBuf, 0, sizeof(sTmpBuf));    
    memcpy(sTmpBuf, sMsgBuf+nOffset, 6);
    TransBcdToAsc(sTmpBuf, 6, Iso8583Buf->sMsgHeader);
    nOffset += 6;
    PrintLog(TRACE, "sMsgHeader[%s]", Iso8583Buf->sMsgHeader);    
    
    //msgtype    
    if(nOffset+2 > nLen)
    {
        PrintLog(ERROR, "UnPack error, buffer overflow in msgtype!");
        return -1;                
    }         
    memset(sTmpBuf, 0, sizeof(sTmpBuf));    
    memcpy(sTmpBuf, sMsgBuf+nOffset, 2);
    TransBcdToAsc(sTmpBuf, 2, Iso8583Buf->sMsgType);
    nOffset += 2;    
    PrintLog(TRACE, "sMsgType[%s]", Iso8583Buf->sMsgType);    
    
    //bitmap
    if(nOffset+8 > nLen)
    {
        PrintLog(ERROR, "UnPack error, buffer overflow in bitmap!");
        return -1;                
    }         
    memset(sTmpBuf, 0, sizeof(sTmpBuf));    
    memcpy(sTmpBuf, sMsgBuf+nOffset, 8);
    if(GetBitMap(sTmpBuf, 8, Iso8583Buf->sBitMap) != 0)
    {
        
        PrintLog(ERROR, "UnPack error, GetBitMap error!");
        return -1;             
    }
    nOffset += 8;    
    PrintLog(TRACE, "sBitMap[%s]", Iso8583Buf->sBitMap);    
         
    
    for(nLix=1; nLix<64; nLix++)
    {        
        if(Iso8583Buf->sBitMap[nLix] == '0')   continue;
            
        //获取域长度    
        switch(Iso8583Buf->FieldInfoBuf[nLix].nLenType)
        {
            if(Iso8583Buf->FieldInfoBuf[nLix].nFieldLen <= 0)
            {
                PrintLog(ERROR, "UnPack error, Filed[%d] FiledLen[%d]  Unavailable!", nLix+1, Iso8583Buf->FieldInfoBuf[nLix].nFieldLen);
                return -1;                             
            }          
              
            case FIXED_LEN:
                Iso8583Buf->FieldInfoBuf[nLix].nActualLen = Iso8583Buf->FieldInfoBuf[nLix].nFieldLen;
                break;
            case TWO_CHANGE_LEN:
                if(nOffset+2 > nLen)
                {
                    PrintLog(ERROR, "UnPack error, buffer overflow in field[%d] len!", nLix);
                    return -1;                     
                }
                memset(sTmpBuf, 0, sizeof(sTmpBuf));
                memset(sTmpBuf1, 0, sizeof(sTmpBuf1));
                switch(Iso8583Buf->FieldInfoBuf[nLix].nLenCodeMode)
                {
                    case ASCII:
                        memcpy(sTmpBuf, sMsgBuf+nOffset, 2);    
                        nOffset += 2;      
                        break;
                    case LEFT_BCD:
                    case RIGHT_BCD:                        
                        memcpy(sTmpBuf, sMsgBuf+nOffset, 1);    
                        TransBcdToAsc(sTmpBuf, 1, sTmpBuf1);  
                        nOffset += 1;                           
                        break;
                    default:
                        PrintLog(ERROR, "UnPack error, unknow LenCodeMode[%d] in Field [%d]", Iso8583Buf->FieldInfoBuf[nLix].nLenCodeMode, nLix+1);
                        return -1;      
                }                              
                Iso8583Buf->FieldInfoBuf[nLix].nActualLen = atoi(sTmpBuf1);       
                break;
            case THREE_CHANGE_LEN:
                if(nOffset+3 > nLen)
                {
                    PrintLog(ERROR, "UnPack error, buffer overflow in field[%d] len!", nLix);
                    return -1;                     
                }
                switch(Iso8583Buf->FieldInfoBuf[nLix].nLenCodeMode)
                {
                    case ASCII:
                        memcpy(sTmpBuf, sMsgBuf+nOffset, 3);    
                        nOffset += 3;      
                        break;
                    case LEFT_BCD:
                    case RIGHT_BCD:                        
                        memcpy(sTmpBuf, sMsgBuf+nOffset, 2);    
                        TransBcdToAsc(sTmpBuf, 2, sTmpBuf1);  //遗留, 这里需要是左靠还是右靠的处理
                        nOffset += 2;                           
                        break;
                    default:
                        PrintLog(ERROR, "UnPack error, unknow LenCodeMode[%d] in Field [%d]", Iso8583Buf->FieldInfoBuf[nLix].nLenCodeMode, nLix+1);
                        return -1;      
                }                    
                Iso8583Buf->FieldInfoBuf[nLix].nActualLen = atoi(sTmpBuf1);                                  
                break;  
            default:
                PrintLog(ERROR, "UnPack error, unknow nLenType[%d] in Field [%d]", Iso8583Buf->FieldInfoBuf[nLix].nLenType, nLix+1);                
                return -1;  
        }    
        
        //获取域内容     
        if(nOffset + Iso8583Buf->FieldInfoBuf[nLix].nActualLen > nLen)
        {
            PrintLog(ERROR, "UnPack error, Filed[%d] nActualLen[%d]  Unavailable!", nLix+1, Iso8583Buf->FieldInfoBuf[nLix].nActualLen);
            return -1;                
        }
        
        switch(Iso8583Buf->FieldInfoBuf[nLix].nFieldCodeMode)   
        {
            case ASCII:
                memcpy(Iso8583Buf->FieldInfoBuf[nLix].sFieldBuf, sMsgBuf+nOffset, Iso8583Buf->FieldInfoBuf[nLix].nActualLen);    
                nOffset += Iso8583Buf->FieldInfoBuf[nLix].nActualLen;      
                break;
            case LEFT_BCD:
            case RIGHT_BCD:          
                nResultLen = Iso8583Buf->FieldInfoBuf[nLix].nActualLen/2+Iso8583Buf->FieldInfoBuf[nLix].nActualLen%2;              
                memcpy(sTmpBuf, sMsgBuf+nOffset, nResultLen);    
                TransBcdToAsc(sTmpBuf, nResultLen, Iso8583Buf->FieldInfoBuf[nLix].sFieldBuf);  //遗留, 这里需要是左靠还是右靠的处理
                nOffset += nResultLen;                           
                break;
            default:
                PrintLog(ERROR, "UnPack error, unknow nFieldCodeMode[%d] in Field [%d]", Iso8583Buf->FieldInfoBuf[nLix].nFieldCodeMode, nLix+1);
                return -1;                     
        }
    }
    
    return 0;    
}

int GetBitMap(unsigned char *sBitMap, short nLen, char *sBitMapList)
{
    short nLix, nLiy, nFlag, nCount;
    
    if(sBitMap == NULL || nLen <8 || sBitMapList == NULL) return -1;
    
    memset(sBitMapList, '0', 64);
    
    nCount = 0;
    for(nLix=0; nLix<8; nLix++)
    {
        nFlag = 0x80;
        for(nLiy=0; nLiy<8; nLiy++)
        {
            if(sBitMap[nLix] & nFlag)
            {
                sBitMapList[nCount] = '1';    
            }    
            nFlag >>= 1;
            nCount++;
        }        
    }
    return 0;    
}

void ShowIso8583(Iso8583 *Iso8583Buf)
{
    short nLix;
    
    if(Iso8583Buf == NULL) return;
        
    PrintLog(TRACE, "sTpdu[%s]", Iso8583Buf->sTpdu);
    PrintLog(TRACE, "sMsgHeader[%s]", Iso8583Buf->sMsgHeader);
    PrintLog(TRACE, "sMsgType[%s]",  Iso8583Buf->sMsgType);
    PrintLog(TRACE, "sBitMap[%s", Iso8583Buf->sBitMap);   
    
    for(nLix=1; nLix<64; nLix++)
    {
        if(Iso8583Buf->sBitMap[nLix] == '0') continue;
        PrintLog(TRACE, "nFiledNum[%d], nLenType[%d], nLenCodeMode[%d],nFieldCodeMode[%d], \
        nFieldLen[%d], nActualLen[%d] sFieldBuf[%s]", nLix+1, Iso8583Buf->FieldInfoBuf[nLix].nLenType, \
        Iso8583Buf->FieldInfoBuf[nLix].nLenCodeMode, Iso8583Buf->FieldInfoBuf[nLix].nFieldCodeMode, \
        Iso8583Buf->FieldInfoBuf[nLix].nFieldLen, Iso8583Buf->FieldInfoBuf[nLix].nActualLen, \
        Iso8583Buf->FieldInfoBuf[nLix].sFieldBuf);
    }
    
    return;    
}
