#include "gmock/gmock.h"
#include "Testshell.cpp"
#include "../SSD/ISsdApi.h"
#include "../SSD/Ssd.h"

#include <iostream>
#include <string>

using namespace testing;

#ifdef _DEBUG
class MockSSD : public ISsdApi {
public:
    MOCK_METHOD(bool, excuteCommand, (std::string commandLine), (override));
};

class MockTestShell :public TestShell {
public:
    MockTestShell(ISsdApi* pSsdApi) : TestShell(pSsdApi)
    {
    }
    MOCK_METHOD(string, readFile, (int lba), (override));

};

class CommandTest : public Test {
protected:
    void doNothingTest(std::string input) {
        testShellOnlyTest(input, "");
    }

    void testShellOnlyTest(std::string input, std::string expect) {
        MockSSD mock;

        TestShell testShell(&mock);
        EXPECT_EQ(testShell.execute(input), expect);
    }

    void executeTest(std::string input, std::string expect) {
        MockSSD mock;

        EXPECT_CALL(mock, excuteCommand(input))
            .Times(1)
            .WillRepeatedly(Return(true));

        TestShell testShell(&mock);
        EXPECT_EQ(testShell.execute(input), expect);
    }

    void readMockTest(std::string input, std::string expect, int lba, std::string mockReadResult) {
        MockSSD mock;

        EXPECT_CALL(mock, excuteCommand(input))
            .Times(1)
            .WillRepeatedly(Return(true));

        MockTestShell mockTestShell(&mock);

        EXPECT_CALL(mockTestShell, readFile(lba))
            .Times(1)
            .WillRepeatedly(Return(mockReadResult));

        EXPECT_EQ(mockTestShell.execute(input), expect);
    }

    void executeFullTest(std::string input, std::string expect) {
        MockSSD mock;

        EXPECT_CALL(mock, excuteCommand(input))
            .Times(100)
            .WillRepeatedly(Return(true));

        TestShell testShell(&mock);
        EXPECT_EQ(testShell.execute(input), expect);
    }
};

// 예외처리 ////////////////////////////////////
//- 없는 명령어를 수행하는 경우 "INVALID COMMAND" 을 출력
//    •어떠한 명령어를 입력하더라도 Runtime Error 가 나오면 안된다
TEST_F(CommandTest, TestInvalidCommand00) {
    testShellOnlyTest("jump", "INVALID COMMAND");
    testShellOnlyTest("sdg", "INVALID COMMAND");
    testShellOnlyTest("call 1", "INVALID COMMAND");
    testShellOnlyTest("fill 3 0xAAAABBBZ", "INVALID COMMAND");
}

//- 입력받은 매개변수가 유효성 검사 수행
//    • 파라미터의 Format 이 정확해야 함
//    • LBA 범위는 0 ~99
//    • A ~F, 0 ~9 까지 숫자 범위만 허용
TEST_F(CommandTest, TestDoNothing) {
    doNothingTest("read -1");
    doNothingTest("read 100");
    doNothingTest("read 1003");
    doNothingTest("write 3 0xAAAABBBZ");
    doNothingTest("write 3 1xAAAABBBK");
    doNothingTest("write 3 00AAAABBBC");
    doNothingTest("write -10 00AAAABBBC");
    doNothingTest("write 1004 00AAAABBBC");
    doNothingTest("write 100 00AAAABBBC");
}


// read ////////////////////////////////////
// TEST Case 00: "read 0" 명령어 처리
TEST_F(CommandTest, TestReadCommand00) {
    executeTest("read 0", "[Read] LBA 00 : 0x00000000");
}

// TEST Case 01: "read 1" 명령어 처리
TEST_F(CommandTest, TestReadCommand01) {
    executeTest("read 3", "[Read] LBA 03 : 0xAAAABBBB");
}

// TEST Case 1: "read 0" 명령어 처리 추가 
TEST_F(CommandTest, TestReadCommand03) {
    readMockTest("read 0", "[Read] LBA 00 : 0x00000000", 0, "0x00000000");
}

// TEST Case 1: "read 3" 명령어 처리 추가
TEST_F(CommandTest, TestReadCommand04) {
    readMockTest("read 3", "[Read] LBA 03 : 0xAAAABBBB", 3, "0xAAAABBBB");
}

// read ////////////////////////////////////
// TEST Case 00: "write 3" 명령어 처리
TEST_F(CommandTest, TestWriteCommand00) {
    executeTest("write 3 0x12345678", "[Write] Done");
}

// exit ////////////////////////////////////
// TEST Case 00: "exit" 명령어 처리
TEST_F(CommandTest, TestExitCommand00) {
    testShellOnlyTest("exit", "[exit] Done");
}

// help ////////////////////////////////////
// TEST Case 00: "help" 명령어 처리
TEST_F(CommandTest, TestHelpCommand00) {
    std::string helpResult = "Team Name : ChillCode\n Member : Oh, Seo, Kang, Lim";// 한글에 오류발생 ChillCode_팀원_오세훈_서병진_강은지_임태웅";
    testShellOnlyTest("help", helpResult);
}

// fullwrite ////////////////////////////////////
TEST_F(CommandTest, TestFullwrite00) {
    executeFullTest("fullwrite 0xABCDFFFF", "[Fullwrite] Done");

}

//// fullread ////////////////////////////////////
//TEST_F(CommandTest, TestFullread00) {
//    executeFullTest("fullread", "[Fullread] Done");
//}

int main()
{
    InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else
int main()
{
    ISsdApi* pSsd = new Ssd();
    TestShell testShell(pSsd);

    string userInput;

    while (1)
    {
        cin >> userInput;
        cout << testShell.execute(userInput);
    }
}
#endif