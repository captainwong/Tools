
// putty_launcher_MFCDlg.h : header file
//

#pragma once
#include <unordered_map>
#include <string>
#include <memory>

// CputtylauncherMFCDlg dialog
class CputtylauncherMFCDlg : public CDialogEx
{
	// Construction
public:
	CputtylauncherMFCDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PUTTY_LAUNCHER_MFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	afx_msg void OnTvnBeginlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult);

private:
	CEdit m_putty_path;
	CTreeCtrl m_tree;
	HIMAGELIST m_image_list;
	HTREEITEM m_root_item = nullptr;

	struct Session {
		std::wstring host = {};
		std::wstring port = {};
		std::wstring type = {};
		std::wstring username = {};
		std::wstring password = {};

		// like: " -ssh root@somewhere.com -pw mypasswordforsomewherecom -P 22"
		std::wstring connection_string() const {
			return std::wstring(L" -") + type + L" " 
				+ username + L"@" + host 
				+ L" -pw " + password 
				+ L" -P " + port;
		}
	};
	typedef std::shared_ptr<Session> SessionPtr;

	struct ItemData {
		bool is_folder = false;
		std::wstring name = {};
		SessionPtr session = {};

		ItemData(bool is_folder = false, const std::wstring& name = L"", SessionPtr session = {})
			: is_folder(is_folder)
			, name(name)
			, session(session)
		{}

	};
	typedef std::shared_ptr<ItemData> ItemDataPtr;

	std::unordered_map<HTREEITEM, ItemDataPtr> m_tree_data = {};

};
