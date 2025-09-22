#include "GuiWindow.h"

#include "Audio/AudioManager.h"
#include "Entity/EntityManager.h"
#include "Guis/GuiBlackScreen.h"
#include "Guis/GuiComponent.h"
#include "Guis/GuiHorizontalCards.h"
#include "Guis/GuiVideoPlayer.h"
#include "Input/InputManager.h"
#include "Themes/ThemesManager.h"
#include "Utils/Log.h"
#include "Utils/ProcessManager.h"

namespace ClassicLauncher
{
    GuiWindow::GuiWindow(GameListManager* gameListManagerRef,
                         AudioManager& audioManagerRef,
                         ProcessManager& processManagerRef)
        : m_guiHorizontalCards(nullptr)
        , mGuiBlackScreen(nullptr)
        , mGuiVideoPlayer(nullptr)
        , mGuiBackground(nullptr)
        , m_gameListManagerRef(gameListManagerRef)
        , m_audioManagerRef(&audioManagerRef)
        , m_processManagerRef(&processManagerRef)
    {
    }

    void GuiWindow::Init()
    {

        m_transform.position.width = 1280.0f; //  todo: refactor    (float)pApplication->GetSpecification().width;
        m_transform.position.height = 720.0f; //  todo: refactor    (float)pApplication->GetSpecification().height;

        mGuiBackground = GetEntityManager()->CreateEntity<GuiComponent>("GuiBackground");

        mGuiBackground->m_transform.position.x = 0;
        mGuiBackground->m_transform.offset.x = 10;
        mGuiBackground->m_transform.offset.y = 10;
        mGuiBackground->m_transform.position.y = 0;
        mGuiBackground->m_transform.position.width = 1280;
        mGuiBackground->m_transform.position.height = 720;

        mGuiBackground->m_transform.source.x = 0;
        mGuiBackground->m_transform.source.y = 562;
        mGuiBackground->m_transform.source.width = 21;
        mGuiBackground->m_transform.source.height = 720;

        mGuiBackground->mTextureName = "sprite";
        AddChild(mGuiBackground);

        m_guiHorizontalCards = GetEntityManager()->CreateEntity<GuiHorizontalCards>("GuiHorizontalCards",
                                                                                    m_gameListManagerRef,
                                                                                    m_audioManagerRef);
        m_guiHorizontalCards->Init();
        AddChild(m_guiHorizontalCards);


#ifdef _DEBUG
        InputManager::SetCategory(main | debug);
#else
        InputManager::SetCategory(main);
#endif
    }

    void GuiWindow::Update()
    {
        GuiCanvas::Update();
        if (Keyboard::IsDown(Keyboard::Key::A))
        {
            m_transform.position.x -= 10;
        }
        if (Keyboard::IsDown(Keyboard::Key::D))
        {
            m_transform.position.x += 10;
        }
        if (Keyboard::IsDown(Keyboard::Key::W))
        {
            m_transform.position.y -= 10;
        }
        if (Keyboard::IsDown(Keyboard::Key::S))
        {
            m_transform.position.y += 10;
        }

        if (Keyboard::IsDown(Keyboard::Key::KP_ADD))
        {
            m_transform.scale.x += 0.1;
            m_transform.scale.y += 0.1;
        }
        if (Keyboard::IsDown(Keyboard::Key::KP_SUBTRACT))
        {
            m_transform.scale.x -= 0.1;
            m_transform.scale.y -= 0.1;
        }

        if (InputManager::IsRelease(InputName::rightFaceDown, main))
        {
            InputManager::DisableInput();
            m_audioManagerRef->PlaySound("click");
            m_guiHorizontalCards->Click();
            if (m_gameListManagerRef->GetCurrentList() == GameListSelect)
            {
               // mGuiBlackScreen->FadeIn();
            }
            else
            {
               // mGuiBlackScreen->FadeInFadeOut();
            }
            GetTimerManager()->SetTimer(mClickTimer, CALLFUNCTION(OnClick, this), this, 0.5f, false);
        }
        if (InputManager::IsRelease(InputName::rightFaceRight, main) &&
            m_gameListManagerRef->GetCurrentList() == GameListSelect) // back
        {
            InputManager::DisableInput();
            // mGuiBlackScreen->FadeInFadeOut();
            GetTimerManager()->SetTimer(mClickTimer, CALLFUNCTION(OnBack, this), this, 0.5f, false);
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
            // ThemesManager::Get().LoadTheme();
            GetTimerManager()->SetTimer(mInputTimer, []() { InputManager::EnableInput(); }, this, 1.0f, false);
        }
    }

    void GuiWindow::OnBack()
    {
        LOG(LOG_CLASSIC_INFO, "Called OnBack");

        if (m_gameListManagerRef->GetCurrentList() == GameListSelect)
        {
            m_guiHorizontalCards->ChangeList(SystemListSelect);
            // ThemesManager::Get().LoadTheme();
            GetTimerManager()->SetTimer(mInputTimer, []() { InputManager::EnableInput(); }, this, 1.0f, false);
        }
    }

    void GuiWindow::Teste()
    {
    }
} // namespace ClassicLauncher