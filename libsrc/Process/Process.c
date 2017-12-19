/**************************************************************************************************
*���ܣ�ʵ�ֽ�����ص�ͨ�ú���
*���ߣ�����
*ʱ�䣺2014-06-30
*�汾: V1.0
***************************************************************************************************/

#include "public.h"
#include "Process.h"

/*�����ػ�����*/
void CreateDaemon()
{
	int i, nFd;
	
	/*��������INIT���̣�˵���Ѿ����ػ�����*/	
	if(getppid() == 1)	return;		
		
	/*����һ���µĽ�����*/
	if( fork() != 0 )	exit(0);
	setsid();
		
	/*����һ���µĽ���*/		
	if( fork() != 0 ) exit(0);
		
	/*�ر������ļ�������*/
	for(i=0; i<getdtablesize(); i++)
	close(i);
	
	/*����׼IO�ض��򵽿��豸�ļ�*/
	nFd = open("/dev/null", O_RDWR);
	dup(nFd);
	dup(nFd);
	
	/*�����½��ļ���Ȩ��*/
	umask(022);
	
	/*�����ź�*/
	//signal(SIGTERM, DealSignal);
	
}

/*�����ź�*/
void DealSignal(int nSignal)
{
	switch(nSignal)
	{
		case SIGTERM:
			printf("��׽��SIGTERM�źţ��˳�����\n");
			exit(0);
		default:
			break;
	}
}
