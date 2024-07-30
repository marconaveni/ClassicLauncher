#include "Card.h"

#include "ObjectManager.h"
#include "Print.h"

void Card::CreateCard(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture, const int idCard)
{
	currentPosition.position = position;
	Rectangle rectangleFavorite = rectangleTexture;
	Rectangle rectangleSelected = rectangleTexture;
	rectangleFavorite.x = 783;
	rectangleSelected.x = 273;
	cardMain = std::make_shared<Object>(textureReference, position, rectangleTexture);
	cardFavorite = std::make_shared<Object>(textureReference, position, rectangleFavorite);
	cardSelected = std::make_shared<Object>(textureReference, position, rectangleSelected);
	cardSelected->position.color.SetColorAlpha(0);
	id = idCard;
}

void Card::RegisterCard() const
{
	ObjectManager::GetInstance()->RegisterObject(cardFavorite);
	ObjectManager::GetInstance()->RegisterObject(cardMain);
	ObjectManager::GetInstance()->RegisterObject(cardSelected);
}

void Card::AddPosition(const Vector2 newPosition)
{
	currentPosition.position = Vector2{ currentPosition.position.x + newPosition.x , currentPosition.position.y + newPosition.y };
	cardMain->position.position = currentPosition.position;
	cardFavorite->position.position = currentPosition.position;
	cardSelected->position.position = currentPosition.position;
}

void Card::ChangePosition(const Position& newPosition)
{
	ChangePosition(newPosition.position);
}

void Card::ChangePosition(const Vector2& newPosition)
{
	currentPosition.position = newPosition;
	cardMain->position.position = newPosition;
	cardFavorite->position.position = newPosition;
	cardSelected->position.position = newPosition;
}

void Card::Tick()
{
	currentPosition = cardSelected->position;

	PRINT_STRING(TextFormat("%d position x %f", id, currentPosition.position.x), 0.2f, TextFormat("id%d", id));
	animationLostFocus.UpdateAnimation();
	animationFocus.UpdateAnimation();
	animationClick.UpdateAnimation();

	if (animationFocus.GetAnimationIsRun())
	{
		cardSelected->position.color = animationFocus.currentPosition.color;
	}
	if (animationLostFocus.GetAnimationIsRun())
	{
		cardSelected->position.color = animationLostFocus.currentPosition.color;
	}
	if (animationClick.GetAnimationIsRun() || animationClick.GetAnimationFinish())
	{
		cardMain->BringToFrontRender();
		cardSelected->BringToFrontRender();
		cardFavorite->BringToFrontRender();
		cardMain->position.color = animationClick.currentPosition.color;
		cardSelected->position.color = animationClick.currentPosition.color;
		cardFavorite->position.color = animationClick.currentPosition.color;
		cardMain->position.scale = animationClick.currentPosition.scale;
		cardSelected->position.scale = animationClick.currentPosition.scale;
		cardFavorite->position.scale = animationClick.currentPosition.scale;
	}
}

void Card::StartAnimationFocus()
{
	Position newPosition = currentPosition;
	newPosition.color.SetColorAlpha(255);
	animationFocus.StartAnimation(10, currentPosition, newPosition, Ease::EaseLinearNone, false);
}

void Card::StartAnimationLostFocus()
{
	Position newPosition = currentPosition;
	newPosition.color.SetColorAlpha(0);  
	animationLostFocus.StartAnimation(10, currentPosition, newPosition, Ease::EaseLinearNone, false);

}

void Card::StartAnimationClick()
{
	currentPosition.scale = Vector2{ 1,1 };
	currentPosition.color.SetColor(WHITE);
	Position newPosition = currentPosition;
	newPosition.scale.x = 2.7f;
	newPosition.scale.y = 2.7f;
	newPosition.color.SetColorAlpha(-255);
	animationClick.StartAnimation(60, currentPosition, newPosition, Ease::EaseQuadOut, true);
}
