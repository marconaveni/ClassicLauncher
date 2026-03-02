#include "Platform.h"

#ifdef _WIN32

#include <filesystem>
#include <windows.h>

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
                           CREATE_NEW_CONSOLE,
                           nullptr,
                           dir.empty() ? nullptr : dir.data(),
                           &info,
                           &processInfo))
        {
            printf("open:");

            // WaitForSingleObject(processInfo.hProcess, INFINITE);

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
        bool isApplicationRunning = true;
        HANDLE processHandle = OpenProcess(SYNCHRONIZE, false, processId);
        if (processHandle == NULL)
        {
            isApplicationRunning = false;
        }
        else
        {
            unsigned int waitResult = WaitForSingleObject(processHandle, 0);
            if (waitResult != WAIT_TIMEOUT)
            {
                isApplicationRunning = false;
            }
            CloseHandle(processHandle);
        }
        return isApplicationRunning;
    }

    bool CloseApplicationRunning(const unsigned int processId)
    {
        bool isApplicationRunning;
        HANDLE ProcessHandle = OpenProcess(PROCESS_TERMINATE, false, processId);
        if (ProcessHandle == NULL)
        {
            return false;
        }
        else
        {
            isApplicationRunning = TerminateProcess(ProcessHandle, 0);
            CloseHandle(ProcessHandle);
        }
        return isApplicationRunning;
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
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "Log.h"
#include "String.h"

namespace ClassicLauncher::Platform
{
 
    std::vector<char*> BuildArgvFromPath(const std::string& fullPath, std::vector<std::string>& paths)
    {
        paths = String::SplitString(fullPath);

        for (auto& path : paths)
        {
            String::ReplaceString(path, "\"", "");
        }
        std::vector<char*> args;
        args.reserve(paths.size() + 1);

        for (const auto& arg : paths)
        {
            args.push_back(const_cast<char*>(arg.c_str()));
        }
        args.push_back(nullptr);
        return args;
    }

    void CreateProc(int& processId, const std::string& fullPath, int& status)
    {
        std::vector<std::string> paths;
        std::vector<char*> args = BuildArgvFromPath(fullPath, paths);

        int pipefd[2];
        pipe(pipefd);

        // closes pipe automatically when executing
        fcntl(pipefd[1], F_SETFD, FD_CLOEXEC);

        pid_t pid = fork();

        if (pid == -1)
        {
            return;
        }

        if (pid == 0)
        {
            close(pipefd[0]); // son writes

            execvp(args[0], args.data());

            // It only gets here if exec fails.
            int err = errno;
            write(pipefd[1], &err, sizeof(err));
            _exit(1);
        }
        else
        {
            close(pipefd[1]); // dad reads

            int err;
            ssize_t n = read(pipefd[0], &err, sizeof(err)); // Commenting out fcntl causes the thread to get stuck here.
            close(pipefd[0]);

            if (n > 0)
            {
                LOG(LOG_CLASSIC_ERROR, "exec failed: %s", strerror(err));
                status = -1;
                processId = 0;
                return;
            }

            processId = pid;
            status = 1; // exec its works !!!!
        }
    }

    bool IsApplicationRunning(const int processId)
    {
        if (processId == 0)
        {
            return false;
        }

        bool isApplicationRunning = false;
        int status = 0;

        pid_t result = waitpid(processId, &status, WNOHANG); // process is running?

        if (result == 0)
        {
            isApplicationRunning = true;
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
            isApplicationRunning = false;
        }
        else
        {
            isApplicationRunning = false;
        }

        return isApplicationRunning;
    }

    bool CloseApplicationRunning(const int processId)
    {
        return (processId != 0) && (kill(processId, SIGTERM) == 0);
    }

    std::string GetExecutableDirectory()
    {
        char buffer[1024];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
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