#include "gmock/gmock.h"

using namespace testing;

#ifdef _DEBUG
int main()
{
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}

#else
int main()
{

}
#endif