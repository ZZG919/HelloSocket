//�ַ�������strcpy
#define  _CRT_SECURE_NO_WARNINGS
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
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock)
	{
		std::cout << "error,����SOCKETʧ��" << std::endl;
	}

	//2�����ӷ����� connect
	sockaddr_in _sin = {};
	//ʹ��ip4Э��
	_sin.sin_family = AF_INET;
	//ʹ��4567�˿�
	_sin.sin_port = htons(4567); //htons����ת��Ϊ����Ķ˿�����
	//�󶨷�����������ip��ַ
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int IsConnect = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == IsConnect) {
		std::cout << "�������ӷ�����ʧ��" << std::endl;
	}
	else {
		std::cout << "����Socket�ɹ�" << std::endl;
	}

	char cmdBuf[128] = {};
	while (true) {
		//3����������ָ��
		std::cin >> cmdBuf;
		//4����������ָ��
		if (strcmp(cmdBuf, "exit") == 0) break;
		else if (strcmp(cmdBuf, "login") == 0) {
			//5����������
			Login  login;
			strcpy(login.userName,"user_zzg");
			strcpy(login.Password,"132412342");
			send(_sock, (char*)&login,sizeof(login), 0);
			//6�����շ��������ص�����
			LoginResult loginret = {};
			recv(_sock, (char*)&loginret, sizeof(LoginResult), 0);
			std::cout << "LoginResult: " << loginret.result << std::endl;
		}
		else if (strcmp(cmdBuf, "logout") == 0) {
			//5����������
			Logout  logout;
			strcpy(logout.userName, "user_zzg_logout");
			send(_sock, (char*)&logout, sizeof(logout), 0);
			//6�����շ��������ص�����
			LogoutResult logoutret = {};
			recv(_sock, (char*)&logoutret, sizeof(LogoutResult), 0);
			std::cout << "LogoutResult: " << logoutret.result << std::endl;
		}
		else {
			std::cout << "��֧���������������" << std::endl;
		}
	}

	//7���ر��׽���closesocket
	closesocket(_sock);
	//Ӧ�ó�������ɶ������Socket���ʹ�ú�Ҫ����WSACleanup�����������Socket��İ󶨲����ͷ�Socket����ռ�õ�ϵͳ��Դ��
	WSACleanup();
	system("pause");
	return 0;
}