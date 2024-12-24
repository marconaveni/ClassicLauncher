#ifndef PROCESS_H
#define PROCESS_H

#include <string>

namespace ClassicLauncher
{

    class Process
    {
    public:

        static void CreateProc(unsigned int& processId, const std::string& fullPath, const std::string& optionalWorkingDirectory);
        static bool IsApplicationRunning(unsigned int processId);
        static bool CloseApplicationRunning(unsigned int processId);
    };

}  // namespace ClassicLauncher

#endif  // PROCESS_H

#include <string>
