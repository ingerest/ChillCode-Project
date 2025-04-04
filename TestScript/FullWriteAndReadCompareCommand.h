#pragma once
#include "ITestCommand.h"

class FullWriteAndReadCompareCommand : public ITestCommand {
public:
    void setShell(TestShell* shell) override;
    bool execute() override;

    bool writeRange(int start, int end, std::vector<std::string>& buffer);
    bool verifyRange(int start, int end, std::vector<std::string>& buffer);

private:
    TestShell* shell_ = nullptr;
};