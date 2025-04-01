#include "gmock/gmock.h"
#include <iostream>
#include <string>

#include "Testshell.cpp"

using namespace testing;

#ifdef _DEBUG

class MockSSD : public ISsdApi {
public:
    MOCK_METHOD(bool, excuteCommand, (std::string commandLine), (override));
    //MOCK_METHOD(void, read, (const std::string&, int, int), (override));
    //MOCK_METHOD(void, write, (const std::string&, int, int), (override));
};

class CommandTest : public Test {
protected:
    void executeTest(std::string input, std::string expect) {
        MockSSD mock;

        EXPECT_CALL(mock, excuteCommand(input))
            .Times(1)
            .WillRepeatedly(Return(true));

        TestShell testShell(&mock);
        EXPECT_EQ(testShell.execute(input), expect);
    }
};

// TEST Case 1: "read" 명령어 처리
{
// TEST Case 1: "read" ��ɾ� ó��
TEST_F(CommandTest, TestReadCommand) {
    executeTest("read 3", "3");
}

//TEST_F(CommandTest, TestReadCommand02) {
//    //• 제작한ssd 프로그램을 실행시켜동작시킨다.
//
//}
//
//TEST_F(CommandTest, TestReadCommand03) {
//    //• 읽은결과를화면에출력한다.
//    //• 출력형태는자유롭게결정한다.
//
//}

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