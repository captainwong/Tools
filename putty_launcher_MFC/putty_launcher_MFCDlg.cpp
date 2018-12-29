
// putty_launcher_MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "putty_launcher_MFC.h"
#include "putty_launcher_MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum MenuId {
	AddFolder = WM_USER + 1,
	AddSession,
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
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	TCHAR szWindows[MAX_PATH + 1] = { 0 };
	GetWindowsDirectory(szWindows, MAX_PATH);
	SHFILEINFO sfi = { 0 };
	UINT nFlags = SHGFI_SYSICONINDEX | (SHGFI_SMALLICON);
	m_image_list = (HIMAGELIST)SHGetFileInfo(szWindows, 0, &sfi, sizeof(sfi), nFlags);
	m_tree.SetImageList(CImageList::FromHandle(m_image_list), 0);
	m_root_item = m_tree.InsertItem(L"Sessions", SIID_FOLDER, SIID_FOLDER);
	m_tree_data[m_root_item] = std::make_shared<ItemData>(true, L"Sessions");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CputtylauncherMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CputtylauncherMFCDlg::OnPaint()
{
	if (IsIconic())
	{
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
	}
	else
	{
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

void CputtylauncherMFCDlg::OnBnClickedButton1()
{

}

void CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	if (pTVDispInfo->item.hItem == m_root_item) {
		TRACE(L"CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1 1");
		*pResult = 1;
	} else {
		TRACE(L"CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1 0");
		*pResult = 0;
	}
}

void CputtylauncherMFCDlg::OnClose()
{
	CDialogEx::OnCancel();
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

		ItemDataPtr data = {};
		{
			auto iter = m_tree_data.find(hItem);
			if (iter == m_tree_data.end()) { break; }
			data = iter->second;
		}

		CMenu menu;
		menu.CreatePopupMenu();

		if (data->is_folder) {
			menu.AppendMenuW(MF_STRING, AddFolder, L"Add Folder");
		}

		menu.AppendMenuW(MF_STRING, AddSession, L"Add Session");

		if (hItem != m_root_item) {
			menu.AppendMenuW(MF_STRING, Delete, L"Delete");
		}

		GetCursorPos(&pt);
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);

	} while (0);
	*pResult = 0;
}


BOOL CputtylauncherMFCDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	int menuId = LOWORD(wParam);

	TRACE(L"menuId:%d", menuId);

	switch (menuId) {
	case AddFolder:
		break;
	case AddSession:
		break;
	case Delete:
		break;
	default:
		break;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


void CputtylauncherMFCDlg::OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	auto hItem = pNMTreeView->itemNew.hItem;
	if (pNMTreeView->action == TVE_EXPAND) {
		m_tree.SetItemImage(hItem, SIID_FOLDEROPEN, SIID_FOLDEROPEN);
	} else if (pNMTreeView->action == TVE_COLLAPSE) {
		m_tree.SetItemImage(hItem, SIID_FOLDER, SIID_FOLDER);
	}
	*pResult = 0;
}
