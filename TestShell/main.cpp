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
    //MOCK_METHOD(void, read, (const std::string&, int, int), (override));
    //MOCK_METHOD(void, write, (const std::string&, int, int), (override));
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

    void readMockTest(std::string input, std::string expect, std::string mockReadResult) {
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

// read ////////////////////////////////////
// TEST Case 00: "read 0" 명령어 처리
TEST_F(CommandTest, TestReadCommand00) {
    executeTest("read 0", "[Read] LBA 00 : 0x00000000");
}

// TEST Case 01: "read 1" 명령어 처리
TEST_F(CommandTest, TestReadCommand01) {
    executeTest("read 3", "[Read] LBA 03 : 0xAAAABBBB");
}

// TEST Case 02: "read 2" 명령어 처리 추가 
TEST_F(CommandTest, TestReadCommand02) {
    readMockTest("read 0", "[Read] LBA 00 : 0x00000000", "0x00000000" );
}

// TEST Case 03: "read 3" 명령어 처리 추가
TEST_F(CommandTest, TestReadCommand03) {
    readMockTest("read 3", "[Read] LBA 00 : 0xAAAABBBB", "0xAAAABBBB");
}

// read ////////////////////////////////////
// TEST Case 00: "write 3" 명령어 처리
TEST_F(CommandTest, TestWriteCommand00) {
    executeTest("write 3", "[Write] Done");
}

// exit ////////////////////////////////////
// TEST Case 00: "exit" 명령어 처리
TEST_F(CommandTest, TestExitCommand00) {
    executeTest("exit", "[exit] Done");
}

// help ////////////////////////////////////
// TEST Case 00: "help" 명령어 처리
TEST_F(CommandTest, TestHelpCommand00) {
    std::string helpResult = "Team Name : ChillCode\n Member : Oh, Seo, Kang, Lim";// 한글에 오류발생 ChillCode_팀원_오세훈_서병진_강은지_임태웅";
    executeTest("help", helpResult);
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