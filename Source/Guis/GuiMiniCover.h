#ifndef GUI_MINI_COVER_H
#define GUI_MINI_COVER_H


#include <string>
#include <vector>

#include "Entity/Entity.h"
#include "Animations/Animatable.h"

namespace ClassicLauncher
{

    class GuiBase;
    class GuiSizeBox;
    class GuiHorizontalBox;
    class EntityManager;
    class GameListManager;

    class GuiMiniCover : public Entity, public Animatable
    {
    public:

        GuiMiniCover(GameListManager* gameListManagerRef);
        EntityType GetType() const override { return EntityType::GuiMiniCoverClass; }
        void Init();
        void Update() override;
        void End() override;
        void SetCovers();
        void SetCover(const std::string& name, GuiBase* miniCover);
        void ClearCovers();

    private:

        struct MiniCover
        {
            GuiBase* gui = nullptr;
            bool focus = false;
        };
        

        std::vector<MiniCover> m_guiMiniCovers;
        GuiHorizontalBox* m_guiHorizontalBox;
        GuiBase* m_arrow;
        int m_size;
        Vector2f m_sizeCover;
        bool m_focus = false;

        GameListManager* m_gameListManagerRef;

        void SetPositionCovers(int numCovers);
    };

} // namespace ClassicLauncher

#endif // GUI_MINI_COVER_H