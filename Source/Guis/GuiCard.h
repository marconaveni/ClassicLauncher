#ifndef GUI_CARD_H
#define GUI_CARD_H

#include <memory>
#include <string>
#include "GuiComponent.h"
#include "Core.h"

namespace ClassicLauncher
{
    class GuiComponent;

    class GuiCard : public GuiComponent
    {
    private:

        float mDefaultCoverWidth = 204.0f;
        float mDefaultCoverHeight = 204.0f;
        Vector2 mContainerSize = { 252.0f, 228.0f };

        TimerHandling mTimer;

        GuiComponent* mCardMain;
        GuiComponent* mCardSelected;
        GuiComponent* mCardFavorite;
        GuiComponent* mCover;
        bool mIsFocus = false;
        bool mIsFront = false;
        void CreateCard(GuiComponent*& card, float sourceX, float sourceY, unsigned char alpha, const char* title);


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