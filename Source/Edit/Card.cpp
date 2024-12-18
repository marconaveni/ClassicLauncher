#include "Card.h"

#include "ObjectManager.h"
#include "Print.h"
#include "TextureManager.h"

void Card::CreateCard(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture, const int idCard, std::shared_ptr<ObjectManager> objectManager)
{
	currentPosition.position = position;
	Rectangle rectangleFavorite = rectangleTexture;
	Rectangle rectangleSelected = rectangleTexture;
	rectangleFavorite.x = 783;
	rectangleSelected.x = 273;
	cardMain = std::make_shared<Object>(textureReference, position, rectangleTexture, objectManager);
	cardFavorite = std::make_shared<Object>(textureReference, position, rectangleFavorite, objectManager);
	cardSelected = std::make_shared<Object>(textureReference, position, rectangleSelected, objectManager);
	cover = std::make_shared<Object>(textureReference, position, Rectangle{ 0,0,0,0 }, objectManager);
	cardSelected->position.color.SetColorAlpha(0);
	id = idCard;
}

void Card::ResetCover()
{
	SetCover(nullptr);
}

void Card::RegisterCard()
{
	objectManager->RegisterObject(cardFavorite);
	objectManager->RegisterObject(cardMain);
	objectManager->RegisterObject(cardSelected);
	objectManager->RegisterObject(cover); 
}

void Card::AddPosition(const Vector2 newPosition)
{
	currentPosition.position = Vector2{ currentPosition.position.x + newPosition.x , currentPosition.position.y + newPosition.y };
	cardMain->position.position = currentPosition.position;
	cardFavorite->position.position = currentPosition.position;
	cardSelected->position.position = currentPosition.position;
	cover->position.position.x = currentPosition.position.x + offSetCover.x;
	cover->position.position.y = currentPosition.position.y + offSetCover.y;

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
	cover->position.position.x = newPosition.x + offSetCover.x;
	cover->position.position.y = newPosition.y + offSetCover.y;
}

void Card::Tick()
{
	currentPosition = cardSelected->position;

	//PRINT_STRING(TextFormat("%d position x %f", id, currentPosition.position.x), 0.2f, TextFormat("id%d", id));
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

void Card::SetCover(Texture2D* textureReference)
{

	if (textureReference != nullptr)
	{
		cover->SetTexture(textureReference, Rectangle{ 0,0,static_cast<float>(textureReference->width),static_cast<float>(textureReference->height) });
		offSetCover.x = (240.0f - static_cast<float>(textureReference->width)) / 2.0f;
		offSetCover.y = (216.0f - static_cast<float>(textureReference->height)) / 2.0f;
	}
	else
	{
		cover->SetTexture(TextureManager::GetInstance()->GetSprite("sprite"), Rectangle{ 1086,1086,204,204 });
		offSetCover.x = (240.0f - 204.0f) / 2.0f;
		offSetCover.y = (216.0f - 204.0f) / 2.0f;
	}


}
