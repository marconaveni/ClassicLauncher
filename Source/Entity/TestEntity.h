#ifndef TEST_ENTITY_H
#define TEST_ENTITY_H

#include "Guis/GuiComponent.h"
#include "Animations/Animation.h"
#include "Window/RayWindow.h"
#include "rl_wrap.h"
#include "raylib.h"
#include <memory>

namespace ClassicLauncher
{
    class TestEntity : public Entity
    {
    public:
        TestEntity()
        {
            // cria filho já na lista de entidades filhos
            m_child = std::make_unique<GuiComponent>();
            AddChild(m_child.get());

            // define transform inicial do pai
            m_transform.position = {200, 150};
            m_transform.scale = {1.0f, 1.0f};
            m_transform.origin = {200, 150}; // centraliza em si mesmo
            m_transform.color = Color(200, 200, 200, 255); // cinza

            // define transform inicial do filho
            m_child->m_transform.position = {300, 10};
            m_child->m_transform.scale = {0.5f, 0.5f};
            m_child->m_transform.color = Color(0, 121, 241, 255); // azul
        }

        ~TestEntity() = default;

        EntityType GetType() const override { return EntityType::GuiWindowClass; };

        void InitAnim()
        {
            Transform start = m_transform;
            Transform end = m_transform;
            end.scale = {1.8f, 1.8f}; // anima para 1.8x

            m_anim.StartAnimation(4.5f, start, end, Ease::EaseSineInOut, true);
        }

        void Update() override
        {
            m_anim.UpdateAnimation();
            m_transform = m_anim.mCurrentTransform;
            Entity::UpdateWorldTransform();
        }

        void Draw() override
        {
            // desenha pai
            ::Rectangle rectParent{m_worldTransform.position.x, m_worldTransform.position.y,
                                 400 * m_worldTransform.scale.x, 300 * m_worldTransform.scale.y};
            ::DrawRectangleRec(rectParent, {200,200,200,255});

            // desenha filho
            ::Rectangle rectChild{m_child->m_worldTransform.position.x, m_child->m_worldTransform.position.y,
                                200 * m_child->m_worldTransform.scale.x, 150 * m_child->m_worldTransform.scale.y};
            ::DrawRectangleRec(rectChild, {0,121,241,255});
        }

    private:
        std::unique_ptr<GuiComponent> m_child;
        Animation m_anim;
    };
}

#endif // TEST_ENTITY_H
