#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <shellapi.h>
#include <io.h>
#include <filesystem>
#include <random>
#include <algorithm>
#include "TestShell.h"
#include "Logger.cpp"
#include "TestScript.cpp"


using namespace std;

TestShell::TestShell() : m_testScript(new TestScript(this)) {}

string TestShell::execute(const string& userInput) {
        try {
            string output = processCommand(userInput);
            return output;
        }
        catch (const exception& e) {
            return e.what();
        }
    }

void TestShell::executeSSD(const string& command, const string& lba, const string& value) {
        Logger::getInstance().log("executeSSD", "Application started");
        string fullCommand = constructCommand(command, lba, value);

        string exePath = "../Release";
        SetCurrentDirectory(exePath.c_str());

        if (!runCommandAndWait("ssd.exe", fullCommand)) {
            Logger::getInstance().log("executeSSD", "runCommandAndWait Failed.");
            throw invalid_argument("");
        }
        Logger::getInstance().log("executeSSD", "Command executed successfully: " + fullCommand);
    }

    string TestShell::readFile() {
        return readFileContent("../Release/ssd_output.txt");
    }

    string TestShell::processCommand(const string& userInput) {
        Logger::getInstance().log("processCommand", "Processing command: " + userInput);
        string command;
        istringstream stream(userInput);
        stream >> command;

        return executeCommand(command, stream);
    }

    string TestShell::executeCommand(const string& command, istringstream& stream) {
        Logger::getInstance().log("executeCommand", "Executing command: " + command);

        if (command == "read") return handleReadCommand(stream);
        if (command == "write") return handleWriteCommand(stream);
        if (command == "exit") return "[exit] Done";
        if (command == "help") return getHelpInfo();
        if (command == "fullwrite") return handleFullWriteCommand(stream);
        if (command == "fullread") return handleFullReadCommand();
        if (command == "erase") return handleEraseCommand(stream);
        if (command == "erase_range") return handleEraseRangeCommand(stream);
        if (command == "flush") return handleFlushCommand();
        if (m_testScript->isTestScriptCommand(command)) return m_testScript->handleTestScriptCommand(command);

        // Need Double Check
        int result = handleRunnerCommand(command);
        if (result == -1)
        {
            Logger::getInstance().log("executeCommand", "INVALID COMMAND: " + command);
            throw invalid_argument("INVALID COMMAND");
        }

        return "";
    }

    string TestShell::constructCommand(const string& command, const string& lba, const string& value) {
        string fullCommand = command;
        if (command != "F") fullCommand += " " + lba;
        if (command == "W" || command == "E") fullCommand += " " + value;
        return fullCommand;
    }

    string TestShell::handleWriteCommand(istringstream& stream) {
        string lbaString, writeValue;
        stream >> lbaString >> writeValue;

        Logger::getInstance().log("handleWriteCommand", "Writing value: " + writeValue + " at LBA: " + lbaString);

        validateLBA(lbaString);
        validateValue(writeValue);

        executeSSD("W", lbaString, writeValue);
        return verifyExecution("[Write] Done", "[Write] Error");
    }

    string TestShell::handleReadCommand(istringstream& stream) {
        string lbaString;
        stream >> lbaString;

        Logger::getInstance().log("handleReadCommand", "Reading from LBA: " + lbaString);

        validateLBA(lbaString);

        executeSSD("R", lbaString, "");
        string readOutput = readFile();

        return (readOutput == "ERROR") ? "[Read] Error" : formatReadResult(lbaString, readOutput);
    }


    string TestShell::handleEraseCommand(istringstream& stream) {
        string startLba;
        stream >> startLba;

        Logger::getInstance().log("handleEraseCommand", "Starting erase command for LBA: " + startLba);

        validateLBA(startLba);
        return performEraseCommand(stream, startLba);
    }

    string TestShell::handleEraseRangeCommand(istringstream& stream) {
        string startLba, endLba;
        stream >> startLba >> endLba;

        Logger::getInstance().log("handleEraseRangeCommand", "Starting erase range command: " + startLba + " to " + endLba);

        validateLBA(startLba);
        validateLBA(endLba);
        return performEraseRangeCommand(startLba, endLba);
    }

    static const int START_LBA = 0;
    static const int END_LBA = 99;
    static const int UNMAP_CHUNK_SIZE = 10;

    string TestShell::readFileContent(const string& filePath) {
        Logger::getInstance().log("readFileContent", "Reading file: " + filePath);

        ifstream file(filePath);
        string line, value;
        if (file.is_open()) {
            while (getline(file, line)) value = line.c_str();
            file.close();

            Logger::getInstance().log("readFileContent", "File read successfully.");
        }
        else
        {
            Logger::getInstance().log("readFileContent", "Failed to open file.");
        }
        return value;
    }

    string TestShell::handleFlushCommand() {
        Logger::getInstance().log("handleFlushCommand", "Executing Flush command");

        executeSSD("F", "", "");
        return verifyExecution("[Flush] Done", "[Flush] Error");
    }

    string TestShell::getHelpInfo() const {
        Logger::getInstance().log("getHelpInfo", "Providing help information.");
        return "Team Name : ChillCode\n Member : Oh, Seo, Kang, Lim\n";
    }

    string TestShell::handleFullWriteCommand(istringstream& stream) {
        string writeValue;
        stream >> writeValue;

        Logger::getInstance().log("handleFullWriteCommand", "Full Write Command with value: " + writeValue);

        validateValue(writeValue);

        for (int lba = START_LBA; lba <= END_LBA; ++lba) {
            executeSSD("W", to_string(lba), writeValue);
            string readOutput = readFile();
            if (readOutput == "ERROR") {
                return "[Fullwrite] Error";
            }
        }

        return "[Fullwrite] Done";
    }

    string TestShell::handleFullReadCommand() {
        Logger::getInstance().log("handleFullReadCommand", "Executing Full Read Command");

        string result;
        for (int lba = START_LBA; lba <= END_LBA; ++lba) {
            result += handleReadCommandForLba(lba) + "\n";
        }

        return result;
    }

    string TestShell::handleReadCommandForLba(int lba) {
        std::istringstream lbaStream(std::to_string(lba));
        return handleReadCommand(lbaStream);
    }

    string TestShell::verifyExecution(const string& successMessage, const string& errorMessage) {
        string value = readFile();
        if (value == "ERROR") {
            Logger::getInstance().log("verifyExecution", errorMessage);
            return errorMessage;
        }
        Logger::getInstance().log("verifyExecution", successMessage);
        return successMessage;
    }

    string TestShell::performEraseCommand(istringstream& stream, const string& startLba) {
        string sizeInString;
        stream >> sizeInString;

        Logger::getInstance().log("performEraseCommand", "Performing erase command with start LBA: " + startLba + ", size: " + sizeInString);

        int size = stoi(sizeInString);
        int lba = stoi(startLba);

        return executeEraseByChunks(lba, size, END_LBA, UNMAP_CHUNK_SIZE);
    }

    string TestShell::executeEraseByChunks(int& lba, int& size, int maxLba, int chunkSize) {
        Logger::getInstance().log("executeEraseByChunks", "Executing erase in chunks.");

        while (size > 0 && lba <= maxLba) {
            int currentChunkSize;

            if (size < chunkSize) {
                currentChunkSize = size;
            }
            else {
                currentChunkSize = chunkSize;
            }

            if (lba + currentChunkSize - 1 > maxLba) {
                currentChunkSize = maxLba - lba + 1;
            }

            Logger::getInstance().log("executeEraseByChunks", "Erasing from LBA: " + to_string(lba) + ", size: " + to_string(currentChunkSize));

            executeSSD("E", to_string(lba), to_string(currentChunkSize));
            string value = readFile();
            if (value == "ERROR") {
                Logger::getInstance().log("executeEraseByChunks", "Erase command failed.");
                return "[Erase] Error";
            }

            lba += currentChunkSize;
            size -= currentChunkSize;
        }

        Logger::getInstance().log("executeEraseByChunks", "Erase command completed successfully.");
        return "[Erase] Done";
    }

    string TestShell::performEraseRangeCommand(const string& startLba, const string& endLba) {
        Logger::getInstance().log("performEraseRangeCommand", "Performing erase range from LBA: " + startLba + " to " + endLba);

        int size = stoi(endLba) - stoi(startLba) + 1;
        std::istringstream eraseStream(startLba + " " + to_string(size));
        return handleEraseCommand(eraseStream);
    }

    string TestShell::formatReadResult(const string& lba, const string& value) const {
        std::ostringstream oss;
        oss << "[Read] LBA "
            << std::setw(2) << std::setfill('0') << lba
            << " : " << value;
        return oss.str();
    }

    bool TestShell::runCommandAndWait(const string& exe, const string& args) {
        Logger::getInstance().log("runCommandAndWait", "Running command: " + exe + " " + args);
        string fullPath = exe + " " + args;

        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;

        if (!CreateProcessA(
            NULL,
            const_cast<char*>(fullPath.c_str()),
            NULL, NULL, FALSE,
            0, NULL, NULL,
            &si, &pi)) {
            Logger::getInstance().log("runCommandAndWait", "Failed to create process for: " + fullPath);
            return false;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        Logger::getInstance().log("runCommandAndWait", "Process completed successfully.");
        return true;
    }

    void TestShell::validateLBA(std::string lba) {
        int num = std::stoi(lba);
        if (num < START_LBA || num > END_LBA)
        {
            throw invalid_argument("");
        }
    }

    void TestShell::validateValue(const std::string& value) {
        if (value.length() != 10 || value.substr(0, 2) != "0x") {
            throw invalid_argument("");
        }

        for (size_t i = 2; i < value.length(); ++i) {
            char c = value[i];
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
                throw invalid_argument("");
            }
        }

        unsigned int hexValue = std::stoul(value, nullptr, 16);
        if (hexValue > 0xFFFFFFFF)
        {
            throw invalid_argument("");
        }
    }

    int TestShell::handleRunnerCommand(string command) {
        HMODULE hModule = LoadLibraryA("TestScript.dll");
        if (!hModule) {
            //std::cerr << "DLL 로딩 실패" << std::endl;  // ToDo : Loger로 처리 필요
            return -1;
        }

        RunTestFunc runTest = (RunTestFunc)GetProcAddress(hModule, "runTest");
        if (!runTest) {
            //std::cerr << "함수 찾기 실패" << std::endl;   // ToDo : Loger로 처리 필요
            FreeLibrary(hModule);
            return -1;
        }

        int result = runTest(command.c_str(), this);

        if (result == -1) return -1;    // "INVALID COMMAND"
        string Result = (result == 1) ? "PASS" : "FAIL";
        std::cout << Result;

        FreeLibrary(hModule);
        return 0;

    }