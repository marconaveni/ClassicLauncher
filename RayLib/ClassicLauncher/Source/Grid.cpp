#include "Grid.h"

#include "Math.h"
#include "ObjectManager.h"
#include "Print.h"
#include "TextureManager.h"


void CardContainer::CreateCard(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture, const int id)
{
	currentPosition = position;
	Rectangle rectangleFavorite = rectangleTexture;
	Rectangle rectangleSelected = rectangleTexture;
	rectangleFavorite.x = 783;
	rectangleSelected.x = 273;
	cardMain = std::make_shared<Card>(textureReference, position, rectangleTexture);
	cardFavorite = std::make_shared<Card>(textureReference, position, rectangleFavorite);
	cardSelected = std::make_shared<Card>(textureReference, position, rectangleSelected);
	cardSelected->position.color.a = 0;
	this->id = id;
}

void CardContainer::RegisterCard() const
{
	ObjectManager::GetInstance()->RegisterObject(cardFavorite);
	ObjectManager::GetInstance()->RegisterObject(cardMain);
	ObjectManager::GetInstance()->RegisterObject(cardSelected);
}

void CardContainer::AddPosition(Vector2 newPosition)
{
	currentPosition = Vector2{ currentPosition.x + newPosition.x , currentPosition.y + newPosition.y };
	cardMain->position.position = currentPosition;
	cardFavorite->position.position = currentPosition;
	cardSelected->position.position = currentPosition;
}

void CardContainer::ChangePosition(const Position& newPosition)
{
	currentPosition = newPosition.position;
	cardMain->position.position = newPosition.position;
	cardFavorite->position.position = newPosition.position;
	cardSelected->position.position = newPosition.position;
}


void CardContainer::Tick()
{
	PRINT_STRING(TextFormat("%d position x %f", id, currentPosition.x), 0.2f, TextFormat("id%d", id));
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
		cardMain->position.color = animationClick.currentPosition.color;
		cardSelected->position.color = animationClick.currentPosition.color;
		cardFavorite->position.color = animationClick.currentPosition.color;
		cardMain->position.scale = animationClick.currentPosition.scale;
		cardSelected->position.scale = animationClick.currentPosition.scale;
		cardFavorite->position.scale = animationClick.currentPosition.scale;
	}
}

void CardContainer::StartAnimationFocus()
{
	Position newPosition = currentPosition;
	Position position{ currentPosition,Vector2{1,1}, cardSelected->position.color };
	newPosition.color.a = 255;
	animationFocus.StartAnimation(10, position, newPosition, Ease::EaseLinearNone, false);
}

void CardContainer::StartAnimationLostFocus()
{
	Position newPosition = currentPosition;
	Position position{ currentPosition,Vector2{1,1}, cardSelected->position.color };
	newPosition.color.a = 0;
	animationLostFocus.StartAnimation(10, currentPosition, newPosition, Ease::EaseLinearNone, false);
}

void CardContainer::StartAnimationClick()
{

	Position newPosition = currentPosition;
	newPosition.scale.x = 1.7f;
	newPosition.scale.y = 1.7f;
	newPosition.color.a = 0;
	animationClick.StartAnimation(22, currentPosition, newPosition, Ease::EaseQuadOut, true);
}


Grid::Grid(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture)
	: Object(textureReference, position, rectangleTexture)
	, positionX(0)
	, bLeft(false)
	, bRight(false)
	, LastDirection(None)
	, idFocus(3)
{
	for (int i = 0; i < 10; i++)
	{
		const float x = static_cast<float>(256 * (i - 2));
		CardContainer card;
		card.CreateCard(TextureManager::GetInstance()->GetSprite("sprite"), Vector2{ x - 120, 130 }, Rectangle{ 528, 15, 246, 270 }, i);
		cardsContainer.push_back(card);
	}


	//cardsContainer[0].cardSelected->SetColor(255, 0, 0);
}

Grid::~Grid()
{
}

void Grid::BeginPlay()
{
	Object::BeginPlay();
}

void Grid::Draw()
{
	Object::Draw();
}

void Grid::EndDraw()
{
	Object::EndDraw();
}

void Grid::EndPlay()
{
	Object::EndPlay();
}

void Grid::Collision()
{
	Object::Collision();
}

void Grid::RegisterCards() const
{
	for (const auto& cardContainer : cardsContainer)
	{
		cardContainer.RegisterCard();
	}
}


void Grid::Tick()
{
	Object::Tick();

	//cardsContainer[3].cardSelected->scale = cardsContainer[3].cardSelected->scale + 2;

	if (IsKeyDown(KEY_B))
	{
		//this->position.position.x = 0;
		//cardsContainer[3].ChangePosition(Position{});
		//Position newPosi;
		////newPosi.position.x = 1280;
		//newPosi.scale.x = 2;
		//newPosi.scale.y = 2;
		//newPosi.color.a = 0;
		//animation.StartAnimationFocus(60, this->position, newPosi );
		//animation2.StartAnimationFocus(60, this->position, newPosi );
		cardsContainer[idFocus].StartAnimationClick();
	}
	//animation.UpdateAnimation();
	//animation2.UpdateAnimation();

	this->position = animation.currentPosition;
	//cardsContainer[3].ChangePosition(animation.currentPosition);

	//return;

	constexpr int speed = 22;

	if (IsKeyDown(KEY_LEFT) && !bRight)
	{
		if (!bLeft)
		{
			cardsContainer[idFocus].StartAnimationLostFocus();
			idFocus--;
			cardsContainer[idFocus].StartAnimationFocus();
		}
		bLeft = true;
	}
	if (IsKeyDown(KEY_RIGHT) && !bLeft)
	{
		if (!bRight)
		{
			cardsContainer[idFocus].StartAnimationLostFocus();
			idFocus++;
			cardsContainer[idFocus].StartAnimationFocus();
		}
		bRight = true;
	}

	if (bRight)
	{
		positionX = positionX - speed;
	}
	else if (bLeft)
	{
		positionX = positionX + speed;
	}


	for (auto& cardContainer : cardsContainer)
	{
		cardContainer.Tick();
		if (positionX > -256 && positionX < 0 && bRight)
		{
			if (idFocus < 3 || idFocus > 6)
			{
				cardContainer.AddPosition(Vector2{ -speed,0 });
			}
			LastDirection = Left;
		}
		else if (positionX > 0 && positionX < 256 && bLeft)
		{
			if (idFocus < 3 || idFocus > 6)
			{
				cardContainer.AddPosition(Vector2{ speed,0 });
			}
			LastDirection = Right;
		}
		else if (positionX <= -256 || positionX >= 256)
		{
			positionX = 0;
			bRight = false;
			bLeft = false;
		}

	}

	if (idFocus < 3 || idFocus > 6)
	{

		if (LastDirection == Left && positionX == 0)
		{
			std::rotate(cardsContainer.begin(), cardsContainer.begin() + 1, cardsContainer.end());
			LastDirection = None;
			idFocus = Math::Clamp(idFocus, 3, 6);
		}
		else if (LastDirection == Right && positionX == 0)
		{
			std::rotate(cardsContainer.rbegin(), cardsContainer.rbegin() + 1, cardsContainer.rend());
			LastDirection = None;
			idFocus = Math::Clamp(idFocus, 3, 6);
		}

	}

	if (!bLeft && !bRight)
	{
		for (size_t i = 0; i < cardsContainer.size(); i++)
		{
			Vector2 pos{ static_cast<float>(cardPositions[i]), cardsContainer[i].currentPosition.y };
			cardsContainer[i].ChangePosition(pos);
		}
	}



}
