#include "TestUtils.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <regex>
#include "../TestShell/TestShell.cpp"

std::string generateRandomValue() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);
    uint32_t value = dist(gen);

    std::stringstream ss;
    ss << "0x"
        << std::uppercase << std::hex
        << std::setw(8) << std::setfill('0')
        << value;
    return ss.str();
}

std::string makeReadResult(int lba, std::string& value) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << lba;
    return "[Read] LBA " + oss.str() + " : " + value;
}

bool executeWriteCommand(TestShell* shell, int lba, std::string& value)
{
    std::string writeCommand = "write " + std::to_string(lba) + " " + value;

    std::string result = shell->execute(writeCommand);
    if (result == "[Write] Error")
        return false;

    return true;
}

std::string executeReadCommand(TestShell* shell, int lba)
{
    std::string readCommand = "read " + std::to_string(lba);
    
    return shell->execute(readCommand);
}

std::string executeEraseCommand(TestShell* shell, int lba, int size)
{
    std::string eraseCommand = "erase " + std::to_string(lba) + " " + std::to_string(size);

    return shell->execute(eraseCommand);
}

std::string executeEraseRangeCommand(TestShell* shell, int startLba, int endLba)
{
    std::string eraseRangeCommand = "erase_range " + std::to_string(startLba) + " " + std::to_string(endLba);

    return shell->execute(eraseRangeCommand);
}
