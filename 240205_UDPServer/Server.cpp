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
	ServerSockAddr.sin_addr.s_addr = INADDR_ANY;
	ServerSockAddr.sin_port = htons(5001);

	bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrLength = sizeof(ClientSockAddr);

	char Message[1024] = { 0, };

	int RecvLength = recvfrom(ServerSocket, Message, sizeof(Message), 0, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);

	cout << "Recv : " << Message << endl;

	FILE* InputFile = fopen("1.png", "rb");
	int SendSize = 0;
	while (1)
	{
		char Buffer[1024] = { 0, };

		int ReadSize = (int)fread(Buffer, sizeof(char), sizeof(Buffer), InputFile);
		if (ReadSize == 0)
		{
			
			int SendLength = sendto(ServerSocket, Buffer, 0, 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));
			// UDP는 끝났다고 서버가 클라이언트에게 말해줘야함
			break;
		}
		int SendLength = sendto(ServerSocket, Buffer, ReadSize, 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSockAddr));

		SendSize++;

		cout << "send : " << SendSize << endl;
	}

	closesocket(ServerSocket);
	fclose(InputFile);


	WSACleanup();

	return 0;
}