#include "GuiWindow.h"

#include "Audio/AudioManager.h"
#include "Entity/EntityManager.h"
#include "Guis/GuiBase.h"
#include "Guis/GuiBlackScreen.h"
#include "Guis/GuiHorizontalCards.h"
#include "Guis/GuiVideoPlayer.h"
#include "Input/InputManager.h"
#include "Themes/ThemesManager.h"
#include "Utils/Log.h"
#include "Utils/ProcessManager.h"
#include "Window/Window.h"

namespace ClassicLauncher
{
    GuiWindow::GuiWindow(GameListManager* gameListManagerRef,
                         AudioManager& audioManagerRef,
                         ProcessManager& processManagerRef)
        : m_guiHorizontalCards(nullptr)
        , m_guiBlackScreen(nullptr)
        , m_guiVideoPlayer(nullptr)
        , m_guiBackground(nullptr)
        , m_gameListManagerRef(gameListManagerRef)
        , m_audioManagerRef(&audioManagerRef)
        , m_processManagerRef(&processManagerRef)
    {
    }

    void GuiWindow::Init()
    {

        const Sizei size{WindowSpecs::Width, WindowSpecs::Height };
        SetSize(size.ToFloat());

        m_guiBackground = GetEntityManager()->CreateEntity<GuiBase>("GuiBackground");

        m_guiBackground->SetPosition(Vector2f{});
        m_guiBackground->SetOffset(Vector2f{});
        m_guiBackground->SetSize(size.ToFloat());
        m_guiBackground->SetSource(0.0f, 562.0f, 21.0f, 720.0f);

        m_guiBackground->m_textureName = "sprite";
        AddChild(m_guiBackground);

        m_guiHorizontalCards = GetEntityManager()->CreateEntity<GuiHorizontalCards>("GuiHorizontalCards",
                                                                                    m_gameListManagerRef,
                                                                                    m_audioManagerRef);
        m_guiHorizontalCards->Init();
        AddChild(m_guiHorizontalCards);

        m_guiBlackScreen = GetEntityManager()->CreateEntity<GuiBlackScreen>("GuiBlackScreen");
        GetEntityManager()->SetZOrder(m_guiBlackScreen, 99);


#ifdef _DEBUG
        InputManager::SetCategory(MAIN_CENTER | DEBUG);
        InitDebug();
#else
        InputManager::SetCategory(MAIN_CENTER);
#endif
    }

    void GuiWindow::Update()
    {
        GuiCanvas::Update();
        if (Keyboard::IsDown(Keyboard::Key::A))
        {
            Vector2f pos = GetPosition();
            pos.x -= 10;
            SetPosition(pos);
        }

        if (Keyboard::IsDown(Keyboard::Key::D))
        {
            Vector2f pos = GetPosition();
            pos.x += 10;
            SetPosition(pos);
        }
        if (Keyboard::IsDown(Keyboard::Key::W))
        {
            Vector2f pos = GetPosition();
            pos.y -= 10;
            SetPosition(pos);
        }
        if (Keyboard::IsDown(Keyboard::Key::S))
        {
            Vector2f pos = GetPosition();
            pos.y += 10;
            SetPosition(pos);
        }

        if (Keyboard::IsDown(Keyboard::Key::KP_ADD))
        {
            SetScale({GetScale() + 0.01f});
        }
        if (Keyboard::IsDown(Keyboard::Key::KP_SUBTRACT))
        {
            SetScale({GetScale() - 0.01f});
        }
        if (Keyboard::IsDown(Keyboard::Key::KP_0))
        {
            SetScale(1);
            SetPosition(0);
        }

        if (InputManager::IsRelease(InputName::rightFaceDown, MAIN_CENTER))
        {
            if (!m_guiHorizontalCards->IsMovement())
            {
                InputManager::DisableInput();
                m_audioManagerRef->PlaySound("click");
                m_guiHorizontalCards->Click();
                if (m_gameListManagerRef->GetCurrentList() == GameListSelect)
                {
                    m_guiBlackScreen->FadeIn();
                }
                else
                {
                    m_guiBlackScreen->FadeInFadeOut();
                }
                GetTimerManager()->SetTimer(m_clickTimer, CALLFUNCTION(OnClick, this), this, 0.5f, false);
            }
        }
        if (InputManager::IsRelease(InputName::rightFaceDown, MAIN_TOP))
        {    
            m_audioManagerRef->PlaySound("click");

        }
        if (InputManager::IsRelease(InputName::rightFaceRight, MAIN_CENTER | MAIN_TOP) &&
            m_gameListManagerRef->GetCurrentList() == GameListSelect) // back
        {
            InputManager::RemoveCategory(MAIN_TOP);
            InputManager::SetCategory(MAIN_CENTER);
            InputManager::DisableInput();
            m_guiBlackScreen->FadeInFadeOut();
            GetTimerManager()->SetTimer(m_clickTimer, CALLFUNCTION(OnBack, this), this, 0.5f, false);
        }
    }

    void GuiWindow::OnClick()
    {
        LOG(LOG_CLASSIC_INFO, "Called OnClick");
        if (m_gameListManagerRef->GetCurrentList() == GameListSelect)
        {
            m_audioManagerRef->Pause();
            m_processManagerRef->CreateProc(m_gameListManagerRef);
        }
        else
        {
            m_guiHorizontalCards->ChangeList(GameListSelect);
            ThemesManager::Get().UpdateTheme();
            GetTimerManager()->SetTimer(m_inputTimer, []() { InputManager::EnableInput(); }, this, 1.0f, false);
        }
    }

    void GuiWindow::OnBack()
    {
        LOG(LOG_CLASSIC_INFO, "Called OnBack");

        if (m_gameListManagerRef->GetCurrentList() == GameListSelect)
        {
            m_guiHorizontalCards->ChangeList(SystemListSelect);
            ThemesManager::Get().UpdateTheme();
            GetTimerManager()->SetTimer(m_inputTimer, []() { InputManager::EnableInput(); }, this, 1.0f, false);
        }
    }

    void GuiWindow::UpdateCovers()
    {
        m_guiHorizontalCards->UpdateCovers();
    }

    void GuiWindow::FadeOutScreen()
    {
        GetTimerManager()->SetTimer(m_inputTimer, [&]() { 
            m_guiBlackScreen->FadeOut();
            m_audioManagerRef->ChangeMusic();
            InputManager::EnableInput(); 
        }, this, 1.0f, false);

    }

#ifdef _DEBUG
    void GuiWindow::InitDebug()
    {
    }
#endif

} // namespace ClassicLauncher