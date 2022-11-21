
// HwaDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hwa.h"
#include "HwaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct Cardinfo
{
	int x, y, c;	// 좌표와 카드 번호
};

struct Card
{
	struct Cardinfo deck[48], holecard[12], mycard[10], comcard[10], my20[5], my10[9], my5[10], my0[24], com20[5], com10[9], com5[10], com0[24], snap[12], vilcard[10], vil20[5], vil10[9], vil5[10], vil0[24];	// 게임의 카드 저장
};

struct Card cd;
CFont font;
LOGFONT If;
int p, endch, mysc, com1sc, com2sc, drawch, myturn = 1, com1turn, com2turn, mode;
int score[48] = { 0,0,5,20,0,0,5,10,0,0,5,20,0,0,5,10,0,0,5,10,0,0,5,10,0,0,5,10,0,0,10,20,0,0,5,10,0,0,5,10,0,0,0,20,0,5,10,20 };	// 점수


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHwaDlg 대화 상자



CHwaDlg::CHwaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HWA_DIALOG, pParent)
	, myscore(_T(""))
	, com2score(_T(""))
	, com1score(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHwaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ME, myscore);
	//  DDX_Text(pDX, IDC_EDIT_COM2, com2score);
	//  DDX_Text(pDX, IDC_EDIT_COM1, com1score);
	//  DDX_Text(pDX, IDC_EDIT_COM1, com1score);
	DDX_Text(pDX, IDC_EDIT_COM2, com2score);
	DDX_Text(pDX, IDC_EDIT_COM1, com1score);
}

BEGIN_MESSAGE_MAP(CHwaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTT_2p, &CHwaDlg::OnClickedButt2p)
	ON_BN_CLICKED(IDC_BUTT_3p, &CHwaDlg::OnClickedButt3p)
	ON_BN_CLICKED(IDC_BUTT_EXIT, &CHwaDlg::OnClickedButtExit)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CHwaDlg 메시지 처리기

BOOL CHwaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	int i, uy, dy, my;
	CRect rect;
	
	GetClientRect(&rect);
	
	
	// 카드 초기화

	for (i = 0; i < 48; i++)
	{
		cd.deck[i].c = -1;
	}
	for (i = 0; i < 12; i++)
	{
		cd.holecard[i].c = -1;
	}
	for (i = 0; i < 12; i++)
	{
		cd.snap[i].c = -1;
	}

	for (i = 0; i < 10; i++)
	{
		cd.mycard[i].c = -1;
		cd.comcard[i].c = -1;
		cd.vilcard[i].c = -1;
	}
	for (i = 0; i < 5; i++)	// 20점 모음
	{
		cd.my20[i].c = -1;
		cd.com20[i].c = -1;
		cd.vil20[i].c = -1;
	}
	for (i = 0; i < 9; i++)	// 10점 모음
	{
		cd.my10[i].c = -1;
		cd.com10[i].c = -1;
		cd.vil10[i].c = -1;
	}
	for (i = 0; i < 10; i++)	// 5점 모음
	{
		cd.my5[i].c = -1;
		cd.com5[i].c = -1;
		cd.vil5[i].c = -1;
	}
	for (i = 0; i < 24; i++)	// 0점 모음
	{
		cd.my0[i].c = -1;
		cd.com0[i].c = -1;
		cd.vil0[i].c = -1;
	}
	
	// 위치 설정

	uy = rect.top + 10;
	dy = rect.bottom - 10;
	my = (dy - uy) / 2 + uy - 37;	// 높이의 중앙 시작점

	for (i = 0; i < 48; i++)	// 덱 위치
	{
		cd.deck[i].x = 10;
		cd.deck[i].y = my;
	}

	for (i = 0; i < 12; i++)	// 홀 카드 위치
	{
		if (i == 0)
			cd.holecard[i].x = cd.deck[0].x + 69;	// 덱이랑 10 간격
		else
			cd.holecard[i].x = cd.holecard[i - 1].x + 59;	// 옆 카드랑 10 간격
		cd.holecard[i].y = cd.deck[0].y;	// 높이는 덱이랑 같음
	}

	for (i = 0; i < 12; i++)	// 짝 맞출때 위치
	{
		if (i == 0)
			cd.snap[i].x = cd.deck[0].x + 69;	// 덱이랑 10 간격
		else
			cd.snap[i].x = cd.snap[i - 1].x + 59;	// 옆 카드랑 10 간격
		cd.snap[i].y = cd.deck[0].y + 30;	// 높이 겹치게 설정
	}

	for (i = 0; i < 10; i++)	// 내 카드랑 컴 카드 위치
	{
		if (i == 0)
		{
			cd.mycard[i].x = cd.deck[0].x;
			cd.mycard[i].y = cd.deck[0].y + 115;
			cd.comcard[i].x = cd.deck[0].x;
			cd.comcard[i].y = cd.deck[0].y - 115;
		}
		else
		{
			cd.mycard[i].x = cd.mycard[i - 1].x + 59;
			cd.mycard[i].y = cd.mycard[0].y;
			cd.comcard[i].x = cd.comcard[i - 1].x + 59;
			cd.comcard[i].y = cd.comcard[0].y;
		}
	}

	for (i = 0; i < 9; i++)	// 나20점, 컴 10점 위치
	{
		if (i == 0)
		{
			cd.my20[i].x = cd.mycard[0].x;
			cd.my20[i].y = cd.mycard[0].y + 95;
			cd.com10[i].x = cd.comcard[0].x;
			cd.com10[i].y = cd.comcard[0].y - 95;
		}
		else
		{
			if (i < 5)
			{
				cd.my20[i].x = cd.my20[i - 1].x + 20;
				cd.my20[i].y = cd.my20[0].y;
			}
			cd.com10[i].x = cd.com10[i - 1].x + 20;
			cd.com10[i].y = cd.com10[0].y;
		}
	}

	for (i = 0; i < 9; i++)	// 나10점, 컴 20점 위치
	{
		if (i == 0)
		{
			cd.my10[i].x = cd.my20[0].x;
			cd.my10[i].y = cd.my20[0].y + 95;
			cd.com20[i].x = cd.com10[0].x;
			cd.com20[i].y = cd.com10[0].y - 95;
		}
		else
		{
			cd.my10[i].x = cd.my10[i - 1].x + 20;
			cd.my10[i].y = cd.my10[0].y;
			if (i < 5)
			{
				cd.com20[i].x = cd.com20[i - 1].x + 20;
				cd.com20[i].y = cd.com20[0].y;
			}
		}
	}

	for (i = 0; i < 24; i++)	// 나5점, 컴 0점 위치
	{
		if (i == 0)
		{
			cd.my5[i].x = cd.my20[0].x + 250;
			cd.my5[i].y = cd.mycard[0].y + 95;
			cd.com0[i].x = cd.com10[0].x + 250;
			cd.com0[i].y = cd.comcard[0].y - 95;
		}
		else
		{
			if (i < 10)
			{
				cd.my5[i].x = cd.my5[i - 1].x + 20;
				cd.my5[i].y = cd.my5[0].y;
			}
			cd.com0[i].x = cd.com0[i - 1].x + 20;
			cd.com0[i].y = cd.com0[0].y;
		}
	}

	for (i = 0; i < 24; i++)	// 나0점, 컴 5점 위치
	{
		if (i == 0)
		{
			cd.my0[i].x = cd.my10[0].x + 250;
			cd.my0[i].y = cd.my5[0].y + 95;
			cd.com5[i].x = cd.com20[0].x + 250;
			cd.com5[i].y = cd.com0[0].y - 95;
		}
		else
		{
			cd.my0[i].x = cd.my0[i - 1].x + 20;
			cd.my0[i].y = cd.my0[0].y;
			if (i < 10)
			{
				cd.com5[i].x = cd.com5[i - 1].x + 20;
				cd.com5[i].y = cd.com5[0].y;
			}
		}
	}

	for (i = 0; i < 7; i++)	// 컴2카드 위치
	{
		if (i == 0)
		{
			cd.vilcard[i].x = cd.holecard[5].x + 400;
			cd.vilcard[i].y = cd.holecard[5].y - 255;
		}
		else
		{
			cd.vilcard[i].x = cd.vilcard[0].x;
			cd.vilcard[i].y = cd.vilcard[i - 1].y + 85;
		}
	}

	for (i = 0; i < 5; i++)	// 컴2 20점 위치
	{
		if (i == 0)
		{
			cd.vil20[i].x = cd.vilcard[0].x + 79;
			cd.vil20[i].y = cd.vilcard[0].y + 117;
		}
		else
		{
			cd.vil20[i].x = cd.vil20[i - 1].x + 20;
			cd.vil20[i].y = cd.vil20[0].y;
		}
	}
	for (i = 0; i < 9; i++)	// 컴2 10점 위치
	{
		if (i == 0)
		{
			cd.vil10[i].x = cd.vil20[0].x;
			cd.vil10[i].y = cd.vil20[0].y + 85;
		}
		else
		{
			cd.vil10[i].x = cd.vil10[i - 1].x + 20;
			cd.vil10[i].y = cd.vil10[0].y;
		}
	}
	for (i = 0; i < 10; i++)	// 컴2 5점 위치
	{
		if (i == 0)
		{
			cd.vil5[i].x = cd.vil20[0].x;
			cd.vil5[i].y = cd.vil10[0].y + 85;
		}
		else
		{
			cd.vil5[i].x = cd.vil5[i - 1].x + 20;
			cd.vil5[i].y = cd.vil5[0].y;
		}
	}
	for (i = 0; i < 24; i++)	// 컴2 0점 위치
	{
		if (i == 0)
		{
			cd.vil0[i].x = cd.vil20[0].x;
			cd.vil0[i].y = cd.vil5[0].y + 85;
		}
		else
		{
			cd.vil0[i].x = cd.vil0[i - 1].x + 20;
			cd.vil0[i].y = cd.vil0[0].y;
		}
	}

	mysc = 0;
	com1sc = 0;
	com2sc = 0;
	myturn = 1;
	com1turn = 0;
	com2turn = 0;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CHwaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHwaDlg::OnPaint()
{
	CClientDC dc(this);
	CBrush br, *oldbr;
	HDC backdc, carddc[48];
	HBITMAP back, card[48];
	int i;

	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	
	// 컬러링

	br.CreateSolidBrush(RGB(50, 70, 110));
	oldbr = dc.SelectObject(&br);
	dc.Rectangle(5, 55, 1138, 845);
	dc.SelectObject(oldbr);
	br.DeleteObject();

	br.CreateSolidBrush(RGB(60, 150, 30));
	oldbr = dc.SelectObject(&br);
	dc.Rectangle(5, 5, 1138, 54);
	dc.SelectObject(oldbr);
	br.DeleteObject();

	br.CreateSolidBrush(RGB(60, 150, 30));
	oldbr = dc.SelectObject(&br);
	dc.Rectangle(5, 846, 1138, 900);
	dc.SelectObject(oldbr);
	br.DeleteObject();

	// 폰트 조절

	ZeroMemory(&If, sizeof(If));
	If.lfHeight = 25;
	lstrcpy(If.lfFaceName, (LPCWSTR)"휴먼매직체");

	font.CreateFontIndirectW(&If);
	GetDlgItem(IDC_EDIT_ME)->SetFont(&font);
	font.Detach();

	font.CreateFontIndirectW(&If);
	GetDlgItem(IDC_EDIT_COM1)->SetFont(&font);
	font.Detach();

	font.CreateFontIndirectW(&If);
	GetDlgItem(IDC_EDIT_COM2)->SetFont(&font);
	font.Detach();

	/*font.CreatePointFont(190, _T("휴먼매직체"));
	GetDlgItem(IDC_EDIT_ME)->SetFont(&font);
	font.Detach();*/

	/*font.CreatePointFont(190, _T("휴먼매직체"));
	GetDlgItem(IDC_EDIT_COM1)->SetFont(&font);
	font.Detach();

	font.CreatePointFont(190, _T("휴먼매직체"));
	GetDlgItem(IDC_EDIT_COM2)->SetFont(&font);
	font.Detach();*/

	if (!drawch && p > 0)	// 드로우 상황 아니고 남은패가 있으면
	{
		back = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_BACK"));
		backdc = CreateCompatibleDC(dc);
		SelectObject(backdc, back);
		BitBlt(dc, cd.deck[0].x, cd.deck[0].y, 47, 76, backdc, 0, 0, SRCCOPY);	// 뒷면 그리기
		DeleteDC(backdc);
		DeleteObject(back);
	}

	// 이미지 출력

	card[0] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_1"));
	card[1] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_2"));
	card[2] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_3"));
	card[3] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_4"));
	card[4] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_5"));
	card[5] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_6"));
	card[6] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_7"));
	card[7] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_8"));
	card[8] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_9"));
	card[9] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_10"));
	card[10] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_11"));
	card[11] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_12"));
	card[12] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_13"));
	card[13] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_14"));
	card[14] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_15"));
	card[15] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_16"));
	card[16] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_17"));
	card[17] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_18"));
	card[18] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_19"));
	card[19] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_20"));
	card[20] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_21"));
	card[21] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_22"));
	card[22] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_23"));
	card[23] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_24"));
	card[24] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_25"));
	card[25] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_26"));
	card[26] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_27"));
	card[27] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_28"));
	card[28] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_29"));
	card[29] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_30"));
	card[30] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_31"));
	card[31] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_32"));
	card[32] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_33"));
	card[33] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_34"));
	card[34] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_35"));
	card[35] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_36"));
	card[36] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_37"));
	card[37] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_38"));
	card[38] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_39"));
	card[39] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_40"));
	card[40] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_41"));
	card[41] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_42"));
	card[42] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_43"));
	card[43] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_44"));
	card[44] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_45"));
	card[45] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_46"));
	card[46] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_47"));
	card[47] = LoadBitmap(AfxGetInstanceHandle(), _T("IDB_48"));

	for (i = 0; i < 48; i++)
	{
		carddc[i] = CreateCompatibleDC(dc);
	}
	for (i = 0; i < 48; i++)
	{
		SelectObject(carddc[i], card[i]);
	}

	if (drawch)	// 드로우 상황이면
	{
		BitBlt(dc, cd.deck[0].x, cd.deck[0].y, 49, 75, carddc[cd.deck[p].c], 0, 0, SRCCOPY);
	}
	for (i = 0; i < 12; i++)	// 홀 카드 그리기
	{
		if (cd.holecard[i].c != -1)
			BitBlt(dc, cd.holecard[i].x, cd.holecard[i].y, 49, 75, carddc[cd.holecard[i].c], 0, 0, SRCCOPY);
	}
	for (i = 0; i < 10; i++)	// 개인 카드 그리기
	{
		if (cd.mycard[i].c != -1)
			BitBlt(dc, cd.mycard[i].x, cd.mycard[i].y, 49, 75, carddc[cd.mycard[i].c], 0, 0, SRCCOPY);
		if (cd.comcard[i].c != -1)
			BitBlt(dc, cd.comcard[i].x, cd.comcard[i].y, 49, 75, carddc[cd.comcard[i].c], 0, 0, SRCCOPY);

	}
	for (i = 0; i < 12; i++)	// 맞출때 카드 그리기
	{
		if (cd.snap[i].c != -1)
			BitBlt(dc, cd.snap[i].x, cd.snap[i].y, 49, 75, carddc[cd.snap[i].c], 0, 0, SRCCOPY);
	}
	for (i = 0; i < 5; i++)	// 20점 모음
	{
		if (cd.my20[i].c != -1)
			BitBlt(dc, cd.my20[i].x, cd.my20[i].y, 49, 75, carddc[cd.my20[i].c], 0, 0, SRCCOPY);
		if (cd.com20[i].c != -1)
			BitBlt(dc, cd.com20[i].x, cd.com20[i].y, 49, 75, carddc[cd.com20[i].c], 0, 0, SRCCOPY);

	}
	for (i = 0; i < 9; i++)	// 10점 모음
	{
		if (cd.my10[i].c != -1)
			BitBlt(dc, cd.my10[i].x, cd.my10[i].y, 49, 75, carddc[cd.my10[i].c], 0, 0, SRCCOPY);
		if (cd.com10[i].c != -1)
			BitBlt(dc, cd.com10[i].x, cd.com10[i].y, 49, 75, carddc[cd.com10[i].c], 0, 0, SRCCOPY);
	}

	for (i = 0; i < 10; i++)	// 5점 모음
	{
		if (cd.my5[i].c != -1)
			BitBlt(dc, cd.my5[i].x, cd.my5[i].y, 49, 75, carddc[cd.my5[i].c], 0, 0, SRCCOPY);
		if (cd.com5[i].c != -1)
			BitBlt(dc, cd.com5[i].x, cd.com5[i].y, 49, 75, carddc[cd.com5[i].c], 0, 0, SRCCOPY);
	}
	for (i = 0; i < 24; i++)	// 0점 모음
	{
		if (cd.my0[i].c != -1)
			BitBlt(dc, cd.my0[i].x, cd.my0[i].y, 49, 75, carddc[cd.my0[i].c], 0, 0, SRCCOPY);
		if (cd.com0[i].c != -1)
			BitBlt(dc, cd.com0[i].x, cd.com0[i].y, 49, 75, carddc[cd.com0[i].c], 0, 0, SRCCOPY);
	}
	if (mode == 3)
	{
		for (i = 0; i < 10; i++)	// 개인 카드 그리기
		{
			if (cd.vilcard[i].c != -1)
				BitBlt(dc, cd.vilcard[i].x, cd.vilcard[i].y, 49, 75, carddc[cd.vilcard[i].c], 0, 0, SRCCOPY);
		}
		for (i = 0; i < 5; i++)	// 20점 모음
		{
			if (cd.vil20[i].c != -1)
				BitBlt(dc, cd.vil20[i].x, cd.vil20[i].y, 49, 75, carddc[cd.vil20[i].c], 0, 0, SRCCOPY);
		}
		for (i = 0; i < 9; i++)	// 10점 모음
		{
			if (cd.vil10[i].c != -1)
				BitBlt(dc, cd.vil10[i].x, cd.vil10[i].y, 49, 75, carddc[cd.vil10[i].c], 0, 0, SRCCOPY);
		}
		for (i = 0; i < 10; i++)	// 5점 모음
		{
			if (cd.vil5[i].c != -1)
				BitBlt(dc, cd.vil5[i].x, cd.vil5[i].y, 49, 75, carddc[cd.vil5[i].c], 0, 0, SRCCOPY);
		}
		for (i = 0; i < 24; i++)	// 0점 모음
		{
			if (cd.vil0[i].c != -1)
				BitBlt(dc, cd.vil0[i].x, cd.vil0[i].y, 49, 75, carddc[cd.vil0[i].c], 0, 0, SRCCOPY);
		}
	}
	for (i = 0; i < 48; i++)
	{
		DeleteDC(carddc[i]);
		DeleteObject(card[i]);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CHwaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHwaDlg::OnClickedButt2p()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int i, r, ch;
	char sctext[100];

	srand((unsigned)time(NULL));

	// 처음 패섞고 판 세팅

	GetDlgItem(IDC_BUTT_3p)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTT_3p)->SetWindowText(_T("3인"));
	GetDlgItem(IDC_BUTT_2p)->SetWindowText(_T("2인"));
	OnInitDialog();

	mode = 2;

	sprintf_s(sctext, "내 점수: %d", mysc);
	myscore = sctext;
	sprintf_s(sctext, "컴 점수: %d", com1sc);
	com1score = sctext;
	sprintf_s(sctext, "");
	com2score = sctext;
	UpdateData(FALSE);

	endch = 1;
	p = 0;
	while (p < 48)
	{
		ch = 1;
		r = rand() % 48;
		for (i = 0; i < p; i++)
		{
			if (r == cd.deck[i].c)
			{
				ch = 0;	// 중복이면 카드 덱에 안집어넣음
				i = p;	// 반복문 종료
			}
		}
		if (ch)
		{
			cd.deck[p].c = r;	// 중복이 아니면 덱에 집어넣음
			p++;	// 넣은 카드 수 증가
		}
	}

	for (i = 0; i < 8; i++)	// 8장 홀 카드 세팅
	{
		p--;	// 덱 감소(48이 초기값)
		cd.holecard[i].c = cd.deck[p].c;
	}

	for (i = 0; i < 10; i++)	// 내카드 밑 컴카드 세팅
	{
		p--;
		cd.mycard[i].c = cd.deck[p].c;
		p--;
		cd.comcard[i].c = cd.deck[p].c;
	}
	InvalidateRect(NULL, TRUE);
	UpdateWindow();

	GetDlgItem(IDC_BUTT_2p)->EnableWindow(FALSE);	// 패섞기 금지
}


void CHwaDlg::OnClickedButt3p()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int i, r, ch;
	char sctext[100];

	srand((unsigned)time(NULL));

	// 처음 패섞고 판 세팅

	GetDlgItem(IDC_BUTT_2p)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTT_2p)->SetWindowText(_T("2인"));
	GetDlgItem(IDC_BUTT_3p)->SetWindowText(_T("3인"));
	OnInitDialog();

	mode = 3;

	sprintf_s(sctext, "내 점수: %d", mysc);
	myscore = sctext;
	sprintf_s(sctext, "컴 점수: %d", com1sc);
	com1score = sctext;
	sprintf_s(sctext, "컴2 점수: %d", com2sc);
	com2score = sctext;
	UpdateData(FALSE);

	endch = 1;
	p = 0;
	while (p < 48)
	{
		ch = 1;
		r = rand() % 48;
		for (i = 0; i < p; i++)
		{
			if (r == cd.deck[i].c)
			{
				ch = 0;	// 중복이면 카드 덱에 안집어넣음
				i = p;	// 반복문 종료
			}
		}
		if (ch)
		{
			cd.deck[p].c = r;	// 중복이 아니면 덱에 집어넣음
			p++;	// 넣은 카드 수 증가
		}
	}

	for (i = 0; i < 6; i++)	// 6장 홀 카드 세팅
	{
		p--;	// 덱 감소(48이 초기값)
		cd.holecard[i].c = cd.deck[p].c;
	}

	for (i = 0; i < 7; i++)	// 내카드 밑 컴카드 세팅
	{
		p--;
		cd.mycard[i].c = cd.deck[p].c;
		p--;
		cd.comcard[i].c = cd.deck[p].c;
		p--;
		cd.vilcard[i].c = cd.deck[p].c;
	}
	InvalidateRect(NULL, TRUE);
	UpdateWindow();

	GetDlgItem(IDC_BUTT_3p)->EnableWindow(FALSE);	// 패섞기 금지
}


void CHwaDlg::OnClickedButtExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OnOK();
}


void CHwaDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	int i, high, clicki, highi, clch, pairch, targetsc, r;
	char sctext[100];

	if (endch)
	{
		if (myturn)
		{
			clch = 0;
			pairch = 0;
			high = -1;
			for (i = 0; i < 10; i++)
			{
				if (((cd.mycard[i].x <= point.x) && (point.x <= cd.mycard[i].x + 49)) && ((cd.mycard[i].y <= point.y) && (point.y <= cd.mycard[i].y + 75)))	// 내 카드 클릭했는지 검사
				{
					if (cd.mycard[i].c != -1)
					{
						clicki = i;	// 클릭한 카드 기억
						i = 10;
						clch = 1;
					}
				}
			}
			if (clch)
			{
				for (i = 0; i < 12; i++)
				{
					if (cd.holecard[i].c != -1)
					{
						if ((cd.mycard[clicki].c / 4) == (cd.holecard[i].c / 4))	// 같은 월이면
						{
							if (high < score[cd.holecard[i].c])	//	높은 점수의 패 찾음
							{
								high = score[cd.holecard[i].c];	// 점수 기억
								highi = i;			// 홀에서 인덱스 기억
								pairch = 1;		// 페어 찾음
							}
						}
					}
				}
				if (pairch)	// 맞는게 있으면
				{
					cd.snap[highi].c = cd.mycard[clicki].c;	// 친카드로 이동
					cd.mycard[clicki].c = -1;	// 내카드 지우기
					InvalidateRect(NULL, true);
					UpdateWindow();
					Sleep(500);
				}
				else
				{
					for (i = 0; i < 12; i++)	// 맞는게 없으면 내려놓기
					{
						if (cd.holecard[i].c == -1)
						{
							cd.holecard[i].c = cd.mycard[clicki].c;	// 내려놓기
							cd.mycard[clicki].c = -1;	// 내카드 지우기
						}
					}
					InvalidateRect(NULL, TRUE);
					UpdateWindow();
					Sleep(500);
				}

				if (pairch)	// 점수 추가하고 카드 가져오기
				{
					switch (score[cd.holecard[highi].c])
					{
					case 20:
						mysc += 20;
						for (i = 0; i < 5; i++)
						{
							if (cd.my20[i].c == -1)
							{
								cd.my20[i].c = cd.holecard[highi].c;
								cd.holecard[highi].c = -1;
								i = 5;
							}
						}
						break;
					case 10:
						mysc += 10;
						for (i = 0; i < 9; i++)
						{
							if (cd.my10[i].c == -1)
							{
								cd.my10[i].c = cd.holecard[highi].c;
								cd.holecard[highi].c = -1;
								i = 9;
							}
						}
						break;
					case 5:
						mysc += 5;
						for (i = 0; i < 10; i++)
						{
							if (cd.my5[i].c == -1)
							{
								cd.my5[i].c = cd.holecard[highi].c;
								cd.holecard[highi].c = -1;
								i = 10;
							}
						}
						break;
					case 0:
						for (i = 0; i < 24; i++)
						{
							if (cd.my0[i].c == -1)
							{
								cd.my0[i].c = cd.holecard[highi].c;
								cd.holecard[highi].c = -1;
								i = 24;
							}
						}
						break;
					}
					switch (score[cd.snap[highi].c])
					{
					case 20:
						mysc += 20;
						for (i = 0; i < 5; i++)
						{
							if (cd.my20[i].c == -1)
							{
								cd.my20[i].c = cd.snap[highi].c;
								cd.snap[highi].c = -1;
								i = 5;
							}
						}
						break;
					case 10:
						mysc += 10;
						for (i = 0; i < 9; i++)
						{
							if (cd.my10[i].c == -1)
							{
								cd.my10[i].c = cd.snap[highi].c;
								cd.snap[highi].c = -1;
								i = 9;
							}
						}
						break;
					case 5:
						mysc += 5;
						for (i = 0; i < 10; i++)
						{
							if (cd.my5[i].c == -1)
							{
								cd.my5[i].c = cd.snap[highi].c;
								cd.snap[highi].c = -1;
								i = 10;
							}
						}
						break;
					case 0:
						for (i = 0; i < 24; i++)
						{
							if (cd.my0[i].c == -1)
							{
								cd.my0[i].c = cd.snap[highi].c;
								cd.snap[highi].c = -1;
								i = 24;
							}
						}
						break;
					}
					sprintf_s(sctext, "내 점수: %d", mysc);
					myscore = sctext;
					UpdateData(FALSE);
					InvalidateRect(NULL, TRUE);
					UpdateWindow();
					Sleep(500);
				}
				drawch = 1;	// 드로우 모드
				p--;	// 덱 인덱스 변경
				pairch = 0;
				high = -1;
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
				Sleep(500);
				drawch = 0;
				for (i = 0; i < 12; i++)
				{
					if (cd.holecard[i].c != -1)
					{
						if ((cd.deck[p].c / 4) == (cd.holecard[i].c / 4))	// 같은 월이면
						{
							if (high < score[cd.holecard[i].c])	//	높은 점수의 패 찾음
							{
								high = score[cd.holecard[i].c];	// 점수 기억
								highi = i;			// 홀에서 인덱스 기억
								pairch = 1;		// 페어 찾음
							}
						}
					}
				}
				if (pairch)	// 맞는게 있으면
				{
					cd.snap[highi].c = cd.deck[p].c;	// 친카드로 이동
					cd.deck[p].c = -1;	// 덱 카드 지우기
					InvalidateRect(NULL, true);
					UpdateWindow();
					Sleep(500);
				}
				else
				{
					for (i = 0; i < 12; i++)	// 맞는게 없으면 내려놓기
					{
						if (cd.holecard[i].c == -1)
						{
							cd.holecard[i].c = cd.deck[p].c;	// 내려놓기
							cd.deck[p].c = -1;	// 덱 카드 지우기
						}
					}
					InvalidateRect(NULL, TRUE);
					UpdateWindow();
					Sleep(500);
				}

				if (pairch)	// 점수 추가하고 카드 가져오기
				{
					switch (score[cd.holecard[highi].c])
					{
					case 20:
						mysc += 20;
						for (i = 0; i < 5; i++)
						{
							if (cd.my20[i].c == -1)
							{
								cd.my20[i].c = cd.holecard[highi].c;
								cd.holecard[highi].c = -1;
								i = 5;
							}
						}
						break;
					case 10:
						mysc += 10;
						for (i = 0; i < 9; i++)
						{
							if (cd.my10[i].c == -1)
							{
								cd.my10[i].c = cd.holecard[highi].c;
								cd.holecard[highi].c = -1;
								i = 9;
							}
						}
						break;
					case 5:
						mysc += 5;
						for (i = 0; i < 10; i++)
						{
							if (cd.my5[i].c == -1)
							{
								cd.my5[i].c = cd.holecard[highi].c;
								cd.holecard[highi].c = -1;
								i = 10;
							}
						}
						break;
					case 0:
						for (i = 0; i < 24; i++)
						{
							if (cd.my0[i].c == -1)
							{
								cd.my0[i].c = cd.holecard[highi].c;
								cd.holecard[highi].c = -1;
								i = 24;
							}
						}
						break;
					}
					switch (score[cd.snap[highi].c])
					{
					case 20:
						mysc += 20;
						for (i = 0; i < 5; i++)
						{
							if (cd.my20[i].c == -1)
							{
								cd.my20[i].c = cd.snap[highi].c;
								cd.snap[highi].c = -1;
								i = 5;
							}
						}
						break;
					case 10:
						mysc += 10;
						for (i = 0; i < 9; i++)
						{
							if (cd.my10[i].c == -1)
							{
								cd.my10[i].c = cd.snap[highi].c;
								cd.snap[highi].c = -1;
								i = 9;
							}
						}
						break;
					case 5:
						mysc += 5;
						for (i = 0; i < 10; i++)
						{
							if (cd.my5[i].c == -1)
							{
								cd.my5[i].c = cd.snap[highi].c;
								cd.snap[highi].c = -1;
								i = 10;
							}
						}
						break;
					case 0:
						for (i = 0; i < 24; i++)
						{
							if (cd.my0[i].c == -1)
							{
								cd.my0[i].c = cd.snap[highi].c;
								cd.snap[highi].c = -1;
								i = 24;
							}
						}
						break;
					}
					sprintf_s(sctext, "내 점수: %d", mysc);
					myscore = sctext;
					UpdateData(FALSE);
					InvalidateRect(NULL, TRUE);
					UpdateWindow();
					Sleep(500);
				}
				myturn = 0;
				com1turn = 1;
			}
		}
		if (com1turn && (clch == 1))	// 컴퓨터 차례
		{
			pairch = 0;
			targetsc = 20;
			for (i = 0; i < 12; i++)	// 높은 점수 찾기
			{
				switch (targetsc)
				{
				case 20:
					if (cd.holecard[i].c != -1)
					{
						if (score[cd.holecard[i].c] == targetsc)
						{
							for (int j = 0; j < 10; j++)	// 컴카드 중 맞는게 있는지 검사
							{
								if (cd.comcard[j].c != -1)
								{
									if ((cd.holecard[i].c / 4) == (cd.comcard[j].c / 4))	// 같은 짝이면
									{
										highi = i;
										cd.snap[i].c = cd.comcard[j].c;
										cd.comcard[j].c = -1;
										j = 10;
										i = 12;
										pairch = 1;
									}
								}
							}
						}
					}
					if (i == 11 && pairch == 0)	// 없으면 다음 점수 찾기
					{
						i = -1;
						targetsc = 10;
					}
					break;
				case 10:
					if (cd.holecard[i].c != -1)
					{
						if (score[cd.holecard[i].c] == targetsc)
						{
							for (int j = 0; j < 10; j++)	// 컴카드 중 맞는게 있는지 검사
							{
								if (cd.comcard[j].c != -1)
								{
									if ((cd.holecard[i].c / 4) == (cd.comcard[j].c / 4))	// 같은 짝이면
									{
										highi = i;
										cd.snap[i].c = cd.comcard[j].c;
										cd.comcard[j].c = -1;
										j = 10;
										i = 12;
										pairch = 1;
									}
								}
							}
						}
					}
					if (i == 11 && pairch == 0)
					{
						i = -1;
						targetsc = 5;
					}
					break;
				case 5:
					if (cd.holecard[i].c != -1)
					{
						if (score[cd.holecard[i].c] == targetsc)
						{
							for (int j = 0; j < 10; j++)	// 컴카드 중 맞는게 있는지 검사
							{
								if (cd.comcard[j].c != -1)
								{
									if ((cd.holecard[i].c / 4) == (cd.comcard[j].c / 4))	// 같은 짝이면
									{
										highi = i;
										cd.snap[i].c = cd.comcard[j].c;
										cd.comcard[j].c = -1;
										j = 10;
										i = 12;
										pairch = 1;
									}
								}
							}
						}
					}
					if (i == 11 && pairch == 0)
					{
						i = -1;
						targetsc = 0;
					}
					break;
				case 0:
					if (cd.holecard[i].c != -1)
					{
						if (score[cd.holecard[i].c] == targetsc)
						{
							for (int j = 0; j < 10; j++)	// 컴카드 중 맞는게 있는지 검사
							{
								if (cd.comcard[j].c != -1)
								{
									if ((cd.holecard[i].c / 4) == (cd.comcard[j].c / 4))	// 같은 짝이면
									{
										highi = i;
										cd.snap[i].c = cd.comcard[j].c;
										cd.comcard[j].c = -1;
										j = 10;
										i = 12;
										pairch = 1;
									}
								}
							}
						}
					}
					break;
				}
			}
			InvalidateRect(NULL, TRUE);
			UpdateWindow();
			Sleep(500);
			if (pairch)	// 맞으면 가져오고 점수 추가
			{
				switch (score[cd.holecard[highi].c])
				{
				case 20:
					com1sc += 20;
					for (i = 0; i < 5; i++)
					{
						if (cd.com20[i].c == -1)
						{
							cd.com20[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 5;
						}
					}
					break;
				case 10:
					com1sc += 10;
					for (i = 0; i < 9; i++)
					{
						if (cd.com10[i].c == -1)
						{
							cd.com10[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 9;
						}
					}
					break;
				case 5:
					com1sc += 5;
					for (i = 0; i < 10; i++)
					{
						if (cd.com5[i].c == -1)
						{
							cd.com5[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 10;
						}
					}
					break;
				case 0:
					for (i = 0; i < 24; i++)
					{
						if (cd.com0[i].c == -1)
						{
							cd.com0[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 24;
						}
					}
					break;
				}
				switch (score[cd.snap[highi].c])
				{
				case 20:
					com1sc += 20;
					for (i = 0; i < 5; i++)
					{
						if (cd.com20[i].c == -1)
						{
							cd.com20[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 5;
						}
					}
					break;
				case 10:
					com1sc += 10;
					for (i = 0; i < 9; i++)
					{
						if (cd.com10[i].c == -1)
						{
							cd.com10[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 9;
						}
					}
					break;
				case 5:
					com1sc += 5;
					for (i = 0; i < 10; i++)
					{
						if (cd.com5[i].c == -1)
						{
							cd.com5[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 10;
						}
					}
					break;
				case 0:
					for (i = 0; i < 24; i++)
					{
						if (cd.com0[i].c == -1)
						{
							cd.com0[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 24;
						}
					}
					break;
				}
				sprintf_s(sctext, "컴 점수: %d", com1sc);
				com1score = sctext;
				UpdateData(FALSE);
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
				Sleep(500);
			}
			else	// 맞는게 없으면
			{
				for (i = 0; i < 10; i++)
				{
					if (cd.comcard[i].c != -1)
					{
						r = i;
						i = 10;
					}
				}
				for (i = 0; i < 12; i++)	// 맞는게 없으면 내려놓기
				{
					if (cd.holecard[i].c == -1)
					{
						cd.holecard[i].c = cd.comcard[r].c;	// 내려놓기
						cd.comcard[r].c = -1;	// 낸 카드 지우기
					}
				}
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
				Sleep(500);
			}
			drawch = 1;	// 드로우 모드
			p--;	// 덱 인덱스 변경
			pairch = 0;
			high = -1;
			InvalidateRect(NULL, TRUE);	// 뒤집은거 보여주기
			UpdateWindow();
			Sleep(500);
			drawch = 0;
			for (i = 0; i < 12; i++)
			{
				if (cd.holecard[i].c != -1)
				{
					if ((cd.deck[p].c / 4) == (cd.holecard[i].c / 4))	// 같은 월이면
					{
						if (high < score[cd.holecard[i].c])	//	높은 점수의 패 찾음
						{
							high = score[cd.holecard[i].c];	// 점수 기억
							highi = i;			// 홀에서 인덱스 기억
							pairch = 1;		// 페어 찾음
						}
					}
				}
			}
			if (pairch)	// 맞는게 있으면
			{
				cd.snap[highi].c = cd.deck[p].c;	// 친카드로 이동
				cd.deck[p].c = -1;	// 덱 카드 지우기
				InvalidateRect(NULL, true);
				UpdateWindow();
				Sleep(500);
			}
			else
			{
				for (i = 0; i < 12; i++)	// 맞는게 없으면 내려놓기
				{
					if (cd.holecard[i].c == -1)
					{
						cd.holecard[i].c = cd.deck[p].c;	// 내려놓기
						cd.deck[p].c = -1;	// 덱 카드 지우기
					}
				}
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
				Sleep(500);
			}

			if (pairch)	// 점수 추가하고 카드 가져오기
			{
				switch (score[cd.holecard[highi].c])
				{
				case 20:
					com1sc += 20;
					for (i = 0; i < 5; i++)
					{
						if (cd.com20[i].c == -1)
						{
							cd.com20[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 5;
						}
					}
					break;
				case 10:
					com1sc += 10;
					for (i = 0; i < 9; i++)
					{
						if (cd.com10[i].c == -1)
						{
							cd.com10[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 9;
						}
					}
					break;
				case 5:
					com1sc += 5;
					for (i = 0; i < 10; i++)
					{
						if (cd.com5[i].c == -1)
						{
							cd.com5[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 10;
						}
					}
					break;
				case 0:
					for (i = 0; i < 24; i++)
					{
						if (cd.com0[i].c == -1)
						{
							cd.com0[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 24;
						}
					}
					break;
				}
				switch (score[cd.snap[highi].c])
				{
				case 20:
					com1sc += 20;
					for (i = 0; i < 5; i++)
					{
						if (cd.com20[i].c == -1)
						{
							cd.com20[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 5;
						}
					}
					break;
				case 10:
					com1sc += 10;
					for (i = 0; i < 9; i++)
					{
						if (cd.com10[i].c == -1)
						{
							cd.com10[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 9;
						}
					}
					break;
				case 5:
					com1sc += 5;
					for (i = 0; i < 10; i++)
					{
						if (cd.com5[i].c == -1)
						{
							cd.com5[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 10;
						}
					}
					break;
				case 0:
					for (i = 0; i < 24; i++)
					{
						if (cd.com0[i].c == -1)
						{
							cd.com0[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 24;
						}
					}
					break;
				}
				sprintf_s(sctext, "컴 점수: %d", com1sc);
				com1score = sctext;
				UpdateData(FALSE);
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
				if (mode == 3)
					Sleep(500);
			}
			if (mode == 2)
			{
				com1turn = 0;
				myturn = 1;
			}
			else if (mode == 3)
			{
				com1turn = 0;
				com2turn = 1;
			}
		}
		if (com2turn)
		{
			pairch = 0;
			targetsc = 20;
			for (i = 0; i < 12; i++)	// 높은 점수 찾기
			{
				switch (targetsc)
				{
				case 20:
					if (cd.holecard[i].c != -1)
					{
						if (score[cd.holecard[i].c] == targetsc)
						{
							for (int j = 0; j < 10; j++)	// 컴카드 중 맞는게 있는지 검사
							{
								if (cd.vilcard[j].c != -1)
								{
									if ((cd.holecard[i].c / 4) == (cd.vilcard[j].c / 4))	// 같은 짝이면
									{
										highi = i;
										cd.snap[i].c = cd.vilcard[j].c;
										cd.vilcard[j].c = -1;
										j = 10;
										i = 12;
										pairch = 1;
									}
								}
							}
						}
					}
					if (i == 11 && pairch == 0)	// 없으면 다음 점수 찾기
					{
						i = -1;
						targetsc = 10;
					}
					break;
				case 10:
					if (cd.holecard[i].c != -1)
					{
						if (score[cd.holecard[i].c] == targetsc)
						{
							for (int j = 0; j < 10; j++)	// 컴카드 중 맞는게 있는지 검사
							{
								if (cd.vilcard[j].c != -1)
								{
									if ((cd.holecard[i].c / 4) == (cd.vilcard[j].c / 4))	// 같은 짝이면
									{
										highi = i;
										cd.snap[i].c = cd.vilcard[j].c;
										cd.vilcard[j].c = -1;
										j = 10;
										i = 12;
										pairch = 1;
									}
								}
							}
						}
					}
					if (i == 11 && pairch == 0)
					{
						i = -1;
						targetsc = 5;
					}
					break;
				case 5:
					if (cd.holecard[i].c != -1)
					{
						if (score[cd.holecard[i].c] == targetsc)
						{
							for (int j = 0; j < 10; j++)	// 컴카드 중 맞는게 있는지 검사
							{
								if (cd.vilcard[j].c != -1)
								{
									if ((cd.holecard[i].c / 4) == (cd.vilcard[j].c / 4))	// 같은 짝이면
									{
										highi = i;
										cd.snap[i].c = cd.vilcard[j].c;
										cd.vilcard[j].c = -1;
										j = 10;
										i = 12;
										pairch = 1;
									}
								}
							}
						}
					}
					if (i == 11 && pairch == 0)
					{
						i = -1;
						targetsc = 0;
					}
					break;
				case 0:
					if (cd.holecard[i].c != -1)
					{
						if (score[cd.holecard[i].c] == targetsc)
						{
							for (int j = 0; j < 10; j++)	// 컴카드 중 맞는게 있는지 검사
							{
								if (cd.vilcard[j].c != -1)
								{
									if ((cd.holecard[i].c / 4) == (cd.vilcard[j].c / 4))	// 같은 짝이면
									{
										highi = i;
										cd.snap[i].c = cd.vilcard[j].c;
										cd.vilcard[j].c = -1;
										j = 10;
										i = 12;
										pairch = 1;
									}
								}
							}
						}
					}
					break;
				}
			}
			InvalidateRect(NULL, TRUE);
			UpdateWindow();
			Sleep(500);
			if (pairch)	// 맞으면 가져오고 점수 추가
			{
				switch (score[cd.holecard[highi].c])
				{
				case 20:
					com2sc += 20;
					for (i = 0; i < 5; i++)
					{
						if (cd.vil20[i].c == -1)
						{
							cd.vil20[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 5;
						}
					}
					break;
				case 10:
					com2sc += 10;
					for (i = 0; i < 9; i++)
					{
						if (cd.vil10[i].c == -1)
						{
							cd.vil10[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 9;
						}
					}
					break;
				case 5:
					com2sc += 5;
					for (i = 0; i < 10; i++)
					{
						if (cd.vil5[i].c == -1)
						{
							cd.vil5[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 10;
						}
					}
					break;
				case 0:
					for (i = 0; i < 24; i++)
					{
						if (cd.vil0[i].c == -1)
						{
							cd.vil0[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 24;
						}
					}
					break;
				}
				switch (score[cd.snap[highi].c])
				{
				case 20:
					com2sc += 20;
					for (i = 0; i < 5; i++)
					{
						if (cd.vil20[i].c == -1)
						{
							cd.vil20[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 5;
						}
					}
					break;
				case 10:
					com2sc += 10;
					for (i = 0; i < 9; i++)
					{
						if (cd.vil10[i].c == -1)
						{
							cd.vil10[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 9;
						}
					}
					break;
				case 5:
					com2sc += 5;
					for (i = 0; i < 10; i++)
					{
						if (cd.vil5[i].c == -1)
						{
							cd.vil5[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 10;
						}
					}
					break;
				case 0:
					for (i = 0; i < 24; i++)
					{
						if (cd.vil0[i].c == -1)
						{
							cd.vil0[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 24;
						}
					}
					break;
				}
				sprintf_s(sctext, "컴2 점수: %d", com2sc);
				com2score = sctext;
				UpdateData(FALSE);
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
				Sleep(500);
			}
			else	// 맞는게 없으면
			{
				for (i = 0; i < 10; i++)
				{
					if (cd.vilcard[i].c != -1)
					{
						r = i;
						i = 10;
					}
				}
				for (i = 0; i < 12; i++)	// 맞는게 없으면 내려놓기
				{
					if (cd.holecard[i].c == -1)
					{
						cd.holecard[i].c = cd.vilcard[r].c;	// 내려놓기
						cd.vilcard[r].c = -1;	// 낸 카드 지우기
					}
				}
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
				Sleep(500);
			}
			drawch = 1;	// 드로우 모드
			p--;	// 덱 인덱스 변경
			pairch = 0;
			high = -1;
			InvalidateRect(NULL, TRUE);	// 뒤집은거 보여주기
			UpdateWindow();
			Sleep(500);
			drawch = 0;
			for (i = 0; i < 12; i++)
			{
				if (cd.holecard[i].c != -1)
				{
					if ((cd.deck[p].c / 4) == (cd.holecard[i].c / 4))	// 같은 월이면
					{
						if (high < score[cd.holecard[i].c])	//	높은 점수의 패 찾음
						{
							high = score[cd.holecard[i].c];	// 점수 기억
							highi = i;			// 홀에서 인덱스 기억
							pairch = 1;		// 페어 찾음
						}
					}
				}
			}
			if (pairch)	// 맞는게 있으면
			{
				cd.snap[highi].c = cd.deck[p].c;	// 친카드로 이동
				cd.deck[p].c = -1;	// 덱 카드 지우기
				InvalidateRect(NULL, true);
				UpdateWindow();
				Sleep(500);
			}
			else
			{
				for (i = 0; i < 12; i++)	// 맞는게 없으면 내려놓기
				{
					if (cd.holecard[i].c == -1)
					{
						cd.holecard[i].c = cd.deck[p].c;	// 내려놓기
						cd.deck[p].c = -1;	// 덱 카드 지우기
					}
				}
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
			}

			if (pairch)	// 점수 추가하고 카드 가져오기
			{
				switch (score[cd.holecard[highi].c])
				{
				case 20:
					com2sc += 20;
					for (i = 0; i < 5; i++)
					{
						if (cd.vil20[i].c == -1)
						{
							cd.vil20[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 5;
						}
					}
					break;
				case 10:
					com2sc += 10;
					for (i = 0; i < 9; i++)
					{
						if (cd.vil10[i].c == -1)
						{
							cd.vil10[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 9;
						}
					}
					break;
				case 5:
					com2sc += 5;
					for (i = 0; i < 10; i++)
					{
						if (cd.vil5[i].c == -1)
						{
							cd.vil5[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 10;
						}
					}
					break;
				case 0:
					for (i = 0; i < 24; i++)
					{
						if (cd.vil0[i].c == -1)
						{
							cd.vil0[i].c = cd.holecard[highi].c;
							cd.holecard[highi].c = -1;
							i = 24;
						}
					}
					break;
				}
				switch (score[cd.snap[highi].c])
				{
				case 20:
					com2sc += 20;
					for (i = 0; i < 5; i++)
					{
						if (cd.vil20[i].c == -1)
						{
							cd.vil20[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 5;
						}
					}
					break;
				case 10:
					com2sc += 10;
					for (i = 0; i < 9; i++)
					{
						if (cd.vil10[i].c == -1)
						{
							cd.vil10[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 9;
						}
					}
					break;
				case 5:
					com2sc += 5;
					for (i = 0; i < 10; i++)
					{
						if (cd.vil5[i].c == -1)
						{
							cd.vil5[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 10;
						}
					}
					break;
				case 0:
					for (i = 0; i < 24; i++)
					{
						if (cd.vil0[i].c == -1)
						{
							cd.vil0[i].c = cd.snap[highi].c;
							cd.snap[highi].c = -1;
							i = 24;
						}
					}
					break;
				}
				sprintf_s(sctext, "컴2 점수: %d", com2sc);
				com2score = sctext;
				UpdateData(FALSE);
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
			}
			com2turn = 0;
			myturn = 1;
		}
		if (mode == 2 && p <= 0)	// 종료 조건
		{
			endch = 0;
			if (mysc > com1sc)
			{
				sprintf_s(sctext, "내 점수: %d   승리!!!", mysc);
				myscore = sctext;
				UpdateData(FALSE);
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
			}
			else if (mysc < com1sc)
			{
				sprintf_s(sctext, "내 점수: %d   패배...", mysc);
				myscore = sctext;
				UpdateData(FALSE);
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
			}
			else
			{
				sprintf_s(sctext, "내 점수: %d   무승부", mysc);
				myscore = sctext;
				UpdateData(FALSE);
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
			}
			GetDlgItem(IDC_BUTT_2p)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTT_2p)->SetWindowText(_T("2p 재경기"));
		}
		else if (mode == 3 && p <= 0)
		{
			endch = 0;
			if (mysc > com1sc && mysc > com2sc)
			{
				sprintf_s(sctext, "내 점수: %d   승리!!!", mysc);
				myscore = sctext;
				UpdateData(FALSE);
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
			}
			else if(mysc==com1sc && mysc == com2sc)
			{
				sprintf_s(sctext, "내 점수: %d   무승부", mysc);
				myscore = sctext;
				UpdateData(FALSE);
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
			}
			else
			{
				sprintf_s(sctext, "내 점수: %d   패배...", mysc);
				myscore = sctext;
				UpdateData(FALSE);
				InvalidateRect(NULL, TRUE);
				UpdateWindow();
			}
			GetDlgItem(IDC_BUTT_3p)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTT_3p)->SetWindowText(_T("3p 재경기"));
		}
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}