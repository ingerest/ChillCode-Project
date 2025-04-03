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
				m_validCommandCount++;
			}
		}
	}
}

void CommandBuffer::reorderCommandBuffer(void)
{
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
		sData = "0xFFFFFFFF";
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

void CommandBuffer::addCommandToBuffer(Command* pCommand)
{
	// add 
	reorderCommandBuffer();
}
