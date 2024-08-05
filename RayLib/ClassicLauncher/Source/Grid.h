#pragma once

#include <memory>
#include <vector>

#include "Animation.h"
#include "Card.h"
#include "Object.h"


enum Direction { None, Left, Right };


class Grid final : public Object
{

    std::vector<Card> cardsContainer;
    float cardPositions[10] { -632 , -376 , -120 , 136 , 392 , 648, 904, 1160, 1416, 1672 };
    int positionX;
    bool bLeft;
    bool bRight;
    Direction lastDirection;
    int idFocus;

public:

    Animation animation;
    Animation animation2;

    Grid(Texture2D* textureReference, Vector2 positionGrid, Rectangle rectangleTexture);
    void BeginPlay() override;
    void Tick() override;
    void Draw() override;
    void EndDraw() override;
    void EndPlay() override;
    void Collision() override;
    void RegisterCards() const;
    void SetFocus(int newId);
    void SetCovers();

};
