#ifndef GUI_HORIZONTAL_CARDS_H
#define GUI_HORIZONTAL_CARDS_H

#include <vector>
#include "Data/GameListManager.h"
#include "Entity/EntityGui.h"
#include "Utils/TimerManager.h"


namespace ClassicLauncher
{

    class EntityGui;
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

    class GuiHorizontalCards : public EntityGui
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
        virtual void SetThemeValue() override;

    private:

        GuiTextBlock* mGuiTitle;
        GuiMiniCover* mMiniCover;
        GuiHorizontalBox* mHorizontalBox;
        GuiFrame* mFrame;
        std::vector<GuiCard*> mGuiCards;
        // float mCardPositions[10]{ -632 - 6, -376 - 6, -120 - 6, 136 - 6, 392 - 6, 648 - 6, 904 - 6, 1160 - 6, 1416 - 6, 1672 - 6 };
        float mPositionX;
        bool mIsLeft;
        bool mIsRight;
        bool mIsNeedUpdate;
        Direction mLastDirection;
        int mIdFocus;
        int mIdLastFocusSystem;
        float mSpeed;
        TimerHandling mTimerInputSpeed;
        
        GameListManager* m_gameListManagerRef;
        AudioManager* m_audioManagerRef;


        void UpdateCards();
        void SetPositionHorizontalBox();
        void ClearCovers();
    };

}  // namespace ClassicLauncher

#endif  // GUI_HORIZONTAL_CARDS_H