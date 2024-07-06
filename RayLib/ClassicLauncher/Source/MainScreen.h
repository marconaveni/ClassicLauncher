#pragma once


#include <memory>

#include "Grid.h"
#include "Object.h"


class MainScreen : public Object
{


public:

	
	std::shared_ptr<Grid> grid;
	
	MainScreen();
	
	~MainScreen() override;
	void BeginPlay() override;
	void Tick() override;
	void Draw() override;
	void EndDraw() override;
	void EndPlay() override;
	void Collision() override;
};

