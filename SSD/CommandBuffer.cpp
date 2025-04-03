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

void CommandBuffer::checkCommandBuffer(void)
{
	getFileList();
}
