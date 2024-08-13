#pragma once

#include <string>

class Process
{
public:

	static void CreateProc(unsigned int& processId, const std::string& fullPath, const std::string& optionalWorkingDirectory);

	static bool IsApplicationRunning(unsigned int processId);

	static bool CloseApplicationRunning(unsigned int processId);

	
#ifndef _WIN32

	
#endif
};

