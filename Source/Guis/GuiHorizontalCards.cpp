#include "GuiHorizontalCards.h"

#include <algorithm>

#include "Audio/AudioManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/SpriteManager.h"
#include "Guis/Components/GuiHorizontalBox.h"
#include "Guis/GuiBase.h"
#include "Guis/GuiCard.h"
#include "Guis/GuiFrame.h"
#include "Guis/GuiHintBar.h"
#include "Guis/GuiMenu.h"
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
#include "Window/Window.h"


namespace ClassicLauncher
{

    GuiHorizontalCards::GuiHorizontalCards(const EntityContext& entityContext,
                                           GameListManager* gameListManagerRef,
                                           AudioManager* audioManagerRef,
                                           Window* window)
        : Entity(entityContext)
        , Animatable(window)
        , m_gameListManagerRef(gameListManagerRef)
        , m_audioManagerRef(audioManagerRef)
    {
        SetSize(Sizef{1280.0f, 720.0f});
    }

    void GuiHorizontalCards::Init()
    {

        m_guiMenuBackground = GetEntityManager()->CreateEntity<GuiBase>("GuiBase");
        m_guiMenuBackground->SetPosition(Vector2f{127, 151});
        m_guiMenuBackground->SetSize(Vector2f{1026, 54});
        m_guiMenuBackground->SetSource(RectFloat{22, 1164, 1026, 54});
        m_guiMenuBackground->m_textureName = "sprite";
        AddChild(m_guiMenuBackground);

        m_guiTitle = GetEntityManager()->CreateEntity<GuiTextBlock>("GuiTitle");
        m_guiTitle->LoadNewFont(Resources::GetFontFile(), 48, 0);
        const float x = (1280 - 1010) / 2;
        m_guiTitle->SetPosition(x, 154.0f);
        m_guiTitle->SetSize(1010.0f, 32.0f);

        m_guiTitle->SetText("Title");
        m_guiTitle->SetTextOverflowPolicy(GuiTextBlock::TextOverflowPolicy::Clip);
        m_guiTitle->SetTextCenter(true);
        AddChild(m_guiTitle);

        m_horizontalBox = GetEntityManager()->CreateEntity<GuiHorizontalBox>("Cards_GuiHorizontalBox");
        AddChild(m_horizontalBox);


        for (int i = 0; i < 10; i++)
        {
            auto* card = GetEntityManager()->CreateEntity<GuiCard>("GuiCard", m_gameListManagerRef, GetFocusManager(), m_audioManagerRef, GetWindow());
            card->CreateCards(0, 0);
            m_horizontalBox->AttachGui(card);
            m_horizontalBox->AddChild(card);
            m_guiCards.emplace_back(card);
        }

        m_miniCover = GetEntityManager()->CreateEntity<GuiMiniCover>("MiniCover", m_gameListManagerRef, GetWindow());
        m_miniCover->Init();
        AddChild(m_miniCover);

        m_hintBar = GetEntityManager()->CreateEntity<GuiHintBar>("GuiHintBar");
        m_hintBar->SetPosition(180, 594.0f);
        m_hintBar->AddHint({1488, 784, 36, 36}, "0");
        m_hintBar->AddHint({1545, 562, 81, 36}, "1");
        m_hintBar->AddHint({1340, 562, 36, 36}, "2");
        m_hintBar->AddHint({1303, 562, 36, 36}, "3");

        AddChild(m_hintBar);

        m_guiTopBar = GetEntityManager()->CreateEntity<GuiBase>("GuiBase");
        m_guiTopBar->SetSize(Vector2f{1280, 102});
        m_guiTopBar->SetSource(RectFloat{22, 706, 1280, 102});
        m_guiTopBar->SetPosition(Vector2f{0.0f, -6.0f});
        m_guiTopBar->m_textureName = "sprite";
        AddChild(m_guiTopBar);

        m_guiBottomBar = GetEntityManager()->CreateEntity<GuiBase>("GuiBase");
        m_guiBottomBar->SetPosition(Vector2f{0, 630});
        m_guiBottomBar->SetSize(Vector2f{1280, 90});
        m_guiBottomBar->SetSource(RectFloat{22, 562, 1280, 90});
        m_guiBottomBar->m_textureName = "sprite";
        AddChild(m_guiBottomBar);

        m_guiMenu = GetEntityManager()->CreateEntity<GuiMenu>("GuiMenu");
        m_guiMenu->Init();
        m_guiTopBar->AddChild(m_guiMenu);


        m_frame = GetEntityManager()->CreateEntity<GuiFrame>("Frame", GetWindow());
        GetEntityManager()->SetZOrder(m_frame, 1);
        SetFocus(3, true);
    }

    void GuiHorizontalCards::SetPositionHorizontalBox()
    {

        const float offsetX = ThemesManager::GetConfigurationThemes().horizontalCardsPositionX;

        const float x = ((GetSize().width - m_horizontalBox->GetSize().width) / 2) + offsetX;
        const float y = ThemesManager::GetConfigurationThemes().horizontalCardsPositionY;

        m_horizontalBox->SetPosition(x, y);
    }

    void GuiHorizontalCards::FocusAnimationBar(InputCategory category)
    {

        if (!m_topBarAnimation)
        {
            return;
        }

        Transform target = m_guiTopBar->GetTransform();
        if (category == MainTop)
        {
            target.position.y = 0;
        }
        if (category == MainCenter)
        {
            target.position.y = -6;
        }
        GetAnimationManager().StartAnimation("bartop", 0.10f, m_guiTopBar, target, Ease::EaseLinearNone, false);
    }

    void GuiHorizontalCards::SetThemeValue()
    {
        const float space = ThemesManager::GetConfigurationThemes().horizontalCardsSpace;
        m_horizontalBox->SetSpace(space);
        SetPositionHorizontalBox();

        const float cardWidth = ((m_horizontalBox->GetSize().width / 10) * m_horizontalBox->GetScale().x + m_horizontalBox->GetSpace());

        const float minX = ((m_guiCards[0]->GetSize().width + m_horizontalBox->GetSpace()) * 3 + m_horizontalBox->GetPosition().x);
        const float maxX = ((m_guiCards[0]->GetSize().width + m_horizontalBox->GetSpace()) * 6 + m_horizontalBox->GetPosition().x);
        const float minY = ThemesManager::GetConfigurationThemes().offsetTopFrame;
        const float maxY = ThemesManager::GetConfigurationThemes().offsetBottomFrame;
        m_frame->SetLimitArea(RectFloat{minX, minY + 27.0f, maxX, maxY + 720.0f});
        m_hintBar->SetTextColor(ThemesManager::GetConfigurationThemes().hintBarFooterColor);
        m_guiTitle->SetColor(ThemesManager::GetConfigurationThemes().titleColor);
        m_topBarAnimation = ThemesManager::GetConfigurationThemes().topBarAnimation;
    }

    void GuiHorizontalCards::Draw()
    {
        Entity::Draw();
    }

    void GuiHorizontalCards::End()
    {
        Entity::End();
    }

    void GuiHorizontalCards::SetFocus(const int newId, bool force)
    {
        m_guiCards[m_idFocus]->CloseVideo();

        m_idFocus = newId;
        m_guiCards[newId]->SetCardFocus();

        if (force)
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


        const GameList* gameList = m_gameListManagerRef->GetCurrentGameList();
        m_guiTitle->SetText((gameList) ? gameList->name : "");
        m_frame->SetFrame();
        SetTextHintBar();
    }

    void GuiHorizontalCards::UpdateCovers()
    {

        if (m_gameListManagerRef->GetGameListSize() == 0)
        {
            return;
        }

        for (int i = 0; i < 10; i++)
        {
            int indexFinal = Utils::SetIndexArray(m_gameListManagerRef->GetGameId() + i - m_idFocus, m_gameListManagerRef->GetGameListSize());
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

        LOG(LogClassicDebug, "Num Sprites Loaded after SetCovers %d", GetSpriteManager()->NumSpritesLoaded());
    }


    void GuiHorizontalCards::RemoveCoversFromScreen()
    {
        if (m_loadTexturesCards < 20 && Texture::GetTextureSizeBytes() < 209715200)
        {
            return;
        }

        const int gameListSize = m_gameListManagerRef->GetGameListSize();
        if (gameListSize == 0)
        {
            return;
        }
        std::vector<bool> keepCover(static_cast<size_t>(gameListSize), false);
        std::vector<bool> keepMiniCover(static_cast<size_t>(gameListSize), false);

        for (int i = 0; i < 10; i++)
        {
            int index = m_gameListManagerRef->GetGameId() + i - m_idFocus;
            index = Utils::SetIndexArray(index, gameListSize);
            index = Math::Clamp(index, 0, gameListSize - 1);
            keepCover[static_cast<size_t>(index)] = true;
        }

        const int themesNumCovers = ThemesManager::GetConfigurationThemes().numCovers;
        const int numMiniCovers = gameListSize < themesNumCovers ? gameListSize + 1 : themesNumCovers;

        for (int i = 0; i < numMiniCovers; i++)
        {
            int index = m_gameListManagerRef->GetGameId() + i - (numMiniCovers / 2);
            index = Utils::SetIndexArray(index, gameListSize);
            index = Math::Clamp(index, 0, gameListSize - 1);
            keepMiniCover[static_cast<size_t>(index)] = true;
        }

        for (int i = 0; i < gameListSize; i++)
        {
            if (!keepCover[i])
            {
                GetSpriteManager()->DeleteSprite(std::to_string(i) + "_CV");
            }
            if (!keepMiniCover[i])
            {
                GetSpriteManager()->DeleteSprite(std::to_string(i) + "_MCV");
            }
        }
        m_loadTexturesCards = 0;
    }

    void GuiHorizontalCards::ChangeList(const CurrentList list)
    {
        ClearCovers();
        CancelMultiply();
        if (list == CurrentList::SystemListSelect)
        {
            m_gameListManagerRef->GetCurrentSystemList()->history.indexCardFocus = m_idFocus;
            m_gameListManagerRef->ChangeGameToSystemList();
            SetFocus(m_idLastFocusSystem, true);
        }
        else
        {
            m_idLastFocusSystem = m_idFocus;
            m_gameListManagerRef->ChangeSystemToGameList();

            GameSystemList* currentGameList = m_gameListManagerRef->GetCurrentSystemList();

            // If GameList or ptr of CurrentList fails it returns to the system selection menu.
            if (!currentGameList || m_gameListManagerRef->GetGameListSize() == 0)
            {
                ChangeList(CurrentList::SystemListSelect);
            }

            SetFocus(currentGameList->history.indexCardFocus, true);
        }
        SetMenuBar();
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
            const bool resultCover = GetSpriteManager()->DeleteSprite(coverName);
            const bool resultMiniCover = GetSpriteManager()->DeleteSprite(miniCoverName);

            if (resultCover && resultMiniCover)
            {
                LOG(LogClassicTrace, "Sprite deleted index: %d\n  > Cover: %s\n  > Mini Cover: %s ", i, coverName.c_str(), miniCoverName.c_str());
            }
        }

        LOG(LogClassicDebug, "Num Sprites Loaded after ClearCovers %d", GetSpriteManager()->NumSpritesLoaded());
    }

    bool GuiHorizontalCards::IsMovement() const
    {
        return m_positionX != 0;
    }

    void GuiHorizontalCards::SetSpeedCards()
    {
        if (InputManager::IsDown(InputName::R1, MainCenter))
        {
            m_multiply = 256.0f;
        }
        else if ((InputManager::IsDown(InputName::DPadLeft, MainCenter) || InputManager::IsDown(InputName::DPadRight, MainCenter)) && !m_isPress)
        {
            PRINT(TEXT("IsPress"));
            CancelMultiply();
            m_isPress = true;
            GetTimerManager()->SetTimer(
                m_timerInputSpeed,
                [&]()
                {
                    PRINT(TEXT("Está acionando"));
                    // const float time = GetWindow()->GetFrameTime();
                    m_multiply = 88.0f;
                },
                this,
                2.5f,
                false);
        }
        else if (InputManager::IsRelease(InputName::DPadLeft, MainCenter) || InputManager::IsRelease(InputName::DPadRight, MainCenter) ||
                 InputManager::IsRelease(InputName::R1, MainCenter))
        {
            PRINT(TEXT("IsRelease"));
            CancelMultiply();
        }

        m_speed = Math::Clamp(m_multiply * 60.0f * GetWindow()->GetFrameTime(), 0.0f, 256.0f);
    }

    void GuiHorizontalCards::SetTextHintBar()
    {
        CurrentList currentList = m_gameListManagerRef->GetCurrentList();
        if (currentList == CurrentList::SystemListSelect)
        {
            m_hintBar->SetText(0, "Select");
            m_hintBar->SetVisibility(1, false);
            m_hintBar->SetText(2, "Accept");
            m_hintBar->SetText(3, "Exit");
        }
        else
        {
            m_hintBar->SetText(0, "Select");
            m_hintBar->SetText(1, "Options");
            m_hintBar->SetVisibility(1, true);
            m_hintBar->SetText(2, "Start Game");
            m_hintBar->SetText(3, "Back");
        }
    }

    void GuiHorizontalCards::SetMenuBar()
    {
        CurrentList currentList = m_gameListManagerRef->GetCurrentList();
        bool enable = (currentList == CurrentList::GameListSelect);

        // m_guiMenu->EnableButton(enable, 2); // todo enable
        m_guiMenu->EnableButton(enable, 3);
        m_guiMenu->EnableButton(enable, 4);
    }

    void GuiHorizontalCards::CancelMultiply()
    {
        m_isPress = false;
        m_multiply = 22.0f;
        GetTimerManager()->ClearTimer(m_timerInputSpeed);
        FocusAnimationBar(MainCenter);
    }

    void GuiHorizontalCards::UpdateInput()
    {
        if (InputManager::IsDown(InputName::DPadLeft, MainCenter) && !m_isRight)
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
        else if (InputManager::IsDown(InputName::DPadLeft, MainTop))
        {
            if (!m_frame->IsFrameMove())
            {
                m_audioManagerRef->PlaySound("cursor");
                m_guiMenu->SetButtonFocus(-1);
            }
        }
        else if (InputManager::IsDown(InputName::DPadRight, MainCenter) && !m_isLeft)
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
        else if (InputManager::IsDown(InputName::DPadRight, MainTop))
        {
            if (!m_frame->IsFrameMove())
            {
                m_audioManagerRef->PlaySound("cursor");
                m_guiMenu->SetButtonFocus(1);
            }
        }
        else if (InputManager::IsDown(InputName::DPadUp, MainCenter))
        {
            if (!m_frame->IsFrameMove())
            {
                m_audioManagerRef->PlaySound("cursor");
                m_guiMenu->SetButtonFocus(0);
                InputManager::RemoveCategory(MainCenter);
                InputManager::SetCategory(MainTop);
                FocusAnimationBar(MainTop);
            }
        }
        else if (InputManager::IsDown(InputName::DPadDown, MainTop))
        {
            if (!m_frame->IsFrameMove())
            {
                m_audioManagerRef->PlaySound("cursor");
                SetFocus(m_idFocus);
                InputManager::RemoveCategory(MainTop);
                InputManager::SetCategory(MainCenter);
                FocusAnimationBar(MainCenter);
            }
        }
    }

    void GuiHorizontalCards::Update()
    {
        Entity::Update();
        Animatable::UpdateAnimation();

        SetSpeedCards();
        UpdateInput();

        const float sizeCard = (m_guiCards[0]->GetSize().width + m_horizontalBox->GetSpace()) * m_horizontalBox->GetScale().x;

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
            m_lastDirection = Direction::Left;
        }
        else if (m_positionX > 0 && m_isLeft)
        {
            if (m_idFocus < 3 || m_idFocus > 6)
            {
                const float x = m_horizontalBox->GetPosition().x + m_speed;
                const float y = m_horizontalBox->GetPosition().y;
                m_horizontalBox->SetPosition(x, y);
            }
            m_lastDirection = Direction::Right;
        }

        if (m_positionX <= -sizeCard || m_positionX >= sizeCard)
        {
            m_positionX = 0;
            m_isRight = false;
            m_isLeft = false;
            m_isNeedUpdate = true;
            UpdateCovers();
            RemoveCoversFromScreen();
            m_loadTexturesCards++;
        }

        if (m_idFocus < 3 || m_idFocus > 6)
        {
            if (m_lastDirection == Direction::Left && m_positionX == 0)
            {
                std::rotate(m_guiCards.begin(), m_guiCards.begin() + 1, m_guiCards.end());
                m_lastDirection = Direction::None;
                m_idFocus = Math::Clamp(m_idFocus, 3, 6);
            }
            else if (m_lastDirection == Direction::Right && m_positionX == 0)
            {
                std::rotate(m_guiCards.rbegin(), m_guiCards.rbegin() + 1, m_guiCards.rend());
                m_lastDirection = Direction::None;
                m_idFocus = Math::Clamp(m_idFocus, 3, 6);
            }
        }
        else
        {
            m_frame->SetFrame();
        }

        UpdateCards();

        const float x = (GetSize().width - m_hintBar->GetSize().width) / 2.0f;
        m_hintBar->SetPosition(x, m_hintBar->GetPosition().y);

        const float m = (GetSize().width - m_guiMenu->GetSize().width) / 2.0f;
        m_guiMenu->SetPosition(m, 33.0f);
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
