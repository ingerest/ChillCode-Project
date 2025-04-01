#include "Ssd.h"
#include <fstream>
#include <sstream>


const std::string SSD_NAND_PATH = "ssd_nand.txt";
const std::string SSD_OUTPUT_PATH = "../Release/ssd_output.txt";

Ssd::Ssd()
{
	m_ssdNandPath = SSD_NAND_PATH;
	m_ssdOutputPath = SSD_OUTPUT_PATH;
};

bool Ssd::excuteCommand(std::string commandLine)
{
	std::ofstream nandFile(m_ssdNandPath);
	std::ofstream outputFile(SSD_OUTPUT_PATH);

	std::vector<std::string> cmdLine = splitString(commandLine);

    
    if (cmdLine[1] == wCommand.cmdName)
    {
        if (cmdLine.size() > MAX_WRITE_PARAM_COUNT)  return false;
        return wCommand.excuteCommand(commandLine);
    }
    else if (cmdLine[1] == rCommand.cmdName)
    {
        if (cmdLine.size() > MAX_READ_PARAM_COUNT)  return false;
        return  rCommand.excuteCommand(commandLine);
    }
    else
    {
        return false;
    }
}


std::vector<std::string> Ssd::splitString(const std::string& str)
{
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string word;

    while (iss >> word)
    {
        result.push_back(word);
    }
}


bool ReadCommand::excuteCommand(std::string commandLine)
{
    return true;
}

bool WriteCommand::excuteCommand(std::string commandLine)
{
    return true;
}