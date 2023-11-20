#pragma once
#include <WinSock2.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")  //加载静态库

class ServerSocket
{
public:
	ServerSocket();
	~ServerSocket();
	bool initSocket(); //初始化函数相关
	void acceptThread(); //accept线程函数
	void recvAndSendThread(SOCKET client);//收发包线程
	void heartCheckThread();//针对长连接的心跳检测

private:
	SOCKET m_server;
};

