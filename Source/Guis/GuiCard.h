#ifndef GUI_CARD_H
#define GUI_CARD_H


#include <string>
#include "GuiComponent.h"
#include "Utils/TimerManager.h"
#include "Components/FocusComponent.h"
#include "Entity/Entity.h"
#include "Entity/EntityGui.h"

namespace ClassicLauncher
{
    class EntityGui;
    class GuiComponent;
    class GuiSizeBox;
    class GuiVideoPlayer;
    class FocusComponent;

    class GuiCard : public EntityGui, FocusComponent
    {

    public:

        GuiCard(int x, int y);
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
        void FocusAnimation(bool bForce, int a,  int b, const char* nameAnimation);
    };

}  // namespace ClassicLauncher

#endif  // GUI_CARD_H