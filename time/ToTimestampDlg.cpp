// ToTimestampDlg.cpp : implementation file
//

#include "pch.h"
#include "time.h"
#include "ToTimestampDlg.h"
#include "afxdialogex.h"

// ToTimestampDlg dialog

IMPLEMENT_DYNAMIC(ToTimestampDlg, CDialogEx)

ToTimestampDlg::ToTimestampDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TO_TIMESTAMP, pParent)
{

}

ToTimestampDlg::~ToTimestampDlg()
{
}

void ToTimestampDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_time);
	DDX_Control(pDX, IDC_EDIT1, m_time_t);
	DDX_Control(pDX, IDC_EDIT2, m_time_ms);
	DDX_Control(pDX, IDC_EDIT3, m_time_us);
}


BEGIN_MESSAGE_MAP(ToTimestampDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ToTimestampDlg::OnBnClickedButton1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &ToTimestampDlg::OnDtnDatetimechangeDatetimepickerDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &ToTimestampDlg::OnDtnDatetimechangeDatetimepickerTime)
END_MESSAGE_MAP()


// ToTimestampDlg message handlers

void ToTimestampDlg::updateTime()
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


void ToTimestampDlg::OnBnClickedButton1()
{
	auto us = jlib::gettimeofdayUsec();
	COleDateTime date = COleDateTime::GetTickCount();
	m_date.SetTime(date);
	m_time.SetTime(date);
	updateTime();
	m_time_ms.SetWindowTextW(std::to_wstring(us / 1000).data());
	m_time_us.SetWindowTextW(std::to_wstring(us).data());
}


void ToTimestampDlg::OnDtnDatetimechangeDatetimepickerDate(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here

	updateTime();

	*pResult = 0;
}


void ToTimestampDlg::OnDtnDatetimechangeDatetimepickerTime(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	updateTime();
	*pResult = 0;
}
