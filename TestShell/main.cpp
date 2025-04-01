#include "gmock/gmock.h"
#include <iostream>
#include <string>

#include "Testshell.cpp"

using namespace testing;

#ifdef _DEBUG

// 테스트 환경 설정
class CommandTest : public Test {
protected:
    void executeTest(std::string input, std::string expect) {
        TestShell testShell;
        EXPECT_EQ(testShell.execute(input), expect);
    }
};

// TEST Case 1: "read" 명령어 처리
TEST_F(CommandTest, TestReadCommand) {
    executeTest("read 3", "3");
}

int main()
{
	InitGoogleMock();
	return RUN_ALL_TESTS();
}

#else
int main()
{

}
#endif