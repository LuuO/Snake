
// SnakeDlg.h : ͷ�ļ�
//

#pragma once


#include "stdafx.h"
#include "Snake.h"
#include "SnakeDlg.h"
#include "afxdialogex.h"
#include "SnakeDefine.h"

// CSnakeDlg �Ի���
class CSnakeDlg : public CDialogEx
{
// ����������
public:
	CSnakeDlg(CWnd* pParent = NULL);
	~CSnakeDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNAKE_DIALOG };
#endif

protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
	HICON m_hIcon;

// ��Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg BOOL PreTranslateMessage(MSG * pMsg);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg VOID OnTimer(UINT_PTR nIDEvent);
	afx_msg VOID OnPaint();
	afx_msg VOID OnClose();
	DECLARE_MESSAGE_MAP()

//Snake��
	CDC m_CDCNum;
	CDC m_CDCExp;
	CDC m_CDCMap;
	CDC *m_pCDCDisplay;
	CBitmap m_CBitmapNum;
	CBitmap m_CBitmapExp;
	CBitmap m_CBitmapMap;
	CBrush Brush;
//Snake����
	WORD m_wSnakeL;
	UCHAR m_ucarrSnakeP[9999][3];
	UCHAR m_ucarrFoodP[2];
	UCHAR m_ucarrOldBody[3];
	UCHAR m_ucarrNum[3][3];
	UCHAR m_ucarrFocus[2] = { 0, 0 };
	UCHAR m_ucMapSize = 21;//���Ϊ11
	UCHAR m_ucNextMapSize = 0;
	WORD m_uiSMS = 77;//��С17
	UCHAR m_ucPixelSize = 25;//�����غ�����Ϊ2
	UCHAR m_ucInput;
	UCHAR m_ucNextInput;
	BOOL m_bNextPressed = FALSE;
	BOOL m_bPressed = FALSE;
	BOOL m_bStarted = FALSE;
	BOOL m_bPaused = FALSE;
	BOOL m_bCrashed = FALSE;
//Snake����
	UCHAR CrashJudge(WORD wX, WORD wY);
	VOID PauseGame();
	VOID DrawNumber();
	VOID ChangeFocus(WPARAM Input);
	VOID ContinueGame(BOOL bSave);
	VOID StartGame();
	VOID SnakeGo(BOOL bAdv);
	VOID DrawMap(BYTE byStu, BOOL bF);
	VOID DrawBG();
	VOID CreateDisplay();
	VOID NewFood();
	VOID ChangeWindowSize();
	VOID Display(UCHAR ucDP);
	VOID SnakeCrash();
	VOID CreateSnakeWindow();
};
