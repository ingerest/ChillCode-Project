#pragma once
#include "ITestCommand.h"

class EraseAndWriteAging : public ITestCommand {
public:
    void setShell(TestShell* shell) override;
    bool execute() override;

    bool eraseRange(int startLba, int endLba);

private:
    TestShell* shell_ = nullptr;
};