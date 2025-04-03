#pragma once
#include "Command.h"

using namespace std;

class EraseCommand : public Command
{
public:
	string cmdName = "E";
	bool excuteCommand(string commandLine, string OutputFile, string WriteFile);
	bool checkVaildParameter(string commandLine, string OutputFile, string WriteFile);
private:
	void eraseLba(void);
	bool parseCommandLine(string commandLine);
	bool checkVaildParameterAndStr2I(void);
	const uint32_t MAX_ERASE_PARAM_COUNT = 3;
	const string ERASE_PATTERN = "0x00000000";
};
