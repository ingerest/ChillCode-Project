#include "gmock/gmock.h"
#include <iostream>
#include <string>

using namespace testing;

#ifdef _DEBUG

// 명령어를 처리하는 클래스 정의
class CommandHandler {
public:
    virtual ~CommandHandler() = default;
    virtual std::string execute(const std::string& command) = 0;
};

// CommandHandler의 mock 클래스 정의
class MockCommandHandler : public CommandHandler {
public:
    MOCK_METHOD(std::string, execute, (const std::string& command), (override));
};

// 테스트 환경 설정
class CommandTest : public ::Test {
protected:
    MockCommandHandler mockCommandHandler;
};

// TEST Case 1: "read" 명령어 처리
TEST_F(CommandTest, TestReadCommand) {
    EXPECT_CALL(mockCommandHandler, execute("read 3"))
        .WillOnce(testing::Return("3"));

    std::string result = mockCommandHandler.execute("read 3");
    EXPECT_EQ(result, "3");
}

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