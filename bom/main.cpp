#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <functional>
#include <algorithm>
#include <cctype>
#include <sstream>

#include <Global/jlib/std_util.h>

using namespace std;
namespace fs = std::experimental::filesystem;

const unsigned char bom[3] = { 0xEF, 0xBB, 0xBF };

typedef std::function<bool(const fs::path&)> operation;
typedef std::function<bool(const fs::path&)> validator;


void usage()
{
	cout << "Usage: bom [option] [folder] " << endl
		<< "Valid options: " << endl
		<< "\tadd: add bom" << endl
		<< "\trem: remove bom" << endl
		<< "Example: " << endl
		<< "\t'bom add C:/proj1' will add boms for all files in C:/proj1" << endl
		<< "\t'bom rem C:/proj2' will remove all boms for all files in C:/proj2" << endl;
}

bool is_bom(const char* buff)
{
	return static_cast<unsigned char>(buff[0]) == bom[0]
		&& static_cast<unsigned char>(buff[1]) == bom[1]
		&& static_cast<unsigned char>(buff[2]) == bom[2];
}

const char* get_bom()
{
	return reinterpret_cast<const char*>(bom);
}

bool validator_for_c_cpp(const fs::path& path)
{
	vector<string> valid_exts = { "h", "c", "cpp", "hpp", "cxx" };
	auto ext = path.extension().string();
	if (!ext.empty()) {
		ext = ext.substr(1);
		std::transform(ext.begin(), ext.end(), ext.begin(), std::tolower);//[](char c) { return static_cast<char>(std::tolower(c)); }
		if (jlib::is_contain(valid_exts, ext)) {
			return true;
		}
	}

	return false;
}

bool add_bom(const fs::path& fpath)
{
	std::ifstream f(fpath.wstring());
	if (!f) { cout << "Cannot open file: " << fpath << endl; return false; }

	f.seekg(0, f.end);
	size_t size = f.tellg();
	f.seekg(0, f.beg);

	if (size > 3) {
		char buff[3] = { 0 };
		f.read(buff, 3);
		if (!is_bom(buff)) {
			f.seekg(0, f.beg);
			auto p = new char[size + 3];
			memcpy(p, get_bom(), 3);
			f.read(p + 3, size);
			f.close();

			std::ofstream out(fpath.wstring(), ios::binary | ios::out);
			out.write(p, size);
			out.close();
			delete[] p;
		}
	}	

	return true;
}

bool remove_bom(const fs::path& fpath)
{
	std::fstream f(fpath.wstring(), ios::binary | ios::in);
	if (!f) { cout << "Cannot open file: " << fpath << endl; return false; }

	char buff[3] = { 0 };
	f.read(buff, 3);
	if (is_bom(buff)) {
		std::stringstream ss;
		ss << f.rdbuf();
		f.close();
		f.open(fpath.wstring(), ios::binary | ios::out);
		f << ss.rdbuf();
		f.close();
	}

	return true;
}

void triverse_folder(const fs::path& folder, validator vtor, operation oper)
{
	for (const auto& p : fs::recursive_directory_iterator(folder)) {
		if (fs::is_regular_file(p) && vtor && vtor(p.path()) && oper) {
			oper(p.path());
		}
	}
}

int wmain(int argc, wchar_t** argv)
{
	if (argc != 3) {
		usage();
		return 0;
	}

	wstring option, folder;
	option = argv[1];
	folder = argv[2];

	error_code ec;
	auto path = fs::canonical(folder, ec);
	if (ec) {
		cout << ec.message() << endl;
		usage();
		return 0;
	}

	if (!fs::is_directory(path) || !fs::exists(path)) {
		cout << "Folder is not a valid directory!" << endl;
		usage();
		return 0;
	}

	if (option == L"add") {
		triverse_folder(path, validator_for_c_cpp, add_bom);
	} else if (option == L"rem") {
		triverse_folder(path, validator_for_c_cpp, remove_bom);
	} else {
		cout << "Not a valid option!" << endl;
		usage();
	}

}
