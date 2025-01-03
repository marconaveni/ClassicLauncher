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
        float mContainerSize = 240.0f;

        Timer<GuiCard>* mTimer;

        std::shared_ptr<GuiComponent> mCardMain;
        std::shared_ptr<GuiComponent> mCardSelected;
        std::shared_ptr<GuiComponent> mCardFavorite;
        std::shared_ptr<GuiComponent> mCover;
        bool mIsFocus = false;
        bool mIsFront = false;
        void CreateCard(std::shared_ptr<GuiComponent>& card, int sourceX, int sourceY, unsigned char alpha);

    public:

        GuiCard(int x, int y);
        virtual ~GuiCard() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiCardClass; }
        void Update() override;
        void SetFocus(bool bForce = false);
        void RemoveFocus(bool bForce = false);
        void SetCover(std::string name = "");
        bool IsFocus();
        void Reset();
        void Click();
    };

}  // namespace ClassicLauncher

#endif  // GUI_CARD_H