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
#include "Guis/GuiBase.h"

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
        SetPosition(0.0f, 0.0f);
        SetSize(256.0f, 280.0f);

        CreateCard(m_cardBackgroundMain, 0, 281, 255, "GuiCardBackgroundMain");
        CreateCard(m_cardBackgroundFavorite, 514, 281, 0, "GuiCardBackgroundFavorite");
        CreateCard(m_cardBackgroundSelected, 257, 281, 0, "GuiCardBackgroundSelected");

        CreateCard(m_coverDefault, 0, 0, 255, "GuiCover", true);
        CreateCard(m_cover, 0, 0, 255, "GuiCover", false);
        AddChild(m_cover);

        m_guiVideoPlayer = GetEntityManager()->CreateEntity<GuiVideoPlayer>("GuiVideoPlayer");
        m_guiVideoPlayer->SetOffset(12.0f, 12.0f);
        AddChild(m_guiVideoPlayer);

        CreateCard(m_cardMain, 0, 0, 255, "GuiCardMain");
        CreateCard(m_cardFavorite, 514, 0, 0, "GuiCardFavorite");
        CreateCard(m_cardSelected, 257, 0, 0, "GuiCardSelected");


        m_coverDefault->SetOffset(24.0f, 13.0f);
        m_coverDefault->SetSize(204.0f, 202.0f);  
        m_coverDefault->SetSource(771.0f, 283.0f, 204.0f, 202.0f);
        m_coverDefault->m_textureName = "sprite";

        SetCover();



        // mCardSelected->SetOffset(Vector2f{-280, -30 });
        // mCardMain->SetOffset(Vector2f{0, 30 });
    }

    void GuiCard::CreateCard(GuiBase*& card,
                             const float sourceX,
                             const float sourceY,
                             unsigned char alpha,
                             const char* title,
                             bool bAddChild)
    {
        card = GetEntityManager()->CreateEntity<GuiBase>(title);

        card->SetPosition(Vector2f{});
        card->SetSize(GetSize());
        card->SetSource(RectFloat{sourceX, sourceY, GetSize().width, GetSize().height});
        card->SetOpacity(alpha);
        card->m_textureName = "sprite";
        if (bAddChild)
        {
            AddChild(card);
        }
    }

    void GuiCard::SetCover(const std::string& name)
    {

        m_coverDefault->SetOpacity(255);
        m_cover->m_textureName = "transparent";
        m_cover->SetSize(Sizef{});  

        if (!name.empty())
        {
            m_coverDefault->SetOpacity(0);
            m_cover->SetOffset(Vector2f{12.0f});
            m_cover->SetSize(Sizef{228.0f, 204.0f});    
            m_cover->SetSource(RectFloat{0.0f, 0.0f, 228.0f, 204.0f});
            m_cover->SetSize(Sizef{228.0f, 204.0f});
            m_cover->m_textureName = name;
            m_isChangeTexture = true;
        }
    }

    void GuiCard::Update()
    {
        GuiCanvas::Update();
        Animatable::UpdateAnimation();

        Texture* textureReference = GetSpriteManager()->GetTexture(m_cover->m_textureName);
        if (textureReference != nullptr && m_isChangeTexture)
        {
            textureReference->SetSmooth(true);

            const float renderScale = ThemesManager::GetScaleRenderer();
            ConfigurationThemes theme = ThemesManager::GetConfigurationThemes(); 

            const float widthTex = textureReference->GetSize().width / renderScale;
            const float HeightTex = textureReference->GetSize().height / renderScale;
            const float xCoverPos = ((228.0f - widthTex) / 2.0f) + theme.offsetImageX;
            const float yCoverPos = ((204.0f - HeightTex) / 2.0f) + theme.offsetImageY;

            m_cover->SetOffset(Vector2f{xCoverPos, yCoverPos}); 
            m_cover->SetSize(Vector2f{widthTex, HeightTex}); 
            m_cover->SetSource(RectFloat{0.0f, 0.0f, widthTex, HeightTex});
            m_isChangeTexture = false;
        }


        if (IsFocus() && InputManager::IsRelease(InputName::leftFaceDown, InputCategory::MAIN))
        {
            m_guiVideoPlayer->InitFullscreen();
            InputManager::SetCategory(VIDEO_FULLSCREEN);
            InputManager::RemoveCategory(MAIN);
        }
        if (IsFocus() && InputManager::IsRelease(InputName::rightFaceRight, InputCategory::VIDEO_FULLSCREEN))
        {   
            InputManager::SetCategory(MAIN);
            InputManager::RemoveCategory(VIDEO_FULLSCREEN);
            m_guiVideoPlayer->StopFullscreen();
        }
        
    }

    void GuiCard::FocusAnimation(bool bForce, const int alphaA, const int alphaB, const std::string& nameAnimation)
    {
        // if (bForce)
        // {
        //     mCardSelected->m_transform.color.SetOpacity(alphaA);
        //     mCardBackgroundSelected->m_transform.color.SetOpacity(alphaA);
        //     return;
        // }
        // mCardSelected->m_transform.color.SetOpacity(alphaB);
        // mCardBackgroundSelected->m_transform.color.SetOpacity(alphaB);
        // Transform target = mCardSelected->m_transform;
        // target.color.a = alphaA;
        // mCardSelected->GetAnimationManager().StartAnimation(nameAnimation, 0.2f, mCardSelected->m_transform, target, Ease::EaseLinearNone, false);
        // mCardBackgroundSelected->GetAnimationManager().StartAnimation(nameAnimation, 0.2f, mCardSelected->m_transform, target, Ease::EaseLinearNone, false);
    }

    void GuiCard::StartVideo()
    {
        if (IsFocus())
        {
            const bool isPlay = m_guiVideoPlayer->Init(m_gameListManagerRef->GetCurrentGameList()->video, 228, 204);
            m_audioManagerRef->MusicVolume(isPlay ? 0.1f : 1.0f);
        }
    }

    void GuiCard::SetCardFocus(bool bForce)
    {
        GetTimerManager()->SetTimer(m_timerVideo, CALLFUNCTION(StartVideo, this), this, 5.0f);     
        SetFocus();

        // if (bForce)
        // {
        //     mCardSelected->m_transform.color.SetOpacity(255);
        //     mCardBackgroundSelected->m_transform.color.SetOpacity(255);
        // }

        //FocusAnimation(bForce, 255, 0, "card-focus");


    }

    void GuiCard::RemoveCardFocus(bool bForce)
    {
        // FocusAnimation(bForce, 0, 255, "card-lost-focus");
        //m_guiVideoPlayer->Stop();
        //m_audioManagerRef->MusicVolume(1.0f);


    }

    void GuiCard::OnFocus()
    {
         Transform targetA = m_cardSelected->GetTransform();
         targetA.color.a = 255;
         Transform targetB = m_cardBackgroundSelected->GetTransform();
         targetB.color.a = 255;
         Transform targetC = m_cardMain->GetTransform();
         targetC.color.a = 0;
         GetAnimationManager().StartAnimation("focus-card-a" , 0.2f, m_cardSelected, targetA, Ease::EaseLinearNone, false);
         GetAnimationManager().StartAnimation("focus-card-b" , 0.2f, m_cardBackgroundSelected, targetB, Ease::EaseLinearNone, false);
    }

    void GuiCard::OnLostFocus()
    {
        if (IsFocus())
        {
            return;
        }

        CloseVideo();
        
        Transform targetA = m_cardSelected->GetTransform();
        targetA.color.a = 0;
        Transform targetB = m_cardBackgroundSelected->GetTransform();
        targetB.color.a = 0;
        GetAnimationManager().StartAnimation("remove-focus-card-a" , 0.2f, m_cardSelected, targetA, Ease::EaseLinearNone, false);
        GetAnimationManager().StartAnimation("remove-focus-card-b" , 0.2f, m_cardBackgroundSelected, targetB, Ease::EaseLinearNone, false);
    }

    void GuiCard::Reset()
    {
        SetScale(Vector2f{1.0f, 1.0f});

        SetOpacity(255);

        m_cardBackgroundFavorite->SetOpacity(0); // todo create logic is favorite
        m_cardFavorite->SetOpacity(0);           // todo create logic is favorite   
    }

    void GuiCard::Click()
    {
        m_guiVideoPlayer->Stop();
        GetTimerManager()->ClearTimer(m_timerVideo);


        const float time = 0.3f;
        const float scale = 1.75f;

        Transform target = GetTransform();

        target.scale.x = scale * GetTransform().scale.x;
        target.scale.y = scale * GetTransform().scale.y;

        const float width = target.position.width * GetTransform().scale.x;
        const float height = target.position.height * GetTransform().scale.y;

        target.position.x += (-width / 2 * scale) + width / 2;
        target.position.y += (-height / 2 * scale) + height / 2;

        target.color.a = 0;

        GetAnimationManager().StartAnimation("card-zoom", time, this, target, Ease::EaseQuadInOut, true);
        GetTimerManager()->SetTimer(m_timerAnimationReset, CALLFUNCTION(Reset, this), this, time * 2);
    }

    void GuiCard::SetFrontCard()
    {

        const int order = (IsFocus()) ? 1 : 0;

        GetEntityManager()->SetZOrder(m_cardSelected, order);
        GetEntityManager()->SetZOrder(m_cardBackgroundSelected, order);
        GetEntityManager()->SetZOrder(m_cardMain, order);
        GetEntityManager()->SetZOrder(m_cardBackgroundMain, order);
        GetEntityManager()->SetZOrder(m_cardFavorite, order);
        GetEntityManager()->SetZOrder(m_cardBackgroundFavorite, order);
        GetEntityManager()->SetZOrder(m_cover, order);
        GetEntityManager()->SetZOrder(m_coverDefault, order);
        GetEntityManager()->SetZOrder(m_guiVideoPlayer, order);
    }

    void GuiCard::CloseVideo()
    {
        m_guiVideoPlayer->Stop();
        m_audioManagerRef->MusicVolume(1.0f);
    }

    void GuiCard::SetThemeValue()
    {
        ConfigurationThemes theme = ThemesManager::GetConfigurationThemes(); 
        m_cover->SetOffset(theme.offsetImageX ,theme.offsetImageY);
        m_coverDefault->SetOffset(theme.offsetDefaultImageX ,theme.offsetDefaultImageY);
        m_guiVideoPlayer->SetOffset(theme.offsetVideoX ,theme.offsetVideoY);
    }

} // namespace ClassicLauncher
