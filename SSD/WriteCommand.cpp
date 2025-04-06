#include "WriteCommand.h"
#include "FilePathConfig.h"
#include <algorithm>

bool WriteCommand::excuteCommand(string commandLine)
{
    parseCmdLine(commandLine);

    writeLba();

    return true;
}

bool WriteCommand::checkValidCmd(size_t cmdParamCount)
{
	return (cmdParamCount > MAX_WRITE_PARAM_COUNT);
}

bool WriteCommand::checkVaildParameterAndStr2I(void)
{
    if (false == Command::checkVaildParameterAndStr2I()) return false;

    if (m_commandParameter.data == "") return false;

    if ((m_commandParameter.data.size() != MAX_DATA_LENGTH)
        || !((m_commandParameter.data[0] == '0') && (m_commandParameter.data[1] == 'x')))
    {
        return false;
    }
    string toData = m_commandParameter.data;
    toData.erase(0, 2);

    transform(toData.begin(), toData.end(), toData.begin(), ::toupper);

    for (char each : toData)
    {
        if (each > '9' || each < '0')
        {
            if (each > 'F' || each < 'A')
            {
                return false;
            }
        }
    }

    m_commandParameter.data = "0x" + toData;
    size_t pos;
    auto aData = stoul(toData, &pos, 16);

    m_commandParameter.nData = static_cast<uint32_t>(aData);;
    
    return true;
}

void WriteCommand::writeLba(void)
{
    ifstream fileIn(SSD_NAND_PATH);
    vector<string> lines;
    string line;

    while (getline(fileIn, line)) {
        lines.push_back(line);
    }

    fileIn.close();

    lines[m_commandParameter.nLba] = (m_commandParameter.lba + " " + m_commandParameter.data);

    ofstream fileOut(SSD_NAND_PATH);
    for (const auto& l : lines) {
        fileOut << l << "\n";
    }
    fileOut.close();
}
