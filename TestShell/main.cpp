#include "gmock/gmock.h"
#include "Testshell.cpp"
#include "../SSD/ISsdApi.h"
#include "../SSD/Ssd.h"

#include <iostream>
#include <string>

using namespace testing;

#ifdef _DEBUG
class MockTestShell : public TestShell {
public:
    MockTestShell()
    {
    }
    MOCK_METHOD(string, readFile, (int lba), (override));
    MOCK_METHOD(bool, executeSSD, (const string& command, const string& lba, const string& value), (override));
};

class CommandTest : public Test {
protected:
    void doNothingTest(std::string input) {
        testShellOnlyTest(input, "");
    }

    void testShellOnlyTest(std::string input, std::string expect) {
        TestShell testShell;
        EXPECT_EQ(testShell.execute(input), expect);
    }

    void executeTest(std::string input, std::string SsdCmd, std::string lba, std::string value, std::string expect) {
        MockTestShell mock;

        EXPECT_CALL(mock, executeSSD(SsdCmd, lba, value))
            .Times(1)
            .WillRepeatedly(Return(true));

        TestShell testShell;
        EXPECT_EQ(testShell.execute(input), expect);
    }

    void readMockTest(std::string input, std::string SsdCmd, std::string lbaString, std::string expect, int lba, std::string mockReadResult) {
        MockTestShell mock;

        EXPECT_CALL(mock, executeSSD(SsdCmd, lbaString, ""))
            .Times(1)
            .WillRepeatedly(Return(true));

        EXPECT_CALL(mock, readFile(lba))
            .Times(1)
            .WillRepeatedly(Return(mockReadResult));

        EXPECT_EQ(mock.execute(input), expect);
    }

    void executeFullTest(std::string input, std::string SsdCmd, std::string value, std::string expect) {
        MockTestShell mock;

        EXPECT_CALL(mock, executeSSD(SsdCmd, _, _))
            .Times(100)
            .WillRepeatedly(Return(true));

        EXPECT_EQ(mock.execute(input), expect);
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
// TEST Case 1: "read 0" 명령어 처리 추가 
TEST_F(CommandTest, TestReadCommand03) {
    readMockTest("read 0", "R", "0", "[Read] LBA 00 : 0x00000000", 0, "0x00000000");
}

// TEST Case 1: "read 3" 명령어 처리 추가
TEST_F(CommandTest, TestReadCommand04) {
    readMockTest("read 3", "R", "3", "[Read] LBA 03 : 0xAAAABBBB", 3, "0xAAAABBBB");
}

// read ////////////////////////////////////
// TEST Case 00: "write 3" 명령어 처리
TEST_F(CommandTest, TestWriteCommand00) {
    executeTest("write 3 0x12345678", "W", "3", "0x12345678", "[Write] Done");
}

// exit ////////////////////////////////////
// TEST Case 00: "exit" 명령어 처리
TEST_F(CommandTest, TestExitCommand00) {
    testShellOnlyTest("exit", "[exit] Done");
}

// help ////////////////////////////////////
// TEST Case 00: "help" 명령어 처리
TEST_F(CommandTest, TestHelpCommand00) {
    std::string helpResult = "Team Name : ChillCode\n Member : Oh, Seo, Kang, Lim\n";// 한글에 오류발생 ChillCode_팀원_오세훈_서병진_강은지_임태웅";
    testShellOnlyTest("help", helpResult);
}

// fullwrite ////////////////////////////////////
TEST_F(CommandTest, TestFullwrite00) {
     executeFullTest("fullwrite 0xABCDFFFF", "W", "0xABCDFFFF", "[Fullwrite] Done");
}

//// fullread ////////////////////////////////////
TEST(TG_FULL, TestFullread00) {
    //////////////////////////////////////////////////
    std::streambuf* original_buf; // 원래의 cout 버퍼
    std::ostringstream buf; // 출력 내용을 저장할 버퍼

    // 기존의 cout의 버퍼를 저장
    original_buf = std::cout.rdbuf();
    std::cout.rdbuf(buf.rdbuf());
    //////////////////////////////////////////////////

    std::string expected = "0x00000000";

    TestShell testShell;
    testShell.execute("fullread");

    std::string str = buf.str();
    // std::istringstream를 사용하여 문자열을 줄바꿈을 기준으로 처리
    std::istringstream stream(str);
    std::string line;

    bool same = true;
    int cnt = 0;
    // 한 줄씩 읽어들여 처리
    while (std::getline(stream, line)) {
        // 각 줄을 처리
        if (line != expected) {
            same = false;
            break;
        }
        cnt++;
    }

    EXPECT_EQ(same, true);  // 모든 값이 초기 값인지?
    EXPECT_EQ(cnt, 100);    // 100개 모두 읽었는지?

    //////////////////////////////////////////////////
    // 출력 리디렉션을 원래대로 복원
    std::cout.rdbuf(original_buf);
    //////////////////////////////////////////////////
}

int main()
{
    InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else
int main()
{
    TestShell testShell;
    string userInput;

    while (1)
    {
        cin >> userInput;

        string output = testShell.execute(userInput);
        cout << output;

        if (output == "INVALID_COMMAND" || output == "[exit] Done")
        {
            break;
        }
    }
}
#endif