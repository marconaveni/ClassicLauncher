#ifndef GUI_HORIZONTAL_CARDS_H
#define GUI_HORIZONTAL_CARDS_H

#include <vector>

#include "Data/GameListManager.h"
#include "Guis/Components/GuiCanvas.h"
#include "Utils/TimerManager.h"


namespace ClassicLauncher
{

    class Animatable;
    class GuiMiniCover;
    class GuiCard;
    class GuiTextBlock;
    class GuiHorizontalBox;
    class GuiFrame;
    class EntityManager;
    class GameListManager;
    class AudioManager;

    enum Direction : std::uint8_t
    {
        None,
        Left,
        Right
    };

    class GuiHorizontalCards : public Entity
    {
    public:

        GuiHorizontalCards(GameListManager* gameListManagerRef, AudioManager* audioManagerRef);
        EntityType GetType() const override { return EntityType::GuiHorizontalCardsClass; }
        void Init();
        void Update() override;
        void Draw() override;
        void End() override;
        void SetFocus(int newId, bool bForce = false);
        void SetCovers();
        void ChangeList(CurrentList list);
        void Click();
        void SetHorizontalBoxValues();
        bool IsMovement() const;
        virtual void SetThemeValue() override;

    private:

        GuiTextBlock* m_guiTitle;
        GuiMiniCover* m_miniCover;
        GuiHorizontalBox* m_horizontalBox;
        GuiFrame* m_frame;
        std::vector<GuiCard*> m_guiCards;
        // float mCardPositions[10]{ -632 - 6, -376 - 6, -120 - 6, 136 - 6, 392 - 6, 648 - 6, 904 - 6, 1160 - 6, 1416 - 6, 1672 - 6 };
        float m_positionX;
        bool m_isLeft;
        bool m_isRight;
        bool m_isNeedUpdate;
        Direction m_lastDirection;
        int m_idFocus;
        int m_idLastFocusSystem;
        float mSpeed;
        float m_multiply = 22;
        bool m_isPress = false;
        TimerHandling m_timerInputSpeed{};

        GameListManager* m_gameListManagerRef;
        AudioManager* m_audioManagerRef;

        void CancelMultiply();
        void UpdateCards();
        void SetPositionHorizontalBox();
        void ClearCovers();
        void SetSpeedCards();
    };

} // namespace ClassicLauncher

#endif // GUI_HORIZONTAL_CARDS_H