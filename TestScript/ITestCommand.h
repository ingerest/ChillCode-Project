#pragma once
#include "TestUtils.h"

class ITestCommand {
public:
    virtual ~ITestCommand() = default;
    virtual void setShell(TestShell* shell) = 0;
    virtual bool execute() = 0;
};