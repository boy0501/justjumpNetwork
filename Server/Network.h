#pragma once

#include <winsock2.h>

extern std::array<class Client*, 3> CLIENTS;
extern int Cnt_Player;
extern int robby_cnt;

extern HANDLE Client0Event, Client0SceneChangeTrigger, Client0SceneChangeIsDone;
extern HANDLE Client1Event, Client1SceneChangeTrigger, Client1SceneChangeIsDone;
extern HANDLE Client2Event, Client2SceneChangeTrigger, Client2SceneChangeIsDone;

void error_display(int err_no);
class Network
{
public:
	SOCKADDR_IN server_addr;
	SOCKET s_socket;
	WSADATA WSAData;
	static class Network* mNetwork;
	Network();
	~Network();

	static class Network* GetNetwork();
	void InitServer();
	SOCKET AcceptClient(SOCKADDR_IN& out_addr);
};

