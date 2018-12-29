#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>

int main()
{
	time_t t = time(nullptr);
	auto now = localtime(&t);
	printf("%04d-%02d-%02d_%02d-%02d-%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
}
