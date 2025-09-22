#include "GuiCard.h"

#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/Texture.h"
#include "Guis/Components/GuiSizeBox.h"
#include "Guis/GuiVideoPlayer.h"
#include "Themes/ThemesManager.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(GameListManager* gameListManagerRef, FocusManager* focusManagerRef)
        : FocusComponent(focusManagerRef, this), mTimer(), mTimerVideo(), m_gameListManagerRef(gameListManagerRef)
    {
        // aqui os ponteiros m_entityManagerReference ainda está nulo como documento isso melhor para evitar de chamar o Getters
    }

    void GuiCard::CreateCards(int x, int y)
    {
        m_transform.position.x = 0; //static_cast<float>(x);
        m_transform.position.y = 0; //static_cast<float>(y);
        m_transform.position.width = 256;
        m_transform.position.height = 280;

        CreateCard(mCardBackgroundMain, 0, 281, 255, "GuiCardBackgroundMain");
        CreateCard(mCardBackgroundFavorite, 514, 281, 0, "GuiCardBackgroundFavorite");
        CreateCard(mCardBackgroundSelected, 257, 281, 0, "GuiCardBackgroundSelected");

        CreateCard(m_coverDefault, 0, 0, 255, "GuiCover", true);
        CreateCard(m_cover, 0, 0, 255, "GuiCover", false);
        AddChild(m_cover);

        mGuiVideoPlayer = GetEntityManager()->CreateEntity<GuiVideoPlayer>("GuiVideoPlayer");
        mGuiVideoPlayer->m_transform.position.x = 12;
        mGuiVideoPlayer->m_transform.position.y = 12;
        AddChild(mGuiVideoPlayer);

        CreateCard(mCardMain, 0, 0, 255, "GuiCardMain");
        CreateCard(mCardFavorite, 514, 0, 0, "GuiCardFavorite");
        CreateCard(mCardSelected, 257, 0, 0, "GuiCardSelected");

        CreateSizeBox();
        SetCover();

        //GetTimerManager()->SetTimer(mTimerVideo, CALLFUNCTION(StartVideo, this), this, 1.0f);
    }

    void GuiCard::CreateCard(GuiComponent*& card,
                             const float sourceX,
                             const float sourceY,
                             unsigned char alpha,
                             const char* title,
                             bool bAddChild)
    {
        card = GetEntityManager()->CreateEntity<GuiComponent>(title);

        card->m_transform.position.x = 0;
        card->m_transform.position.y = 0;
        card->m_transform.position.width = m_transform.position.width;
        card->m_transform.position.height = m_transform.position.height;


        card->m_transform.source.x = sourceX;
        card->m_transform.source.y = sourceY;
        card->m_transform.source.width = m_transform.position.width;
        card->m_transform.source.height = m_transform.position.height;
        card->m_transform.color.SetOpacity(alpha);
        card->mTextureName = "sprite";
        if (bAddChild)
        {
            AddChild(card);
        }
    }

    void GuiCard::CreateSizeBox()
    {
        // mSizeBoxImage = GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBoxImage");
        // mSizeBoxImage->m_transform.position.width = 228.0f;
        // mSizeBoxImage->m_transform.position.height = 204.0f;
        // //mSizeBoxImage->m_transform.offset.x = 12.0f;
        // //mSizeBoxImage->m_transform.offset.y = 12.0f;
        // mSizeBoxImage->AttachGui(mCover);
        // AddChild(mSizeBoxImage);

        // mSizeBoxVideoPlayer = GetEntityManager()->CreateEntity<GuiSizeBox>("GuiSizeBoxVideo");
        // mSizeBoxVideoPlayer->m_transform.position.width = 228.0f;
        // mSizeBoxVideoPlayer->m_transform.position.height = 204.0f;
        // //mSizeBoxVideoPlayer->m_transform.offset.x = 12.0f;
        // //mSizeBoxVideoPlayer->m_transform.offset.y = 12.0f;
        // mSizeBoxVideoPlayer->AttachGui(mGuiVideoPlayer);
        // AddChild(mSizeBoxVideoPlayer);
    }

    void GuiCard::SetCover(const std::string& name)
    {
        // m_coverDefault->m_transform.position.x = 24.0f;
        // m_coverDefault->m_transform.position.y = 13.0f;
        SetOffset(m_coverDefault, {24.0f, 13.0f});
        m_coverDefault->m_transform.position.width = 204.0f;
        m_coverDefault->m_transform.position.height = 202.0f;

        m_coverDefault->m_transform.source.x = 771;
        m_coverDefault->m_transform.source.y = 283;
        m_coverDefault->m_transform.source.width = 204.0f;
        m_coverDefault->m_transform.source.height = 202.0f;
        m_coverDefault->mTextureName = "sprite";

        m_cover->mTextureName = "transparent";
        if (!name.empty())
        {
            //m_cover->m_transform.position.x = 12.0f;
            //m_cover->m_transform.position.y = 12.0f;
            SetOffset(m_cover, {12.0f, 12.0f});
            m_cover->m_transform.position.width = 228.0f;
            m_cover->m_transform.position.height = 204.0f;

            m_cover->m_transform.source.x = 0;
            m_cover->m_transform.source.y = 0;
            m_cover->m_transform.source.width = 228.0f;
            m_cover->m_transform.source.height = 204.0f;
            m_cover->mTextureName = name;
        }
    }

    void GuiCard::Update()
    {
        GuiCanvas::Update();

        Texture* textureReference = GetSpriteManager()->GetTexture(m_cover->mTextureName);
        if (textureReference != nullptr)
        {
            textureReference->SetSmooth(true);

            const float renderScale = ThemesManager::GetScaleRenderer();

            const float widthTex = textureReference->GetSize().x / renderScale;
            const float HeightTex = textureReference->GetSize().y / renderScale;
            const float xCoverPos = ((228.0f - widthTex) / 2.0f) + 12;
            const float yCoverPos = ((204.0f - HeightTex) / 2.0f) + 12;
            SetOffset(m_cover, {xCoverPos, yCoverPos});
            m_cover->m_transform.position.width = widthTex;
            m_cover->m_transform.position.height = HeightTex;

            m_cover->m_transform.source.x = 0;
            m_cover->m_transform.source.y = 0;
            m_cover->m_transform.source.width = widthTex;
            m_cover->m_transform.source.height = HeightTex;
        }


        // mSizeBox->mProperties.offset.x += 0.03f;
        // mSizeBox->mProperties.offset.y += 0.03f;
        // const Texture* textureReference = GetSpriteManager()->GetTexture(mCover->mTextureName);
        // const Animation& pAnim = GetAnimation("card-zoom");
        // if (textureReference != nullptr && mCover->mTextureName != "sprite" && !pAnim.mIsRunning &&
        //     mCover->m_transform.position.width == 0 && mCover->m_transform.position.height == 0)
        // {
        //     const float scale = ThemesManager::GetScaleTexture();
        //     mCover->m_transform.position.width = textureReference->GetSize().x / scale;
        //     mCover->m_transform.position.height = textureReference->GetSize().y / scale;
        //     mSizeBoxImage->UpdateGuiAttachment();
        //}
    }

    void GuiCard::FocusAnimation(bool bForce, const int a, const int b, const char* nameAnimation)
    {
        // if (bForce)
        // {
        //     mCardSelected->m_transform.color.SetOpacity(a);
        //     mCardBackgroundSelected->m_transform.color.SetOpacity(a);
        //     return;
        // }
        // mCardSelected->m_transform.color.SetOpacity(b);
        // mCardBackgroundSelected->m_transform.color.SetOpacity(b);
        // Transform target = mCardSelected->m_transform;
        // target.color.a = a;
        // mCardSelected
        //     ->StartAnimation(nameAnimation, 0.2f, mCardSelected->m_transform, target, Ease::EaseLinearNone, false);
        // mCardBackgroundSelected
        //     ->StartAnimation(nameAnimation, 0.2f, mCardSelected->m_transform, target, Ease::EaseLinearNone, false);
    }

    void GuiCard::StartVideo()
    {

        if (mIsFocus)
        {
            mGuiVideoPlayer->Init(m_gameListManagerRef->GetCurrentGameList()->video, 228, 204);
        }

        // if (mIsFocus)
        // {
        //mGuiVideoPlayer->Init("/mnt/arquivos/Emulators/roms/switch/media/videos/Mario Party Superstars [01006FE013472000].mp4", 228, 204);
        //mGuiVideoPlayer->Init("/mnt/arquivos/Emulators/roms/snes/brazil/media/videos/Top Racer (J) [T+Por].mp4", 228, 204);
        //mGuiVideoPlayer->Init("/mnt/arquivos/Emulators/roms/snes/brazil/media/videos/Megaman VII (U) [T+Por].mp4", 228, 204);
        //     mSizeBoxVideoPlayer->SetCropGuiAttachment(true);
        // }
    }

    void GuiCard::SetCardFocus(bool bForce)
    {
        mIsFocus = true;
        //FocusAnimation(bForce, 255, 0, "card-focus");
        GetTimerManager()->SetTimer(mTimerVideo, CALLFUNCTION(StartVideo, this), this, 5.0f);
        SetFocus();
    }

    void GuiCard::RemoveCardFocus(bool bForce)
    {
        mIsFocus = false;
        // FocusAnimation(bForce, 0, 255, "card-lost-focus");
        mGuiVideoPlayer->Stop();
        // mSizeBoxVideoPlayer->SetCropGuiAttachment(false);
    }

    void GuiCard::OnFocus()
    {
    }

    void GuiCard::OnLostFocus()
    {
    }

    bool GuiCard::IsFocus() const
    {
        return mIsFocus;
    }

    void GuiCard::Reset()
    {
        // mIsFront = false;
        // m_transform.scale.x = 1.0f;
        // m_transform.scale.y = 1.0f;

        // mCardBackgroundMain->m_transform.color.SetOpacity(255);
        // mCardMain->m_transform.color.SetOpacity(255);
        // mCardBackgroundSelected->m_transform.color.SetOpacity(255);
        // mCardSelected->m_transform.color.SetOpacity(255);
        // mCover->m_transform.color.SetOpacity(255);
        // mSizeBoxImage->m_transform.color.SetOpacity(255);
        // mSizeBoxVideoPlayer->m_transform.color.SetOpacity(255);
        // mGuiVideoPlayer->m_transform.color.SetOpacity(255);

        // mCardBackgroundFavorite->m_transform.color.SetOpacity(0); // todo create logic is favorite
        // mCardFavorite->m_transform.color.SetOpacity(0);           // todo create logic is favorite
        // if (mCover->mTextureName == "sprite")
        // {
        //     SetCover();
        // }
        // if (!mIsFocus)
        // {
        //     RemoveCardFocus(true);
        // }
    }

    void GuiCard::Click()
    {
        // mGuiVideoPlayer->Stop();
        // GetTimerManager()->ClearTimer(mTimerVideo);

        // mIsFront = true;

        const float time = 3.3f;
        const float scale = 2.0f;

        Transform target = m_transform ;

       // target.position = m_worldTransform.position;
        //m_transform.position.x = -100;
        //return; 

        target.scale.x = scale;
        target.scale.y = scale;

        const float width = target.position.width * m_worldTransform.scale.x;
        const float height = target.position.height * m_worldTransform.scale.y;

        //target.position.x +=  (-width / 2 * scale) + width / 2;
        //target.position.y +=  (-width / 2 * scale) + width / 2;
        target.position.x = -width * (scale - 1);
        //target.position.y += -height * (scale - 1);

        target.color.a = 0;

        //m_animationTransform.position.x = -10;
        StartAnimation("card-zoom", time, m_worldTransform, target, Ease::EaseQuadInOut, true);
        // GetTimerManager()->SetTimer(mTimer, CALLFUNCTION(Reset, this), this, time * 2);
    }

    void GuiCard::SetFrontCard()
    {

        // const int order = (mIsFront) ? 1 : 0;

        // GetEntityManager()->SetZOrder(mCardSelected, order);
        // GetEntityManager()->SetZOrder(mCardBackgroundSelected, order);
        // GetEntityManager()->SetZOrder(mCardMain, order);
        // GetEntityManager()->SetZOrder(mCardBackgroundMain, order);
        // GetEntityManager()->SetZOrder(mCardFavorite, order);
        // GetEntityManager()->SetZOrder(mCardBackgroundFavorite, order);
        // GetEntityManager()->SetZOrder(mCover, order);
        // GetEntityManager()->SetZOrder(mGuiVideoPlayer, order);
    }

    void GuiCard::SetThemeValue()
    {
        // mSizeBoxImage->m_transform.offset.x = ThemesManager::Get().mConfigurationThemes.offsetImageX;
        // mSizeBoxImage->m_transform.offset.y = ThemesManager::Get().mConfigurationThemes.offsetImageY;
        // mSizeBoxVideoPlayer->m_transform.offset.x = ThemesManager::Get().mConfigurationThemes.offsetVideoX;
        // mSizeBoxVideoPlayer->m_transform.offset.y = ThemesManager::Get().mConfigurationThemes.offsetVideoY;
    }

} // namespace ClassicLauncher
