#pragma once

#include "MainScreen.h"
#include "ObjectManager.h"
#include <memory>


class Engine 
{

	Engine() = default;

	std::shared_ptr<ObjectManager> objectManager;


public:
	
	std::shared_ptr<MainScreen> mainScreen;

	static Engine* GetInstance();

	void BeginPlay();

	void Tick();

	void EndDraw();

	void EndPlay();

};
