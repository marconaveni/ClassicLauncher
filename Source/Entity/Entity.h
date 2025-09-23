#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "Data/Transform.h"

namespace ClassicLauncher
{

    enum class EntityType
    {
        GuiComponentClass = 1,
        GuiWindowClass,
        GuiCardClass,
        GuiHorizontalCardsClass,
        GuiMiniCoverClass,
        GuiVideoPlayerClass,
        GuiTextBlockClass,
        GuiSizeBoxClass,
        GuiHorizontalBoxClass,
        GuiBlackScreenClass,
        GuiFrameClass,
        GuiCanvasClass,
    };

    class SpriteManager;
    class TimerManager;
    class EntityManager;
    class FocusManager;

    class Entity
    {
    public:

        Entity();
        virtual ~Entity() = default;
        bool operator<(const Entity& entity) const { return entity.mId < mId; }
        bool operator>(const Entity& entity) const { return mZOrder > entity.mZOrder; }
        [[nodiscard]] virtual EntityType GetType() const = 0;
        virtual void Update() {}
        virtual void Draw() {}
        virtual void End() {}
        virtual void UpdateWorldTransform();           // Update the position of the entity
        virtual void SelfDelete();                     // Delete the entity and all its children
        virtual void AddChild(Entity* childEntity);    // Add a child to the entity
        virtual void RemoveChild(Entity* childEntity); // Remove a child from the entity
        virtual void RemoveAllChildren();
        virtual void RemoveRootChild();
        virtual void SetThemeValue() {};
        std::vector<Entity*>& GetChildren();
        Entity* GetRootEntity();
        [[nodiscard]] Entity* GetParent() const { return mParent; }
        void RemoveParent() { mParent = nullptr; }
        void EnableScissorMode(float x, float y, float width, float height);
        void DisableScissorMode() { mScissorMode = false; }
        void SetVisible(const bool bEnable) { mVisible = bEnable; }
        [[nodiscard]] int GetZOrder() const { return mZOrder; }
        [[nodiscard]] int GetIdZOrder() const { return mIdZOrder; }

        Transform m_transform;
        Transform m_worldTransform;
        std::string mTextureName = "transparent";
        RectFloat mScissorArea;

    protected:

        Entity* mParent = nullptr;
        RectFloat m_finalTransformRect;
        TimerManager* GetTimerManager() { return m_timerManagerRef; }
        SpriteManager* GetSpriteManager() { return m_spriteManagerReference; }
        EntityManager* GetEntityManager() { return m_entityManagerReference; }
        FocusManager* GetFocusManager() { return m_focusManagerRef; }

    private:

        friend class EntityManager;
        friend class RenderEntities;
        friend class FocusComponent;

        bool mToDelete;
        bool mToDraw;
        bool mScissorMode;
        bool mVisible;
        int mZOrder;
        int mIdZOrder;
        int mId;
        std::vector<Entity*> mChildEntities;
        std::string mNameId;

        SpriteManager* m_spriteManagerReference;
        TimerManager* m_timerManagerRef;
        EntityManager* m_entityManagerReference;
        FocusManager* m_focusManagerRef;

        void SetZOrder(int zOrder);
    };

} // namespace ClassicLauncher

#endif // ENTITY_H