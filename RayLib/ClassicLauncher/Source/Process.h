#pragma once

#include <string>

class Process
{
public:

	static void CreateProc(unsigned int& processId, std::string fullPath, std::string optionalWorkingDirectory);

	static bool IsApplicationRunning(unsigned int processId);

	static bool CloseApplicationRunning(unsigned int processId);
};

