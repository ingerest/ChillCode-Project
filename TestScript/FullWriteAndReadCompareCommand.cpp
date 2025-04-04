#include "FullWriteAndReadCompareCommand.h"

void FullWriteAndReadCompareCommand::setShell(TestShell* shell) {
    shell_ = shell;
}

bool FullWriteAndReadCompareCommand::execute() {
    constexpr int totalBlocks = 100;
    constexpr int blockStep = 5;

    std::vector<std::string> writeBuffer;
    writeBuffer.reserve(totalBlocks);

    for (int index = 0; index < totalBlocks; index += blockStep) {
        int endIndex = index + blockStep;

        if (!writeRange(index, endIndex, writeBuffer))
            return false;

        if (!verifyRange(index, endIndex, writeBuffer))
            return false;
    }

    return true;
}


bool FullWriteAndReadCompareCommand::writeRange(int start, int end, std::vector<std::string>& buffer) {
    for (int i = start; i < end; ++i) {
        std::string randValue = generateRandomValue();
        
        if (executeWriteCommand(shell_, i, randValue) == false)
        {
            return false;
        }

        buffer.push_back(randValue);
    }

    return true;
}

bool  FullWriteAndReadCompareCommand::verifyRange(int start, int end, std::vector<std::string>& buffer) {
    for (int i = start; i < end; ++i) {
        
        std::string result = executeReadCommand(shell_, i);
        if (result == "[Read] Error")
            return false;

        std::string expected = makeReadResult(i, buffer[i]);
        if (result != expected)
            return false;
    }

    return true;
}