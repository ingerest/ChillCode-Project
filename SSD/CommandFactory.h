#pragma once
#include "Command.h"

using namespace std;

class CommandFactory
{
public:
	unique_ptr<Command> getCommandObjct(string str);
	static CommandFactory* getInstance(void);
private:
	CommandFactory() = default;
	static CommandFactory* m_instance;
};
