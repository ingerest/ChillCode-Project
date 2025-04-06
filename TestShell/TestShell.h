#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "Logger.cpp"
#include "TestScript.cpp"

class TestShell {
public:
    typedef int (*RunTestFunc)(const char*, TestShell*);

    TestShell();  // Constructor

    std::string execute(const std::string& userInput);
    virtual void executeSSD(const std::string& command, const std::string& lba, const std::string& value);
    virtual std::string readFile();

    // Command processing methods
    std::string processCommand(const std::string& userInput);
    std::string executeCommand(const std::string& command, std::istringstream& stream);
    std::string constructCommand(const std::string& command, const std::string& lba, const std::string& value);

    // Handlers for commands
    std::string handleWriteCommand(std::istringstream& stream);
    std::string handleReadCommand(std::istringstream& stream);
    std::string handleEraseCommand(std::istringstream& stream);
    std::string handleEraseRangeCommand(std::istringstream& stream);
    std::string getHelpInfo() const;
    std::string handleFullWriteCommand(std::istringstream& stream);
    std::string handleFullReadCommand();
    string handleReadCommandForLba(int lba);
    void validateLBA(std::string lba);
    void validateValue(const std::string& value);

private:
    TestScript* m_testScript;

    // Constants
    static const int START_LBA = 0;
    static const int END_LBA = 99;
    static const int UNMAP_CHUNK_SIZE = 10;

    // File handling and execution methods
    std::string readFileContent(const std::string& filePath);
    std::string handleFlushCommand();
    std::string verifyExecution(const std::string& successMessage, const std::string& errorMessage);
    std::string performEraseCommand(std::istringstream& stream, const std::string& startLba);
    std::string executeEraseByChunks(int& lba, int& size, int maxLba, int chunkSize);
    std::string performEraseRangeCommand(const std::string& startLba, const std::string& endLba);
    std::string formatReadResult(const std::string& lba, const std::string& value) const;

    // Process execution
    bool runCommandAndWait(const std::string& exe, const std::string& args);
    int handleRunnerCommand(std::string command);
};