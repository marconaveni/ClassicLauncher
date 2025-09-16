#ifndef GUI_MINI_COVER_H
#define GUI_MINI_COVER_H


#include <string>
#include <vector>
#include "Entity/EntityGui.h"

namespace ClassicLauncher
{

    class EntityGui;
    class GuiComponent;
    class GuiSizeBox;
    class GuiHorizontalBox;
    class EntityManager;

    class GuiMiniCover : public EntityGui
    {
    public:

        GuiMiniCover();
        EntityType GetType() const override { return EntityType::GuiMiniCoverClass; }
        void Init();
        void Update() override;
        void End() override;
        void SetCovers();
        static void SetCover(const std::string& name, GuiComponent* miniCover);
        void ClearCovers();

    private:

        std::vector<GuiComponent*> mGuiCovers;
        std::vector<GuiSizeBox*> mGuiSizeBoxs;
        GuiHorizontalBox* mGuiHorizontalBox;
        GuiComponent* mArrow;
        int mSize;
        Vector2f mSizeCover;


        void SetPositionCovers(int numCovers);
    };

}  // namespace ClassicLauncher

#endif  // GUI_MINI_COVER_H