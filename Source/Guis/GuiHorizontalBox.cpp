#include "GuiHorizontalBox.h"

#include <algorithm>
#include <memory>
#include "Core.h"
#include "Guis/GuiMiniCover.h"

namespace ClassicLauncher
{

    GuiHorizontalBox::GuiHorizontalBox()
        : mApplication(&Application::Get()), mPositionX(0), bLeft(false), bRight(false), mLastDirection(None), mIdFocus(0), mSpeed(22) {};

    void GuiHorizontalBox::Init()
    {
        for (int i = 0; i < 10; i++)
        {
            const int x = 256 * (i - 2);
            auto card = mApplication->GetEntityManager()->CreateEntity<GuiCard>("GuiCard", x - 120, 0);
            AddChild(card.get());
            mGuiCards.emplace_back(card);
        }

        mMiniCover = mApplication->GetEntityManager()->CreateEntity<GuiMiniCover>("MiniCover");
        mMiniCover->Init();
        AddChild(mMiniCover.get());

        SetCovers();
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
        bLeft = true;
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

            TraceLog(LOG_DEBUG, "index final %d line %d", indexFinal, __LINE__);
            const std::string name = std::to_string(indexFinal) + "_CV";
            const std::string path = manager->GetCurrentGameList(indexFinal)->image;

            if (!path.empty())
            {
                spriteManager->LoadSprite(name, path, 228, 204);
                mGuiCards[i]->SetCover(name);
            }
            else
            {
                mGuiCards[i]->SetCover();
            }
        }
        mMiniCover->SetCovers();
    }

    void GuiHorizontalBox::ChangeList(const CurrentList list)
    {
        ClearCovers();
        if (list == SystemListSelect)
        {
            mApplication->GetGameListManager()->ChangeGameToSystemList();
        }
        else
        {
            mApplication->GetGameListManager()->ChangeSystemToGameList();
        }
        SetCovers();
        SetFocus(3, true);
    }

    void GuiHorizontalBox::Click()
    {
        mGuiCards[mIdFocus]->Click();
    }

    void GuiHorizontalBox::ClearCovers()
    {
        int size = mApplication->GetGameListManager()->GetGameListSize();
        for (int i = 0; i < size; i++)
        {
            bool bResult1 = mApplication->GetSpriteManager()->DeleteSprite(std::to_string(i) + "_CV");
            bool bResult2 = mApplication->GetSpriteManager()->DeleteSprite(std::to_string(i) + "_MCV");

            if (!bResult1 || !bResult2)
            {
                TraceLog(LOG_WARNING, "Error deleting sprite %d", i);
            }
        }
    }

    void GuiHorizontalBox::Update()
    {
        GuiComponent::Update();

        mProperties.y = 228;

            
        

        if (IsKeyReleased(KEY_V))
        {
            mSpeed = 22;
        }
        if (IsKeyReleased(KEY_B))
        {
            mSpeed = 88;
        }

        if (IsKeyDown(KEY_LEFT) && !bRight)
        {
            if (!bLeft)
            {
                mApplication->GetAudioManager()->PlayCursor();
                mApplication->GetGameListManager()->AddId(-1);
                SetFocus(mIdFocus - 1);
            }
            bLeft = true;
        }
        if (IsKeyDown(KEY_RIGHT) && !bLeft)
        {
            if (!bRight)
            {
                mApplication->GetAudioManager()->PlayCursor();
                mApplication->GetGameListManager()->AddId(1);
                SetFocus(mIdFocus + 1);
            }
            bRight = true;
        }

        if (bRight)
        {
            mPositionX = mPositionX - mSpeed;
        }
        else if (bLeft)
        {
            mPositionX = mPositionX + mSpeed;
        }

        for (const auto& cardContainer : mGuiCards)
        {
            if (mPositionX > -256 && mPositionX < 0 && bRight)
            {
                if (mIdFocus < 3 || mIdFocus > 6)
                {
                    cardContainer->mProperties.x -= mSpeed;
                }
                mLastDirection = Left;
            }
            else if (mPositionX > 0 && mPositionX < 256 && bLeft)
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
                bRight = false;
                bLeft = false;
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

        if (!bLeft && !bRight)
        {
            for (size_t i = 0; i < mGuiCards.size(); i++)
            {
                mGuiCards[i]->mProperties.x = mCardPositions[i];
            }
        }
    }

}  // namespace ClassicLauncher