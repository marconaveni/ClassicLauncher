#ifndef GUI_GRID_H
#define GUI_GRID_H

#include <memory>
#include <vector>

#include "Application.h"
#include "GuiCard.h"
#include "GuiComponent.h"

namespace ClassicLauncher
{

    class GuiComponent;
    class Application;
    class GuiCard;

    enum Direction
    {
        None,
        Left,
        Right
    };

    class GuiGrid : public GuiComponent
    {

        Application* mApplication;
        std::vector<std::shared_ptr<GuiCard>> mGuiCards;
        float mCardPositions[10]{ -632, -376, -120, 136, 392, 648, 904, 1160, 1416, 1672 };
        int mPositionX;
        bool bLeft;
        bool bRight;
        Direction mLastDirection;
        int mIdFocus;
        unsigned int mSpeed;

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

}  // namespace ClassicLauncher

#endif  // GUI_GRID_H