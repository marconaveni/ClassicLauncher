#ifndef PROCESS_H
#define PROCESS_H

#include <string>

namespace ClassicLauncher::Process
{

    void CreateProc(unsigned int& processId, const std::string& fullPath, const std::string& optionalWorkingDirectory);
    bool IsApplicationRunning(unsigned int processId);
    bool CloseApplicationRunning(unsigned int processId);

}  // namespace ClassicLauncher::Process

#endif  // PROCESS_H

#include <string>
