#include "Grid.h"

#include "GameListManager.h"
#include "ImageLoader.h"
#include "Math.h"
#include "Print.h"
#include "SoundComponent.h"
#include "TextureManager.h"
#include "UtilsFunctionLibrary.h"


Grid::Grid()
	: positionX(0)
	, bLeft(false)
	, bRight(false)
	, lastDirection(None)
	, idFocus(0)
{

	// todo for construction layout
	SetTexture(TextureManager::GetInstance()->GetSprite("ref"), Rectangle{ 0, 0, 1280, 720 }); 


	for (int i = 0; i < 10; i++)
	{
		const float x = static_cast<float>(256 * (i - 2));
		Card card;
		card.CreateCard(TextureManager::GetInstance()->GetSprite("sprite"), Vector2{ x - 120, 228 }, Rectangle{ 528, 15, 246, 270 }, i);
		cardsContainer.push_back(card);
	}

	SetFocus(3);
	ImageLoader::GetInstance()->StartLoadingLoadTexture(GameListManager::GetInstance()->GetGameId());

}

void Grid::BeginPlay()
{
	Object::BeginPlay();
	SetCovers();
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
	GameListManager::GetInstance()->ClearGameList();
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


void Grid::SetFocus(const int newId)
{
	if(cardsContainer[idFocus].bFocus && idFocus == newId) return;

	cardsContainer[idFocus].bFocus = false;
	cardsContainer[idFocus].StartAnimationLostFocus();
	idFocus = newId;
	cardsContainer[newId].StartAnimationFocus();
	cardsContainer[newId].bFocus = true;
	bLeft = true;
}

void Grid::SetCovers()
{
	GameListManager* manager = GameListManager::GetInstance();


	if (manager->GetGameListSize() == 0)
	{
		ClearCovers();
	}

	for (int i = 0; i < 10; i++)
	{
		int indexFinal = UtilsFunctionLibrary::SetIndexArray(manager->GetGameId() + i - idFocus, manager->GetGameListSize());
		indexFinal = UtilsFunctionLibrary::SetIndexArray(indexFinal, manager->GetGameListSize());
		indexFinal = Math::Clamp(indexFinal, 0, manager->GetGameListSize() - 1);
		if (manager->GetGameListSize() > 0)
		{
			Texture2D* texture = TextureManager::GetInstance()->GetCover(indexFinal);
			const std::string* image = &manager->GetCurrentGameList(indexFinal)->image;
			cardsContainer[i].SetCover(!image->empty() ? texture : nullptr);
		}
	}
}

void Grid::ClearCovers()
{
	for (int i = 0; i < 10; i++)
	{
		cardsContainer[i].ResetCover();
	}
}

void Grid::Tick()
{
	Object::Tick();

	if (IsKeyDown(KEY_B))
	{
		cardsContainer[idFocus].StartAnimationClick();
		GameListManager::GetInstance()->ClearGameList();
	}

	if (IsKeyDown(KEY_V) && !bLeft)
	{
		SetFocus(3);
	}

	PRINT_STRING(TextFormat("idGamelist: %d", GameListManager::GetInstance()->GetGameId()), 0.2f, "getid", BLUE);

	SetCovers();
	constexpr int speed = 22;

	if (IsKeyDown(KEY_LEFT) && !bRight)
	{
		if (!bLeft)
		{
			SoundComponent::GetInstance()->PlayCursor();
			GameListManager::GetInstance()->AddId(-1);
			ImageLoader::GetInstance()->StartLoadingLoadTexture(GameListManager::GetInstance()->GetGameId());
			cardsContainer[idFocus].StartAnimationLostFocus();
			cardsContainer[idFocus].bFocus = false;
			idFocus--;
			cardsContainer[idFocus].StartAnimationFocus();
			cardsContainer[idFocus].bFocus = true;
		}
		bLeft = true;
	}
	if (IsKeyDown(KEY_RIGHT) && !bLeft)
	{
		if (!bRight)
		{
			SoundComponent::GetInstance()->PlayCursor();
			GameListManager::GetInstance()->AddId(1);
			ImageLoader::GetInstance()->StartLoadingLoadTexture(GameListManager::GetInstance()->GetGameId());
			cardsContainer[idFocus].StartAnimationLostFocus();
			cardsContainer[idFocus].bFocus = false;
			idFocus++;
			cardsContainer[idFocus].bFocus = true;
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
			lastDirection = Left;
		}
		else if (positionX > 0 && positionX < 256 && bLeft)
		{
			if (idFocus < 3 || idFocus > 6)
			{
				cardContainer.AddPosition(Vector2{ speed,0 });
			}
			lastDirection = Right;
		}
		else if (positionX <= -256 || positionX >= 256)
		{
			positionX = 0;
			bRight = false;
			bLeft = false;
			//SetCovers();
			std::vector<int> v;
			ImageLoader::GetInstance()->UnloadGameListTextureOutRange(v, 0);
		}

	}

	if (idFocus < 3 || idFocus > 6)
	{

		if (lastDirection == Left && positionX == 0)
		{
			std::rotate(cardsContainer.begin(), cardsContainer.begin() + 1, cardsContainer.end());
			lastDirection = None;
			idFocus = Math::Clamp(idFocus, 3, 6);
		}
		else if (lastDirection == Right && positionX == 0)
		{
			std::rotate(cardsContainer.rbegin(), cardsContainer.rbegin() + 1, cardsContainer.rend());
			lastDirection = None;
			idFocus = Math::Clamp(idFocus, 3, 6);
		}

	}

	if (!bLeft && !bRight)
	{
		for (size_t i = 0; i < cardsContainer.size(); i++)
		{
			Vector2 pos{ cardPositions[i], cardsContainer[i].currentPosition.position.y };

			cardsContainer[i].ChangePosition(pos);
		}
	}



}
