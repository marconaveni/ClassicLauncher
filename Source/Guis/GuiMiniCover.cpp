#include "GuiMiniCover.h"
#include "Core.h"
#include "Graphics/SpriteManager.h"

namespace ClassicLauncher
{
    GuiMiniCover::GuiMiniCover()
        : mGuiCovers(), mArrow(nullptr), mSize(32), mSizeCover(29.0f, 38.0f)
        // : mGuiCovers(), mArrow(nullptr), mSize(16), mSizeCover(39.0f, 48.0f) // test 
    {
    }

    void GuiMiniCover::Init()
    {
        mProperties.y = 505.0f;
        mProperties.width = 1280.0f;
        mProperties.height = 72.0f;

        mGuiHorizontalBox = GetApplication()->GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
        mGuiHorizontalBox->mProperties.x = mProperties.width / 2.0f;
        mGuiHorizontalBox->mProperties.y = 21.0f;
        mGuiHorizontalBox->SetAutoSize(true);
        mGuiHorizontalBox->SetAffectScale(true);
        AddChild(mGuiHorizontalBox);

        for (int i = 0; i < mSize; i++)
        {
            const int x = 29 * i;
            auto miniCover = GetApplication()->GetEntityManager()->CreateEntity<GuiComponent>("miniCover");
            auto sizeBox = GetApplication()->GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBox");

            miniCover->mTextureName = "transparent";

            sizeBox->mProperties.width = mSizeCover.x;
            sizeBox->mProperties.height = mSizeCover.y;
            sizeBox->AttachGui(miniCover);

            mGuiHorizontalBox->AttachGui(sizeBox);
            mGuiCovers.emplace_back(miniCover);
            mGuiSizeBoxs.emplace_back(sizeBox);
        }

        std::vector<Rectangle> recs = { { 798.0f, 1017.0f, 30.0f, 18.0f }, { 834.0f, 1017.0f, 30.0f, 18.0f }, { 870.0f, 1017.0f, 30.0f, 18.0f } };

        mArrow = GetApplication()->GetEntityManager()->CreateEntity<GuiComponent>("arrow");
        mArrow->mProperties.x = mProperties.width / 2;
        mArrow->mTextureName = "sprite";
        mArrow->AddAnimationFrame("frame", 0.2f, recs);
        AddChild(mArrow);
    }

    void GuiMiniCover::Update()
    {
        GuiComponent::Update();

        for (auto& miniCover : mGuiCovers)
        {
            Texture2D* textureReference = GetApplication()->GetSpriteManager()->GetTexture(miniCover->mTextureName);
            const float scale = Themes::GetScaleTexture();
            if (textureReference != nullptr && miniCover->mTextureName != "sprite" && miniCover->mProperties.width == 0 && miniCover->mProperties.height == 0)
            {
                miniCover->mProperties.width = textureReference->width / scale;
                miniCover->mProperties.height = textureReference->height / scale;
            }
        }
    }

    void GuiMiniCover::End()
    {
        GuiComponent::End();
    }

    void GuiMiniCover::SetPositionCovers(int numCovers)
    {
        mGuiHorizontalBox->mProperties.x = (mProperties.width - (mSizeCover.x * numCovers)) / 2.0f;
        mArrow->mProperties.x = (numCovers % 2 == 0) ? (-mProperties.x + mProperties.width) / 2 : (-mProperties.x + mProperties.width - 15) / 2;
        mArrow->mProperties.x--;
        mGuiHorizontalBox->mProperties.x *= mProperties.rootScaleX;
        mArrow->mProperties.x *= mProperties.rootScaleX;
    }

    void GuiMiniCover::SetCovers()
    {
        ClearCovers();
        GameListManager* pManager = GetApplication()->GetGameListManager();
        SpriteManager* pSpriteManager = GetApplication()->GetSpriteManager();
        const int gameListSize = pManager->GetGameListSize();
        const float scale = Themes::GetScaleTexture();

        if (gameListSize == 0)
        {
            return;
        }

        const int numCovers = gameListSize < mSize ? gameListSize + 1 : mSize;

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
                pSpriteManager->LoadSprite(name, fileName, (mSizeCover.x - 1) * scale, mSizeCover.y * scale);
            }

            if (i - 1 >= 0 && i <= static_cast<int>(mGuiCovers.size()) - 2)
            {
                SetCover(name, mGuiCovers.at(i));
            }
        }

        SetPositionCovers(numCovers);
    }

    void GuiMiniCover::SetCover(std::string name, GuiComponent* miniCover)
    {
        miniCover->mTextureName = name;
        if (name == "sprite")
        {
            miniCover->mProperties.width = 28;
            miniCover->mProperties.height = 28;
            miniCover->mProperties.sourceX = 1260;
            miniCover->mProperties.sourceY = 1056;
        }
        else
        {
            miniCover->mProperties.width = 0;
            miniCover->mProperties.height = 0;
            miniCover->mProperties.sourceX = 0;
            miniCover->mProperties.sourceY = 0;
        }
    }

    void GuiMiniCover::ClearCovers()
    {
        for (auto& guiCover : mGuiCovers)
        {
            SetCover("transparent", guiCover);
        }
    }

}  // namespace ClassicLauncher
