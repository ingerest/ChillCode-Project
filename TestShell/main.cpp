#include "gmock/gmock.h"
#include <iostream>
#include <string>

#include "Testshell.cpp"

using namespace testing;

#ifdef _DEBUG

// �׽�Ʈ ȯ�� ����
class CommandTest : public Test {
protected:
    void executeTest(std::string input, std::string expect) {
        TestShell testShell;
        EXPECT_EQ(testShell.execute(input), expect);
    }
};

// TEST Case 1: "read" ��ɾ� ó��
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