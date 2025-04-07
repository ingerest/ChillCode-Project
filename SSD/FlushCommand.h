#pragma once
#include "Command.h"

using namespace std;

class FlushCommand : public Command
{
public:
	string cmdName = "F";
	bool excuteCommand(string commandLine);
	bool checkVaildParameter(string commandLine);
private:
	bool isNeedData(void) { return false; }
	bool checkValidCmd(size_t cmdParamCount) { return false; }
	bool parseCmdLineAndCheckValidCmd(string commandLine);
};