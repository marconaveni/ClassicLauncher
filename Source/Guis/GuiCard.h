#ifndef GUI_CARD_H
#define GUI_CARD_H

#include "GuiComponent.h"
#include "raylib.h"
#include <memory>
#include <string>

namespace ClassicLauncher
{
    class GuiComponent;
    
    class GuiCard : public GuiComponent
    {
    private:

        std::shared_ptr<GuiComponent> cardMain;
        std::shared_ptr<GuiComponent> cardSelected;
        std::shared_ptr<GuiComponent> cardFavorite;
        std::shared_ptr<GuiComponent> cover;
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

    };
    

}

#endif // GUI_CARD_H