
// putty_launcher_MFC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include <unordered_map>
#include <string>
#include <memory>

// CPuttyLauncherMFCApp:
// See putty_launcher_MFC.cpp for the implementation of this class
//

class CPuttyLauncherMFCApp : public CWinApp
{
public:
	CPuttyLauncherMFCApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPuttyLauncherMFCApp theApp;



struct Session {
	std::wstring name = {};
	std::wstring host = {};
	std::wstring port = {};
	std::wstring type = {};
	std::wstring username = {};
	std::wstring password = {};

	// like: " -ssh root@somewhere.com -pw mypasswordforsomewherecom -P 22"
	std::wstring connection_string(bool show_pwd = false) const {
		return std::wstring(L" -") + type + L" "
			+ username + L"@" + host
			+ L" -pw " + (show_pwd ? password : L"********")
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

