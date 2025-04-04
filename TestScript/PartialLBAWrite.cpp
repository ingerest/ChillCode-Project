#include "PartialLBAWrite.h"

void PartialLBAWrite::setShell(TestShell* shell)
{
	shell_ = shell;
}

bool PartialLBAWrite::execute()
{
    std::vector<int> lbas = { 4, 0, 3, 1, 2 };
    string randValue = "";

    for (int i = 0; i < 100; i++)
    {
        randValue = generateRandomValue();

        if (writeRange(lbas, randValue) == false)
        {
            return false;
        }

        if (readRange(lbas, randValue) == false)
        {
            return false;
        }

    }

    return true;
}

bool PartialLBAWrite::writeRange(const std::vector<int>& lbas, std::string& value)
{
    for (auto lba : lbas)
    {
        if (executeWriteCommand(shell_, lba, value) == false)
        {
            return false;
        }
    }
    return true;
}

bool PartialLBAWrite::readRange(const std::vector<int>& lbas, std::string& value)
{
    string compareValue = "";

    for (auto lba : lbas)
    {
        compareValue = makeReadResult(lba, value);
        if (executeReadCommand(shell_, lba) != compareValue)
        {
            return false;
        }
    }
    return true;
}
