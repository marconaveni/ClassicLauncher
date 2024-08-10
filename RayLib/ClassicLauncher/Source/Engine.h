#pragma once

#include "MainScreen.h"


class Engine 
{

	Engine() = default;

public:
	
	std::shared_ptr<MainScreen> mainScreen;

	static Engine* GetInstance();

	void BeginPlay();

	void Tick();

	void EndDraw();

	void EndPlay();

};
