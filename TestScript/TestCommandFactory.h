#pragma once
#include "ITestCommand.h"

class TestCommandFactory {
public:
    static std::unique_ptr<ITestCommand> create(const std::string& commandName, TestShell* shell);
private:
    static std::string extractCommandKey(const std::string& commandName);
};