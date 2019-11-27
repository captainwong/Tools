// CurrentTimeDlg.cpp : implementation file
//

#include "pch.h"
#include "time.h"
#include "CurrentTimeDlg.h"
#include "afxdialogex.h"

// CurrentTimeDlg dialog

IMPLEMENT_DYNAMIC(CurrentTimeDlg, CDialogEx)

CurrentTimeDlg::CurrentTimeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CURRENT_TIME, pParent)
{

}

CurrentTimeDlg::~CurrentTimeDlg()
{
}

void CurrentTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT7, m_cur_local);
	DDX_Control(pDX, IDC_EDIT8, m_cur_sys);
	DDX_Control(pDX, IDC_EDIT4, m_cur_s);
	DDX_Control(pDX, IDC_EDIT5, m_cur_ms);
	DDX_Control(pDX, IDC_EDIT6, m_cur_us);
	DDX_Control(pDX, IDC_CHECK1, m_cur_tick);
}


BEGIN_MESSAGE_MAP(CurrentTimeDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CurrentTimeDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CurrentTimeDlg message handlers


BOOL CurrentTimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_cur_tick.SetCheck(1);
	OnBnClickedCheck1();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CurrentTimeDlg::OnBnClickedCheck1()
{
	ticking_ = m_cur_tick.GetCheck() ? true : false;
	if (ticking_) {
		SetTimer(1, 1, nullptr);
	} else {
		KillTimer(1);
	}
}


void CurrentTimeDlg::OnTimer(UINT_PTR nIDEvent)
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


