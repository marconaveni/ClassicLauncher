#pragma once


#include "AnimationFrame.h"
#include "Object.h"

class ObjectAnimated : public Object
{

	AnimationFrame animation;

public:
	ObjectAnimated(Texture2D* textureReference, const Vector2& positionObj, const Rectangle& rectangleTexture)
		: Object(textureReference, positionObj, rectangleTexture),
		  animation()
	{
	}

	void BeginPlay() override;
	void Tick() override;
	void Draw() override;
	void EndDraw() override;
	void EndPlay() override;
	void Collision() override;
	void AddAnimation(const std::string& animationName, const std::vector<Vector2>& newAnimation);
	void UpdateAnimation();
	void ChangeAnimation(const std::string& animationName);
	void SetAnimationSpeed(float delay);
};
