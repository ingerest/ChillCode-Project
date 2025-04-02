#include "Ssd.h"


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
        return wCommand.excuteCommand(commandLine, &outputFile, &nandFile);
    }
    else if (cmdLine[1] == rCommand.cmdName)
    {
        if (cmdLine.size() > MAX_READ_PARAM_COUNT)  return false;
        return  rCommand.excuteCommand(commandLine, &outputFile);
    }
    else
    {
        return false;
    }
}


std::vector<std::string>splitString(const std::string& str)
{
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string word;

    while (iss >> word)
    {
        result.push_back(word);
    }

    return result;
}


bool Command::checkVaildParameterAndStr2I()
{
    for (char each : m_commandParameter.lba)
    {
        if (each > '9' || each < '0') return false;
    }

    if ((stoi(m_commandParameter.lba) > MAX_LBA) || (stoi(m_commandParameter.lba) < MIN_LBA))
    {
        // write error  m_commandParameter.outputFilePath
        return false;
    }

    if (cmdName == "W") 
    {
        if ((m_commandParameter.data.size() > 10)
            || !((m_commandParameter.data[0] == '0') && (m_commandParameter.data[1] == 'x')))
        {
            return false;
        }
    }

    
    for (char each : m_commandParameter.data)
    {
        if (each > '9' || each < '0')
        {
            if (each > 'f' || each < 'a')
            {
                return false;
            }
        }
    }

    m_commandParameter.nData = stoi(m_commandParameter.data.erase(0, 2));
    m_commandParameter.nLba = stoi(m_commandParameter.lba);

    return true;
}


void ReadCommand::parseCommandLine(string commandLine)
{
    std::vector<std::string> cmdLine = splitString(commandLine);
    m_commandParameter.lba = cmdLine[2];
}

void WriteCommand::parseCommandLine(string commandLine)
{
    std::vector<std::string> cmdLine = splitString(commandLine);
    m_commandParameter.lba = cmdLine[2];
    m_commandParameter.data = cmdLine[2];
}


bool ReadCommand::excuteCommand(string commandLine, ofstream* pOutput)
{
    parseCommandLine(commandLine);
    if (false == checkVaildParameterAndStr2I()) return false;

    return true;
}

bool WriteCommand::excuteCommand(string commandLine, ofstream* pOutput, ofstream* pWriteFileP)
{
    parseCommandLine(commandLine);
    if (false == checkVaildParameterAndStr2I()) return false;

    return true;
}