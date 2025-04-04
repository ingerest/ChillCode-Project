#include "../TestShell/TestShell.cpp"
#include "TestCommandFactory.h"
#include <chrono>

extern "C" __declspec(dllexport) int runTest(const char* command, TestShell* shell);
int runTest(const char* command, TestShell* shell) {

    if (command == nullptr || shell == nullptr)
    {
        return -1;
    }

    const std::string commandLine(command);

    //std::cout << "[Command] " << commandLine << std::endl;  // 화면 표시 삭제

    auto test = TestCommandFactory::create(commandLine, shell);
    if (!test) {
        //std::cerr << "[Error] Unknown command: " << commandLine << std::endl;// 화면 표시 삭제
        return -1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    bool result = test->execute();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    //std::cout << "Running Time: " << duration.count() << " s" << std::endl;// 화면 표시 삭제

    return result;
}