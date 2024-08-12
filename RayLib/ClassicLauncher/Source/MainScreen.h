#pragma once


#include <memory>

#include "Grid.h"
#include "MiniCovers.h"
#include "Object.h"
#include "PlatformProcess.h"


class MainScreen : public Object
{


public:

	std::shared_ptr<Grid> grid;
	std::shared_ptr<MiniCovers> miniCovers;
	std::shared_ptr<PlatformProcess> platformProcess;
	
	MainScreen();

	void Initialize(const std::shared_ptr<MainScreen>& mainScreenRef);
	void BeginPlay() override;
	void Tick() override;
	void Draw() override;
	void EndDraw() override;
	void EndPlay() override;
	void Collision() override;
};

