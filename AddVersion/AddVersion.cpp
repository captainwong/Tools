// AddVersion.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <jlib/obsolete/deelx.h>

/*
1.After build:
	Project -- property -- build event -- after build -- command line:
	"$(ProjectDir)AfterBuild.bat" $(ProjectDir) $(TargetDir)
2.AddMajorVersion: $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini M 1
3.AddMinorVersion: $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini m 1
4.AddRevisionVersion: $(ProjectDir)VersionNo.h $(ProjectDir)VersionNo.ini r 1


AfterBuild.bat:
echo %1
echo %2
AddVersion.exe "%1VersionNo.h" "%1VersionNo.ini" "b" "1"
copy "%1VersionNo.ini" "%2VersionNo.ini"
*/

int do_pattern(const TCHAR* file_path, const char* pattern, const char* shed,
	int inc_pos, int inc_by, char* old_version, int old_vlen);



int _tmain(int argc, _TCHAR* argv[])
{
	//TCHAR ab[] = "003f\x22abc";// hengbo - bo\x22abcdef\x5b\x5d - 03 - 02 - 11\x0a";
	//int i = 0;
	do {
		if (argc != 5)
			break;

		const TCHAR* h_file = argv[1];
		const TCHAR* i_file = argv[2];

		size_t section_len = wcslen(argv[3]);
		if (section_len != 1)
			break;

		size_t number_len = wcslen(argv[4]);
		if (number_len != 1)
			break;

		wchar_t section = argv[3][0];
		int number = _wtoi(argv[4]);
		if (!(0 <= number && number < 100))
			break;

		// 0 for decrement 1
		if (number == 0) {
			number = -1;
		}

		const char* pattern = "(\\d+)[,.]\\s?(\\d+)[,.]\\s?(\\d+)[,.]\\s?(\\d+)";
		int inc_pos = 0;
		bool true_command = true;
		switch (section) {
		case L'M':
			inc_pos = 1;
			break;
		case L'm':
			inc_pos = 2;
			break;
		case L'r':
			inc_pos = 3;
			break;
		case L'b':
			inc_pos = 4;
			break;
		default:
			true_command = false;
			break;
		}

		if (!true_command)
			break;

		//const TCHAR* h_file = L"D:/AlarmCenterNet/AlarmCenter/VersionNo.h";
		//const TCHAR* i_file = L"D:/AlarmCenterNet/AlarmCenter/versionno.ini";
		//TCHAR snumber[32] = { 0 };
		//_itoa_s(number, snumber, 32, 10);
		char old_ver[128] = { 0 };
		do_pattern(h_file, pattern, ",", inc_pos, number, old_ver, sizeof(old_ver));
		//do_pattern(i_file, pattern, ".", inc_pos, number);

		do {
			FILE* file = NULL;
			_tfopen_s(&file, i_file, L"wb");
			if (file == NULL) {
				wprintf(L"open '%s' failed\n", i_file);
				break;
			}
			printf("ini new version: %s\n", old_ver);
			fwrite(old_ver, sizeof(char), strlen(old_ver), file);
			fflush(file);
			fclose(file);
		} while (0);

		return 0;
	} while (0);

	for (int i = 0; i < argc; i++) {
		wprintf(L"%s\n", argv[i]);
	}
	wprintf_s(L"\
usage: AddVersion [versionNo.h path] [versionNo.ini path] [section] [number]\r\n\
section: M m r b\r\n\
\tM: major version\r\n\
\tm: minor version\r\n\
\tr: revision version\r\n\
\tb: build times\r\n\
number: add by number, 1~100 for version increment, 0 for version decrement 1.\r\n");
	return 0;
}

int do_pattern(const TCHAR* file_path, const char* pattern, const char* shed, int inc_pos, int inc_by, char* old_version, int old_vlen)
{
	int oplen = 0;
	FILE *file = NULL;
	_tfopen_s(&file, file_path, L"rb");
	if (file == NULL)
		return -1;
	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	char *buffer = new char[length + 1];
	memset(buffer, 0, length + 1);
	fseek(file, 0, SEEK_SET);
	oplen = fread_s(buffer, (length + 1) * sizeof(char), 1, length, file);
	fclose(file);

	CRegexpT <char> regexp;
	regexp.Compile(pattern);
	MatchResult result = regexp.Match(buffer);
	while (result.IsMatched()) {
		int start = result.GetStart();
		int end = result.GetEnd();
		int content_len = end - start;
		if (content_len <= 0)
			break;

		char group1[32] = { 0 };
		char group2[32] = { 0 };
		char group3[32] = { 0 };
		char group4[32] = { 0 };
		int group1_len = result.GetGroupEnd(1) - result.GetGroupStart(1);
		int group2_len = result.GetGroupEnd(2) - result.GetGroupStart(2);
		int group3_len = result.GetGroupEnd(3) - result.GetGroupStart(3);
		int group4_len = result.GetGroupEnd(4) - result.GetGroupStart(4);
		if (group1_len <= 0 || group2_len <= 0 || group3_len <= 0 || group4_len <= 0)
			break;

		strncpy_s(group1, 32, buffer + result.GetGroupStart(1), group1_len);
		strncpy_s(group2, 32, buffer + result.GetGroupStart(2), group2_len);
		strncpy_s(group3, 32, buffer + result.GetGroupStart(3), group3_len);
		strncpy_s(group4, 32, buffer + result.GetGroupStart(4), group4_len);
		int number1 = atoi(group1);
		int number2 = atoi(group2);
		int number3 = atoi(group3);
		int number4 = atoi(group4);

		if (old_version) {
			snprintf(old_version, old_vlen, "%d.%d.%d.%d\n", number1, number2, number3, number4);
			printf("old version: %s\n", old_version);
		}

		char* result_content = new char[content_len * 2];
		memset(result_content, 0, content_len * 2);
		if (inc_pos == 1) {
			number1+= inc_by;
			number2 = 0;
			number3 = 0;
		} else if (inc_pos == 2) {
			number2+= inc_by;
			number3 = 0;
		} else if (inc_pos == 3) {
			number3+= inc_by;
		} else if (inc_pos == 4) {
			number4+= inc_by;
		}

		sprintf_s(result_content, content_len * 2, "%d%s%d%s%d%s%d",
			number1, shed, number2, shed,
			number3, shed, number4);
		printf_s("header new version: %s\n", result_content);

		char* content = regexp.Replace(buffer, result_content);
		delete[] result_content;
		_tfopen_s(&file, file_path, L"wb");
		if (file == NULL) {
			regexp.ReleaseString(content);
			break;
		}
		oplen = fwrite(content, sizeof(char), strlen(content), file);
		fflush(file);
		fclose(file);
		regexp.ReleaseString(content);
		return 0;
	}
	wprintf_s(L"do_pattern failed\n");
	return -1;
}
