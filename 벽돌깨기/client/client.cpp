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

class Maping {   // �� ���� �θ� Ŭ����
protected:		  // �ڽ� Ŭ������ ��� ����
	int imap[10][6];  // ��ü �� ����
	int t;	// �ҿ�ð�
public:
	Maping();  // �ʱ�ȭ�� ������
	~Maping(); // ���� ����� �ȳ� �Ҹ���
	void Draw_Map(); // �� ����
};

class Con_Ball : public Maping {  // �� ���� �ڽ� Ŭ���� (�θ� Ŭ���� ���)
private:
	int dr;				// �������� �ɼ� ����
	int ballx, bally;   // ���� ��ǥ
	int map[10][6];    // �� ����
	int check;			// ���� ������ �˻�
public:
	void Init_G();		// �ʱ�ȭ �� ��ü ���α׷� ����
	void Move_Ball();	// �� ���
	void Get_Map();		// �θ� Ŭ�������� ������ ���� ����
	int Get_Val(int i, int j);
	void Key_In();		// �� ��� ���� Ű ����

	void Move_Up();  // �������� ���
	void Move_Right();  // ���������� ���
	void Move_Left();  // �������� ���

	void Move_Down(); // �ϰ� ����
	void Get_Down(); // �������� �ϰ�
	void Right_Down();//���������� �ϰ�
	void Left_Down(); // �������� �ϰ�

	void Breaking(); // ��������

	void Print_Ball(); // �����

	void Endgame();	// ������ �������� üũ�ϰ� ����� ���� Ȯ��
	int Regame(void *sc);	// �ٽ� �����Ұ��� Ȯ��
};

int THD::Soc_Cli()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	int mySocket = socket(PF_INET, SOCK_STREAM, 0);
	if (mySocket == -1) {
		cout << "Ŭ���̾�Ʈ ������ �����ϴµ� ���� �߽��ϴ�" << endl;
		return 0;
	}
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	if (connect(mySocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
		cout << "������ �����ϴµ� �����߽��ϴ�" << endl;
		system("pause");
		return 0;
	}
	GameStart((void*)mySocket);	// ���� ����
}

void THD::GameStart(void *sc)
{
	Con_Ball b;
	int a;
	int mySocket = (int)sc;

	a = 1;
	while (a)	// ���� ���� �� ���� ����
	{
		if (start == 1)	// ùȭ�鿡 ���ӽ����� �����ϸ� ����
		{
			b.Init_G();
			a = b.Regame(sc);
		}
		else           // �ƴϸ� �ٷ� ����
		{
			a = 0;
			closesocket(mySocket);
		}
	}
}

Maping::Maping()	// ���� or ���� ����
{
	int put;
	
	cout << "1. ���ӽ���" << endl;
	cout << "2. ����" << endl;

	t = 0;
	put = getch();
	while (put != '1' && put != '2')
	{
		put = getch();
	}
	if (put == '1')	// 1������ ���� �ƴϸ� �ٷ� ����
		start = 1;
}

void Maping::Draw_Map()
{
	int i, j, c;
	int rd;

	srand((unsigned)time(NULL));
	c = 0;
	rd = rand() % 3; // �������� ���� ��� ����

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (i <= 2)	  // ���� ��� ����
			{
				if (rd == 0) imap[i][j] = 1;
				else if (rd == 1) imap[i][j] = 2;
				else if (rd == 2) imap[i][j] = 3;
				++c;
			}
			else imap[i][j] = BLANK;  // �������� �� ����
			if (c == 3)
			{
				c = 0;   // ���� ũ�� ����
				rd = rand() % 3; // �������� ���� ��� ����
			}
		}
	}
}

Maping::~Maping()	// �������� �ȳ���
{
	system("cls");
	cout << "������ �����մϴ�!" << endl << endl;
	system("pause");
}

void Con_Ball::Init_G()
{
	Draw_Map();	// �� ����

	Move_Ball(); // �� ����
}

int Con_Ball::Regame(void *sc)
{
	int put;
	int mySocket = (int)sc;
	int tm[3] = { 0 };

	tm[0] = t;	// �ҿ�ð� ����
	cout << "�ҿ�ð�: " << t << endl;
	send(mySocket, (char*)tm, sizeof(tm), 0);	// ������ �ҿ�ð� ����
	cout << endl << endl << "�ٽ��Ͻ÷��� 1�� �����Ϸ��� 2���� �����ּ���";

	t = 0;
	put = getch();
	while (put != '1' && put != '2')
	{
		put = getch();
	}
	if (put == '1')	// �����
		return 1;
	else            // �����ϰ� ���� ����
	{
		closesocket(mySocket);
		return 0;
	}
}

void Con_Ball::Move_Ball()
{
	Get_Map();   // �� ȹ��
	bally = 9;
	dr = rand() % 6;  // ���� x ��ǥ ����
	ballx = dr;
	map[bally][ballx] = BALL;  // ���� �� ����

	system("cls");
	Print_Ball();  // ó�� ȭ��
	Key_In();      // ���� ����
}

void Con_Ball::Get_Map() // �θ�Ŭ������ �� ���� ����
{
	int i, j;

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 6; j++) {
			map[i][j] = Get_Val(i, j);
		}
	}
}

int Con_Ball::Get_Val(int i, int j)  // �θ�Ŭ������ �� ���� ����
{
	return imap[i][j]; // �θ�Ŭ������ �迭 imap�� ������ ����
}

void Con_Ball::Key_In()  // Ű �Է�
{
	int put;

	cout << endl << endl;
	cout << " Press Space";
	put = getch();
	while (put != 32)	// �����̽��� ������ ����
		put = getch();
	check = 1;
	while (check) {  // ���� ����
		dr = rand() % 3;
		if (dr == 0) Move_Up();
		if (dr == 1) Move_Right();
		if (dr == 2) Move_Left();
	}
}

void Con_Ball::Breaking()	// ���� ���� ����
{
	map[bally - 1][ballx] = BLANK;
}

void Con_Ball::Endgame()
{
	int i, j, sum;

	sum = 0;
	for (i = 0; i < 10; i++)	// ���� ���Ҵ��� �˻�
	{
		for (j = 0; j < 6; j++)
		{
			sum += map[i][j];
		}
	}
	if (sum == BALL)	// ���� �������� ����
	{
		check = 0;
	}
}

void Con_Ball::Move_Up()  // ���� ���
{
	int n, m;  // ���� ��ǥ �ӽ����� ����
	int key = 1;

	while (key) {
		if (bally == 0) key = 0;	// õ�� ����
		else if (map[bally - 1][ballx] != BLANK)	// ����Ϸ��� ���� ���� �ٷ� �����̸� ��������
		{
			Breaking();
			key = 0;
			system("cls"); // ȭ�� ����
			Print_Ball();  // ��ǥ��
			Sleep(30);
			++t;
		}
		else
		{
			m = ballx;  // ���� ��ǥ Ȯ��
			n = bally;
			map[n][m] = BLANK;  // ���� ��ǥ ���� ��ȿȭ
			--bally;    // ���� ���� ���
			map[bally][ballx] = BALL;
			if (bally != 0)
			{
				if (map[bally - 1][ballx] != BLANK)	// ��������
				{
					Breaking();
					Endgame();	// �� ������ Ȯ��
					key = 0;
				}
			}
			system("cls"); // ȭ�� ����
			Print_Ball();  // ��ǥ��
			Sleep(30);
			++t;
		}
	}
	Move_Down();
}

void Con_Ball::Move_Right()  // ���� ���
{
	int n, m, ch = 0;
	int key = 1;

	while (key) {
		if (bally == 0) key = 0;	// õ�� ����
		else if (map[bally - 1][ballx] != BLANK)	// ����Ϸ��� ���� �ٷ� ���� �����̸� ���� ����
		{
			Breaking();
			Endgame();	// �� ������ Ȯ��
			key = 0;
			ch = 1;	// ���� ���� üũ
			system("cls");  // ȭ�� ����
			Print_Ball();  // ���
			Sleep(30);
			++t;
		}
		else if (ballx == 5) key = 0;	// ������ �� �ε����°� ����
										//else if (map[bally][ballx + 1] != BLANK) key = 0;	// �������� �����̸� ����ȸ��
		else if (map[bally - 1][ballx + 1] != BLANK)	// ���� ���������� ��������
		{
			key = 0;
			ch = 1;
		}
		else  // ���
		{
			m = ballx;   // ���� ��ǥ Ȯ��
			n = bally;
			map[n][m] = BLANK;  // ���� �� ����
			--bally;    // ���

			if (ballx < 5) ballx++;  // x ��ǥ ���� �ȿ��� ����
			if (ballx == 5) key = 0;  // x ��ǥ �ʰ� �� ���� ����

			map[bally][ballx] = BALL;  // ��ǥ��
			if (bally != 0)
			{
				if (map[bally - 1][ballx] != BLANK)	// ���� ����
				{
					Breaking();
					Endgame();	// �� ������ Ȯ��
					key = 0;
					ch = 1;	// ���� ���� üũ
				}
			}
			system("cls");  // ȭ�� ����
			Print_Ball();  // ���
			Sleep(30);
			++t;
		}
	}

	if (ballx == 5 && ch == 0) Move_Left();  // ���� ȸ��(���� �ȃ�����)
	Move_Down();  // �ϰ�
}

void Con_Ball::Move_Left()
{
	int n, m, ch = 0;
	int key = 1;

	while (key) {
		if (bally == 0) key = 0;	// õ�� ����
		else if (map[bally - 1][ballx] != BLANK)	// ��� �Ϸ��� ���� �Ʒ��� �ٷ� �����̸� ��������
		{
			Breaking();
			Endgame();	// �� ������ Ȯ��
			key = 0;
			ch = 1;	// ���� ���� üũ
			system("cls");
			Print_Ball();
			Sleep(30);
			++t;
		}
		else if (ballx == 0) key = 0;	// ���� �� �ε����°� ����
										//else if (map[bally][ballx - 1] != BLANK) key = 0;	// ������ �����̸� ������ȸ��
		else if (map[bally - 1][ballx - 1] != BLANK)	// �»�� ���������� ��������
		{
			key = 0;
			ch = 1;
		}
		else
		{
			m = ballx;
			n = bally;
			map[n][m] = BLANK;
			--bally;    // ���

			if (ballx > 0) ballx--;
			if (ballx == 0) key = 0;

			map[bally][ballx] = BALL;
			if (bally != 0)
			{
				if (map[bally - 1][ballx] != BLANK)	// ��������
				{
					Breaking();
					Endgame();	// �� ������ Ȯ��
					key = 0;
					ch = 1;	// ���� ���� üũ
				}
			}
			system("cls");
			Print_Ball();
			Sleep(30);
			++t;
		}
	}

	if (ballx == 0 && ch == 0) Move_Right(); // ������ ȸ��(���� �Ȳ�����)
	Move_Down();
}

void Con_Ball::Move_Down()
{
	int nm;

	nm = rand() % 3;

	if (check)	// �� �ȱ����ٸ� ��� �̵�
	{
		if (nm == 0) Get_Down();  // ���� �ϰ�
		if (nm == 1) Right_Down(); // ���� �ϰ�
		if (nm == 2) Left_Down();  // ���� �ϰ�
	}
}

void Con_Ball::Get_Down()  // ���� �ϰ�
{
	int n, m;
	int key = 1;

	while (key) {
		if (bally == 9) key = 0;	// �ٴ� ����
		else if (map[bally + 1][ballx] != BLANK) key = 0;	// �ϰ����� �ٷ� �Ʒ��� �����̸� �ٽ� ����
		else
		{
			m = ballx;
			n = bally;
			map[n][m] = BLANK;
			++bally;   // ���� �ϰ�
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
		if (bally == 9) key = 0;	// �ٴ� ����
		else if (map[bally + 1][ballx] != BLANK)	// �ϰ� ���� �ٷ� �Ʒ��� �����̸� �ٽ� ����
		{
			key = 0;
			ch = 1; // ���� ������ üũ
		}
		else if (ballx == 5) key = 0;	// ������ �� �ε����°� ����
										//else if (map[bally][ballx + 1] != BLANK) key = 0;	// �������� �����̸� ����ȸ��
		else if (map[bally + 1][ballx + 1] != BLANK)	// ���ϴ� ���������� �ö󰡱�
		{
			key = 0;
			ch = 1;
		}
		else
		{
			m = ballx;
			n = bally;
			map[n][m] = BLANK;
			++bally;    // �ϰ�

			if (ballx < 5) ballx++;  // x ��ǥ ��������, �������� �ϰ�
			if (ballx == 5) key = 0; // ���� �ε������� Ȯ��

			map[bally][ballx] = BALL;
			if (bally != 9)
			{
				if (map[bally + 1][ballx] != BLANK)	// �Ʒ��� ���̸� �ٽ� ����
				{
					key = 0;
					ch = 1; // ���� ������ üũ
				}
			}
			system("cls");
			Print_Ball();
			Sleep(30);
			++t;
		}
	}

	if (ballx == 5 && ch == 0) Left_Down(); // ���� ȸ��(�� �ȸ�������)
}

void Con_Ball::Left_Down()
{
	int n, m, ch = 0;
	int key = 1;

	while (key) {
		if (bally == 9) key = 0;	// �ٴ� ����
		else if (map[bally + 1][ballx] != BLANK)	// �ϰ� ���� �ٷ� �Ʒ��� ���̸� �ٽ� ����
		{
			key = 0;
			ch = 1;	// �� ������ üũ
		}
		else if (ballx == 0) key = 0;	// ���� �� �ε����°� ����
										//else if (map[bally][ballx - 1] != BLANK) key = 0;	// ������ �����̸� ������ȸ��
		else if (map[bally + 1][ballx - 1] != BLANK)	// ���ϴ� ���������� �ö󰡱�
		{
			key = 0;
			ch = 1;
		}
		else
		{
			m = ballx;
			n = bally;
			map[n][m] = BLANK;
			++bally;    // �ϰ�

			if (ballx > 0) ballx--;  // x ��ǥ ��������,�������� �ϰ�
			if (ballx == 0) key = 0;

			map[bally][ballx] = BALL;
			if (bally != 9)
			{
				if (map[bally + 1][ballx] != BLANK)	// �Ʒ��� ���̸� �ٽ� ����
				{
					key = 0;
					ch = 1; // �� ������ üũ
				}
			}
			system("cls");
			Print_Ball();
			Sleep(30);
			++t;
		}
	}

	if (ballx == 0 && ch == 0) Right_Down(); // ������ ȸ��(�� �ȸ�������)
}

void Con_Ball::Print_Ball() // ȭ�� ���
{
	int i, j;

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 6; j++) {
			if (map[i][j] == BLANK) cout << "  ";  //�� ĭ
			else if (map[i][j] == 1) cout << "��";  //��
			else if (map[i][j] == 2) cout << "��";  //��
			else if (map[i][j] == 3) cout << "��";  //��
			else if (map[i][j] == BALL) cout << "��";  //��
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