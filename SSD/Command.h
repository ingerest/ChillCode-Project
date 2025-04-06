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
	uint32_t	 nLba;
	uint32_t	 nData;  // Write : data, Erase : length
};

class Command
{
public:
	string cmdName;
	virtual bool excuteCommand(string commandLine) = 0;
	bool checkVaildParameter(string commandLine);
	bool updateOutputFile(string data);
	uint32_t getLba(void) { return m_commandParameter.nLba; }
protected:
	CommandParameter m_commandParameter;
	bool parseCmdLineAndCheckValidCmd(string commandLine);
	bool checkVaildParameterAndStr2I(void);
	bool updateErrorMsg2TextFile(void);
	bool checkValidCmd(size_t cmdParamCount);
	bool isNeedData(void) { return false;}
	size_t parseCmdLine(string commandLine);

private:
	const uint32_t MAX_LBA = 99;
	const uint32_t MIN_LBA = 0;
};

vector<string> splitString(const string& str);