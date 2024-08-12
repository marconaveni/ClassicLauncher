#pragma once
#include "Object.h"


class PlatformProcess : public Object
{
public:

	unsigned int processId;
	bool bRunning;

	PlatformProcess();
	void CreateProc(const std::string& fullPath, const std::string& optionalWorkingDirectory);
	void BeginPlay() override;
	void Tick() override;
	void Draw() override;
	void EndDraw() override;
	void EndPlay() override;
	void Collision() override;
};
