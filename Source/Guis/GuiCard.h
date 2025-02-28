#ifndef GUI_CARD_H
#define GUI_CARD_H

#include <memory>
#include <string>
#include "GuiComponent.h"
#include "Components/FocusComponent.h"

namespace ClassicLauncher
{
    class EntityGui;
    class GuiComponent;
    class GuiSizeBox;
    class GuiVideoPlayer;
    class FocusComponent;

    class GuiCard : public EntityGui, FocusComponent
    {
    private:

        TimerHandling mTimer;
        TimerHandling mTimerVideo;

        GuiComponent* mCardMain;
        GuiComponent* mCardSelected;
        GuiComponent* mCardFavorite;
        GuiComponent* mCardBackgroundMain;
        GuiComponent* mCardBackgroundSelected;
        GuiComponent* mCardBackgroundFavorite;
        GuiSizeBox* mSizeBoxImage;
        GuiSizeBox* mSizeBoxVideoPlayer;
        GuiVideoPlayer* mGuiVideoPlayer;
        GuiComponent* mCover;
        bool mIsFocus = false;
        bool mIsFront = false;
        void CreateCard(GuiComponent*& card, float sourceX, float sourceY, unsigned char alpha, const char* title, bool bAddChild = true);
        void CreateSizeBox();
        void StartVideo();
        void FocusAnimation(bool bForce, int a, const int b, const char* nameAnimation);

    public:

        GuiCard(float x, float y);
        virtual ~GuiCard() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiCardClass; }
        void Update() override;
        void SetCardFocus(bool bForce = false);
        void RemoveCardFocus(bool bForce = false);
        virtual void OnFocus() override;
        virtual void OnLostFocus() override;
        void SetCover(std::string name = "");
        bool IsFocus();
        void Reset();
        void Click();
        void SetFrontCard();
    };

}  // namespace ClassicLauncher

#endif  // GUI_CARD_H