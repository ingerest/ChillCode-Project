#include "gmock/gmock.h"
#include "ISsdApi.cpp"
#include <string>

using namespace testing;

class MockSSD : public ISsdApi {
public:
    MOCK_METHOD(bool, excuteCommand, (std::string commandLine), (override));
    //MOCK_METHOD(void, read, (const std::string&, int, int), (override));
    //MOCK_METHOD(void, write, (const std::string&, int, int), (override));
};


TEST(SSDTest, executeCommandSuccess)
{
    MockSSD mock;

    std::string command = "W 2 0xAAAABBBB";

    EXPECT_CALL(mock, excuteCommand(command))
        .Times(1)
        .WillRepeatedly(Return(true));

    mock.excuteCommand(command);
}

TEST(SSDTest, executeCommandInvalidCommand)
{
    MockSSD mock;

    std::string command = "D 2 0xAAAABBBB";

    EXPECT_CALL(mock, excuteCommand(command))
        .Times(1)
        .WillRepeatedly(Return(false));

    mock.excuteCommand(command);
}

TEST(SSDTest, executeCommandLBAOutOfRange)
{
    MockSSD mock;

    std::string command = "W 100 0xAAAABBBB";

    EXPECT_CALL(mock, excuteCommand(command))
        .Times(1)
        .WillRepeatedly(Return(false));

    mock.excuteCommand(command);
}

TEST(SSDTest, executeCommandLBAValueErrorInvalidValue1)
{
    MockSSD mock;

    std::string command = "W 100 0xAAAABB!!";

    EXPECT_CALL(mock, excuteCommand(command))
        .Times(1)
        .WillRepeatedly(Return(false));

    mock.excuteCommand(command);
}

TEST(SSDTest, executeCommandLBAValueErrorInvalidValue2)
{
    MockSSD mock;

    std::string command = "W 100 AxAAAABBBB";

    EXPECT_CALL(mock, excuteCommand(command))
        .Times(1)
        .WillRepeatedly(Return(false));

    mock.excuteCommand(command);
}

TEST(SSDTest, executeCommandLBAValueErrorByteOutOfRange)
{
    MockSSD mock;

    std::string command = "W 100 0xAAAABBAABBBB";

    EXPECT_CALL(mock, excuteCommand(command))
        .Times(1)
        .WillRepeatedly(Return(false));

    mock.excuteCommand(command);
}