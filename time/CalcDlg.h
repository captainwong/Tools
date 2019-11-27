#pragma once


// CalcDlg dialog

class CalcDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CalcDlg)

public:
	CalcDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CalcDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CALC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CDateTimeCtrl m_date;
	CDateTimeCtrl m_time;
	CComboBox m_op;
	CEdit m_number;
	CComboBox m_unit;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnDtnDatetimechangeDatetimepickerDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDtnDatetimechangeDatetimepickerTime(NMHDR* pNMHDR, LRESULT* pResult);

	void calc();
public:
	CDateTimeCtrl m_date2;
	CDateTimeCtrl m_time2;
	afx_msg void OnCbnSelchangeComboOp();
	afx_msg void OnEnChangeEditNumber();
	afx_msg void OnCbnSelchangeComboUnit();
	virtual BOOL OnInitDialog();
	CEdit m_asms;
};
