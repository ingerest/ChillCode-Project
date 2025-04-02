#include "gmock/gmock.h"
#include "Testshell.cpp"
#include "../SSD/ISsdApi.h"

#include <iostream>
#include <string>

using namespace testing;

#ifdef _DEBUG

class MockSSD : public ISsdApi {
public:
    MOCK_METHOD(bool, excuteCommand, (std::string commandLine), (override));
    MOCK_METHOD(void, read, (const std::string&, int, int), (override));
    MOCK_METHOD(void, write, (const std::string&, int, int), (override));
};

class MockTestShell :public TestShell {
public:
    MockTestShell(ISsdApi* pSsdApi) : TestShell(pSsdApi)
    {
    }
    MOCK_METHOD(string, readFile, (), ());

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

    void executeMockTest(std::string input, std::string expect, std::string mockReadResult) {
        MockSSD mock;

        EXPECT_CALL(mock, excuteCommand(input))
            .Times(1)
            .WillRepeatedly(Return(true));

        MockTestShell mockTestShell(&mock);

        EXPECT_CALL(mockTestShell, readFile())
            .Times(1)
            .WillRepeatedly(Return(mockReadResult));

        EXPECT_EQ(mockTestShell.execute(input), expect);
    }
};

// TEST Case 1: "read 0" 명령어 처리
TEST_F(CommandTest, TestReadCommand00) {
    executeTest("read 0", "[Read] LBA 00 : 0x00000000");
}

// TEST Case 1: "read 3" 명령어 처리
TEST_F(CommandTest, TestReadCommand01) {
    executeTest("read 3", "[Read] LBA 00 : 0xAAAABBBB");
}

// TEST Case 1: "write 3" 명령어 처리
TEST_F(CommandTest, TestReadCommand02) {
    executeTest("write 3", "[Write] Done");
}

// TEST Case 1: "read 0" 명령어 처리 추가 
TEST_F(CommandTest, TestReadCommand03) {
    executeMockTest("read 0", "[Read] LBA 00 : 0x00000000", "0x00000000" );
}

// TEST Case 1: "read 3" 명령어 처리 추가
TEST_F(CommandTest, TestReadCommand04) {
    executeMockTest("read 3", "[Read] LBA 00 : 0xAAAABBBB", "0xAAAABBBB");
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