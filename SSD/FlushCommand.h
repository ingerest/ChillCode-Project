#pragma once
#include "Command.h"

using namespace std;

class FlushCommand : public Command
{
public:
	string cmdName = "F";
	bool excuteCommand(string commandLine, string OutputFile, string WriteFile);
	bool checkVaildParameter(string commandLine, string OutputFile, string WriteFile);
private:
	bool parseCommandLine(string commandLine);
};