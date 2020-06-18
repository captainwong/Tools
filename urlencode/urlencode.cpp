#ifdef _WIN32
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifdef _WIN32
#include <jlib/win32/UnicodeTool.h>
#endif

char rfc3986[256] = { 0 };

void init()
{
	for (int i = 0; i < 256; i++) {
		rfc3986[i] = isalnum((unsigned char)i) || i == '~' || i == '-' || i == '.' || i == '_'
			? i
			: 0;
	}
}

/* caller responsible for memory */
void encode(const char* s, char* enc, char* tb)
{
	for (; *s; s++) {
		if (tb[(unsigned char)*s])
			sprintf(enc, "%c", (unsigned char)tb[*s]);
		else
			sprintf(enc, "%%%02X", (unsigned char)(*s));
		while (*++enc)
			;
	}
}

/* caller responsible for memory */
char* url_encode(const char* str)
{
	char* enc = (char*)malloc(strlen(str) * 3 + 1);
	enc[0] = 0;
	encode(str, enc, rfc3986);
	return enc;
}

void usage(const char* exe)
{
	printf("Usage: %s <content>"
#ifdef _WIN32
		   " [utf8]\n"
#else
		   "\n"
#endif

		   "content: Content to be url-encoded\n"
#ifdef _WIN32
		   "utf8:    By default, content is treated as mbcs, you can use `%s <content> utf8` to convert content to utf-8 and then do url-encode\n",
		   exe, exe);
#else
		, exe);
#endif
		   
}

int main(int argc, char** argv)
{
	init();

#ifdef _DEBUG
	{
		const char s[] = { ' ', '\0' };
		char* enc = url_encode(s);
		assert(strcmp("%20", enc) == 0);
		free(enc);
	}

	{
		// 我是中国人 utf8
		const unsigned char s[] = { 0xE6, 0x88, 0x91, 0xE6, 0x98, 0xAF, 0xE4, 0xB8, 0xAD, 0xE5, 0x9B, 0xBD, 0xE4, 0xBA, 0xBA, '\0' };
		char* enc = url_encode((const char*)s);
		assert(strcmp("%E6%88%91%E6%98%AF%E4%B8%AD%E5%9B%BD%E4%BA%BA", enc) == 0);
		free(enc);
	}
#endif

	if (argc < 2) {
		usage(argv[0]);
		return 0;
	}

	const char* content = argv[1];

#ifdef _WIN32
	bool convert_to_utf8 = argc > 2 && strcmp(argv[2], "utf8") == 0;
	std::string content_;	
	if (convert_to_utf8) {
		content_ = jlib::win32::mbcs_to_utf8(argv[1]);
		content = content_.data();
	}
#endif

	char* enc = url_encode(content);
	printf("%s\n", enc);
	free(enc);

	return 0;
}
