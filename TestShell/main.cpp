#include "gmock/gmock.h"
#include <iostream>
#include <string>

using namespace testing;

#ifdef _DEBUG

// ��ɾ ó���ϴ� Ŭ���� ����
class CommandHandler {
public:
    virtual ~CommandHandler() = default;
    virtual std::string execute(const std::string& command) = 0;
};

// CommandHandler�� mock Ŭ���� ����
class MockCommandHandler : public CommandHandler {
public:
    MOCK_METHOD(std::string, execute, (const std::string& command), (override));
};

// �׽�Ʈ ȯ�� ����
class CommandTest : public ::Test {
protected:
    MockCommandHandler mockCommandHandler;
};

// TEST Case 1: "read" ��ɾ� ó��
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