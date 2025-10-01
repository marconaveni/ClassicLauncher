#include "GuiMiniCover.h"

#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/SpriteManager.h"
#include "Guis/Components/GuiHorizontalBox.h"
#include "Guis/Components/GuiSizeBox.h"
#include "Guis/GuiBase.h"
#include "Helper.h"
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
        SetPosition({0, 505.0f});
        SetSize({1280.0f, 72.0f});
        // m_transform.position.y = 505.0f;
        // m_transform.position.width = 1280.0f;
        // m_transform.position.height = 72.0f;

        mGuiHorizontalBox = GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
        mGuiHorizontalBox->SetPosition({0, 20.0f});
        // mGuiHorizontalBox->m_transform.position.x = 0; //m_transform.position.width / 2.0f;
        // mGuiHorizontalBox->m_transform.position.y = 20.0f;
        mGuiHorizontalBox->SetSpace(1.0f);
        AddChild(mGuiHorizontalBox);

        for (int i = 0; i < mSize; i++)
        {
            MiniCover miniCover;
            miniCover.gui = GetEntityManager()->CreateEntity<GuiBase>("miniCover");
            miniCover.gui->SetSize(mSizeCover);
            // miniCover.gui->m_transform.position.width = mSizeCover.x;
            // miniCover.gui->m_transform.position.height = mSizeCover.y;


            miniCover.gui->SetSource({976.0f, 283.0f}, mSizeCover);
           // miniCover.gui->m_transform.source.x = 976;
           // miniCover.gui->m_transform.source.y = 283;
           // miniCover.gui->m_transform.source.width = mSizeCover.x;
           // miniCover.gui->m_transform.source.height = mSizeCover.y;

            miniCover.gui->mTextureName = "sprite";


            mGuiHorizontalBox->AttachGui(miniCover.gui);
            mGuiCovers.emplace_back(miniCover);
        }

        std::vector<RectFloat> recs = {RectFloat{1236.0f, 0.0f, 30.0f, 18.0f},
                                       RectFloat{1267.0f, 0.0f, 30.0f, 18.0f},
                                       RectFloat{1298.0f, 0.0f, 30.0f, 18.0f}};

        mArrow = GetEntityManager()->CreateEntity<GuiBase>("arrow");
        mArrow->SetPosition(GetSize().width / 2, 0);
        //mArrow->m_transform.position.x = m_transform.position.width / 2;
        mArrow->mTextureName = "sprite";
        GetAnimationManager().AddAnimationFrame("frame",  0.2f, mArrow, recs);
        AddChild(mArrow);
    }

    void GuiMiniCover::Update()
    {
        Entity::Update();
        Animatable::UpdateAnimation();

        for (auto& miniCover : mGuiCovers)
        {
            Texture* textureReference = GetSpriteManager()->GetTexture(miniCover.gui->mTextureName);
            const float scale = ThemesManager::GetScaleRenderer();
            if (textureReference != nullptr && miniCover.gui->mTextureName != "sprite" && miniCover.gui->mTextureName != "transparent")
            {
                const Sizef& textureSize = textureReference->GetSize().ToFloat();
                miniCover.gui->SetSize(textureSize / scale);
                miniCover.gui->SetSource({0, 0}, textureSize / scale);
                // miniCover.gui->m_transform.position.width = textureReference->GetSize().x / scale;
                // miniCover.gui->m_transform.position.height = textureReference->GetSize().y / scale;
                // miniCover.gui->m_transform.source.width = textureReference->GetSize().x / scale;
                // miniCover.gui->m_transform.source.height = textureReference->GetSize().y / scale;
            }
 
            if (miniCover.focus)
            {
                const float position = miniCover.gui->GetPosition().x + mGuiHorizontalBox->GetPosition().x;
                mArrow->SetPosition(position, mArrow->GetPosition().y);
                //mArrow->m_transform.position.x = miniCover.gui->m_transform.position.x + mGuiHorizontalBox->m_transform.position.x;
            }
            
            
        }
    }

    void GuiMiniCover::End()
    {
        Entity::End();
    }

    void GuiMiniCover::SetPositionCovers(int numCovers)
    {
        const float x = (GetSize().width - ((mSizeCover.x + 1) * numCovers)) / 2.0f;
        mGuiHorizontalBox->SetPosition(x , mGuiHorizontalBox->GetPosition().y);
    }

    void GuiMiniCover::SetCovers()
    {
        ClearCovers();

        const int gameListSize = m_gameListManagerRef->GetGameListSize();
        const float scaleRender = ThemesManager::GetScaleRenderer();

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
                GetSpriteManager()->LoadSprite(name, fileName, mSizeCover.x * scaleRender, mSizeCover.y * scaleRender);
            }
            
            mGuiCovers.at(i).focus = (indexFinal == m_gameListManagerRef->GetGameId());

            if (i - 1 >= 0 && i <= static_cast<int>(mGuiCovers.size()) - 2)
            {
                SetCover(name, mGuiCovers.at(i).gui);
            }
        }

        SetPositionCovers(numCovers);
    }

    void GuiMiniCover::SetCover(const std::string& name, GuiBase* miniCover)
    {
        
        miniCover->mTextureName = name;
        if (name == "sprite")
        {
            miniCover->SetSize(28.0f, 28.0f);
            miniCover->SetSource({976.0f, 283.0f}, miniCover->GetSize());
            // miniCover->m_transform.position.width = 28;
            // miniCover->m_transform.position.height = 28;
            // miniCover->m_transform.source.x = 976;
            // miniCover->m_transform.source.y = 283;
            // miniCover->m_transform.source.width = 28;
            // miniCover->m_transform.source.height = 28;
        }
        else
        {
            miniCover->SetSize(0.0f, 0.0f);
            miniCover->SetSource(RectFloat{});
            // miniCover->m_transform.position.width = 28;
            // miniCover->m_transform.position.height = 28;
            // miniCover->m_transform.source.width = 0;
            // miniCover->m_transform.source.height = 0;
            // miniCover->m_transform.source.x = 0;
            // miniCover->m_transform.source.y = 0;
        }
    }

    void GuiMiniCover::ClearCovers()
    {
        for (auto& guiCover : mGuiCovers)
        {
            guiCover.focus = false;
            SetCover("transparent", guiCover.gui);
        }
    }

} // namespace ClassicLauncher
