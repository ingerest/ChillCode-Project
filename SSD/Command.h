#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory> 

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
	virtual bool parseCommandLine(string commandLine) = 0;
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
	void readLba(void);
	bool parseCommandLine(string commandLine);
	const uint32_t MAX_READ_PARAM_COUNT = 2;
};

class WriteCommand : public Command
{
public:
	string cmdName = "W";
	bool excuteCommand(string commandLine, string OutputFile, string WriteFileP);
private:
	void writeLba(void);
	bool parseCommandLine(string commandLine);
	const uint32_t MAX_WRITE_PARAM_COUNT = 3;
};


class CommandFactory
{
public:
	unique_ptr<Command> getCommandObjct(string str);
private:
};


vector<string> splitString(const string& str);