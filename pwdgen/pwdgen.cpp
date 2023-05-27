#include <jlib/jlib/util/rand.h>
#include <stdio.h>

using namespace jlib;

void usage(const char* argv0)
{
	printf("Usage: %s [password length] [use special chars]\n"
		   "  password length: default 16\n"
		   "  use special chars: default 0\n", argv0);
}

const char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
const char specialChars[] = R"(~!@#$%^&*()_+-=[]{};:\|,./<>?)";


int main(int argc, char** argv)
{
	int len = 16;
	bool useSpecialChars = false;

	if (argc > 1) {
		len = atoi(argv[1]);
		if (len < 6) { usage(argv[0]); return 0; }
	}
	if (argc > 2) {
		useSpecialChars = atoi(argv[2]) == 1;
	}

	const char* source = alphabet;
	size_t source_size = sizeof(alphabet) - 2;
	/*if (!useSpecialChars) {
		source = noSpecialChars;
		source_size = sizeof(noSpecialChars) - 2;
	}*/

	std::string pwd;
	auto rgn = seeded_random_engine();
	std::uniform_int_distribution<size_t> dist(0, source_size);
	for (int i = 0; i < len; i++) {
		pwd += source[dist(rgn)];
	}

	if (useSpecialChars) {
		int slen = len / 8;
		if (slen == 0)
			slen = len / 4;
		if (slen == 0)
			slen = len / 2;
		if (slen == 0)
			slen = 1;

		size_t s_source_size = sizeof(specialChars) - 2;
		std::uniform_int_distribution<size_t> s_dist(0, s_source_size);
		std::uniform_int_distribution<size_t> p_dist(0, len);

		for (int i = 0; i < slen; i++) {
			pwd[p_dist(rgn)] = specialChars[s_dist(rgn)];
		}
	}
	
	printf("%s\n", pwd.data());
	
	return 0;
}
