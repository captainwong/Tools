#pragma once


// CurrentTimeDlg dialog

class CurrentTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CurrentTimeDlg)

public:
	CurrentTimeDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CurrentTimeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CURRENT_TIME };
#endif

protected:
	bool ticking_ = false;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CEdit m_cur_local;
	CEdit m_cur_sys;
	CEdit m_cur_s;
	CEdit m_cur_ms;
	CEdit m_cur_us;
	CButton m_cur_tick;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	virtual BOOL OnInitDialog();
};
