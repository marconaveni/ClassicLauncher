#ifndef GUI_GRID_H
#define GUI_GRID_H

#include <vector>
#include <memory>

#include "GuiCard.h"
#include "GuiComponent.h"
#include "Application.h"

namespace ClassicLauncher
{

    class GuiComponent;
    class Application;
    class GuiCard;


    enum Direction { None, Left, Right };


    class GuiGrid : public GuiComponent
    {
        
        Application* app;
        std::vector<std::shared_ptr<GuiCard>> cardsContainer;
        float cardPositions[10] { -632 , -376 , -120 , 136 , 392 , 648, 904, 1160, 1416, 1672 };
        int positionX;
        bool bLeft;
        bool bRight;
        Direction lastDirection;
        int idFocus;
        unsigned int speed;

    public:

        GuiGrid();
        void Init();
        void Update() override;
        void Draw() override;
        void End() override;
        void SetFocus(int newId);
        void SetCovers();
        EntityType GetType() const override { return EntityType::GuiGridClass; }
    };

}

#endif // GUI_GRID_H