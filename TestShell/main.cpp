#include "gmock/gmock.h"
#include <iostream>
#include <string>

#include "Testshell.cpp"

using namespace testing;

#ifdef _DEBUG

// 테스트 환경 설정
class CommandTest : public Test {
protected:
    CommandTest() {
        mockSSD mockSsd;
        TestShell testShell(&mockSsd);
    }
};

// TEST Case 1: "read" 명령어 처리
TEST_F(CommandTest, TestReadCommand01) {
    //• 3번 LBA 를 읽는다.
    EXPECT_CALL(mockSsd, read(3))
        .Times(3);  // 3번 호출 확인

    testShell.execute("read 5");
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