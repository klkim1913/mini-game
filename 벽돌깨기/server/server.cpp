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
	static unsigned __stdcall GetResultFromClient(void *arg);	// 클라이언트로부터 결과 받아오기
};
int THD::Soc_Ser()
{
	mutex = CreateMutex(NULL, FALSE, NULL);
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	cout << "서버 소켓을 생성 했습니다." << endl;
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
		cout << "서버 소켓을 바인드 하는데 실패했습니다." << endl;
		return 0;
	}
	if (listen(serverSocket, SOMAXCONN) == -1) {
		cout << "서버 소켓을 listen 모드로 설정하는데 실패했습니다" << endl;
		return 0;
	}
	cout << "게임 서버가 실행 되었습니다." << endl;
	cout << "동시 접속자 수: " << clientNumber << endl;
	while (1) {
		struct sockaddr_in clientAddress;
		int clientAddressSize = sizeof(clientAddress);
		int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress,
			&clientAddressSize);
		WaitForSingleObject(mutex, INFINITE);
		clientNumber++;	// 현재 접속중인 클라이언트 수 증가
		allClientSocket[clientNumber - 1] = clientSocket;
		cout << "동시 접속자 수: " << clientNumber << endl;
		ReleaseMutex(mutex);
		unsigned long thread;
		thread = _beginthreadex(NULL, 0, GetResultFromClient, (void*)clientSocket, 0, &threadID);
	}
	cout << "서버를 종료 합니다" << endl;
	return 0;
}
unsigned __stdcall THD::GetResultFromClient(void *arg)
{
	int timefromClient = 0;

	int myClientSocket = (int)arg;
	int i, a;
	int fromcltm[3];	// 소요시간 전달받는 배열

	a = 1;
	while (a)	// 접속 종료시까지 결과 받아들임
	{
		int readlen = recv(myClientSocket, (char*)fromcltm, sizeof(fromcltm), 0);	// 소요시간 받음
		if (readlen > 0)	// 게임 종료후 소요시간 출력
		{
			cout << "현재 플레이어 소요시간: " << fromcltm[0] << endl;
		}
		else if (readlen <= 0)	// 0 이하면 반복 종료
			a = 0;
	}
	// 종료한 클라이언트 소켓 제거 부분 
		for (i = 0; i < clientNumber; i++) {	// 하나씩 당겨옴
			if (allClientSocket[i] == myClientSocket) {
				for (; i < clientNumber - 1; i++) {
					allClientSocket[i] = allClientSocket[i + 1];
				}
				clientNumber--;//전체 클라이언트의 수를 1감소 시킴
				cout << "동시 접속자 수: " << clientNumber << endl;
				closesocket(myClientSocket);//현재 클라이언트와 연결된 소켓을 종료함
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