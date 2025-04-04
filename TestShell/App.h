#pragma once
#include "TestShell.cpp"

class App {
public:
    App();
    int run(int argc, char* argv[]); // ��ü ������

private:
    TestShell shell_;

    int runRunnerMode(const std::string& filename);
    int runCommandMode();
    int callDllTestScript(std::string command);
};