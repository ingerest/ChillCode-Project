#include "CommandBuffer.h"
#include "Command.h"
#include "CommandFactory.h"
#include <filesystem>
#include <algorithm>

using namespace std;

namespace fs = std::filesystem;

const string COMMAND_BUFFER_PATH = "./buffer/";

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
	string commandBufferPath = m_folderPath;
	commandBufferPath.pop_back();

	if (!fs::exists(commandBufferPath))
	{
		fs::create_directories(commandBufferPath);
		for (string defaultFilename : m_dafaultFileName)
		{
			string filePath = m_folderPath + defaultFilename;
			ofstream file(filePath);
			file.close();
		}
	}


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
				vector<string>vCommand = splitString(fileName, '_');
				
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
				m_validCommandCount++;
			}
		}
	}

	sort(m_currentFileName.begin(), m_currentFileName.end());

}

void CommandBuffer::reorderCommandBufferAndStore(void)
{
	vector<string> changedFileName;
	changedFileName = m_currentFileName;
	
	string lastCommand = m_currentFileName[m_validCommandCount - 1];
	
	lastCommand.erase(0, 2);

	vector<string>vCommand = splitString(lastCommand, '_');

	if (vCommand[0] == "W")
	{
		uint32_t nLba = stoi(vCommand[1]);
		if (m_writeBitmap.getBit(nLba) == 1)
		{	
			uint32_t validCommandCount = m_validCommandCount - 1;
			for (uint32_t index = 0; index < validCommandCount; index++)
			{
				vector<string>targetCommand = splitString(changedFileName[index], '_');
				if ((targetCommand[1] == "W") && (targetCommand[2] == vCommand[1]))
				{
					changedFileName.erase(changedFileName.begin() + index);
					changedFileName.insert(changedFileName.begin() + (m_validCommandCount - 1), m_dafaultFileName[m_validCommandCount - 1]);
					m_validCommandCount--;
					break;
				}
			}
		}		
	}
	else if (vCommand[0] == "E")
	{
		uint32_t nStartLba = stoi(vCommand[1]);
		uint32_t nLength = stoi(vCommand[2]);
		uint32_t nEndLba = nStartLba + nLength - 1;
		uint32_t indexLba = nStartLba;


		for (; indexLba <= nEndLba; indexLba++)
		{
			if (m_writeBitmap.getBit(indexLba) == 1)
			{
				uint32_t validCommandCount = m_validCommandCount - 1;
				for (uint32_t index = 0; index < validCommandCount; index++)
				{
					vector<string>targetCommand = splitString(changedFileName[index], '_');
					if ((targetCommand[1] == "W") && (targetCommand[2] == to_string(indexLba)))
					{
						changedFileName.erase(changedFileName.begin() + index);
						changedFileName.insert(changedFileName.begin() + (m_validCommandCount - 1), m_dafaultFileName[m_validCommandCount - 1]);
						m_validCommandCount--;
						break;
					}
				}
			}
		}


		for (int32_t index = m_validCommandCount - 2 ; index >=  0; index--)
		{
			vector<string>targetCommand = splitString(changedFileName[index], '_');
			if (targetCommand[1] == "E")
			{
				uint32_t nTagetStartLba = stoi(targetCommand[2]);
				uint32_t nTargetLenth = stoi(targetCommand[3]);
				uint32_t nTargetEndLba = nTagetStartLba + nTargetLenth - 1;

				if ((nTagetStartLba == nEndLba + 1) || (nTargetEndLba + 1 == nStartLba))
				{
					if ((nTargetLenth + nLength) <= 10)
					{
						uint32_t	nNewStartLba = (nTagetStartLba > nStartLba) ? nStartLba : nTagetStartLba;
						uint32_t nNewLenth = nTargetLenth + nLength;
						changedFileName.erase(changedFileName.begin() + (m_validCommandCount - 1));
						changedFileName.insert(changedFileName.begin() + (m_validCommandCount - 1), m_dafaultFileName[m_validCommandCount - 1]);
						changedFileName.erase(changedFileName.begin() + index);

						string strNewFile = to_string(m_validCommandCount - 1) + "_" + targetCommand[1] + "_" + to_string(nNewStartLba) + "_" + to_string(nNewLenth);
						changedFileName.insert(changedFileName.begin() + (m_validCommandCount - 2), strNewFile);
						m_validCommandCount--;
						break;
					}
					else continue;
				}
				else if ((nTagetStartLba > nEndLba) || (nStartLba > nTargetEndLba))
				{
					continue;
				}
				else
				{
					uint32_t	nNewStartLba = (nTagetStartLba > nStartLba) ? nStartLba : nTagetStartLba;
					uint32_t nNewEndLba = (nTargetEndLba > nEndLba) ? nTargetEndLba : nEndLba;
					
					if ((nNewEndLba - nNewStartLba + 1) <= 10)
					{
						uint32_t nNewLenth = (nNewEndLba - nNewStartLba + 1);
						changedFileName.erase(changedFileName.begin() + m_validCommandCount - 1);
						changedFileName.insert(changedFileName.begin() + m_validCommandCount - 1, m_dafaultFileName[m_validCommandCount - 1]);
						changedFileName.erase(changedFileName.begin() + index);

						string strNewFile = to_string(m_validCommandCount - 1) + "_" + targetCommand[1] + "_" + to_string(nNewStartLba) + "_" + to_string(nNewLenth);
						changedFileName.insert(changedFileName.begin() + m_validCommandCount - 2, strNewFile);
						m_validCommandCount--; 
						break;
					}
					else continue;
				}						
			}
		}
	}

	for (uint32_t index = 0; index < m_validCommandCount; index++)
	{
		vector<string>targetCommand = splitString(changedFileName[index], '_');

		if ((index + 1) != stoi(targetCommand[0]))
		{
			changedFileName[index].erase(0, 1);
			changedFileName[index] = to_string(index + 1) + changedFileName[index];
		}
	}

	for (uint32_t index = 0; index < MAX_COMMAND_BUFFER_SIZE; index++)
	{
		if (!fs::exists(COMMAND_BUFFER_PATH + m_currentFileName[index])) 
		{
			cerr << "오류: 변경하려는 파일이 존재하지 않습니다.\n";
			return;
		}
		if (m_currentFileName[index] != changedFileName[index])
			fs::rename(COMMAND_BUFFER_PATH + m_currentFileName[index], COMMAND_BUFFER_PATH + changedFileName[index]);
	}

}

void CommandBuffer::resetCommandBuffer(void)
{
	for (uint32_t index = 0; index < m_validCommandCount; index++)
	{
		if (!fs::exists(COMMAND_BUFFER_PATH + m_currentFileName[index]))
		{
			cerr << "오류: 변경하려는 파일이 존재하지 않습니다.\n";
			return;
		}
		fs::rename(COMMAND_BUFFER_PATH + m_currentFileName[index], COMMAND_BUFFER_PATH + m_dafaultFileName[index]);
	}
	m_currentFileName = m_dafaultFileName;
	m_validCommandCount = 0;
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

			if ((vCommand[1] == "W") && (vCommand[2] == to_string(nLBA)))
			{
				sData = vCommand[3];
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
	for (uint32_t index = 0; index < m_validCommandCount; index++)
	{
		string cmdLine = m_currentFileName[index];
		
		cmdLine.erase(0, 2);

		string commandType = cmdLine.substr(0, 1);
		replace(cmdLine.begin(), cmdLine.end(), '_', ' ');
		CommandFactory::getInstance()->getCommandObjct(commandType)->excuteCommand(cmdLine, "../Release/ssd_output.txt", "ssd_nand.txt");
	}

	resetCommandBuffer();
}

void CommandBuffer::addCommandToBuffer(vector<string> cmdLine)
{
	string oldFileName = m_currentFileName[m_validCommandCount];
	
	m_currentFileName[m_validCommandCount] 
		= to_string(m_validCommandCount + 1) + "_" + cmdLine[0] + "_" + cmdLine[1] + "_" + cmdLine[2];
	fs::rename(COMMAND_BUFFER_PATH + oldFileName, COMMAND_BUFFER_PATH + m_currentFileName[m_validCommandCount]);

	m_validCommandCount++;

	reorderCommandBufferAndStore();
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
