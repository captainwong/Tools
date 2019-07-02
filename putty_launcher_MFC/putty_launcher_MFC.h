
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


typedef std::wstring String;

struct Session {
	String name = {};
	String host = {};
	String port = {};
	String type = {};
	String username = {};
	String authtype = {};
	String password = {};
	String ppkpath = {};

	// like: " -ssh root@somewhere.com -pw mypasswordforsomewherecom -P 22"
	String connection_string(bool show_pwd = false) const {
		String str = (_T(" -")) + type + _T(" ")
			+ username + _T("@") + host
			+ _T(" -P ") + port;

		if (authtype == _T("ppk")) {
			str += _T(" -i ") + ppkpath;
		} else {
			str += _T(" -pw ") + (show_pwd ? password : _T("********"));
		}

		return str;
	}
};
typedef std::shared_ptr<Session> SessionPtr;

struct ItemData {
	bool is_folder = false;
	String name = {};
	SessionPtr session = {};

	ItemData(bool is_folder = false, const String& name = _T(""), SessionPtr session = {})
		: is_folder(is_folder)
		, name(name)
		, session(session)
	{}

};
typedef std::shared_ptr<ItemData> ItemDataPtr;

