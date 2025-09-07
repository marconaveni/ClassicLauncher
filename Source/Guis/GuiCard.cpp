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
        mTransform.position.x = x;
        mTransform.position.y = y;
        mTransform.position.width = 256;
        mTransform.position.height = 280;

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
        card->mTransform.position.width = mTransform.position.width;
        card->mTransform.position.height = mTransform.position.height;
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
        mSizeBoxImage->mTransform.position.width = 228.0f;
        mSizeBoxImage->mTransform.position.height = 204.0f;
        mSizeBoxImage->mTransform.offset.x = 12.0f;
        mSizeBoxImage->mTransform.offset.y = 12.0f;
        mSizeBoxImage->AttachGui(mCover);
        AddChild(mSizeBoxImage);

        mSizeBoxVideoPlayer = GetApplication()->GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBoxVideo");
        mSizeBoxVideoPlayer->mTransform.position.width = 228.0f;
        mSizeBoxVideoPlayer->mTransform.position.height = 204.0f;
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
        const rlw::Texture2D* textureReference = GetApplication()->GetSpriteManager()->GetTexture(mCover->mTextureName);
        const Animation& pAnim = GetAnimation("card-zoom");
        if (textureReference != nullptr && mCover->mTextureName != "sprite" && !pAnim.mIsRunning && mCover->mTransform.position.width == 0 && mCover->mTransform.position.height == 0)
        {
            const float scale = Themes::GetScaleTexture();
            mCover->mTransform.position.width = textureReference->width / scale;
            mCover->mTransform.position.height = textureReference->height / scale;
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
        Transformation target = mCardSelected->mTransform;
        target.color.a = a;
        mCardSelected->StartAnimation(nameAnimation, 0.2f, mCardSelected->mTransform, target, Ease::EaseLinearNone, false);
        mCardBackgroundSelected->StartAnimation(nameAnimation, 0.2f, mCardSelected->mTransform, target, Ease::EaseLinearNone, false);
    }

    void GuiCard::StartVideo()
    {
        // && !GetApplication()->GetProcessManager()->IsApplicationRunning()
        if (mIsFocus )
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
            mCover->mTransform.position.width = 204.0f;
            mCover->mTransform.position.height = 202.0f;
            mCover->mTransform.sourceX = 771;
            mCover->mTransform.sourceY = 283;
            mCover->mTextureName = "sprite";
        }
        else
        {
            mCover->mTransform.position.width = 0;
            mCover->mTransform.position.height = 0;
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
        mTransform.scale.x = 1.0f;
        mTransform.scale.y = 1.0f;

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
        GetApplication()->GetTimerManager()->ClearTimer(mTimerVideo);

        mIsFront = true;

        const float time = 0.3f;
        const float scale = 1.75f;

        Transformation target = mTransform;

        target.scale.x = scale;
        target.scale.y = scale;

        target.position.x += (-target.position.width / 2 * target.scale.x) + target.position.width / 2;
        target.position.y += (-target.position.height / 2 * target.scale.y) + target.position.height / 2;

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

    void GuiCard::SetThemeValue()
    {
        mSizeBoxImage->mTransform.offset.x = GetApplication()->GetThemes()->mConfigurationThemes.offsetImageX;
        mSizeBoxImage->mTransform.offset.y = GetApplication()->GetThemes()->mConfigurationThemes.offsetImageY;
        mSizeBoxVideoPlayer->mTransform.offset.x = GetApplication()->GetThemes()->mConfigurationThemes.offsetVideoX;
        mSizeBoxVideoPlayer->mTransform.offset.y = GetApplication()->GetThemes()->mConfigurationThemes.offsetVideoY;
    }

}  // namespace ClassicLauncher
