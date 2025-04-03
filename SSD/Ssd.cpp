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
    if (commandLine == "") false;

	vector<string> cmdLine = splitString(commandLine);

    auto pCommand = cmdFactory.getCommandObjct(cmdLine[0]);

    if (pCommand == nullptr) return false;


    // check command buffer : count/status
    // execute or store
    // result : command count 
    // cmdBuffer.checkCommandBuffer();

    
    return pCommand->excuteCommand(commandLine, m_ssdOutputPath, m_ssdNandPath);

}
