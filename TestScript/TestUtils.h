#pragma once
#include <string>
#include "../TestShell/TestShell.cpp"
std::string generateRandomValue();
std::string makeReadResult(int lba, std::string& value);

std::string executeReadCommand(TestShell* shell, int lba);
std::string executeEraseCommand(TestShell* shell, int lba, int size);
std::string executeEraseRangeCommand(TestShell* shell, int startLba, int endLba);

bool executeWriteCommand(TestShell* shell, int lba, std::string& value);