/*
*���ܣ�һЩTCPͨѶ��ص�ͨ�ú���
*���ߣ�����
*ʱ�䣺2015��11��3��
*/

#include "public.h"
#include "Tcp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
*��������  CreateSever
*��  �ܣ�  ����TCP�����
*��  �Σ�  inPort->����˼����Ķ˿�
*����ֵ��  ����0->�ɹ� -1->ʧ��
*˵  ��:
*/
int CreateSever(int inPort)
{
    int inFd, inOptVal, inRt;
    struct sockaddr_in SeverSockAddr;
    
    //����socket
    inFd = socket(AF_INET, SOCK_STREAM, 0);
    if(inFd == -1)
    {
        PrintLog(ERROR, "socket error[%d] errmsg[%s]", errno, strerror(errno));    
        return -1;
    }
    
    //����socketѡ��, socket����������
    inOptVal = 1;
    inRt = setsockopt(inFd, SOL_SOCKET, SO_REUSEADDR, &inOptVal, sizeof(inOptVal));
    if(inRt == -1)
    {
        PrintLog(ERROR, "setsockopt SO_REUSEADDR error[%d] errmsg[%s]", errno, strerror(errno));
        CloseSocket(inFd);
        return -1;    
    }
    
    //�󶨶˿�
    memset(&SeverSockAddr, 0, sizeof(SeverSockAddr));
    SeverSockAddr.sin_family = AF_INET;     
    SeverSockAddr.sin_port = htons(inPort);  
    SeverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);   
    inRt = bind(inFd, (struct sockaddr *)&SeverSockAddr, sizeof(SeverSockAddr));
    if(inRt == -1)
    {
        PrintLog(ERROR, "bind error[%d] errmsg[%s]", errno, strerror(errno));
        CloseSocket(inFd);
        return -1;    
    }    
    
    //�����˿�
    inRt = listen(inFd, 50);
    if(inRt == -1)
    {
        PrintLog(ERROR, "listen error[%d] errmsg[%s]", errno, strerror(errno));
        CloseSocket(inFd);
        return -1;    
    }     
    
    return inFd;        
}

/*
*��������  AcceptSocket
*��  �ܣ�  ��������
*��  �Σ�  inFd->socket�׽���
*����ֵ��  ����0->�ɹ� -1->ʧ��
*˵  ��:
*/
int AcceptSocket(int inFd)
{
    struct sockaddr_in SockAddrClient;
    int inLen, inAcceptId;
    char sIpString[20];	
    
    memset(&SockAddrClient, 0, sizeof(SockAddrClient));
    while(1)
    {
        inLen = 0;
        inAcceptId = accept(inFd, (struct sockaddr *)&SockAddrClient, &inLen);
        if(inAcceptId != -1)
        {
            break;
        }
        else
        {
            if(errno != EINTR)
            {
                PrintLog(ERROR, "accept error[%d] errmsg[%s]", errno, strerror(errno));
                return -1;              
            }
        }
    }      
    
    memset(sIpString, 0, sizeof(sIpString));
    strcpy(sIpString, inet_ntoa(SockAddrClient.sin_addr));
    PrintLog(TRACE, "accept from IP[%s]", sIpString);
    return inAcceptId;
}

/*
*��������  ConnectToSever
*��  �ܣ�  ���ӵ������
*��  �Σ�  sIpString->�����IP��ַ��, inPort->����˼����˿�
*����ֵ��  ����0->�ɹ� -1->ʧ��
*˵  ��:
*/
int ConnectToSever(char *sIpString, int inPort)
{
    int inFd, inRt;
    struct sockaddr_in SeverSockAddr;       
    
    //����socket
    inFd = socket(AF_INET, SOCK_STREAM, 0);
    if(inFd == -1)
    {
        PrintLog(ERROR, "socket error[%d] errmsg[%s]", errno, strerror(errno));    
        return -1;
    } 
    
    //���÷������˵�IP��ַ���˿ڡ���ַ��
    memset(&SeverSockAddr, 0, sizeof(SeverSockAddr));
    SeverSockAddr.sin_family      = AF_INET; 
    SeverSockAddr.sin_addr.s_addr = inet_addr(sIpString);  
    SeverSockAddr.sin_port        = htons(inPort);
    
    //���ӵ���������
    inRt = connect(inFd, (struct sockaddr *)&SeverSockAddr, sizeof(SeverSockAddr));\
    if(inRt == -1)
    {
        PrintLog(ERROR, "connect error[%d] errmsg[%s]", errno, strerror(errno));
        CloseSocket(inFd);
        return -1;                 
    }
    
    return inFd;
}


/*
*��������  ReadSockect
*��  �ܣ�  ��socket��ȡ������
*��  �Σ�  inFd->socket�׽��� inLen->��ȡ���ֽ���
*��  ��:   sDataBuf->��ȡ���ݵĴ�Ż���
*����ֵ��  ʵ�ʶ�ȡ���ֽ���
*˵  ��:
*/
int ReadSocket(int inFd, unsigned char *sDataBuf, int inLen)
{
    int inLeft, inOffset, nReadNum;
     
    inLeft = inLen;
    inOffset = 0;
    while(inLeft > 0)
    {
        nReadNum = read(inFd, sDataBuf+inOffset, inLeft);    
        if(nReadNum == -1)
        {
            PrintLog(ERROR, "read error[%d] errmsg[%s]", errno, strerror(errno));
            break;
        }         
        inOffset += nReadNum;
        inLeft  -= nReadNum;
    } 
    
    return (inLen-inLeft);      
}


/*
*��������  WriteSocket
*��  �ܣ�  ��socketдȡ������
*��  �Σ�  inFd->socket�׽��� inLen->��Ҫд�����ݵ��ֽ���
*��  ��:   sDataBuf->д�����ݵĴ�Ż���
*����ֵ��  ʵ��д����ֽ���
*˵  ��:
*/
int WriteSocket(int inFd, unsigned char *sDataBuf, int inLen)
{
    int inLeft, inOffset, nWriteum;
     
    inLeft = inLen;
    inOffset = 0;
    while(inLeft > 0)
    {
        nWriteum = write(inFd, sDataBuf+inOffset, inLeft);    
        if(nWriteum == -1)
        {
            PrintLog(ERROR, "read error[%d] errmsg[%s]", errno, strerror(errno));
            break;
        }         
        inOffset += nWriteum;
        inLeft  -= nWriteum;
    } 
    
    return (inLen-inLeft);      
}

/*
*��������  CloseSocket
*��  �ܣ�  �ر�socket�׽���
*��  �Σ�  inFd->socket�׽��� 
*˵  ��:
*/
void CloseSocket(int inFd)
{
    if(inFd >0) close(inFd);    
}



