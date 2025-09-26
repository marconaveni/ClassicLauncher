#include "GuiCard.h"

#include "Data/GameListManager.h"
#include "Entity/EntityManager.h"
#include "Graphics/SpriteManager.h"
#include "Graphics/Texture.h"
#include "Guis/Components/GuiSizeBox.h"
#include "Guis/GuiVideoPlayer.h"
#include "Themes/ThemesManager.h"
#include "Audio/AudioManager.h"
#include "Input/InputManager.h"

namespace ClassicLauncher
{

    GuiCard::GuiCard(GameListManager* gameListManagerRef, FocusManager* focusManagerRef, AudioManager* audioManagerRef)
        : FocusComponent(focusManagerRef)
        , m_timerVideo()
        , m_gameListManagerRef(gameListManagerRef)
        , m_audioManagerRef(audioManagerRef)
    {
        // aqui os ponteiros m_entityManagerReference ainda está nulo como documento isso melhor para evitar de chamar o Getters
    }

    void GuiCard::CreateCards(int x, int y)
    {
        m_transform.position.x = 0; //static_cast<float>(x);
        m_transform.position.y = 0; //static_cast<float>(y);
        m_transform.position.width = 256;
        m_transform.position.height = 280;
        //m_transform.origin = {256 / 2 , 280 / 2};

        CreateCard(mCardBackgroundMain, 0, 281, 255, "GuiCardBackgroundMain");
        CreateCard(mCardBackgroundFavorite, 514, 281, 0, "GuiCardBackgroundFavorite");
        CreateCard(mCardBackgroundSelected, 257, 281, 0, "GuiCardBackgroundSelected");

        CreateCard(m_coverDefault, 0, 0, 255, "GuiCover", true);
        CreateCard(m_cover, 0, 0, 255, "GuiCover", false);
        AddChild(m_cover);

        mGuiVideoPlayer = GetEntityManager()->CreateEntity<GuiVideoPlayer>("GuiVideoPlayer");
        mGuiVideoPlayer->m_transform.offset.x = 12;
        mGuiVideoPlayer->m_transform.offset.y = 12;
        AddChild(mGuiVideoPlayer);

        CreateCard(mCardMain, 0, 0, 255, "GuiCardMain");
        CreateCard(mCardFavorite, 514, 0, 0, "GuiCardFavorite");
        CreateCard(mCardSelected, 257, 0, 0, "GuiCardSelected");


        m_coverDefault->m_transform.offset.x = 24.0f;
        m_coverDefault->m_transform.offset.y = 13.0f;
        m_coverDefault->m_transform.position.width = 204.0f;
        m_coverDefault->m_transform.position.height = 202.0f;

        m_coverDefault->m_transform.source.x = 771;
        m_coverDefault->m_transform.source.y = 283;
        m_coverDefault->m_transform.source.width = 204.0f;
        m_coverDefault->m_transform.source.height = 202.0f;
        m_coverDefault->mTextureName = "sprite";

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

    void GuiCard::SetCover(const std::string& name)
    {

        m_coverDefault->m_transform.color.SetOpacity(255);
        m_cover->mTextureName = "transparent";

        if (!name.empty())
        {
            m_cover->m_transform.offset.x = 12.0f;
            m_cover->m_transform.offset.y = 12.0f;
            m_cover->m_transform.position.width = 228.0f;
            m_cover->m_transform.position.height = 204.0f;

            m_cover->m_transform.source.x = 0;
            m_cover->m_transform.source.y = 0;
            m_cover->m_transform.source.width = 228.0f;
            m_cover->m_transform.source.height = 204.0f;
            m_cover->mTextureName = name;
            m_coverDefault->m_transform.color.SetOpacity(0);
            m_isChangeTexture = true;
        }
    }

    void GuiCard::Update()
    {
        GuiCanvas::Update();

        Texture* textureReference = GetSpriteManager()->GetTexture(m_cover->mTextureName);
        if (textureReference != nullptr && m_isChangeTexture)
        {
            textureReference->SetSmooth(true);

            const float renderScale = ThemesManager::GetScaleRenderer();

            const float widthTex = textureReference->GetSize().x / renderScale;
            const float HeightTex = textureReference->GetSize().y / renderScale;
            const float xCoverPos = ((228.0f - widthTex) / 2.0f) + 12;
            const float yCoverPos = ((204.0f - HeightTex) / 2.0f) + 12;

            m_cover->m_transform.offset.x = xCoverPos;
            m_cover->m_transform.offset.y = yCoverPos;
            m_cover->m_transform.position.width = widthTex;
            m_cover->m_transform.position.height = HeightTex;

            m_cover->m_transform.source.x = 0;
            m_cover->m_transform.source.y = 0;
            m_cover->m_transform.source.width = widthTex;
            m_cover->m_transform.source.height = HeightTex;
            m_isChangeTexture = false;
        }


        if (IsFocus() && Keyboard::IsReleased(Keyboard::DOWN))
        {
            mGuiVideoPlayer->InitFullscreen();
        }
        if (IsFocus() && Keyboard::IsReleased(Keyboard::UP))
        {   
            mGuiVideoPlayer->StopFullscreen();
        }
        
    }

    void GuiCard::FocusAnimation(bool bForce, const int a, const int b, const char* nameAnimation)
    {
        if (bForce)
        {
            mCardSelected->m_transform.color.SetOpacity(a);
            mCardBackgroundSelected->m_transform.color.SetOpacity(a);
            return;
        }
        mCardSelected->m_transform.color.SetOpacity(b);
        mCardBackgroundSelected->m_transform.color.SetOpacity(b);
        Transform target = mCardSelected->m_transform;
        target.color.a = a;
        mCardSelected->StartAnimation(nameAnimation, 0.2f, mCardSelected->m_transform, target, Ease::EaseLinearNone, false);
        mCardBackgroundSelected->StartAnimation(nameAnimation, 0.2f, mCardSelected->m_transform, target, Ease::EaseLinearNone, false);
    }

    void GuiCard::StartVideo()
    {
        if (IsFocus())
        {
            const bool isPlay = mGuiVideoPlayer->Init(m_gameListManagerRef->GetCurrentGameList()->video, 228, 204);
            m_audioManagerRef->MusicVolume(isPlay ? 0.1f : 1.0f);
        }
    }

    void GuiCard::SetCardFocus(bool bForce)
    {
        FocusAnimation(bForce, 255, 0, "card-focus");
        GetTimerManager()->SetTimer(m_timerVideo, CALLFUNCTION(StartVideo, this), this, 5.0f);
        SetFocus();
    }

    void GuiCard::RemoveCardFocus(bool bForce)
    {
        FocusAnimation(bForce, 0, 255, "card-lost-focus");
        mGuiVideoPlayer->Stop();
        m_audioManagerRef->MusicVolume(1.0f);
    }

    void GuiCard::OnFocus()
    {
    }

    void GuiCard::OnLostFocus()
    {
    }

    // bool GuiCard::IsFocus() const
    // {
    //     return mIsFocus;
    // }

    void GuiCard::Reset()
    {
        // mIsFront = false;
        m_transform.scale.x = 1.0f;
        m_transform.scale.y = 1.0f;

        m_transform.color.SetOpacity(255);
        //mCardBackgroundMain->m_transform.color.SetOpacity(255);
        //mCardMain->m_transform.color.SetOpacity(255);
        //mCardBackgroundSelected->m_transform.color.SetOpacity(255);
        //mCardSelected->m_transform.color.SetOpacity(255);
        //m_cover->m_transform.color.SetOpacity(255);
        //m_coverDefault->m_transform.color.SetOpacity(255);
        //mGuiVideoPlayer->m_transform.color.SetOpacity(255);

        mCardBackgroundFavorite->m_transform.color.SetOpacity(0); // todo create logic is favorite
        mCardFavorite->m_transform.color.SetOpacity(0);           // todo create logic is favorite
        
        // if (m_cover->mTextureName == "sprite")
        // {
        //     SetCover();
        // }
        //if (!mIsFocus)
        //{
        //    RemoveCardFocus(true);
        //}
    }

    void GuiCard::Click()
    {
        mGuiVideoPlayer->Stop();
        GetTimerManager()->ClearTimer(m_timerVideo);


        const float time = 0.3f;
        const float scale = 1.75f;

        Transform target = m_transform;

        target.scale.x = scale;
        target.scale.y = scale;

        const float width = target.position.width * m_worldTransform.scale.x;
        const float height = target.position.height * m_worldTransform.scale.y;

        target.position.x += (-width / 2 * scale) + width / 2;
        target.position.y += (-height / 2 * scale) + height / 2;

        target.color.a = 0;

        StartAnimation("card-zoom", time, m_transform, target, Ease::EaseQuadInOut, false);
        GetTimerManager()->SetTimer(m_timerAnimationReset, CALLFUNCTION(Reset, this), this, time * 2);
    }

    void GuiCard::SetFrontCard()
    {

        const int order = (IsFocus()) ? 1 : 0;

        GetEntityManager()->SetZOrder(mCardSelected, order);
        GetEntityManager()->SetZOrder(mCardBackgroundSelected, order);
        GetEntityManager()->SetZOrder(mCardMain, order);
        GetEntityManager()->SetZOrder(mCardBackgroundMain, order);
        GetEntityManager()->SetZOrder(mCardFavorite, order);
        GetEntityManager()->SetZOrder(mCardBackgroundFavorite, order);
        GetEntityManager()->SetZOrder(m_cover, order);
        GetEntityManager()->SetZOrder(m_coverDefault, order);
        GetEntityManager()->SetZOrder(mGuiVideoPlayer, order);
    }

    void GuiCard::SetThemeValue()
    {
        // mSizeBoxImage->m_transform.offset.x = ThemesManager::Get().mConfigurationThemes.offsetImageX;
        // mSizeBoxImage->m_transform.offset.y = ThemesManager::Get().mConfigurationThemes.offsetImageY;
        // mSizeBoxVideoPlayer->m_transform.offset.x = ThemesManager::Get().mConfigurationThemes.offsetVideoX;
        // mSizeBoxVideoPlayer->m_transform.offset.y = ThemesManager::Get().mConfigurationThemes.offsetVideoY;
    }

} // namespace ClassicLauncher
