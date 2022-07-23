
//����windows��winsock2ͷ�ļ������ں����ĳ�ͻ
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>

#include "data.h"
// ���socket������
#pragma comment(lib,"ws2_32.lib")

int main() {
	//ʹ��2�汾��socket, wVersionΪ�㽫ʹ�õ�Winsock�汾�ţ�wHighVersionΪ�����Winsock��̬��֧�ֵ���߰汾��ע�⣬���ǵĸ��ֽڴ���ΰ汾�����ֽڴ������汾��
	WORD ver = MAKEWORD(2, 2);  //����ΪwVersion,wHighVersion
	//��ʼ��socket��Ϣ
	WSADATA dat;
	//���ص�ǰ�汾��socket
	WSAStartup(ver, &dat);

	//1������һ��socket
	//Э�飬���ͣ�����ֵ
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//2��bind �����ڽ��ܿͻ������ӵ�����˿�
	sockaddr_in _sin = {};
	//ʹ��ip4Э��
	_sin.sin_family = AF_INET;
	//ʹ��4567�˿�
	_sin.sin_port = htons(4567); //htons����ת��Ϊ����Ķ˿�����
	//�󶨱��ص�����ip��ַ
	_sin.sin_addr.S_un.S_addr = INADDR_ANY; //inet_addr("127.0.0.1");
	if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == SOCKET_ERROR)
	{
		std::cout << "Error,�����ڽ��ܿͻ������ӵ�����˿�ʧ��" << std::endl;
	}
	else { std::cout << "������˿ڳɹ���" << std::endl; }

	// 3��listen ��������˿�
	if (listen(_sock, 5) != SOCKET_ERROR)
	{
		std::cout << "��������˿ڳɹ�" << std::endl;
	}
	else {
		std::cout << "��������˿�ʧ��" << std::endl;
	}
	//4 accept�ȴ����ܿͻ�������
	sockaddr_in clientAddr = {};
	int nAddrlen = sizeof(clientAddr);
	SOCKET _cSock = INVALID_SOCKET;
	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrlen);
	if (_cSock == INVALID_SOCKET)
	{
		std::cout << "���󣬽��ܵ���Ч�ͻ���SOCKET..." << std::endl;
	}
	std::cout << "�¿ͻ��˼��룺IP = " << inet_ntoa(clientAddr.sin_addr) << std::endl;

	char _recvBuf[128] = {};
	//5��ѭ�����տͻ�����Ϣ
	while (true)
	{
		DataHeader header = {};
		int nlen = recv(_cSock,(char*)&header,sizeof(header), 0);
		if (nlen <= 0)
		{
			std::cout << "�ͻ����Ѿ��˳����������" << std::endl;
			break;
		}
		switch (header.cmd)
		{
		case CMD_LOGIN:
		{
			Login login = {};
			recv(_cSock, (char*)&login+sizeof(DataHeader), sizeof(Login)-sizeof(DataHeader), 0);
			std::cout << "��¼����" << login.userName << "��¼����:" << login.Password << std::endl;
			// ����������֤���
			LoginResult ret;
			send(_cSock, (char*)&ret, sizeof(LoginResult), 0);
		}
		break;
		case CMD_LOGOUT:
		{
			Logout logout = {};
			recv(_cSock, (char*)&logout + sizeof(DataHeader), sizeof(Logout) - sizeof(DataHeader), 0);
			std::cout << "��¼����" << logout.userName << std::endl;
			// ����������֤���
			LogoutResult ret;
			send(_cSock, (char*)&ret, sizeof(LogoutResult), 0);
		}
		break;
		default:
			header.cmd = CMD_ERROR;
			header.dataLength = 0;
			send(_cSock, (char*)&header, sizeof(header), 0);
		}
	}

	//6���ر��׽���closesocket
	closesocket(_sock);
	//Ӧ�ó�������ɶ������Socket���ʹ�ú�Ҫ����WSACleanup�����������Socket��İ󶨲����ͷ�Socket����ռ�õ�ϵͳ��Դ��
	WSACleanup();
	system("pause");
	return 0;
}
