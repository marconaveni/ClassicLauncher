#include "ObjectAnimated.h"

void ObjectAnimated::BeginPlay()
{
	Object::BeginPlay();
}

void ObjectAnimated::Tick()
{
	Object::Tick();
	UpdateAnimation();
}

void ObjectAnimated::Draw()
{
	Object::Draw();
}

void ObjectAnimated::EndDraw()
{
	Object::EndDraw();
}

void ObjectAnimated::EndPlay()
{
	Object::EndPlay();
}

void ObjectAnimated::Collision()
{
	Object::Collision();
}

void ObjectAnimated::AddAnimation(const std::string& animationName, const std::vector<Vector2>& newAnimation)
{
	animation.AddAnimation(animationName, newAnimation);
}

void ObjectAnimated::UpdateAnimation()
{
	animation.UpdateAnimation();
	const Vector2 newAnimation = animation.GetCurrentFrame();
	spriteResource.source.x = newAnimation.x;
	spriteResource.source.y = newAnimation.y;
}

void ObjectAnimated::ChangeAnimation(const std::string& animationName)
{
	if (animation.currentAnimation != animationName)
	{
		animation.currentAnimation = animationName;
		animation.frame = 0;
	}
}

void ObjectAnimated::SetAnimationSpeed(const float delay)
{
	animation.delay.duration = delay;
}
