#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "ISsdApi.h"

using namespace std;


struct CommandParameter
{
	std::string lba;
	std::string data;
};

class Command
{
public:
	string cmdName;
	virtual bool excuteCommand(std::string commandLine) = 0;
	bool checkVaildParameter();	
private:
	CommandParameter m_commandParameter;
};



class ReadCommand : public Command
{
public:
	string cmdName = "R";
	bool excuteCommand(std::string commandLine);
};

class WriteCommand : public Command
{
public:
	string cmdName = "W";
	bool excuteCommand(std::string commandLine);
};


class Ssd : public ISsdApi
{
public:
	Ssd()
	{

	};

	bool excuteCommand(std::string commandLine);
private:
	const uint32_t MAX_READ_PARAM_COUNT = 3;
	const uint32_t MAX_WRITE_PARAM_COUNT = 4;
	WriteCommand wCommand;
	ReadCommand rCommand;
	std::string m_ssdNandPath;
	std::string m_ssdOutputPath;

	std::vector<std::string> splitString(const std::string& str);
};
