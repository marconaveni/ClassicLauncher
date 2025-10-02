#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

#include "Data/Transform.h"

namespace ClassicLauncher
{

    enum class EntityType
    {
        GuiBaseClass = 1,
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
        [[nodiscard]] Entity* GetParent() const { return m_parent; }
        void RemoveParent() { m_parent = nullptr; }
        void EnableScissorMode(float x, float y, float width, float height);
        void DisableScissorMode() { m_isScissorMode = false; }
        void SetVisible(const bool bEnable) { m_isVisible = bEnable; }
        [[nodiscard]] ZOrder GetZOrder() const { return m_zOrder; }
        [[nodiscard]] bool GetTransformIsDirty() const { return m_isTransformDirty; }
        void MarkTransformAsDirty();
        

        // clang-format off
        // Gettter and Setters Transforms

        void SetPosition(float x, float y);
        inline void SetPosition(Vector2f position) { SetPosition(position.x, position.y); };
        Vector2f GetPosition() const;

        void SetSize(float width, float height);  // note: Size is m_transform.position.width and height 
        inline void SetSize(Vector2f size) { SetSize(size.x, size.y); };
        Sizef GetSize() const;
       
        void SetSource(float x, float y, float width, float height);
        inline void SetSource(Vector2f position, Sizef size) { SetSource(position.x, position.y, size.width, size.height); };
        inline void SetSource(RectFloat source) { SetSource(source.x, source.y, source.width, source.height); };
        RectFloat GetSource() const;

        void SetOffset(float x, float y);
        inline void SetOffset(Vector2f offset) { SetOffset(offset.x, offset.y); };
        Vector2f GetOffset() const;
        
        void SetOrigin(float x, float y);
        inline void SetOrigin(Vector2f origin) { SetOrigin(origin.x, origin.y); };
        Vector2f GetOrigin() const;
        
        void SetScale(float x, float y);
        inline void SetScale(Vector2f scale) { SetScale(scale.x, scale.y); };
        Vector2f GetScale() const;

        void SetRotation(float rotation);
        float GetRotation() const;
            
        void SetColor(float r, float g, float b);
        void SetColor(float r, float g, float b, float a);
        void SetColor(Color color);
        void SetColorRed(float r);
        void SetColorGreen(float g);
        void SetColorBlue(float b);
        void SetOpacity(float a);
        Color GetColor() const;

        Transform& GetTransformRef();
        Transform& GetWorldTransformRef();
        const Transform& GetTransform() const { return m_transform; }
        const Transform& GetWorldTransform() const { return m_worldTransform; }

        // End Getters and Setters
        // clang-format on

        
        RectFloat m_scissorArea;
        std::string m_textureName = "transparent";


        struct ZOrder
        {
            int id = 0;
            int insertionIndex = 0;
        };

        struct FinalRenderTransform
        {
            RectFloat transform{};
            RectFloat source{};
            Vector2f origin{};
        };

        FinalRenderTransform m_finalRender{};
        
        //RectFloat m_finalTransformRect;

    protected:

        Entity* m_parent = nullptr;
        std::vector<Entity*> m_childEntities;

        TimerManager* GetTimerManager() { return m_timerManagerRef; }
        SpriteManager* GetSpriteManager() { return m_spriteManagerReference; }
        EntityManager* GetEntityManager() { return m_entityManagerReference; }
        FocusManager* GetFocusManager() { return m_focusManagerRef; }

    private:

        friend class EntityManager;
        friend class RenderEntities;
        friend class FocusComponent;

        Transform m_transform;
        Transform m_worldTransform;

        bool m_isCanDelete;
        bool m_isCanDraw;
        bool m_isScissorMode;
        bool m_isVisible;
        bool m_isTransformDirty{true};

        ZOrder m_zOrder{};
        std::string m_nameId;

        SpriteManager* m_spriteManagerReference;
        TimerManager* m_timerManagerRef;
        EntityManager* m_entityManagerReference;
        FocusManager* m_focusManagerRef;

        // note: this should not be called directly use entity manager
        void SetZOrder(int zOrder);
    };

} // namespace ClassicLauncher

#endif // ENTITY_H