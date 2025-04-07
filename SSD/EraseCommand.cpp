#include "EraseCommand.h"
#include "FilePathConfig.h"
#include <algorithm>

using namespace std;


bool EraseCommand::checkVaildParameterAndStr2I(void)
{
    if (false == Command::checkVaildParameterAndStr2I()) return false;

    if (m_commandParameter.data == "") return false;    
    
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

    return true;
}

bool EraseCommand::excuteCommand(string commandLine)
{
    parseCmdLine(commandLine);
    eraseLba();

    return true;
}

void EraseCommand::eraseLba(void)
{
    ifstream fileIn(SSD_NAND_PATH);
    vector<string> lines;
    string line;
    uint32_t index = 0;

    while (getline(fileIn, line)) {
        lines.push_back(line);
    }

    fileIn.close();

    m_commandParameter.nLba = stoi(m_commandParameter.lba);
    m_commandParameter.nData = stoi(m_commandParameter.data);

    for (index = m_commandParameter.nLba; index < (m_commandParameter.nLba + m_commandParameter.nData); index++)
    {
        lines[index] = ( to_string(index) + " " + ERASE_PATTERN);
    }
    ofstream fileOut(SSD_NAND_PATH);
    for (const auto& l : lines) {
        fileOut << l << "\n";
    }
    fileOut.close();
}


bool EraseCommand::checkValidCmd(size_t cmdParamCount)
{
    return (cmdParamCount <= MAX_ERASE_PARAM_COUNT);
}
