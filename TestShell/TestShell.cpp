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

#include "../SSD/ISsdApi.h"

using namespace std;

class TestShell {
public:
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
        string fullCommand = command + " " + lba;

        if (command == "W") {
            fullCommand += " " + value;
        }

        string exePath = "../Release";
        SetCurrentDirectory(exePath.c_str());

        ShellExecute(NULL,
            "open",
            "ssd.exe",
            fullCommand.c_str(),
            NULL,
            SW_HIDE
        );
    }

    virtual string readFile() {
        string filePath = "../Release/ssd_output.txt";
        ifstream file(filePath);

        if (!file.is_open()) {
            throw invalid_argument("");
        }

        string line;
        string value;

        while (getline(file, line))
        {
            value = line.c_str();
        }
        file.close();

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
        else {
            throw invalid_argument("INVALID COMMAND");
        }
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

    string formatReadResult(const string& lba, const string& value) const {
        std::ostringstream oss;
        oss << "[Read] LBA "
            << std::setw(2) << std::setfill('0') << lba
            << " : " << value;
        return oss.str();
    }

    void validateLBA(const std::string& lba) {
        int num = std::stoi(lba);
        if (num < 0 && num > 99)
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
