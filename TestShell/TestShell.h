#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <shellapi.h>
#include <io.h>
#include <filesystem>
#include <random>
#include <algorithm>

#include "Logger.cpp"

using namespace std;

class TestShell {
public:
    typedef int (*RunTestFunc)(const char*, TestShell*);

    virtual string execute(const string& userInput);
    virtual void executeSSD(const string& command, const string& lba, const string& value);
    virtual string readFile();

private:
    static const int START_LBA = 0;
    static const int END_LBA = 99;
    static const int UNMAP_CHUNK_SIZE = 10;
    static const int INPUT_ARRAY_SIZE = 100;
    static const int PARTIAL_LBA_LIMIT = 30;
    static const int COMPARE_BATCH_SIZE = 5;

    string processCommand(const string& userInput);
    string executeCommand(const string& command, istringstream& stream);
    bool isTestScriptCommand(const string& command);
    string handleTestScriptCommand(const string& command);
    string constructCommand(const string& command, const string& lba, const string& value);
    string readFileContent(const string& filePath);
    string handleFlushCommand();
    string handleEraseCommand(istringstream& stream);
    string handleEraseRangeCommand(istringstream& stream);
    string handleWriteCommand(istringstream& stream);
    string handleReadCommand(istringstream& stream);
    string getHelpInfo() const;
    string handleFullWriteCommand(istringstream& stream);
    string handleFullReadCommand();
    string handleReadCommandForLba(int lba);
    string verifyExecution(const string& successMessage, const string& errorMessage);
    string performEraseCommand(istringstream& stream, const string& startLba);
    string executeEraseByChunks(int& lba, int& size, int maxLba, int chunkSize);
    string performEraseRangeCommand(const string& startLba, const string& endLba);
    string formatReadResult(const string& lba, const string& value) const;
    bool runCommandAndWait(const string& exe, const string& args);
    void validateLBA(std::string lba);
    void validateValue(const std::string& value);

    unsigned int generateRandomHex();
    std::string unsignedIntToHexString(unsigned int value);
    std::string intToHexString(int value);
    bool writeScriptSubCommand(int lba, string value);
    bool readCompare(int lba, string expected);

    string handle1_FullWriteAndReadCompareCommand();
    string handle2_PartialLBACommand();
    string handle3_WriteReadAgingCommand();
    bool eraseRangeScriptSubCommand(int s, int e);
    string handle4_EraseAndWriteAgingCommand();

    int handleRunnerCommand(string command);
};