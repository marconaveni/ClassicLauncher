#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include "Data/Color.h"
#include "Data/Rectangle.h"

namespace ClassicLauncher
{

    class Transformation
    {
    private:

        friend class RenderSystem;

        RectFloat mTransform;
        RectFloat mSource;
        Vector2f mScale;

        float rootX;
        float rootY;
        float rootScaleX;
        float rootScaleY;

        void SetTransforms(const float m);

    public:

        Vector2f offset;
        float x;
        float y;
        float width;
        float height;
        float scaleX;
        float scaleY;
        float rotation;
        Color color;

        float sourceX;
        float sourceY;
        float scaleWidth;
        float scaleHeight;

        Transformation()
            : rootX(0)
            , rootY(0)
            , rootScaleX(1)
            , rootScaleY(1)
            , offset()
            , x(0)
            , y(0)
            , width(0)
            , height(0)
            , scaleX(1)
            , scaleY(1)
            , rotation(0)
            , color({ 255, 255, 255, 255 })
            , sourceX(0)
            , sourceY(0)
            , scaleWidth(0)
            , scaleHeight(0)
            , mScale{1.0f, 1.0f}
        {
        }

        RectFloat GetTransform() { return mTransform; }
        RectFloat GetSource() { return mSource; }
        Vector2f GetScale() { return mScale; }
        Vector2f GetRootPosition() { return Vector2f(rootX, rootY); }
        Vector2f GetRootScale() { return Vector2f(rootScaleX, rootScaleY); }
        void UpdateTransform(Transformation& otherTransform);
    };

    // class Transformation;

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
        virtual void SetThemeValue() {};
        std::vector<Entity*>& GetChilds();
        Entity* GetRootEntity();
        Entity* GetParent() { return mParent; }
        void RemoveParent() { mParent = nullptr; }
        void EnableScissorMode(float x, float y, float width, float height);
        void DisableScissorMode() { mScissorMode = false; }
        void SetVisible(bool bEnable) { mVisible = bEnable; }
        int GetZOrder() const { return mZOrder; }
        int GetIdZOrder() const { return mIdZOrder; }

        Transformation mTransform;
        std::string mTextureName = "transparent";
        RectFloat mScissorArea;

    private:

        void SetZOrder(int zOrder);
    };

}  // namespace ClassicLauncher

#endif  // ENTITY_H