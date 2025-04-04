#pragma once
#include "ITestCommand.h"

class WriteReadAging : public ITestCommand {
public:
    void setShell(TestShell* shell) override;
    bool execute() override;

private:
    TestShell* shell_ = nullptr;
};