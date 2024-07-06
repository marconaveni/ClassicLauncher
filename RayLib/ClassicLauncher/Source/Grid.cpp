#include "Grid.h"

#include "ObjectManager.h"
#include "Print.h"
#include "TextureManager.h"


void CardContainer::CreateCard(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture, const int id)
{
	currentPosition = position;
	cardMain = std::make_shared<Card>(textureReference, position, rectangleTexture);
	cardFavorite = std::make_shared<Card>(textureReference, position, rectangleTexture);
	cardSelected = std::make_shared<Card>(textureReference, position, rectangleTexture);
	this->id = id;
}

void CardContainer::RegisterCard() const
{
	ObjectManager::GetInstance()->RegisterObject(cardMain);
	ObjectManager::GetInstance()->RegisterObject(cardFavorite);
	ObjectManager::GetInstance()->RegisterObject(cardSelected);
}

void CardContainer::AddPosition(Vector2 newPosition)
{
	currentPosition = Vector2{ currentPosition.x + newPosition.x , currentPosition.y + newPosition.y };
	cardMain->position = currentPosition;
	cardFavorite->position = currentPosition;
	cardSelected->position = currentPosition;
}

void CardContainer::ChangePosition(Vector2 newPosition)
{
	currentPosition = newPosition;
	cardMain->position = currentPosition;
	cardFavorite->position = currentPosition;
	cardSelected->position = currentPosition;
}


void CardContainer::Tick()
{
	PRINT_STRING(TextFormat("%d position x %f", id, currentPosition.x), 0.2f, TextFormat("id%d", id));
}



Grid::Grid(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture)
	: Object(textureReference, position, rectangleTexture)
	, positionX(0)
	, bLeft(false)
	, bRight(false)
	, LastDirection(None)
{
	//Image img = LoadImage("C:\\Projetos\\ClassicLauncher\\RayLib\\ClassicLauncher\\Resources\\themes\\default\\sprite.png");
	//ImageResize(&img, 1290, 1290);
	//texture = LoadTexture("C:\\Projetos\\ClassicLauncher\\RayLib\\ClassicLauncher\\Resources\\themes\\default\\sprite.png");

	for (int i = 0; i < 10; i++)
	{
		const float x = static_cast<float>(256 * (i - 2));
		CardContainer card;
		card.CreateCard(TextureManager::GetInstance()->GetSprite("sprite"), Vector2{x - 120, 130}, Rectangle{528, 15, 246, 270}, i);
		cardsContainer.push_back(card);
	}


	cardsContainer[0].cardSelected->SetColor(255, 0, 0);
	

	//UnloadImage(img);
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

	constexpr int speed = 22;

	//if (IsKeyDown(KEY_B))
	//{
	//	animation.StartAnimation();
	//	this->position.x = 0;
	//	cardsContainer[3].ChangePosition(Vector2{ 0,0 });
	//}
	//animation.UpdateAnimation();

	//this->position.x += animation.currentPositionX;
	//cardsContainer[3].ChangePosition(Vector2{ this->position.x,0 });

	//return;

	if (IsKeyDown(KEY_LEFT) && !bRight)
	{
		bLeft = true;
	}
	if (IsKeyDown(KEY_RIGHT) && !bLeft)
	{
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
			cardContainer.AddPosition(Vector2{ -speed,0 });
			LastDirection = Left;
		}
		else if (positionX > 0 && positionX < 256 && bLeft)
		{
			cardContainer.AddPosition(Vector2{ speed,0 });
			LastDirection = Right;
		}
		else if (positionX <= -256 || positionX >= 256)
		{
			positionX = 0;
			bRight = false;
			bLeft = false;

		}

	}

	if (LastDirection == Left && positionX == 0)
	{
		std::rotate(cardsContainer.begin(), cardsContainer.begin() + 1, cardsContainer.end());
		LastDirection = None;
	}
	else if (LastDirection == Right && positionX == 0)
	{
		std::rotate(cardsContainer.rbegin(), cardsContainer.rbegin() + 1, cardsContainer.rend());
		LastDirection = None;
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
