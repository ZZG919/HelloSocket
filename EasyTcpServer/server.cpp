
//避免windows和winsock2头文件中早期函数的冲突
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>

#include "data.h"
// 添加socket依赖包
#pragma comment(lib,"ws2_32.lib")

int main() {
	//使用2版本的socket, wVersion为你将使用的Winsock版本号，wHighVersion为载入的Winsock动态库支持的最高版本，注意，它们的高字节代表次版本，低字节代表主版本。
	WORD ver = MAKEWORD(2, 2);  //参数为wVersion,wHighVersion
	//初始化socket信息
	WSADATA dat;
	//加载当前版本的socket
	WSAStartup(ver, &dat);

	//1、建立一个socket
	//协议，类型，返回值
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//2、bind 绑定用于接受客户端连接的网络端口
	sockaddr_in _sin = {};
	//使用ip4协议
	_sin.sin_family = AF_INET;
	//使用4567端口
	_sin.sin_port = htons(4567); //htons用于转化为网络的端口类型
	//绑定本地的网络ip地址
	_sin.sin_addr.S_un.S_addr = INADDR_ANY; //inet_addr("127.0.0.1");
	if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == SOCKET_ERROR)
	{
		std::cout << "Error,绑定用于接受客户端连接的网络端口失败" << std::endl;
	}
	else { std::cout << "绑定网络端口成功。" << std::endl; }

	// 3、listen 监听网络端口
	if (listen(_sock, 5) != SOCKET_ERROR)
	{
		std::cout << "监听网络端口成功" << std::endl;
	}
	else {
		std::cout << "监听网络端口失败" << std::endl;
	}
	//4 accept等待接受客户端连接
	sockaddr_in clientAddr = {};
	int nAddrlen = sizeof(clientAddr);
	SOCKET _cSock = INVALID_SOCKET;
	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrlen);
	if (_cSock == INVALID_SOCKET)
	{
		std::cout << "错误，接受到无效客户端SOCKET..." << std::endl;
	}
	std::cout << "新客户端加入：IP = " << inet_ntoa(clientAddr.sin_addr) << std::endl;

	char _recvBuf[128] = {};
	//5、循环接收客户端消息
	while (true)
	{
		DataHeader header = {};
		int nlen = recv(_cSock,(char*)&header,sizeof(header), 0);
		if (nlen <= 0)
		{
			std::cout << "客户端已经退出，任务结束" << std::endl;
			break;
		}
		switch (header.cmd)
		{
		case CMD_LOGIN:
		{
			Login login = {};
			recv(_cSock, (char*)&login+sizeof(DataHeader), sizeof(Login)-sizeof(DataHeader), 0);
			std::cout << "登录名：" << login.userName << "登录密码:" << login.Password << std::endl;
			// 忽略密码验证情况
			LoginResult ret;
			send(_cSock, (char*)&ret, sizeof(LoginResult), 0);
		}
		break;
		case CMD_LOGOUT:
		{
			Logout logout = {};
			recv(_cSock, (char*)&logout + sizeof(DataHeader), sizeof(Logout) - sizeof(DataHeader), 0);
			std::cout << "登录名：" << logout.userName << std::endl;
			// 忽略密码验证情况
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

	//6、关闭套接字closesocket
	closesocket(_sock);
	//应用程序在完成对请求的Socket库的使用后，要调用WSACleanup函数来解除与Socket库的绑定并且释放Socket库所占用的系统资源。
	WSACleanup();
	system("pause");
	return 0;
}
