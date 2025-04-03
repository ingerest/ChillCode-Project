#include "../TestShell/TestShell.cpp"
#include <windows.h>
#include <random>
#include <iomanip>
#include <sstream>

bool runFullWriteAndReadCompare(TestShell* shell);
bool runPartialLBAWrite(TestShell* shell);
bool runWriteReadAging(TestShell* shell);

extern "C" __declspec(dllexport)
int runTest(const char* command, TestShell* shell) {

    string commandLine = string(command);

    std::cout << "[Command] " << commandLine << std::endl;

    if (commandLine == "1_FullWriteAndReadCompare")
    {
        return runFullWriteAndReadCompare(shell);
    }
    else if (commandLine == "2_PartialLBAWrite")
    {
        return runPartialLBAWrite(shell);
    }
    else if (commandLine == "3_WriteReadAging")
    {
        return runWriteReadAging(shell);
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

bool runWriteReadAging(TestShell* shell)
{
    int index = 0;
    int endIndex = 0;
    string writeCommand = "";
    string readCommand = "";
    string randValue1 = "";
    string randValue2 = "";
    string compareValue = "";
    string result = "";

    std::ostringstream oss;

    for (int i = 0; i < 200; i++)
    {
        randValue1 = generateRandomValue();
        writeCommand = "write 0 " + randValue1;
        result = shell->execute(writeCommand);
        if (result == "[Write] Error")
        {
            return false;
        }

        randValue2 = generateRandomValue();
        writeCommand = "write 99 " + randValue2;
        result = shell->execute(writeCommand);
        if (result == "[Write] Error")
        {
            return false;
        }

        readCommand = "read 0";
        result = shell->execute(readCommand);

        if (result == "[Read] Error")
        {
            return false;
        }

        compareValue = "[Read] LBA 00 : " + randValue1;
        if ((result == compareValue) == false)
        {
            return false;
        }

        readCommand = "read 99";
        result = shell->execute(readCommand);

        if (result == "[Read] Error")
        {
            return false;
        }

        compareValue = "[Read] LBA 99 : " + randValue2;
        if ((result == compareValue) == false)
        {
            return false;
        }

    }

    return true;
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

    std::ostringstream oss;

    while (index < 100)
    {
        endIndex += 5;

        for (int i = index; i < endIndex; i++)
        {
            randValue = generateRandomValue();
            writeCommand = "write " + std::to_string(i) + " " + randValue;
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

bool runPartialLBAWrite(TestShell* shell)
{
    int index = 0;
    int endIndex = 0;
    string writeCommand = "";
    string readCommand = "";
    string randValue = "";
    string compareValue = "";
    string result = "";

    std::ostringstream oss;

    for (int i = 0; i < 100; i++)
    {
        randValue = generateRandomValue();
        writeCommand = "write 4 " + randValue;
        result = shell->execute(writeCommand);
        if (result == "[Write] Error")
        {
            return false;
        }
        writeCommand = "write 0 " + randValue;
        result = shell->execute(writeCommand);
        if (result == "[Write] Error")
        {
            return false;
        }
        writeCommand = "write 3 " + randValue;
        result = shell->execute(writeCommand);
        if (result == "[Write] Error")
        {
            return false;
        }
        writeCommand = "write 1 " + randValue;
        result = shell->execute(writeCommand);
        if (result == "[Write] Error")
        {
            return false;
        }

        writeCommand = "write 2 " + randValue;
        result = shell->execute(writeCommand);
        if (result == "[Write] Error")
        {
            return false;
        }

        for (int j = 0; j < 5; j++)
        {
            readCommand = "read " + std::to_string(j);
            result = shell->execute(readCommand);

            if (result == "[Read] Error")
            {
                return false;
            }

            oss = std::ostringstream();
            oss << std::setw(2) << std::setfill('0') << j;
            compareValue = "[Read] LBA " + oss.str() + " : " + randValue;
            if ((result == compareValue) == false)
            {
                return false;
            }
        }

    }

    return true;
}