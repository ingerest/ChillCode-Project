#include "gmock/gmock.h"
#include "Ssd.h"
#include <iostream>
#define DEBUG_FLAG 0

int main(int argc, char** argv)
{
#if DEBUG_FLAG == 0
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	std::string commandLine = "";
	if (argc < 2) return 1;

	for (int i = 1; i < argc; ++i) {
		commandLine += argv[i];
		if (i < argc - 1) commandLine += " ";
	}

	Ssd ssd;
	ssd.excuteCommand(commandLine);

	return 0;
#endif
}