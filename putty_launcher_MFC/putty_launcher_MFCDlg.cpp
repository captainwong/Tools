
// putty_launcher_MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "putty_launcher_MFC.h"
#include "putty_launcher_MFCDlg.h"
#include "afxdialogex.h"
#include "CreateSessionDlg.h"
#include <Shlwapi.h>  
#pragma comment(lib, "shlwapi.lib")  //Windows API   PathFileExists  
#include <jlib/jlib/win32/file_op.h>
#include <jlib/jlib/utf8.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum MenuId {
	AddFolder = WM_USER + 1,
	AddSession,
	Rename,
	Ping,
	Connect,
	Edit,
	Delete,

};


#define MY_A2W(s) utf8::a2w(s).data()
#define MY_W2A utf8::w2a


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
	DDX_Control(pDX, IDC_BUTTON_CONNECT_SESSION, m_btn_connect_session);
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
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CputtylauncherMFCDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_SESSION, &CputtylauncherMFCDlg::OnBnClickedButtonConnectSession)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CputtylauncherMFCDlg::OnNMDblclkTree1)
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

	initTree();
	loadFromJson();

	m_tree.SelectItem(m_root_item);
	m_tree.Expand(m_root_item, TVE_EXPAND);
	m_tree.Invalidate();

	m_btn_connect_session.EnableWindow(0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CputtylauncherMFCDlg::initTree()
{
	TCHAR szWindows[MAX_PATH + 1] = { 0 };
	GetWindowsDirectory(szWindows, MAX_PATH);
	SHFILEINFO sfi = { 0 };
	UINT nFlags = SHGFI_SYSICONINDEX | (SHGFI_SMALLICON);
	m_image_list = (HIMAGELIST)SHGetFileInfo(szWindows, 0, &sfi, sizeof(sfi), nFlags);
	m_tree.SetImageList(CImageList::FromHandle(m_image_list), 0);
	m_root_item = createItem(_T("Sessions"), nullptr, true);
}

CString CputtylauncherMFCDlg::getDataPath() const
{
	CString path;
	AfxGetModuleFileName(AfxGetInstanceHandle(), path);
	path = path.Left(path.ReverseFind('\\') + 1);
	path += _T("putty_launcher");
	CreateDirectory(path, NULL);
	return path;
}

void CputtylauncherMFCDlg::parseFolderAndCreate(Json::Value & folders, HTREEITEM parentItem)
{
	//USES_CONVERSION;
	if (folders.isArray()) {
		for (Json::ArrayIndex i = 0; i < folders.size(); i++) {
			auto& folder = folders[i];
			auto hItem = createItem(MY_A2W(folder["name"].asCString()), parentItem, true, folder["expand"].asBool(), false);

			auto sub_folders = folder["folders"];
			parseFolderAndCreate(sub_folders, hItem);

			auto& sessions = folder["sessions"];
			parseSessionAndCreate(sessions, hItem);
		}
	}
}

void CputtylauncherMFCDlg::parseSessionAndCreate(Json::Value & sessions, HTREEITEM parentItem)
{
	//USES_CONVERSION;
	if (sessions.isArray()) {
		for (Json::ArrayIndex i = 0; i < sessions.size(); i++) {
			auto& session = sessions[i];
			CString name = MY_A2W(session["name"].asCString());
			CString host = MY_A2W(session["host"].asCString());
			CString port = MY_A2W(session["port"].asCString());
			CString type = MY_A2W(session["type"].asCString());
			CString username = MY_A2W(session["username"].asCString());

			CString authtype = _T("pwd");
			if (session.isMember("authtype")) {
				authtype = MY_A2W(session["authtype"].asCString());
			}

			CString password = MY_A2W(session["password"].asCString());

			CString ppkpath = _T("");
			if (session.isMember("ppkpath")) {
				ppkpath = MY_A2W(session["ppkpath"].asCString());
			}

			auto ss = std::make_shared<Session>();
			ss->name = name.GetBuffer();
			ss->host = host.GetBuffer();
			ss->port = port.GetBuffer();
			ss->type = type.GetBuffer();
			ss->username = username.GetBuffer();
			ss->authtype = authtype.GetBuffer();
			ss->password = password.GetBuffer();
			ss->ppkpath = ppkpath.GetBuffer();

			createItem(name, parentItem, false, false, false, ss);
		}
	}
}

void CputtylauncherMFCDlg::parseTreeAndSave(HTREEITEM parentItem, Json::Value & value)
{
	//USES_CONVERSION;
	auto& folders = value["folders"];
	auto& sessions = value["sessions"];

	auto hItem = m_tree.GetChildItem(parentItem);
	while (hItem) {
		auto data = getItemData(hItem);
		if (!data)break;
		if (data->is_folder) {
			Json::Value folder;
			folder["name"] = MY_W2A(data->name.data());
			folder["expand"] = (m_tree.GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED) ? true : false;

			parseTreeAndSave(hItem, folder);
			folders.append(folder);
		} else {
			const auto& ss = data->session;
			Json::Value session;

#define saveKey(key) session[#key] = MY_W2A(ss->key.data());

			//saveKey(name);

			session["name"] = MY_W2A(ss->name.data());
			session["host"] = MY_W2A(ss->host.data());
			session["port"] = MY_W2A(ss->port.data());
			session["type"] = MY_W2A(ss->type.data());
			session["username"] = MY_W2A(ss->username.data());
			session["authtype"] = MY_W2A(ss->authtype.data());
			session["password"] = MY_W2A(ss->password.data());
			session["ppkpath"] = MY_W2A(ss->ppkpath.data());
			sessions.append(session);
		}

		hItem = m_tree.GetNextSiblingItem(hItem);
	}
}

void CputtylauncherMFCDlg::loadFromJson()
{
	//USES_CONVERSION;

	auto path = getDataPath() + _T("\\putty_launcher.json");
	CFile f;
	if (f.Open(path, CFile::modeRead)) {
		UINT len = f.GetLength();
		auto p = new char[len + 1];
		f.Read(p, len);
		p[len] = 0;

		Json::Value root;
		Json::Reader reader;
		if (reader.parse(p, p + len, root)) {
			m_putty_path.SetWindowText(MY_A2W(root["putty"].asCString()));
			m_chk_show_pwd.SetCheck(root["show_pwd"].asInt());

			auto& data = root["data"];
			parseFolderAndCreate(data["folders"], m_root_item);
			parseSessionAndCreate(data["sessions"], m_root_item);

			delete[] p;
			return;
		}

		delete[] p;
	}

	findPutty();
	m_chk_show_pwd.SetCheck(0);
}

void CputtylauncherMFCDlg::saveToJson()
{
	//USES_CONVERSION;
	Json::Value root;

	CString putty_path;
	m_putty_path.GetWindowText(putty_path);
	root["putty"] = MY_W2A(putty_path.GetBuffer());
	root["show_pwd"] = m_chk_show_pwd.GetCheck();

	auto& data = root["data"];
	parseTreeAndSave(m_root_item, data);
	
	auto content = Json::StyledWriter().write(root);
	auto path = getDataPath() + _T("\\putty_launcher.json");
	CFile f;
	if (f.Open(path, CFile::OpenFlags::modeWrite | CFile::OpenFlags::modeCreate)) {
		f.Write(content.data(), content.length());
	}
}

void CputtylauncherMFCDlg::findPutty()
{
	TCHAR path[MAX_PATH + 1] = { 0 };
	SHGetSpecialFolderPath(NULL, path, CSIDL_PROGRAM_FILES, 0);
	CString cpath;
	cpath.Format(_T("%s\\PuTTY\\putty.exe"), path);
	if (PathFileExists(cpath.GetBuffer())) {
		cpath.Format(_T("\"%s\\PuTTY\\putty.exe\""), path);
		m_putty_path.SetWindowText(cpath);
	}
}

HTREEITEM CputtylauncherMFCDlg::createItem(LPCTSTR text, HTREEITEM parentItem, bool is_folder, bool expand, bool refresh, SessionPtr session)
{
	int nImage = is_folder ? SIID_FOLDER : SIID_SERVER;

	HTREEITEM hItemAfter = TVI_LAST;
	if (is_folder) {
		hItemAfter = TVI_FIRST;
		auto hItem = m_tree.GetChildItem(parentItem);
		while (hItem) {
			auto data = getItemData(hItem);
			if (data && data->is_folder) {
				hItemAfter = hItem;
			} else {
				break;
			}

			hItem = m_tree.GetNextSiblingItem(hItem);
		}
	}

	String name = text;
	if (session) {
		name = session->name + _T(" (") + session->username + _T("@") + session->host + _T(")");
	}

	auto hItem = m_tree.InsertItem(name.data(), nImage, nImage, parentItem ? parentItem : TVI_ROOT, hItemAfter);
	m_tree_data[hItem] = std::make_shared<ItemData>(is_folder, name, session);

	if (expand) {
		m_tree.Expand(m_tree.GetParentItem(hItem), TVE_EXPAND);
		m_tree.Expand(hItem, TVE_EXPAND);
	}

	if (refresh) {
		m_tree.SelectItem(hItem);
		m_tree.Invalidate();
	}

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
		m_putty_path.GetWindowText(path);
		if (path.IsEmpty()) {
			path = _T("putty");
		}
		path += _T(" ");
		path += data->session->connection_string(m_chk_show_pwd.GetCheck() ? true : false).data();
		m_session_connection_string.SetWindowText(path);
		m_btn_connect_session.EnableWindow();
	} else {
		m_session_connection_string.SetWindowText(_T(""));
		m_btn_connect_session.EnableWindow(0);
	}
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
	saveToJson();
	CDialogEx::OnCancel();
}

void CputtylauncherMFCDlg::OnBnClickedButton1()
{
	std::wstring path;
	COMDLG_FILTERSPEC cf[2] = {};
	cf[0].pszName = L"Exe Files";
	cf[0].pszSpec = L"*.exe";
	cf[1].pszName = L"All Files";
	cf[1].pszSpec = L"*.*";

	if (jlib::win32::getOpenFileDialogResult(path, m_hWnd, L"", L"exe", 2, cf)) {
		path.insert(0, 1, L'\"');
		path.push_back(L'\"');
		m_putty_path.SetWindowText((path.data()));
	}
}

void CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	if (pTVDispInfo->item.hItem == m_root_item) {
		TRACE(L"CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1 1\n");
		*pResult = 1;
	} else {
		auto data = getItemData(pTVDispInfo->item.hItem);
		if (data && data->is_folder) {
			TRACE(L"CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1 0\n");
			*pResult = 0;
		} else {
			TRACE(L"CputtylauncherMFCDlg::OnTvnBeginlabeleditTree1 1 m_is_editting_label true\n");
			m_is_editting_label = true;
			*pResult = 1;
		}
	}
}

void CputtylauncherMFCDlg::OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	if (!pTVDispInfo->item.pszText || _tcslen(pTVDispInfo->item.pszText) == 0) {
		
	} else {
		auto data = getItemData(pTVDispInfo->item.hItem);
		if (data) {
			if (data->is_folder) {
				data->name = pTVDispInfo->item.pszText;
				TRACE(_T("%s\n"), pTVDispInfo->item.pszText);
			} else {

			}
			*pResult = 1;
		}
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
			menu.AppendMenu(MF_STRING, AddFolder, _T("Add &Folder"));
			menu.AppendMenu(MF_STRING, AddSession, _T("Add &Session"));
			menu.AppendMenu(MF_STRING, Rename, _T("&Rename"));
		} else {
			menu.AppendMenu(MF_STRING, Ping, _T("&Ping"));
			menu.AppendMenu(MF_STRING, Connect, _T("&Connect"));
			menu.AppendMenu(MF_STRING, Edit, _T("&Edit"));
		}

		if (hItem != m_root_item) {
			menu.AppendMenu(MF_STRING, Delete, _T("&Delete"));
		}

		GetCursorPos(&pt);
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);

	} while (0);
	*pResult = 0;
}

void CputtylauncherMFCDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
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
		OnBnClickedButtonConnectSession();

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
		createItem(_T("New Folder"), m_cur_item, true);
		break;
	case AddSession:
	{
		CCreateSessionDlg dlg(this);
		int ret = dlg.DoModal();
		if (ret != IDOK)break;
		createItem(_T(""), m_cur_item, false, true, true, dlg.getSession());
	}
		break;
	case Rename:
		m_tree.EditLabel(m_cur_item);
		break;
	case Ping:
	{
		auto data = getItemData(m_cur_item);
		if (data && !data->is_folder && data->session) {
			ShellExecuteW(NULL, NULL, L"cmd", (L"/C ping " + data->session->host + L" -t").data(), NULL, SW_SHOWNORMAL);
		}
	}
		break;

	case Connect:
		OnBnClickedButtonConnectSession();
		break;
	case Edit:
	{
		auto session = getItemData(m_cur_item)->session;
		CCreateSessionDlg dlg(this);
		dlg.setEditMode(session);
		int ret = dlg.DoModal();
		if (ret != IDOK)break;
		String text = session->name + _T(" (") + session->username + _T("@") + session->host + _T(")");
		m_tree.SetItemText(m_cur_item, text.data());
		updateConnectionString(m_cur_item);
	}
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
		if (pMsg->message == WM_KEYDOWN) {
			if (pMsg->wParam == VK_RETURN) {
				m_is_editting_label = false;
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

void CputtylauncherMFCDlg::OnBnClickedCheck1()
{
	updateConnectionString(m_cur_item);
}

void CputtylauncherMFCDlg::OnBnClickedButtonSave()
{
	saveToJson();
}

void CputtylauncherMFCDlg::OnBnClickedButtonConnectSession()
{
	auto data = getItemData(m_cur_item);
	if (data && !data->is_folder && data->session) {
		CString path;
		m_putty_path.GetWindowText(path);
		if (path.IsEmpty()) {
			path = _T("putty");
		}
		
		CString param = data->session->connection_string(true).data();

		//
		//std::system((path));

		ShellExecute(m_hWnd, _T("open"), path, param, NULL, SW_SHOW);
	}
}

