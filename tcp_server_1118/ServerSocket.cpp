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
	closesocket(m_server); //�رշ�����׽���
	WSACleanup();//�ر����绷��
}

bool ServerSocket::initSocket()
{
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa); //�������绷��
	//SOCK_STREAM  ��ʾTCPЭ��
	//SOCK_DGRAM   ��ʾUDPЭ��
	m_server = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);//��С��ת�������תС�ˣ������ֽ���ת�����ֽ��򣬰�short��ת��
	addr.sin_addr.S_un.S_addr = 0; //0Ϊ��������ip

	int ret_bind = ::bind(m_server, (sockaddr*)&addr, sizeof(addr));   
	if (ret_bind) {
		cout << "bind fail" << WSAGetLastError() << endl;
		return false;
	}
	int ret_listen = listen(m_server, 5); //����������ϴ󣬽�5�ĳ�100
	if (ret_listen) {
		cout << "listen fail" << WSAGetLastError() << endl;
		return false;
	}
	//��ʼ���ɹ�
	//����accept�߳�
	thread accept_thread(&ServerSocket::acceptThread, this);
	accept_thread.detach(); //��ֹ�쳣����

	//������������߳�
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
		cout << "�ȴ��ͻ�������..." << endl;
		client = accept(m_server, (sockaddr*)&addr, &len);
		cout <<"client:" << client << endl;
		if (client == SOCKET_ERROR) {
			cout << "socket�쳣" << WSAGetLastError() <<endl;
			break;
		}
		else if (client == 0) {
			cout<<"socket�ѹر�"<< WSAGetLastError() << endl;
			break;
		}
		//����
		//�����շ����̣߳�����������
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
			cout << "socket�쳣�ˣ�" << WSAGetLastError << endl;
			break;
		}
		cout << "buffer:" << buffer << endl;
		send(client, buffer, len_recv, 0);
	}
	//�ر��׽���
	closesocket(client);
	*/

	while (true)
	{
		char buffer[1024] = { 0 };
		int len_recv = recv(client, buffer, sizeof(buffer), 0);
		//cout << "len_recv:" << len_recv << endl;
		if (len_recv <= 0) {
			cout << "socket�쳣�ˣ�" << WSAGetLastError << endl;
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
	//�ر��׽���
	closesocket(client);


}

void ServerSocket::heartCheckThread()
{

}
