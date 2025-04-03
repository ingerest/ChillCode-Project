#include "../TestShell/TestShell.cpp"
#include <windows.h>
#include <random>
#include <iomanip>
#include <sstream>

bool runFullWriteAndReadCompare(TestShell* shell);

extern "C" __declspec(dllexport)
int runTest(const char* command, TestShell* shell) {

    string commandLine = string(command);

    std::cout << "[Command] " << commandLine << std::endl;

    if (commandLine == "1_FullWriteAndReadCompare")
    {
        cout << "pass" << endl;
        return runFullWriteAndReadCompare(shell);
    }
    else
    {
        cout << "fail" << endl;
    }
    return 0;
}

string generateRandomValue() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

    uint32_t value = dist(gen);

    std::stringstream ss;
    ss << "0x"
        << std::uppercase << std::hex
        << std::setw(8) << std::setfill('0')
        << value;

    return ss.str();
}

bool runFullWriteAndReadCompare(TestShell* shell)
{
    int index = 0;
    int endIndex = 0;
    string writeCommand = "";
    string readCommand = "";
    string randValue = "";
    string compareValue = "";
    string result = "";

    vector<string> writeBuffer;
    cout << "temp" << endl;

    std::ostringstream oss;

    while (index < 100)
    {
        endIndex += 5;

        for (int i = index; i < endIndex; i++)
        {
            randValue = generateRandomValue();
            writeCommand = "write " + std::to_string(i) + " "  + randValue;
            writeBuffer.push_back(randValue);
            result = shell->execute(writeCommand);

            if (result == "[Write] Error")
            {
                return false;
            }
        }

        for (int i = index; i < endIndex; i++)
        {
            readCommand = "read " + std::to_string(i);
            result = shell->execute(readCommand);

            if (result == "[Read] Error")
            {
                return false;
            }
            
            oss = std::ostringstream();
            oss << std::setw(2) << std::setfill('0') << i;
            compareValue = "[Read] LBA " + oss.str() + " : " + writeBuffer[i];
            if ((result == compareValue) == false)
            {
                return false;
            }
        }

        index += 5;
    }

    return true;
}
