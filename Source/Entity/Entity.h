#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include "Core.h"
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
    };

    class Entity
    {
    private:

        friend class EntityManager;
        friend class RenderSystem;

        bool mToDelete;
        bool mToDraw;
        bool mScissorMode;
        bool mVisible;
        int mZOrder;
        int mIdZOrder;
        int mId;
        std::vector<Entity*> mChildEntities;
        std::string mNameId;

    protected:

        Entity* mParent = nullptr;

    public:

        Entity();
        virtual ~Entity() = default;
        bool operator<(const Entity& entity) const { return entity.mId < mId; }
        bool operator>(const Entity& entity) const { return mZOrder > entity.mZOrder; }
        virtual EntityType GetType() const = 0;
        virtual void Update() {}
        virtual void Draw() {}
        virtual void End() {}
        virtual void UpdatePosition();                  // Update the position of the entity
        virtual void SelfDelete();                      // Delete the entity and all its children
        virtual void AddChild(Entity* childEntity);     // Add a child to the entity
        virtual void RemoveChild(Entity* childEntity);  // Remove a child from the entity
        virtual void RemoveAllChilds();
        virtual void RemoveRootChild();
        std::vector<Entity*>& GetChilds();
        Entity* GetRootEntity();
        Entity* GetParent() { return mParent; }
        void RemoveParent() { mParent = nullptr; }
        void EnableScissorMode(float x, float y, float width, float height);
        void DisableScissorMode() { mScissorMode = false; }
        void SetVisible(bool bEnable) { mVisible = bEnable; }
        int GetZOrder() const { return mZOrder; }
        int GetIdZOrder() const { return mIdZOrder; }

        Transform mTransform;
        std::string mTextureName = "transparent";
        Rectangle mScissorArea;

    private:

        void SetZOrder(int zOrder);
    };

}  // namespace ClassicLauncher

#endif  // ENTITY_H