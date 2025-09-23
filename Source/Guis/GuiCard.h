#ifndef GUI_CARD_H
#define GUI_CARD_H


#include <string>

#include "Components/FocusComponent.h"
#include "Entity/Entity.h"
#include "Guis/Components/GuiCanvas.h"
#include "GuiComponent.h"
#include "Utils/TimerManager.h"

namespace ClassicLauncher
{
    class EntityGui;
    class GuiComponent;
    class GuiSizeBox;
    class GuiVideoPlayer;
    class FocusComponent;
    class EntityManager;
    class GameListManager;
    class AudioManager;

    class GuiCard : public GuiCanvas, FocusComponent
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
        void SetCover(const std::string& name = "");
        [[nodiscard]] bool IsFocus() const;
        void Reset();
        void Click();
        void SetFrontCard();
        virtual void SetThemeValue() override;


    private:

        TimerHandling mTimerVideo;

        GuiComponent* mCardMain;
        GuiComponent* mCardSelected;
        GuiComponent* mCardFavorite;
        GuiComponent* mCardBackgroundMain;
        GuiComponent* mCardBackgroundSelected;
        GuiComponent* mCardBackgroundFavorite;
        GuiVideoPlayer* mGuiVideoPlayer;
        GuiComponent* m_coverDefault;
        GuiComponent* m_cover;
        bool mIsFocus = false;
        bool mIsFront = false;
        bool m_isChangeTexture = false;

        GameListManager* m_gameListManagerRef;
        AudioManager* m_audioManagerRef;

        void CreateCard(GuiComponent*& card,
                        float sourceX,
                        float sourceY,
                        unsigned char alpha,
                        const char* title,
                        bool bAddChild = true);
        void StartVideo();
        void FocusAnimation(bool bForce, int a, int b, const char* nameAnimation);
    };

} // namespace ClassicLauncher

#endif // GUI_CARD_H