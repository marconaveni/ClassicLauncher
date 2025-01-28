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
        GuiVideoPlayer = 6,
        GuiTextBox = 7,
    };

    class Entity
    {
    private:

        friend class EntityManager;

        bool mToDelete = false;
        bool mToDraw = true;
        bool mScissorMode = false;
        bool mVisible = true;
        bool mBringToFront = false;
        int mZOrder = 0;
        int mIdZOrder = 0;
        int mId = 0;
        std::vector<Entity*> mChildEntities;
        std::string mNameId;

    protected:

        Entity* mParent = nullptr;

    public:

        Entity() = default;
        virtual ~Entity() = default;
        bool operator<(const Entity& entity) const { return entity.mId < mId; }
        bool operator>(const Entity& entity) const { return mZOrder > entity.mZOrder; }
        virtual EntityType GetType() const = 0;
        virtual void Update() {}
        virtual void Draw() {}
        virtual void End() {}
        virtual void UpdatePosition();               // Update the position of the entity
        virtual void SelfDelete();                   // Delete the entity and all its children
        virtual void AddChild(Entity* childEntity);  // Add a child to the entity
        virtual void RemoveChild(Entity* childEntity);                 // Remove a child from the entity
        virtual void RemoveAllChilds(); 
        std::vector<Entity*>& GetChilds();
        Entity* GetRootEntity();
        void EnableScissorMode(float x, float y, float width, float height);
        void DisableScissorMode() { mScissorMode = false; }
        void SetVisible(bool bEnable) { mVisible = bEnable; }
        int GetZOrder() const { return mZOrder; }
        int GetIdZOrder() const { return mIdZOrder; }
        void SetBringToFront() { mBringToFront = true; }

        TransformProperties mProperties;
        std::string mTextureName = "transparent";
        Rectangle mScissorArea;

    private:

        void SetZOrder(int zOrder);
    };

}  // namespace ClassicLauncher

#endif  // ENTITY_H