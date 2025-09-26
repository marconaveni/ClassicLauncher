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

        struct ZOrder;

        Entity();
        virtual ~Entity() = default;
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
        [[nodiscard]] ZOrder GetZOrder() const { return m_zOrder; }
        const Transform& GetWorldTransform() const { return m_worldTransform; }

        Transform m_transform;
        Transform m_worldTransform;
        std::string mTextureName = "transparent";
        RectFloat mScissorArea;

        
        struct ZOrder
        {
            int id = 0;
            int insertionIndex = 0;
        };
        RectFloat m_finalTransformRect;

    protected:

        Entity* mParent = nullptr;
        std::vector<Entity*> mChildEntities;
        
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

        ZOrder m_zOrder {};
        std::string mNameId;

        SpriteManager* m_spriteManagerReference;
        TimerManager* m_timerManagerRef;
        EntityManager* m_entityManagerReference;
        FocusManager* m_focusManagerRef;

        // note: this should not be called directly use entity manager
        void SetZOrder(int zOrder); 
    };

} // namespace ClassicLauncher

#endif // ENTITY_H