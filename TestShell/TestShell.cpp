#pragma once

#include <iostream>
#include <sstream>
#include <string>

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
                // Need Implement output file read
                return "3";
            }
        }
        else if (command == "write")
        {

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

        return "";
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