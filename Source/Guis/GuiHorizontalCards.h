#ifndef GUI_HORIZONTAL_CARDS_H
#define GUI_HORIZONTAL_CARDS_H

#include <memory>
#include <vector>
#include <string>

#include "Application.h"
#include "GuiCard.h"
#include "GuiComponent.h"
#include "Guis/GuiTextBlock.h"

namespace ClassicLauncher
{

    class GuiComponent;
    class GuiMiniCover;
    class Application;
    class GuiCard;
    class GuiTextBlock;

    enum Direction
    {
        None,
        Left,
        Right
    };


    class GuiHorizontalCards : public GuiComponent
    {
        GuiTextBlock* mGuiTitle;
        GuiMiniCover* mMiniCover;
        std::vector<GuiCard*> mGuiCards;
        float mCardPositions[10]{ -632 - 6, -376 - 6, -120 - 6, 136 - 6, 392 - 6, 648 - 6, 904 - 6, 1160 - 6, 1416 - 6, 1672 - 6 };
        float mPositionX;
        bool mIsLeft;
        bool mIsRight;
        Direction mLastDirection;
        int mIdFocus;
        int mIdLastFocusSystem;
        float mSpeed;
        TimerHandling mTimerInputSpeed;

    public:

        GuiHorizontalCards();
        void Init();
        void Update() override;
        void Draw() override;
        void End() override;
        void SetFocus(int newId, bool bForce = false);
        void SetCovers();
        EntityType GetType() const override { return EntityType::GuiHorizontalCardsClass; }
        void ChangeList(const CurrentList list);
        void Click();

    private:

        void ClearCovers();
    };

}  // namespace ClassicLauncher

#endif  // GUI_HORIZONTAL_CARDS_H