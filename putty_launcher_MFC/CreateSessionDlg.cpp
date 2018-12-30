// CreateSessionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "putty_launcher_MFC.h"
#include "CreateSessionDlg.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(CCreateSessionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCreateSessionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateSessionDlg message handlers


BOOL CCreateSessionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (session) {
		m_name.SetWindowTextW(session->name.data());
		m_host.SetWindowTextW(session->host.data());
		m_port.SetWindowTextW(session->port.data());
		m_user.SetWindowTextW(session->username.data());
		m_passwd.SetWindowTextW(session->password.data());

	}

	m_radio_ssh.SetCheck(1);
	//m_radio_ssh.EnableWindow(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CCreateSessionDlg::OnBnClickedOk()
{
	CString name, host, port, user, pass;
	m_name.GetWindowTextW(name);
	m_host.GetWindowTextW(host);
	m_port.GetWindowTextW(port);
	m_user.GetWindowTextW(user);
	m_passwd.GetWindowTextW(pass);

	if (name.IsEmpty() || host.IsEmpty() || port.IsEmpty() || user.IsEmpty() || pass.IsEmpty()) {
		MessageBoxW(L"Can't be empty!", L"Error", MB_ICONERROR);
		return;
	}

	if (!session) {
		session = std::make_shared<Session>();
	}
	
	session->name = name.GetBuffer();
	session->host = host.GetBuffer();
	session->port = port.GetBuffer();
	session->type = L"ssh";
	session->username = user.GetBuffer();
	session->password = pass.GetBuffer();

	CDialogEx::OnOK();
}
