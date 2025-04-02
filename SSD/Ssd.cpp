#include "Ssd.h"
#include <algorithm>


const string SSD_NAND_PATH = "ssd_nand.txt";
const string SSD_OUTPUT_PATH = "../Release/ssd_output.txt";

CommandFactory cmdFactory;

Ssd::Ssd()
{
	m_ssdNandPath = SSD_NAND_PATH;
	m_ssdOutputPath = SSD_OUTPUT_PATH;
};

bool Ssd::excuteCommand(string commandLine)
{
    if (commandLine == "") false;

	std::vector<string> cmdLine = splitString(commandLine);

    auto pCommand = cmdFactory.getCommandObjct(cmdLine[0]);

    if (pCommand == nullptr) return false;
    
    return pCommand->excuteCommand(commandLine, m_ssdOutputPath, m_ssdNandPath);

}
