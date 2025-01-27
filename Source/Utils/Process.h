#ifndef PROCESS_H
#define PROCESS_H

#include <string>

namespace ClassicLauncher::Process
{

#ifdef _WIN32
    void CreateProc(unsigned int& processId, const std::string& fullPath, const std::string& optionalWorkingDirectory, int& status);
    bool IsApplicationRunning(unsigned int processId);
    bool CloseApplicationRunning(unsigned int processId);
#else
    void CreateProc(int& processId, const std::string& fullPath);
    bool IsApplicationRunning(int processId);
    bool CloseApplicationRunning(int processId);
#endif

}  // namespace ClassicLauncher::Process

#endif  // PROCESS_H
