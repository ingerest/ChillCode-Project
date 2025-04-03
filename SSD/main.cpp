#include "gmock/gmock.h"
#include "Ssd.h"
#include <iostream>
#define DEBUG_FLAG 1

//int main(int argc, char** argv)
int main()
{
#if DEBUG_FLAG == 0
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
#if 0 
	std::string commandLine = "";
	if (argc < 2) return 1;

	for (int i = 1; i < argc; ++i) {
		commandLine += argv[i];
		if (i < argc - 1) commandLine += " ";
	}
#endif
	std::string line;
	std::getline(std::cin, line);

	Ssd ssd;
	ssd.excuteCommand(line);

	return 0;
#endif
}