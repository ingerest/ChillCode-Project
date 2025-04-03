
#include "Command.h"
#include <algorithm>

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

bool Command::updateErrorMsg2TextFile(void)
{
    ofstream ouputFile(m_commandParameter.OutputFile);
    ouputFile << "ERROR";
    ouputFile.close();
    return false;
 }

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

    if (m_commandParameter.data != "")
    {
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

        auto aa = stoul(toData, &pos, 16);
        m_commandParameter.nData = static_cast<uint32_t>(aa);
    }
    m_commandParameter.nLba = stoi(m_commandParameter.lba);

    return true;
}
