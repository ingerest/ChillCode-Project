#include "App.h"
#include <iostream>

App::App() {}

int App::run(int argc, char* argv[]) {

    if (argc >= 2) {
        std::string arg = argv[1];

        if (arg.ends_with(".txt") == false) {
            Logger::getInstance().log("App::run", "Not Text File !!!");
            return -1;
        }

        return runRunnerMode(arg);
    }

    Logger::getInstance().log("App::run", "Command Mode Start");
    return runCommandMode();
}

int App::runRunnerMode(const string& filename) {
    ifstream file(filename);
    if (file.is_open() == false) {
        Logger::getInstance().log("App::run", "File Open Fail !!!");
        return -1;
    }

    string command;
    int result;
    string resultString;
    while (getline(file, command)) {
        if (command.empty()) continue;
        cout << command << "\t___\tRun ...";
        result = callDllTestScript(command);
        resultString = (result == 1) ? "PASS" : "FAIL!";
        cout << resultString << endl;
    }

    return 0;
}

int App::runCommandMode() {

    Logger::getInstance().log("App::run", "Command Mode Start.");

    while (true) {
        cout << "\n>>";
        string input;
        getline(cin, input);

        string output = shell_.execute(input);
        cout << output << endl;

        if (output == "[exit] Done")
            break;
    }

    return 0;
}

typedef int (*RunTestFunc)(const char*, TestShell*);

int App::callDllTestScript(std::string command)
{
    HMODULE hModule = LoadLibraryA("TestScript.dll");
    if (!hModule) {
        Logger::getInstance().log("DllInvoker::invoke", "DLL Load Fail!!!");
        return -1;
    }

    RunTestFunc runTest = (RunTestFunc)GetProcAddress(hModule, "runTest");
    if (!runTest) {
        Logger::getInstance().log("DllInvoker::invoke", "runTest Function Not Found!!!");
        FreeLibrary(hModule);
        return -1;
    }
    TestShell shellTemp;
    int result = runTest(command.c_str(), &shellTemp);

    FreeLibrary(hModule);
    return result;
}