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
		unsigned int WaitResult = WaitForSingleObject(ProcessHandle, INFINITE);
		if (WaitResult != WAIT_TIMEOUT)
		{
			bApplicationRunning = false;
		}
		CloseHandle(ProcessHandle);
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


#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include "StringFunctionLibrary.h"


void Process::CreateProc(unsigned int& processId, const std::string& fullPath, const std::string& optionalWorkingDirectory)
{

	std::vector<std::string> paths = StringFunctionLibrary::SplitString(fullPath);

	for (auto& path : paths)
	{
		StringFunctionLibrary::ReplaceString(path, "\"", "");
	}


	pid_t pid = fork();
	if (pid == -1)
	{
		std::cerr << "Falha ao criar o processo filho." << std::endl;         // Erro ao criar o processo filho
		return;
	}

	if (pid == 0) // Processo filho
	{

		std::vector<char*> args;// Criar um array de ponteiros para char
		for (const auto& arg : paths) {
			args.push_back(const_cast<char*>(arg.c_str())); // Converte std::string para char*
		}
		args.push_back(nullptr);

		if (execvp(args[0], args.data()) == -1) {
			std::cerr << "Falha ao executar o programa." << std::endl;
			return;
		}
	}

	processId = pid;
}

bool Process::IsApplicationRunning(const unsigned int processId)
{
	bool bApplicationRunning;
	int status;

	pid_t result = waitpid(processId, &status, WNOHANG);  // Verifica se o processo ainda está em execução

	if (result == 0)
	{
		bApplicationRunning = true;
		std::cout << "O processo filho ainda está em execução..." << std::endl;
	}
	else if (result == processId)
	{

		if (WIFEXITED(status))  // Processo filho terminou
		{
			std::cout << "O processo filho terminou com o status: " << WEXITSTATUS(status) << std::endl;
		}
		else
		{
			std::cout << "O processo filho não terminou corretamente." << std::endl;
		}
		bApplicationRunning = false;
	}
	else
	{
		bApplicationRunning = false;
		//std::cerr << "Erro ao monitorar o processo filho." << std::endl;                
	}

	return bApplicationRunning;
}

bool Process::CloseApplicationRunning(const unsigned int processId)
{

	if (processId == 0)
	{
		return false;
	}

	if (kill(processId, SIGTERM) == 0) // Tenta enviar o sinal SIGTERM para o processo
	{
		std::cout << "O sinal SIGTERM foi enviado com sucesso para o processo." << std::endl;
		return true;
	}
	else
	{
		std::cerr << "Erro ao tentar fechar o processo: " << strerror(errno) << std::endl;
		return false;
	}


}


#endif