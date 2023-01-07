# AddVersion

A small tool used for visual studio project versoin management.

[reference](http://support.microsoft.com/?id=237870)

链接已失效.

简单说就是先使用 `vs` 的资源管理器删除 `VERSION` 类型的资源（如果没有资源先随意添加任意类型的资源以生成 `rc` 文件），然后手动修改 `rc` 文件，在末尾引入我们需要的版本资源文件 `rc2`

```rc
#ifndef APSTUDIO_INVOKED
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 3 resource.
//

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)
LANGUAGE 9, 1
#include "AlarmCenterPro.rc2"  // non-Microsoft Visual C++ edited resources
#endif

/////////////////////////////////////////////////////////////////////////////
#endif    // not APSTUDIO_INVOKED
```

其中 `#include "AlarmCenterPro.rc2"` 为手动创建的资源文件，仅用来存储版本号，内容为

```rc
//
// AlarmCenterPro.RC2 - resources Microsoft Visual C++ does not edit directly
//

#ifdef APSTUDIO_INVOKED
#error this file is not editable by Microsoft Visual C++
#endif //APSTUDIO_INVOKED


/////////////////////////////////////////////////////////////////////////////
// Add manually edited resources here...
/////////////////////////////////////////////////////////////////////////////
//
// Version
//
#include "VersionNo.h"

#define COMPANY_HENGBO "HengBo"

VS_VERSION_INFO VERSIONINFO
	FILEVERSION FILEVER
	PRODUCTVERSION PRODUCTVER
	FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
	FILEFLAGS 0x1L
#else
	FILEFLAGS 0x0L
#endif
	FILEOS 0x40004L
	FILETYPE 0x1L
	FILESUBTYPE 0x0L
BEGIN
	BLOCK "StringFileInfo"
	BEGIN
		BLOCK "040904B0"
		BEGIN
			VALUE "CompanyName", COMPANY_HENGBO
			VALUE "FileDescription", "AlarmCenterPro"
			VALUE "FileVersion", STRFILEVER
			VALUE "InternalName", "AlarmCenterPro.exe"
			VALUE "LegalCopyright", "(c) HengBo.  All rights reserved."
			VALUE "OriginalFilename", "AlarmCenterPro.exe"
			VALUE "ProductName", "HengBo AlarmCenterPro"
			VALUE "ProductVersion", STRPRODUCTVER
		END
	END
	BLOCK "VarFileInfo"
	BEGIN
		VALUE "Translation", 0x409, 1200
	END
END
/////////////////////////////////////////////////////////////////////////////
```

关键在以下三行

```rc
#include "VersionNo.h"
FILEVERSION FILEVER
PRODUCTVERSION PRODUCTVER
VALUE "FileVersion", STRFILEVER
VALUE "ProductVersion", STRPRODUCTVER
```

`FILEVER, PRODUCTVER, STRFILEVER, STRPRODUCTVER` 是在 `VersionNo.h` 内定义的，因此我们可以方便地改变版本号.

`VersionNo.h` 内容为

```cpp
#pragma once
#define FILEVER 3,8,10,16850
#define PRODUCTVER 3,8,10,16850
#define STRFILEVER "3,8,10,16850\0"
#define STRPRODUCTVER "3,8,10,16850\0"
```

`AfterBuilt.bat` 内容为

```bat
REM usage:
REM "$(ProjectDir)AfterBuild.bat" $(ProjectDir) $(TargetDir) 
echo -----------------------------------------------------
echo Congrats! You've built your solution successfully! :)
echo %1
echo %2
"G:\dev\Tools\Release\AddVersion.exe" "%1VersionNo.h" "%1VersionNo.ini" "b" "1"
copy "%1VersionNo.ini" "%2VersionNo.ini"
echo Add file's version OK!
echo -----------------------------------------------------
```

最后修改工程属性，`Build Events -> Post-Build Event -> Command Line` 内填写

```bat
"$(ProjectDir)AfterBuild.bat" $(ProjectDir) $(TargetDir) 
```

如此即可实现每当编译成功，即触发 `Post-Build Event` 的命令，调用 `AfterBuild.bat`，调用 `AddVersion.exe` 自动将版本号的最后一位加一.

并顺便以旧版本号生成一个 `ini` 文件，可以被其他程序读取为工程的版本号.



File "VersionNo.h" must exsists in project directory, content must be like "major.minor.revision.build", e.g. "1.0.0.2345".

After execute the command, it will generate a file names "VersionNo.ini" in folder "Debug" or "Release" based on your current project setting.

The file "VersionNo.ini" could be used in nsis file to make an installer.

## Usage

```cmd
AddVersion [versionNo.h path] [versionNo.ini path] [section] [number]
section: M m r b
M: major version
m: minor version
r: revision version
b: build times
number: add by number, 1~100 for version increment, 0 for version decrement 1.
```

## Sample

1. AddMajorVersion

	`AddVersion.exe $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini M 1`

2. AddMinorVersion

	`AddVersion.exe $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini m 1`

3. AddRevisionVersion

	`AddVersion.exe $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini r 1`

4. DecRevisionVersion

	`AddVersion.exe $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini r 0`

5. Auto increment build version

	In visual studio, go through Project -- property -- build event -- after build -- command line:

	`$(ProjectDir)AfterBuild.bat $(ProjectDir) $(TargetDir)`
	
	This script will automaticall increment build version everytime you successfully build project.