#pragma once
#include "ITestCommand.h"

class PartialLBAWrite : public ITestCommand {
public:
    void setShell(TestShell* shell) override;
    bool execute() override;

    bool readRange(const std::vector<int>& lbas, std::string& value);
    bool writeRange(const std::vector<int>& lbas, std::string& value);

private:
    TestShell* shell_ = nullptr;
};