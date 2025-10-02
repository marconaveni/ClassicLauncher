#ifndef GUI_CARD_H
#define GUI_CARD_H


#include <string>

#include "Components/FocusComponent.h"
#include "Entity/Entity.h"
#include "Animations/Animatable.h"
#include "Guis/Components/GuiCanvas.h"
#include "Utils/TimerManager.h"

namespace ClassicLauncher
{
    class Animatable;
    class GuiBase;
    class GuiSizeBox;
    class GuiVideoPlayer;
    class FocusComponent;
    class EntityManager;
    class GameListManager;
    class AudioManager;

    class GuiCard : public GuiCanvas, public FocusComponent, public Animatable
    {

    public:

        GuiCard(GameListManager* gameListManagerRef, FocusManager* focusManagerRef, AudioManager* audioManagerRef);
        void CreateCards(int x, int y);


        virtual EntityType GetType() const override { return EntityType::GuiCardClass; }
        void Update() override;
        void SetCardFocus(bool bForce = false);
        void RemoveCardFocus(bool bForce = false);
        virtual void OnFocus() override;
        virtual void OnLostFocus() override;
        virtual const Transform& OwnerWorldTransform() const override { return GetWorldTransform(); };
        void SetCover(const std::string& name = "");
        void Reset();
        void Click();
        void SetFrontCard();
        virtual void SetThemeValue() override;


    private:

        TimerHandling m_timerVideo;
        TimerHandling m_timerAnimationReset;

        GuiBase* m_cardMain;
        GuiBase* m_cardSelected;
        GuiBase* m_cardFavorite;
        GuiBase* m_cardBackgroundMain;
        GuiBase* m_cardBackgroundSelected;
        GuiBase* m_cardBackgroundFavorite;
        GuiBase* m_coverDefault;
        GuiBase* m_cover;
        GuiVideoPlayer* m_guiVideoPlayer;

        bool m_isChangeTexture = false;

        GameListManager* m_gameListManagerRef;
        AudioManager* m_audioManagerRef;

        void CreateCard(GuiBase*& card,
                        float sourceX,
                        float sourceY,
                        unsigned char alpha,
                        const char* title,
                        bool bAddChild = true);
        void StartVideo();
        void FocusAnimation(bool bForce, const int alphaA, const int alphaB, const std::string& nameAnimation);
    };

} // namespace ClassicLauncher

#endif // GUI_CARD_H