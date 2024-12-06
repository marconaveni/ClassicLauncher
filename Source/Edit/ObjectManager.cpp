#include "ObjectManager.h"

#include <algorithm>
#include <random>

/* ObjectManager* ObjectManager::GetInstance()
{
    static ObjectManager object;
    return &object;
} */
namespace ClassicLauncher
{
	static ObjectManager* instanceObjectManager = nullptr;

	ObjectManager::ObjectManager()
	{
		if (instanceObjectManager == nullptr)
		{
			instanceObjectManager = this;
		}
	}

    ObjectManager::~ObjectManager()
    {
		instanceObjectManager = nullptr;
    }

    void ObjectManager::RegisterObject(std::shared_ptr<Object> object)
	{
		/* 	if (object->bRegisterObject)
			{
				return;
			}
			object->bRegisterObject = true; */
		// objects.push_back(object);
	}
	ObjectManager& ObjectManager::Get()
	{
		return *instanceObjectManager;
	}

	/* void ObjectManager::RegisterObject(const std::shared_ptr<Object>& object)
	{
		if (object->bRegisterObject)
		{
			return;
		}
		object->bRegisterObject = true;
		objects.push_back(object);
	} */

	/* void ObjectManager::UnRegisterObject()
	{
		for (auto& object : objects)
		{
			object->bRegisterObject = false;
		}
		objectsGet.clear();
		objectsGet.shrink_to_fit();
		objects.clear();
		objects.shrink_to_fit();
	} */

	/* std::vector<Object*>& ObjectManager::GetAllObjects()
	{
		if (objectsGet.size() != objects.size())
		{
			objectsGet.clear();
			objectsGet.shrink_to_fit();
			objectsGet.reserve(objects.size());
			std::transform(objects.cbegin(), objects.cend(), std::back_inserter(objectsGet),
				[](auto& ptr) { return ptr.get(); });
		}
		return objectsGet;
	} */

	void ObjectManager::BeginPlay() const
	{
		/* 	for (auto& objShared : objects)
			{
				objShared->BeginPlay();
				objShared->Collision();
				objShared->Tick();
			} */
	}

	void ObjectManager::Tick() const
	{
		/* 	for (auto& objShared : objects)
			{

				objShared->Collision();
				objShared->Tick();

			} */
	}

	void ObjectManager::Draw() const
	{
		/* 	std::vector<Object*> frontObjects;
			for (auto& objShared : objects)
			{
				if(objShared->bToFrontRender)
				{
					frontObjects.push_back(objShared.get());
					continue;
				}
				objShared->Draw();
			}
			for (auto& objShared : frontObjects)
			{
				objShared->bToFrontRender = false;
				objShared->Draw();
			} */
	}

	void ObjectManager::EndDraw() const
	{
		/* 	for (auto& objShared : objects)
			{
				objShared->EndDraw();
			} */
	}

	void ObjectManager::EndPlay() const
	{
		/* 	for (auto& objShared : objects)
			{
				objShared->EndPlay();
			} */
	}
} // namespace ClassicLauncher