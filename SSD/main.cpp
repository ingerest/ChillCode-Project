#include "gmock/gmock.h"
#include "Ssd.h"
#include <iostream>
#define DEBUG_FLAG 1

int main()
{
#if DEBUG_FLAG == 0
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	std::string line;
	std::getline(std::cin, line);

	Ssd ssd;
	ssd.excuteCommand(line);

	return 0;
#endif
}