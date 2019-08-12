// CreateSessionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "putty_launcher_MFC.h"
#include "CreateSessionDlg.h"
#include "afxdialogex.h"
#include <jlib/jlib/win32/file_op.h>

// CCreateSessionDlg dialog

IMPLEMENT_DYNAMIC(CCreateSessionDlg, CDialogEx)

CCreateSessionDlg::CCreateSessionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CREATE_SESSION, pParent)
{

}

CCreateSessionDlg::~CCreateSessionDlg()
{
}

void CCreateSessionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_name);
	DDX_Control(pDX, IDC_EDIT2, m_host);
	DDX_Control(pDX, IDC_EDIT3, m_port);
	DDX_Control(pDX, IDC_RADIO1, m_radio_ssh);
	DDX_Control(pDX, IDC_EDIT4, m_user);
	DDX_Control(pDX, IDC_EDIT5, m_passwd);
	DDX_Control(pDX, IDC_EDIT6, m_ppk);
	DDX_Control(pDX, IDC_BUTTON_PPK, m_btn_ppk);
	DDX_Control(pDX, IDC_RADIO_AUTH_PWD, m_radio_pwd);
	DDX_Control(pDX, IDC_RADIO_AUTH_PPK, m_radio_ppk);
}


BEGIN_MESSAGE_MAP(CCreateSessionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCreateSessionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_AUTH_PWD, &CCreateSessionDlg::OnBnClickedRadioAuthPwd)
	ON_BN_CLICKED(IDC_RADIO_AUTH_PPK, &CCreateSessionDlg::OnBnClickedRadioAuthPpk)
	ON_BN_CLICKED(IDC_BUTTON_PPK, &CCreateSessionDlg::OnBnClickedButtonPpk)
END_MESSAGE_MAP()


// CCreateSessionDlg message handlers


BOOL CCreateSessionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_radio_pwd.SetCheck(1);
	OnBnClickedRadioAuthPwd();

	if (session) {
		m_name.SetWindowText(session->name.data());
		m_host.SetWindowText(session->host.data());
		m_port.SetWindowText(session->port.data());
		m_user.SetWindowText(session->username.data());

		if (session->authtype == _T("ppk")) {
			m_ppk.SetWindowText(session->ppkpath.data());
			m_radio_ppk.SetCheck(1);
			OnBnClickedRadioAuthPpk();
		} else {
			m_passwd.SetWindowText(session->password.data());
		}
	}

	m_radio_ssh.SetCheck(1);
	//m_radio_ssh.EnableWindow(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CCreateSessionDlg::OnBnClickedOk()
{
	CString name, host, port, user, pass, ppk;
	m_name.GetWindowText(name);
	m_host.GetWindowText(host);
	m_port.GetWindowText(port);
	m_user.GetWindowText(user);
	m_passwd.GetWindowText(pass);
	m_ppk.GetWindowText(ppk);

	if (name.IsEmpty() || host.IsEmpty() || port.IsEmpty() || user.IsEmpty()) {
		MessageBox(_T("Can't be empty!"), _T("Error"), MB_ICONERROR);
		return;
	}

	if (!session) {
		session = std::make_shared<Session>();
	}
	
	session->name = name.GetBuffer();
	session->host = host.GetBuffer();
	session->port = port.GetBuffer();
	session->type = _T("ssh");
	session->username = user.GetBuffer();
	session->authtype = m_radio_ppk.GetCheck() ? _T("ppk") : _T("pwd");
	session->password = pass.GetBuffer();
	session->ppkpath = ppk.GetBuffer();

	CDialogEx::OnOK();
}

void CCreateSessionDlg::OnBnClickedRadioAuthPwd()
{
	m_passwd.EnableWindow();
	m_ppk.EnableWindow(0);
	m_btn_ppk.EnableWindow(0);
}

void CCreateSessionDlg::OnBnClickedRadioAuthPpk()
{
	m_passwd.EnableWindow(0);
	m_ppk.EnableWindow();
	m_btn_ppk.EnableWindow();
}


void CCreateSessionDlg::OnBnClickedButtonPpk()
{
	std::wstring path;
	COMDLG_FILTERSPEC cf[2] = {};
	cf[0].pszName = L"PPK Files";
	cf[0].pszSpec = L"*.ppk";
	cf[1].pszName = L"All Files";
	cf[1].pszSpec = L"*.*";
	if (!jlib::get_file_open_dialog_result(path, m_hWnd, L"", L"ppk", 2, cf)) {
		return;
	}

	m_ppk.SetWindowText(((L"\"" + path + L"\"").data()));
}
