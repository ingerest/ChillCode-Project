#include "TestUtils.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <regex>

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

std::string extractCommandKey(const std::string& fullCommand) {
    std::regex commandRegex(R"(^(\d+))");
    std::smatch match;
    if (std::regex_search(fullCommand, match, commandRegex)) {
        return match.str(1);
    }
    return "";
}