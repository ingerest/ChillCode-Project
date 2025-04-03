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
    MOCK_METHOD(string, readFile, (), (override));
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

        EXPECT_EQ(mock.execute(input), expect);
    }
    void executeTest(std::string input, std::string value, std::string expect) {
        std::string SsdCmd = getSsdCmd(input);
        if (SsdCmd != "R" && SsdCmd != "W") return; // EXPECT 처리 필요
        std::string lba = getLba(input);
        if (lba == "") return;                // EXPECT 처리 필요

        executeTest(input, SsdCmd, lba, value, expect);
    }

    void readMockTest(std::string input, std::string SsdCmd, std::string lbaString, std::string expect, int lba, std::string mockReadResult) {
        MockTestShell mock;

        EXPECT_CALL(mock, executeSSD(SsdCmd, lbaString, ""))
            .Times(1)
            .WillRepeatedly(Return(true));

        EXPECT_CALL(mock, readFile())
            .Times(1)
            .WillRepeatedly(Return(mockReadResult));

        EXPECT_EQ(mock.execute(input), expect);
    }
    void readMockTest(std::string input, std::string expect, std::string mockReadResult) {
        std::string SsdCmd = getSsdCmd(input);
        if (SsdCmd != "R" && SsdCmd != "W") return; // EXPECT 처리 필요
        std::string lbaString = getLba(input);
        if (lbaString == "") return;                // EXPECT 처리 필요

        readMockTest(input, SsdCmd, lbaString, expect, stoi(lbaString), mockReadResult);
    }

    void executeFullwriteTest(std::string input, std::string SsdCmd, std::string value, std::string expect) {
        MockTestShell mock;

        EXPECT_CALL(mock, executeSSD(SsdCmd, _, _))
            .Times(100)
            .WillRepeatedly(Return(true));

        EXPECT_EQ(mock.execute(input), expect);
    }
    void executeFullwriteTest(std::string input, std::string value, std::string expect) {
        std::string SsdCmd = getSsdCmd(input);
        if (SsdCmd != "W") return; // EXPECT 처리 필요

        executeFullwriteTest(input, SsdCmd, value, expect);
    }


private:
    std::string getSsdCmd(std::string input) {
        size_t pos = input.find(' ');                       // 첫 번째 공백을 찾기
        if (pos == std::string::npos) pos = input.length(); // 공백이 없으면 마지막 위치

        input = input.substr(0, pos);                       // 첫 단어 추출

        if (input == "read" || input == "fullread") return "R";
        if (input == "write" || input == "fullwrite") return "W";
        return "";
    }

    std::string getLba(std::string input) {
        size_t pos = input.find(' ');               // 첫 번째 공백을 찾기
        if (pos == std::string::npos) return "";    // 공백이 없으면 return

        input = input.substr(pos + 1, input.length());

        pos = input.find(' ');                              // 두 번째 공백을 찾기
        if (pos == std::string::npos) pos = input.length(); // 공백이 없으면 마지막 위치

        return input.substr(0, pos);
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
    readMockTest("read 0", "[Read] LBA 00 : 0x00000000", "0x00000000");
}

// TEST Case 1: "read 3" 명령어 처리 추가
TEST_F(CommandTest, TestReadCommand04) {
    readMockTest("read 3", "[Read] LBA 03 : 0xAAAABBBB", "0xAAAABBBB");
}

// read ////////////////////////////////////
// TEST Case 00: "write 3" 명령어 처리
TEST_F(CommandTest, TestWriteCommand00) {
    executeTest("write 3 0x12345678", "0x12345678", "[Write] Done");
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
    executeFullwriteTest("fullwrite 0xABCDFFFF", "W", "0xABCDFFFF", "[Fullwrite] Done");
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
    std::string str = testShell.execute("fullread");
    std::istringstream stream(str);
    std::string line;

    bool same = true;
    int cnt = 0;
    // 한 줄씩 읽어들여 처리
    while (std::getline(stream, line)) {
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << cnt;
        string lba = oss.str();


        std::string expected = "[Read] LBA " + lba + " : 0x00000000";

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

    while (1)
    {
        std::string input;

        std::cout << "\n>>";
        std::getline(std::cin, input);

        string output = testShell.execute(input);
        cout << output;

        if (output == "[exit] Done")
        {
            break;
        }
    }
}
#endif