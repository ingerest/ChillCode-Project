#pragma once

#include "Command.h"
#include "Bitmap128.h"

using namespace std;

class CommandBuffer
{
public:
	CommandBuffer();
	void checkCommandBuffer(void);
	bool checkCacheHit(Command* pCommand);
	bool isFullCommandBuffer(void);
	void triggerCommandProcessing(void);
	void addCommandToBuffer(vector<string> cmdLine);
private:
	void getFileList(void);
	void reorderCommandBufferAndStore(void);
	void resetCommandBuffer(void);
	vector<string> splitString(const string& str, char delimiter);

	vector<string> m_dafaultFileName
		= { "1_empty", "2_empty", "3_empty" , "4_empty" , "5_empty" };
	vector<string> m_currentFileName;
	string m_folderPath;
	uint32_t m_validCommandCount;
	Bitset128 m_eraseBitmap;
	Bitset128 m_writeBitmap;
		
	const uint32_t MAX_COMMAND_BUFFER_SIZE = 5;
};