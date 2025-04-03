#include "Ssd.h"
#include "CommandFactory.h"
#include "CommandBuffer.h"


const string SSD_NAND_PATH = "ssd_nand.txt";
const string SSD_OUTPUT_PATH = "../Release/ssd_output.txt";

CommandFactory cmdFactory;
CommandBuffer cmdBuffer;

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

    auto pCommand = cmdFactory.getCommandObjct(command);

    if (pCommand == nullptr) return false;

    // check command buffer : count/status
    cmdBuffer.checkCommandBuffer();
    
	if (false == pCommand->checkVaildParameter(commandLine, m_ssdOutputPath, m_ssdNandPath))
	{
		return false;
	}

	if (command == "R")
	{
		if (false == cmdBuffer.checkCacheHit(pCommand.get()))
		{
			return pCommand->excuteCommand(commandLine, m_ssdOutputPath, m_ssdNandPath);
		}
		else
		{
			return true;
		}
	}
	
	if (cmdBuffer.isFullCommandBuffer() || (command == "F"))
	{
		cmdBuffer.triggerCommandProcessing();
	}

	cmdBuffer.addCommandToBuffer(cmdLine);
    
    return true;

}
