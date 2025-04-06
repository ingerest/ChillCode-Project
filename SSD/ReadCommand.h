#pragma once
#include "Command.h"

using namespace std;

class ReadCommand : public Command
{
public:
	string cmdName = "R";
	bool excuteCommand(string commandLine);
private:
	void readLba(void);
	bool checkValidCmd(size_t cmdParamCount);
	const uint32_t MAX_READ_PARAM_COUNT = 2;
};
