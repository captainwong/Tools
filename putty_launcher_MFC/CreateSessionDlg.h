#pragma once


// CCreateSessionDlg dialog

class CCreateSessionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateSessionDlg)

public:
	CCreateSessionDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCreateSessionDlg();

	void setEditMode(SessionPtr session) { this->session = session; }
	SessionPtr getSession() const { return session; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CREATE_SESSION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();

private:
	SessionPtr session = {};
	CEdit m_name;
	CEdit m_host;
	CEdit m_port;
	CButton m_radio_ssh;
	CEdit m_user;
	CEdit m_passwd;
};
