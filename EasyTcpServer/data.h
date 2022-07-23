#pragma once
enum CMD {
	CMD_LOGIN,
	CMD_LOGOUT,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR,
};

struct DataHeader {
	short cmd;
	short dataLength;
};
//	DataPackage
struct Login : public DataHeader
{
	Login() {
		cmd = CMD_LOGIN;
		dataLength = sizeof(Login);
	}
	char userName[32];
	char Password[32];
};
struct LoginResult : public DataHeader {
	LoginResult() {
		cmd = CMD_LOGIN_RESULT;
		dataLength = sizeof(LoginResult);
		result = false;
	}
	bool result;
};
struct Logout : public DataHeader {
	Logout() {
		cmd = CMD_LOGOUT;
		dataLength = sizeof(Logout);
	}
	char userName[32];
};
struct LogoutResult : public DataHeader {
	LogoutResult() {
		cmd = CMD_LOGOUT_RESULT;
		dataLength = sizeof(LogoutResult);
		result = false;
	}
	bool result;
};