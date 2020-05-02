// FromTimestampDlg.cpp : implementation file
//

#include "pch.h"
#include "time.h"
#include "FromTimestampDlg.h"
#include "afxdialogex.h"

// FromTimestampDlg dialog

IMPLEMENT_DYNAMIC(FromTimestampDlg, CDialogEx)

FromTimestampDlg::FromTimestampDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FROM_TIMESTAMP, pParent)
{

}

FromTimestampDlg::~FromTimestampDlg()
{
}

void FromTimestampDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_time);
	DDX_Control(pDX, IDC_EDIT1, m_time_t);
	DDX_Control(pDX, IDC_EDIT2, m_time_ms);
	DDX_Control(pDX, IDC_EDIT3, m_time_us);
}


BEGIN_MESSAGE_MAP(FromTimestampDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &FromTimestampDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &FromTimestampDlg::OnEnChangeEditS)
	ON_EN_CHANGE(IDC_EDIT2, &FromTimestampDlg::OnEnChangeEditMS)
	ON_EN_CHANGE(IDC_EDIT3, &FromTimestampDlg::OnEnChangeEditUS)
END_MESSAGE_MAP()


// FromTimestampDlg message handlers

void FromTimestampDlg::updateTime()
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

	{
		auto r = jlib::time_tToSystemTimeLocal(s);
		assert(memcmp(&st, &r, sizeof(r)) == 0);
	}

	time_t ms = s * 1000;
	time_t us = ms * 1000;

	m_time_t.SetWindowTextW(std::to_wstring(s).data());
	m_time_ms.SetWindowTextW(std::to_wstring(ms).data());
	m_time_us.SetWindowTextW(std::to_wstring(us).data());
}

void FromTimestampDlg::OnBnClickedButton1()
{
	auto us = jlib::gettimeofdayUsec();
	COleDateTime date = COleDateTime::GetTickCount();
	m_date.SetTime(date);
	m_time.SetTime(date);
	updateTime();
	m_time_ms.SetWindowTextW(std::to_wstring(us / 1000).data());
	m_time_us.SetWindowTextW(std::to_wstring(us).data());
}


void FromTimestampDlg::OnEnChangeEditS()
{
	CString s;
	m_time_t.GetWindowTextW(s);
	time_t t = _ttoi64(s);
	COleDateTime dt(t);
	m_date.SetTime(dt);
	m_time.SetTime(dt);
}


void FromTimestampDlg::OnEnChangeEditMS()
{
	CString s;
	m_time_ms.GetWindowTextW(s);
	time_t t = _ttoi64(s) / 1000;
	COleDateTime dt(t);
	m_date.SetTime(dt);
	m_time.SetTime(dt);
}


void FromTimestampDlg::OnEnChangeEditUS()
{
	CString s;
	m_time_us.GetWindowTextW(s);
	time_t t = _ttoi64(s) / 1000000;
	COleDateTime dt(t);
	m_date.SetTime(dt);
	m_time.SetTime(dt);
}
