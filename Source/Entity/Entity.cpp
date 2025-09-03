#include "Entity.h"

namespace ClassicLauncher
{

    void Transformation::SetTransforms(const float m)
    {
        // float _rootX = rootX * m;
        // float _rootY = rootY * m;
        float _x = ((x + rootScaleX * offset.x) * m) + (rootX * m);
        float _y = ((y + rootScaleY * offset.y) * m) + (rootY * m);
        float _width = width * m;
        float _height = height * m;

        float _sourceX = sourceX * m;
        float _sourceY = sourceY * m;
        float _scaleWidth = scaleWidth * m;
        float _scaleHeight = scaleHeight * m;

        // _x = _rootX + _x;
        // _y = _rootY + _y;
        // _width = _width;
        // _height = _height;
        // _sourceX = _sourceX;
        // _sourceY = _sourceY;
        _scaleWidth = _scaleWidth > 0.0f ? _scaleWidth : _width;
        _scaleHeight = _scaleHeight > 0.0f ? _scaleHeight : _height;

        mScale.Set(scaleX * rootScaleX, scaleY * rootScaleY);
        mSource.Set(_sourceX, _sourceY, _width, _height);
        mTransform.Set(_x, _y, mScale.x * _scaleWidth, mScale.y * _scaleHeight);
    }

    void Transformation::UpdateTransform(Transformation& otherTransform)
    {
        otherTransform.rootX = x + rootX;
        otherTransform.rootY = y + rootY;
        otherTransform.rootScaleX = scaleX * rootScaleX;
        otherTransform.rootScaleY = scaleY * rootScaleY;
        otherTransform.color.a = color.a <= otherTransform.color.a ? color.a : otherTransform.color.a;
    }









    Entity::Entity()
        : mToDelete(false), mToDraw(true), mScissorMode(false), mVisible(true), mZOrder(0), mIdZOrder(0), mId(0), mChildEntities(), mNameId()
    {
    }

    void Entity::UpdatePosition()
    {
        for (auto& entity : mChildEntities)
        {
            // entity->mTransform.rootX = mTransform.x + mTransform.rootX;
            // entity->mTransform.rootY = mTransform.y + mTransform.rootY;
            // entity->mTransform.rootScaleX = mTransform.scaleX * mTransform.rootScaleX;
            // entity->mTransform.rootScaleY = mTransform.scaleY * mTransform.rootScaleY;
            // entity->mTransform.color.a = mTransform.color.a <= entity->mTransform.color.a ? mTransform.color.a : entity->mTransform.color.a;
            mTransform.UpdateTransform(entity->mTransform);
            entity->UpdatePosition();
        }
    }

    void Entity::SelfDelete()
    {
        mToDelete = true;
        for (auto& entity : mChildEntities)
        {
            entity->SelfDelete();
            Entity* e = GetRootEntity();
            if (e != this)
            {
                e->RemoveChild(this);
            }
        }
    }

    void Entity::AddChild(Entity* childEntity)
    {
        if (childEntity->mParent != this)
        {
            childEntity->mParent = this;
            mChildEntities.emplace_back(childEntity);
        }
    }

    void Entity::RemoveChild(Entity* childEntity)
    {
        mChildEntities.erase(std::remove_if(mChildEntities.begin(),
                                            mChildEntities.end(),
                                            [childEntity](const Entity* entity)
                                            {
                                                return entity == childEntity;  // Return true element
                                            }),
                             mChildEntities.end());
    }

    void Entity::RemoveAllChilds()
    {
        mChildEntities.clear();
    }

    void Entity::RemoveRootChild()
    {
        Entity* entity = GetRootEntity();
        if (entity != this)
        {
            entity->RemoveChild(this);
        }
    }

    std::vector<Entity*>& Entity::GetChilds()
    {
        return mChildEntities;
    }

    Entity* Entity::GetRootEntity()
    {
        Entity* rootParent = mParent;
        if (rootParent != nullptr)
        {
            rootParent = rootParent->GetRootEntity();
        }
        return (rootParent != nullptr) ? rootParent : this;
    }

    void Entity::EnableScissorMode(float x, float y, float width, float height)
    {
        mScissorMode = true;
        mScissorArea.Set(x, y, width, height);
        for (auto& childEntity : mChildEntities)
        {
            childEntity->EnableScissorMode(mScissorArea.x, mScissorArea.y, mScissorArea.width, mScissorArea.height);
        }
    }

    void Entity::SetZOrder(int zOrder)
    {
        mZOrder = zOrder;
    }

}  // namespace ClassicLauncher
