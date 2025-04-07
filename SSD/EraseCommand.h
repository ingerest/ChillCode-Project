#pragma once
#include "Command.h"

using namespace std;

class EraseCommand : public Command
{
public:
	string cmdName = "E";
	bool excuteCommand(string commandLine);
private:
	void eraseLba(void);
	size_t parseCmdLine(string commandLine);
	bool checkVaildParameterAndStr2I(void);
	bool isNeedData(void) { return true; }
	bool checkValidCmd(size_t cmdParamCount);
	const uint32_t MAX_ERASE_PARAM_COUNT = 3;
	const string ERASE_PATTERN = "0x00000000";
};
