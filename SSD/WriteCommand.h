#pragma once
#include "Command.h"

using namespace std;

class WriteCommand : public Command
{
public:
	string cmdName = "W";
	bool excuteCommand(string commandLine, string OutputFile, string WriteFile);
	bool checkVaildParameter(string commandLine, string OutputFile, string WriteFile);
private:
	void writeLba(void);
	bool parseCommandLine(string commandLine);
	const uint32_t MAX_WRITE_PARAM_COUNT = 3;
};
