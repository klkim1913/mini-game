#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <process.h>

#define LOAD 0
#define WALL 1
#define APPLE 2
#define COM1 3
#define COM2 4
#define COM3 5
#define GAMER 6
#define TOLEFT 7
#define TORIGHT 8
#define BOOM 9

int map[5][10] = { { 0,0,0,0,0,0,0,0,0,0 },		// �� ����
				   { 0,1,1,1,1,0,1,1,1,0 },
				   { 0,0,0,0,0,0,0,0,0,0 },
				   { 0,1,1,1,1,0,1,1,1,0 },
				   { 0,0,0,0,0,0,0,0,0,0 } };

int Gi, Gj, C1j, C2j, C3j;	// �� ��ü���� ��ǥ
HANDLE hMutex;
int key = 1;
int apch1, apch2, apch3;

unsigned __stdcall Com1Move(void *arg);
unsigned __stdcall Com2Move(void *arg);
unsigned __stdcall Com3Move(void *arg);
unsigned __stdcall GamerMove(void *arg);
unsigned __stdcall CheckEnd(void *agr);

void SetMap();
void PrintMap();

int main()
{
	unsigned Com1ThId, Com2ThId, Com3ThId, GamerThId, ChThId;	// ������ ���̵� ����
	unsigned long hCom1Th, hCom2Th, hCom3Th, hGamerTh, hChTh;
	HANDLE hThread[5];

	srand((unsigned)time(NULL));
	hMutex = CreateMutex(NULL, FALSE, NULL);

	SetMap();
	hCom1Th = _beginthreadex(NULL, 0, Com1Move, NULL, 0, &Com1ThId);
	hCom2Th = _beginthreadex(NULL, 0, Com2Move, NULL, 0, &Com2ThId);
	hCom3Th = _beginthreadex(NULL, 0, Com3Move, NULL, 0, &Com3ThId);
	hGamerTh = _beginthreadex(NULL, 0, GamerMove, NULL, 0, &GamerThId);
	hChTh = _beginthreadex(NULL, 0, CheckEnd, NULL, 0, &ChThId);

	hThread[0] = (HANDLE)hCom1Th;
	hThread[1] = (HANDLE)hCom2Th;
	hThread[2] = (HANDLE)hCom3Th;
	hThread[3] = (HANDLE)hGamerTh;
	hThread[4] = (HANDLE)hChTh;
	WaitForMultipleObjects(5, hThread, TRUE, INFINITE);

	system("pause");
	return 0;
}

void SetMap()		// �� �ʱ�ȭ
{
	int i, j, r;
	int ap;

	for (i = 0; i < 5; i++)			// ��� �� ����(�ٽ� ������ ��)
	{
		for (j = 0; j < 10; j++)
		{
			if (i == 1 || i == 3)
			{
				if (j == 0 || j == 5 || j == 9)
					map[i][j] = LOAD;
				else
					map[i][j] = WALL;
			}
			else
				map[i][j] = 0;
		}
	}

	printf("����� ���� Ȯ��: ");
	scanf("%d", &ap);
	for (i = 0; i < 5; i++)		// Ȯ���� ���� ��� ����
	{
		for (j = 0; j < 10; j++)
		{
			if (map[i][j] == LOAD)
			{
				r = rand() % 100 + 1;
				if (r <= ap)
				{
					map[i][j] = APPLE;
				}
			}
		}
	}
	
	r = rand() % 10;	// ��1 ����
	if (map[0][r] == APPLE)	// ��� ����
		apch1 = 1;
	map[0][r] = COM1;
	C1j = r;
	r = rand() % 10;	// ��2 ����
	if (map[2][r] == APPLE)
		apch2 = 1;
	map[2][r] = COM2;
	C2j = r;
	r = rand() % 10;	// ��3 ����
	if (map[4][r] == APPLE)
		apch3 = 1;
	map[4][r] = COM3;
	C3j = r;
	
	Gi = rand() % 5;
	Gj = rand() % 10;
	while (map[Gi][Gj] == WALL || map[Gi][Gj] == COM1 || map[Gi][Gj] == COM2 || map[Gi][Gj] == COM3)	// ���̸� ����
	{
		Gi = rand() % 5;
		Gj = rand() % 10;
	}
	map[Gi][Gj] = GAMER;

	PrintMap();
}

unsigned __stdcall Com1Move(void *arg)		// ��1�� ������
{
	int way1 = TORIGHT;
	int v1 = 2000;
	int vcnt1 = 0;

	while (key)
	{
		if (C1j == 9)		// ������ȯ ���� �˻�
		{
			way1 = TOLEFT;
		}
		else if (C1j == 0)
		{
			way1 = TORIGHT;
		}

		if (way1 == TORIGHT)	// ���������� ����
		{
			if (map[0][C1j + 1] == APPLE)	// �տ� ��� �ִ��� üũ
			{
				if (apch1 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[0][C1j] = LOAD;
					C1j++;
					map[0][C1j] = COM1;
				}
				else if (apch1 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[0][C1j] = APPLE;
					C1j++;
					map[0][C1j] = COM1;
				}
				apch1 = 1;
			}
			else   // �տ� ����� ���ٸ�
			{
				if (apch1 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[0][C1j] = LOAD;
					C1j++;
					map[0][C1j] = COM1;
				}
				else if (apch1 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[0][C1j] = APPLE;
					C1j++;
					map[0][C1j] = COM1;
				}
				apch1 = 0;
			}
		}
		if (way1 == TOLEFT)	// �������� ����
		{
			if (map[0][C1j - 1] == APPLE)	// �տ� ��� �ִ��� üũ
			{
				if (apch1 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[0][C1j] = LOAD;
					C1j--;
					map[0][C1j] = COM1;
				}
				else if (apch1 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[0][C1j] = APPLE;
					C1j--;
					map[0][C1j] = COM1;
				}
				apch1 = 1;
			}
			else   // �տ� ����� ���ٸ�
			{
				if (apch1 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[0][C1j] = LOAD;
					C1j--;
					map[0][C1j] = COM1;
				}
				else if (apch1 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[0][C1j] = APPLE;
					C1j--;
					map[0][C1j] = COM1;
				}
				apch1 = 0;
			}
		}
		if (vcnt1 % 5 == 0)	// ���� �ð��� ������ �ӵ� ����
		{
			v1 -= 30;
		}
		vcnt1++;

		PrintMap();
		Sleep(v1);
	}

	return 0;
}

unsigned __stdcall Com2Move(void *arg)		// ��2�� ������
{
	int way2 = TORIGHT;
	int v2 = 2000;
	int vcnt2 = 0;

	while (key)
	{
		if (C2j == 9)		// ������ȯ ���� �˻�
		{
			way2 = TOLEFT;
		}
		else if (C2j == 0)
		{
			way2 = TORIGHT;
		}

		if (way2 == TORIGHT)	// ���������� ����
		{
			if (map[2][C2j + 1] == APPLE)	// �տ� ��� �ִ��� üũ
			{
				if (apch2 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[2][C2j] = LOAD;
					C2j++;
					map[2][C2j] = COM2;
				}
				else if (apch2 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[2][C2j] = APPLE;
					C2j++;
					map[2][C2j] = COM2;
				}
				apch2 = 1;
			}
			else   // �տ� ����� ���ٸ�
			{
				if (apch2 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[2][C2j] = LOAD;
					C2j++;
					map[2][C2j] = COM2;
				}
				else if (apch2 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[2][C2j] = APPLE;
					C2j++;
					map[2][C2j] = COM2;
				}
				apch2 = 0;
			}
		}
		if (way2 == TOLEFT)	// �������� ����
		{
			if (map[2][C2j - 1] == APPLE)	// �տ� ��� �ִ��� üũ
			{
				if (apch2 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[2][C2j] = LOAD;
					C2j--;
					map[2][C2j] = COM2;
				}
				else if (apch2 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[2][C2j] = APPLE;
					C2j--;
					map[2][C2j] = COM2;
				}
				apch2 = 1;
			}
			else   // �տ� ����� ���ٸ�
			{
				if (apch2 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[2][C2j] = LOAD;
					C2j--;
					map[2][C2j] = COM2;
				}
				else if (apch2 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[2][C2j] = APPLE;
					C2j--;
					map[2][C2j] = COM2;
				}
				apch2 = 0;
			}
		}
		if (vcnt2 % 5 == 0)	// ���� �ð��� ������ �ӵ� ����
		{
			v2 -= 30;
		}
		vcnt2++;

		PrintMap();
		Sleep(v2);
	}

	return 0;
}

unsigned __stdcall Com3Move(void *arg)		// ��2�� ������
{
	int way3 = TORIGHT;
	int v3 = 2000;
	int vcnt3 = 0;

	while (key)
	{
		if (C3j == 9)		// ������ȯ ���� �˻�
		{
			way3 = TOLEFT;
		}
		else if (C3j == 0)
		{
			way3 = TORIGHT;
		}

		if (way3 == TORIGHT)	// ���������� ����
		{
			if (map[4][C3j + 1] == APPLE)	// �տ� ��� �ִ��� üũ
			{
				if (apch3 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[4][C3j] = LOAD;
					C3j++;
					map[4][C3j] = COM2;
				}
				else if (apch3 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[4][C3j] = APPLE;
					C3j++;
					map[4][C3j] = COM2;
				}
				apch3 = 1;
			}
			else   // �տ� ����� ���ٸ�
			{
				if (apch3 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[4][C3j] = LOAD;
					C3j++;
					map[4][C3j] = COM2;
				}
				else if (apch3 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[4][C3j] = APPLE;
					C3j++;
					map[4][C3j] = COM2;
				}
				apch3 = 0;
			}
		}
		if (way3 == TOLEFT)	// �������� ����
		{
			if (map[4][C3j - 1] == APPLE)	// �տ� ��� �ִ��� üũ
			{
				if (apch3 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[4][C3j] = LOAD;
					C3j--;
					map[4][C3j] = COM2;
				}
				else if (apch3 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[4][C3j] = APPLE;
					C3j--;
					map[4][C3j] = COM2;
				}
				apch3 = 1;
			}
			else   // �տ� ����� ���ٸ�
			{
				if (apch3 == 0)	// ������ ���ڸ��� ����� �����ٸ�
				{
					map[4][C3j] = LOAD;
					C3j--;
					map[4][C3j] = COM2;
				}
				else if (apch3 == 1)	// ������ ���ڸ��� ����� �־��ٸ�
				{
					map[4][C3j] = APPLE;
					C3j--;
					map[4][C3j] = COM2;
				}
				apch3 = 0;
			}
		}
		if (vcnt3 % 5 == 0)	// ���� �ð��� ������ �ӵ� ����
		{
			v3 -= 30;
		}
		vcnt3++;

		PrintMap();
		Sleep(v3);
	}
	return 0;
}

void PrintMap()	// �� ���
{
	int i, j;

	WaitForSingleObject(hMutex, INFINITE);
	system("cls");
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (map[i][j] == LOAD)
				printf("��");
			else if (map[i][j] == WALL)
				printf("��");
			else if (map[i][j] == APPLE)
				printf("��");
			else if (map[i][j] == COM1 || map[i][j] == COM2 || map[i][j] == COM3)
				printf("��");
			else if (map[i][j] == GAMER)
				printf("��");
			else if (map[i][j] == BOOM)
				printf("��");
		}
		printf("\n");
	}
	ReleaseMutex(hMutex);
}

unsigned __stdcall GamerMove(void *arg)	//  ���̸� ������
{
	int put;

	while (key)
	{
		put = _getch();
		if (put == 224)
		{
			put = _getch();
			switch (put)
			{
			case 72:	// ��
				if (Gi != 0 && map[Gi - 1][Gj] != WALL)	// ���̳� ���������̸� ����
				{
					map[Gi][Gj] = LOAD;
					Gi--;
					map[Gi][Gj] = GAMER;
				}
				break;
			case 80:	// �Ʒ�
				if (Gi != 4 && map[Gi + 1][Gj] != WALL)
				{
					map[Gi][Gj] = LOAD;
					Gi++;
					map[Gi][Gj] = GAMER;
				}
				break;
			case 75:	// ����
				if (Gj == 0)
				{
					map[Gi][Gj] = LOAD;
					Gj = 9;
					map[Gi][Gj] = GAMER;
				}
				else if (map[Gi][Gj - 1] != WALL)
				{
					map[Gi][Gj] = LOAD;
					Gj--;
					map[Gi][Gj] = GAMER;
				}
				break;
			case 77:	// ������
				if (Gj == 9)
				{
					map[Gi][Gj] = LOAD;
					Gj = 0;
					map[Gi][Gj] = GAMER;
				}
				else if (map[Gi][Gj + 1] != WALL)
				{
					map[Gi][Gj] = LOAD;
					Gj++;
					map[Gi][Gj] = GAMER;
				}
				break;
			}
		}
		PrintMap();
	}


	return 0;
}

unsigned __stdcall CheckEnd(void *agr)	// ������ ���� 2�� üũ
{
	int i, j, apcnt, k = 0;
	int put;

	while (key)
	{
		WaitForSingleObject(hMutex, INFINITE);
		apcnt = 0;
		if (Gi == 0 && Gj == C1j)	// ��1�� ��������
		{
			map[Gi][Gj] = BOOM;
			k = 1;
			PrintMap();
		}
		else if (Gi == 2 && Gj == C2j)	// ��2�� ��������
		{
			map[Gi][Gj] = BOOM;
			k = 1;
			PrintMap();
		}
		else if (Gi == 4 && Gj == C3j)	// ��3�� ��������
		{
			map[Gi][Gj] = BOOM;
			k = 1;
			PrintMap();
		}

		for (i = 0; i < 5; i++)
		{
			for (j = 0; j < 10; j++)
			{
				if (map[i][j] == APPLE)
				{
					apcnt++;
				}
			}
		}
		if (apcnt == 0 && apch1 != 1 && apch2 != 1 && apch3 != 1)
		{
			PrintMap();
			printf("\nCLEAR!!\n\n");
			k = 1;
		}

		if (k == 1)
		{
			printf("�ٽ��Ͻ÷��� y �ƴϸ� n�� �����ּ���\n");
			put = _getch();
			while (put != 'n' && put != 'y')
				put = _getch();
			if (put == 'n')
				key = 0;
			else if (put == 'y')
			{
				system("cls");
				SetMap();
				k = 0;
			}
		}
		ReleaseMutex(hMutex);
	}
	return 0;
}