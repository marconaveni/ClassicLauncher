#include "GuiGrid.h"


#include "Utils/Core.h"
#include <algorithm>  
#include <memory>  

namespace ClassicLauncher
{


	GuiGrid::GuiGrid()
		: positionX(0)
		, bLeft(false)
		, bRight(false)
		, lastDirection(None)
		, idFocus(0)
	{
	}

    void GuiGrid::Init()
    {
		app = &Application::Get();

		for (int i = 0; i < 10; i++)
		{
			const float x = static_cast<float>(256 * (i - 2));
			auto card = app->GetEntityManager()->CreateEntity<GuiCard>(x - 120, 228);
        	AddChild(card.get());
			cardsContainer.emplace_back(card);

		}

		SetFocus(3);
		//ImageLoader::GetInstance()->StartLoadingLoadTexture(GameListManager::GetInstance()->GetGameId());
    }

    void GuiGrid::Draw()
	{
		GuiComponent::Draw();
	}



    void GuiGrid::End()
    {
		GuiComponent::End();
		//GameListManager::GetInstance()->ClearGameList();
    }


	void GuiGrid::RegisterCards() const
	{
		// for (const auto& cardContainer : cardsContainer)
		// {
		// 	//cardContainer.RegisterCard();
		// }
	}


	void GuiGrid::SetFocus(const int newId)
	{
		if(cardsContainer[idFocus]->IsFocus() && idFocus == newId) return;

		cardsContainer[idFocus]->SetFocus();
		//cardsContainer[idFocus].StartAnimationLostFocus();
		idFocus = newId;
		//cardsContainer[newId].StartAnimationFocus();
		cardsContainer[newId]->SetFocus();
		bLeft = true;
	}

	void GuiGrid::SetCovers()
	{
		GameListManager* manager = app->GetGameListManager();

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
				// Texture2D* texture = &TextureManager::GetInstance()->GetCover(indexFinal)->texture;
				// const std::string* image = &manager->GetCurrentGameList(indexFinal)->image;
				// if(image == nullptr)
				// {
				// 	continue;
				// }
				// cardsContainer[i].SetCover(!image->empty() ? texture : nullptr);
			}
		}
	}

	void GuiGrid::ClearCovers()
	{

	}

	void GuiGrid::Update()
	{
		GuiComponent::Update();


		if (IsKeyDown(KEY_B))
		{
			// cardsContainer[idFocus].StartAnimationClick();
			// GameListManager::GetInstance()->ClearGameList();
		}

		if (IsKeyDown(KEY_V) && !bLeft)
		{
			SetFocus(3);
		}

		//PRINT_STRING(TextFormat("idGamelist: %d", GameListManager::GetInstance()->GetGameId()), 0.2f, "getid", BLUE);

		SetCovers();
		constexpr int speed = 22;

		if (IsKeyDown(KEY_LEFT) && !bRight)
		{
			if (!bLeft)
			{
				// SoundComponent::GetInstance()->PlayCursor();
				// GameListManager::GetInstance()->AddId(-1);
				// ImageLoader::GetInstance()->StartLoadingLoadTexture(GameListManager::GetInstance()->GetGameId());
				// cardsContainer[idFocus].StartAnimationLostFocus();
				// cardsContainer[idFocus].bFocus = false;
				idFocus--;
				// cardsContainer[idFocus].StartAnimationFocus();
				// cardsContainer[idFocus].bFocus = true;
			}
			bLeft = true;
		}
		if (IsKeyDown(KEY_RIGHT) && !bLeft)
		{
			if (!bRight)
			{
				// SoundComponent::GetInstance()->PlayCursor();
				// GameListManager::GetInstance()->AddId(1);
				// ImageLoader::GetInstance()->StartLoadingLoadTexture(GameListManager::GetInstance()->GetGameId());
				// cardsContainer[idFocus].StartAnimationLostFocus();
				// cardsContainer[idFocus].bFocus = false;
				idFocus++;
				// cardsContainer[idFocus].bFocus = true;
				// cardsContainer[idFocus].StartAnimationFocus();
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
			if (positionX > -256 && positionX < 0 && bRight)
			{
				if (idFocus < 3 || idFocus > 6)
				{
					cardContainer->x -= speed;
				}
				lastDirection = Left;
			}
			else if (positionX > 0 && positionX < 256 && bLeft)
			{
				if (idFocus < 3 || idFocus > 6)
				{
					cardContainer->x += speed;
				}
				lastDirection = Right;
			}
			else if (positionX <= -256 || positionX >= 256)
			{
				positionX = 0;
				bRight = false;
				bLeft = false;
				//SetCovers();
				//ImageLoader::GetInstance()->UnloadGameListTextureOutRange();
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
				cardsContainer[i]->x = cardPositions[i];
			}
		}

	}

} //