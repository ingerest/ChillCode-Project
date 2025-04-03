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

        if (!writeAndReadRange(shell_, index, endIndex, writeBuffer))
            return false;

        if (!verifyRange(shell_, index, endIndex, writeBuffer))
            return false;
    }

    return true;
}


bool FullWriteAndReadCompareCommand::writeAndReadRange(TestShell* shell, int start, int end, std::vector<std::string>& buffer) {
    for (int i = start; i < end; ++i) {
        std::string randValue = generateRandomValue();
        std::string writeCommand = "write " + std::to_string(i) + " " + randValue;

        std::string result = shell->execute(writeCommand);
        if (result == "[Write] Error")
            return false;

        buffer.push_back(randValue);
    }

    return true;
}

bool  FullWriteAndReadCompareCommand::verifyRange(TestShell* shell, int start, int end, std::vector<std::string>& buffer) {
    for (int i = start; i < end; ++i) {
        std::string readCommand = "read " + std::to_string(i);
        std::string result = shell->execute(readCommand);
        if (result == "[Read] Error")
            return false;

        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << i;

        std::string expected = "[Read] LBA " + oss.str() + " : " + buffer[i];
        if (result != expected)
            return false;
    }

    return true;
}