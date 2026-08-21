#ifndef GUI_HORIZONTAL_CARDS_H
#define GUI_HORIZONTAL_CARDS_H

#include <vector>

#include "Animations/Animatable.h"
#include "Data/GameListManager.h"
#include "Guis/Components/GuiCanvas.h"
#include "Input/InputManager.h"
#include "Utils/TimerManager.h"

namespace ClassicLauncher
{

    class Animatable;
    class GuiMiniCover;
    class GuiCard;
    class GuiTextBlock;
    class GuiHorizontalBox;
    class GuiFrame;
    class GuiHintBar;
    class EntityManager;
    class GameListManager;
    class AudioManager;
    class GuiMenu;
    class GuiBase;
    class Window;


    class GuiHorizontalCards : public Entity, public Animatable
    {
    public:

        enum class Direction : std::uint8_t
        {
            None,
            Left,
            Right
        };

        explicit GuiHorizontalCards(const EntityContext& entityContext, GameListManager* gameListManagerRef, Window* window);
        EntityType GetType() const override { return EntityType::GuiHorizontalCardsClass; }
        void Init();
        void Update() override;
        void Draw() override;
        void End() override;
        void SetFocus(int newId, bool force = false);
        void Focus();
        void UpdateCovers();
        void RemoveCoversFromScreen();
        void ChangeList(CurrentList list);
        void Click();
        bool IsMovement() const;
        virtual void SetThemeValue() override;

    private:

        GuiTextBlock* m_guiTitle{nullptr};
        GuiMiniCover* m_miniCover{nullptr};
        GuiHorizontalBox* m_horizontalBox{nullptr};
        GuiFrame* m_frame{nullptr};
        GuiHintBar* m_hintBar{nullptr};
        GuiMenu* m_guiMenu{nullptr};
        GuiBase* m_guiMenuBackground{nullptr};
        GuiBase* m_guiTopBar{nullptr};
        GuiBase* m_guiBottomBar{nullptr};
        std::vector<GuiCard*> m_guiCards{};
        float m_positionX{0};
        bool m_isLeft{false};
        bool m_isRight{false};
        bool m_isNeedUpdate{false};
        Direction m_lastDirection{Direction::None};
        int m_idFocus{0};
        int m_idLastFocusSystem{3};
        float m_speed{22.0f};
        float m_multiply{22.0f};
        bool m_isPress{false};
        TimerHandling m_timerInputSpeed{};
        bool m_topBarAnimation{false};
        int m_loadTexturesCards{0};

        GameListManager* m_gameListManagerRef{nullptr};

        void CancelMultiply();
        void UpdateCards();
        void UpdateInput();
        void SetPositionHorizontalBox();
        void FocusAnimationBar(InputCategory category);
        void ClearCovers();
        void SetSpeedCards();
        void SetTextHintBar();
        void SetMenuBar();
    };

} // namespace ClassicLauncher

#endif // GUI_HORIZONTAL_CARDS_H