
// timeDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "time.h"
#include "timeDlg.h"
#include "afxdialogex.h"
#include "CurrentTimeDlg.h"
#include "ToTimestampDlg.h"
#include "FromTimestampDlg.h"
#include "CalcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
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


// CtimeDlg dialog



CtimeDlg::CtimeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TIME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CtimeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CtimeDlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CtimeDlg message handlers

BOOL CtimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 25;
	rc.bottom -= 5;
	rc.left += 5;
	rc.right -= 5;

	m_tab.InsertItem(0, L"Current");
	{
		auto dlg = new CurrentTimeDlg();
		dlg->Create(IDD_DIALOG_CURRENT_TIME, &m_tab);
		dlg->MoveWindow(rc);
		dlgs.push_back(dlg);
	}

	m_tab.InsertItem(1, L"ToTimestamp");
	{
		auto dlg = new ToTimestampDlg();
		dlg->Create(IDD_DIALOG_TO_TIMESTAMP, &m_tab);
		dlg->MoveWindow(rc);
		dlgs.push_back(dlg);
	}

	m_tab.InsertItem(2, L"FromTimestamp");
	{
		auto dlg = new FromTimestampDlg();
		dlg->Create(IDD_DIALOG_FROM_TIMESTAMP, &m_tab);
		dlg->MoveWindow(rc);
		dlgs.push_back(dlg);
	}

	m_tab.InsertItem(3, L"Calc");
	{
		auto dlg = new CalcDlg();
		dlg->Create(IDD_DIALOG_CALC, &m_tab);
		dlg->MoveWindow(rc);
		dlgs.push_back(dlg);
	}


	dlgs[cursel]->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CtimeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CtimeDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CtimeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtimeDlg::OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	dlgs[cursel]->ShowWindow(SW_HIDE);
	cursel = m_tab.GetCurSel();
	dlgs[cursel]->ShowWindow(SW_SHOW);

	*pResult = 0;
}

