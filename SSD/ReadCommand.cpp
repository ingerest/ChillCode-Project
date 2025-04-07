#include "ReadCommand.h"
#include "FilePathConfig.h"


void ReadCommand::readLba(void)
{
    string tempStr;
    uint32_t index = 0;
    ifstream nandFile(SSD_NAND_PATH);
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

    ofstream ouputFile(SSD_OUTPUT_PATH);
    ouputFile << lbaData[1];
    ouputFile.close();
}

bool ReadCommand::checkValidCmd(size_t cmdParamCount)
{
    return (cmdParamCount <= MAX_READ_PARAM_COUNT);
}

bool ReadCommand::excuteCommand(string commandLine)
{
    readLba();

    return true;
}

