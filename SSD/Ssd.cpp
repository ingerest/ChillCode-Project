#include "Ssd.h"


const string SSD_NAND_PATH = "ssd_nand.txt";
const string SSD_OUTPUT_PATH = "../Release/ssd_output.txt";

Ssd::Ssd()
{
	m_ssdNandPath = SSD_NAND_PATH;
	m_ssdOutputPath = SSD_OUTPUT_PATH;
};

bool Ssd::excuteCommand(string commandLine)
{
	std::vector<string> cmdLine = splitString(commandLine);
        
    if (cmdLine[0] == wCommand.cmdName)
    {
        if (cmdLine.size() > MAX_WRITE_PARAM_COUNT)  return false;
        return wCommand.excuteCommand(commandLine, m_ssdOutputPath, m_ssdNandPath);
    }
    else if (cmdLine[0] == rCommand.cmdName)
    {
        if (cmdLine.size() > MAX_READ_PARAM_COUNT)  return false;
        return  rCommand.excuteCommand(commandLine, m_ssdOutputPath, m_ssdNandPath);
    }
    else
    {
        return false;
    }
}


std::vector<string>splitString(const string& str)
{
    std::vector<string> result;
    std::istringstream iss(str);
    string word;

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
        std::ofstream ouputFile(m_commandParameter.OutputFile);
        ouputFile << "ERROR";
        ouputFile.close();
        return false;
    }

    if (m_commandParameter.data != "")
    {
        if ((m_commandParameter.data.size() != MAX_DATA_LENGTH)
            || !((m_commandParameter.data[0] == '0') && (m_commandParameter.data[1] == 'x')))
        {
            return false;
        }
        string toData = m_commandParameter.data;
        toData.erase(0, 2);
        for (char each : toData)
        {
            if (each > '9' || each < '0')
            {
                if (each > 'f' || each < 'a')
                {
                    return false;
                }
            }
        }
    }
    if (m_commandParameter.data != "")
    {
        string toData = m_commandParameter.data;
        toData.erase(0, 2);
        size_t pos;

        auto aa = stoul(toData,&pos,16);
        m_commandParameter.nData = static_cast<uint32_t>(aa);
    }
    m_commandParameter.nLba = stoi(m_commandParameter.lba);

    return true;
}


void Command::readLBA(void)
{
    string tempStr;
    uint32_t index = 0;
    ifstream nandFile(m_commandParameter.WriteFile);
    vector<string> lbaData;
    while (1)
    {
        getline(nandFile, tempStr);
        if (index == m_commandParameter.nLba)
        {
            lbaData = splitString(tempStr);
           if (lbaData[0] == m_commandParameter.lba) break;
           else
           {
               cerr << "Invalid line number." << endl;
               break;
           }
        }
        index++;
    }

    std::ofstream ouputFile(m_commandParameter.OutputFile);    
    ouputFile << lbaData[1];
    ouputFile.close();
}

void ReadCommand::parseCommandLine(string commandLine)
{
    std::vector<string> cmdLine = splitString(commandLine);
    m_commandParameter.lba = cmdLine[1];
    m_commandParameter.data = "";
}

void WriteCommand::parseCommandLine(string commandLine)
{
    std::vector<string> cmdLine = splitString(commandLine);
    m_commandParameter.lba = cmdLine[1];
    m_commandParameter.data = cmdLine[2];
}


bool ReadCommand::excuteCommand(string commandLine, string OutputFile, string WriteFile)
{
    string readData;
    parseCommandLine(commandLine);
    m_commandParameter.OutputFile = OutputFile;
    m_commandParameter.WriteFile = WriteFile;
    if (false == checkVaildParameterAndStr2I()) return false; 
    
    readLBA();
    

    return true;
}

bool WriteCommand::excuteCommand(string commandLine, string OutputFile, string WriteFile)
{
    parseCommandLine(commandLine);
    m_commandParameter.OutputFile = OutputFile;
    m_commandParameter.WriteFile = WriteFile;
    if (false == checkVaildParameterAndStr2I()) return false;

    writeLba();

    return true;
}

void WriteCommand::writeLba(void)
{
    ifstream fileIn(m_commandParameter.WriteFile);
    vector<string> lines;
    string line;

    while (getline(fileIn, line)) {
        lines.push_back(line);
    }

    fileIn.close();

    lines[m_commandParameter.nLba] =  (m_commandParameter.lba + " " + m_commandParameter.data);

    std::ofstream fileOut(m_commandParameter.WriteFile);
    for (const auto& l : lines) {
        fileOut << l << "\n";
    }
    fileOut.close();
}