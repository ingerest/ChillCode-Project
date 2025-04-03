#include "CommandFactory.h"
#include "ReadCommand.h"
#include "WriteCommand.h"
#include "EraseCommand.h"


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

	if (str == "E")
	{
		return make_unique<EraseCommand>();
	}

	return nullptr;
}