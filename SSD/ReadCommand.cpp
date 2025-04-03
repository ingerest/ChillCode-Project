#include "ReadCommand.h"


void ReadCommand::readLba(void)
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

    ofstream ouputFile(m_commandParameter.OutputFile);
    ouputFile << lbaData[1];
    ouputFile.close();
}


bool ReadCommand::parseCommandLine(string commandLine)
{
    vector<string> cmdLine = splitString(commandLine);

    if (cmdLine.size() > MAX_READ_PARAM_COUNT)  return false;
    m_commandParameter.lba = cmdLine[1];
    m_commandParameter.data = "";

    return true;
}


bool ReadCommand::excuteCommand(string commandLine, string OutputFile, string WriteFile)
{    
    readLba();

    return true;
}

bool ReadCommand::checkVaildParameter(string commandLine, string OutputFile, string WriteFile)
{
    if (false == parseCommandLine(commandLine)) return false;

    m_commandParameter.OutputFile = OutputFile;
    m_commandParameter.WriteFile = WriteFile;

    if (false == checkVaildParameterAndStr2I()) return false;
    return true;
}

