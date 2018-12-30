
// putty_launcher_MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "putty_launcher_MFC.h"
#include "putty_launcher_MFCDlg.h"
#include "afxdialogex.h"
#include "CreateSessionDlg.h"
#include <Shlwapi.h>  
#pragma comment(lib, "shlwapi.lib")  //Windows API   PathFileExists  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum MenuId {
	AddFolder = WM_USER + 1,
	AddSession,
	Rename,
	Delete,

};


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CputtylauncherMFCDlg dialog



CputtylauncherMFCDlg::CputtylauncherMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PUTTY_LAUNCHER_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CputtylauncherMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_putty_path);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_CHECK1, m_chk_show_pwd);
	DDX_Control(pDX, IDC_EDIT6, m_session_connection_string);
}

BEGIN_MESSAGE_MAP(CputtylauncherMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CputtylauncherMFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CputtylauncherMFCDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CputtylauncherMFCDlg::OnBnClickedButton1)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE1, &CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CputtylauncherMFCDlg::OnNMRClickTree1)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE1, &CputtylauncherMFCDlg::OnTvnItemexpandedTree1)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, &CputtylauncherMFCDlg::OnTvnEndlabeleditTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CputtylauncherMFCDlg::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_CHECK1, &CputtylauncherMFCDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CputtylauncherMFCDlg message handlers

BOOL CputtylauncherMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	findPutty();

	m_chk_show_pwd.SetCheck(0);

	TCHAR szWindows[MAX_PATH + 1] = { 0 };
	GetWindowsDirectory(szWindows, MAX_PATH);
	SHFILEINFO sfi = { 0 };
	UINT nFlags = SHGFI_SYSICONINDEX | (SHGFI_SMALLICON);
	m_image_list = (HIMAGELIST)SHGetFileInfo(szWindows, 0, &sfi, sizeof(sfi), nFlags);
	m_tree.SetImageList(CImageList::FromHandle(m_image_list), 0);
	m_root_item = createItem(L"Sessions", true);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CputtylauncherMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	} else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CputtylauncherMFCDlg::OnPaint()
{
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	} else {
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CputtylauncherMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CputtylauncherMFCDlg::OnBnClickedOk()
{
}

void CputtylauncherMFCDlg::OnBnClickedCancel()
{
}

void CputtylauncherMFCDlg::OnClose()
{
	CDialogEx::OnCancel();
}

void CputtylauncherMFCDlg::OnBnClickedButton1()
{

}

void CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	if (pTVDispInfo->item.hItem == m_root_item) {
		TRACE(L"CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1 1\n");
		*pResult = 1;
	} else {
		TRACE(L"CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1 0 m_is_editting_label true\n");
		m_is_editting_label = true;
		*pResult = 0;
	}
}

void CputtylauncherMFCDlg::OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	if (!pTVDispInfo->item.pszText || wcslen(pTVDispInfo->item.pszText) == 0) {
		*pResult = 0;
	} else {
		*pResult = 1;
	}

	m_is_editting_label = false;

	TRACE(L"m_is_editting_label false\n");
}

void CputtylauncherMFCDlg::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	do {
		CPoint pt;
		GetCursorPos(&pt);
		m_tree.ScreenToClient(&pt);
		HTREEITEM hItem = m_tree.HitTest(pt);
		if (!hItem) {
			break;
		}

		m_cur_item = hItem;

		auto data = getItemData(hItem);
		if (!data) { break; }

		CMenu menu;
		menu.CreatePopupMenu();

		if (data->is_folder) {
			menu.AppendMenuW(MF_STRING, AddFolder, L"Add &Folder");
			menu.AppendMenuW(MF_STRING, AddSession, L"Add &Session");
		}

		if (hItem != m_root_item) {
			menu.AppendMenuW(MF_STRING, Rename, L"&Rename");
			menu.AppendMenuW(MF_STRING, Delete, L"&Delete");
		}

		GetCursorPos(&pt);
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);

	} while (0);
	*pResult = 0;
}


BOOL CputtylauncherMFCDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int menuId = LOWORD(wParam);

	TRACE(L"menuId:%d\n", menuId);

	switch (menuId) {
		if (!m_cur_item) { break; }
	case AddFolder:
		createItem(L"New Folder", true, SessionPtr(), m_cur_item);
		break;
	case AddSession:
	{
		CCreateSessionDlg dlg(this);
		int ret = dlg.DoModal();
		if (ret != IDOK)break;
		createItem(L"", false, dlg.getSession(), m_cur_item);
	}
		break;
	case Rename:
		m_tree.EditLabel(m_cur_item);
		break;
	case Delete:
		break;
	default:
		break;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

BOOL CputtylauncherMFCDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_is_editting_label) {
		if (pMsg->message == WM_KEYUP) {
			if (pMsg->wParam == VK_RETURN) {
				m_tree.EndEditLabelNow(FALSE);
				return TRUE;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CputtylauncherMFCDlg::OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	auto hItem = pNMTreeView->itemNew.hItem;
	auto data = getItemData(hItem);
	if (data && data->is_folder) {
		if (pNMTreeView->action == TVE_EXPAND) {
			m_tree.SetItemImage(hItem, SIID_FOLDEROPEN, SIID_FOLDEROPEN);
		} else if (pNMTreeView->action == TVE_COLLAPSE) {
			m_tree.SetItemImage(hItem, SIID_FOLDER, SIID_FOLDER);
		}
	} else {
		m_tree.SetItemImage(hItem, SIID_SERVER, SIID_SERVER);
	}

	*pResult = 0;
}

void CputtylauncherMFCDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_cur_item = pNMTreeView->itemNew.hItem;
	updateConnectionString(m_cur_item);

	*pResult = 0;
}

void CputtylauncherMFCDlg::findPutty()
{
	wchar_t path[MAX_PATH + 1] = { 0 };
	SHGetSpecialFolderPathW(NULL, path, CSIDL_PROGRAM_FILES, 0);
	CString cpath;
	cpath.Format(L"%s\\PuTTY\\putty.exe", path);
	if (PathFileExistsW(cpath.GetBuffer())) {
		cpath.Format(L"\"%s\\PuTTY\\putty.exe\"", path);
		m_putty_path.SetWindowTextW(cpath);
	}
}

HTREEITEM CputtylauncherMFCDlg::createItem(LPCTSTR text, bool is_folder, SessionPtr session, HTREEITEM parentItem)
{
	int nImage = is_folder ? SIID_FOLDER : SIID_SERVER;
	std::wstring name = text;
	if (session) {
		name = session->name + L" (" + session->username + L"@" + session->host + L")";
	}
	auto hItem = m_tree.InsertItem(name.data(), nImage, nImage, parentItem ? parentItem : TVI_ROOT);
	m_tree_data[hItem] = std::make_shared<ItemData>(is_folder, name, session);
	m_tree.Expand(m_tree.GetParentItem(hItem), TVE_EXPAND);
	m_tree.SelectItem(hItem);
	m_tree.Invalidate();

	return hItem;
}

ItemDataPtr CputtylauncherMFCDlg::getItemData(HTREEITEM hItem) const
{
	auto iter = m_tree_data.find(hItem);
	if (iter != m_tree_data.end()) {
		return iter->second;
	}

	return ItemDataPtr();
}

void CputtylauncherMFCDlg::updateConnectionString(HTREEITEM hItem)
{
	auto data = getItemData(hItem);
	if (data && !data->is_folder && data->session) {
		CString path;
		m_putty_path.GetWindowTextW(path);
		if (path.IsEmpty()) {
			path = L"putty";
		}
		path += L" ";
		path += data->session->connection_string(m_chk_show_pwd.GetCheck() ? true : false).data();
		m_session_connection_string.SetWindowTextW(path);
	}
}

void CputtylauncherMFCDlg::OnBnClickedCheck1()
{
	updateConnectionString(m_cur_item);
}
