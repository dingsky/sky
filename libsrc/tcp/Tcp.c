/*
*功能：一些TCP通讯相关的通用函数
*作者：丁春
*时间：2015年11月3号
*/

#include "public.h"
#include "Tcp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
*函数名：  CreateSever
*功  能：  创建TCP服务端
*入  参：  inPort->服务端监听的端口
*返回值：  大于0->成功 -1->失败
*说  明:
*/
int CreateSever(int inPort)
{
    int inFd, inOptVal, inRt;
    struct sockaddr_in SeverSockAddr;
    
    //创建socket
    inFd = socket(AF_INET, SOCK_STREAM, 0);
    if(inFd == -1)
    {
        PrintLog(ERROR, "socket error[%d] errmsg[%s]", errno, strerror(errno));    
        return -1;
    }
    
    //设置socket选项, socket可立即重用
    inOptVal = 1;
    inRt = setsockopt(inFd, SOL_SOCKET, SO_REUSEADDR, &inOptVal, sizeof(inOptVal));
    if(inRt == -1)
    {
        PrintLog(ERROR, "setsockopt SO_REUSEADDR error[%d] errmsg[%s]", errno, strerror(errno));
        CloseSocket(inFd);
        return -1;    
    }
    
    //绑定端口
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
    
    //监听端口
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
*函数名：  AcceptSocket
*功  能：  接收请求
*入  参：  inFd->socket套接字
*返回值：  大于0->成功 -1->失败
*说  明:
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
*函数名：  ConnectToSever
*功  能：  连接到服务端
*入  参：  sIpString->服务端IP地址串, inPort->服务端监听端口
*返回值：  大于0->成功 -1->失败
*说  明:
*/
int ConnectToSever(char *sIpString, int inPort)
{
    int inFd, inRt;
    struct sockaddr_in SeverSockAddr;       
    
    //创建socket
    inFd = socket(AF_INET, SOCK_STREAM, 0);
    if(inFd == -1)
    {
        PrintLog(ERROR, "socket error[%d] errmsg[%s]", errno, strerror(errno));    
        return -1;
    } 
    
    //设置服务器端的IP地址、端口、地址族
    memset(&SeverSockAddr, 0, sizeof(SeverSockAddr));
    SeverSockAddr.sin_family      = AF_INET; 
    SeverSockAddr.sin_addr.s_addr = inet_addr(sIpString);  
    SeverSockAddr.sin_port        = htons(inPort);
    
    //连接到服务器端
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
*函数名：  ReadSockect
*功  能：  从socket读取数据流
*入  参：  inFd->socket套接字 inLen->读取的字节数
*出  参:   sDataBuf->读取数据的存放缓存
*返回值：  实际读取的字节数
*说  明:
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
*函数名：  WriteSocket
*功  能：  往socket写取数据流
*入  参：  inFd->socket套接字 inLen->需要写的数据的字节数
*出  参:   sDataBuf->写入数据的存放缓存
*返回值：  实际写入的字节数
*说  明:
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
*函数名：  CloseSocket
*功  能：  关闭socket套接字
*入  参：  inFd->socket套接字 
*说  明:
*/
void CloseSocket(int inFd)
{
    if(inFd >0) close(inFd);    
}



