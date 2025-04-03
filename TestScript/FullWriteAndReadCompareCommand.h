#pragma once
#include "ITestCommand.h"
#include <vector>

class FullWriteAndReadCompareCommand : public ITestCommand {
public:
    void setShell(TestShell* shell) override;
    bool execute() override;

    bool writeAndReadRange(TestShell* shell, int start, int end, std::vector<std::string>& buffer);
    bool verifyRange(TestShell* shell, int start, int end, std::vector<std::string>& buffer);

private:
    TestShell* shell_ = nullptr;
};