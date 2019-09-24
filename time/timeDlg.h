
// timeDlg.h : header file
//

#pragma once


// CtimeDlg dialog
class CtimeDlg : public CDialogEx
{
// Construction
public:
	CtimeDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	bool ticking_ = false;

	void updateTime();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_date;
	CDateTimeCtrl m_time;
	CEdit m_time_t;
	CEdit m_time_ms;
	CEdit m_time_us;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnDtnDatetimechangeDatetimepickerDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDtnDatetimechangeDatetimepickerTime(NMHDR* pNMHDR, LRESULT* pResult);
	CEdit m_cur_local;
	CEdit m_cur_sys;
	CEdit m_cur_s;
	CEdit m_cur_ms;
	CEdit m_cur_us;
	CButton m_cur_tick;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
