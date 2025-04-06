#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "ISsdApi.h"
#include "Command.h"
#include "FilePathConfig.h"

using namespace std;

class Ssd : public ISsdApi
{
public:
	Ssd();

	bool excuteCommand(string commandLine);
private:
	string m_ssdNandPath;
	string m_ssdOutputPath;

	
};

