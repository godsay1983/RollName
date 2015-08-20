// RollNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RollName.h"
#include "RollNameDlg.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <ctime>
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRollNameDlg dialog




CRollNameDlg::CRollNameDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CRollNameDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bSwitchOn = false;
}

void CRollNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTAINER, m_wndContainer);
	DDX_Control(pDX, IDC_BUTTON_ROLL, m_rollBtn);
}

BEGIN_MESSAGE_MAP(CRollNameDlg, CBCGPDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ROLL, &CRollNameDlg::OnBnClickedButtonRoll)
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRollNameDlg message handlers

BOOL CRollNameDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	CBCGPVisualContainer* pContainer = m_wndContainer.GetVisualContainer();
	ASSERT_VALID(pContainer);

	pContainer->SetFillBrush(
		CBCGPBrush(CBCGPColor(CBCGPColor::Silver, .1), CBCGPColor::White, CBCGPBrush::BCGP_GRADIENT_RADIAL_BOTTOM_RIGHT));
	pContainer->SetOutlineBrush(CBCGPBrush(CBCGPColor::Gray));

	m_pTextIndicator = new CBCGPTextGaugeImpl("", CBCGPColor::Gray, pContainer);

	CBCGPTextFormat m_TextFormat = m_pTextIndicator->GetTextFormat();
	m_TextFormat.SetFontSize(160);
	m_pTextIndicator->SetTextColor(CBCGPColor::Red);
	m_pTextIndicator->SetTextFormat(m_TextFormat);

	ReadNames();
	Repos();
	UpdateData(FALSE);
	
	bStart = false;
	bSwitchOn = true;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRollNameDlg::Repos()
{
	CBCGPVisualContainer* pContainer = m_wndContainer.GetVisualContainer();
	ASSERT_VALID(pContainer);

	if (pContainer == NULL || pContainer->GetCount() == 0)
	{
		return;
	}

	CBCGPRect rect = pContainer->GetRect();

	CBCGPSize indicatorSize(100, 30);

	CBCGPRect rectIndicator = rect;
	double marginHorz = (rectIndicator.Width() - indicatorSize.cx) / 2;
	double marginVert = (rectIndicator.Height() - indicatorSize.cy) / 2;
	rectIndicator.SetSize(indicatorSize);
	rectIndicator.OffsetRect(marginHorz, marginVert);

	m_pTextIndicator->SetRect(rectIndicator);
	m_wndContainer.RedrawWindow();
}

void CRollNameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CBCGPDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRollNameDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBCGPDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRollNameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRollNameDlg::OnBnClickedButtonRoll()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	bStart = !bStart;
	if (bStart)
	{
		SetTimer(ROLL_NAME, 10, NULL);
		m_rollBtn.SetWindowText("停  止");
		CRect rect;
		GetWindowRect(rect);
		CBCGPTextFormat m_TextFormat = m_pTextIndicator->GetTextFormat();
		m_TextFormat.SetFontSize((float)(min(rect.Height()*0.5, rect.Width()*0.2)));
		m_pTextIndicator->SetTextFormat(m_TextFormat);
		m_wndContainer.RedrawWindow();
	}
	else
	{
		KillTimer(ROLL_NAME);
		CString name = RollName().c_str();
		m_pTextIndicator->SetText(name);
		CRect rect;
		GetWindowRect(rect);
		CBCGPTextFormat m_TextFormat = m_pTextIndicator->GetTextFormat();
		m_TextFormat.SetFontSize((float)(min(rect.Height()*0.6, rect.Width()*0.3)));
		m_pTextIndicator->SetTextFormat(m_TextFormat);
		m_wndContainer.RedrawWindow();
		m_rollBtn.SetWindowText("开  始");
	}
}

string CRollNameDlg::RollName()
{
	DWORD time = ::GetCurrentTime();
	int size = names.size();
	int ran = Roll(size, time);
	string name = names[ran];
	if (size > 5)
	{
		names.erase(names.begin() + ran);
		AddRolledName(name.c_str());
	}
	return name;
}

int CRollNameDlg::Roll(int nMax, double num)
{
	boost::random::mt19937 gen(num);
	boost::random::uniform_int_distribution<> dist(0, nMax-1);
	return dist(gen);
}

void CRollNameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ROLL_NAME)
	{
		DWORD time = ::GetCurrentTime();
		int size = names.size();
		int ran = Roll(size, time);
		m_pTextIndicator->SetText(names[ran].c_str());
		m_wndContainer.RedrawWindow();
	}
	CBCGPDialog::OnTimer(nIDEvent);
}

void CRollNameDlg::ReadNames()
{
	CStdioFile file;
	file.Open("names.txt", CFile::modeRead);
	CString name;
	while (file.ReadString(name))
	{
		names.push_back(name.GetString());
	}
	file.Close();
}

void CRollNameDlg::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);
	
	if (bSwitchOn)
	{
		m_wndContainer.SetWindowPos(&CWnd::wndTop, 0, 0, cx, cy-50, SWP_NOACTIVATE);
		Repos();
		CRect btnRect;
		m_rollBtn.GetClientRect(btnRect);
		m_rollBtn.MoveWindow(cx/2 - btnRect.Width()/2, cy-40, btnRect.Width(), btnRect.Height());

		CBCGPTextFormat m_TextFormat = m_pTextIndicator->GetTextFormat();
		CRect rect;
		GetWindowRect(rect);
		m_TextFormat.SetFontSize((float)(min(rect.Height()*0.6, rect.Width()*0.3)));
		m_pTextIndicator->SetTextFormat(m_TextFormat);
		m_wndContainer.RedrawWindow();
	}
	// TODO: 在此处添加消息处理程序代码
}

void CRollNameDlg::AddRolledName(CString name)
{
	rolledNames.push_back(name.GetString());
	if (rolledNames.size() > 5)
	{
		names.push_back(rolledNames.front());
		rolledNames.erase(rolledNames.begin());
	}
}