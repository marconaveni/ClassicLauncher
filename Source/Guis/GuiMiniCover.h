#ifndef GUI_MINI_COVER_H
#define GUI_MINI_COVER_H

#include <memory>
#include <string>
#include <vector>

#include "Application.h"
#include "GuiComponent.h"
#include "Guis/GuiSizeBox.h"
#include "Guis/GuiHorizontalBox.h"

namespace ClassicLauncher
{

    class EntityGui;
    class GuiComponent;
    class GuiSizeBox;
    class GuiHorizontalBox;

    class GuiMiniCover : public EntityGui
    {
    private:

        std::vector<GuiComponent*> mGuiCovers;
        std::vector<GuiSizeBox*> mGuiSizeBoxs;
        GuiHorizontalBox* mGuiHorizontalBox;
        GuiComponent* mArrow;
        int mSize;
        Vector2 mSizeCover;

    public:

        GuiMiniCover();
        virtual ~GuiMiniCover() override = default;
        EntityType GetType() const override { return EntityType::GuiMiniCoverClass; }
        void Init();
        void Update() override;
        void End() override;
        void SetCovers();
        void SetCover(std::string name, GuiComponent* miniCover);
        void ClearCovers();

    private:

        void SetPositionCovers(int numCovers);
    };

}  // namespace ClassicLauncher

#endif  // GUI_MINI_COVER_H