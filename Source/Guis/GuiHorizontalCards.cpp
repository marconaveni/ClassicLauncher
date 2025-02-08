#include "GuiHorizontalCards.h"

#include <algorithm>
#include <memory>
#include "Core.h"
#include "Guis/GuiMiniCover.h"
#include "Guis/GuiTextBlock.h"

namespace ClassicLauncher
{

    GuiHorizontalCards::GuiHorizontalCards()
        : mGuiTitle(nullptr), mMiniCover(nullptr), mPositionX(0), mIsLeft(false), mIsRight(false), mLastDirection(None), mIdFocus(0), mIdLastFocusSystem(3), mSpeed(22.0f)
    {
    }

    void GuiHorizontalCards::Init()
    {
        mProperties.y = 0;

        EntityManager* pEntityManager = GetApplication()->GetEntityManager();
        mGuiTitle = pEntityManager->CreateEntity<GuiTextBlock>("GuiTitle", Resources::GetFont(), 48, 0);
        mGuiTitle->mProperties.x = 400;
        mGuiTitle->mProperties.y = 154;
        mGuiTitle->SetText("Title");
        mGuiTitle->SetDesiredWidth(1010);
        mGuiTitle->SetTextOverflowPolicy(TextOverflowPolicy::clip);
        AddChild(mGuiTitle);

        for (float cardPosition : mCardPositions)
        {
            const float x = cardPosition;
            const float y = 222.0f;
            auto card = pEntityManager->CreateEntity<GuiCard>("GuiCard", x, y);
            AddChild(card);
            mGuiCards.emplace_back(card);
        }

        mMiniCover = pEntityManager->CreateEntity<GuiMiniCover>("MiniCover");
        mMiniCover->Init();
        AddChild(mMiniCover);

        SetFocus(3, true);
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
        mGuiCards[mIdFocus]->RemoveFocus(bForce);
        mIdFocus = newId;
        mGuiCards[newId]->SetFocus(bForce);
        mIsLeft = true;
        mGuiTitle->SetText(GetApplication()->GetGameListManager()->GetCurrentGameList()->name);
        const float scale = Themes::GetScaleTexture();
        mGuiTitle->mProperties.x = (1280.0f / 2.0f) - ((mGuiTitle->GetMeasureTextBox().GetIntX() / 2));
        mGuiTitle->mProperties.x = Math::Clamp(mGuiTitle->mProperties.x, 135, 1280);
    }

    void GuiHorizontalCards::SetCovers()
    {
        GameListManager* manager = GetApplication()->GetGameListManager();
        SpriteManager* spriteManager = GetApplication()->GetSpriteManager();

        if (manager->GetGameListSize() == 0) return;

        for (int i = 0; i < 10; i++)
        {
            int indexFinal = UtilsFunctionLibrary::SetIndexArray(manager->GetGameId() + i - mIdFocus, manager->GetGameListSize());
            indexFinal = UtilsFunctionLibrary::SetIndexArray(indexFinal, manager->GetGameListSize());
            indexFinal = Math::Clamp(indexFinal, 0, manager->GetGameListSize() - 1);

            const std::string name = std::to_string(indexFinal) + "_CV";
            const std::string path = manager->GetCurrentGameList(indexFinal)->image;

            if (!path.empty())
            {
                const float scale = Themes::GetScaleTexture();
                spriteManager->LoadSprite(name, path, int(228.0f * scale), int(204.0f * scale));
                mGuiCards[i]->SetCover(name);
            }
            else
            {
                mGuiCards[i]->SetCover();
            }
        }

        mMiniCover->SetCovers();

        LOG(LOG_CLASSIC_DEBUG, "Num Sprites Loaded after SetCovers %d", spriteManager->NumSpritesLoaded());
    }

    void GuiHorizontalCards::ChangeList(const CurrentList list)
    {
        Application* pApplication = GetApplication();
        ClearCovers();
        if (list == SystemListSelect)
        {
            pApplication->GetGameListManager()->GetCurrentSystemList()->history.indexCardFocus = mIdFocus;
            pApplication->GetGameListManager()->ChangeGameToSystemList();
            SetFocus(mIdLastFocusSystem, true);
        }
        else
        {
            mIdLastFocusSystem = mIdFocus;
            pApplication->GetGameListManager()->ChangeSystemToGameList();
            SetFocus(pApplication->GetGameListManager()->GetCurrentSystemList()->history.indexCardFocus, true);
        }
    }

    void GuiHorizontalCards::Click()
    {
        mGuiCards[mIdFocus]->Click();
        for (GuiCard*& card : mGuiCards)
        {
            card->SetFrontCard();
        }
    }

    void GuiHorizontalCards::ClearCovers()
    {
        const int size = GetApplication()->GetGameListManager()->GetGameListSize();
        SpriteManager* pSpriteManager = GetApplication()->GetSpriteManager();
        for (int i = 0; i < size; i++)
        {
            const std::string coverName = std::to_string(i) + "_CV";
            const std::string miniCoverName = std::to_string(i) + "_MCV";
            const bool bResult1 = pSpriteManager->DeleteSprite(coverName);
            const bool bResult2 = pSpriteManager->DeleteSprite(miniCoverName);

            if (bResult1 && bResult2)
            {
                LOG(LOG_CLASSIC_TRACE, "Sprite deleted index: %d\n  > Cover: %s\n  > Mini Cover: %s ", i, coverName.c_str(), miniCoverName.c_str());
            }
        }

        LOG(LOG_CLASSIC_DEBUG, "Num Sprites Loaded after ClearCovers %d", pSpriteManager->NumSpritesLoaded());
    }

    int fps = 60;
    void GuiHorizontalCards::Update()
    {
        EntityGui::Update();

        if (InputManager::IsDown(InputName::leftFaceDown, debug))
        {
            mProperties.scaleX += 0.1;
            mProperties.scaleY += 0.1;
            PRINT(TEXT("Set Scale to %.2f", mProperties.scaleX));
        }
        if (InputManager::IsDown(InputName::leftFaceUp, debug))
        {
            mProperties.scaleX -= 0.1;
            mProperties.scaleY -= 0.1;
            PRINT(TEXT("Set Scale to %.2f", mProperties.scaleX));
        }

        if (IsKeyReleased(KEY_SEVEN) || IsKeyReleased(KEY_SIX))
        {
            if (IsKeyReleased(KEY_SIX))
            {
                fps = 15;
            }
            fps += 15;
            SetTargetFPS(fps);
            PRINT(TEXT("Set FPS to %d", fps));
        }

        if (InputManager::IsDown(InputName::rightTriggerFront, main))
        {
            mSpeed = Math::Clamp(256.0f * 60.0f * GetFrameTime(), 0.0f, 256.0f);
        }
        else if (InputManager::IsPress(InputName::leftFaceLeft, main) || InputManager::IsPress(InputName::leftFaceRight, main))
        {
            mSpeed = 20.0f * 60.0f * GetFrameTime();
            GetApplication()->GetTimerManager()->SetTimer(
                mTimerInputSpeed, [&]() { mSpeed = Math::Clamp(88.0f * 60.0f * GetFrameTime(), 0.0f, 256.0f); }, this, 2.5f, false);
        }

        PRINT(TEXT("mSpeed %.8f", mSpeed), 5.0f, "mspeed");

        if (InputManager::IsDown(InputName::leftFaceLeft, main) && !mIsRight)
        {
            if (!mIsLeft)
            {
                Application* pApplication = GetApplication();
                pApplication->GetAudioManager()->PlayCursor();
                pApplication->GetGameListManager()->AddId(-1);
                SetFocus(mIdFocus - 1);
            }
            mIsLeft = true;
        }

        if (InputManager::IsDown(InputName::leftFaceRight, main) && !mIsLeft)
        {
            if (!mIsRight)
            {
                Application* pApplication = GetApplication();
                pApplication->GetAudioManager()->PlayCursor();
                pApplication->GetGameListManager()->AddId(1);
                SetFocus(mIdFocus + 1);
            }
            mIsRight = true;
        }

        if (mIsRight)
        {
            mPositionX = mPositionX - mSpeed;
        }
        else if (mIsLeft)
        {
            mPositionX = mPositionX + mSpeed;
        }

        for (const auto& cardContainer : mGuiCards)
        {
            if (mPositionX > -356 && mPositionX < 0 && mIsRight)
            {
                if (mIdFocus < 3 || mIdFocus > 6)
                {
                    cardContainer->mProperties.x -= mSpeed;
                }
                mLastDirection = Left;
            }
            else if (mPositionX > 0 && mPositionX < 356 && mIsLeft)
            {
                if (mIdFocus < 3 || mIdFocus > 6)
                {
                    cardContainer->mProperties.x += mSpeed;
                }
                mLastDirection = Right;
            }
        }

        if (mPositionX <= -256 || mPositionX >= 256)
        {
            mPositionX = 0;
            mIsRight = false;
            mIsLeft = false;
            SetCovers();
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

        if (!mIsLeft && !mIsRight)
        {
            for (size_t i = 0; i < mGuiCards.size(); i++)
            {
                mGuiCards.at(i)->mProperties.x = (float)mCardPositions[i];
            }
        }
    }

}  // namespace ClassicLauncher