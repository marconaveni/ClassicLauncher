#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>

namespace ClassicLauncher::Platform
{

#ifdef _WIN32
    #ifndef HOME_DIR
    #define HOME_DIR "USERPROFILE" // account dir windows ex: "C:\Users\User"
    #endif // HOME_DIR
#else
    #ifndef HOME_DIR
    #define HOME_DIR "HOME" // home dir linux ex: "/home/user"
    #endif // HOME_DIR
#endif

#ifdef _WIN32
    void CreateProc(unsigned int& processId, const std::string& fullPath, const std::string& optionalWorkingDirectory, int& status);
    bool IsApplicationRunning(unsigned int processId);
    bool CloseApplicationRunning(unsigned int processId);
#else
    void CreateProc(int& processId, const std::string& fullPath);
    bool IsApplicationRunning(int processId);
    bool CloseApplicationRunning(int processId);
#endif

    std::string GetExecutableDirectory();
    std::string GetWorkingDirectory();
    
}  // namespace ClassicLauncher::Platform

#endif  // PLATFORM_H
