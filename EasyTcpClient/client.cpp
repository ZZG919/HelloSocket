//避免windows和winsock2头文件中早期函数的冲突
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>

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
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock)
	{
		std::cout << "error,建立SOCKET失败" << std::endl;
	}

	//2、连接服务器 connect
	sockaddr_in _sin = {};
	//使用ip4协议
	_sin.sin_family = AF_INET;
	//使用4567端口
	_sin.sin_port = htons(4567); //htons用于转化为网络的端口类型
	//绑定服务器的网络ip地址
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int IsConnect = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == IsConnect) {
		std::cout << "错误，连接服务器失败" << std::endl;
	}
	else {
		std::cout << "建立Socket成功" << std::endl;
	}

	//3、接收服务器的信息 recv
	char recvBuf[256] = {};
	int nlen = recv(_sock, recvBuf, 256, 0);
	if (nlen > 0)
	{
		std::cout << "接收到的数据为：" << recvBuf << std::endl;
	}
	else {
		std::cout << "接收到数据失败" << std::endl;
	}
	//6、关闭套接字closesocket
	closesocket(_sock);
	//应用程序在完成对请求的Socket库的使用后，要调用WSACleanup函数来解除与Socket库的绑定并且释放Socket库所占用的系统资源。
	WSACleanup();

	system("pause");
	return 0;
}