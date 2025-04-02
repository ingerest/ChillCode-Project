#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <cstdlib>

#include "../SSD/ISsdApi.h"

string SSD_EXE_FILE_PATH = "../release/SSD.exe";

using namespace std;

class TestShell {
public:
    string execute(const string& userInput)
    {
        try 
        {
            string ret = processCommand(userInput);
            return ret;
        }
        catch (const exception& e)
        {
            return e.what();
        }
    }

    string processCommand(const string& userInput)
    {
        string command;
        istringstream stream(userInput);
        stream >> command;

        if (command == "read")
        {
            string lbaString;
            stream >> lbaString;
            int lba = getLba(lbaString);

            bool ret = executeSSD(userInput, lbaString);
            if (ret == true)
            {
                string value = readFile(lba);

                std::ostringstream oss;
                oss << "[Read] LBA " 
                    << std::setw(2) << std::setfill('0') << lba
                    << " : " << std::setw(8) << std::setfill('0') << value;

                std::string result = oss.str();

                return result;
            }
        }
        else if (command == "write")
        {
            string lbaString;
            stream >> lbaString;
            int lba = getLba(lbaString);

            string value;
            stream >> value;
            if (validateValue(value) == false)
            {
                throw invalid_argument("");
            }

            std::string result = "";

            bool ret = executeSSD(userInput, lbaString, value);
            if (ret)
            {
                result = "[Write] Done";
            }
            else
            {
                throw invalid_argument("");
            }
            return result;
        }
        else if (command == "exit")
        {
            return "[exit] Done";
        }
        else if (command == "help")
        {
            return "Team Name : ChillCode\n Member : Oh, Seo, Kang, Lim";
        }
        else if (command == "fullwrite")
        {
            string value;
            stream >> value;

            int startLba = 0;
            int endLba = 99;

            for (int lba = startLba; lba <= endLba; lba++)
            {
                std::ostringstream lbaString;
                std::ostringstream valueString;

                lbaString << std::setw(2) << std::setfill('0') << lba;
                valueString << std::setw(8) << std::setfill('0') << value;

                bool ret = executeSSD(userInput, lbaString.str(), valueString.str());
                if (ret == false)
                {
                    throw invalid_argument("");
                }
            }

            return "[Full Write] Done";
        }
        else if (command == "fullread")
        {
            int startLba = 0;
            int endLba = 99;

            string result;

            for (int lba = startLba; lba <= endLba; lba++)
            {
                string value = readFile(lba);
                std::ostringstream oss;
                oss << "[Read] LBA "
                    << std::setw(2) << std::setfill('0') << lba
                    << " : " << std::setw(8) << std::setfill('0') << value;

                result += oss.str();

            }
            return result;
        }
        else
        {
            throw invalid_argument("INVALID COMMAND");
        }
    }

    virtual string readFile(int targetLba) {
        string filePath = "../Release/ssd_output.txt";

        ifstream file(filePath);
        string line;

        if (!file.is_open()) {
            throw invalid_argument("");
        }

        while (getline(file, line)) {
            stringstream ss(line);
            int lba;
            string value;

            ss >> lba >> value;
            if (lba == targetLba) {
                file.close();
                return value;
            }
        }

        file.close();
        throw invalid_argument("");
    }

private:
    void splitCommand(const string& userInput, string& command, string& arg1, string& arg2) {
        istringstream stream(userInput);

        stream >> command;
        stream >> arg1;
        stream >> arg2;
    }

    int getLba(const string& lba)
    {
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

    bool validateValue(const std::string& value) {
        if (value.length() != 10) {
            return false;
        }

        if (value.substr(0, 2) != "0x") {
            return false;
        }

        for (size_t i = 2; i < value.length(); ++i) {
            char c = value[i];
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
                return false;
            }
        }

        unsigned int hexValue;
        try {
            hexValue = std::stoul(value, nullptr, 16);  // 16진수로 변환
        }
        catch (const std::invalid_argument& e) {
            return false;
        }
        catch (const std::out_of_range& e) {
            return false;
        }

        if (hexValue < 0x00000000 || hexValue > 0xFFFFFFFF) {
            return false;
        }

        return true;
    }
    
    bool executeSSD(string command, string lba, string value = "")
    {
        string fullCommand = SSD_EXE_FILE_PATH + " " + command + " " + lba;

        if (command == "W")
        {
            fullCommand += " " + value;
        }

        int ret = std::system(fullCommand.c_str());
        if (ret != 0) {
            throw invalid_argument("");
        }

        // output.txt 확인

        return true;
    }
};