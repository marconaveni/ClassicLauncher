#include "GuiMiniCover.h"

#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/SpriteManager.h"
#include "Guis/GuiComponent.h"
#include "Guis/GuiHorizontalBox.h"
#include "Guis/GuiSizeBox.h"
#include "Themes/ThemesManager.h"
#include "Utils/Math.h"
#include "Utils/Utils.h"

namespace ClassicLauncher
{
    GuiMiniCover::GuiMiniCover(GameListManager* gameListManagerRef)
        : mArrow(nullptr), mSize(32), mSizeCover(28.0f, 40.0f), m_gameListManagerRef(gameListManagerRef)
    // : mGuiCovers(), mArrow(nullptr), mSize(23), mSizeCover(40.0f, 58.0f) // test
    {
    }

    void GuiMiniCover::Init()
    {
        m_transform.position.y = 505.0f;
        m_transform.position.width = 1280.0f;
        m_transform.position.height = 72.0f;

        mGuiHorizontalBox = GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
        mGuiHorizontalBox->m_transform.position.x = 0; //m_transform.position.width / 2.0f;
        mGuiHorizontalBox->m_transform.position.y = 20.0f;
        mGuiHorizontalBox->SetAutoSize(true);
        //mGuiHorizontalBox->SetAffectScale(true);
        //mGuiHorizontalBox->SetSpace(1.0f);
        AddChild(mGuiHorizontalBox);

        for (int i = 0; i < mSize; i++)
        {
            auto* miniCover = GetEntityManager()->CreateEntity<GuiComponent>("miniCover");
            //auto* sizeBox = GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBox");
            miniCover->m_transform.position.width = mSizeCover.x;
            miniCover->m_transform.position.height = mSizeCover.y;
            
            miniCover->m_transform.source.x = 976;
            miniCover->m_transform.source.y = 283;
            miniCover->m_transform.source.width = mSizeCover.x;
            miniCover->m_transform.source.height = mSizeCover.y;

            miniCover->mTextureName = "sprite";

            mGuiHorizontalBox->AttachGui(miniCover);
            mGuiCovers.emplace_back(miniCover);
        }

        std::vector<RectFloat> recs = {RectFloat{1236.0f, 0.0f, 30.0f, 18.0f},
                                       RectFloat{1267.0f, 0.0f, 30.0f, 18.0f},
                                       RectFloat{1298.0f, 0.0f, 30.0f, 18.0f}};

        mArrow = GetEntityManager()->CreateEntity<GuiComponent>("arrow");
        mArrow->m_transform.position.x = m_transform.position.width / 2;
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
            const float scale = ThemesManager::GetScaleRenderer();
            if (textureReference != nullptr && miniCover->mTextureName != "sprite")
            {
                miniCover->m_transform.position.width = textureReference->GetSize().x / scale;
                miniCover->m_transform.position.height = textureReference->GetSize().y / scale;
                miniCover->m_transform.source.width = textureReference->GetSize().x / scale;
                miniCover->m_transform.source.height = textureReference->GetSize().y / scale;
            }
        }
    }

    void GuiMiniCover::End()
    {
        EntityGui::End();
    }

    void GuiMiniCover::SetPositionCovers(int numCovers)
    {

        //mGuiHorizontalBox->m_transform.position.x = mGuiHorizontalBox->m_transform.position.width / 2; 
        mGuiHorizontalBox->m_transform.position.x =
            (m_transform.position.width - ((mSizeCover.x + 1) * numCovers)) / 2.0f;
        mArrow->m_transform.position.x =
            mGuiHorizontalBox->m_transform.position.x + ((mSizeCover.x + 1) * numCovers) / 2.0f;
        mArrow->m_transform.position.x =
            (numCovers % 2 == 0) ? mArrow->m_transform.position.x
                                 : mArrow->m_transform.position.x - (mArrow->m_transform.position.width / 2);
        mArrow->m_transform.position.x--;

        //mGuiHorizontalBox->m_transform.position.x *= m_transform.root.scale.x;
        //mArrow->m_transform.position.x *= m_transform.root.scale.x;
    }

    void GuiMiniCover::SetCovers()
    {
        ClearCovers();

        const int gameListSize = m_gameListManagerRef->GetGameListSize();
        const float scale = ThemesManager::GetScaleRenderer();

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
            miniCover->m_transform.position.width = 28;
            miniCover->m_transform.position.height = 28;
            miniCover->m_transform.source.x = 976;
            miniCover->m_transform.source.y = 283;
            miniCover->m_transform.source.width = 28;
            miniCover->m_transform.source.height = 28;
        }
        else
        {
            miniCover->m_transform.source.x = 0;
            miniCover->m_transform.source.y = 0;
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
