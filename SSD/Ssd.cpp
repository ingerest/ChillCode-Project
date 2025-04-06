#include "Ssd.h"
#include "CommandFactory.h"
#include "CommandBuffer.h"

Ssd::Ssd()
{
	m_ssdNandPath = SSD_NAND_PATH;
	m_ssdOutputPath = SSD_OUTPUT_PATH;
};

bool Ssd::excuteCommand(string commandLine)
{
    if (commandLine == "") return false;

	vector<string> cmdLine = splitString(commandLine);
	string command = cmdLine[0];

	auto pCommand = CommandFactory::getInstance()->getCommandObjct(command);

    if (pCommand == nullptr) return false;

    // check command buffer : count/status
    CommandBuffer::getInstance()->checkCommandBuffer();
    
	if (false == pCommand->checkVaildParameter(commandLine, m_ssdOutputPath, m_ssdNandPath))
	{
		return false;
	}

	if (command == "R")
	{
		if (false == CommandBuffer::getInstance()->checkCacheHit(pCommand.get()))
		{
			return pCommand->excuteCommand(commandLine, m_ssdOutputPath, m_ssdNandPath);
		}
		else
		{
			return true;
		}
	}
	
	if (CommandBuffer::getInstance()->isFullCommandBuffer() || (command == "F"))
	{
		CommandBuffer::getInstance()->triggerCommandProcessing();
	}

	if (command != "F") CommandBuffer::getInstance()->addCommandToBuffer(cmdLine);
    
    return true;

}
