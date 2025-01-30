#include "GuiCard.h"
#include "Application.h"
#include "Graphics/SpriteAnimator.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(int x, int y)
        : mTimer(), GuiComponent()
    {
        mProperties.x = x;
        mProperties.y = y;

        CreateCard(mCardMain, 528 - 6, 9, 255, "cardMain");
        CreateCard(mCardFavorite, 783 - 6, 9, 0, "cardFavorite");
        CreateCard(mCardSelected, 273 - 6, 9, 0, "cardSelected");
        CreateCard(mCover, 0, 0, 255, "cover");

        mProperties.width = 252;
        mProperties.height = 276;

        SetCover();
    }

    void GuiCard::CreateCard(GuiComponent*& card, int sourceX, int sourceY, unsigned char alpha, const char* title)
    {
        Application* app = &Application::Get();

        card = app->GetEntityManager()->CreateEntity<GuiComponent>(title);
        card->mProperties.width = 252;
        card->mProperties.height = 276;
        card->mProperties.sourceX = sourceX;
        card->mProperties.sourceY = sourceY;
        card->mProperties.color.SetOpacity(alpha);
        card->mTextureName = "sprite";

        AddChild(card);
    }

    void GuiCard::Update()
    {
        GuiComponent::Update();

        Application* pApplication = &Application::Get();
        if (pApplication == nullptr)
        {
            return;
        }
        Texture2D* textureReference = pApplication->GetSpriteManager()->GetTexture(mCover->mTextureName);
        Animation pAnim = mCover->GetAnimation("card-zoom");
        if (textureReference != nullptr && mCover->mTextureName != "sprite" && !pAnim.mIsRunning && mCover->mProperties.width == 0 && mCover->mProperties.height == 0)
        {
            const float scale = Themes::GetScaleTexture();
            mCover->mProperties.x = (mContainerSize.x - static_cast<float>(textureReference->width / scale)) / 2.0f;
            mCover->mProperties.y = (mContainerSize.y - static_cast<float>(textureReference->height / scale)) / 2.0f;
            mCover->mProperties.width = textureReference->width / scale;
            mCover->mProperties.height = textureReference->height / scale;
        }
    }

    void GuiCard::SetFocus(bool bForce)
    {
        mIsFocus = true;
        if (bForce)
        {
            mCardSelected->mProperties.color.SetOpacity(255);
            return;
        }
        TransformProperties target = mCardSelected->mProperties;
        target.color.a = 255;
        mCardSelected->StartAnimation("card-focus", 0.2f, mCardSelected->mProperties, target, Ease::EaseLinearNone, false);
    }

    void GuiCard::RemoveFocus(bool bForce)
    {
        mIsFocus = false;
        if (bForce)
        {
            mCardSelected->mProperties.color.SetOpacity(0);
            return;
        }
        TransformProperties target = mCardSelected->mProperties;
        target.color.a = 0;
        mCardSelected->StartAnimation("card-lost-focus", 0.2f, mCardSelected->mProperties, target, Ease::EaseLinearNone, false);
    }

    void GuiCard::SetCover(std::string name)
    {
        if (name.empty())
        {
            mCover->mProperties.width = mDefaultCoverWidth;
            mCover->mProperties.height = mDefaultCoverHeight;
            mCover->mProperties.sourceX = 1086;
            mCover->mProperties.sourceY = 1086;
            mCover->mProperties.x = (mContainerSize.x - mDefaultCoverWidth) / 2;
            mCover->mProperties.y = (mContainerSize.y - mDefaultCoverHeight) / 2;
            mCover->mTextureName = "sprite";
        }
        else
        {
            mCover->mProperties.width = 0;
            mCover->mProperties.height = 0;
            mCover->mProperties.sourceX = 0;
            mCover->mProperties.sourceY = 0;
            mCover->mProperties.x = 0;
            mCover->mProperties.y = 0;
            mCover->mTextureName = name;
        }
    }

    bool GuiCard::IsFocus()
    {
        return mIsFocus;
    }

    void GuiCard::Reset()
    {
        mIsFront = false;
        mCardMain->mProperties.color.SetOpacity(255);
        mCardSelected->mProperties.color.SetOpacity(255);
        mCover->mProperties.color.SetOpacity(255);
        mProperties.scaleX = 1.0f;
        mProperties.scaleY = 1.0f;
        if (mCover->mTextureName == "sprite")
        {
            SetCover();
        }
        if (!mIsFocus)
        {
            RemoveFocus(true);
        }
    }

    void GuiCard::Click()
    {
        Application* pApplication = &Application::Get();

        if (!pApplication) return;

        mIsFront = true;

        const float time = 0.3f;
        const float scale = 1.75f;

        TransformProperties target = mProperties;

        target.scaleX = scale;
        target.scaleY = scale;

        target.x += (-target.width / 2 * target.scaleX) + target.width / 2;
        target.y += (-target.height / 2 * target.scaleY) + target.height / 2;

        StartAnimation("card-zoom", time, mProperties, target, Ease::EaseQuadInOut, true);

        TransformProperties targetMain = mCardMain->mProperties;
        TransformProperties targetSelected = mCardSelected->mProperties;
        TransformProperties targetCover = mCover->mProperties;

        targetMain.color.a = 0;
        targetSelected.color.a = 0;
        targetCover.color.a = 0;
        mCardMain->StartAnimation("card-zoom", time, mCardMain->mProperties, targetMain, Ease::EaseQuadInOut, false);
        mCardSelected->StartAnimation("card-zoom", time, mCardSelected->mProperties, targetSelected, Ease::EaseQuadInOut, false);
        mCover->StartAnimation("card-zoom", time, mCover->mProperties, targetCover, Ease::EaseQuadInOut, false);

        pApplication->GetEntityManager()->SetTimer(mTimer, CALLFUNCTION(Reset, this), this, time * 3);
    }

    void GuiCard::SetFrontCard()
    {
        Application* p = &Application::Get();
        if (mIsFront)
        {
            p->GetEntityManager()->SetZOrder(mCardSelected, 1);
            p->GetEntityManager()->SetZOrder(mCardMain, 1);
            p->GetEntityManager()->SetZOrder(mCardFavorite, 1);
            p->GetEntityManager()->SetZOrder(mCover, 1);
        }
        else
        {
            Application* p = &Application::Get();
            p->GetEntityManager()->SetZOrder(mCardSelected, 0);
            p->GetEntityManager()->SetZOrder(mCardMain, 0);
            p->GetEntityManager()->SetZOrder(mCardFavorite, 0);
            p->GetEntityManager()->SetZOrder(mCover, 0);
        }
    }

}  // namespace ClassicLauncher
