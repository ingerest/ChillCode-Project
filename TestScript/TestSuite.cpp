#include "../TestShell/TestShell.cpp"
#include "TestCommandFactory.h"

extern "C" __declspec(dllexport)
int runTest(const char* command, TestShell* shell) {

    if (command == nullptr || shell == nullptr)
    {
        return -1;
    }

    const std::string commandLine(command);

    std::cout << "[Command] " << commandLine << std::endl;

    auto test = TestCommandFactory::create(commandLine, shell);
    if (!test) {
        std::cerr << "[Error] Unknown command: " << commandLine << std::endl;
        return 0;
    }

    return test->execute();
}