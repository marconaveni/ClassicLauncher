#include "GuiHorizontalCards.h"

#include <algorithm>

#include "Audio/AudioManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/SpriteManager.h"
#include "Guis/Components/GuiHorizontalBox.h"
#include "Guis/GuiCard.h"
#include "Guis/GuiFrame.h"
#include "Guis/GuiMiniCover.h"
#include "Guis/GuiTextBlock.h"
#include "Helper.h"
#include "Input/InputManager.h"
#include "Themes/ConfigurationThemes.h"
#include "Themes/ThemesManager.h"
#include "Utils/Log.h"
#include "Utils/Math.h"
#include "Utils/Resources.h"
#include "Utils/Utils.h"
#include "Window/RayWindow.h"


namespace ClassicLauncher
{

    GuiHorizontalCards::GuiHorizontalCards(GameListManager* gameListManagerRef, AudioManager* audioManagerRef)
        : m_guiTitle(nullptr)
        , m_miniCover(nullptr)
        , m_positionX(0)
        , m_isLeft(false)
        , m_isRight(false)
        , m_isNeedUpdate(false)
        , m_lastDirection(None)
        , m_idFocus(0)
        , m_idLastFocusSystem(3)
        , m_speed(22.0f)
        , m_gameListManagerRef(gameListManagerRef)
        , m_audioManagerRef(audioManagerRef)
    {
        SetSize(Sizef{1280.0f, 720.0f});
    }

    void GuiHorizontalCards::Init()
    {

        m_guiTitle = GetEntityManager()->CreateEntity<GuiTextBlock>("GuiTitle", Resources::GetFontFile(), 48, 0);

        const float x = (1280 - 1010) / 2;
        m_guiTitle->SetPosition(x, 154.0f);
        m_guiTitle->SetSize(1010.0f, 32.0f);

        m_guiTitle->SetText("Title");
        m_guiTitle->SetTextOverflowPolicy(TextOverflowPolicy::CLIP);
        m_guiTitle->SetTextCenter(true);
        AddChild(m_guiTitle);

        m_horizontalBox = GetEntityManager()->CreateEntity<GuiHorizontalBox>("Cards_GuiHorizontalBox");
        AddChild(m_horizontalBox);


        for (int i = 0; i < 10; i++)
        {
            auto* card = GetEntityManager()->CreateEntity<GuiCard>("GuiCard",
                                                                   m_gameListManagerRef,
                                                                   GetFocusManager(),
                                                                   m_audioManagerRef);
            card->CreateCards(0, 0);
            m_horizontalBox->AttachGui(card);
            m_horizontalBox->AddChild(card);
            m_guiCards.emplace_back(card);
        }

        m_miniCover = GetEntityManager()->CreateEntity<GuiMiniCover>("MiniCover", m_gameListManagerRef);
        m_miniCover->Init();
        AddChild(m_miniCover);

        m_frame = GetEntityManager()->CreateEntity<GuiFrame>("Frame");
        GetEntityManager()->SetZOrder(m_frame, 1);
        SetFocus(3, true);
    }

    // void GuiHorizontalCards::SetHorizontalBoxValues()
    // {
    //     // const float space = ThemesManager::GetConfigurationThemes().horizontalCardsSpace;
    //     // const float x = m_horizontalBox->GetPosition().x;
    //     // const float y = ThemesManager::GetConfigurationThemes().horizontalCardsPositionY;

    //     // m_horizontalBox->SetSpace(space);
    //     // m_horizontalBox->SetPosition(x, y);
    // }

    void GuiHorizontalCards::SetPositionHorizontalBox()
    {
        
        const float offsetX = ThemesManager::GetConfigurationThemes().horizontalCardsPositionX;

        const float x = ((GetSize().width - m_horizontalBox->GetSize().width) / 2) + offsetX;
        const float y = ThemesManager::GetConfigurationThemes().horizontalCardsPositionY;

        m_horizontalBox->SetPosition(x, y);
    }

    void GuiHorizontalCards::SetThemeValue()
    {
        const float space = ThemesManager::GetConfigurationThemes().horizontalCardsSpace;
        m_horizontalBox->SetSpace(space);
        SetPositionHorizontalBox();

        const float cardWidth = ((m_horizontalBox->GetSize().width / 10) * m_horizontalBox->GetScale().x + m_horizontalBox->GetSpace())  ;
        
        
        //const float minX = ((m_guiCards[0]->GetSize().width + m_horizontalBox->GetSpace()) * 3 + m_horizontalBox->GetPosition().x) * m_horizontalBox->GetScale().x ;
        //const float maxX = ((m_guiCards[0]->GetSize().width + m_horizontalBox->GetSpace()) * 6 + m_horizontalBox->GetPosition().x) * m_horizontalBox->GetScale().x ;
        const float minX = ((m_guiCards[0]->GetSize().width + m_horizontalBox->GetSpace()) * 3 + m_horizontalBox->GetPosition().x)  ;
        const float maxX = ((m_guiCards[0]->GetSize().width + m_horizontalBox->GetSpace()) * 6 + m_horizontalBox->GetPosition().x) ;
        m_frame->SetLimitArea(RectFloat{minX , 0.0f, maxX, 720.0f});
    }

    void GuiHorizontalCards::Draw()
    {
        Entity::Draw();
    }

    void GuiHorizontalCards::End()
    {
        Entity::End();
    }

    void GuiHorizontalCards::SetFocus(const int newId, bool bForce)
    {
        m_guiCards[m_idFocus]->CloseVideo();

        m_idFocus = newId;
        m_guiCards[newId]->SetCardFocus(bForce);

        if (bForce)
        {
            if (newId <= 3)
            {
                m_isLeft = true;
            }
            else if (newId >= 4)
            {
                m_isRight = true;
            }
            SetPositionHorizontalBox();
        }


        const GameList* pGameList = m_gameListManagerRef->GetCurrentGameList();
        m_guiTitle->SetText((pGameList) ? pGameList->name : "");
        m_frame->SetFrame();
    }

    void GuiHorizontalCards::SetCovers()
    {

        if (m_gameListManagerRef->GetGameListSize() == 0)
        {
            return;
        }

        for (int i = 0; i < 10; i++)
        {
            int indexFinal = Utils::SetIndexArray(m_gameListManagerRef->GetGameId() + i - m_idFocus,
                                                  m_gameListManagerRef->GetGameListSize());
            indexFinal = Utils::SetIndexArray(indexFinal, m_gameListManagerRef->GetGameListSize());
            indexFinal = Math::Clamp(indexFinal, 0, m_gameListManagerRef->GetGameListSize() - 1);

            const std::string name = std::to_string(indexFinal) + "_CV";
            const std::string path = m_gameListManagerRef->GetCurrentGameList(indexFinal)->image;

            if (!path.empty())
            {
                const float renderScale = ThemesManager::GetScaleRenderer();
                GetSpriteManager()->LoadSprite(name, path, int(228.0f * renderScale), int(204.0f * renderScale));
                m_guiCards[i]->SetCover(name);
            }
            else
            {
                m_guiCards[i]->SetCover();
            }
        }

        m_miniCover->SetCovers();
        SetPositionHorizontalBox();

        LOG(LOG_CLASSIC_DEBUG, "Num Sprites Loaded after SetCovers %d", GetSpriteManager()->NumSpritesLoaded());
    }

    void GuiHorizontalCards::ChangeList(const CurrentList list)
    {
        ClearCovers();
        CancelMultiply();
        if (list == SystemListSelect)
        {
            m_gameListManagerRef->GetCurrentSystemList()->history.indexCardFocus = m_idFocus;
            m_gameListManagerRef->ChangeGameToSystemList();
            SetFocus(m_idLastFocusSystem, true);
        }
        else
        {
            m_idLastFocusSystem = m_idFocus;
            m_gameListManagerRef->ChangeSystemToGameList();

            // If GameList fails it returns to the system selection menu.
            if (m_gameListManagerRef->GetGameListSize() == 0)
            {
                ChangeList(CurrentList::SystemListSelect);
            }

            SetFocus(m_gameListManagerRef->GetCurrentSystemList()->history.indexCardFocus, true);
        }
    }

    void GuiHorizontalCards::Click()
    {
        m_guiCards[m_idFocus]->Click();
        m_frame->Click();
        for (GuiCard*& card : m_guiCards)
        {
            card->SetFrontCard();
        }
    }

    void GuiHorizontalCards::ClearCovers()
    {
        const int size = m_gameListManagerRef->GetGameListSize();

        for (int i = 0; i < size; i++)
        {
            const std::string coverName = std::to_string(i) + "_CV";
            const std::string miniCoverName = std::to_string(i) + "_MCV";
            const bool bResult1 = GetSpriteManager()->DeleteSprite(coverName);
            const bool bResult2 = GetSpriteManager()->DeleteSprite(miniCoverName);

            if (bResult1 && bResult2)
            {
                LOG(LOG_CLASSIC_TRACE,
                    "Sprite deleted index: %d\n  > Cover: %s\n  > Mini Cover: %s ",
                    i,
                    coverName.c_str(),
                    miniCoverName.c_str());
            }
        }

        LOG(LOG_CLASSIC_DEBUG, "Num Sprites Loaded after ClearCovers %d", GetSpriteManager()->NumSpritesLoaded());
    }

    bool GuiHorizontalCards::IsMovement() const
    {
        return m_positionX != 0;
    }

    void GuiHorizontalCards::SetSpeedCards()
    {
        if (InputManager::IsDown(InputName::rightTriggerFront, MAIN))
        {
            m_multiply = 256.0f;
        }
        else if ((InputManager::IsDown(InputName::leftFaceLeft, MAIN) ||
                  InputManager::IsDown(InputName::leftFaceRight, MAIN)) &&
                 !m_isPress)
        {
            PRINT(TEXT("IsPress"));
            CancelMultiply();
            m_isPress = true;
            GetTimerManager()->SetTimer(
                m_timerInputSpeed,
                [&]()
                {
                    PRINT(TEXT("Está acionando"));
                    const float time = RayWindow::GetFrameTime();
                    m_multiply = 88.0f;
                },
                this,
                2.5f,
                false);
        }
        else if (InputManager::IsRelease(InputName::leftFaceLeft, MAIN) ||
                 InputManager::IsRelease(InputName::leftFaceRight, MAIN))
        {
            PRINT(TEXT("IsRelease"));
            CancelMultiply();
        }

        m_speed = Math::Clamp(m_multiply * 60.0f * RayWindow::GetFrameTime(), 0.0f, 256.0f);
    }

    void GuiHorizontalCards::CancelMultiply()
    {
        m_isPress = false;
        m_multiply = 22.0f;
        GetTimerManager()->ClearTimer(m_timerInputSpeed);
    }

    void GuiHorizontalCards::Update()
    {
        Entity::Update();


        SetSpeedCards();

        const float sizeCard = (m_guiCards[0]->GetSize().width + m_horizontalBox->GetSpace()) * m_horizontalBox->GetScale().x ;
        PRINT(TEXT("sizeCard: %.2f", sizeCard), 2.5f, "sizeCard");


        if (InputManager::IsDown(InputName::leftFaceLeft, MAIN) && !m_isRight)
        {
            if (!m_isLeft)
            {
                m_audioManagerRef->PlaySound("cursor");
                m_gameListManagerRef->AddId(-1);
                SetFocus(m_idFocus - 1);
            }
            m_isLeft = true;
            m_isNeedUpdate = true;
        }

        if (InputManager::IsDown(InputName::leftFaceRight, MAIN) && !m_isLeft)
        {
            if (!m_isRight)
            {
                m_audioManagerRef->PlaySound("cursor");
                m_gameListManagerRef->AddId(1);
                SetFocus(m_idFocus + 1);
            }
            m_isRight = true;
            m_isNeedUpdate = true;
        }

        if (m_isRight)
        {
            m_positionX = m_positionX - m_speed;
        }
        else if (m_isLeft)
        {
            m_positionX = m_positionX + m_speed;
        }

        if (m_positionX < 0 && m_isRight)
        {
            if (m_idFocus < 3 || m_idFocus > 6)
            {
                const float x = m_horizontalBox->GetPosition().x - m_speed;
                const float y = m_horizontalBox->GetPosition().y;
                m_horizontalBox->SetPosition(x, y);
            }
            m_lastDirection = Left;
        }
        else if (m_positionX > 0 && m_isLeft)
        {
            if (m_idFocus < 3 || m_idFocus > 6)
            {
                const float x = m_horizontalBox->GetPosition().x + m_speed;
                const float y = m_horizontalBox->GetPosition().y;
                m_horizontalBox->SetPosition(x, y);
            }
            m_lastDirection = Right;
        }

        if (m_positionX <= -sizeCard || m_positionX >= sizeCard)
        {
            m_positionX = 0;
            m_isRight = false;
            m_isLeft = false;
            m_isNeedUpdate = true;
            SetCovers();
            // todo: add clean textures of vram outside of the screen
        }

        if (m_idFocus < 3 || m_idFocus > 6)
        {
            if (m_lastDirection == Left && m_positionX == 0)
            {
                std::rotate(m_guiCards.begin(), m_guiCards.begin() + 1, m_guiCards.end());
                m_lastDirection = None;
                m_idFocus = Math::Clamp(m_idFocus, 3, 6);
            }
            else if (m_lastDirection == Right && m_positionX == 0)
            {
                std::rotate(m_guiCards.rbegin(), m_guiCards.rbegin() + 1, m_guiCards.rend());
                m_lastDirection = None;
                m_idFocus = Math::Clamp(m_idFocus, 3, 6);
            }
        }
        else
        {
            m_frame->SetFrame();
        }

        UpdateCards();
    }

    void GuiHorizontalCards::UpdateCards()
    {
        if (!m_isLeft && !m_isRight && m_isNeedUpdate)
        {
            m_horizontalBox->ClearAll();
            for (auto& guiCard : m_guiCards)
            {
                m_horizontalBox->AttachGui(guiCard);
            }
            m_isNeedUpdate = false;
        }
    }

} // namespace ClassicLauncher