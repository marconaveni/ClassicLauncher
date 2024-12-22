#ifndef GUI_CARD_H
#define GUI_CARD_H

#include <memory>
#include <string>
#include "GuiComponent.h"
#include "raylib.h"

namespace ClassicLauncher
{
    class GuiComponent;

    class GuiCard : public GuiComponent
    {
    private:

        std::shared_ptr<GuiComponent> mCardMain;
        std::shared_ptr<GuiComponent> mCardSelected;
        std::shared_ptr<GuiComponent> mCardFavorite;
        std::shared_ptr<GuiComponent> mCover;
        bool bFocus;
        void CreateCard(std::shared_ptr<GuiComponent>& card, int sourceX, int sourceY, unsigned char alpha);

    public:

        GuiCard(int x, int y);
        virtual ~GuiCard() override = default;
        virtual EntityType GetType() const override { return EntityType::GuiCardClass; }
        void Update() override;
        void SetFocus();
        void RemoveFocus();
        void SetCover(std::string name = "");
        bool IsFocus();
        virtual void AnimationStarted(std::string name) override;
        virtual void AnimationUpdate(std::string name) override;
        virtual void AnimationFinished(std::string name) override;
    };

}  // namespace ClassicLauncher

#endif  // GUI_CARD_H