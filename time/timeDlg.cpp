
// timeDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "time.h"
#include "timeDlg.h"
#include "afxdialogex.h"
#include <jlib/base/time.h>
#include <string>

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
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_time);
	DDX_Control(pDX, IDC_EDIT1, m_time_t);
	DDX_Control(pDX, IDC_EDIT2, m_time_ms);
	DDX_Control(pDX, IDC_EDIT3, m_time_us);
	DDX_Control(pDX, IDC_EDIT7, m_cur_local);
	DDX_Control(pDX, IDC_EDIT8, m_cur_sys);
	DDX_Control(pDX, IDC_EDIT4, m_cur_s);
	DDX_Control(pDX, IDC_EDIT5, m_cur_ms);
	DDX_Control(pDX, IDC_EDIT6, m_cur_us);
	DDX_Control(pDX, IDC_CHECK1, m_cur_tick);
}

BEGIN_MESSAGE_MAP(CtimeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtimeDlg::OnBnClickedButton1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CtimeDlg::OnDtnDatetimechangeDatetimepickerDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CtimeDlg::OnDtnDatetimechangeDatetimepickerTime)
	ON_BN_CLICKED(IDC_CHECK1, &CtimeDlg::OnBnClickedCheck1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CtimeDlg message handlers

void CtimeDlg::updateTime()
{
	/*SYSTEMTIME date, time;
	m_date.GetTime(&date);
	m_time.GetTime(&time);
	date.wHour = time.wHour;
	date.wMinute = time.wMinute;
	date.wSecond = time.wSecond;
	date.wMilliseconds = time.wMilliseconds;*/

	COleDateTime date, time;
	m_date.GetTime(date); m_time.GetTime(time);
	//date.SetTime(time.GetHour(), time.GetMinute(), time.GetSecond());
	time.SetDateTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	
	SYSTEMTIME st;
	time.GetAsSystemTime(st);

	time_t s = jlib::systemTimeToTime_t(st);
	time_t ms = s * 1000;
	time_t us = ms * 1000;

	m_time_t.SetWindowTextW(std::to_wstring(s).data());
	m_time_ms.SetWindowTextW(std::to_wstring(ms).data());
	m_time_us.SetWindowTextW(std::to_wstring(us).data());
}

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
	m_cur_tick.SetCheck(1);
	OnBnClickedCheck1();

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



void CtimeDlg::OnBnClickedButton1()
{
	auto us = jlib::gettimeofdayUsec();
	COleDateTime date = COleDateTime::GetTickCount();
	m_date.SetTime(date);
	m_time.SetTime(date);
	updateTime();
	m_time_ms.SetWindowTextW(std::to_wstring(us / 1000).data());
	m_time_us.SetWindowTextW(std::to_wstring(us).data());
}


void CtimeDlg::OnDtnDatetimechangeDatetimepickerDate(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	updateTime();

	*pResult = 0;
}


void CtimeDlg::OnDtnDatetimechangeDatetimepickerTime(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	updateTime();
	*pResult = 0;
}


void CtimeDlg::OnBnClickedCheck1()
{
	ticking_ = m_cur_tick.GetCheck() ? true : false;
	if (ticking_) {
		SetTimer(1, 1, nullptr);
	} else {
		KillTimer(1);
	}
}


void CtimeDlg::OnTimer(UINT_PTR nIDEvent)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString s;
	s.Format(L"%4d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	m_cur_local.SetWindowTextW(s);

	GetSystemTime(&time);
	s.Format(L"%4d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	m_cur_sys.SetWindowTextW(s);

	auto us = jlib::gettimeofdayUsec();
	m_cur_s.SetWindowTextW(std::to_wstring(us / jlib::MICRO_SECONDS_PER_SECOND).data());
	m_cur_ms.SetWindowTextW(std::to_wstring(us / 1000).data());
	m_cur_us.SetWindowTextW(std::to_wstring(us).data());

	CDialogEx::OnTimer(nIDEvent);
}
