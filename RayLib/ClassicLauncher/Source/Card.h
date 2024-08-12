#pragma once

#include <memory>

#include "Animation.h"
#include "Object.h"

class Card
{
public:
    std::shared_ptr<Object> cardMain;
    std::shared_ptr<Object> cardSelected;
    std::shared_ptr<Object> cardFavorite;
    std::shared_ptr<Object> cover;
    int id = 0;
    int idList = 0;
    Position currentPosition;
    Vector2 offSetCover;

    Card() = default;

    void CreateCard(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture, int idCard);

    void ResetCover();

    void RegisterCard() const;

    void AddPosition(Vector2 newPosition);

    void ChangePosition(const Position& newPosition);

    void ChangePosition(const Vector2& newPosition);

    void Tick();

    void StartAnimationFocus();

    void StartAnimationLostFocus();

    void StartAnimationClick();

    void SetCover(Texture2D* textureReference = nullptr);

    Animation animationFocus;

    Animation animationLostFocus;

    Animation animationClick;

    bool bFocus;
};