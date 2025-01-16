#ifndef GUI_HORIZONTAL_BOX_H
#define GUI_HORIZONTAL_BOX_H

#include <memory>
#include <vector>
#include <string>

#include "Application.h"
#include "GuiCard.h"
#include "GuiComponent.h"
#include "Guis/GuiTextBox.h"

namespace ClassicLauncher
{

    class GuiComponent;
    class GuiMiniCover;
    class Application;
    class GuiCard;
    class GuiTextBox;

    enum Direction
    {
        None,
        Left,
        Right
    };


    class GuiHorizontalBox : public GuiComponent
    {
        Application* mApplication;
        std::shared_ptr<GuiTextBox> mGuiTitle;
        std::shared_ptr<GuiMiniCover> mMiniCover;
        std::vector<std::shared_ptr<GuiCard>> mGuiCards;
        float mCardPositions[10]{ -632, -376, -120, 136, 392, 648, 904, 1160, 1416, 1672 };
        int mPositionX;
        bool mIsLeft;
        bool mIsRight;
        Direction mLastDirection;
        int mIdFocus;
        int mIdLastFocusSystem;
        float mSpeed;

    public:

        GuiHorizontalBox();
        void Init();
        void Update() override;
        void Draw() override;
        void End() override;
        void SetFocus(int newId, bool bForce = false);
        void SetCovers();
        EntityType GetType() const override { return EntityType::GuiHorizontalBoxClass; }
        void ChangeList(const CurrentList list);
        void Click();

    private:

        void ClearCovers();
    };

}  // namespace ClassicLauncher

#endif  // GUI_HORIZONTAL_BOX_H