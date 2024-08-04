#include "MiniCovers.h"

#include "GameListManager.h"
#include "ObjectManager.h"
#include "UtilsFunctionLibrary.h"

MiniCovers::MiniCovers()
{
	CreateCovers();
}

void MiniCovers::CreateCovers()
{
	for (int i = 0; i < 30; i++)
	{
		auto cover = std::make_shared<Object>(nullptr, Vector2{ static_cast<float>(205 + (29 * i))   ,530 }, Rectangle{ 0,0,28,40 });
		objectsCover.push_back(cover);
	}
}

void MiniCovers::RegisterCovers() const
{
	for (auto& cover : objectsCover)
	{
		ObjectManager::GetInstance()->RegisterObject(cover);
	}
}

void MiniCovers::SetCovers() const
{
	GameListManager* manager = GameListManager::GetInstance();

	const int size = manager->GetGameListSize() < 30 ? manager->GetGameListSize() : 30;
	for (int i = 0; i < size; i++)
	{
		int indexFinal = UtilsFunctionLibrary::SetIndexArray(manager->GetId() + i - (std::round(size / 2)), manager->GetGameListSize());
		indexFinal = Math::Clamp(indexFinal, 0, manager->GetGameListSize() - 1);
		if (!manager->GetAllGameList().empty())
		{
			Texture2D* texture = &manager->GetAllGameList()[indexFinal]->textureMini;
			objectsCover[i]->SetTexture(texture, Rectangle{ 0,0, (float)texture->width, (float)texture->height });
		}
	}

	SetPosition(size);
}

void MiniCovers::SetPosition(const int size) const
{
	const int x = (1280 - (29 * size)) / 2;
	for (int i = 0; i < 30; i++)
	{
		objectsCover[i]->position.position = Vector2{ static_cast<float>(x + (29 * i)),500 };
	}
}

void MiniCovers::BeginPlay()
{
	Object::BeginPlay();
}

void MiniCovers::Tick()
{
	Object::Tick();
	SetCovers();
}
