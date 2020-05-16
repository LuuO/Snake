
// SnakeDlg.cpp : 实现文件


#include "stdafx.h"
#include "Snake.h"
#include "SnakeDlg.h"
#include "afxdialogex.h"
#include "SnakeDefine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSnakeDlg 对话框

CSnakeDlg::CSnakeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SNAKE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSnakeDlg::~CSnakeDlg()
{
}

VOID CSnakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnakeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CSnakeDlg 消息处理程序G

BOOL CSnakeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//蛇，食物信息初始化
	m_wSnakeL = 13;
	m_ucarrSnakeP[0][X] = 15;m_ucarrSnakeP[0][Y] = 15;m_ucarrSnakeP[0][D] = DOWN;
	m_ucarrSnakeP[1][X] = 15;m_ucarrSnakeP[1][Y] = 14;m_ucarrSnakeP[1][D] = DOWN;
	m_ucarrSnakeP[2][X] = 15;m_ucarrSnakeP[2][Y] = 13;m_ucarrSnakeP[2][D] = DOWN;
	m_ucarrSnakeP[3][X] = 15;m_ucarrSnakeP[3][Y] = 12;m_ucarrSnakeP[3][D] = DOWN;
	m_ucarrSnakeP[4][X] = 15;m_ucarrSnakeP[4][Y] = 11;m_ucarrSnakeP[4][D] = DOWN;
	m_ucarrSnakeP[5][X] = 15;m_ucarrSnakeP[5][Y] = 10;m_ucarrSnakeP[5][D] = DOWN;
	m_ucarrSnakeP[6][X] = 15;m_ucarrSnakeP[6][Y] = 9;m_ucarrSnakeP[6][D] = DOWN;
	m_ucarrSnakeP[7][X] = 15;m_ucarrSnakeP[7][Y] = 8;m_ucarrSnakeP[7][D] = DOWN;
	m_ucarrSnakeP[8][X] = 15;m_ucarrSnakeP[8][Y] = 7;m_ucarrSnakeP[8][D] = DOWN;
	m_ucarrSnakeP[9][X] = 14;m_ucarrSnakeP[9][Y] = 7;m_ucarrSnakeP[9][D] = RIGHT;
	m_ucarrSnakeP[10][X] = 13;m_ucarrSnakeP[10][Y] = 7;m_ucarrSnakeP[10][D] = RIGHT;
	m_ucarrSnakeP[11][X] = 12;m_ucarrSnakeP[11][Y] = 7;m_ucarrSnakeP[11][D] = RIGHT;
	m_ucarrSnakeP[12][X] = 11;m_ucarrSnakeP[12][Y] = 7;m_ucarrSnakeP[12][D] = RIGHT;
	m_ucarrOldBody[X] = 10;m_ucarrOldBody[Y] = 7;m_ucarrOldBody[D] = RIGHT;
	m_ucarrFoodP[X] = 11;m_ucarrFoodP[Y] = 11;

	CreateDisplay();

	SetTimer(TIMER_CREATE, 10, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

VOID CSnakeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		Display(DP_MAP);
		if (m_bPaused)
			Display(DP_EXP);
		CDialogEx::OnPaint();
	}
}

//按键
BOOL CSnakeDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//非暂停模式
		if (!m_bPaused)
		{
			switch (pMsg->wParam)
			{
			case VK_RETURN:
				if (m_bStarted == FALSE)
				{
					if (KillTimer(TIMER_SNAKEADV))
						if (m_uiSMS == 77)
							m_uiSMS = 177;
					StartGame();
					return true;
					break;
				}
			case 'Q':
			case 'P':
			case 'C':
			case VK_F1:
			case VK_TAB:
			case VK_ESCAPE:
				PauseGame();
				return true;
				break;
			default:
				break;
			}
			//只有游戏开始后才可以反应的按键（键盘预输入技术）
			if (m_bStarted == TRUE)
				switch (pMsg->wParam)
				{
				case 'W':
				case VK_UP:
					if (m_bPressed && (m_uiSMS < 300))
					{
						if (m_ucInput != DOWN)
						{
							m_ucNextInput = UP;
							m_bNextPressed = TRUE;
							m_bPressed = FALSE;
						}
						else
								m_ucInput = UP;
					}
					else
						if (m_ucarrSnakeP[0][D] != DOWN)
						{
							m_ucInput = UP;
							if (m_ucarrSnakeP[0][D] != UP)
								m_bPressed = TRUE;
						}
					break;
				case 'S':
				case VK_DOWN:
					if (m_bPressed && (m_uiSMS < 300))
					{
						if (m_ucInput != UP)
						{
							m_ucNextInput = DOWN;
							m_bNextPressed = TRUE;
							m_bPressed = FALSE;
						}
						else
							m_ucInput = DOWN;
					}
					else
						if (m_ucarrSnakeP[0][D] != UP)
						{
							m_ucInput = DOWN;
							if (m_ucarrSnakeP[0][D] != DOWN)
								m_bPressed = TRUE;
						}
					break;
				case 'A':
				case VK_LEFT:
					if (m_bPressed && (m_uiSMS < 300))
					{
						if (m_ucInput != RIGHT)
						{
							m_ucNextInput = LEFT;
							m_bNextPressed = TRUE;
							m_bPressed = FALSE;
						}
						else
							m_ucInput = LEFT;
					}
					else
						if (m_ucarrSnakeP[0][D] != RIGHT)
						{
							m_ucInput = LEFT;
							if (m_ucarrSnakeP[0][D] != LEFT)
								m_bPressed = TRUE;
						}
					break;
				case 'D':
				case VK_RIGHT:
					if (m_bPressed && (m_uiSMS < 300))
					{
						if (m_ucInput != LEFT)
						{
							m_ucNextInput = RIGHT;
							m_bNextPressed = TRUE;
							m_bPressed = FALSE;
						}
						else
							m_ucInput = RIGHT;
					}
					else
						if (m_ucarrSnakeP[0][D] != LEFT)
						{
							m_ucInput = RIGHT;
							if (m_ucarrSnakeP[0][D] != RIGHT)
								m_bPressed = TRUE;
						}
					break;
				default:
					break;
				}
		}
		//暂停模式
		else
		{
			switch (pMsg->wParam)
			{
			case 'W':
			case 'S':
			case 'A':
			case 'D':
			case VK_UP:
			case VK_DOWN:
			case VK_LEFT:
			case VK_RIGHT:
			case VK_TAB:
				ChangeFocus(pMsg->wParam);
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case VK_NUMPAD0:
			case VK_NUMPAD1:
			case VK_NUMPAD2:
			case VK_NUMPAD3:
			case VK_NUMPAD4:
			case VK_NUMPAD5:
			case VK_NUMPAD6:
			case VK_NUMPAD7:
			case VK_NUMPAD8:
			case VK_NUMPAD9:
				if (m_ucarrFocus[X] == 0 && m_ucarrFocus[Y] == 0)
					ChangeFocus(VK_TAB);
				else
					ChangeFocus(pMsg->wParam);
				break;
			case 'P':
			case 'C':
			case VK_F1:
			case VK_ESCAPE:
				ContinueGame(FALSE);
				break;
			case VK_RETURN:
				ContinueGame(TRUE);
				break;
			case 'Q':
				OnClose();
				break;
			default:
				break;
			}
		}
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//响应计时器
VOID CSnakeDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_SNAKEGO:
		SnakeGo(FALSE);
		break;
	case TIMER_SNAKEADV:
		SnakeGo(TRUE);
		break;
	case TIMER_CREATE:
		KillTimer(TIMER_CREATE);
		CreateSnakeWindow();
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

//新游戏
VOID CSnakeDlg::StartGame()
{
	WORD sgame_i;
	CHAR sgame_a;
	srand(GetTickCount());
	m_bStarted = TRUE;
	m_bCrashed = FALSE;

	if (m_ucNextMapSize)
	{
		m_ucMapSize = m_ucNextMapSize;
		m_ucNextMapSize = 0;
		ChangeWindowSize();
	}

	//蛇，食物信息初始化
	m_wSnakeL = 4;
	m_ucarrSnakeP[0][X] = 1 + rand() % (m_ucMapSize - 1 + 1);
	m_ucarrSnakeP[0][Y] = 1 + rand() % (m_ucMapSize - 1 + 1);
	if (m_ucarrSnakeP[0][X] < (m_ucMapSize / 2))
	{
		if (m_ucarrSnakeP[0][X] < 5)
			m_ucarrSnakeP[0][X] = 5;
		m_ucarrSnakeP[0][D] = RIGHT;
		m_ucInput = RIGHT;
		m_ucarrOldBody[D] = RIGHT;
		sgame_a = -1;
	}
	else
	{
		if (m_ucarrSnakeP[0][X] > m_ucMapSize - 5)
			m_ucarrSnakeP[0][X] = m_ucMapSize - 5;
		m_ucarrSnakeP[0][D] = LEFT;
		m_ucInput = LEFT;
		m_ucarrOldBody[D] = LEFT;
		sgame_a = 1;
	}
	if (m_ucarrSnakeP[0][Y] < 4)
		m_ucarrSnakeP[0][Y] = 4;
	if (m_ucarrSnakeP[0][Y] > m_ucMapSize - 4)
		m_ucarrSnakeP[0][Y] = m_ucMapSize - 4;
	for (sgame_i = 1;sgame_i < m_wSnakeL;sgame_i++)
	{
		m_ucarrSnakeP[sgame_i][X] = m_ucarrSnakeP[sgame_i - 1][X] + sgame_a;
		m_ucarrSnakeP[sgame_i][Y] = m_ucarrSnakeP[sgame_i - 1][Y];
		m_ucarrSnakeP[sgame_i][D] = m_ucarrSnakeP[sgame_i - 1][D];
	}
	m_ucarrOldBody[X] = m_ucarrSnakeP[m_wSnakeL - 1][X] + sgame_a;
	m_ucarrOldBody[Y] = m_ucarrSnakeP[m_wSnakeL - 1][Y];

	DrawBG();

	NewFood();
	DrawMap(DM_DAS, FALSE);
	Display(DP_MAP);

	if ((1000 - m_uiSMS) > 0)
		Sleep(1000 - m_uiSMS);
	SetTimer(TIMER_SNAKEGO, m_uiSMS, NULL);

	return;
}

//蛇头向前一步，转向（广告模式下自动），判断食物，蛇位置数组
VOID CSnakeDlg::SnakeGo(BOOL bAdv)
{
	m_bPressed = FALSE;
	WORD sgo_i;

	if (bAdv)
	{
		if (m_ucarrSnakeP[0][X] == 7 && m_ucarrSnakeP[0][Y] == 7)
			m_ucarrSnakeP[0][D] = RIGHT;
		if (m_ucarrSnakeP[0][X] == 7 && m_ucarrSnakeP[0][Y] == 15)
			m_ucarrSnakeP[0][D] = UP;
		if (m_ucarrSnakeP[0][X] == 15 && m_ucarrSnakeP[0][Y] == 7)
			m_ucarrSnakeP[0][D] = DOWN;
		if (m_ucarrSnakeP[0][X] == 15 && m_ucarrSnakeP[0][Y] == 15)
			m_ucarrSnakeP[0][D] = LEFT;
	}
	else
	{
		m_ucarrSnakeP[0][D] = m_ucInput;
	}

	if (m_bNextPressed == TRUE)
	{
		m_ucInput = m_ucNextInput;
		m_bNextPressed = FALSE;
	}

	m_ucarrOldBody[X] = m_ucarrSnakeP[m_wSnakeL - 1][X];
	m_ucarrOldBody[Y] = m_ucarrSnakeP[m_wSnakeL - 1][Y];
	m_ucarrOldBody[D] = m_ucarrSnakeP[m_wSnakeL - 1][D];
	for (sgo_i= m_wSnakeL - 1;sgo_i>0;sgo_i--)
	{
		m_ucarrSnakeP[sgo_i][X] = m_ucarrSnakeP[sgo_i - 1][X];
		m_ucarrSnakeP[sgo_i][Y] = m_ucarrSnakeP[sgo_i - 1][Y];
		m_ucarrSnakeP[sgo_i][D] = m_ucarrSnakeP[sgo_i - 1][D];
	}

	switch (m_ucarrSnakeP[0][D])
	{
	case UP:
		m_ucarrSnakeP[0][Y] = m_ucarrSnakeP[1][Y] - 1;
		break;
	case DOWN:
		m_ucarrSnakeP[0][Y] = m_ucarrSnakeP[1][Y] + 1;
		break;
	case LEFT:
		m_ucarrSnakeP[0][X] = m_ucarrSnakeP[1][X] - 1;
		break;
	case RIGHT:
		m_ucarrSnakeP[0][X] = m_ucarrSnakeP[1][X] + 1;
		break;
	}

	if (bAdv)
	{
		DrawMap(DM_GO, TRUE);
		return;
	}

	///判断蛇头的位置
	switch (CrashJudge(m_ucarrSnakeP[0][X], m_ucarrSnakeP[0][Y]))
	{
	case MAP_SNAKE:
		SnakeCrash();
		break;
	case MAP_EMPTY:
		DrawMap(DM_GO, TRUE);
		break;
	case MAP_FOODS:
		m_wSnakeL += 1;
		m_ucarrSnakeP[m_wSnakeL - 1][X] = m_ucarrOldBody[X];
		m_ucarrSnakeP[m_wSnakeL - 1][Y] = m_ucarrOldBody[Y];
		m_ucarrSnakeP[m_wSnakeL - 1][D] = m_ucarrOldBody[D];
		NewFood();
		DrawMap(DM_EAT, TRUE);
		break;
	}

	return;
}

//若byStu为DM_GO则画出新蛇头，清除旧蛇尾，若为DM_EAT则画出变色的整条蛇和食物，DAS改为真，若为DM_DAS或DAS为真则重画整条蛇，DAS改为假，若bF为真，则开启流畅模式，调用Display()输出
VOID CSnakeDlg::DrawMap(BYTE byStu, BOOL bF)
{
	STATIC BOOL DAS = FALSE;/*画出蛇的全部*/
	DWORD dm_timeofnext;
	WORD left = 0, right = 0, up = 0, down = 0;
	WORD x1 = 0, y1 = 0, x2= 0, y2 = 0;
	WORD dm_i;
	SHORT dm_a = 0;
	WORD freshtimes = (double)m_uiSMS * 60 / 1000;
	if (byStu != DM_DAS)
		dm_a = freshtimes - 1;

	if (byStu == DM_DAS || byStu == DM_EAT || DAS)
	{
		//画出旧头部
		switch (m_ucarrSnakeP[1][D])
		{
		case LEFT:
			left = 1;
			break;
		case RIGHT:
			right = 1;
			break;
		case UP:
			up = 1;
			break;
		case DOWN:
			down = 1;
			break;
		}
		if (byStu == DM_EAT)
			Brush.CreateSolidBrush(CO_HEADX);
		else
			Brush.CreateSolidBrush(CO_HEAD);
		m_CDCMap.SelectObject(Brush);
		m_CDCMap.Rectangle((m_ucarrSnakeP[1][X] - 1) * m_ucPixelSize + CON - left, (m_ucarrSnakeP[1][Y] - 1) * m_ucPixelSize + CON - up, (m_ucarrSnakeP[1][X]) * m_ucPixelSize + CON + right, (m_ucarrSnakeP[1][Y]) * m_ucPixelSize + CON + down);
		Brush.DeleteObject();
		left = 0; right = 0; up = 0; down = 0;
		//画出旧身体
		if (byStu == DM_EAT)
			Brush.CreateSolidBrush(CO_BODYX);
		else
			Brush.CreateSolidBrush(CO_BODY);
		m_CDCMap.SelectObject(Brush);
		for (dm_i = 2;dm_i < m_wSnakeL;dm_i++)
		{
			switch (m_ucarrSnakeP[dm_i][D])
			{
			case LEFT:
				left = 1;
				break;
			case RIGHT:
				right = 1;
				break;
			case UP:
				up = 1;
				break;
			case DOWN:
				down = 1;
				break;
			}
			m_CDCMap.Rectangle((m_ucarrSnakeP[dm_i][X] - 1) * m_ucPixelSize + CON - left, (m_ucarrSnakeP[dm_i][Y] - 1) * m_ucPixelSize + CON - up, (m_ucarrSnakeP[dm_i][X]) * m_ucPixelSize + CON + right, (m_ucarrSnakeP[dm_i][Y]) * m_ucPixelSize + CON + down);
			left = 0; right = 0; up = 0; down = 0;
		}
		//画出旧尾巴
		switch (m_ucarrOldBody[D])
		{
		case LEFT:
			left = 1;
			break;
		case RIGHT:
			right = 1;
			break;
		case UP:
			up = 1;
			break;
		case DOWN:
			down = 1;
			break;
		}
		m_CDCMap.Rectangle((m_ucarrOldBody[X] - 1) * m_ucPixelSize + CON - left, (m_ucarrOldBody[Y] - 1) * m_ucPixelSize + CON - up, (m_ucarrOldBody[X]) * m_ucPixelSize + CON + right, (m_ucarrOldBody[Y]) * m_ucPixelSize + CON + down);
		Brush.DeleteObject();
		left = 0; right = 0; up = 0; down = 0;
	}
	
	for (;dm_a >= 0;dm_a--)
	{
		dm_timeofnext = GetTickCount() + (1000 / 60);
		//画出头部
		x1 = (m_ucarrSnakeP[0][X] - 1) * m_ucPixelSize + CON;
		y1 = (m_ucarrSnakeP[0][Y] - 1) * m_ucPixelSize + CON;
		x2 = (m_ucarrSnakeP[0][X]) * m_ucPixelSize + CON;
		y2 = (m_ucarrSnakeP[0][Y]) * m_ucPixelSize + CON;

		switch (m_ucarrSnakeP[0][D])
		{
		case LEFT:
			x1 += (double)m_ucPixelSize / freshtimes * dm_a;
			break;
		case RIGHT:
			x2 -= (double)m_ucPixelSize / freshtimes * dm_a;
			break;
		case UP:
			y1 += (double)m_ucPixelSize / freshtimes * dm_a;
			break;
		case DOWN:
			y2 -= (double)m_ucPixelSize / freshtimes * dm_a;
			break;
		}
		switch (m_ucarrSnakeP[1][D])
		{
		case LEFT:
			x2 += 1;
			break;
		case RIGHT:
			x1 -= 1;
			break;
		case UP:
			y2 += 1;
			break;
		case DOWN:
			y1 -= 1;
			break;
		}
		if (byStu == DM_EAT)
			Brush.CreateSolidBrush(CO_HEADX);
		else
			Brush.CreateSolidBrush(CO_HEAD);
		m_CDCMap.SelectObject(Brush);
		m_CDCMap.Rectangle(x1, y1, x2, y2);
		Brush.DeleteObject();
		//画出第二节
		x1 = (m_ucarrSnakeP[1][X] - 1) * m_ucPixelSize + CON;
		y1 = (m_ucarrSnakeP[1][Y] - 1) * m_ucPixelSize + CON;
		x2 = (m_ucarrSnakeP[1][X]) * m_ucPixelSize + CON;
		y2 = (m_ucarrSnakeP[1][Y]) * m_ucPixelSize + CON;

		switch (m_ucarrSnakeP[1][D])
		{
		case LEFT:
			x1 += (double)m_ucPixelSize / freshtimes * dm_a - 1;
			break;
		case RIGHT:
			x2 -= (double)m_ucPixelSize / freshtimes * dm_a - 1;
			break;
		case UP:
			y1 += (double)m_ucPixelSize / freshtimes * dm_a - 1;
			break;
		case DOWN:
			y2 -= (double)m_ucPixelSize / freshtimes * dm_a - 1;
			break;
		}
		if (byStu == DM_EAT)
			Brush.CreateSolidBrush(CO_BODYX);
		else
			Brush.CreateSolidBrush(CO_BODY);
		m_CDCMap.SelectObject(Brush);
		m_CDCMap.Rectangle(x1, y1, x2, y2);
		Brush.DeleteObject();
		//擦去尾巴
		if (!(byStu == DM_EAT))
		{
			x1 = (m_ucarrOldBody[X] - 1) * m_ucPixelSize + CON;
			y1 = (m_ucarrOldBody[Y] - 1) * m_ucPixelSize + CON;
			x2 = (m_ucarrOldBody[X]) * m_ucPixelSize + CON;
			y2 = (m_ucarrOldBody[Y]) * m_ucPixelSize + CON;

			switch (m_ucarrOldBody[D])
			{
			case LEFT:
				x1 += (double)m_ucPixelSize / freshtimes * dm_a - 1;
				break;
			case RIGHT:
				x2 -= (double)m_ucPixelSize / freshtimes * dm_a - 1;
				break;
			case UP:
				y1 += (double)m_ucPixelSize / freshtimes * dm_a - 1;
				break;
			case DOWN:
				y2 -= (double)m_ucPixelSize / freshtimes * dm_a - 1;
				break;
			}
			Brush.CreateSolidBrush(CO_BG);
			m_CDCMap.SelectObject(Brush);
			m_CDCMap.Rectangle(x1, y1, x2, y2);
			Brush.DeleteObject();
		}

		//画出食物
		Brush.CreateSolidBrush(CO_FOOD);
		m_CDCMap.SelectObject(Brush);
		m_CDCMap.Rectangle((m_ucarrFoodP[X] - 1) * m_ucPixelSize + CON, (m_ucarrFoodP[Y] - 1) * m_ucPixelSize + CON, (m_ucarrFoodP[X]) * m_ucPixelSize + CON, (m_ucarrFoodP[Y]) * m_ucPixelSize + CON);
		Brush.DeleteObject();

		Display(DP_MAP);

		if (dm_a)
			while (1)
			{
				if (GetTickCount() >= dm_timeofnext)
					break;
				else
					Sleep(1);
			}
	}

	DAS = FALSE;
	if (byStu == DM_EAT)
		DAS = TRUE;

	return;
}

//用背景覆盖
VOID CSnakeDlg::DrawBG()
{
	Brush.CreateSolidBrush(CO_BG);
	m_CDCMap.SelectObject(Brush);
	m_CDCMap.Rectangle(0, 0, WINDOWSIZE, WINDOWSIZE);
	Brush.DeleteObject();
	Brush.CreateSolidBrush(CO_CON);
	m_CDCMap.SelectObject(Brush);
	m_CDCMap.Rectangle(0, 0, WINDOWSIZE, CON);
	m_CDCMap.Rectangle(0, 0, CON, WINDOWSIZE);
	m_CDCMap.Rectangle(0, WINDOWSIZE - CON, WINDOWSIZE, WINDOWSIZE);
	m_CDCMap.Rectangle(WINDOWSIZE - CON, 0, WINDOWSIZE, WINDOWSIZE);
	Brush.DeleteObject();
}

//获取CDC指针，创建储存于内存中的画面（位图）
VOID CSnakeDlg::CreateDisplay()
{
	m_pCDCDisplay = GetDC();

	m_CBitmapNum.LoadBitmap(IDB_NUM);
	m_CDCNum.CreateCompatibleDC(NULL);
	m_CDCNum.SelectObject(&m_CBitmapNum);
	m_CDCNum.SelectStockObject(NULL_PEN);

	m_CBitmapExp.LoadBitmap(IDB_EXP);
	m_CDCExp.CreateCompatibleDC(NULL);
	m_CDCExp.SelectObject(&m_CBitmapExp);
	m_CDCExp.SelectStockObject(NULL_PEN);

	m_CBitmapMap.CreateCompatibleBitmap(m_pCDCDisplay, WINDOWSIZE, WINDOWSIZE);
	m_CDCMap.CreateCompatibleDC(NULL);
	m_CDCMap.SelectObject(&m_CBitmapMap);
	m_CDCMap.SelectStockObject(NULL_PEN);
}

//输出当前画面（位图）
VOID CSnakeDlg::Display(UCHAR ucDP)
{
	switch (ucDP)
	{
	case DP_MAP:
		m_pCDCDisplay->BitBlt(0, 0, WINDOWSIZE, WINDOWSIZE, &m_CDCMap, 0, 0, SRCCOPY);
		break;
	case DP_EXP:
		m_pCDCDisplay->BitBlt((WINDOWSIZE - 300) / 2, (WINDOWSIZE - 500) / 2, 300, 500, &m_CDCExp, 0, 0, SRCCOPY);
		break;
	}
}

//刷新食物坐标
VOID CSnakeDlg::NewFood()
{
	do
	{
		m_ucarrFoodP[X] = 1 + rand() % (m_ucMapSize - 1 + 1);
		m_ucarrFoodP[Y] = 1 + rand() % (m_ucMapSize - 1 + 1);
	} while (CrashJudge(m_ucarrFoodP[X], m_ucarrFoodP[Y]) != MAP_EMPTY);
}

//更改窗口大小，并保持位置
VOID CSnakeDlg::ChangeWindowSize()
{
	CRect CWS_CRect;
	GetWindowRect(CWS_CRect);

	WORD CWS_i, CWS_a = CWS_CRect.Width();
	SHORT CWS_SizeNow;//改为WORD型会死机
	if (WINDOWSIZE > CWS_CRect.Width())
	{
		m_CDCMap.DeleteDC();
		m_CBitmapMap.DeleteObject();
		m_CBitmapMap.CreateCompatibleBitmap(m_pCDCDisplay, WINDOWSIZE, WINDOWSIZE);
		m_CDCMap.CreateCompatibleDC(NULL);
		m_CDCMap.SelectObject(&m_CBitmapMap);
		m_CDCMap.SelectStockObject(NULL_PEN);

		DrawBG();
		DrawMap(DM_DAS, FALSE);

		for (CWS_i = 1, CWS_SizeNow = CWS_CRect.Width() + CMMS;CWS_SizeNow < WINDOWSIZE;CWS_i += 1, CWS_SizeNow += CMMS)
		{
			GetWindowRect(CWS_CRect);
			::SetWindowPos(
				this->m_hWnd,
				NULL,
				CWS_CRect.left - CMMS / 2,
				CWS_CRect.top - CMMS / 2,
				CWS_SizeNow + WTCW,
				CWS_SizeNow + WTCW,
				SWP_NOZORDER | SWP_NOREDRAW);
			m_pCDCDisplay->BitBlt(
				0,
				0,
				CWS_SizeNow,
				CWS_SizeNow,
				&m_CDCMap,
				(WINDOWSIZE - CWS_a - CWS_i * CMMS) / 2,
				(WINDOWSIZE - CWS_a - CWS_i * CMMS) / 2,
				SRCCOPY);
			Sleep(1);
		}
		::SetWindowPos(
			this->m_hWnd,
			NULL,
			NULL,
			NULL,
			WINDOWSIZE + WTCW,
			WINDOWSIZE + WTCW,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
		m_pCDCDisplay->BitBlt(
			0,
			0,
			WINDOWSIZE,
			WINDOWSIZE,
			&m_CDCMap,
			0,
			0,
			SRCCOPY);
		CenterWindow();
	}
	else
	{
		for (CWS_i = 1, CWS_SizeNow = CWS_CRect.Width() - CMMS;CWS_SizeNow > WINDOWSIZE;CWS_i += 1, CWS_SizeNow -= CMMS)
		{
			GetWindowRect(CWS_CRect);
			::SetWindowPos(
				this->m_hWnd,
				NULL,
				CWS_CRect.left + CMMS / 2,
				CWS_CRect.top + CMMS / 2,
				CWS_SizeNow + WTCW,
				CWS_SizeNow + WTCW,
				SWP_NOZORDER | SWP_NOREDRAW);
			m_pCDCDisplay->BitBlt(
				0,
				0,
				CWS_SizeNow,
				CWS_SizeNow,
				&m_CDCMap,
				CWS_i * CMMS / 2,
				CWS_i * CMMS / 2,
				SRCCOPY);
			Sleep(1);
		}
		::SetWindowPos(
			this->m_hWnd,
			NULL,
			NULL,
			NULL,
			WINDOWSIZE + WTCW,
			WINDOWSIZE + WTCW,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
		m_pCDCDisplay->BitBlt(
			0,
			0,
			WINDOWSIZE,
			WINDOWSIZE,
			&m_CDCMap,
			0,
			0,
			SRCCOPY);
		CenterWindow();

		m_CDCMap.DeleteDC();
		m_CBitmapMap.DeleteObject();
		m_CBitmapMap.CreateCompatibleBitmap(m_pCDCDisplay, WINDOWSIZE, WINDOWSIZE);
		m_CDCMap.CreateCompatibleDC(NULL);
		m_CDCMap.SelectObject(&m_CBitmapMap);
		m_CDCMap.SelectStockObject(NULL_PEN);

		DrawBG();
		DrawMap(DM_DAS, FALSE);
	}
}

//碰撞判断（输入坐标）（通过取色），若是背景颜色则返回MAP_EMPTY，食物返回MAP_FOODS，其他返回MAP_SNAKE
UCHAR CSnakeDlg::CrashJudge(WORD wX, WORD wY)
{
	wX = m_ucPixelSize*wX + CON - 2;
	wY = m_ucPixelSize*wY + CON - 2;
	switch (m_CDCMap.GetPixel(wX, wY))
	{
	case CO_BG:
		return MAP_EMPTY;
	case CO_FOOD:
		return MAP_FOODS;
	default:
		return MAP_SNAKE;
	}
}

//关闭时删除内存中的画面（位图）,释放CDC指针
VOID CSnakeDlg::OnClose()
{
	KillTimer(TIMER_SNAKEGO);
	KillTimer(TIMER_SNAKEADV);

	if(m_bPaused)
		m_CDCMap.BitBlt((WINDOWSIZE - 300) / 2, (WINDOWSIZE - 500) / 2, 300, 500, &m_CDCExp, 0, 0, SRCCOPY);
	m_ucMapSize = 0;m_ucPixelSize = 0;
	ChangeWindowSize();

	m_CDCMap.DeleteDC();
	m_CDCExp.DeleteDC();
	m_CDCNum.DeleteDC();
	m_CBitmapMap.DeleteObject();
	m_CBitmapExp.DeleteObject();
	m_CBitmapNum.DeleteObject();
	ReleaseDC(m_pCDCDisplay);
	EndDialog(NULL);
}

//暂停
VOID CSnakeDlg::PauseGame()
{
	WORD PG_i, PG_XPos;
	m_bPaused = TRUE;
	KillTimer(TIMER_SNAKEGO);
	KillTimer(TIMER_SNAKEADV);

	//LENGTH
	for (PG_i = 10000, PG_XPos = 171;PG_i > 0;PG_i /= 10, PG_XPos += 24)
		m_CDCExp.BitBlt(PG_XPos, 52, 18, 30, &m_CDCNum, m_wSnakeL / PG_i % 10 * 18, 0, SRCCOPY);

	m_ucarrNum[0][0] = 0;m_ucarrNum[1][0] = (30 - (m_uiSMS - 17) / 10) / 10;m_ucarrNum[2][0] = (30 - (m_uiSMS - 17) / 10) % 10;
	m_ucarrNum[0][1] = m_ucMapSize / 100 % 10;m_ucarrNum[1][1] = m_ucMapSize / 10 % 10;m_ucarrNum[2][1] = m_ucMapSize % 10;
	m_ucarrNum[0][2] = 0;m_ucarrNum[1][2] = m_ucPixelSize / 10 % 10;m_ucarrNum[2][2] = m_ucPixelSize % 10;

	DrawNumber();
	Display(DP_EXP);
}

VOID CSnakeDlg::DrawNumber()
{
	WORD DN_i, DN_XPos;

	//SPEED(1-30）
	m_CDCExp.BitBlt(243, 98, 18, 30, &m_CDCNum, m_ucarrNum[1][0] * 18, 0, SRCCOPY);
	m_CDCExp.BitBlt(267, 98, 18, 30, &m_CDCNum, m_ucarrNum[2][0] * 18, 0, SRCCOPY);
	//MAPSIZE
	for (DN_i = 0, DN_XPos = 219;DN_i <= 2;DN_i += 1, DN_XPos += 24)
		m_CDCExp.BitBlt(DN_XPos, 144, 18, 30, &m_CDCNum, m_ucarrNum[DN_i][1] * 18, 0, SRCCOPY);
	//PIXELSIZE
	m_CDCExp.BitBlt(243, 190, 18, 30, &m_CDCNum, m_ucarrNum[1][2] * 18, 0, SRCCOPY);
	m_CDCExp.BitBlt(267, 190, 18, 30, &m_CDCNum, m_ucarrNum[2][2] * 18, 0, SRCCOPY);
}

VOID CSnakeDlg::ChangeFocus(WPARAM Input)
{
	switch (Input)
	{
	//改变焦点
	case VK_TAB:
		if (m_ucarrFocus[X] == 0 && m_ucarrFocus[Y] == 0)
			m_ucarrFocus[X] = 1;
		else
		{
			if (m_ucarrFocus[Y] == 2)
				m_ucarrFocus[Y] = 0;
			else
				m_ucarrFocus[Y] += 1;
		}
		m_ucarrFocus[X] = 0;
		break;
	case 'W':
	case VK_UP:
		if (m_ucarrFocus[Y] != 0)
			m_ucarrFocus[Y] -= 1;
		break;
	case 'S':
	case VK_DOWN:
		if (m_ucarrFocus[Y] != 2)
			m_ucarrFocus[Y] += 1;
		break;
	case 'A':
	case VK_LEFT:
		if (m_ucarrFocus[X] != 0)
			m_ucarrFocus[X] -= 1;
		break;
	case 'D':
	case VK_RIGHT:
		if (m_ucarrFocus[X] != 2)
			m_ucarrFocus[X] += 1;
		break;
	//改变数据
	default:
		if (Input >= 0x60 && Input <= 0x69)
			m_ucarrNum[m_ucarrFocus[X]][m_ucarrFocus[Y]] = Input - 0x60;
		else
			m_ucarrNum[m_ucarrFocus[X]][m_ucarrFocus[Y]] = Input - 48;
		if (m_ucarrFocus[X] == 2)
		{
			m_ucarrFocus[X] = 0;
			if (m_ucarrFocus[Y] == 2)
				m_ucarrFocus[Y] = 0;
			else
				m_ucarrFocus[Y] += 1;
		}
		else
			m_ucarrFocus[X] += 1;
		break;
	}

	if (m_ucarrFocus[X] == 0 && m_ucarrFocus[Y] == 0)
		m_ucarrFocus[X] = 1;
	if (m_ucarrFocus[X] == 0 && m_ucarrFocus[Y] == 2)
		m_ucarrFocus[X] = 1;
	DrawNumber();
	m_CDCExp.BitBlt(m_ucarrFocus[X] * 24 + 219, m_ucarrFocus[Y] * 46 + 98, 18, 30, &m_CDCNum, m_ucarrNum[m_ucarrFocus[X]][m_ucarrFocus[Y]] * 18, 30, SRCCOPY);
	Display(DP_EXP);
}

VOID CSnakeDlg::ContinueGame(BOOL bSave)
{
	//保存数据
	if (bSave)
	{
		//SPEED
		if ((m_ucarrNum[1][0] * 10 + m_ucarrNum[2][0]) >= 1 && (m_ucarrNum[1][0] * 10 + m_ucarrNum[2][0]) <= 30)
			m_uiSMS = (30 - (m_ucarrNum[1][0] * 10 + m_ucarrNum[2][0])) * 10 + 17;
		//MAPSIZE
		if (m_bStarted || m_bCrashed)
		{
			if ((m_ucarrNum[0][1] * 100 + m_ucarrNum[1][1] * 10 + m_ucarrNum[2][1]) >= 11 && m_ucMapSize != m_ucarrNum[0][1] * 100 + m_ucarrNum[1][1] * 10 + m_ucarrNum[2][1])
			{
				if (m_bStarted)
				{
					m_ucMapSize = m_ucarrNum[0][1] * 100 + m_ucarrNum[1][1] * 10 + m_ucarrNum[2][1];
					if ((m_ucarrNum[1][2] * 10 + m_ucarrNum[2][2]) >= 2 && (m_ucarrNum[1][2] * 10 + m_ucarrNum[2][2]) != m_ucPixelSize)
						m_ucPixelSize = m_ucarrNum[1][2] * 10 + m_ucarrNum[2][2];
					ChangeWindowSize();
					StartGame();
					goto ContinueTheGame;
				}
				else
				{
					m_ucNextMapSize = m_ucarrNum[0][1] * 100 + m_ucarrNum[1][1] * 10 + m_ucarrNum[2][1];
					if ((m_ucarrNum[1][2] * 10 + m_ucarrNum[2][2]) >= 2 && (m_ucarrNum[1][2] * 10 + m_ucarrNum[2][2]) != m_ucPixelSize)
						m_ucPixelSize = m_ucarrNum[1][2] * 10 + m_ucarrNum[2][2];
					if (!m_bCrashed)
						SetTimer(TIMER_SNAKEADV, m_uiSMS, NULL);
					StartGame();
					goto ContinueTheGame;
				}
			}
		}
		//PIXELSIZE
		if ((m_ucarrNum[1][2] * 10 + m_ucarrNum[2][2]) >= 2 && (m_ucarrNum[1][2] * 10 + m_ucarrNum[2][2]) != m_ucPixelSize)
		{
			m_ucPixelSize = m_ucarrNum[1][2] * 10 + m_ucarrNum[2][2];
			ChangeWindowSize();
		}
	}

	Display(DP_MAP);
	if (m_bStarted)
	{
		if ((1000 - m_uiSMS) > 0)
			Sleep(1000 - m_uiSMS);
		SetTimer(TIMER_SNAKEGO, m_uiSMS, NULL);
	}
	else
		if (!m_bCrashed)
			SetTimer(TIMER_SNAKEADV, m_uiSMS, NULL);

ContinueTheGame:

	m_ucarrFocus[X] = 0;
	m_ucarrFocus[Y] = 0;
	m_bPaused = FALSE;
}

//游戏结束
VOID CSnakeDlg::SnakeCrash()
{
	KillTimer(TIMER_SNAKEGO);
	Display(DP_MAP);
	m_bStarted = FALSE;
	m_bCrashed = TRUE;
}

//移动窗口
LRESULT CSnakeDlg::OnNcHitTest(CPoint Cpoint)
{
	UINT a = CDialog::OnNcHitTest(Cpoint);
	return ((a == HTCLIENT) ? HTCAPTION : a);
}

VOID CSnakeDlg::CreateSnakeWindow()
{
	DrawBG();
	DrawMap(DM_DAS, FALSE);
	::SetWindowPos(this->m_hWnd, NULL, NULL, NULL, 0, 0, SWP_NOZORDER | SWP_NOREDRAW | SWP_NOMOVE);
	CenterWindow();
	ChangeWindowSize();
	SetTimer(TIMER_SNAKEADV, m_uiSMS, NULL);
}