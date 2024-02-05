#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

using namespace std;

#pragma comment(lib, "ws2_32")

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(5001);
	int ServerSockAddrLength = sizeof(ServerSockAddr);

	char Buffer[1024] = "Get";

	int SendLength = sendto(ServerSocket, Buffer, (int)strlen(Buffer), 0, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	FILE* OutputFile = fopen("2.png", "wb");

	while (1)
	{
		char Message[1024] = { 0, };

		int RecvLength = recvfrom(ServerSocket, Message, sizeof(Message), 0, (SOCKADDR*)&ServerSockAddr, &ServerSockAddrLength);
		int WriteSize = (int)fwrite(Message, sizeof(char), RecvLength, OutputFile);
		if (RecvLength == 0 || WriteSize == 0)
		{
			break;
		}
		//cout << "Recv : " << Buffer << endl;
	}

	closesocket(ServerSocket);
	fclose(OutputFile);


	WSACleanup();

	return 0;
}