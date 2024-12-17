#include "GuiMiniCover.h"
#include "Graphics/SpriteManager.h"
#include "Utils/Core.h"

namespace ClassicLauncher
{
    GuiMiniCover::GuiMiniCover()
        : app(&Application::Get())
    {
    }

    void GuiMiniCover::Init()
    {
        x = 20;
        y = 526;
        for (int i = 0; i < 30; i++)
	    {
            const int x = 29 * i;
			auto miniCover = app->GetEntityManager()->CreateEntity<GuiComponent>();
            miniCover->x = x;

            miniCover->width = 144;
            miniCover->height = 114;
            miniCover->sourceX = 1116;
            miniCover->sourceY = 1131;
            miniCover->scaleWidth = 29;
            miniCover->scaleHeight = 29;
            miniCover->textureName = "sprite";

        	AddChild(miniCover.get());
			guiCovers.emplace_back(miniCover);
	    }
    }

    void GuiMiniCover::Update()
    {
        GuiComponent::Update();
        ClearCovers();
        SetCovers();
    }

    void GuiMiniCover::End()
    {
        GuiComponent::End();
    }

    void GuiMiniCover::SetPosition(int numCovers)
    {
        const int size = (1280 - (29 * numCovers)) / 2;
        x = size;

	    //const float arrowPositionX = (numCovers % 2 == 0) ? 640.0f : 625.0f;
	    //arrow->position.position = Vector2{ arrowPositionX , positionY };

    }

    void GuiMiniCover::SetCovers()
    {

        GameListManager* manager = app->GetGameListManager();
        SpriteManager* spriteManager = app->GetSpriteManager();

        if (manager->GetGameListSize() == 0) return;
        
        const int numCovers = manager->GetGameListSize() < 30 ? manager->GetGameListSize() : 30;

        for (int i = 0; i < numCovers; i++)
        {
            int indexFinal = UtilsFunctionLibrary::SetIndexArray(manager->GetGameId() + i - static_cast<int>(std::round(numCovers / 2)), manager->GetGameListSize());
            indexFinal = Math::Clamp(indexFinal, 0, manager->GetGameListSize() - 1);
        
            const std::string fileName = manager->GetCurrentGameList(indexFinal)->image;
            if (!fileName.empty())
            {
                const std::string name = std::to_string(indexFinal) + "_MCV";
                spriteManager->LoadSprite(name, fileName, 28, 45);
                SetCover(name, guiCovers[i].get());
            }
            else
            {
                SetCover("sprite", guiCovers[i].get());
            }
            
        }

        SetPosition(numCovers);
    }

    void GuiMiniCover::SetCover(std::string name, GuiComponent* miniCover)
    {

        miniCover->textureName = name;
        if (name == "sprite")
        {
            miniCover->width = 144;
            miniCover->height = 114;
            miniCover->sourceX = 1116;
            miniCover->sourceY = 1131;
            miniCover->scaleWidth = 29;
            miniCover->scaleHeight = 29;
        }
        else
        {
            miniCover->width = 0;
            miniCover->height = 0;
            miniCover->sourceX = 0;
            miniCover->sourceY = 0;
            miniCover->scaleWidth = 0;
            miniCover->scaleHeight = 0;
        }
        
    }

    void GuiMiniCover::ClearCovers()
    {
        for (auto& guiCover : guiCovers)
        {
            SetCover("transparent", guiCover.get());
        }
    }

} // namespace ClassicLauncher

