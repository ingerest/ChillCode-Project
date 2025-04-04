#include "TestCommandFactory.h"
#include "FullWriteAndReadCompareCommand.h"
#include "PartialLBAWrite.h"
#include "WriteReadAging.h"
#include <unordered_map>
#include <functional>

// 커맨드 등록 테이블
static const std::unordered_map<std::string, std::function<std::unique_ptr<ITestCommand>()>> commandMap = {
    {"1", []() { return std::make_unique<FullWriteAndReadCompareCommand>(); }},
    {"2", []() { return std::make_unique<PartialLBAWrite>(); }},
    {"3", []() { return std::make_unique<WriteReadAging>(); }}
};

std::string TestCommandFactory::extractCommandKey(const std::string& commandName) {
    size_t pos = commandName.find('_');
    if (pos != std::string::npos) {
        return commandName.substr(0, pos);
    }
    return "";
}

std::unique_ptr<ITestCommand> TestCommandFactory::create(const std::string& commandName, TestShell* shell) {
    std::string key = extractCommandKey(commandName);

    if (key == "")
    {
        return nullptr;
    }

    auto it = commandMap.find(key);
    if (it == commandMap.end()) {
        return nullptr;
    }

    std::unique_ptr<ITestCommand> cmd = it->second();
    cmd->setShell(shell);
    return cmd;
}