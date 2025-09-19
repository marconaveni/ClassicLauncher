#include "GuiMiniCover.h"

#include "Entity/EntityManager.h"
#include "Graphics/SpriteManager.h"
#include "Guis/GuiComponent.h"
#include "Guis/GuiHorizontalBox.h"
#include "Guis/GuiSizeBox.h"
#include "Utils/Math.h"
#include "Utils/Utils.h"



#include "Data/GameListManager.h"
#include "Themes/Themes.h"

namespace ClassicLauncher
{
    GuiMiniCover::GuiMiniCover(GameListManager* gameListManagerRef)
        : mArrow(nullptr), mSize(32), mSizeCover(28.0f, 40.0f), m_gameListManagerRef(gameListManagerRef)
    // : mGuiCovers(), mArrow(nullptr), mSize(23), mSizeCover(40.0f, 58.0f) // test
    {
    }

    void GuiMiniCover::Init()
    {
        mTransform.position.y = 505.0f;
        mTransform.position.width = 1280.0f;
        mTransform.position.height = 72.0f;

        mGuiHorizontalBox = GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
        mGuiHorizontalBox->mTransform.position.x = mTransform.position.width / 2.0f;
        mGuiHorizontalBox->mTransform.position.y = 20.0f;
        mGuiHorizontalBox->SetAutoSize(true);
        mGuiHorizontalBox->SetAffectScale(true);
        mGuiHorizontalBox->SetSpace(1.0f);
        AddChild(mGuiHorizontalBox);

        for (int i = 0; i < mSize; i++)
        {
            auto* miniCover = GetEntityManager()->CreateEntity<GuiComponent>("miniCover");
            auto* sizeBox = GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBox");

            miniCover->mTextureName = "transparent";

            sizeBox->mTransform.position.width = mSizeCover.x;
            sizeBox->mTransform.position.height = mSizeCover.y;
            sizeBox->AttachGui(miniCover);

            mGuiHorizontalBox->AttachGui(sizeBox);
            mGuiCovers.emplace_back(miniCover);
            mGuiSizeBoxs.emplace_back(sizeBox);
        }

        std::vector<RectFloat> recs = { RectFloat{1236.0f, 0.0f, 30.0f, 18.0f}, 
                                        RectFloat{1267.0f, 0.0f, 30.0f, 18.0f},
                                        RectFloat{1298.0f, 0.0f, 30.0f, 18.0f}
                                      };

        mArrow = GetEntityManager()->CreateEntity<GuiComponent>("arrow");
        mArrow->mTransform.position.x = mTransform.position.width / 2;
        mArrow->mTextureName = "sprite";
        mArrow->AddAnimationFrame("frame", 0.2f, recs);
        AddChild(mArrow);
    }

    void GuiMiniCover::Update()
    {
        EntityGui::Update();

        for (auto& miniCover : mGuiCovers)
        {
            Texture* textureReference = GetSpriteManager()->GetTexture(miniCover->mTextureName);
            const float scale = ThemesManager::GetScaleTexture();
            if (textureReference != nullptr && miniCover->mTextureName != "sprite" &&
                miniCover->mTransform.position.width == 0 && miniCover->mTransform.position.height == 0)
            {
                miniCover->mTransform.position.width = textureReference->GetSize().x / scale;
                miniCover->mTransform.position.height = textureReference->GetSize().y / scale;
            }
        }
    }

    void GuiMiniCover::End()
    {
        EntityGui::End();
    }

    void GuiMiniCover::SetPositionCovers(int numCovers)
    {
        mGuiHorizontalBox->mTransform.position.x =
            (mTransform.position.width - ((mSizeCover.x + 1) * numCovers)) / 2.0f;
        mArrow->mTransform.position.x =
            mGuiHorizontalBox->mTransform.position.x + ((mSizeCover.x + 1) * numCovers) / 2.0f;
        mArrow->mTransform.position.x = (numCovers % 2 == 0)
                                            ? mArrow->mTransform.position.x
                                            : mArrow->mTransform.position.x - (mArrow->mTransform.position.width / 2);
        mArrow->mTransform.position.x--;

        mGuiHorizontalBox->mTransform.position.x *= mTransform.root.scale.x;
        mArrow->mTransform.position.x *= mTransform.root.scale.x;
    }

    void GuiMiniCover::SetCovers()
    {
        ClearCovers();
        //GameListManager* pManager = GetApplication()->GetGameListManager();
        //SpriteManager* pSpriteManager = GetApplication()->GetSpriteManager();
        const int gameListSize = m_gameListManagerRef->GetGameListSize();
        const float scale = ThemesManager::GetScaleTexture();

        if (gameListSize == 0)
        {
            return;
        }

        const int numCovers = gameListSize < mSize ? gameListSize + 1 : mSize;

        for (int i = 0; i < numCovers; i++)
        {
            const int index = m_gameListManagerRef->GetGameId() + i - static_cast<int>(std::round(numCovers / 2));
            int indexFinal = Utils::SetIndexArray(index, gameListSize);
            indexFinal = Math::Clamp(indexFinal, 0, gameListSize - 1);

            const std::string fileName = m_gameListManagerRef->GetCurrentGameList(indexFinal)->image;
            std::string name = "sprite";

            if (!fileName.empty())
            {
                name = std::to_string(indexFinal) + "_MCV";
                GetSpriteManager()->LoadSprite(name, fileName, mSizeCover.x * scale, mSizeCover.y * scale);
            }

            if (i - 1 >= 0 && i <= static_cast<int>(mGuiCovers.size()) - 2)
            {
                SetCover(name, mGuiCovers.at(i));
            }
        }

        SetPositionCovers(numCovers);
    }

    void GuiMiniCover::SetCover(const std::string& name, GuiComponent* miniCover)
    {
        miniCover->mTextureName = name;
        if (name == "sprite")
        {
            miniCover->mTransform.position.width = 28;
            miniCover->mTransform.position.height = 28;
            miniCover->mTransform.source.x = 976;
            miniCover->mTransform.source.y = 283;
        }
        else
        {
            miniCover->mTransform.position.width = 0;
            miniCover->mTransform.position.height = 0;
            miniCover->mTransform.source.x = 0;
            miniCover->mTransform.source.y = 0;
        }
    }

    void GuiMiniCover::ClearCovers()
    {
        for (auto& guiCover : mGuiCovers)
        {
            SetCover("transparent", guiCover);
        }
    }

} // namespace ClassicLauncher
