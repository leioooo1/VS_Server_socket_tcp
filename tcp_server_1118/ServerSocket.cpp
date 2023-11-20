#include "stdafx.h"
#include "ServerSocket.h"
#include <thread>
struct Data {
	int type;
	int len;
	char da[1000];
};
ServerSocket::ServerSocket()
{
	if (initSocket()) {
		cout << "init socket ok" << endl;
	}
	else {
		cout << "init socket fail" << endl;
	}
}

ServerSocket::~ServerSocket()
{
	closesocket(m_server); //关闭服务端套接字
	WSACleanup();//关闭网络环境
}

bool ServerSocket::initSocket()
{
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa); //启动网络环境
	//SOCK_STREAM  表示TCP协议
	//SOCK_DGRAM   表示UDP协议
	m_server = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);//大小端转换，大端转小端：主机字节序转网络字节序，按short来转换
	addr.sin_addr.S_un.S_addr = 0; //0为本机任意ip

	int ret_bind = ::bind(m_server, (sockaddr*)&addr, sizeof(addr));   
	if (ret_bind) {
		cout << "bind fail" << WSAGetLastError() << endl;
		return false;
	}
	int ret_listen = listen(m_server, 5); //如果并发量较大，将5改成100
	if (ret_listen) {
		cout << "listen fail" << WSAGetLastError() << endl;
		return false;
	}
	//初始化成功
	//启动accept线程
	thread accept_thread(&ServerSocket::acceptThread, this);
	accept_thread.detach(); //防止异常结束

	//启动心跳检测线程
	thread heart_cheak_thread(&ServerSocket::heartCheckThread, this);
	heart_cheak_thread.detach();
	return true;
}

void ServerSocket::acceptThread()
{
	while (true) {
		SOCKET client;
		SOCKADDR_IN addr;
		int len = sizeof(addr);
		cout << "等待客户端连接..." << endl;
		client = accept(m_server, (sockaddr*)&addr, &len);
		cout <<"client:" << client << endl;
		if (client == SOCKET_ERROR) {
			cout << "socket异常" << WSAGetLastError() <<endl;
			break;
		}
		else if (client == 0) {
			cout<<"socket已关闭"<< WSAGetLastError() << endl;
			break;
		}
		//正常
		//启动收发包线程，启动长连接
		thread recv_send_thread(&ServerSocket::recvAndSendThread, this, client);
		recv_send_thread.detach();
	}
}

void ServerSocket::recvAndSendThread(SOCKET client)
{
	/*
	while (true)
	{
		char buffer[1024] = { 0 };
		int len_recv = recv(client, buffer, sizeof(buffer), 0);
		cout << "len_recv:" << len_recv << endl;
		if (len_recv <= 0) {
			cout << "socket异常了：" << WSAGetLastError << endl;
			break;
		}
		cout << "buffer:" << buffer << endl;
		send(client, buffer, len_recv, 0);
	}
	//关闭套接字
	closesocket(client);
	*/

	while (true)
	{
		char buffer[1024] = { 0 };
		int len_recv = recv(client, buffer, sizeof(buffer), 0);
		//cout << "len_recv:" << len_recv << endl;
		if (len_recv <= 0) {
			cout << "socket异常了：" << WSAGetLastError << endl;
			break;
		}
		
		
		int type = *(int *)buffer;
		Data * d = (Data*)buffer;
		if (type == 100) {		
			cout << d->da<<endl;
		}
		//cout << "buffer:" << buffer << endl;
		send(client, buffer, len_recv, 0);  
	}
	//关闭套接字
	closesocket(client);


}

void ServerSocket::heartCheckThread()
{

}
