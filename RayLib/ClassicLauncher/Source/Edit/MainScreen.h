#pragma once


#include <memory>

#include "GameListManager.h"
#include "Grid.h"
#include "MiniCovers.h"
#include "Object.h"
#include "PlatformProcess.h"
#include "ThreadManager.h"


class MainScreen : public Object
{


public:

	ThreadManager<int> threadLoad;

	std::shared_ptr<Grid> grid;
	std::shared_ptr<MiniCovers> miniCovers;
	std::shared_ptr<PlatformProcess> platformProcess;

	MainScreen();

	void Initialize(const std::shared_ptr<MainScreen>& mainScreenRef);
	void BeginPlay() override;
	void Tick() override;
	void ChangeGrid(CurrentList list);
	void Draw() override;
	void EndDraw() override;
	void EndPlay() override;
	void Collision() override;

	void ClearCovers();
};

