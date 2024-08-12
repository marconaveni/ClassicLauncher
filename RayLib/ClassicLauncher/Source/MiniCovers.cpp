#include "MiniCovers.h"

#include "GameListManager.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "Types.h"
#include "UtilsFunctionLibrary.h"

MiniCovers::MiniCovers()
	:positionY(505.0f)
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

	arrow = std::make_shared<ObjectAnimated>(TextureManager::GetInstance()->GetSprite("sprite"), Vector2{ 0   ,0 }, Rectangle{ 0,0,30,18 });
}

void MiniCovers::RegisterCovers() const
{
	for (auto& cover : objectsCover)
	{
		ObjectManager::GetInstance()->RegisterObject(cover);
	}
	ObjectManager::GetInstance()->RegisterObject(arrow);
}

void MiniCovers::SetCovers() const
{
	GameListManager* manager = GameListManager::GetInstance();

	const int size = manager->GetGameListSize() < 30 ? manager->GetGameListSize() : 30;

	if(size == 0)
	{
		for (int i = 0; i < 30; i++)
		{
			objectsCover[i]->SetTexture(nullptr, Rectangle{ 0,0, 0,0 });
		}
	}


	for (int i = 0; i < size; i++)
	{
		int indexFinal = UtilsFunctionLibrary::SetIndexArray(manager->GetId() + i - static_cast<int>(std::round(size / 2)), manager->GetGameListSize());
		indexFinal = Math::Clamp(indexFinal, 0, manager->GetGameListSize() - 1);
		if (size > 0)
		{
			Texture2D* texture = &manager->GetCurrentGameList(indexFinal)->textureMini;
			objectsCover[i]->SetTexture(texture, Rectangle{ 0,0, static_cast<float>(texture->width), static_cast<float>(texture->height) });
		}
	}

	SetPosition(size);
}

void MiniCovers::SetPosition(const int size) const
{
	const int x = (1280 - (29 * size)) / 2;
	for (int i = 0; i < 30; i++)
	{
		objectsCover[i]->position.position = Vector2{ static_cast<float>(x + (29 * i)), positionY + 21 };
	}

	const float arrowPositionX = (size % 2 == 0) ? 640.0f : 625.0f;
	arrow->position.position = Vector2{ arrowPositionX , positionY };
}

void MiniCovers::BeginPlay()
{
	Object::BeginPlay();

	const std::vector<Vector2> frames = {
		Vector2{ 798,1017 },
		Vector2{ 834,1017 },
		Vector2{ 870,1017 }
	};

	arrow->AddAnimation("frame", frames);
	arrow->SetAnimationSpeed(0.1f);
	arrow->ChangeAnimation("frame");
}

void MiniCovers::Tick()
{
	Object::Tick();
	SetCovers();


}
