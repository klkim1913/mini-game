#include <Winsock2.h>
#include <Windows.h>
#include <iostream>
#include <process.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

#define BLANK 0
#define BALL 4
#define PORT 5500

using namespace std;

int start = 0;

class THD {
public:
	int Soc_Cli();
	void GameStart(void *sc);
};

class Maping {   // 맵 생성 부모 클래스
protected:		  // 자식 클래스에 상속 가능
	int imap[10][6];  // 전체 맵 변수
	int t;	// 소요시간
public:
	Maping();  // 초기화면 생성자
	~Maping(); // 게임 종료시 안내 소멸자
	void Draw_Map(); // 맵 생성
};

class Con_Ball : public Maping {  // 공 조정 자식 클래스 (부모 클래스 상속)
private:
	int dr;				// 여러가지 옵션 변수
	int ballx, bally;   // 볼의 좌표
	int map[10][6];    // 맵 정보
	int check;			// 게임 끝난지 검사
public:
	void Init_G();		// 초기화 및 전체 프로그램 조정
	void Move_Ball();	// 공 쏘기
	void Get_Map();		// 부모 클래스에서 생성된 맵을 받음
	int Get_Val(int i, int j);
	void Key_In();		// 공 쏘기 위한 키 선택

	void Move_Up();  // 수직으로 상승
	void Move_Right();  // 오른쪽으로 상승
	void Move_Left();  // 왼쪽으로 상승

	void Move_Down(); // 하강 조정
	void Get_Down(); // 수직으로 하강
	void Right_Down();//오른쪽으로 하강
	void Left_Down(); // 왼쪽으로 하강

	void Breaking(); // 벽돌깨기

	void Print_Ball(); // 맵출력

	void Endgame();	// 게임이 끝났는지 체크하고 재시작 여부 확인
	int Regame(void *sc);	// 다시 게임할건지 확인
};

int THD::Soc_Cli()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	int mySocket = socket(PF_INET, SOCK_STREAM, 0);
	if (mySocket == -1) {
		cout << "클라이언트 소켓을 생성하는데 실패 했습니다" << endl;
		return 0;
	}
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	if (connect(mySocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
		cout << "서버와 연결하는데 실패했습니다" << endl;
		system("pause");
		return 0;
	}
	GameStart((void*)mySocket);	// 게임 시작
}

void THD::GameStart(void *sc)
{
	Con_Ball b;
	int a;
	int mySocket = (int)sc;

	a = 1;
	while (a)	// 서버 연결 시 게임 시작
	{
		if (start == 1)	// 첫화면에 게임시작을 선택하면 시작
		{
			b.Init_G();
			a = b.Regame(sc);
		}
		else           // 아니면 바로 종료
		{
			a = 0;
			closesocket(mySocket);
		}
	}
}

Maping::Maping()	// 시작 or 종료 결정
{
	int put;
	
	cout << "1. 게임시작" << endl;
	cout << "2. 종료" << endl;

	t = 0;
	put = getch();
	while (put != '1' && put != '2')
	{
		put = getch();
	}
	if (put == '1')	// 1누르면 시작 아니면 바로 종료
		start = 1;
}

void Maping::Draw_Map()
{
	int i, j, c;
	int rd;

	srand((unsigned)time(NULL));
	c = 0;
	rd = rand() % 3; // 랜덤으로 벽돌 모양 지정

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (i <= 2)	  // 벽돌 모양 지정
			{
				if (rd == 0) imap[i][j] = 1;
				else if (rd == 1) imap[i][j] = 2;
				else if (rd == 2) imap[i][j] = 3;
				++c;
			}
			else imap[i][j] = BLANK;  // 나머지는 빈 공간
			if (c == 3)
			{
				c = 0;   // 벽돌 크기 지정
				rd = rand() % 3; // 랜덤으로 벽돌 모양 지정
			}
		}
	}
}

Maping::~Maping()	// 서버종료 안내문
{
	system("cls");
	cout << "서버를 종료합니다!" << endl << endl;
	system("pause");
}

void Con_Ball::Init_G()
{
	Draw_Map();	// 맵 생성

	Move_Ball(); // 공 조정
}

int Con_Ball::Regame(void *sc)
{
	int put;
	int mySocket = (int)sc;
	int tm[3] = { 0 };

	tm[0] = t;	// 소요시간 담음
	cout << "소요시간: " << t << endl;
	send(mySocket, (char*)tm, sizeof(tm), 0);	// 서버로 소요시간 전달
	cout << endl << endl << "다시하시려면 1번 종료하려면 2번을 눌러주세요";

	t = 0;
	put = getch();
	while (put != '1' && put != '2')
	{
		put = getch();
	}
	if (put == '1')	// 재시작
		return 1;
	else            // 종료하고 소켓 닫음
	{
		closesocket(mySocket);
		return 0;
	}
}

void Con_Ball::Move_Ball()
{
	Get_Map();   // 맵 획득
	bally = 9;
	dr = rand() % 6;  // 최초 x 좌표 설정
	ballx = dr;
	map[bally][ballx] = BALL;  // 최초 공 설정

	system("cls");
	Print_Ball();  // 처음 화면
	Key_In();      // 게임 시작
}

void Con_Ball::Get_Map() // 부모클래스의 맵 정보 받음
{
	int i, j;

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 6; j++) {
			map[i][j] = Get_Val(i, j);
		}
	}
}

int Con_Ball::Get_Val(int i, int j)  // 부모클래스의 맵 정보 받음
{
	return imap[i][j]; // 부모클래스의 배열 imap의 정보를 받음
}

void Con_Ball::Key_In()  // 키 입력
{
	int put;

	cout << endl << endl;
	cout << " Press Space";
	put = getch();
	while (put != 32)	// 스페이스를 누르면 시작
		put = getch();
	check = 1;
	while (check) {  // 무한 루프
		dr = rand() % 3;
		if (dr == 0) Move_Up();
		if (dr == 1) Move_Right();
		if (dr == 2) Move_Left();
	}
}

void Con_Ball::Breaking()	// 위의 공을 격파
{
	map[bally - 1][ballx] = BLANK;
}

void Con_Ball::Endgame()
{
	int i, j, sum;

	sum = 0;
	for (i = 0; i < 10; i++)	// 공만 남았는지 검사
	{
		for (j = 0; j < 6; j++)
		{
			sum += map[i][j];
		}
	}
	if (sum == BALL)	// 공만 남았으면 종료
	{
		check = 0;
	}
}

void Con_Ball::Move_Up()  // 수직 상승
{
	int n, m;  // 공의 좌표 임시저장 공간
	int key = 1;

	while (key) {
		if (bally == 0) key = 0;	// 천장 막기
		else if (map[bally - 1][ballx] != BLANK)	// 상승하려는 순간 위가 바로 벽돌이면 벽돌깨기
		{
			Breaking();
			key = 0;
			system("cls"); // 화면 갱신
			Print_Ball();  // 공표시
			Sleep(30);
			++t;
		}
		else
		{
			m = ballx;  // 이전 좌표 확보
			n = bally;
			map[n][m] = BLANK;  // 이전 좌표 공을 무효화
			--bally;    // 공의 수직 상승
			map[bally][ballx] = BALL;
			if (bally != 0)
			{
				if (map[bally - 1][ballx] != BLANK)	// 벽돌깨기
				{
					Breaking();
					Endgame();	// 다 깼는지 확인
					key = 0;
				}
			}
			system("cls"); // 화면 갱신
			Print_Ball();  // 공표시
			Sleep(30);
			++t;
		}
	}
	Move_Down();
}

void Con_Ball::Move_Right()  // 우측 상승
{
	int n, m, ch = 0;
	int key = 1;

	while (key) {
		if (bally == 0) key = 0;	// 천장 막기
		else if (map[bally - 1][ballx] != BLANK)	// 상승하려는 순간 바로 위가 벽돌이면 벽돌 깨기
		{
			Breaking();
			Endgame();	// 다 깼는지 확인
			key = 0;
			ch = 1;	// 벽돌 꺤지 체크
			system("cls");  // 화면 갱신
			Print_Ball();  // 출력
			Sleep(30);
			++t;
		}
		else if (ballx == 5) key = 0;	// 오른쪽 벽 부딪히는거 판정
										//else if (map[bally][ballx + 1] != BLANK) key = 0;	// 오른쪽이 벽돌이면 왼쪽회전
		else if (map[bally - 1][ballx + 1] != BLANK)	// 우상단 벽돌있으면 내려오기
		{
			key = 0;
			ch = 1;
		}
		else  // 상승
		{
			m = ballx;   // 이전 좌표 확보
			n = bally;
			map[n][m] = BLANK;  // 이전 공 삭제
			--bally;    // 상승

			if (ballx < 5) ballx++;  // x 좌표 범위 안에서 증가
			if (ballx == 5) key = 0;  // x 좌표 초과 시 증가 멈춤

			map[bally][ballx] = BALL;  // 공표시
			if (bally != 0)
			{
				if (map[bally - 1][ballx] != BLANK)	// 벽돌 깨기
				{
					Breaking();
					Endgame();	// 다 깼는지 확인
					key = 0;
					ch = 1;	// 벽돌 꺤지 체크
				}
			}
			system("cls");  // 화면 갱신
			Print_Ball();  // 출력
			Sleep(30);
			++t;
		}
	}

	if (ballx == 5 && ch == 0) Move_Left();  // 왼쪽 회전(벽돌 안꺴을때)
	Move_Down();  // 하강
}

void Con_Ball::Move_Left()
{
	int n, m, ch = 0;
	int key = 1;

	while (key) {
		if (bally == 0) key = 0;	// 천장 막기
		else if (map[bally - 1][ballx] != BLANK)	// 상승 하려는 순간 아래가 바로 벽돌이면 벽돌깨기
		{
			Breaking();
			Endgame();	// 다 깼는지 확인
			key = 0;
			ch = 1;	// 벽돌 꺤지 체크
			system("cls");
			Print_Ball();
			Sleep(30);
			++t;
		}
		else if (ballx == 0) key = 0;	// 왼쪽 벽 부딪히는거 판정
										//else if (map[bally][ballx - 1] != BLANK) key = 0;	// 왼쪽이 벽돌이면 오른쪽회전
		else if (map[bally - 1][ballx - 1] != BLANK)	// 좌상단 벽돌있으면 내려오기
		{
			key = 0;
			ch = 1;
		}
		else
		{
			m = ballx;
			n = bally;
			map[n][m] = BLANK;
			--bally;    // 상승

			if (ballx > 0) ballx--;
			if (ballx == 0) key = 0;

			map[bally][ballx] = BALL;
			if (bally != 0)
			{
				if (map[bally - 1][ballx] != BLANK)	// 벽돌깨기
				{
					Breaking();
					Endgame();	// 다 깼는지 확인
					key = 0;
					ch = 1;	// 벽돌 꺤지 체크
				}
			}
			system("cls");
			Print_Ball();
			Sleep(30);
			++t;
		}
	}

	if (ballx == 0 && ch == 0) Move_Right(); // 오른쪽 회전(벽돌 안깼을때)
	Move_Down();
}

void Con_Ball::Move_Down()
{
	int nm;

	nm = rand() % 3;

	if (check)	// 다 안깨졌다면 계속 이동
	{
		if (nm == 0) Get_Down();  // 수짓 하강
		if (nm == 1) Right_Down(); // 우측 하강
		if (nm == 2) Left_Down();  // 좌측 하강
	}
}

void Con_Ball::Get_Down()  // 수직 하강
{
	int n, m;
	int key = 1;

	while (key) {
		if (bally == 9) key = 0;	// 바닥 막기
		else if (map[bally + 1][ballx] != BLANK) key = 0;	// 하강순간 바로 아래가 벽돌이면 다시 위로
		else
		{
			m = ballx;
			n = bally;
			map[n][m] = BLANK;
			++bally;   // 수직 하강
			map[bally][ballx] = BALL;
			system("cls");
			Print_Ball();
			Sleep(30);
			++t;
		}
	}
}

void Con_Ball::Right_Down()
{
	int n, m, ch = 0;
	int key = 1;

	while (key) {
		if (bally == 9) key = 0;	// 바닥 막기
		else if (map[bally + 1][ballx] != BLANK)	// 하강 순간 바로 아래가 벽돌이면 다시 위로
		{
			key = 0;
			ch = 1; // 벽돌 만난지 체크
		}
		else if (ballx == 5) key = 0;	// 오른쪽 벽 부딪히는거 판정
										//else if (map[bally][ballx + 1] != BLANK) key = 0;	// 오른쪽이 벽돌이면 왼쪽회전
		else if (map[bally + 1][ballx + 1] != BLANK)	// 우하단 벽돌있으면 올라가기
		{
			key = 0;
			ch = 1;
		}
		else
		{
			m = ballx;
			n = bally;
			map[n][m] = BLANK;
			++bally;    // 하강

			if (ballx < 5) ballx++;  // x 좌표 우측조정, 우측으로 하강
			if (ballx == 5) key = 0; // 벽에 부딪혔는지 확인

			map[bally][ballx] = BALL;
			if (bally != 9)
			{
				if (map[bally + 1][ballx] != BLANK)	// 아래가 벽이면 다시 위로
				{
					key = 0;
					ch = 1; // 벽돌 만난지 체크
				}
			}
			system("cls");
			Print_Ball();
			Sleep(30);
			++t;
		}
	}

	if (ballx == 5 && ch == 0) Left_Down(); // 왼쪽 회전(벽 안만났을때)
}

void Con_Ball::Left_Down()
{
	int n, m, ch = 0;
	int key = 1;

	while (key) {
		if (bally == 9) key = 0;	// 바닥 막기
		else if (map[bally + 1][ballx] != BLANK)	// 하강 순간 바로 아래가 벽이면 다시 위로
		{
			key = 0;
			ch = 1;	// 벽 만난지 체크
		}
		else if (ballx == 0) key = 0;	// 왼쪽 벽 부딪히는거 판정
										//else if (map[bally][ballx - 1] != BLANK) key = 0;	// 왼쪽이 벽돌이면 오른쪽회전
		else if (map[bally + 1][ballx - 1] != BLANK)	// 좌하단 벽돌있으면 올라가기
		{
			key = 0;
			ch = 1;
		}
		else
		{
			m = ballx;
			n = bally;
			map[n][m] = BLANK;
			++bally;    // 하강

			if (ballx > 0) ballx--;  // x 좌표 좌측조정,좌측으로 하강
			if (ballx == 0) key = 0;

			map[bally][ballx] = BALL;
			if (bally != 9)
			{
				if (map[bally + 1][ballx] != BLANK)	// 아래가 벽이면 다시 위로
				{
					key = 0;
					ch = 1; // 벽 만난지 체크
				}
			}
			system("cls");
			Print_Ball();
			Sleep(30);
			++t;
		}
	}

	if (ballx == 0 && ch == 0) Right_Down(); // 오른쪽 회전(벽 안만났을때)
}

void Con_Ball::Print_Ball() // 화면 출력
{
	int i, j;

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 6; j++) {
			if (map[i][j] == BLANK) cout << "  ";  //두 칸
			else if (map[i][j] == 1) cout << "■";  //■
			else if (map[i][j] == 2) cout << "◆";  //◆
			else if (map[i][j] == 3) cout << "☆";  //☆
			else if (map[i][j] == BALL) cout << "●";  //●
		}
		cout << endl;;
	}
}

int main()
{
	THD cli;

	cli.Soc_Cli();

	return 0;
}