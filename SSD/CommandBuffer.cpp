#include "CommandBuffer.h"
#include <filesystem>

using namespace std;

namespace fs = std::filesystem;

const string COMMAND_BUFFER_PATH = "./buffer";

CommandBuffer::CommandBuffer()
{
	m_folderPath = COMMAND_BUFFER_PATH;
	m_validCommandCount = 0;
	m_currentFileName.clear();
	m_eraseBitmap.clearAll();
	m_writeBitmap.clearAll();
}

void CommandBuffer::getFileList(void)
{
	m_currentFileName.clear();
	m_validCommandCount = 0;
	for (const auto& entry : fs::directory_iterator(m_folderPath))
	{
		if (entry.is_regular_file())
		{
			string fileName = entry.path().filename().string();
			m_currentFileName.push_back(fileName);
			if (fileName.erase(0,1) != "_empty")
			{
				vector<string>vCommand = splitString(m_currentFileName[m_validCommandCount], '_');
				
				if (vCommand[1] == "E")
				{
					uint32_t nLba = stoi(vCommand[2]);
					uint32_t nEndLba = nLba + stoi(vCommand[3]);
					for (; nLba < nEndLba; nLba++)
					{
						m_eraseBitmap.setBit(nLba);
					}
				}
				else if (vCommand[1] == "W")
				{
					m_writeBitmap.setBit(stoi(vCommand[2]));
				}
				else
				{
					cout << "Error: Invalid command type" << endl;
					return;
				}
				m_validCommandCount++;
			}
		}
	}
}

void CommandBuffer::reorderCommandBuffer(void)
{
	for (uint32_t index = 0; index < m_validCommandCount; index++)
	{
	}
}


void CommandBuffer::checkCommandBuffer(void)
{
	getFileList();
}

bool CommandBuffer::checkCacheHit(Command* pCommand)
{
	if (pCommand == nullptr) return false;

	uint32_t nLBA = pCommand->getLba();
	string sData;

	if (m_eraseBitmap.getBit(nLBA) == 1)
	{
		sData = "0x00000000";
	}
	else if (m_writeBitmap.getBit(nLBA) == 1)
	{
		for (uint32_t index = 0; index < m_validCommandCount; index++)
		{
			vector<string>vCommand = splitString(m_currentFileName[index], '_');

			if ((vCommand[1] == "W") && (vCommand[4] == string("0x" + to_string(nLBA))))
			{
				sData = vCommand[4];
				break;
			}
		}
	}
	else
	{
		return false;
	}
	pCommand->updateOutputFile(sData);

	return true;
}

bool CommandBuffer::isFullCommandBuffer(void)
{
	return (m_validCommandCount == MAX_COMMAND_BUFFER_SIZE);
}

void CommandBuffer::triggerCommandProcessing(void)
{

}

void CommandBuffer::addCommandToBuffer(vector<string> cmdLine)
{
	m_currentFileName[m_validCommandCount] 
		= to_string(m_validCommandCount) + "_" + cmdLine[0] + "_" + cmdLine[1] + "_" + cmdLine[2];
		m_validCommandCount++;
	reorderCommandBuffer();
}


vector<string> CommandBuffer::splitString(const string& str, char delimiter) 
{
	vector<string> result;
	stringstream ss(str);
	string token;

	while (getline(ss, token, delimiter)) {
		result.push_back(token);
	}
	return result;
}
