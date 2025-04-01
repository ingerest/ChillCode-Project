#include <iostream>
#include <string>

using namespace std;


class ISsdApi
{
public:
	virtual bool excuteCommand(std::string commandLine) = 0;
};
