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
            string ret = processCommand(userInput);
            return ret;
        }
        catch (const exception& e) {
            return e.what();
        }
    }

    virtual bool executeSSD(const string& command, const string& lba, const string& value) {
        string fullCommand = command + " " + lba;
        if (command == "W") {
            fullCommand += " " + value;
        }

        string exePath1 = "../Release";
        int isExist = _access(exePath1.c_str(), 0);

        if (isExist == true)
        {
            cout << "Exist1\n";
        }

        char buffer[MAX_PATH];
        if (GetCurrentDirectory(MAX_PATH, buffer)) {
            std::cout << "Current working directory: " << buffer << std::endl;
        }
        else {
            std::cerr << "Error getting current directory!" << std::endl;
        }


        SetCurrentDirectory(exePath1.c_str());

        if (GetCurrentDirectory(MAX_PATH, buffer)) {
            std::cout << "Changed working directory: " << buffer << std::endl;
        }
        else {
            std::cerr << "Error getting current directory!" << std::endl;
        }

        ShellExecute(NULL,
            "open",
            "ssd.exe",
            fullCommand.c_str(),
            NULL,
            SW_SHOW
        );

        
        if (isSsdError()) {
            return false;
        }

        return true;
    }

    virtual string readFile(int targetLba) {
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
            cout << value;
        }

        file.close();
        return value;
    }

private:
    string processCommand(const string& userInput) {
        string command;
        istringstream stream(userInput);
        stream >> command;

        if (command == "read") {
            return handleReadCommand(stream);
        }
        else if (command == "write") {
            return handleWriteCommand(stream);
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

    string handleReadCommand(istringstream& stream) {
        string lbaString;
        stream >> lbaString;
        int lba = getLba(lbaString);

        bool ret = executeSSD("R", lbaString, "");
        if (ret) {
            string value = readFile(lba);
            return formatReadResult(lba, value);
        }
        return "[Read] Error";
    }

    string handleWriteCommand(istringstream& stream) {
        string lbaString;
        stream >> lbaString;
        int lba = getLba(lbaString);

        string value;
        stream >> value;
        if (!validateValue(value)) {
            throw invalid_argument("");
        }

        bool ret = executeSSD("W", lbaString, value);
        return ret ? "[Write] Done" : "[Write] Error";
    }

    string getHelpInfo() const {
        return "Team Name : ChillCode\n Member : Oh, Seo, Kang, Lim\n";
    }

    string handleFullWriteCommand(istringstream& stream) {
        string value;
        stream >> value;

        int startLba = 0;
        int endLba = 99;

        for (int lba = startLba; lba <= endLba; ++lba) {
            string lbaString = formatLBA(lba);
            if (!executeSSD("W", lbaString, value)) {
                throw invalid_argument("");
            }
        }

        return "[Full Write] Done";
    }

    string handleFullReadCommand() {
        int startLba = 0;
        int endLba = 99;

        string result;
        for (int lba = startLba; lba <= endLba; ++lba) {
            string lbaString = formatLBA(lba);
            if (!executeSSD("R", lbaString, "")) {
                throw invalid_argument("");
            }

            string value = readFile(lba);
            result += formatReadResult(lba, value);
        }

        return result;
    }

    string formatLBA(int lba) const {
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << lba;
        return oss.str();
    }

    string formatReadResult(int lba, const string& value) const {
        std::ostringstream oss;
        oss << "[Read] LBA "
            << std::setw(2) << std::setfill('0') << lba
            << " : " << std::setw(8) << std::setfill('0') << value;
        return oss.str();
    }


    bool isSsdError() const {
        string filePath = "../Release/ssd_output.txt";

        ifstream file(filePath);
        if (!file.is_open()) {
            throw invalid_argument("");
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            if (ss.str() == "ERROR") {
                return true;
            }
        }

        return false;
    }

    int getLba(const string& lba) {
        for (char ch : lba) {
            if (!std::isdigit(ch)) {
                throw std::invalid_argument("");
            }
        }

        int num = std::stoi(lba);
        if (num < 0 || num > 99) {
            throw std::invalid_argument("");
        }

        return num;
    }

    bool validateValue(const std::string& value) const {
        if (value.length() != 10 || value.substr(0, 2) != "0x") {
            return false;
        }

        for (size_t i = 2; i < value.length(); ++i) {
            char c = value[i];
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
                return false;
            }
        }

        try {
            unsigned int hexValue = std::stoul(value, nullptr, 16);  // Convert to hex
            return (hexValue >= 0x00000000 && hexValue <= 0xFFFFFFFF);
        }
        catch (const std::invalid_argument& e) {
            return false;
        }
        catch (const std::out_of_range& e) {
            return false;
        }
    }

};
