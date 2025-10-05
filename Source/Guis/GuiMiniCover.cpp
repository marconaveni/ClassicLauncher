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
#include "Input/InputManager.h"

namespace ClassicLauncher
{
    GuiMiniCover::GuiMiniCover(GameListManager* gameListManagerRef)
        : m_arrow(nullptr), m_size(32), m_sizeCover(28.0f, 40.0f), m_gameListManagerRef(gameListManagerRef)
        // : m_arrow(nullptr), m_size(23), m_sizeCover(40.0f, 58.0f), m_gameListManagerRef(gameListManagerRef) //test
    {
    }

    void GuiMiniCover::Init()
    {
        SetPosition({0, 505.0f});
        SetSize({1280.0f, 72.0f});

        CreateMiniCovers();

        std::vector<RectFloat> recs = {RectFloat{1236.0f, 0.0f, 30.0f, 18.0f},
                                       RectFloat{1267.0f, 0.0f, 30.0f, 18.0f},
                                       RectFloat{1298.0f, 0.0f, 30.0f, 18.0f}};

        m_arrow = GetEntityManager()->CreateEntity<GuiBase>("arrow");
        m_arrow->SetPosition(GetSize().width / 2, 0);
        m_arrow->m_textureName = "sprite";
        GetAnimationManager().AddAnimationFrame("frame",  0.2f, m_arrow, recs);
        AddChild(m_arrow);
    }

    void GuiMiniCover::CreateMiniCovers()
    {

        if (m_guiMiniCovers.size() > 0)
        {
             m_guiHorizontalBox->SelfDelete();
             RemoveChild(m_guiHorizontalBox);
             m_guiMiniCovers.clear();
        }
        LOG(LOG_CLASSIC_WARNING, "filhos %d" , GetChildren().size());

        m_guiHorizontalBox = GetEntityManager()->CreateEntity<GuiHorizontalBox>("GuiHorizontalBox");
        m_guiHorizontalBox->SetPosition({0, 20.0f});
        m_guiHorizontalBox->SetSpace(1.0f);
        AddChild(m_guiHorizontalBox);

        for (int i = 0; i < m_size; i++)
        {
            MiniCover miniCover;
            miniCover.gui = GetEntityManager()->CreateEntity<GuiBase>("miniCover");
            miniCover.gui->SetSize(m_sizeCover);
            miniCover.gui->SetSource({976.0f, 283.0f}, m_sizeCover);

            miniCover.sizeBox = GetEntityManager()->CreateEntity<GuiSizeBox>("sizeBox");
            miniCover.sizeBox->SetSize(m_sizeCover);

            miniCover.gui->m_textureName = "sprite";

            miniCover.sizeBox->AddChild(miniCover.gui);
            miniCover.sizeBox->SetMargin(Margin::LEFT_ONLY);

            m_guiHorizontalBox->AttachGui(miniCover.sizeBox);
            m_guiHorizontalBox->AddChild(miniCover.sizeBox);
            m_guiMiniCovers.emplace_back(miniCover);
        }
    }

    void GuiMiniCover::Update()
    {
        Entity::Update();
        Animatable::UpdateAnimation();

        for (auto& miniCover : m_guiMiniCovers)
        {
            Texture* textureReference = GetSpriteManager()->GetTexture(miniCover.gui->m_textureName);
            const float scale = ThemesManager::GetScaleRenderer();
            if (textureReference != nullptr && miniCover.gui->m_textureName != "sprite" && miniCover.gui->m_textureName != "transparent")
            {
                const Sizef& textureSize = textureReference->GetSize().ToFloat();
                miniCover.gui->SetSize(textureSize / scale);
                miniCover.gui->SetSource({0, 0}, textureSize / scale);
            }
 
            if (miniCover.focus)
            {
                const float position = miniCover.sizeBox->GetPosition().x + m_guiHorizontalBox->GetPosition().x;
                const float offset = (miniCover.sizeBox->GetSize().width - m_arrow->GetSize().width) / 2;
                m_arrow->SetPosition(position + offset, m_arrow->GetPosition().y);
            }        
            
        }

        if (Keyboard::IsReleased(Keyboard::V))
        {
            m_size = 23; 
            m_sizeCover = Vector2f(40.0f, 58.0f);
            CreateMiniCovers();
        }
        
    }

    void GuiMiniCover::End()
    {
        Entity::End();
    }

    void GuiMiniCover::SetPositionCovers(int numCovers)
    {
        const float x = (GetSize().width - ((m_sizeCover.x + 1) * numCovers)) / 2.0f;
        m_guiHorizontalBox->SetPosition(x , m_guiHorizontalBox->GetPosition().y);
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

        const int numCovers = gameListSize < m_size ? gameListSize + 1 : m_size;

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
                GetSpriteManager()->LoadSprite(name, fileName, m_sizeCover.x * scaleRender, m_sizeCover.y * scaleRender);
            }
            
            m_guiMiniCovers.at(i).focus = (indexFinal == m_gameListManagerRef->GetGameId());

            if (i - 1 >= 0 && i <= static_cast<int>(m_guiMiniCovers.size()) - 2)
            {
                SetCover(name, m_guiMiniCovers.at(i).gui);
                m_guiMiniCovers.at(i).sizeBox->Update();
            }
        }

        SetPositionCovers(numCovers);
    }

    void GuiMiniCover::SetCover(const std::string& name, GuiBase* miniCover)
    {
        
        miniCover->m_textureName = name;
        if (name == "sprite")
        {
            miniCover->SetSize(28.0f, 28.0f);
            miniCover->SetSource({976.0f, 283.0f}, miniCover->GetSize());
        }
        else
        {
            miniCover->SetSize(0.0f, 0.0f);
            miniCover->SetSource(RectFloat{});
        }
    }

    void GuiMiniCover::ClearCovers()
    {
        for (auto& guiCover : m_guiMiniCovers)
        {
            guiCover.focus = false;
            SetCover("transparent", guiCover.gui);
        }
    }

} // namespace ClassicLauncher
