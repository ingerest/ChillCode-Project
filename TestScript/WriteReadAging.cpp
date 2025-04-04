#include "WriteReadAging.h"

void WriteReadAging::setShell(TestShell* shell)
{
	shell_ = shell;
}

bool WriteReadAging::execute()
{
    string randValue1 = "";
    string randValue2 = "";
    string compareValue = "";
    string result = "";

    for (int i = 0; i < 200; i++)
    {
        randValue1 = generateRandomValue();
        randValue2 = generateRandomValue();

        if (executeWriteCommand(shell_, 0, randValue1) == false)
        {
            return false;
        }

        if (executeWriteCommand(shell_, 99, randValue2) == false)
        {
            return false;
        }
        

        if ((result=executeReadCommand(shell_, 0)) == "[Read] Error")
        {
            return false;
        }

        compareValue = makeReadResult(0, randValue1);
        if ((result == compareValue) == false)
        {
            return false;
        }

        if ((result=executeReadCommand(shell_, 99)) == "[Read] Error")
        {
            return false;
        }

        compareValue = makeReadResult(99, randValue2);
        if ((result == compareValue) == false)
        {
            return false;
        }

    }

    return true;
}
