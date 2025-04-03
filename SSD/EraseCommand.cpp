#include "EraseCommand.h"
#include <algorithm>

using namespace std;

bool EraseCommand::parseCommandLine(string commandLine)
{
    vector<string> cmdLine = splitString(commandLine);

    if (cmdLine.size() > MAX_ERASE_PARAM_COUNT)  return false;
    m_commandParameter.lba = cmdLine[1];
    m_commandParameter.data = cmdLine[2];

    return true;
}

bool EraseCommand::checkVaildParameterAndStr2I(void)
{
    for (char each : m_commandParameter.lba)
    {
        if (each == '-') return updateErrorMsg2TextFile();
        if (each > '9' || each < '0') return false;
    }

    if ((stoi(m_commandParameter.lba) > 99) || (stoi(m_commandParameter.lba) < 0))
    {
        return updateErrorMsg2TextFile();
    }

    if (m_commandParameter.data != "")
    {
        string toData = m_commandParameter.data;
        int32_t nData = stoi(toData);

        if (toData.size() > 2) return false;

        if (nData == 0) return false;
        
        for (char each : toData)
        {
            if (each == '-') return updateErrorMsg2TextFile();
            if (each > '9' || each < '0') return false;
        }

        if((nData > 10) || (nData < 0)) return updateErrorMsg2TextFile();
        
        m_commandParameter.nData = static_cast<uint32_t>(nData);
    }
    m_commandParameter.nLba = stoi(m_commandParameter.lba);

    return true;
}

bool EraseCommand::excuteCommand(string commandLine, string OutputFile, string WriteFile)
{
    if (false == parseCommandLine(commandLine)) return false;

    m_commandParameter.OutputFile = OutputFile;
    m_commandParameter.WriteFile = WriteFile;

    if (false == checkVaildParameterAndStr2I()) return false;

    eraseLba();

    return true;
}

void EraseCommand::eraseLba(void)
{
    ifstream fileIn(m_commandParameter.WriteFile);
    vector<string> lines;
    string line;
    uint32_t index = 0;

    while (getline(fileIn, line)) {
        lines.push_back(line);
    }

    fileIn.close();

    for (index = m_commandParameter.nLba; index < (m_commandParameter.nLba + m_commandParameter.nData); index++)
    {
        lines[index] = ( to_string(index) + " " + ERASE_PATTERN);
    }
    ofstream fileOut(m_commandParameter.WriteFile);
    for (const auto& l : lines) {
        fileOut << l << "\n";
    }
    fileOut.close();
}
