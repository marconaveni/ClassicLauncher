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
        virtual void Update() override;
        void SetCardFocus(bool bForce = false);
        void RemoveCardFocus(bool bForce = false);
        virtual void OnFocus() override;
        virtual void OnLostFocus(FocusCategory previousFocusCategory) override;
        virtual const Transform& OwnerWorldTransform() const override { return GetWorldTransform(); };
        void SetCover(const std::string& name = "");
        void Reset();
        void Click();
        void SetFrontCard();
        void CloseVideo();
        virtual void SetThemeValue() override;


    private:

        TimerHandling m_timerVideo{};
        TimerHandling m_timerAnimationReset{};

        GuiBase* m_cardMain{nullptr};
        GuiBase* m_cardSelected{nullptr};
        GuiBase* m_cardFavorite{nullptr};
        GuiBase* m_cardBackgroundMain{nullptr};
        GuiBase* m_cardBackgroundSelected{nullptr};
        GuiBase* m_cardBackgroundFavorite{nullptr};
        GuiBase* m_coverDefault{nullptr};
        GuiBase* m_cover{nullptr};
        GuiVideoPlayer* m_guiVideoPlayer{nullptr};

        bool m_isChangeTexture{false};

        GameListManager* m_gameListManagerRef{nullptr};
        AudioManager* m_audioManagerRef{nullptr};

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