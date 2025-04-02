#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <fstream>

#include "../SSD/ISsdApi.h"

using namespace std;

class TestShell {
public:
    TestShell(ISsdApi* pSsdApi) : m_pSsdApi(pSsdApi)
    {
    }

    string execute(const string& userInput)
    {
        try 
        {
            string ret = processCommand(userInput);
            return ret;
        }
        catch (const exception& e)
        {
            cout << e.what();
        }
    }

    string processCommand(const string& userInput)
    {
        string command;
        istringstream stream(userInput);
        stream >> command;

        if (command == "read")
        {
            bool ret = m_pSsdApi->excuteCommand(userInput);

            if (ret == true)
            {
                string lbaString;
                stream >> lbaString;

                int lba = getLba(lbaString);
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
            std::string result = "";

            bool ret = m_pSsdApi->excuteCommand(userInput);

            if (ret)
            {
                result = "[Write] Done";
            }
            else
            {
                throw invalid_argument("INVALID COMMAND");
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
                std::ostringstream oss;
                oss << "Write "
                    << std::setw(2) << std::setfill('0') << lba
                    << " " << std::setw(8) << std::setfill('0') << value;

                std::string userInput = oss.str();

                bool ret = m_pSsdApi->excuteCommand(userInput);
                if (ret == false)
                {
                    throw invalid_argument("INVALID COMMAND");
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
            throw invalid_argument("Failed to open file.");
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
        throw invalid_argument("No matching lba");
    }

private:
    ISsdApi* m_pSsdApi;

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
                throw std::invalid_argument("INVALID COMMAND: Non-digit character found");
            }
        }

        int num = std::stoi(lba);
        if (num < 0 || num > 99) {
            throw std::invalid_argument("INVALID COMMAND: Value out of range (0 to 99)");
        }

        return num;
    }

    
};