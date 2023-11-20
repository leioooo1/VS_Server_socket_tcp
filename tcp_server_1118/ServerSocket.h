#pragma once
#include <WinSock2.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")  //���ؾ�̬��

class ServerSocket
{
public:
	ServerSocket();
	~ServerSocket();
	bool initSocket(); //��ʼ���������
	void acceptThread(); //accept�̺߳���
	void recvAndSendThread(SOCKET client);//�շ����߳�
	void heartCheckThread();//��Գ����ӵ��������

private:
	SOCKET m_server;
};

