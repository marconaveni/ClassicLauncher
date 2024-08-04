#pragma once
#include <vector>

#include "Object.h"
#include "raylib.h"

class MiniCovers : public Object
{
public:

	std::vector<Texture2D> covers;

	std::vector<std::shared_ptr<Object>> objectsCover;

	MiniCovers();

	void CreateCovers();

	void RegisterCovers() const;

	void SetCovers() const;

	void SetPosition(const int size) const;
	void BeginPlay() override;
	void Tick() override;
};
