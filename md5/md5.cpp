// md5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <cassert>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>

#ifdef _DEBUG
#pragma comment(lib, "F:\\dev_libs\\cryptopp563\\cryptopp\\Win32\\Output\\DebugMDD\\cryptlib.lib")
#else
#pragma comment(lib, "F:\\dev_libs\\cryptopp563\\cryptopp\\Win32\\Output\\ReleaseMd\\cryptlib.lib")
#endif // _DEBUG


std::string md5(const std::string& message)
{
	byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];

	CryptoPP::Weak::MD5 hash;
	hash.CalculateDigest(digest, (const byte*)message.c_str(), message.length());

	CryptoPP::HexEncoder encoder(0, false);
	std::string output;

	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();

	return output;
}

int main(int argc, char** argv)
{
	assert(md5("") == "d41d8cd98f00b204e9800998ecf8427e");
	assert(md5("The quick brown fox jumps over the lazy dog") == "9e107d9d372bb6826bd81d3542a419d6");
	assert(md5("The quick brown fox jumps over the lazy dog.") == "e4d909c290d0fb1ca068ffaddf22cbd0");

	std::string message = argc == 2 ? argv[1] : "";
	std::cout << md5(message) << std::endl;

	return 0;
}
