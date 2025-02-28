#include "GuiCard.h"
#include "Application.h"
#include "Graphics/SpriteAnimator.h"
#include "Guis/GuiSizeBox.h"
#include "Guis/GuiVideoPlayer.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(const float x, const float y)
        : FocusComponent(GetApplication(), this), mTimer(), mTimerVideo()
    {
        mTransform.x = x;
        mTransform.y = y;
        mTransform.width = 256;
        mTransform.height = 280;

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
        card->mTransform.width = mTransform.width;
        card->mTransform.height = mTransform.height;
        card->mTransform.sourceX = sourceX;
        card->mTransform.sourceY = sourceY;
        card->mTransform.color.SetOpacity(alpha);
        card->mTextureName = "sprite";
        if (bAddChild)
        {
            AddChild(card);
        }
    }

    void GuiCard::CreateSizeBox()
    {
        mSizeBoxImage = GetApplication()->GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBoxImage");
        mSizeBoxImage->mTransform.width = 228.0f;
        mSizeBoxImage->mTransform.height = 204.0f;
        mSizeBoxImage->mTransform.offset.x = 12.0f;
        mSizeBoxImage->mTransform.offset.y = 12.0f;
        mSizeBoxImage->AttachGui(mCover);
        AddChild(mSizeBoxImage);

        mSizeBoxVideoPlayer = GetApplication()->GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBoxVideo");
        mSizeBoxVideoPlayer->mTransform.width = 228.0f;
        mSizeBoxVideoPlayer->mTransform.height = 204.0f;
        mSizeBoxVideoPlayer->mTransform.offset.x = 12.0f;
        mSizeBoxVideoPlayer->mTransform.offset.y = 12.0f;
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
        if (textureReference != nullptr && mCover->mTextureName != "sprite" && !pAnim.mIsRunning && mCover->mTransform.width == 0 && mCover->mTransform.height == 0)
        {
            const float scale = Themes::GetScaleTexture();
            mCover->mTransform.width = textureReference->width / scale;
            mCover->mTransform.height = textureReference->height / scale;
            mSizeBoxImage->UpdateGuiAttachment();
        }
    }

    void GuiCard::FocusAnimation(bool bForce, int a, const int b, const char* nameAnimation)
    {
        if (bForce)
        {
            mCardSelected->mTransform.color.SetOpacity(a);
            mCardBackgroundSelected->mTransform.color.SetOpacity(a);
            return;
        }
        mCardSelected->mTransform.color.SetOpacity(b);
        mCardBackgroundSelected->mTransform.color.SetOpacity(b);
        Transform target = mCardSelected->mTransform;
        target.color.a = a;
        mCardSelected->StartAnimation(nameAnimation, 0.2f, mCardSelected->mTransform, target, Ease::EaseLinearNone, false);
        mCardBackgroundSelected->StartAnimation(nameAnimation, 0.2f, mCardSelected->mTransform, target, Ease::EaseLinearNone, false);
    }

    void GuiCard::StartVideo()
    {
        if (mIsFocus)
        {
            mGuiVideoPlayer->Init(GetApplication()->GetGameListManager()->GetCurrentGameList()->video, 204, 205);
            mSizeBoxVideoPlayer->SetCropGuiAttachment(true);
        }
    }

    void GuiCard::SetCardFocus(bool bForce)
    {
        mIsFocus = true;
        FocusAnimation(bForce, 255, 0, "card-focus");
        GetApplication()->GetTimerManager()->SetTimer(mTimerVideo, CALLFUNCTION(StartVideo, this), this, 5.0f);
        SetFocus();
    }

    void GuiCard::RemoveCardFocus(bool bForce)
    {
        mIsFocus = false;
        FocusAnimation(bForce, 0, 255, "card-lost-focus");
        mGuiVideoPlayer->Stop();
        mSizeBoxVideoPlayer->SetCropGuiAttachment(false);
    }

    void GuiCard::OnFocus()
    {
    }

    void GuiCard::OnLostFocus()
    {
    }

    void GuiCard::SetCover(std::string name)
    {
        if (name.empty())
        {
            mCover->mTransform.width = 204.0f;
            mCover->mTransform.height = 202.0f;
            mCover->mTransform.sourceX = 771;
            mCover->mTransform.sourceY = 283;
            mCover->mTextureName = "sprite";
        }
        else
        {
            mCover->mTransform.width = 0;
            mCover->mTransform.height = 0;
            mCover->mTransform.sourceX = 0;
            mCover->mTransform.sourceY = 0;
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
        mTransform.scaleX = 1.0f;
        mTransform.scaleY = 1.0f;

        mCardBackgroundMain->mTransform.color.SetOpacity(255);
        mCardMain->mTransform.color.SetOpacity(255);
        mCardBackgroundSelected->mTransform.color.SetOpacity(255);
        mCardSelected->mTransform.color.SetOpacity(255);
        mCover->mTransform.color.SetOpacity(255);
        mSizeBoxImage->mTransform.color.SetOpacity(255);
        mSizeBoxVideoPlayer->mTransform.color.SetOpacity(255);
        mGuiVideoPlayer->mTransform.color.SetOpacity(255);

        mCardBackgroundFavorite->mTransform.color.SetOpacity(0);  // todo create logic is favorite
        mCardFavorite->mTransform.color.SetOpacity(0);            // todo create logic is favorite
        if (mCover->mTextureName == "sprite")
        {
            SetCover();
        }
        if (!mIsFocus)
        {
            RemoveCardFocus(true);
        }
    }

    void GuiCard::Click()
    {
        mGuiVideoPlayer->Stop();

        mIsFront = true;

        const float time = 0.3f;
        const float scale = 1.75f;

        Transform target = mTransform;

        target.scaleX = scale;
        target.scaleY = scale;

        target.x += (-target.width / 2 * target.scaleX) + target.width / 2;
        target.y += (-target.height / 2 * target.scaleY) + target.height / 2;

        target.color.a = 0;
        StartAnimation("card-zoom", time, mTransform, target, Ease::EaseQuadInOut, true);
        GetApplication()->GetTimerManager()->SetTimer(mTimer, CALLFUNCTION(Reset, this), this, time * 2);
    }

    void GuiCard::SetFrontCard()
    {
        Application* pApplication = GetApplication();
        const int order = (mIsFront) ? 1 : 0;

        pApplication->GetEntityManager()->SetZOrder(mCardSelected, order);
        pApplication->GetEntityManager()->SetZOrder(mCardBackgroundSelected, order);
        pApplication->GetEntityManager()->SetZOrder(mCardMain, order);
        pApplication->GetEntityManager()->SetZOrder(mCardBackgroundMain, order);
        pApplication->GetEntityManager()->SetZOrder(mCardFavorite, order);
        pApplication->GetEntityManager()->SetZOrder(mCardBackgroundFavorite, order);
        pApplication->GetEntityManager()->SetZOrder(mCover, order);
        pApplication->GetEntityManager()->SetZOrder(mGuiVideoPlayer, order);
    }

}  // namespace ClassicLauncher
