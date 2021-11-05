
#include "Network.h"

#include "../../Protocol/protocol.h"

Network* Network::mNetwork = nullptr;

void error_display(int err_no)
{
	wchar_t* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	std::wcout << lpMsgBuf << std::endl;
	//while (true);
	LocalFree(lpMsgBuf);
}
Network::Network()
{
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	s_socket = socket(AF_INET, SOCK_STREAM, 0);
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

}


Network::~Network()
{
	closesocket(s_socket);
	WSACleanup();
	delete mNetwork;
}


Network* Network::GetNetwork()
{
	if (mNetwork == nullptr)
		mNetwork = new Network();
	return mNetwork;
}


void Network::ConnectServer()
{
	int ret = connect(s_socket, (SOCKADDR*)(&server_addr), sizeof(server_addr));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "연결오류" << std::endl;
		exit(0);
		
	}
}