#include "GuiCard.h"
#include "Application.h"
#include "Graphics/SpriteAnimator.h"
#include "Guis/GuiSizeBox.h"
#include "Guis/GuiVideoPlayer.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(const float x, const float y)
        : mTimer(), mTimerVideo()
    {
        mProperties.x = x;
        mProperties.y = y;
        mProperties.width = 256;
        mProperties.height = 280;

        CreateCard(mCardBackgroundMain, 0, 281, 255, "GuiCardBackgroundMain");
        CreateCard(mCardBackgroundFavorite, 514, 281, 0, "GuiCardBackgroundFavorite");
        CreateCard(mCardBackgroundSelected, 257, 281, 0, "GuiCardBackgroundSelected");

        CreateCard(mCover, 0, 0, 255, "GuiCover", false);
        mGuiVideoPlayer = GetApplication()->GetEntityManager()->CreateEntity<GuiVideoPlayer>("GuiVideoPlayer");

        CreateCard(mCardMain, 0, 0, 255, "GuiCardMain");
        CreateCard(mCardFavorite, 514, 0, 0, "GuiCardFavorite");
        CreateCard(mCardSelected, 257, 0, 0, "GuiCardSelected");

        CreateSizeBox();
        SetCover();

        // mCardSelected->mProperties.offset.x = -15;
    }

    void GuiCard::CreateCard(GuiComponent*& card, const float sourceX, const float sourceY, unsigned char alpha, const char* title, bool bAddChild)
    {
        card = GetApplication()->GetEntityManager()->CreateEntity<GuiComponent>(title);
        card->mProperties.width = mProperties.width;
        card->mProperties.height = mProperties.height;
        card->mProperties.sourceX = sourceX;
        card->mProperties.sourceY = sourceY;
        card->mProperties.color.SetOpacity(alpha);
        card->mTextureName = "sprite";
        if (bAddChild)
        {
            AddChild(card);
        }
    }

    void GuiCard::CreateSizeBox()
    {
        mSizeBoxImage = GetApplication()->GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBoxImage");
        mSizeBoxImage->mProperties.width = 228.0f;
        mSizeBoxImage->mProperties.height = 204.0f;
        mSizeBoxImage->mProperties.offset.x = 12.0f;
        mSizeBoxImage->mProperties.offset.y = 12.0f;
        mSizeBoxImage->AttachGui(mCover);
        AddChild(mSizeBoxImage);

        mSizeBoxVideoPlayer = GetApplication()->GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBoxVideo");
        mSizeBoxVideoPlayer->mProperties.width = 228.0f;
        mSizeBoxVideoPlayer->mProperties.height = 204.0f;
        mSizeBoxVideoPlayer->mProperties.offset.x = 12.0f;
        mSizeBoxVideoPlayer->mProperties.offset.y = 12.0f;
        mSizeBoxVideoPlayer->AttachGui(mGuiVideoPlayer);
        AddChild(mSizeBoxVideoPlayer);
    }

    void GuiCard::Update()
    {
        EntityGui::Update();
        // mSizeBox->mProperties.offset.x += 0.03f;
        // mSizeBox->mProperties.offset.y += 0.03f;
        const Texture2D* textureReference = GetApplication()->GetSpriteManager()->GetTexture(mCover->mTextureName);
        const Animation& pAnim = GetAnimation("card-zoom");
        if (textureReference != nullptr && mCover->mTextureName != "sprite" && !pAnim.mIsRunning && mCover->mProperties.width == 0 && mCover->mProperties.height == 0)
        {
            const float scale = Themes::GetScaleTexture();
            mCover->mProperties.width = textureReference->width / scale;
            mCover->mProperties.height = textureReference->height / scale;
            mSizeBoxImage->UpdateGuiAttachment();
        }
    }

    void GuiCard::FocusAnimation(bool bForce, int a, const int b, const char* nameAnimation)
    {
        if (bForce)
        {
            mCardSelected->mProperties.color.SetOpacity(a);
            mCardBackgroundSelected->mProperties.color.SetOpacity(a);
            return;
        }
        mCardSelected->mProperties.color.SetOpacity(b);
        mCardBackgroundSelected->mProperties.color.SetOpacity(b);
        TransformProperties target = mCardSelected->mProperties;
        target.color.a = a;
        mCardSelected->StartAnimation(nameAnimation, 0.2f, mCardSelected->mProperties, target, Ease::EaseLinearNone, false);
        mCardBackgroundSelected->StartAnimation(nameAnimation, 0.2f, mCardSelected->mProperties, target, Ease::EaseLinearNone, false);
    }

    void GuiCard::StartVideo()
    {
        if (mIsFocus)
        {
            mGuiVideoPlayer->Init(GetApplication()->GetGameListManager()->GetCurrentGameList()->video, 204, 205);
            mSizeBoxVideoPlayer->SetCropGuiAttachment(true);
        }
    }

    void GuiCard::SetFocus(bool bForce)
    {
        mIsFocus = true;
        FocusAnimation(bForce, 255, 0, "card-focus");
        GetApplication()->GetTimerManager()->SetTimer(mTimerVideo, CALLFUNCTION(StartVideo, this), this, 5.0f);
    }

    void GuiCard::RemoveFocus(bool bForce)
    {
        mIsFocus = false;
        FocusAnimation(bForce, 0, 255, "card-lost-focus");
        mGuiVideoPlayer->Stop();
        mSizeBoxVideoPlayer->SetCropGuiAttachment(false);
    }

    void GuiCard::SetCover(std::string name)
    {
        if (name.empty())
        {
            mCover->mProperties.width = 204.0f;
            mCover->mProperties.height = 202.0f;
            mCover->mProperties.sourceX = 771;
            mCover->mProperties.sourceY = 283;
            mCover->mTextureName = "sprite";
        }
        else
        {
            mCover->mProperties.width = 0;
            mCover->mProperties.height = 0;
            mCover->mProperties.sourceX = 0;
            mCover->mProperties.sourceY = 0;
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
        mProperties.scaleX = 1.0f;
        mProperties.scaleY = 1.0f;
        
        mCardBackgroundMain->mProperties.color.SetOpacity(255);
        mCardMain->mProperties.color.SetOpacity(255);
        mCardBackgroundSelected->mProperties.color.SetOpacity(255);
        mCardSelected->mProperties.color.SetOpacity(255);
        mCover->mProperties.color.SetOpacity(255);
        mSizeBoxImage->mProperties.color.SetOpacity(255);
        mSizeBoxVideoPlayer->mProperties.color.SetOpacity(255);
        mGuiVideoPlayer->mProperties.color.SetOpacity(255);
        
        mCardBackgroundFavorite->mProperties.color.SetOpacity(0);  // todo create logic is favorite
        mCardFavorite->mProperties.color.SetOpacity(0);            // todo create logic is favorite
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
            pApplication->GetEntityManager()->SetZOrder(mCardBackgroundSelected, 1);
            pApplication->GetEntityManager()->SetZOrder(mCardMain, 1);
            pApplication->GetEntityManager()->SetZOrder(mCardBackgroundMain, 1);
            pApplication->GetEntityManager()->SetZOrder(mCardFavorite, 1);
            pApplication->GetEntityManager()->SetZOrder(mCardBackgroundFavorite, 1);
            pApplication->GetEntityManager()->SetZOrder(mCover, 1);
            pApplication->GetEntityManager()->SetZOrder(mGuiVideoPlayer, 1);
        }
        else
        {
            pApplication->GetEntityManager()->SetZOrder(mCardSelected, 0);
            pApplication->GetEntityManager()->SetZOrder(mCardBackgroundSelected, 0);
            pApplication->GetEntityManager()->SetZOrder(mCardMain, 0);
            pApplication->GetEntityManager()->SetZOrder(mCardBackgroundMain, 0);
            pApplication->GetEntityManager()->SetZOrder(mCardFavorite, 0);
            pApplication->GetEntityManager()->SetZOrder(mCardBackgroundFavorite, 0);
            pApplication->GetEntityManager()->SetZOrder(mCover, 0);
            pApplication->GetEntityManager()->SetZOrder(mGuiVideoPlayer, 0);
        }
    }

}  // namespace ClassicLauncher
