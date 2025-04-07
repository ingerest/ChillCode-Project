#pragma once
#include "Command.h"

using namespace std;

class WriteCommand : public Command
{
public:
	string cmdName = "W";
	bool excuteCommand(string commandLine);
private:
	size_t parseCmdLine(string commandLine);
	bool checkVaildParameterAndStr2I(void);
	void writeLba(void);
	bool isNeedData(void) { return true; }
	bool checkValidCmd(size_t cmdParamCount);
	const uint32_t MAX_WRITE_PARAM_COUNT = 3;
	const uint32_t MAX_DATA_LENGTH = 10;
};
