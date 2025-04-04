#include "EraseAndWriteAging.h"

void EraseAndWriteAging::setShell(TestShell* shell)
{
	shell_ = shell;
}

bool EraseAndWriteAging::execute()
{
	constexpr int totalBlocks = 30;
	constexpr int lbaStep = 2;
	constexpr int maxLBA = 100;

	string randValue;

	if (eraseRange(0, 2) == false)
	{
		return false;
	}

	for (int i = 0; i < totalBlocks; i++)
	{
		for (int lba = 2; lba < maxLBA; lba += lbaStep)
		{
			randValue = generateRandomValue();
			if (executeWriteCommand(shell_, lba, randValue) == false)
			{
				return false;
			}

			randValue = generateRandomValue();
			if (executeWriteCommand(shell_, lba, randValue) == false)
			{
				return false;
			}

			if (eraseRange(lba, lba + lbaStep) == false)
			{
				return false;
			}
		}
	}

	return true;
}

bool EraseAndWriteAging::eraseRange(int startLba, int endLba)
{
	if (executeEraseRangeCommand(shell_, startLba, endLba) == "[Erase] Error")
	{
		return false;
	}

	return true;
}
