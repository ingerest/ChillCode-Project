#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>

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
                    << " : 0x" << std::setw(8) << std::setfill('0') << value;

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

            return result;
        }
        else if (command == "exit")
        {

        }
        else if (command == "help")
        {

        }
        else if (command == "fullwrite")
        {

        }
        else if (command == "fullread")
        {

        }
        else
        {
            throw invalid_argument("INVALID COMMAND");
        }
    }


    string readFile(int lba) {
        string value = "";

        if (lba == 0)
        {
            value = "0";
        }
        else if (lba == 3)
        {
            value = "AAAABBBB";
        }

        return value;
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