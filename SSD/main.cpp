#include "gmock/gmock.h"

#define DEBUF_FLAG 0

int main()
{
#if DEBUG_FLAG == 0
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}