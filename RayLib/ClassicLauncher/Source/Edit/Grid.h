#pragma once

#include <vector>

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

    Grid(std::shared_ptr<ObjectManager> objectManager);
    void BeginPlay() override;
    void Tick() override;
    void Draw() override;
    void EndDraw() override;
    void EndPlay() override;
    void Collision() override;
    void RegisterCards() const;
    void SetFocus(int newId);
    void SetCovers();
    void ClearCovers();

};
