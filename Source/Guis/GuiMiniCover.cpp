#include "GuiMiniCover.h"
#include "Core.h"
#include "Graphics/SpriteManager.h"

namespace ClassicLauncher
{
    GuiMiniCover::GuiMiniCover()
        : mApplication(&Application::Get()), mSize(32)
    {
    }

    void GuiMiniCover::Init()
    {
        x = 20;
        y = 298;
        for (int i = 0; i < mSize; i++)
        {
            const int x = 29 * i;
            auto miniCover = mApplication->GetEntityManager()->CreateEntity<GuiComponent>("miniCover");
            miniCover->x = x;
            miniCover->textureName = "transparent";

            AddChild(miniCover.get());
            mGuiCovers.emplace_back(miniCover);
        }
    }

    void GuiMiniCover::Update()
    {
        GuiComponent::Update();
    }

    void GuiMiniCover::End()
    {
        GuiComponent::End();
    }

    void GuiMiniCover::SetPosition(int numCovers)
    {
        x = (1280 - (29 * numCovers)) / 2;

        // const float arrowPositionX = (numCovers % 2 == 0) ? 640.0f : 625.0f;
        // arrow->position.position = Vector2{ arrowPositionX , positionY };
    }

    void GuiMiniCover::SetCovers()
    {
        ClearCovers();
        GameListManager* pManager = mApplication->GetGameListManager();
        SpriteManager* pSpriteManager = mApplication->GetSpriteManager();
        const int gameListSize = pManager->GetGameListSize();

        if (gameListSize == 0) return;

        const int numCovers = gameListSize < mSize ? gameListSize : mSize;

        for (int i = 0; i < numCovers; i++)
        {
            const int index = pManager->GetGameId() + i - static_cast<int>(std::round(numCovers / 2));
            int indexFinal = UtilsFunctionLibrary::SetIndexArray(index, gameListSize);
            indexFinal = Math::Clamp(indexFinal, 0, gameListSize - 1);

            const std::string fileName = pManager->GetCurrentGameList(indexFinal)->image;
            std::string name = "sprite";

            if (!fileName.empty())
            {
                name = std::to_string(indexFinal) + "_MCV";
                pSpriteManager->LoadSprite(name, fileName, 28, 45);
            }

            if (i - 1 >= 0 && i <= mGuiCovers.size() - 2)
            {
                SetCover(name, mGuiCovers.at(i).get());
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
        for (auto& guiCover : mGuiCovers)
        {
            SetCover("transparent", guiCover.get());
        }
    }

}  // namespace ClassicLauncher
