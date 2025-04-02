#pragma once
#include "Command.h"

using namespace std;

class CommandFactory
{
public:
	unique_ptr<Command> getCommandObjct(string str);
private:
};
