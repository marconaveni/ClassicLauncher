#include "GuiGrid.h"

#include <algorithm>
#include <memory>
#include "Utils/Core.h"

namespace ClassicLauncher
{

    GuiGrid::GuiGrid() : mApplication(&Application::Get()), mPositionX(0), bLeft(false), bRight(false), mLastDirection(None), mIdFocus(0), mSpeed(22) {}

    void GuiGrid::Init()
    {

        for (int i = 0; i < 10; i++)
        {
            const int x = 256 * (i - 2);
            auto card = mApplication->GetEntityManager()->CreateEntity<GuiCard>(x - 120, 0);
            // auto card = app->GetEntityManager()->CreateEntity<GuiCard>(0, 228);
            AddChild(card.get());
            mGuiCards.emplace_back(card);
        }

        SetFocus(3);
        SetCovers();
    }

    void GuiGrid::Draw() { GuiComponent::Draw(); }

    void GuiGrid::End() { GuiComponent::End(); }

    void GuiGrid::SetFocus(const int newId)
    {
        if (mGuiCards[mIdFocus]->IsFocus() && mIdFocus == newId) return;

        mGuiCards[mIdFocus]->RemoveFocus();
        // cardsContainer[idFocus].StartAnimationLostFocus();
        mIdFocus = newId;
        // cardsContainer[newId].StartAnimationFocus();
        mGuiCards[newId]->SetFocus();
        bLeft = true;
    }

    void GuiGrid::SetCovers()
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
    }

    void GuiGrid::Update()
    {
        GuiComponent::Update();

        y = 228;

        if (IsKeyReleased(KEY_V) && !bLeft)
        {
            mSpeed = 22;
        }
        if (IsKeyReleased(KEY_B) && !bLeft)
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
                    cardContainer->x -= mSpeed;
                }
                mLastDirection = Left;
            }
            else if (mPositionX > 0 && mPositionX < 256 && bLeft)
            {
                if (mIdFocus < 3 || mIdFocus > 6)
                {
                    cardContainer->x += mSpeed;
                }
                mLastDirection = Right;
            }
            else if (mPositionX <= -256 || mPositionX >= 256)
            {
                mPositionX = 0;
                bRight = false;
                bLeft = false;
                SetCovers();
                // x = 0;
                // ImageLoader::GetInstance()->UnloadGameListTextureOutRange();
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
                mGuiCards[i]->x = mCardPositions[i];
            }
        }
    }

}  // namespace ClassicLauncher