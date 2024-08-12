#include "PlatformProcess.h"

#include "Process.h"
#include "SoundComponent.h"


PlatformProcess::PlatformProcess()
	: processId(0), bRunning(false)
{
}

void PlatformProcess::CreateProc(const std::string& fullPath, const std::string& optionalWorkingDirectory)
{
	Process::CreateProc(processId, fullPath, optionalWorkingDirectory);
}

void PlatformProcess::BeginPlay()
{
	Object::BeginPlay();
}

void PlatformProcess::Tick()
{
	Object::Tick();
	if (Process::IsApplicationRunning(processId))
	{
		printf("running\n");
		if (!bRunning)
		{
			SoundComponent::GetInstance()->PauseMusic();
			printf("open app\n");
			bRunning = true;
		}
	}
	else
	{
		if (bRunning)
		{
			SoundComponent::GetInstance()->PlayMusic();
			printf("close app\n");
			bRunning = false;
		}
	}


	if (IsGamepadButtonReleased(0, 5))
	{
		printf("pressed\n");
		Process::CloseApplicationRunning(processId);
	}
}

void PlatformProcess::Draw()
{
	//Object::Draw();
}

void PlatformProcess::EndDraw()
{
	//Object::EndDraw();
}

void PlatformProcess::EndPlay()
{
	Object::EndPlay();
}

void PlatformProcess::Collision()
{
}
