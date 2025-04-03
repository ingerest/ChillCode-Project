#pragma once

#include "Command.h"

using namespace std;

class CommandBuffer
{
public:
	CommandBuffer();
	void checkCommandBuffer(void);
private:
	void getFileList(void);

	vector<string> m_dafaultFileName
		= { "1_empty", "2_empty", "3_empty" , "4_empty" , "5_empty" };
	vector<string> m_currentFileName;
	string m_folderPath;
	uint32_t m_validCommandCount;

};