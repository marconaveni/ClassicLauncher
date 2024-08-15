#pragma once
#include <vector>

#include "Object.h"
#include "ObjectAnimated.h"
#include <memory>
#include "raylib.h"

class MiniCovers : public Object
{
public:

	float positionY;

	std::vector<Texture2D> covers;

	std::vector<std::shared_ptr<Object>> objectsCover;

	std::shared_ptr<ObjectAnimated> arrow;

	MiniCovers();

	void CreateCovers();

	void RegisterCovers() const;

	void SetCovers() const;
	void ClearCovers() const;

	void SetPosition(const int size) const;
	void BeginPlay() override;
	void Tick() override;
};
