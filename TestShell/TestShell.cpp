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

#include "Logger.cpp"

using namespace std;

class TestShell {
public:
    typedef int (*RunTestFunc)(const char*, TestShell*);

    string execute(const string& userInput) {
        try {
            string output = processCommand(userInput);
            return output;
        }
        catch (const exception& e) {
            return e.what();
        }
    }

    virtual void executeSSD(const string& command, const string& lba, const string& value) {
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

    virtual string readFile() {
        return readFileContent("../Release/ssd_output.txt");
    }

private:
    static const int START_LBA = 99;
    static const int END_LBA = 99;
    static const int UNMAP_CHUNK_SIZE = 10;
    static const int INPUT_ARRAY_SIZE = 100;
    static const int PARTIAL_LBA_LIMIT = 30;
    static const int COMPARE_BATCH_SIZE = 5;

    string processCommand(const string& userInput) {
        Logger::getInstance().log("processCommand", "Processing command: " + userInput);
        string command;
        istringstream stream(userInput);
        stream >> command;

        return executeCommand(command, stream);
    }

    string executeCommand(const string& command, istringstream& stream) {
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
        if (isTestScriptCommand(command)) return handleTestScriptCommand(command);

        // Need Double Check
        int result = handleRunnerCommand(command);
        if (result == -1)
        {
            Logger::getInstance().log("executeCommand", "INVALID COMMAND: " + command);
            throw invalid_argument("INVALID COMMAND");
        }

        return "";
    }

    bool isTestScriptCommand(const string& command) {
        return command == "1_FullWriteAndReadCompare" || command == "1_" ||
            command == "2_PartialLBAWrite" || command == "2_" ||
            command == "3_WriteReadAging" || command == "3_" ||
            command == "4_EraseAndWriteAging" || command == "4_";
    }

    string handleTestScriptCommand(const string& command) {
        Logger::getInstance().log("handleTestScriptCommand", "Handling test script command: " + command);

        if (command == "1_FullWriteAndReadCompare" || command == "1_")
            return handle1_FullWriteAndReadCompareCommand();
        if (command == "2_PartialLBAWrite" || command == "2_")
            return handle2_PartialLBACommand();
        if (command == "3_WriteReadAging" || command == "3_")
            return handle3_WriteReadAgingCommand();
        if (command == "4_EraseAndWriteAging" || command == "4_")
            return handle4_EraseAndWriteAgingCommand();

        Logger::getInstance().log("handleTestScriptCommand", "Test script command not recognized: " + command);
        return "INVALID COMMAND";
    }

    string constructCommand(const string& command, const string& lba, const string& value) {
        string fullCommand = command;
        if (command != "F") fullCommand += " " + lba;
        if (command == "W" || command == "E") fullCommand += " " + value;
        return fullCommand;
    }

    string readFileContent(const string& filePath) {
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

    string handleFlushCommand() {
        Logger::getInstance().log("handleFlushCommand", "Executing Flush command");

        executeSSD("F", "", "");
        return verifyExecution("[Flush] Done", "[Flush] Error");
    }

    string handleEraseCommand(istringstream& stream) {
        string startLba;
        stream >> startLba;

        Logger::getInstance().log("handleEraseCommand", "Starting erase command for LBA: " + startLba);

        validateLBA(startLba);
        return performEraseCommand(stream, startLba);
    }

    string handleEraseRangeCommand(istringstream& stream) {
        string startLba, endLba;
        stream >> startLba >> endLba;

        Logger::getInstance().log("handleEraseRangeCommand", "Starting erase range command: " + startLba + " to " + endLba);

        validateLBA(startLba);
        validateLBA(endLba);
        return performEraseRangeCommand(startLba, endLba);
    }

    string handleWriteCommand(istringstream& stream) {
        string lbaString, writeValue;
        stream >> lbaString >> writeValue;

        Logger::getInstance().log("handleWriteCommand", "Writing value: " + writeValue + " at LBA: " + lbaString);

        validateLBA(lbaString);
        validateValue(writeValue);

        executeSSD("W", lbaString, writeValue);
        return verifyExecution("[Write] Done", "[Write] Error");
    }

    string handleReadCommand(istringstream& stream) {
        string lbaString;
        stream >> lbaString;

        Logger::getInstance().log("handleReadCommand", "Reading from LBA: " + lbaString);

        validateLBA(lbaString);

        executeSSD("R", lbaString, "");
        string readOutput = readFile();

        return (readOutput == "ERROR") ? "[Read] Error" : formatReadResult(lbaString, readOutput);
    }

    string getHelpInfo() const {
        Logger::getInstance().log("getHelpInfo", "Providing help information.");
        return "Team Name : ChillCode\n Member : Oh, Seo, Kang, Lim\n";
    }

    string handleFullWriteCommand(istringstream& stream) {
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

    string handleFullReadCommand() {
        Logger::getInstance().log("handleFullReadCommand", "Executing Full Read Command");

        string result;
        for (int lba = START_LBA; lba <= END_LBA; ++lba) {
            result += handleReadCommandForLba(lba) + "\n";
        }

        return result;
    }

    string handleReadCommandForLba(int lba) {
        std::istringstream lbaStream(std::to_string(lba));
        return handleReadCommand(lbaStream);
    }

    string verifyExecution(const string& successMessage, const string& errorMessage) {
        string value = readFile();
        if (value == "ERROR") {
            Logger::getInstance().log("verifyExecution", errorMessage);
            return errorMessage;
        }
        Logger::getInstance().log("verifyExecution", successMessage);
        return successMessage;
    }

    string performEraseCommand(istringstream& stream, const string& startLba) {
        string sizeInString;
        stream >> sizeInString;

        Logger::getInstance().log("performEraseCommand", "Performing erase command with start LBA: " + startLba + ", size: " + sizeInString);

        int size = stoi(sizeInString);
        int lba = stoi(startLba);

        return executeEraseByChunks(lba, size, END_LBA, UNMAP_CHUNK_SIZE);
    }

    string executeEraseByChunks(int& lba, int& size, int maxLba, int chunkSize) {
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

    string performEraseRangeCommand(const string& startLba, const string& endLba) {
        Logger::getInstance().log("performEraseRangeCommand", "Performing erase range from LBA: " + startLba + " to " + endLba);

        int size = stoi(endLba) - stoi(startLba) + 1;
        std::istringstream eraseStream(startLba + " " + to_string(size));
        return handleEraseCommand(eraseStream);
    }

    string formatReadResult(const string& lba, const string& value) const {
        std::ostringstream oss;
        oss << "[Read] LBA "
            << std::setw(2) << std::setfill('0') << lba
            << " : " << value;
        return oss.str();
    }

    bool runCommandAndWait(const string& exe, const string& args) {
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

    void validateLBA(std::string lba) {
        int num = std::stoi(lba);
        if (num < START_LBA || num > END_LBA)
        {
            throw invalid_argument("");
        }
    }

    void validateValue(const std::string& value) {
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

    //////////////// SCRIPT ////////////////////////////
    unsigned int generateRandomHex() {
        // 난수 생성 엔진 설정
        std::random_device rd;
        std::mt19937 gen(rd()); // Mersenne Twister 엔진
        std::uniform_int_distribution<unsigned int> dist(0, 0xFFFFFFFF);

        // 랜덤 값 생성
        return dist(gen);
    }

    std::string unsignedIntToHexString(unsigned int value) {
        std::stringstream ss;
        ss << "0x"
            << std::setw(8)       // 8자리로 설정 (32비트 크기)
            << std::setfill('0')  // 0으로 채우기
            << std::hex           // 16진수 형식으로 출력
            << std::uppercase     // 대문자로 출력
            << value;             // 정수 값 삽입
        return ss.str();
    }

    std::string intToHexString(int value) {
        std::stringstream ss;
        ss << "0x"
            << std::setw(8)       // 8자리로 설정 (32비트 크기)
            << std::setfill('0')  // 0으로 채우기
            << std::hex           // 16진수 형식으로 출력
            << std::uppercase     // 대문자로 출력
            << value;             // 정수 값 삽입
        return ss.str();
    }

    bool writeScriptSubCommand(int lba, string value) {
        string input = "write " + std::to_string(lba) + " " + value;
        istringstream stream(input);
        string command;
        stream >> command;

        string result = handleWriteCommand(stream);

        if (result == "[Write] Done") return true;
        return false;
    };

    bool readCompare(int lba, string expected) {
        string input = "read " + std::to_string(lba);
        istringstream stream(input);
        string command;
        stream >> command;

        string result = handleReadCommand(stream).substr(16, 26);

        if (result == expected) return true;
        return false;
    };

    string handle1_FullWriteAndReadCompareCommand() {
        string inputs[100];

        for (int i = 0; i < 100; i++) {
            inputs[i] = intToHexString(i);
            writeScriptSubCommand(i, inputs[i]);

            if (i % 5 != 4) continue;

            for (int j = 0; j < 5; j++) {
                int idx = i - 4 + j;
                if (readCompare(idx, inputs[idx]) == false) {
                    return "FAIL";
                }
            }

        }
        return "PASS";
    }

    string handle2_PartialLBACommand() {
        string inputs[30];

        for (int i = 0; i < 30; i++) {
            inputs[i] = intToHexString(i);
            writeScriptSubCommand(4, inputs[i]);
            writeScriptSubCommand(0, inputs[i]);
            writeScriptSubCommand(3, inputs[i]);
            writeScriptSubCommand(1, inputs[i]);
            writeScriptSubCommand(2, inputs[i]);

            for (int j = 0; j < 5; j++) {
                int idx = j;
                if (readCompare(idx, inputs[i]) == false) {
                    return "FAIL";
                }
            }
        }
        return "PASS";
    }

    string handle3_WriteReadAgingCommand() {
        string inputs[200];

        for (int i = 0; i < 30; i++) {
            inputs[i] = unsignedIntToHexString(generateRandomHex());
            writeScriptSubCommand(0, inputs[i]);
            writeScriptSubCommand(99, inputs[i]);

            if (readCompare(0, inputs[i]) == false) return "FAIL";
            if (readCompare(99, inputs[i]) == false) return "FAIL";
        }
        return "PASS";
    }

    bool eraseRangeScriptSubCommand(int s, int e) {
        string input = "erase_range " + std::to_string(s) + " " + std::to_string(e);
        istringstream stream(input);
        string command;
        stream >> command;

        string result = handleEraseRangeCommand(stream);

        if (result == "[Erase] Done") return true;
        return false;
    };

    string handle4_EraseAndWriteAgingCommand() {

        string inputs[2];

        for (int i = 0; i < 30; i++) {

            for (int j = 2; j < 99; j += 2) {
                inputs[0] = unsignedIntToHexString(generateRandomHex());
                inputs[1] = unsignedIntToHexString(generateRandomHex());

                if (writeScriptSubCommand(j, inputs[0]) == false) return "FAIL";
                if (writeScriptSubCommand(j, inputs[1]) == false) return "FAIL";
                if (eraseRangeScriptSubCommand(j, (((j + 2) < 100) ? (j + 2) : 99)) == false) return "FAIL";
            }
        }
        return "PASS";
    }

    int handleRunnerCommand(string command) {
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
};
