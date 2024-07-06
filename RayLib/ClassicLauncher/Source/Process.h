#pragma once

#include <string>

class Process
{
	static void CreateProc(unsigned int& ProcessId, std::string FullPath, bool Hidden, std::string OptionalWorkingDirectory);

	static bool IsApplicationRunning(unsigned int ProcessId);

	static bool CloseApplicationRunning(unsigned int ProcessId);
};

