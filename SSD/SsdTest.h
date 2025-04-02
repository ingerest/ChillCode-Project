#pragma once
#include "gmock/gmock.h"
#include "ISsdApi.h"
#include "Ssd.h"
#include <string>

using namespace testing;

class SsdTest {
public:
    virtual bool excuteCommand(std::string commandLine) = 0;
    virtual std::string commandResultTest(std::string commandLine) = 0;
};

class SsdUtil {
public:
    std::vector<std::string>splitString(const std::string& str)
    {
        std::vector<std::string> result;
        std::istringstream iss(str);
        std::string word;

        while (iss >> word)
        {
            result.push_back(word);
        }

        return result;
    }

    std::string getWriteLBA(const std::string& findStr)
    {
        std::ifstream file(SSD_NAND_PATH);

        if (!file) {
            return "";
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.find(findStr) != std::string::npos) {
                return line;
            }
        }

        return "";
    }

    std::string getReadLBA()
    {
        std::ifstream file(SSD_OUTPUT_PATH);

        if (!file) {
            return "";
        }

        std::string line;
        if (std::getline(file, line)) {
            return line;
        }

        return "";
    }
private:
    const std::string SSD_NAND_PATH = "ssd_nand.txt";
    const std::string SSD_OUTPUT_PATH = "../Release/ssd_output.txt";
};

class MockSSD : public SsdTest {
public:
    MOCK_METHOD(bool, excuteCommand, (std::string commandLine), (override));
    MOCK_METHOD(std::string, commandResultTest, (std::string commandLine), (override));
};

class SsdFixture : public Test
{
public:
    MockSSD mock;
    SsdUtil util;
    Ssd ssd;

    void commandInvalidTest(std::string command, bool returnValue)
    {
        EXPECT_CALL(mock, excuteCommand(command))
            .Times(1)
            .WillRepeatedly(Return(returnValue));

        EXPECT_EQ(mock.excuteCommand(command), ssd.excuteCommand(command));
    }

    void writeResultTest(std::string command, std::string compareValue)
    {
        EXPECT_CALL(mock, commandResultTest(command))
            .Times(1)
            .WillRepeatedly(Return(compareValue));

        if (ssd.excuteCommand(command))
        {
            EXPECT_EQ(mock.commandResultTest(command), util.getWriteLBA(compareValue));
        }
        else
        {
            EXPECT_EQ(mock.commandResultTest(command), util.getReadLBA());
        }
    }

    void readResultTest(std::string command, std::string compareValue)
    {
        EXPECT_CALL(mock, commandResultTest(command))
            .Times(1)
            .WillRepeatedly(Return(compareValue));

        ssd.excuteCommand(command);

        EXPECT_EQ(mock.commandResultTest(command), util.getReadLBA());
    }
};