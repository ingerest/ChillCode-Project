#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "ISsdApi.h"

using namespace std;


struct CommandParameter
{
	string lba;
	string data;
	string OutputFile;
	string WriteFile;
	uint32_t	 nLba;
	uint32_t	 nData;
};

class Command
{
public:
	string cmdName;
	virtual bool excuteCommand(string commandLine, string OutputFile, string WriteFile) = 0;

protected:
	CommandParameter m_commandParameter;
	void readLBA(void);
	virtual void parseCommandLine(string commandLine) = 0;
	bool checkVaildParameterAndStr2I(void);

private:
	const uint32_t MAX_DATA_LENGTH = 10;
	const uint32_t MAX_LBA = 99;
	const uint32_t MIN_LBA = 0;

};



class ReadCommand : public Command
{
public:
	string cmdName = "R";
	bool excuteCommand(string commandLine, string OutputFile, string WriteFile);
private:
	void parseCommandLine(string commandLine);
};

class WriteCommand : public Command
{
public:
	string cmdName = "W";
	bool excuteCommand(string commandLine, string OutputFile, string WriteFileP);
private:
	void writeLba();
	void parseCommandLine(string commandLine);
};


class Ssd : public ISsdApi
{
public:
	Ssd();

	bool excuteCommand(std::string commandLine);
private:
	const uint32_t MAX_READ_PARAM_COUNT = 2;
	const uint32_t MAX_WRITE_PARAM_COUNT = 3;
	WriteCommand wCommand;
	ReadCommand rCommand;
	std::string m_ssdNandPath;
	std::string m_ssdOutputPath;

	
};

std::vector<std::string> splitString(const std::string& str);
