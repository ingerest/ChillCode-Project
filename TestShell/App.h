#pragma once
#include "TestShell.cpp"

class App {
public:
    App();
    int run(int argc, char* argv[]); // 전체 진입점

private:
    TestShell shell_;

    int runRunnerMode(const std::string& filename);
    int runCommandMode();
    int callDllTestScript(std::string command);
};