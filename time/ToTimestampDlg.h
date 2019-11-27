#pragma once


// ToTimestampDlg dialog

class ToTimestampDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ToTimestampDlg)

public:
	ToTimestampDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ToTimestampDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TO_TIMESTAMP };
#endif

protected:
	void updateTime();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CDateTimeCtrl m_date;
	CDateTimeCtrl m_time;
	CEdit m_time_t;
	CEdit m_time_ms;
	CEdit m_time_us;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnDtnDatetimechangeDatetimepickerDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDtnDatetimechangeDatetimepickerTime(NMHDR* pNMHDR, LRESULT* pResult);
};
