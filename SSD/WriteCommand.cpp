#include "WriteCommand.h"

bool WriteCommand::parseCommandLine(string commandLine)
{
    vector<string> cmdLine = splitString(commandLine);

    if (cmdLine.size() > MAX_WRITE_PARAM_COUNT)  return false;
    m_commandParameter.lba = cmdLine[1];
    m_commandParameter.data = cmdLine[2];

    return true;
}

bool WriteCommand::excuteCommand(string commandLine, string OutputFile, string WriteFile)
{
    if (false == parseCommandLine(commandLine)) return false;

    m_commandParameter.OutputFile = OutputFile;
    m_commandParameter.WriteFile = WriteFile;

    if (false == checkVaildParameterAndStr2I()) return false;

    writeLba();

    return true;
}

bool WriteCommand::checkVaildParameter(string commandLine, string OutputFile, string WriteFile)
{
    if (false == parseCommandLine(commandLine)) return false;

    m_commandParameter.OutputFile = OutputFile;
    m_commandParameter.WriteFile = WriteFile;

    if (false == checkVaildParameterAndStr2I()) return false;

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

    lines[m_commandParameter.nLba] = (m_commandParameter.lba + " " + m_commandParameter.data);

    ofstream fileOut(m_commandParameter.WriteFile);
    for (const auto& l : lines) {
        fileOut << l << "\n";
    }
    fileOut.close();
}
