#include <winsock2.h>
#include <Windows.h>
#include <iostream>
#include <process.h>
#include <stdlib.h>
#include <time.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

#define  PORT 5500
HANDLE  mutex;
int clientNumber = 0;
unsigned  threadID;
int allClientSocket[100];

class THD {
public:
	int Soc_Ser();
	static unsigned __stdcall GetResultFromClient(void *arg);	// Ŭ���̾�Ʈ�κ��� ��� �޾ƿ���
};
int THD::Soc_Ser()
{
	mutex = CreateMutex(NULL, FALSE, NULL);
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	cout << "���� ������ ���� �߽��ϴ�." << endl;
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
		cout << "���� ������ ���ε� �ϴµ� �����߽��ϴ�." << endl;
		return 0;
	}
	if (listen(serverSocket, SOMAXCONN) == -1) {
		cout << "���� ������ listen ���� �����ϴµ� �����߽��ϴ�" << endl;
		return 0;
	}
	cout << "���� ������ ���� �Ǿ����ϴ�." << endl;
	cout << "���� ������ ��: " << clientNumber << endl;
	while (1) {
		struct sockaddr_in clientAddress;
		int clientAddressSize = sizeof(clientAddress);
		int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress,
			&clientAddressSize);
		WaitForSingleObject(mutex, INFINITE);
		clientNumber++;	// ���� �������� Ŭ���̾�Ʈ �� ����
		allClientSocket[clientNumber - 1] = clientSocket;
		cout << "���� ������ ��: " << clientNumber << endl;
		ReleaseMutex(mutex);
		unsigned long thread;
		thread = _beginthreadex(NULL, 0, GetResultFromClient, (void*)clientSocket, 0, &threadID);
	}
	cout << "������ ���� �մϴ�" << endl;
	return 0;
}
unsigned __stdcall THD::GetResultFromClient(void *arg)
{
	int timefromClient = 0;

	int myClientSocket = (int)arg;
	int i, a;
	int fromcltm[3];	// �ҿ�ð� ���޹޴� �迭

	a = 1;
	while (a)	// ���� ����ñ��� ��� �޾Ƶ���
	{
		int readlen = recv(myClientSocket, (char*)fromcltm, sizeof(fromcltm), 0);	// �ҿ�ð� ����
		if (readlen > 0)	// ���� ������ �ҿ�ð� ���
		{
			cout << "���� �÷��̾� �ҿ�ð�: " << fromcltm[0] << endl;
		}
		else if (readlen <= 0)	// 0 ���ϸ� �ݺ� ����
			a = 0;
	}
	// ������ Ŭ���̾�Ʈ ���� ���� �κ� 
		for (i = 0; i < clientNumber; i++) {	// �ϳ��� ��ܿ�
			if (allClientSocket[i] == myClientSocket) {
				for (; i < clientNumber - 1; i++) {
					allClientSocket[i] = allClientSocket[i + 1];
				}
				clientNumber--;//��ü Ŭ���̾�Ʈ�� ���� 1���� ��Ŵ
				cout << "���� ������ ��: " << clientNumber << endl;
				closesocket(myClientSocket);//���� Ŭ���̾�Ʈ�� ����� ������ ������
			}
		}
	return 0;
}

int main()
{
	THD soc;

	soc.Soc_Ser();

	return 0;
}