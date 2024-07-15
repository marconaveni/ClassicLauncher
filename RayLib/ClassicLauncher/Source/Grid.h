#pragma once

#include <memory>
#include <vector>

#include "Animation.h"
#include "Card.h"
#include "Object.h"


enum Direction { None, Left, Right };

struct CardContainer
{
    std::shared_ptr<Card> cardMain;
    std::shared_ptr<Card> cardSelected;
    std::shared_ptr<Card> cardFavorite;
    int id = 0;
    Vector2 currentPosition;

    CardContainer() = default;

    void CreateCard(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture, int id);

    void RegisterCard() const;

    void AddPosition(Vector2 newPosition) ;

    void ChangePosition(const Position& newPosition) ;

    void Tick();

    void StartAnimationFocus();

    void StartAnimationLostFocus();

    void StartAnimationClick();

    Animation animationFocus;

    Animation animationLostFocus;

    Animation animationClick;

    bool bFocus;
};

class Grid : public Object
{

    std::vector<CardContainer> cardsContainer;
    int cardPositions[10] { -632 , -376 , -120 , 136 , 392 , 648, 904, 1160, 1416, 1672 };
    int positionX;
    bool bLeft;
    bool bRight;
    Direction LastDirection;
    int idFocus;

public:

    Animation animation;
    Animation animation2;

    Grid(Texture2D* textureReference, Vector2 position, Rectangle rectangleTexture);
    ~Grid() override;
    void BeginPlay() override;
    void Tick() override;
    void Draw() override;
    void EndDraw() override;
    void EndPlay() override;
    void Collision() override;
    void RegisterCards() const;

};
