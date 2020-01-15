// md5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cassert>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>

#ifdef _DEBUG
#pragma comment(lib, "G:\\dev_libs\\cryptopp563\\cryptopp\\Win32\\Output\\DebugMDD\\cryptlib.lib")
#else
#pragma comment(lib, "G:\\dev_libs\\cryptopp563\\cryptopp\\Win32\\Output\\ReleaseMd\\cryptlib.lib")
#endif // _DEBUG


std::string fmd5(const std::string& file)
{
	/*std::string digest;
	CryptoPP::Weak1::MD5 md5;
	CryptoPP::HashFilter hashfilter(md5);
	hashfilter.Attach(new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest), false));
	CryptoPP::FileSource(file.c_str(), true, &hashfilter);
	return digest;*/

	CryptoPP::Weak1::MD5 md5;
	const size_t size = CryptoPP::Weak1::MD5::DIGESTSIZE * 2;
	byte buf[size] = { 0 };
	CryptoPP::FileSource(file.c_str(), true, new CryptoPP::HashFilter(md5, new CryptoPP::HexEncoder(new CryptoPP::ArraySink(buf, size), false)));
	std::string digest = std::string(reinterpret_cast<const char*>(buf), size);
	return digest;
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage: %s file\n", argv[0]);
		exit(0);
	}

	std::string file = argv[1];
	std::cout << fmd5(file) << std::endl;

	return 0;
}
