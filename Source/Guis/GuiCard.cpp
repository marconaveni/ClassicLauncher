#include "GuiCard.h"
#include "Application.h"
#include "Graphics/SpriteAnimator.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(int x, int y)
        : mTimer(nullptr), GuiComponent()
    {
        mProperties.x = x;
        mProperties.y = y;

        CreateCard(mCardMain, 528, 15, 255);
        CreateCard(mCardFavorite, 783, 15, 0);
        CreateCard(mCardSelected, 273, 15, 0);
        CreateCard(mCover, 0, 0, 255);

        SetCover();
    }

    void GuiCard::CreateCard(std::shared_ptr<GuiComponent>& card, int sourceX, int sourceY, unsigned char alpha)
    {
        Application* app = &Application::Get();

        card = app->GetEntityManager()->CreateEntity<GuiComponent>("card");
        card->mProperties.width = 246;
        card->mProperties.height = 270;
        card->mProperties.sourceX = sourceX;
        card->mProperties.sourceY = sourceY;
        card->mProperties.color.SetOpacity(alpha);
        card->mTextureName = "sprite";

        AddChild(card.get());
    }

    void GuiCard::Update()
    {
        GuiComponent::Update();

        if (mIsFront)
        {
            mCardSelected->SetBringToFront();
            mCardMain->SetBringToFront();
            mCardFavorite->SetBringToFront();
            mCover->SetBringToFront();
        }

        Application* pApplication = &Application::Get();
        if (pApplication == nullptr)
        {
            return;
        }
        Texture2D* textureReference = pApplication->GetSpriteManager()->GetTexture(mCover->mTextureName);
        Animation pAnim = mCover->GetAnimation("card-zoom");
        if (textureReference != nullptr && mCover->mTextureName != "sprite" && !pAnim.mIsRunning)
        {
            mCover->mProperties.x = (240.0f - static_cast<float>(textureReference->width)) / 2.0f;
            mCover->mProperties.y = (216.0f - static_cast<float>(textureReference->height)) / 2.0f;
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
            mCover->mProperties.x = (240 - 204) / 2;
            mCover->mProperties.y = (216 - 204) / 2;
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
        mCardSelected->mProperties.color.SetOpacity(mIsFocus ? 255.0f : 0.0f);
        mCardMain->mProperties.color.SetOpacity(255);
        mCover->mProperties.color.SetOpacity(255);
        mCardSelected->mProperties.scale = 1.0f;
        mCardMain->mProperties.scale = 1.0f;
        mCover->mProperties.scale = 1.0f;
        mCardSelected->mProperties.x = 0.0f;
        mCardMain->mProperties.x = 0.0f;
        mCover->mProperties.x = 0.0f;
        mCardSelected->mProperties.y = 0.0f;
        mCardMain->mProperties.y = 0.0f;
        mCover->mProperties.y = 0.0f;
        if(mCover->mTextureName == "sprite")
        {
            SetCover();
        }
    }

    void GuiCard::Click()
    {
        Application* pApplication = &Application::Get();
        if (!pApplication)
        {
            return;
        }
        mIsFront = true;

        Texture2D* textureReference = pApplication->GetSpriteManager()->GetTexture(mCover->mTextureName);

        const float time = 0.3f;
        const float scale = 1.75f;

        TransformProperties target = mCardSelected->mProperties;
        TransformProperties targetCover = mCover->mProperties;

        target.color.a = 0;
        target.scale = scale;

        target.x = (-target.width / 2 * target.scale) + target.width / 2;
        target.y = (-target.height / 2 * target.scale) + target.height / 2;

        targetCover.color.a = 0;
        targetCover.scale = scale;

        float width = (textureReference != nullptr && mCover->mTextureName != "sprite") ? static_cast<float>(textureReference->width) : mDefaultCoverWidth;
        float height = (textureReference != nullptr && mCover->mTextureName != "sprite") ? static_cast<float>(textureReference->height) : mDefaultCoverHeight;

        width = (mContainerSize - width) / 2.0f;
        height = (mContainerSize - height) / 2.0f;

        targetCover.x = ((-mContainerSize + width) / 2 * targetCover.scale) + (mContainerSize + width) / 2;
        targetCover.y = ((-mContainerSize + height) / 2 * targetCover.scale) + (mContainerSize + height) / 2;

        mCardSelected->StartAnimation("card-zoom", time, mCardSelected->mProperties, target, Ease::EaseQuadInOut, false);
        mCardMain->StartAnimation("card-zoom", time, mCardMain->mProperties, target, Ease::EaseQuadInOut, false);
        mCover->StartAnimation("card-zoom", time, mCover->mProperties, targetCover, Ease::EaseQuadInOut, false);

        pApplication->GetEntityManager()->SetTimer(mTimer, CALLFUNCTION(Reset, this), this, time * 2);
    }

}  // namespace ClassicLauncher
