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
		, speed(22)
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
		SetCovers();
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

	void GuiGrid::SetFocus(const int newId)
	{
		if(cardsContainer[idFocus]->IsFocus() && idFocus == newId) return;

		cardsContainer[idFocus]->RemoveFocus();
		//cardsContainer[idFocus].StartAnimationLostFocus();
		idFocus = newId;
		//cardsContainer[newId].StartAnimationFocus();
		cardsContainer[newId]->SetFocus();
		bLeft = true;
	}

	void GuiGrid::SetCovers()
	{
		GameListManager* manager = app->GetGameListManager();
		SpriteManager* SpriteManager = app->GetSpriteManager();

		if (manager->GetGameListSize() == 0) return;

		for (int i = 0; i < 10; i++)
		{
			int indexFinal = UtilsFunctionLibrary::SetIndexArray(manager->GetGameId() + i - idFocus, manager->GetGameListSize());
			indexFinal = UtilsFunctionLibrary::SetIndexArray(indexFinal, manager->GetGameListSize());
			indexFinal = Math::Clamp(indexFinal, 0, manager->GetGameListSize() - 1);


			TraceLog(LOG_DEBUG, "index final %d line %d", indexFinal , __LINE__ );
			const std::string name = std::to_string(indexFinal) + "_CV";
			const std::string path = manager->GetCurrentGameList(indexFinal)->image;

			if(!path.empty())
			{
				SpriteManager->LoadSprite(name, path, 228, 204);
				cardsContainer[i]->SetCover(name);
			}
			else
			{
				cardsContainer[i]->SetCover();
			}
				
			
		}
	}

	void GuiGrid::Update()
	{
		GuiComponent::Update();


		//SetFocus(3);
		if (IsKeyReleased(KEY_V) && !bLeft)
		{
			speed = 22;
		}
		if (IsKeyReleased(KEY_B) && !bLeft)
		{
			speed = 88;
		}




		if (IsKeyDown(KEY_LEFT) && !bRight)
		{
			if (!bLeft)
			{
				app->GetAudioManager()->PlayCursor();
				app->GetGameListManager()->AddId(-1);
				SetFocus(idFocus - 1);
			}
			bLeft = true;
		}
		if (IsKeyDown(KEY_RIGHT) && !bLeft)
		{
			if (!bRight)
			{
				app->GetAudioManager()->PlayCursor();
				app->GetGameListManager()->AddId(1);
				SetFocus(idFocus + 1);
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
				SetCovers();
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