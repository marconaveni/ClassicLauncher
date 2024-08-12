#include "Process.h"

#ifdef _WIN32
#include <windows.h>

std::wstring utf8_to_wstring(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), wstrTo.data(), size_needed);
    return wstrTo;
}

std::string wstring_to_utf8(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), strTo.data(), size_needed, NULL, NULL);
    return strTo;
}


void Process::CreateProc(unsigned int& processId, const std::string& fullPath, const std::string& optionalWorkingDirectory)
{

    std::wstring path = utf8_to_wstring(fullPath);
    std::wstring dir = utf8_to_wstring(optionalWorkingDirectory);

    STARTUPINFOW info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;


    if (CreateProcessW(
        nullptr,
        path.data(), 
        nullptr,
        nullptr,
        TRUE,
        0,
        nullptr,
        dir.empty() ? nullptr : dir.data(),
        &info,
        &processInfo
    ))
    {
        printf("open:");
        //WaitForSingleObject(processInfo.hProcess, INFINITE);
        processId = static_cast<unsigned int>(processInfo.dwProcessId);

        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);

    }
    else
    {
        printf("Error on create a process: %lu\n", GetLastError());
    }
}

bool Process::IsApplicationRunning(const unsigned int processId)
{
    bool bApplicationRunning = true;
    HANDLE ProcessHandle = OpenProcess(SYNCHRONIZE, false, processId);
    if (ProcessHandle == NULL)
    {
        bApplicationRunning = false;
    }
    else
    {
        unsigned int WaitResult = WaitForSingleObject(ProcessHandle, 0);
        if (WaitResult != WAIT_TIMEOUT)
        {
            bApplicationRunning = false;
        }
        ::CloseHandle(ProcessHandle);
    }
    return bApplicationRunning;
}

bool Process::CloseApplicationRunning(const unsigned int processId)
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



#else


void Process::CreateProc(unsigned int& processId, std::string fullPath, std::string optionalWorkingDirectory)
{

}

bool Process::IsApplicationRunning(const unsigned int processId)
{
    bool bApplicationRunning = true;
    return bApplicationRunning;
}

bool Process::CloseApplicationRunning(const unsigned int processId)
{
    bool bApplicationRunning;
    return bApplicationRunning;
}


#endif