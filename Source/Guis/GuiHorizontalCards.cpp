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
        : mGuiTitle(nullptr)
        , mMiniCover(nullptr)
        , mPositionX(0)
        , mIsLeft(false)
        , mIsRight(false)
        , mIsNeedUpdate(false)
        , mLastDirection(None)
        , mIdFocus(0)
        , mIdLastFocusSystem(3)
        , mSpeed(22.0f)
        , m_gameListManagerRef(gameListManagerRef)
        , m_audioManagerRef(audioManagerRef)
    {
        SetSize(Sizef{1280.0f, 720.0f});
    }

    void GuiHorizontalCards::Init()
    {

        mGuiTitle = GetEntityManager()->CreateEntity<GuiTextBlock>("GuiTitle", Resources::GetFont(), 48, 0);

        const float x = (1280 - 1010) / 2;
        mGuiTitle->SetPosition(x, 154.0f);
        mGuiTitle->SetSize(1010.0f, 32.0f);

        mGuiTitle->SetText("Title");
        //mGuiTitle->SetDesiredWidth(1010);
        mGuiTitle->SetTextOverflowPolicy(TextOverflowPolicy::clip);
        mGuiTitle->SetTextCenter(true);
        AddChild(mGuiTitle);

        mHorizontalBox = GetEntityManager()->CreateEntity<GuiHorizontalBox>("Cards_GuiHorizontalBox");
        AddChild(mHorizontalBox);
        SetHorizontalBoxValues();

        for (int i = 0; i < 10; i++)
        {
            auto* card = GetEntityManager()->CreateEntity<GuiCard>("GuiCard",
                                                                   m_gameListManagerRef,
                                                                   GetFocusManager(),
                                                                   m_audioManagerRef);
            card->CreateCards(0, 0);
            mHorizontalBox->AttachGui(card);
            mHorizontalBox->AddChild(card);
            mGuiCards.emplace_back(card);
        }

        SetPositionHorizontalBox();

        mMiniCover = GetEntityManager()->CreateEntity<GuiMiniCover>("MiniCover", m_gameListManagerRef);
        mMiniCover->Init();
        AddChild(mMiniCover);

        mFrame = GetEntityManager()->CreateEntity<GuiFrame>("Frame", GetFocusManager());
        GetEntityManager()->SetZOrder(mFrame, 1);
        SetFocus(3, true);
    }

    void GuiHorizontalCards::SetHorizontalBoxValues()
    {
        const float space = ThemesManager::Get().mConfigurationThemes.horizontalCardsSpace;
        const float x = mHorizontalBox->GetPosition().x;
        const float y = ThemesManager::Get().mConfigurationThemes.horizontalCardsPositionY;

        mHorizontalBox->SetSpace(space);
        mHorizontalBox->SetPosition(x, y);
    }

    void GuiHorizontalCards::SetThemeValue()
    {
        SetHorizontalBoxValues();
        SetPositionHorizontalBox();
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
        mIdFocus = newId;
        mGuiCards[newId]->SetCardFocus(bForce);
        mIsLeft = true;

        const GameList* pGameList = m_gameListManagerRef->GetCurrentGameList();
        mGuiTitle->SetText((pGameList) ? pGameList->name : "");
        mFrame->SetFrame();
    }

    void GuiHorizontalCards::SetCovers()
    {

        if (m_gameListManagerRef->GetGameListSize() == 0)
        {
            return;
        }

        for (int i = 0; i < 10; i++)
        {
            int indexFinal = Utils::SetIndexArray(m_gameListManagerRef->GetGameId() + i - mIdFocus,
                                                  m_gameListManagerRef->GetGameListSize());
            indexFinal = Utils::SetIndexArray(indexFinal, m_gameListManagerRef->GetGameListSize());
            indexFinal = Math::Clamp(indexFinal, 0, m_gameListManagerRef->GetGameListSize() - 1);

            const std::string name = std::to_string(indexFinal) + "_CV";
            const std::string path = m_gameListManagerRef->GetCurrentGameList(indexFinal)->image;

            if (!path.empty())
            {
                const float renderScale = ThemesManager::GetScaleRenderer();
                GetSpriteManager()->LoadSprite(name, path, int(228.0f * renderScale), int(204.0f * renderScale));
                mGuiCards[i]->SetCover(name);
            }
            else
            {
                mGuiCards[i]->SetCover();
            }
        }

        mMiniCover->SetCovers();
        SetPositionHorizontalBox();

        LOG(LOG_CLASSIC_DEBUG, "Num Sprites Loaded after SetCovers %d", GetSpriteManager()->NumSpritesLoaded());
    }

    void GuiHorizontalCards::SetPositionHorizontalBox()
    {
        const float x = ((GetSize().width - mHorizontalBox->GetSize().width) / 2) + ThemesManager::Get().mConfigurationThemes.horizontalCardsPositionX;
        const float y = mHorizontalBox->GetPosition().y;
        mHorizontalBox->SetPosition(x, y);
    }

    void GuiHorizontalCards::ChangeList(const CurrentList list)
    {
        ClearCovers();
        CancelMultiply();
        if (list == SystemListSelect)
        {
            m_gameListManagerRef->GetCurrentSystemList()->history.indexCardFocus = mIdFocus;
            m_gameListManagerRef->ChangeGameToSystemList();
            SetFocus(mIdLastFocusSystem, true);
        }
        else
        {
            mIdLastFocusSystem = mIdFocus;
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
        mGuiCards[mIdFocus]->Click();
        mFrame->Click();
        for (GuiCard*& card : mGuiCards)
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
        return mPositionX != 0;
    }

    void GuiHorizontalCards::SetSpeedCards()
    {
        if (InputManager::IsDown(InputName::rightTriggerFront, main))
        {
            m_multiply = 256.0f;
        }
        else if ((InputManager::IsDown(InputName::leftFaceLeft, main) ||
                  InputManager::IsDown(InputName::leftFaceRight, main)) &&
                 !m_isPress)
        {
            PRINT(TEXT("IsPress"));
            CancelMultiply();
            m_isPress = true;
            GetTimerManager()->SetTimer(
                mTimerInputSpeed,
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
        else if (InputManager::IsRelease(InputName::leftFaceLeft, main) ||
                 InputManager::IsRelease(InputName::leftFaceRight, main))
        {
            PRINT(TEXT("IsRelease"));
            CancelMultiply();
        }

        mSpeed = Math::Clamp(m_multiply * 60.0f * RayWindow::GetFrameTime(), 0.0f, 256.0f);
    }

    void GuiHorizontalCards::CancelMultiply()
    {
        m_isPress = false;
        m_multiply = 22.0f;
        GetTimerManager()->ClearTimer(mTimerInputSpeed);
    }

    int fps = 60;
    void GuiHorizontalCards::Update()
    {
        Entity::Update();


        SetSpeedCards();


        if (InputManager::IsDown(InputName::leftFaceLeft, main) && !mIsRight)
        {
            if (!mIsLeft)
            {
                m_audioManagerRef->PlaySound("cursor");
                m_gameListManagerRef->AddId(-1);
                SetFocus(mIdFocus - 1);
            }
            mIsLeft = true;
            mIsNeedUpdate = true;
        }

        if (InputManager::IsDown(InputName::leftFaceRight, main) && !mIsLeft)
        {
            if (!mIsRight)
            {
                m_audioManagerRef->PlaySound("cursor");
                m_gameListManagerRef->AddId(1);
                SetFocus(mIdFocus + 1);
            }
            mIsRight = true;
            mIsNeedUpdate = true;
        }

        if (mIsRight)
        {
            mPositionX = mPositionX - mSpeed;
        }
        else if (mIsLeft)
        {
            mPositionX = mPositionX + mSpeed;
        }

        if (mPositionX > -356 && mPositionX < 0 && mIsRight)
        {
            if (mIdFocus < 3 || mIdFocus > 6)
            {
                const float x = mHorizontalBox->GetPosition().x - mSpeed;
                const float y = mHorizontalBox->GetPosition().y;
                mHorizontalBox->SetPosition(x, y);
            }
            mLastDirection = Left;
        }
        else if (mPositionX > 0 && mPositionX < 356 && mIsLeft)
        {
            if (mIdFocus < 3 || mIdFocus > 6)
            {
                const float x = mHorizontalBox->GetPosition().x + mSpeed;
                const float y = mHorizontalBox->GetPosition().y;
                mHorizontalBox->SetPosition(x, y);
            }
            mLastDirection = Right;
        }

        if (mPositionX <= -256 || mPositionX >= 256)
        {
            mPositionX = 0;
            mIsRight = false;
            mIsLeft = false;
            SetCovers();
            mIsNeedUpdate = true;
            // todo: add clean textures of vram outside of the screen
        }

        if (mIdFocus < 3 || mIdFocus > 6)
        {
            if (mLastDirection == Left && mPositionX == 0)
            {
                std::rotate(mGuiCards.begin(), mGuiCards.begin() + 1, mGuiCards.end());
                mLastDirection = None;
                mIdFocus = Math::Clamp(mIdFocus, 3, 6);
            }
            else if (mLastDirection == Right && mPositionX == 0)
            {
                std::rotate(mGuiCards.rbegin(), mGuiCards.rbegin() + 1, mGuiCards.rend());
                mLastDirection = None;
                mIdFocus = Math::Clamp(mIdFocus, 3, 6);
            }
        }
        else
        {
            mFrame->SetFrame();
        }

        UpdateCards();
    }

    void GuiHorizontalCards::UpdateCards()
    {
        if (!mIsLeft && !mIsRight && mIsNeedUpdate)
        {
            mHorizontalBox->ClearAll();
            for (auto& guiCard : mGuiCards)
            {
                mHorizontalBox->AttachGui(guiCard);
            }
            mIsNeedUpdate = false;
        }
    }

} // namespace ClassicLauncher