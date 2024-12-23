#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include "Core.h"

namespace ClassicLauncher
{

    enum class EntityType
    {
        None = 0,
        GuiComponentClass = 1,
        GuiWindowClass = 2,
        GuiCardClass = 3,
        GuiHorizontalBoxClass = 4,
        GuiMiniCoverClass = 5,
    };

    class Entity
    {
    private:

        friend class EntityManager;

        bool bToDelete = false;
        bool bToDraw = true;
        bool bScissorMode = false;
        bool bVisible = true;
        bool bBringToFront = false;
        int mZOrder = 0;
        std::vector<Entity*> mChildEntities;
        std::string mNameId;

    protected:

        Entity* mParent = nullptr;

    public:

        Entity() = default;
        virtual ~Entity() = default;
        bool operator<(const Entity& entity) const { return mZOrder < entity.mZOrder; }
        bool operator>(const Entity& entity) const { return mZOrder > entity.mZOrder; }
        virtual EntityType GetType() const = 0;
        virtual void Update() {}
        virtual void Draw() {}
        virtual void End() {}
        virtual void UpdatePosition();               // Update the position of the entity
        virtual void SelfDelete();                   // Delete the entity and all its children
        virtual void AddChild(Entity* childEntity);  // Add a child to the entity
        virtual void RemoveChild();                  // Remove a child from the entity
        std::vector<Entity*>& GetChilds();
        Entity* GetRootEntity();
        void EnableScissorMode(float x, float y, float width, float height);
        void DisableScissorMode() { bScissorMode = false; }
        void SetVisible(bool bEnable) { bVisible = bEnable; }
        int GetZOrder() const { return mZOrder; }
        void SetBringToFront() { bBringToFront = true; }

        TransformProperties mProperties;
        std::string mTextureName = "transparent";
        Rectangle mScissorArea;

    private:

        void SetZOrder(int zOrder);
    };

}  // namespace ClassicLauncher

#endif  // ENTITY_H