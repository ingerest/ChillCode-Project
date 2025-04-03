#pragma once
#include "Command.h"

using namespace std;

class ReadCommand : public Command
{
public:
	string cmdName = "R";
	bool excuteCommand(string commandLine, string OutputFile, string WriteFile);
	bool checkVaildParameter(string commandLine, string OutputFile, string WriteFile);
private:
	void readLba(void);
	bool parseCommandLine(string commandLine);
	const uint32_t MAX_READ_PARAM_COUNT = 2;
};
