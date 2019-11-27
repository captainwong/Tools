// CalcDlg.cpp : implementation file
//

#include "pch.h"
#include "time.h"
#include "CalcDlg.h"
#include "afxdialogex.h"


enum Op {
	Add,
	Minus,
	OpCount,
};

static constexpr wchar_t* OpString[] = {
	L"Add",
	L"Minus"
};

enum Unit {
	Year,
	Month,
	Week,
	Day,
	Hour,
	Minute,
	Second,
	Ms,
	Us,
	UnitCount,
};

static constexpr wchar_t* UnitString[] = {
	L"Year",
	L"Month",
	L"Week",
	L"Day",
	L"Hour",
	L"Minute",
	L"Second",
	L"Millisecond",
	L"Microsecond"
};

// CalcDlg dialog

IMPLEMENT_DYNAMIC(CalcDlg, CDialogEx)

CalcDlg::CalcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CALC, pParent)
{

}

CalcDlg::~CalcDlg()
{
}

void CalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_op, m_op);
	DDX_Control(pDX, IDC_EDIT_NUMBER, m_number);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_unit);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_time);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_date2);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_time2);
	DDX_Control(pDX, IDC_EDIT_NUMBER2, m_asms);
}


BEGIN_MESSAGE_MAP(CalcDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CalcDlg::OnBnClickedButton1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CalcDlg::OnDtnDatetimechangeDatetimepickerDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CalcDlg::OnDtnDatetimechangeDatetimepickerTime)
	ON_CBN_SELCHANGE(IDC_COMBO_op, &CalcDlg::OnCbnSelchangeComboOp)
	ON_EN_CHANGE(IDC_EDIT_NUMBER, &CalcDlg::OnEnChangeEditNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_UNIT, &CalcDlg::OnCbnSelchangeComboUnit)
END_MESSAGE_MAP()


// CalcDlg message handlers

BOOL CalcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = Op::Add; i < Op::OpCount; i++) {
		m_op.InsertString(i, OpString[i]);
	}

	for (int i = Unit::Year; i < Unit::UnitCount; i++) {
		m_unit.InsertString(i, UnitString[i]);
	}

	m_op.SetCurSel(Op::Add);
	m_unit.SetCurSel(Unit::Year);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CalcDlg::OnBnClickedButton1()
{
	auto us = jlib::gettimeofdayUsec();
	COleDateTime date = COleDateTime::GetTickCount();
	m_date.SetTime(date);
	m_time.SetTime(date);	

	calc();
}


void CalcDlg::OnDtnDatetimechangeDatetimepickerDate(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	calc();

	*pResult = 0;
}


void CalcDlg::OnDtnDatetimechangeDatetimepickerTime(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	calc();

	*pResult = 0;
}


void CalcDlg::OnCbnSelchangeComboOp()
{
	calc();
}


void CalcDlg::OnEnChangeEditNumber()
{
	calc();
}


void CalcDlg::OnCbnSelchangeComboUnit()
{
	calc();
}

void CalcDlg::calc()
{
	COleDateTime date, time;
	m_date.GetTime(date); m_time.GetTime(time);
	//date.SetTime(time.GetHour(), time.GetMinute(), time.GetSecond());
	time.SetDateTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	SYSTEMTIME st;
	time.GetAsSystemTime(st);

	time_t source = jlib::systemTimeToTime_t(st);

	{
		auto r = jlib::time_tToSystemTimeLocal(source);
		assert(memcmp(&st, &r, sizeof(r)) == 0);
	}

	Op op = (Op)m_op.GetCurSel();
	Unit unit = (Unit)m_unit.GetCurSel();
	CString number; m_number.GetWindowTextW(number);
	time_t n = _ttoi64(number);

	time_t second = 0;
	switch (unit) {
	case Year:
		second = n * 365 * 24 * 60 * 60;
		break;
	case Month:
		second = n * 30 * 24 * 60 * 60;
		break;
	case Week:
		second = n * 7 * 24 * 60 * 60;
		break;
	case Day:
		second = n * 1 * 24 * 60 * 60;
		break;
	case Hour:
		second = n * 60 * 60;
		break;
	case Minute:
		second = n * 60;
		break;
	case Second:
		second = n;
		break;
	case Ms:
		second = n / 1000;
		break;
	case Us:
		second = n / 1000000;
		break;
	default:
		return;
		break;
	}

	m_asms.SetWindowTextW(std::to_wstring(second).data());

	switch (op) {
	case Add:
		break;
	case Minus:
		second = -second;
		break;
	default:return;
	}

	auto result = source + second;
	COleDateTime dt(result);
	m_date2.SetTime(dt);
	m_time2.SetTime(dt);

}


