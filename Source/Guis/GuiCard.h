#ifndef GUI_CARD_H
#define GUI_CARD_H

#include <memory>
#include <string>
#include "GuiComponent.h"

namespace ClassicLauncher
{
    class EntityGui;
    class GuiComponent;
    class GuiSizeBox;

    class GuiCard : public EntityGui
    {
    private:
        
        TimerHandling mTimer;

        GuiComponent* mCardMain;
        GuiComponent* mCardSelected;
        GuiComponent* mCardFavorite;
        GuiSizeBox* mSizeBox;
        GuiComponent* mCover;
        bool mIsFocus = false;
        bool mIsFront = false;
        void CreateCard(GuiComponent*& card, float sourceX, float sourceY, unsigned char alpha, const char* title);
        void CreateSizeBox();

    public:

        GuiCard(float x, float y);
        virtual ~GuiCard() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiCardClass; }
        void Update() override;
        void SetFocus(bool bForce = false);
        void RemoveFocus(bool bForce = false);
        void SetCover(std::string name = "");
        bool IsFocus();
        void Reset();
        void Click();
        void SetFrontCard();
    };

}  // namespace ClassicLauncher

#endif  // GUI_CARD_H