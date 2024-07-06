#include "Process.h"
#include <windows.h>

void Process::CreateProc(unsigned int& ProcessId, std::string FullPath, bool Hidden, std::string OptionalWorkingDirectory)
{
    std::wstring path(L"D:\\Jogos\\Need For Speed Underground 2\\SPEED2.EXE");
    std::wstring dir(L"D:\\Jogos\\Need For Speed Underground 2\\");


    STARTUPINFOW info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;


    if (CreateProcessW(nullptr, &path[0], nullptr, nullptr, TRUE, 0, nullptr, &dir[0], &info, &processInfo))
    {
        printf("aberto:");
        //WaitForSingleObject(processInfo.hProcess, INFINITE);
        ProcessId = static_cast<unsigned int>(processInfo.dwProcessId);

        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);

    }
    else
    {
        printf("Erro ao criar o processo. Codigo de erro: %d\n", GetLastError());
    }
}

bool Process::IsApplicationRunning(unsigned int ProcessId)
{
    bool bApplicationRunning = true;
    HANDLE ProcessHandle = ::OpenProcess(SYNCHRONIZE, false, ProcessId);
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

bool Process::CloseApplicationRunning(unsigned int ProcessId)
{
    bool bApplicationRunning = false;
    HANDLE ProcessHandle = OpenProcess(PROCESS_TERMINATE, false, ProcessId);
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
