#include "CommandFactory.h"
#include "ReadCommand.h"
#include "WriteCommand.h"


unique_ptr<Command> CommandFactory::getCommandObjct(string str)
{
	if (str == "W")
	{
		return make_unique<WriteCommand>();
	}

	if (str == "R")
	{
		return make_unique<ReadCommand>();
	}

	return nullptr;
}