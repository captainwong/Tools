#include <jlib/jlib/util/rand.h>
#include <stdio.h>

using namespace jlib;

void usage(const char* argv0)
{
	printf("Usage: %s [password length] [use special chars]\n"
		   "  password length: default 16\n"
		   "  use special chars: default 0\n", argv0);
}

const char noSpecialChars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
const char withSpecialChars[] = R"(abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~!@#$%^&*()_+-=[]{};:\|,./<>?)";


int main(int argc, char** argv)
{
	int len = 16;
	bool useSpecialChars = false;

	if (argc > 1) {
		len = atoi(argv[1]);
		if (len < 1) { usage(argv[0]); return 0; }
	}
	if (argc > 2) {
		useSpecialChars = atoi(argv[1]) == 1;
	}

	const char* source = withSpecialChars;
	size_t source_size = sizeof(withSpecialChars) - 2;
	if (!useSpecialChars) {
		source = noSpecialChars;
		source_size = sizeof(noSpecialChars) - 2;
	}

	std::string pwd;
	auto rgn = seeded_random_engine();
	std::uniform_int_distribution<size_t> dist(0, source_size);
	for (int i = 0; i < len; i++) {
		pwd += source[dist(rgn)];
	}
	
	printf("%s\n", pwd.data());
	
	return 0;
}
