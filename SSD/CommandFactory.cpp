#include "CommandFactory.h"
#include "ReadCommand.h"
#include "WriteCommand.h"
#include "EraseCommand.h"
#include "FlushCommand.h"

CommandFactory* CommandFactory::m_instance = nullptr;

CommandFactory* CommandFactory::getInstance(void)
{
	if (m_instance == nullptr) {
		m_instance = new CommandFactory();
	}
	return m_instance;
}


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

	if (str == "F")
	{
		return make_unique<FlushCommand>();
	}

	return nullptr;
}