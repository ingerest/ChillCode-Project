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
#include <io.h>  // _access 함수를 사용하려면 필요
#include <filesystem>  // C++17 standard library
#include <random>

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

        string fullCommand = command;

        if (command != "F")
        {
            fullCommand += " " + lba;
        }

        if (command == "W" || command == "E") {
            fullCommand += " " + value;
        }

        string exePath = "../Release";
        SetCurrentDirectory(exePath.c_str());

        if (runCommandAndWait("ssd.exe", fullCommand) == false)
        {
            throw invalid_argument("INVALID COMMAND");
        }
    }

    bool runCommandAndWait(const string& exe, const string& args) {
        string fullPath = exe + " " + args;

        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;

        if (!CreateProcessA(
            NULL,
            const_cast<char*>(fullPath.c_str()),
            NULL, NULL, FALSE,
            0, NULL, NULL,
            &si, &pi)) {
            return false;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }


    virtual string readFile() {
        string filePath = "../Release/ssd_output.txt";
        ifstream file(filePath);

        string line;
        string value;

        if (file.is_open()) {
            while (getline(file, line))
            {
                value = line.c_str();
            }
            file.close();
        }

        return value;
    }

private:
    string processCommand(const string& userInput) {
        string command;
        istringstream stream(userInput);
        stream >> command;

        if (command == "read" || command == "write") {
            string lbaString;
            stream >> lbaString;
            validateLBA(lbaString);

            if (command == "read")
            {
                return handleReadCommand(lbaString);
            }
            else
            {
                return handleWriteCommand(stream, lbaString);
            }
        }
        else if (command == "exit") {
            return "[exit] Done";
        }
        else if (command == "help") {
            return getHelpInfo();
        }
        else if (command == "fullwrite") {
            return handleFullWriteCommand(stream);
        }
        else if (command == "fullread") {
            return handleFullReadCommand();
        }
        else if (command == "erase") {
            return handleEraseCommand(stream);
        }
        else if (command == "erase_range") {
            return handleEraseRangeCommand(stream);
        }
        else if (command == "flush") {
            executeSSD("F", "", "");

            string value = readFile();
            if (value == "ERROR")
            {
                return "[Flush] Error";
            }

            return "[Flush] Done";
        }
        // Test Script
        else if (command == "1_FullWriteAndReadCompare" || command == "1_") {
            return handle1_FullWriteAndReadCompareCommand();
        }
        else if (command == "2_PartialLBAWrite" || command == "2_") {
            return handle2_PartialLBACommand();
        }
        else if (command == "3_WriteReadAging" || command == "3_") {
            return handle3_WriteReadAgingCommand();
        }
        else if (command == "4_EraseAndWriteAging" || command == "4_") {
            return handle4_EraseAndWriteAgingCommand();
        }
        else {
            int res = handleRunnerCommand(command);
            if (res == -1) throw invalid_argument("INVALID COMMAND");
        }
    }

    string handleEraseCommand(std::istringstream& stream)
    {
        string startLba;
        stream >> startLba;
        validateLBA(startLba);

        string sizeInString;
        stream >> sizeInString;

        int maxLba = 99;
        int size = stoi(sizeInString);
        int lba = stoi(startLba);
        int chunkSize = 10;

        while (size > 0 && lba <= maxLba) {
            int currentChunkSize;
            if (size > chunkSize) {
                currentChunkSize = chunkSize;
            }
            else {
                currentChunkSize = size;
            }

            if (lba + currentChunkSize - 1 > maxLba)
            {
                currentChunkSize = maxLba - lba + 1;
            }

            executeSSD("E", to_string(lba), to_string(currentChunkSize));

            string value = readFile();
            if (value == "ERROR")
            {
                return "[Erase] Error";
            }

            lba += currentChunkSize;
            size -= currentChunkSize;
        }

        return "[Erase] Done";
    }

    string handleEraseRangeCommand(std::istringstream& stream)
    {
        string startLba;
        stream >> startLba;
        validateLBA(startLba);

        string endLba;
        stream >> endLba;
        validateLBA(endLba);

        int size = stoi(endLba) - stoi(startLba) + 1;
        std::istringstream eraseStream(startLba + " " + to_string(size));

        return handleEraseCommand(eraseStream);
    }

    string handleWriteCommand(std::istringstream& stream, std::string& lbaString)
    {
        string writeValue;
        stream >> writeValue;
        validateValue(writeValue);

        executeSSD("W", lbaString, writeValue);

        string value = readFile();
        if (value == "ERROR")
        {
            return "[Write] Error";
        }

        return "[Write] Done";
    }

    string handleReadCommand(std::string& lbaString)
    {
        executeSSD("R", lbaString, "");

        string readOutput = readFile();
        if (readOutput == "ERROR")
        {
            return "[Read] Error";
        }

        string ret = formatReadResult(lbaString, readOutput);
        return ret;
    }

    string getHelpInfo() const {
        return "Team Name : ChillCode\n Member : Oh, Seo, Kang, Lim\n";
    }

    string handleFullWriteCommand(istringstream& stream) {
        string writeValue;
        stream >> writeValue;
        validateValue(writeValue);

        int startLba = 0;
        int endLba = 99;

        for (int lba = startLba; lba <= endLba; ++lba) {
            executeSSD("W", to_string(lba), writeValue);
            string readOutput = readFile();
            if (readOutput == "ERROR")
            {
                return "[Fullwrite] Error";
            }
        }

        return "[Fullwrite] Done";
    }

    string handleFullReadCommand() {
        int startLba = 0;
        int endLba = 99;

        string result;
        for (int lba = startLba; lba <= endLba; ++lba) {
            string lbaString = to_string(lba);
            result += handleReadCommand(lbaString) + "\n";
        }

        return result;
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
        string lbaString;
        stream >> lbaString;
        validateLBA(lbaString);

        string result = handleWriteCommand(stream, lbaString);

        if (result == "[Write] Done") return true;
        return false;
    };

    bool readCompare(int lba, string expected) {
        string input = "read " + std::to_string(lba);
        istringstream stream(input);
        string command;
        stream >> command;
        string lbaString;
        stream >> lbaString;
        validateLBA(lbaString);

        string result = handleReadCommand(lbaString).substr(16, 26);

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
            std::cerr << "DLL 로딩 실패" << std::endl;  // ToDo : Loger로 처리 필요
            return -1;
        }

        RunTestFunc runTest = (RunTestFunc)GetProcAddress(hModule, "runTest");
        if (!runTest) {
            std::cerr << "함수 찾기 실패" << std::endl;   // ToDo : Loger로 처리 필요
            FreeLibrary(hModule);
            return -1;
        }

        TestShell shell;
        int result = runTest(command.c_str(), &shell);
        if (result == -1) return -1;    // "INVALID COMMAND"
        string Result = (result == 1) ? "PASS" : "FAIL";
        std::cout << Result;

        FreeLibrary(hModule);
        return 0;

    }

    string formatReadResult(const string& lba, const string& value) const {
        std::ostringstream oss;
        oss << "[Read] LBA "
            << std::setw(2) << std::setfill('0') << lba
            << " : " << value;
        return oss.str();
    }

    void validateLBA(std::string lba) {
        int num = std::stoi(lba);
        if (num < 0 || num > 99)
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
};
