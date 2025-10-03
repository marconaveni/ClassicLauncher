#include "Platform.h"

#ifdef _WIN32

#include <windows.h>
#include <filesystem>

namespace ClassicLauncher::Platform
{

    std::wstring utf8_to_wstring(const std::string& str)
    {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), wstrTo.data(), size_needed);
        return wstrTo;
    }

    std::string wstring_to_utf8(const std::wstring& wstr)
    {
        const int wStrSize = static_cast<int>(wstr.size());
        int isSizeNeed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wStrSize, NULL, 0, NULL, NULL);

        std::string strTo(isSizeNeed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wStrSize, strTo.data(), isSizeNeed, NULL, NULL);
        return strTo;
    }

    void CreateProc(unsigned int& processId,
                    const std::string& fullPath,
                    const std::string& optionalWorkingDirectory,
                    int& status)
    {
        std::wstring path = utf8_to_wstring(fullPath);
        std::wstring dir = utf8_to_wstring(optionalWorkingDirectory);

        STARTUPINFOW info = {sizeof(info)};
        PROCESS_INFORMATION processInfo;

        if (CreateProcessW(nullptr,
                           path.data(),
                           nullptr,
                           nullptr,
                           TRUE,
                           0,
                           nullptr,
                           dir.empty() ? nullptr : dir.data(),
                           &info,
                           &processInfo))
        {
            printf("open:");

            //WaitForSingleObject(processInfo.hProcess, INFINITE);

            processId = static_cast<unsigned int>(processInfo.dwProcessId);

            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
            status = 1;
        }
        else
        {
            printf("Error on create a process: %lu\n", GetLastError());
            processId = 0;
            status = -1;
        }
    }

    bool IsApplicationRunning(const unsigned int processId)
    {
        bool bApplicationRunning = true;
        HANDLE ProcessHandle = OpenProcess(SYNCHRONIZE, false, processId);
        if (ProcessHandle == NULL)
        {
            bApplicationRunning = false;
        }
        else
        {
            unsigned int WaitResult = WaitForSingleObject(ProcessHandle, 1000);
            if (WaitResult != WAIT_TIMEOUT)
            {
                bApplicationRunning = false;
            }
            CloseHandle(ProcessHandle);
        }
        return bApplicationRunning;
    }

    bool CloseApplicationRunning(const unsigned int processId)
    {
        bool bApplicationRunning;
        HANDLE ProcessHandle = OpenProcess(PROCESS_TERMINATE, false, processId);
        if (ProcessHandle == NULL)
        {
            return false;
        }
        else
        {
            bApplicationRunning = TerminateProcess(ProcessHandle, 0);
            CloseHandle(ProcessHandle);
        }
        return bApplicationRunning;
    }

    std::string GetExecutableDirectory()
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::filesystem::path exePath(buffer);
        return exePath.parent_path().string();
    }

    std::string GetWorkingDirectory()
    {
        return std::filesystem::current_path().string();
    }

} // namespace ClassicLauncher::Platform

#else

#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <filesystem>
#include <vector>

#include "Log.h"
#include "String.h"

namespace ClassicLauncher::Platform
{

    void CreateProc(int& processId, const std::string& fullPath)
    {
        std::vector<std::string> paths = String::SplitString(fullPath);

        for (auto& path : paths)
        {
            String::ReplaceString(path, "\"", "");
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            LOG(LOG_CLASSIC_ERROR, "Failed to created child process.");
            return;
        }

        if (pid == 0) // Child Process
        {
            std::vector<char*> args;
            for (const auto& arg : paths)
            {
                args.push_back(const_cast<char*>(arg.c_str()));
            }
            args.push_back(nullptr);

            if (execvp(args[0], args.data()) == -1)
            {
                LOG(LOG_CLASSIC_ERROR, "Failed to execute program.");
                _exit(1); // execvp failed
            }
            LOG(LOG_CLASSIC_ERROR, "process child. %d", pid);
            _exit(0); // exit the child process
        }
        else
        {
            processId = pid;
        }
    }

    bool IsApplicationRunning(const int processId)
    {
        if (processId == 0)
            return false;

        bool bApplicationRunning = false;
        int status = 0;

        pid_t result = waitpid(processId, &status, WNOHANG); // process is running?

        if (result == 0)
        {
            bApplicationRunning = true;
            LOG(LOG_CLASSIC_TRACE, "The child process is running...");
        }
        else if (result == processId)
        {
            if (WIFEXITED(status))
            {
                LOG(LOG_CLASSIC_DEBUG, "The child process terminated with status: %d ", WEXITSTATUS(status));
            }
            else
            {
                LOG(LOG_CLASSIC_ERROR, "The child process terminated with error.");
            }
            bApplicationRunning = false;
        }
        else
        {
            bApplicationRunning = false;
        }

        return bApplicationRunning;
    }

    bool CloseApplicationRunning(const int processId)
    {
        return (processId != 0) && (kill(processId, SIGTERM) == 0);
    }

    std::string GetExecutableDirectory()
    {
        char buffer[1024];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
        buffer[len] = '\0';
        std::filesystem::path exePath(buffer);
        return exePath.parent_path().string();
    }

    std::string GetWorkingDirectory()
    {
        return std::filesystem::current_path().string();
    }

} // namespace ClassicLauncher::Platform

#endif //_WIN32