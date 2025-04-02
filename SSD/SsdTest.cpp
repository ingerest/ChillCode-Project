#include "gmock/gmock.h"
#include "ISsdApi.h"
#include "Ssd.h"
#include <string>

using namespace testing;

class MockSSD : public ISsdApi {
public:
    MOCK_METHOD(bool, excuteCommand, (std::string commandLine), (override));
    //MOCK_METHOD(void, read, (const std::string&, int, int), (override));
    //MOCK_METHOD(void, write, (const std::string&, int, int), (override));
};

class SsdFixture : public Test
{
public:
    MockSSD mock;
    Ssd ssd;

    void executeCommandTest(std::string command, bool returnValue)
    {
        EXPECT_CALL(mock, excuteCommand(command))
            .Times(1)
            .WillRepeatedly(Return(returnValue));

        mock.excuteCommand(command);

        bool result = ssd.excuteCommand(command);

        EXPECT_EQ(returnValue, result);
    }
};


TEST_F(SsdFixture, executeCommandSuccess)
{
    executeCommandTest("W 2 0xAAAABBBB", true);
}

TEST_F(SsdFixture, executeCommandInvalidCommand)
{
    executeCommandTest("D 2 0xAAAABBBB", false);
}

TEST_F(SsdFixture, executeCommandLBAOutOfRange)
{
    executeCommandTest("W 100 0xAAAABBBB", false);
}

TEST_F(SsdFixture, executeCommandLBAValueErrorInvalidValue1)
{
    executeCommandTest("W 2 0xAAAABB!!", false);
}

TEST_F(SsdFixture, executeCommandLBAValueErrorInvalidValue2)
{
    executeCommandTest("W 2 AxAAAABBBB", false);
}

TEST_F(SsdFixture, executeCommandLBAValueErrorByteOutOfRange)
{
    executeCommandTest("W 2 0xAAAABBAABBBB", false);
}