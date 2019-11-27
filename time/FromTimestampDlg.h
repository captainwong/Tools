#pragma once


// FromTimestampDlg dialog

class FromTimestampDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FromTimestampDlg)

public:
	FromTimestampDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FromTimestampDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FROM_TIMESTAMP };
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
	afx_msg void OnEnChangeEditS();
	afx_msg void OnEnChangeEditMS();
	afx_msg void OnEnChangeEditUS();
};
