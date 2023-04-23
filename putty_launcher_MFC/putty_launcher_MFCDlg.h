
// putty_launcher_MFCDlg.h : header file
//

#pragma once

#include <json/jsoncpp-stdafx/json.h>


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


protected:
	void initTree();
	CString getDataPath() const;
	void parseFolderAndCreate(Json::Value& item, HTREEITEM parentItem);
	void parseSessionAndCreate(Json::Value& item, HTREEITEM parentItem);
	void parseTreeAndSave(HTREEITEM hItem, Json::Value& value);
	void loadFromJson();
	void saveToJson();
	void findPutty();
	HTREEITEM createItem(LPCTSTR text, HTREEITEM parentItem = nullptr, bool is_folder = false, bool expand = true, bool refresh = true, SessionPtr session = {});
	ItemDataPtr getItemData(HTREEITEM hItem) const;
	void updateConnectionString(HTREEITEM hItem);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	afx_msg void OnTvnBeginlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonConnectSession();

	void reboot_session();
	void shutdown_session();

private:
	CEdit m_putty_path;
	CString m_plink_path;
	CTreeCtrl m_tree;
	CButton m_chk_show_pwd;
	CEdit m_session_connection_string;
	CButton m_btn_connect_session;

	HIMAGELIST m_image_list;
	HTREEITEM m_root_item = nullptr;
	HTREEITEM m_cur_item = nullptr;
	bool m_is_editting_label = false;

	std::unordered_map<HTREEITEM, ItemDataPtr> m_tree_data = {};

};
