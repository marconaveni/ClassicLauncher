#ifndef GUI_MINI_COVER_H
#define GUI_MINI_COVER_H


#include <string>
#include <vector>

#include "Animations/Animatable.h"
#include "Entity/Entity.h"


namespace ClassicLauncher
{

    class GuiBase;
    class GuiSizeBox;
    class GuiHorizontalBox;
    class EntityManager;
    class GameListManager;
    class Window;

    class GuiMiniCover : public Entity, public Animatable
    {
    public:

        GuiMiniCover(GameListManager* gameListManagerRef, Window* window);
        EntityType GetType() const override { return EntityType::GuiMiniCoverClass; }
        void Init();
        void Update() override;
        void End() override;
        virtual void SetThemeValue() override;
        void SetCovers();
        void SetCover(const std::string& name, GuiBase* miniCover);
        void ClearCovers();

    private:

        struct MiniCover
        {
            GuiSizeBox* sizeBox{nullptr};
            GuiBase* gui{nullptr};
            bool focus{false};
        };


        std::vector<MiniCover> m_guiMiniCovers{};
        GuiHorizontalBox* m_guiHorizontalBox{nullptr};
        GuiBase* m_arrow{nullptr};
        int m_numCovers{32};
        Vector2f m_sizeCover{28.0f, 40.0f};
        bool m_focus{false};

        GameListManager* m_gameListManagerRef{nullptr};

        void SetPositionCovers(int numCovers);
        void CreateMiniCovers();
    };

} // namespace ClassicLauncher

#endif // GUI_MINI_COVER_H