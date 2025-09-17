#include "GuiCard.h"


#include "Entity/EntityManager.h"
#include "Graphics/Texture.h"
#include "Guis/GuiSizeBox.h"
#include "Guis/GuiVideoPlayer.h"
#include "Themes/Themes.h"

#include "Data/GameListManager.h"
#include "Graphics/SpriteManager.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(GameListManager* gameListManagerRef, FocusManager* focusManagerRef)
        : FocusComponent(focusManagerRef, this), mTimer(), mTimerVideo(), m_gameListManagerRef(gameListManagerRef)
    {
        // aqui os ponteiros m_entityManagerReference ainda está nulo como documento isso melhor para evitar de chamar o Getters
    }

    void GuiCard::CreateCards(int x, int y)
    {
        mTransform.position.x = static_cast<float>(x);
        mTransform.position.y = static_cast<float>(y);
        mTransform.position.width = 256;
        mTransform.position.height = 280;

        CreateCard(mCardBackgroundMain, 0, 281, 255, "GuiCardBackgroundMain");
        CreateCard(mCardBackgroundFavorite, 514, 281, 0, "GuiCardBackgroundFavorite");
        CreateCard(mCardBackgroundSelected, 257, 281, 0, "GuiCardBackgroundSelected");

        CreateCard(mCover, 0, 0, 255, "GuiCover", false);
        mGuiVideoPlayer = GetEntityManager()->CreateEntity<GuiVideoPlayer>("GuiVideoPlayer");

        CreateCard(mCardMain, 0, 0, 255, "GuiCardMain");
        CreateCard(mCardFavorite, 514, 0, 0, "GuiCardFavorite");
        CreateCard(mCardSelected, 257, 0, 0, "GuiCardSelected");

        CreateSizeBox();
        SetCover();
    }

    void GuiCard::CreateCard(GuiComponent*& card,
                             const float sourceX,
                             const float sourceY,
                             unsigned char alpha,
                             const char* title,
                             bool bAddChild)
    {
        card = GetEntityManager()->CreateEntity<GuiComponent>(title);
        card->mTransform.position.width = mTransform.position.width;
        card->mTransform.position.height = mTransform.position.height;
        card->mTransform.source.x = sourceX;
        card->mTransform.source.y = sourceY;
        card->mTransform.color.SetOpacity(alpha);
        card->mTextureName = "sprite";
        if (bAddChild)
        {
            AddChild(card);
        }
    }

    void GuiCard::CreateSizeBox()
    {
        mSizeBoxImage = GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBoxImage");
        mSizeBoxImage->mTransform.position.width = 228.0f;
        mSizeBoxImage->mTransform.position.height = 204.0f;
        mSizeBoxImage->mTransform.offset.x = 12.0f;
        mSizeBoxImage->mTransform.offset.y = 12.0f;
        mSizeBoxImage->AttachGui(mCover);
        AddChild(mSizeBoxImage);

        mSizeBoxVideoPlayer = GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBoxVideo");
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
        const Texture* textureReference = GetSpriteManager()->GetTexture(mCover->mTextureName);
        const Animation& pAnim = GetAnimation("card-zoom");
        if (textureReference != nullptr && mCover->mTextureName != "sprite" && !pAnim.mIsRunning &&
            mCover->mTransform.position.width == 0 && mCover->mTransform.position.height == 0)
        {
            const float scale = ThemesManager::GetScaleTexture();
            mCover->mTransform.position.width = textureReference->GetSize().x / scale;
            mCover->mTransform.position.height = textureReference->GetSize().y / scale;
            mSizeBoxImage->UpdateGuiAttachment();
        }
    }

    void GuiCard::FocusAnimation(bool bForce, const int a, const int b, const char* nameAnimation)
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
        mCardSelected
            ->StartAnimation(nameAnimation, 0.2f, mCardSelected->mTransform, target, Ease::EaseLinearNone, false);
        mCardBackgroundSelected
            ->StartAnimation(nameAnimation, 0.2f, mCardSelected->mTransform, target, Ease::EaseLinearNone, false);
    }

    void GuiCard::StartVideo()
    {
        // && !GetApplication()->GetProcessManager()->IsApplicationRunning()
        if (mIsFocus)
        {
            mGuiVideoPlayer->Init(m_gameListManagerRef->GetCurrentGameList()->video, 204, 205);
            mSizeBoxVideoPlayer->SetCropGuiAttachment(true);
        }
    }

    void GuiCard::SetCardFocus(bool bForce)
    {
        mIsFocus = true;
        FocusAnimation(bForce, 255, 0, "card-focus");
        GetTimerManager()->SetTimer(mTimerVideo, CALLFUNCTION(StartVideo, this), this, 5.0f);
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

    void GuiCard::SetCover(const std::string& name)
    {
        if (name.empty())
        {
            mCover->mTransform.position.width = 204.0f;
            mCover->mTransform.position.height = 202.0f;
            mCover->mTransform.source.x = 771;
            mCover->mTransform.source.y = 283;
            mCover->mTextureName = "sprite";
        }
        else
        {
            mCover->mTransform.position.width = 0;
            mCover->mTransform.position.height = 0;
            mCover->mTransform.source.x = 0;
            mCover->mTransform.source.y = 0;
            mCover->mTextureName = name;
        }
    }

    bool GuiCard::IsFocus() const
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

        mCardBackgroundFavorite->mTransform.color.SetOpacity(0); // todo create logic is favorite
        mCardFavorite->mTransform.color.SetOpacity(0);           // todo create logic is favorite
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
        GetTimerManager()->ClearTimer(mTimerVideo);

        mIsFront = true;

        const float time = 0.3f;
        const float scale = 1.75f;

        Transform target = mTransform;

        target.scale.x = scale;
        target.scale.y = scale;

        target.position.x += (-target.position.width / 2 * target.scale.x) + target.position.width / 2;
        target.position.y += (-target.position.height / 2 * target.scale.y) + target.position.height / 2;

        target.color.a = 0;
        StartAnimation("card-zoom", time, mTransform, target, Ease::EaseQuadInOut, true);
        GetTimerManager()->SetTimer(mTimer, CALLFUNCTION(Reset, this), this, time * 2);
    }

    void GuiCard::SetFrontCard()
    {

        const int order = (mIsFront) ? 1 : 0;

        GetEntityManager()->SetZOrder(mCardSelected, order);
        GetEntityManager()->SetZOrder(mCardBackgroundSelected, order);
        GetEntityManager()->SetZOrder(mCardMain, order);
        GetEntityManager()->SetZOrder(mCardBackgroundMain, order);
        GetEntityManager()->SetZOrder(mCardFavorite, order);
        GetEntityManager()->SetZOrder(mCardBackgroundFavorite, order);
        GetEntityManager()->SetZOrder(mCover, order);
        GetEntityManager()->SetZOrder(mGuiVideoPlayer, order);
    }

    void GuiCard::SetThemeValue()
    {
        mSizeBoxImage->mTransform.offset.x = ThemesManager::Get().mConfigurationThemes.offsetImageX;
        mSizeBoxImage->mTransform.offset.y = ThemesManager::Get().mConfigurationThemes.offsetImageY;
        mSizeBoxVideoPlayer->mTransform.offset.x = ThemesManager::Get().mConfigurationThemes.offsetVideoX;
        mSizeBoxVideoPlayer->mTransform.offset.y = ThemesManager::Get().mConfigurationThemes.offsetVideoY;
    }

} // namespace ClassicLauncher
