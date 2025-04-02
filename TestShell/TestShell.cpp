#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include "../SSD/ISsdApi.h"

using namespace std;

class TestShell {
public:
    TestShell(ISsdApi* pSsdApi) : m_pSsdApi(pSsdApi)
    {
    }

    string execute(const string& userInput)
    {
        string command;
        istringstream stream(userInput);
        stream >> command;

        if (command == "read")
        {
            bool ret = m_pSsdApi->excuteCommand(userInput);

            if (ret == true)
            {
                string lba;
                stream >> lba;

                string value;

                if (lba == "0")
                {
                    lba = "00";
                    value = "00000000"; // = readOutputFile();
                }
                else if (lba == "3")
                {
                    lba = "03";
                    value = "AAAABBBB"; // = readOutputFile();
                }

                std::ostringstream oss;
                oss << "[Read] LBA " << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << lba
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
            cout << "INVALID COMMAND";
        }
    }

    void read()
    {

    }

    void write()
    {

    }

private:
    ISsdApi* m_pSsdApi;

    void splitCommand(const string& userInput, string& command, string& arg1, string& arg2) {
        istringstream stream(userInput);

        stream >> command;
        stream >> arg1;
        stream >> arg2;
    }
};