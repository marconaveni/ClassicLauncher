#include "GuiHorizontalCards.h"

#include <algorithm>


#include "Entity/EntityManager.h"
#include "Graphics/SpriteManager.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "Guis/GuiCard.h"
#include "Guis/GuiFrame.h"
#include "Guis/GuiHorizontalBox.h"
#include "Guis/GuiMiniCover.h"
#include "Guis/GuiTextBlock.h"
#include "Helper.h"
#include "Themes/ConfigurationThemes.h"
#include "Utils/Log.h"
#include "Utils/Math.h"
#include "Utils/Resources.h"
#include "Utils/Utils.h"
#include "Window/RayWindow.h"
#include "Themes/Themes.h"


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
        mTransform.position.width = 1280;
        mTransform.position.height = 720;
    }

    void GuiHorizontalCards::Init()
    {

        mGuiTitle = GetEntityManager()->CreateEntity<GuiTextBlock>("GuiTitle", Resources::GetFont(), 48, 0);
        mGuiTitle->mTransform.position.x = 400;
        mGuiTitle->mTransform.position.y = 154;
        mGuiTitle->SetText("Title");
        mGuiTitle->SetDesiredWidth(1010);
        mGuiTitle->SetTextOverflowPolicy(TextOverflowPolicy::clip);
        AddChild(mGuiTitle);

        mHorizontalBox = GetEntityManager()->CreateEntity<GuiHorizontalBox>("Cards_GuiHorizontalBox");
        SetHorizontalBoxValues();
        AddChild(mHorizontalBox);

        for (int i = 0; i < 10; i++)
        {
            auto* card = GetEntityManager()->CreateEntity<GuiCard>("GuiCard", m_gameListManagerRef, GetFocusManager());
            card->CreateCards(0, 0);
            mHorizontalBox->AttachGui(card);
            mGuiCards.emplace_back(card);
        }

        SetPositionHorizontalBox();

        mMiniCover = GetEntityManager()->CreateEntity<GuiMiniCover>("MiniCover", m_gameListManagerRef);
        mMiniCover->Init();
        AddChild(mMiniCover);

        mFrame = GetEntityManager()->CreateEntity<GuiFrame>("Frame", GetFocusManager());
        GetEntityManager()->SetZOrder(mFrame, 80);
        AddChild(mFrame);

        SetFocus(3, true);
    }

    void GuiHorizontalCards::SetHorizontalBoxValues()
    {
        mHorizontalBox->SetAutoSize(true);
        const float space = ThemesManager::Get().mConfigurationThemes.horizontalCardsSpace;
        const float y = ThemesManager::Get().mConfigurationThemes.horizontalCardsPositionY;

        mHorizontalBox->SetSpace(space);
        mHorizontalBox->mTransform.position.y = y;
    }

    void GuiHorizontalCards::SetThemeValue()
    {
        SetHorizontalBoxValues();
        SetPositionHorizontalBox();
    }

    void GuiHorizontalCards::Draw()
    {
        EntityGui::Draw();
    }

    void GuiHorizontalCards::End()
    {
        EntityGui::End();
    }

    void GuiHorizontalCards::SetFocus(const int newId, bool bForce)
    {
        mGuiCards[mIdFocus]->RemoveCardFocus(bForce);
        mIdFocus = newId;
        mGuiCards[newId]->SetCardFocus(bForce);
        mIsLeft = true;

        const GameList* pGameList = m_gameListManagerRef->GetCurrentGameList();
        mGuiTitle->SetText((pGameList) ? pGameList->name : "");
        // const float scale = Themes::GetScaleTexture();
        mGuiTitle->mTransform.position.x = (1280.0f / 2.0f) - ((mGuiTitle->GetMeasureTextBox().x / 2));
        mGuiTitle->mTransform.position.x = Math::Clamp(mGuiTitle->mTransform.position.x, 135, 1280);
    }

    void GuiHorizontalCards::SetCovers()
    {
        //GameListManager* manager = GetApplication()->GetGameListManager();
        //SpriteManager* spriteManager = GetApplication()->GetSpriteManager();

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
                const float scale = ThemesManager::GetScaleTexture();
                GetSpriteManager()->LoadSprite(name, path, int(228.0f * scale), int(204.0f * scale));
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
        mHorizontalBox->mTransform.position.x =
            ((1280 - mHorizontalBox->mTransform.position.width) / 2) +
            ThemesManager::Get().mConfigurationThemes.horizontalCardsPositionX;
    }

    void GuiHorizontalCards::ChangeList(const CurrentList list)
    {
        // GameListManager* pGameListManager = GetApplication()->GetGameListManager();
        ClearCovers();
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
        // SpriteManager* pSpriteManager = GetApplication()->GetSpriteManager();
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

    int fps = 60;
    void GuiHorizontalCards::Update()
    {
        EntityGui::Update();

        // if (InputManager::IsDown(InputName::leftFaceDown, debug))
        //{
        //     mTransform.scaleX += 0.1;
        //     mTransform.scaleY += 0.1;
        //     PRINT(TEXT("Set Scale to %.2f", mTransform.scaleX));
        // }
        // if (InputManager::IsDown(InputName::leftFaceUp, debug))
        //{
        //     mTransform.scaleX -= 0.1;
        //     mTransform.scaleY -= 0.1;
        //     PRINT(TEXT("Set Scale to %.2f", mTransform.scaleX));
        // }

        if (Keyboard::IsReleased(Keyboard::SEVEN) || Keyboard::IsReleased(Keyboard::SIX))
        {
            if (Keyboard::IsReleased(Keyboard::SIX))
            {
                fps = 15;
            }
            fps += 15;
            // rlw::SetTargetFPS(fps);
            // PRINT(TEXT("Set FPS to %d", fps));
        }

        if (InputManager::IsDown(InputName::rightTriggerFront, main))
        {
            mSpeed = Math::Clamp(256.0f * 60.0f * RayWindow::GetFrameTime(), 0.0f, 256.0f);
        }
        else if (InputManager::IsPress(InputName::leftFaceLeft, main) ||
                 InputManager::IsPress(InputName::leftFaceRight, main))
        {
            const float time = RayWindow::GetFrameTime();
            mSpeed = 20.0f * 60.0f * time;
            GetTimerManager()->SetTimer(
                mTimerInputSpeed,
                [&]()
                {
                    const float time = RayWindow::GetFrameTime();
                    mSpeed = Math::Clamp(88.0f * 60.0f * time, 0.0f, 256.0f);
                },
                this,
                2.5f,
                false);
        }

        // PRINT(TEXT("mSpeed %.8f", mSpeed), 5.0f, "mspeed");
        // PRINT(TEXT("mPositionX %.8f", mPositionX), 5.0f, "mPositionX");
        // PRINT(TEXT("mHorizontalBox->mProperties.x %.8f", mHorizontalBox->mProperties.x), 5.0f, "mHorizontalBox->mProperties.x");

        if (InputManager::IsDown(InputName::leftFaceLeft, main) && !mIsRight)
        {
            if (!mIsLeft)
            {
                m_audioManagerRef->PlayCursor();
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
                m_audioManagerRef->PlayCursor();
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
                mHorizontalBox->mTransform.position.x -= mSpeed;
            }
            mLastDirection = Left;
        }
        else if (mPositionX > 0 && mPositionX < 356 && mIsLeft)
        {
            if (mIdFocus < 3 || mIdFocus > 6)
            {
                mHorizontalBox->mTransform.position.x += mSpeed;
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