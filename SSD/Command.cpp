
#include "Command.h"
#include "FilePathConfig.h"

using namespace std;

vector<string>splitString(const string& str)
{
    vector<string> result;
    istringstream iss(str);
    string word;

    while (iss >> word)
    {
        result.push_back(word);
    }

    return result;
}

bool Command::updateOutputFile(string data)
{
    ofstream ouputFile(SSD_OUTPUT_PATH);
    ouputFile << data;
    ouputFile.close();
    return false;
 }

bool Command::updateErrorMsg2TextFile(void)
{
    ofstream ouputFile(SSD_OUTPUT_PATH);
    ouputFile << "ERROR";
    ouputFile.close();
    return false;
}

bool Command::checkVaildParameter(string commandLine)
{
    if (false == parseCmdLineAndCheckValidCmd(commandLine)) return false;

    if (false == checkVaildParameterAndStr2I()) return false;

    return true;
}

bool Command::parseCmdLineAndCheckValidCmd(string commandLine)
{
    return checkValidCmd(parseCmdLine(commandLine));
}

size_t Command::parseCmdLine(string commandLine)
{
    vector<string> cmdLine = splitString(commandLine);

    m_commandParameter.lba = cmdLine[1];
	if (true == isNeedData())
	{
		m_commandParameter.data = cmdLine[2];
	}
	return cmdLine.size();
}

#if 0
bool Command::checkValidCmd(size_t cmdParamCount)
{
    return false;
}
#endif

bool Command::checkVaildParameterAndStr2I(void)
{
    for (char each : m_commandParameter.lba)
    {
        if (each == '-') return updateErrorMsg2TextFile();
        if (each > '9' || each < '0') return false;
    }

    if ((stoi(m_commandParameter.lba) > MAX_LBA) || (stoi(m_commandParameter.lba) < MIN_LBA))
    {
        return updateErrorMsg2TextFile();
    }

    m_commandParameter.nLba = stoi(m_commandParameter.lba);

    return true;
}
