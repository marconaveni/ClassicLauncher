#include "GuiCard.h"
#include "Application.h"
#include "Graphics/SpriteAnimator.h"
#include "Guis/GuiSizeBox.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(const float x, const float y)
        : EntityGui(), mTimer()
    {
        mProperties.x = x;
        mProperties.y = y;
        mProperties.width = 252;
        mProperties.height = 276;

        CreateCard(mCardMain, 528 - 6, 9, 255, "GuiCardMain");
        CreateCard(mCardFavorite, 783 - 6, 9, 0, "GuiCardFavorite");
        CreateCard(mCardSelected, 273 - 6, 9, 0, "GuiCardSelected");
        CreateCard(mCover, 0, 0, 255, "GuiCover");
        CreateSizeBox();
        AddChild(mCardMain);
        AddChild(mCardFavorite);
        AddChild(mCardSelected);
        SetCover();
    }

    void GuiCard::CreateCard(GuiComponent*& card, const float sourceX, const float sourceY, unsigned char alpha, const char* title)
    {
        card = GetApplication()->GetEntityManager()->CreateEntity<GuiComponent>(title);
        card->mProperties.width = mProperties.width;
        card->mProperties.height = mProperties.height;
        card->mProperties.sourceX = sourceX;
        card->mProperties.sourceY = sourceY;
        card->mProperties.color.SetOpacity(alpha);
        card->mTextureName = "sprite";
    }

    void GuiCard::CreateSizeBox()
    {
        mSizeBox = GetApplication()->GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBox");
        mSizeBox->mProperties.width = 228.0f;
        mSizeBox->mProperties.height = 204.0f;
        mSizeBox->mProperties.offset.x = 12.0f;
        mSizeBox->mProperties.offset.y = 12.0f;
        mSizeBox->AttachGui(mCover);
        AddChild(mSizeBox);
    }

    void GuiCard::Update()
    {
        EntityGui::Update();
        //mSizeBox->mProperties.offset.x += 0.03f;
        //mSizeBox->mProperties.offset.y += 0.03f;
        const Texture2D* textureReference = GetApplication()->GetSpriteManager()->GetTexture(mCover->mTextureName);
        const Animation& pAnim = GetAnimation("card-zoom");
        if (textureReference != nullptr && mCover->mTextureName != "sprite" && !pAnim.mIsRunning && mCover->mProperties.width == 0 && mCover->mProperties.height == 0)
        {
            const float scale = Themes::GetScaleTexture();
            mCover->mProperties.width = textureReference->width / scale;
            mCover->mProperties.height = textureReference->height / scale;
            mSizeBox->UpdateGuiAttachment();
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
        mCardSelected->mProperties.color.SetOpacity(0);
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
        mCardSelected->mProperties.color.SetOpacity(255);
        TransformProperties target = mCardSelected->mProperties;
        target.color.a = 0;
        mCardSelected->StartAnimation("card-lost-focus", 0.2f, mCardSelected->mProperties, target, Ease::EaseLinearNone, false);
    }

    void GuiCard::SetCover(std::string name)
    {
        if (name.empty())
        {
            mCover->mProperties.width = 204.0f;
            mCover->mProperties.height = 204.0f;
            mCover->mProperties.sourceX = 1086;
            mCover->mProperties.sourceY = 1086;
            mCover->mProperties.x = 0;
            mCover->mProperties.y = 0;
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
        mSizeBox->mProperties.color.SetOpacity(255);
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
        mIsFront = true;

        const float time = 0.3f;
        const float scale = 1.75f;

        TransformProperties target = mProperties;

        target.scaleX = scale;
        target.scaleY = scale;

        target.x += (-target.width / 2 * target.scaleX) + target.width / 2;
        target.y += (-target.height / 2 * target.scaleY) + target.height / 2;

        target.color.a = 0;
        StartAnimation("card-zoom", time, mProperties, target, Ease::EaseQuadInOut, true);
        GetApplication()->GetTimerManager()->SetTimer(mTimer, CALLFUNCTION(Reset, this), this, time * 2);
    }

    void GuiCard::SetFrontCard()
    {
        Application* pApplication = GetApplication();
        if (mIsFront)
        {
            pApplication->GetEntityManager()->SetZOrder(mCardSelected, 1);
            pApplication->GetEntityManager()->SetZOrder(mCardMain, 1);
            pApplication->GetEntityManager()->SetZOrder(mCardFavorite, 1);
            pApplication->GetEntityManager()->SetZOrder(mCover, 1);
        }
        else
        {
            pApplication->GetEntityManager()->SetZOrder(mCardSelected, 0);
            pApplication->GetEntityManager()->SetZOrder(mCardMain, 0);
            pApplication->GetEntityManager()->SetZOrder(mCardFavorite, 0);
            pApplication->GetEntityManager()->SetZOrder(mCover, 0);
        }
    }

}  // namespace ClassicLauncher
