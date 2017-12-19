/**************************************************************************************************
*功能：实现进程相关的通用函数
*作者：丁春
*时间：2014-06-30
*版本: V1.0
***************************************************************************************************/

#include "public.h"
#include "Process.h"

/*创建守护进程*/
void CreateDaemon()
{
	int i, nFd;
	
	/*父进程是INIT进程，说明已经是守护进程*/	
	if(getppid() == 1)	return;		
		
	/*创建一个新的进程组*/
	if( fork() != 0 )	exit(0);
	setsid();
		
	/*创建一个新的进程*/		
	if( fork() != 0 ) exit(0);
		
	/*关闭所有文件描述符*/
	for(i=0; i<getdtablesize(); i++)
	close(i);
	
	/*将标准IO重定向到空设备文件*/
	nFd = open("/dev/null", O_RDWR);
	dup(nFd);
	dup(nFd);
	
	/*设置新建文件的权限*/
	umask(022);
	
	/*处理信号*/
	//signal(SIGTERM, DealSignal);
	
}

/*处理信号*/
void DealSignal(int nSignal)
{
	switch(nSignal)
	{
		case SIGTERM:
			printf("捕捉到SIGTERM信号，退出程序\n");
			exit(0);
		default:
			break;
	}
}
