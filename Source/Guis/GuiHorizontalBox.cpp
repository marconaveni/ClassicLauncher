#include "GuiHorizontalBox.h"

#include <algorithm>
#include <memory>
#include "Core.h"
#include "Guis/GuiMiniCover.h"
#include "Utils/Resources.h"

namespace ClassicLauncher
{

    GuiHorizontalBox::GuiHorizontalBox()
        : mApplication(&Application::Get()), mPositionX(0), mIsLeft(false), mIsRight(false), mLastDirection(None), mIdFocus(0), mIdLastFocusSystem(3), mSpeed(22.0f) {};

    void GuiHorizontalBox::Init()
    {
        mProperties.y = 0;

        mGuiTitle = mApplication->GetEntityManager()->CreateEntity<GuiTextBox>("GuiTitle", Resources::GetFont().c_str(), 48, 0);
        mGuiTitle->mProperties.x = 400;
        mGuiTitle->mProperties.y = 154;
        mGuiTitle->SetText("Title");
        mGuiTitle->SetDesiredWidth(1010);
        mGuiTitle->SetTextOverflowPolicy(TextOverflowPolicy::clip);
        AddChild(mGuiTitle.get());

        for (int cardPosition : mCardPositions)
        {
            const int x = cardPosition;
            auto card = mApplication->GetEntityManager()->CreateEntity<GuiCard>("GuiCard", x , 222);
            AddChild(card.get());
            mGuiCards.emplace_back(card);
        }

        mMiniCover = mApplication->GetEntityManager()->CreateEntity<GuiMiniCover>("MiniCover");
        mMiniCover->Init();
        AddChild(mMiniCover.get());

        SetFocus(3, true);
    }

    void GuiHorizontalBox::Draw()
    {
        GuiComponent::Draw();
    }

    void GuiHorizontalBox::End()
    {
        GuiComponent::End();
    }

    void GuiHorizontalBox::SetFocus(const int newId, bool bForce)
    {
        mGuiCards[mIdFocus]->RemoveFocus(bForce);
        mIdFocus = newId;
        mGuiCards[newId]->SetFocus(bForce);
        mIsLeft = true;
        mGuiTitle->SetText(mApplication->GetGameListManager()->GetCurrentGameList()->name.c_str());
        const float scale = Themes::GetScaleTexture();
        mGuiTitle->mProperties.x = (1280.0f / 2.0f) - ((mGuiTitle->GetMeasureTextBox().GetIntX() / 2));
        mGuiTitle->mProperties.x = Math::Clamp(mGuiTitle->mProperties.x, 135, 1280);
    }

    void GuiHorizontalBox::SetCovers()
    {
        GameListManager* manager = mApplication->GetGameListManager();
        SpriteManager* spriteManager = mApplication->GetSpriteManager();

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

    void GuiHorizontalBox::ChangeList(const CurrentList list)
    {
        ClearCovers();
        if (list == SystemListSelect)
        {
            mApplication->GetGameListManager()->GetCurrentSystemList()->history.indexCardFocus = mIdFocus;
            mApplication->GetGameListManager()->ChangeGameToSystemList();
            SetFocus(mIdLastFocusSystem, true);
        }
        else
        {
            mIdLastFocusSystem = mIdFocus;
            mApplication->GetGameListManager()->ChangeSystemToGameList();
            SetFocus(mApplication->GetGameListManager()->GetCurrentSystemList()->history.indexCardFocus, true);
        }
    }

    void GuiHorizontalBox::Click()
    {
        mGuiCards[mIdFocus]->Click();
    }

    void GuiHorizontalBox::ClearCovers()
    {
        int size = mApplication->GetGameListManager()->GetGameListSize();
        SpriteManager* spriteManager = mApplication->GetSpriteManager();
        for (int i = 0; i < size; i++)
        {
            const std::string coverName = std::to_string(i) + "_CV";
            const std::string miniCoverName = std::to_string(i) + "_MCV";
            const bool bResult1 = spriteManager->DeleteSprite(coverName);
            const bool bResult2 = spriteManager->DeleteSprite(miniCoverName);

            if (bResult1 && bResult2)
            {
                LOG(LOG_CLASSIC_TRACE, "Sprite deleted index: %d\n  > Cover: %s\n  > Mini Cover: %s ", i, coverName.c_str(), miniCoverName.c_str());
            }
        }

        LOG(LOG_CLASSIC_DEBUG, "Num Sprites Loaded after ClearCovers %d", spriteManager->NumSpritesLoaded());
    }

    void GuiHorizontalBox::Update()
    {
        GuiComponent::Update();

        if (InputManager::IsDown(InputName::rightTriggerFront))
        {
            mSpeed = 255;
        }
        else if (InputManager::IsDown(InputName::leftFaceLeft) || InputManager::IsDown(InputName::leftFaceRight))
        {
            mSpeed += 0.35f;
            mSpeed = Math::Clamp(mSpeed, 0.0f, 88.0f);
        }
        else
        {
            mSpeed = 22.0f;
        }

        if (InputManager::IsDown(InputName::leftFaceLeft) && !mIsRight)
        {
            if (!mIsLeft)
            {
                mApplication->GetAudioManager()->PlayCursor();
                mApplication->GetGameListManager()->AddId(-1);
                SetFocus(mIdFocus - 1);
            }
            mIsLeft = true;
        }

        if (InputManager::IsDown(InputName::leftFaceRight) && !mIsLeft)
        {
            if (!mIsRight)
            {
                mApplication->GetAudioManager()->PlayCursor();
                mApplication->GetGameListManager()->AddId(1);
                SetFocus(mIdFocus + 1);
            }
            mIsRight = true;
        }

        if (mIsRight)
        {
            mPositionX = mPositionX - (int)mSpeed;
        }
        else if (mIsLeft)
        {
            mPositionX = mPositionX + (int)mSpeed;
        }

        for (const auto& cardContainer : mGuiCards)
        {
            if (mPositionX > -256 && mPositionX < 0 && mIsRight)
            {
                if (mIdFocus < 3 || mIdFocus > 6)
                {
                    cardContainer->mProperties.x -= mSpeed;
                }
                mLastDirection = Left;
            }
            else if (mPositionX > 0 && mPositionX < 256 && mIsLeft)
            {
                if (mIdFocus < 3 || mIdFocus > 6)
                {
                    cardContainer->mProperties.x += mSpeed;
                }
                mLastDirection = Right;
            }
            else if (mPositionX <= -256 || mPositionX >= 256)
            {
                mPositionX = 0;
                mIsRight = false;
                mIsLeft = false;
                SetCovers();
                // todo: add clean textures of vram outside of the screen
            }
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
                mGuiCards[i]->mProperties.x = (float)mCardPositions[i];
            }
        }
    }

}  // namespace ClassicLauncher